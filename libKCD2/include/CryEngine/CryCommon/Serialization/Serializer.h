// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once

#include <vector>
#include "Serialization/Assert.h"
#include "Serialization/TypeID.h"

namespace Serialization{

class IArchive;

typedef bool(*SerializeStructFunc)(void*, IArchive&);

typedef bool(*SerializeContainerFunc)(void*, IArchive&, size_t index);
typedef size_t(*ContainerResizeFunc)(void*, size_t size);
typedef size_t(*ContainerSizeFunc)(void*);

// Struct serializer. 
struct SStruct{/*{{{*/
	friend class IArchive;
public:
	SStruct()
	: object_(0)
	, size_(0)
	, serializeFunc_(0)
	{
	}

	SStruct(TypeID type, void* object, size_t size, SerializeStructFunc Serialize)
	: type_(type)
	, object_(object)
	, size_(size)
	, serializeFunc_(Serialize)
	{
		YASLI_ASSERT(object != 0);
	}

	SStruct(const SStruct& _original)
	: type_(_original.type_)
	, object_(_original.object_)
	, size_(_original.size_)
	, serializeFunc_(_original.serializeFunc_)
	{
	}

	template<class T>
	explicit SStruct(T& object){
		type_=  TypeID::get<T>();
		object_ = (void*)(&object);
		size_ = sizeof(T);
		serializeFunc_ = &SStruct::serializeRaw<T>;
	}

	template<class T>
	explicit SStruct(T& object, TypeID type){
		type_ =  type;
		object_ = (void*)(&object);
		size_ = sizeof(T);
		serializeFunc_ = &SStruct::serializeRaw<T>;
	}

	bool operator()(IArchive& ar, const char* name, const char* label) const;
	bool operator()(IArchive& ar) const;
	operator bool() const{ return object_ && serializeFunc_; }
	void* pointer() const{ return object_; }
	TypeID type() const{ return type_; }
	size_t size() const{ return size_; }

	template<class T>
	static bool serializeRaw(void* rawPointer, IArchive& ar){
		YASLI_ESCAPE(rawPointer, return false);
		((T*)(rawPointer))->Serialize(ar);
		return true;
	}

	void Serialize(IArchive& ar) { YASLI_ASSERT(0); }
private:

	TypeID type_;
	void* object_;
	size_t size_;
	SerializeStructFunc serializeFunc_;
};/*}}}*/
typedef std::vector<SStruct> SStructs;

// ---------------------------------------------------------------------------

class IContainer{
public:
	virtual size_t size() const = 0;
	virtual size_t resize(size_t size) = 0;
	virtual bool isFixedSize() const{ return false; }

	virtual void* pointer() const = 0;
	virtual TypeID type() const = 0;
	virtual bool next() = 0;
	virtual void extractInPlacePointers(IArchive& ar) const { YASLI_ASSERT(0 && "Not implemented"); }

	virtual void* elementPointer() const = 0;
	virtual size_t elementSize() const = 0;

	virtual bool operator()(IArchive& ar, const char* name, const char* label) = 0;
	virtual operator bool() const = 0;
	virtual void serializeNewElement(IArchive& ar, const char* name = "", const char* label = 0) const = 0;
};

template<class T>
class ContainerArray : public IContainer{/*{{{*/
	friend class IArchive;
public:
	explicit ContainerArray(T* array = 0, int size = 0)
	: array_(array)
	, size_(size)
	, index_(0)
	{
	}

	virtual void extractInPlacePointers(IArchive& ar) const
	{
	}

	// from ContainerSerializationInterface:
	size_t size() const{ return size_; }
	size_t resize(size_t size){
		index_ = 0;
		return size_;
	}

	void* pointer() const{ return reinterpret_cast<void*>(array_); }
	TypeID type() const{ return TypeID::get<T>(); }
	void* elementPointer() const { return &array_[index_]; }
	size_t elementSize() const { return sizeof(T); }
	virtual bool isFixedSize() const{ return true; }

	bool operator()(IArchive& ar, const char* name, const char* label){
		YASLI_ESCAPE(size_t(index_) < size_, return false);
		return ar(array_[index_], name, label);
	}
	operator bool() const{ return array_ != 0; }
	bool next(){
		++index_;
		return size_t(index_) < size_;
	}
	void serializeNewElement(IArchive& ar, const char* name, const char* label) const{
		T element;
		ar(element, name, label);
	}
	// ^^^

private:
	T* array_;
	int index_;
	size_t size_;
};/*}}}*/

class IClassFactory;
class IPointer
{
public:
	virtual TypeID type() const = 0;
	virtual void create(TypeID type) const = 0;
	virtual TypeID baseType() const = 0;
	virtual SStruct serializer() const = 0;
	virtual void* get() const = 0;
	virtual IClassFactory* factory() const = 0;
	virtual void extractInPlacePointers(IArchive& ar) const{ YASLI_ASSERT(0 && "Not implemented"); }
	
	void Serialize(IArchive& ar) const;
};

class IString
{
public:
	virtual void set(const char* value) = 0;
	virtual const char* get() const = 0;
	virtual const char** getInplacePointer() const{ return 0; }
};
class IWString
{
public:
	virtual void set(const wchar_t* value) = 0;
	virtual const wchar_t* get() const = 0;
	virtual const wchar_t** getInplacePointer() const{ return 0; }
};

}
// vim:ts=4 sw=4:
