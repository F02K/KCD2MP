// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once

#include <map>

#include "Serialization/Serializer.h"
#include "Serialization/TypeID.h"

namespace Serialization{ class IArchive; }

template<class T>
bool Serialize(Serialization::IArchive& ar, T& object, const char* name, const char* label);

namespace Serialization{

class CEnumDescription;
template <class Enum>
CEnumDescription& getEnumDescription();
bool serializeEnum(const CEnumDescription& desc, IArchive& ar, int& value, const char* name, const char* label);

class IArchive
{
public:
	enum ArchiveCaps {
		INPUT = 1 << 0,
		OUTPUT = 1 << 1,
		TEXT = 1 << 2,
		BINARY = 1 << 3,
		EDIT = 1 << 4,
		INPLACE = 1 << 5,
	};

	IArchive(int caps)
	: caps_(caps)
	, filter_(0)
	, innerContext_(0)
	{
	}
	virtual ~IArchive() {}

	bool IsInput() const{ return caps_ & INPUT ? true : false; }
	bool IsOutput() const{ return caps_ & OUTPUT ? true : false; }
	bool IsEdit() const{ return caps_ & EDIT ? true : false; }
	bool IsInPlace() const{ return caps_ & INPLACE ? true : false; }
	bool GetCaps(int caps) const { return (caps_ & caps) == caps; }

	void SetFilter(int filter){
		filter_ = filter;
	}
	int GetFilter() const{ return filter_; }
	bool Filter(int flags) const{
		YASLI_ASSERT(flags != 0 && "flags is supposed to be a bit mask");
		YASLI_ASSERT(filter_ && "Filter is not set!");
		return (filter_ & flags) != 0;
	}

	virtual void Warning(const char* message) {}
	virtual void InPlacePointer(void** pointer, size_t offset) { YASLI_ASSERT(0 && "Not implemented"); }
	virtual bool operator()(bool& value, const char* name = "", const char* label = 0)           { notImplemented(); return false; }
	virtual bool operator()(unsigned char& value, const char* name = "", const char* label = 0) { notImplemented(); return false; }
	virtual bool operator()(signed char& value, const char* name = "", const char* label = 0)   { notImplemented(); return false; }
	virtual bool operator()(char& value, const char* name = "", const char* label = 0)          { notImplemented(); return false; }
	virtual bool operator()(short& value, const char* name = "", const char* label = 0)   { notImplemented(); return false; }
	virtual bool operator()(unsigned short& value, const char* name = "", const char* label = 0) { notImplemented(); return false; }
	virtual bool operator()(int& value, const char* name = "", const char* label = 0)            { notImplemented(); return false; }
	virtual bool operator()(unsigned int& value, const char* name = "", const char* label = 0)   { notImplemented(); return false; }
	virtual bool operator()(long long& value, const char* name = "", const char* label = 0)        { notImplemented(); return false; }
	virtual bool operator()(unsigned long long& value, const char* name = "", const char* label = 0)        { notImplemented(); return false; }
	virtual bool operator()(float& value, const char* name = "", const char* label = 0)          { notImplemented(); return false; }
	virtual bool operator()(double& value, const char* name = "", const char* label = 0)         { notImplemented(); return false; }

	virtual bool operator()(IString& value, const char* name = "", const char* label = 0)    { notImplemented(); return false; }
	virtual bool operator()(IWString& value, const char* name = "", const char* label = 0)    { notImplemented(); return false; }
	virtual bool operator()(const SStruct& ser, const char* name = "", const char* label = 0) { notImplemented(); return false; }
	virtual bool operator()(IContainer& ser, const char* name = "", const char* label = 0) { return false; }
	virtual bool operator()(IPointer& ptr, const char* name = "", const char* label = 0);
	
	template<class T>
	bool operator()(const T& value, const char* name = "", const char* label = 0);

	virtual bool OpenBlock(const char* name, const char* label) { return true; }
	virtual void CloseBlock() {}

	// there is no point to support long double since it is represented as double on MSVC
	bool operator()(long double& value, const char* name = "", const char* label = 0)         { notImplemented(); return false; }
	// fall back to int implementation for long
	bool operator()(long& value, const char* name = "", const char* label = 0) { return operator()(*reinterpret_cast<int*>(&value), name, label); }
	bool operator()(unsigned long& value, const char* name = "", const char* label = 0) { return operator()(*reinterpret_cast<unsigned int*>(&value), name, label); }
	
