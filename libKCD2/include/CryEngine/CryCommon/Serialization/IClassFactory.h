// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once
#include <map>

#include "Serialization/Assert.h"
#include "Serialization/TypeID.h"

namespace Serialization{

class IArchive;
class TypeDescription;

class IClassFactory{
public: 
	IClassFactory(TypeID baseType)
	: baseType_(baseType)
	, nullLabel_(0)
	{
	}

	virtual size_t size() const = 0;
	virtual const TypeDescription* descriptionByIndex(int index) const = 0;	
	virtual const TypeDescription* descriptionByType(TypeID type) const = 0;
	virtual TypeID findTypeByName(const char* name) const = 0;	
	virtual size_t sizeOf(TypeID typeID) const = 0;
	virtual void serializeNewByIndex(IArchive& ar, int index, const char* name, const char* label) = 0;

	bool setNullLabel(const char* label){ nullLabel_ = label ? label : ""; return true; }
	const char* nullLabel() const{ return nullLabel_; }
protected:
	TypeID baseType_;
	const char* nullLabel_;
};


struct TypeIDWithFactory
{
	TypeID type;
	IClassFactory* factory;

	TypeIDWithFactory(TypeID type = TypeID(), IClassFactory* factory = 0)
	: type(type)
	, factory(factory)
	{
	}
};


}

bool Serialize(Serialization::IArchive& ar, Serialization::TypeIDWithFactory& value, const char* name, const char* label);
// vim:ts=4 sw=4:
