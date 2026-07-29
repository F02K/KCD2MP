// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once
#include <map>

#include "Serialization/Assert.h"
#include "Serialization/IClassFactory.h"
#include "Serialization/TypeID.h"

namespace Serialization{

class IArchive;

class TypeDescription{
public:
	TypeDescription(TypeID typeID, const char* name, const char *label, std::size_t size)
	: name_(name)
	, label_(label)
	, size_(size)
	, typeID_(typeID)
	{
		const size_t bufLen = sizeof(typeID.typeInfo_->name);
		strncpy(typeID.typeInfo_->name, name, bufLen - 1);
		typeID.typeInfo_->name[bufLen-1] = '\0';
	}
	const char* name() const{ return name_; }
	const char* label() const{ return label_; }
	std::size_t size() const{ return size_; }
	TypeID typeID() const{ return typeID_; }

protected:
	const char* name_;
	const char* label_;
	std::size_t size_;
	TypeID typeID_;
};

class ClassFactoryManager{
public:
	static ClassFactoryManager& the(){
		static ClassFactoryManager factoryManager;
		return factoryManager;
	}

	const IClassFactory* find(TypeID baseType) const{
		Factories::const_iterator it = factories_.find(baseType);
		if(it == factories_.end())
			return 0;
		else
			return it->second;
	}

	void registerFactory(TypeID type, const IClassFactory* factory){
		factories_[type] = factory;
	}
protected:
	typedef std::map<TypeID, const IClassFactory*> Factories;
	Factories factories_;
};

template<class BaseType>
class ClassFactory : public IClassFactory{
public:
	static ClassFactory& the(){
		static ClassFactory factory;
		return factory;
	}

	class CreatorBase{
	public:
		virtual ~CreatorBase() {}
		virtual BaseType* create() const = 0;
		const TypeDescription& description() const{ return *description_; }
		void* vptr() const{ return vptr_; }
	protected:
		const TypeDescription* description_;
		void* vptr_;
	};

	static void* extractVPtr(BaseType* ptr)
	{
		return *((void**)ptr);
	}

	template<class Derived>
	class Creator : public CreatorBase{
	public:
		Creator(const TypeDescription* description){
			this->description_ = description;

			// TODO: remove unnecessary static initialization
			Derived vptrProbe;
			vptr_ = extractVPtr(&vptrProbe);

			ClassFactory::the().registerCreator(this);
		}
		BaseType* create() const{
			return new Derived();
		}
	};

	ClassFactory()
	: IClassFactory(TypeID::get<BaseType>())
	{
		ClassFactoryManager::the().registerFactory(baseType_, this);
	}

	typedef std::map<TypeID, CreatorBase*> TypeToCreatorMap;

	BaseType* create(TypeID derivedType) const
	{
		typename TypeToCreatorMap::const_iterator it = typeToCreatorMap_.find(derivedType);
		if(it != typeToCreatorMap_.end())
			return it->second->create();
		else
			return 0;
	}

	TypeID getTypeID(BaseType* ptr) const
	{
		if (ptr == 0)
			return TypeID();
		void* vptr = extractVPtr(ptr);
		VPtrToCreatorMap::const_iterator it = vptrToCreatorMap_.find(vptr);
		if (it == vptrToCreatorMap_.end())
			return TypeID();
		return it->second->description().typeID();
	}

	size_t sizeOf(TypeID derivedType) const
	{
		typename TypeToCreatorMap::const_iterator it = typeToCreatorMap_.find(derivedType);
		if(it != typeToCreatorMap_.end())
			return it->second->description().size();
		else
			return 0;
	}

	BaseType* createByIndex(int index) const
	{
		YASLI_ASSERT(size_t(index) < creators_.size());
		return creators_[index]->create();
	}

	void serializeNewByIndex(IArchive& ar, int index, const char* name, const char* label)
	{
		YASLI_ESCAPE(size_t(index) < creators_.size(), return);
		BaseType* ptr = creators_[index]->create();
		ar(*ptr, name, label);
		delete ptr;
	}
	// from ClassFactoryInterface:
	size_t size() const{ return creators_.size(); }
	const TypeDescription* descriptionByIndex(int index) const{
		if(size_t(index) >= int(creators_.size()))
			return 0;
		return &creators_[index]->description();
	}

	const TypeDescription* descriptionByType(TypeID type) const{
		const size_t numCreators = creators_.size();
		for (size_t i = 0; i < numCreators; ++i) {
			if (type == creators_[i]->description().typeID())
				return &creators_[i]->description();
		}
		return 0;
	}

	virtual TypeID findTypeByName(const char* name) const {
		const size_t numCreators = creators_.size();
		for (size_t i = 0; i < numCreators; ++i) {
			const TypeID& typeID = creators_[i]->description().typeID();
			if (strcmp(name, typeID.name()) == 0)
				return typeID;
		}
		return TypeID();
	}
	// ^^^

protected:
	void registerCreator(CreatorBase* creator){
		typeToCreatorMap_[creator->description().typeID()] = creator;
		creators_.push_back(creator);
		vptrToCreatorMap_[creator->vptr()] =  creator;
	}


	TypeToCreatorMap typeToCreatorMap_;
	std::vector<CreatorBase*> creators_;

	typedef std::map<void*, CreatorBase*> VPtrToCreatorMap;
	VPtrToCreatorMap vptrToCreatorMap_;
};


class TypeLibrary{
public:
	static TypeLibrary& the();
	const TypeDescription* findByName(const char*) const;
	const TypeDescription* find(TypeID type) const;

	const TypeDescription* registerType(const TypeDescription* info);
protected:
	TypeLibrary();

	typedef std::map<TypeID, const TypeDescription*> TypeToDescriptionMap;
	TypeToDescriptionMap typeToDescriptionMap_;
};

}

#define SERIALIZATION_TYPE_NAME(Type, name) \
namespace{ \
	const Serialization::TypeDescription Type##_Description(Serialization::TypeID::get<Type>(), #Type, name, sizeof(Type)); \
	bool registered_##Type = Serialization::TypeLibrary::the().registerType(&Type##_Description) != 0; \
}

#define SERIALIZATION_CLASS_NULL(BaseType, name) \
namespace { \
    bool BaseType##_NullRegistered = Serialization::ClassFactory<BaseType>::the().setNullLabel(name); \
}

#define SERIALIZATION_CLASS(BaseType, Type, name) \
	const Serialization::TypeDescription Type##BaseType##_DerivedDescription(Serialization::TypeID::get<Type>(), #Type, name, sizeof(Type)); \
	Serialization::ClassFactory<BaseType>::Creator<Type> Type##BaseType##_Creator(Serialization::TypeLibrary::the().registerType(&Type##BaseType##_DerivedDescription)); \
	int dummyForType_##Type##BaseType;

#define SERIALIZATION_FORCE_CLASS(BaseType, Type) \
	extern int dummyForType_##Type##BaseType; \
	int* dummyForTypePtr_##Type##BaseType = &dummyForType_##Type##BaseType + 1;

#include "ClassFactoryImpl.h"