	// IArchive::SContext can be used to establish access to outer objects in serialization stack.
	//
	// Example:
	//   Scene::Serialize(...) {
	//     IArchive::SContext context(ar, this);
	//     ar(rootNode, ...);
	//   }
	//
	//   Node::Serialize(...) {
	//     Scene* scene = ar.FindContext<Scene>();
	//   }
	struct SContextLink {
		SContextLink* outer;
		TypeID type;
		void* contextObject;
	};
	template<class T>
	struct SContext : SContextLink {
		SContext(IArchive& ar, T* context) : ar_(ar) {
			outer = ar_.SetInnerContext(this);
			type = TypeID::get<T>();
			contextObject = (void*)context;
		}
		~SContext() { ar_.SetInnerContext(outer); }
	private:
		IArchive &ar_;
	};

	template<class T>
	T* FindContext() const {
		TypeID type(TypeID::get<T>());
		SContextLink* context = innerContext_;
		while (context) {
			if (context->type == type)
				return (T*)context->contextObject;
			context = context->outer;
		}
		return 0;
	}

	SContextLink* SetInnerContext(SContextLink* context) { 
		SContextLink* result = innerContext_;
		innerContext_ = context; 
		return result;
	}
	SContextLink* GetInnerContext() const{ return innerContext_; }
protected:


	void notImplemented() { YASLI_ASSERT(0 && "Not implemented!"); }

	int caps_;
	int filter_;

	SContextLink* innerContext_;
};

namespace detail{

template<bool C, class T1, class T2>
struct Selector{};

template<class T1, class T2>
struct Selector<false, T1, T2>{
    typedef T2 type;
};

template<class T1, class T2>
struct Selector<true, T1, T2>{
    typedef T1 type;
};

template<class C, class T1, class T2>
struct Select{
    typedef typename Selector<C::value, T1,T2>::type selected_type;
    typedef typename selected_type::type type;
};

template<class T>
struct Identity{
    typedef T type;
};


template<class T>
struct IsArray{
    enum{ value = false };
};

template<class T, int Size>
struct IsArray< T[Size] >{
    enum{ value = true };
};

template<class T, int Size>
struct ArraySize{
    enum{ value = true };
};

template<class T>
struct SerializeStruct{
	static bool invoke(IArchive& ar, T& value, const char* name, const char* label){
		return ar(SStruct(value), name, label);
	};
};

template<class Enum>
struct SerializeEnum{
	static bool invoke(IArchive& ar, Enum& value, const char* name, const char* label){
		const CEnumDescription& enumDescription = getEnumDescription<Enum>();
		return serializeEnum(enumDescription, ar, reinterpret_cast<int&>(value), name, label);
	};
};

template<class T>
struct SerializeArray{};

template<class T, int Size>
struct SerializeArray<T[Size]>{
	static bool invoke(IArchive& ar, T value[Size], const char* name, const char* label){
		ContainerArray<T> ser(value, Size);
		return ar(static_cast<IContainer&>(ser), name, label);
	}
};


template<class T>
struct IsClass{
private:
    struct NoType{ char dummy; };
    struct YesType{ char dummy[100]; };

    template<class U>
    static YesType function_helper(void(U::*)(void));

    template<class U>
    static NoType function_helper(...);
public:
    enum{ value = (sizeof(function_helper<T>(0)) == sizeof(YesType))};
};

}

template<class T>
bool IArchive::operator()(const T& value, const char* name, const char* label){
    return ::Serialize(*this, const_cast<T&>(value), name, label);
}

inline bool IArchive::operator()(IPointer& ptr, const char* name, const char* label)
{
	return (*this)(SStruct(const_cast<IPointer&>(ptr)), name, label);
}

}

template<class T, int Size>
bool Serialize(Serialization::IArchive& ar, T object[Size], const char* name, const char* label)
{
	YASLI_ASSERT(0);
	return false;
}

template<class T>
bool Serialize(Serialization::IArchive& ar, const T& object, const char* name, const char* label)
{
	T::unable_to_serialize_CONST_object();
	YASLI_ASSERT(0);
	return false;
}

template<class T>
bool Serialize(Serialization::IArchive& ar, T& object, const char* name, const char* label)
{
	using namespace Serialization::detail;

	return
		Select< IsClass<T>,
				Identity< SerializeStruct<T> >,
				Select< IsArray<T>,
					Identity< SerializeArray<T> >,
					Identity< SerializeEnum<T> >
				>
		>::type::invoke(ar, object, name, label);
}

#include "Serialization/SerializerImpl.h"

// vim: ts=4 sw=4:
