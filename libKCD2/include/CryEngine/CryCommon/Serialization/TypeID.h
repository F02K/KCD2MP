// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once

#include "Serialization/Assert.h"
#include "Serialization/Strings.h"
#include <string.h>

namespace Serialization{

class IArchive;
struct TypeInfo;
class TypeID{
	friend class TypesFactory;
public:
	TypeID() : runtimeID_(0), typeInfo_(0) {}

	TypeID(const TypeID& original)
	: runtimeID_(original.runtimeID_)
	, typeInfo_(original.typeInfo_)
	{
	}

	operator bool() const{
		return *this != TypeID();
	}

	template<class T>
	static TypeID get(){
#ifdef _MSC_VER
		static TypeInfo typeInfo(sizeof(T), __FUNCSIG__);
#else
		static TypeInfo typeInfo(sizeof(T), __PRETTY_FUNCTION__);
#endif
		return typeInfo.id;
	}
	std::size_t sizeOf() const;
	const char* name() const;
	bool registered() const;

	bool operator==(const TypeID& rhs) const{
		return runtimeID_ == rhs.runtimeID_;
	}
	bool operator!=(const TypeID& rhs) const{
		return runtimeID_ != rhs.runtimeID_;
	}
	bool operator<(const TypeID& rhs) const{
		return runtimeID_ < rhs.runtimeID_;
	}
private:
	unsigned int runtimeID_;
	TypeInfo* typeInfo_;
	friend TypeInfo;
	friend class TypeDescription;
};

struct TypeInfo
{
	TypeID id;
	size_t size;
	char name[64];

	template<size_t nameLen>
	static void extractTypeName(char (&name)[nameLen], const char* funcName)
	{
		const char* s = strchr(funcName, '<');
		const char* send = strrchr(funcName, '>');
		YASLI_ASSERT(s != 0  && send != 0);
		if (s != send)
			++s;
		if(strncmp(s, "class ", 6) == 0)
			s += 6;
		else if(strncmp(s, "struct ", 7) == 0)
			s += 7;

		const char* nextNamespace = s;
		while (true) 
		{
			nextNamespace = strstr(s, "::");
			if (!nextNamespace)
				break;
			s = nextNamespace + 2;
		}

		char* d = name;
		const char* dend = name + sizeof(name) - 1;
		while (d != dend && s != send)
		{
			*d = *s;
			++s;
			++d;
		}
		*d = '\0';
		YASLI_ASSERT(s == send && "Type name doesn't fit into the buffer");
	}

	static unsigned int generateTypeID()
	{
		static unsigned int counter = 0;
		return ++counter;
	}

	TypeInfo(size_t size, const char* templatedFunctionName)
	: size(size)
	{
		id.runtimeID_ = generateTypeID();
		extractTypeName(name, templatedFunctionName);
		id.typeInfo_ = this;
	}
};

inline const char* TypeID::name() const{
	if (typeInfo_)
		return typeInfo_->name;
	else
		return "";
}

inline size_t TypeID::sizeOf() const{
	if (typeInfo_)
		typeInfo_->size;
	else
		return 0;
}

template<class T>
T* createDerivedClass(TypeID typeID);

}

//bool Serialize(Serialization::IArchive& ar, Serialization::TypeID& typeID, const char* name, const char* label);
