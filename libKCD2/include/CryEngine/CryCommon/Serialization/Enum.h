// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once

#include <vector>
#include <map>

#include "StringList.h"
#include "Serialization/TypeID.h"

namespace Serialization{

class IArchive;

struct LessStrCmp
{
	bool operator()(const char* l, const char* r) const{
		return strcmp(l, r) < 0;
	}
};

class CEnumDescription{
public:
	inline int value(const char* name) const;
	inline int valueByLabel(const char* label) const;
	inline const char* name(int value) const;
	inline const char* label(int value) const;
	inline const char* nameByIndex(int index) const;
	inline const char* indexByName(const char* name) const;
	inline int indexByValue(int value) const;

	inline bool Serialize(IArchive& ar, int& value, const char* name, const char* label) const;
	inline bool serializeBitVector(IArchive& ar, int& value, const char* name, const char* label) const;

	void add(int value, const char* name, const char* label = ""); // TODO
	const StringListStatic& names() const{ return names_; }
	const StringListStatic& labels() const{ return labels_; }
	inline StringListStatic nameCombination(int bitVector) const;
	inline StringListStatic labelCombination(int bitVector) const;
	bool registered() const { return !names_.empty(); }
	TypeID type() const{ return type_; }
private:
	StringListStatic names_;
	StringListStatic labels_;

	typedef std::map<const char*, int, LessStrCmp> NameToValue;
	NameToValue nameToValue_;
	typedef std::map<const char*, int, LessStrCmp> LabelToValue;
	LabelToValue labelToValue_;
	typedef std::map<int, int> ValueToIndex;
	ValueToIndex valueToIndex_;
	typedef std::map<int, const char*> ValueToName;
	ValueToName valueToName_;
	typedef std::map<int, const char*> ValueToLabel;
	ValueToName valueToLabel_;
	TypeID type_;
};

template<class Enum>
class EnumDescriptionImpl : public CEnumDescription{
public: static CEnumDescription& the(){
		static EnumDescriptionImpl description;
		return description;
	}
};

template<class Enum>
CEnumDescription& getEnumDescription(){
	return EnumDescriptionImpl<Enum>::the();
}

inline bool serializeEnum(const CEnumDescription& desc, IArchive& ar, int& value, const char* name, const char* label){
	return desc.Serialize(ar, value, name, label);
}

}

#define SERIALIZATION_ENUM_BEGIN(Type, label)                                                \
    namespace {                                                                     \
        bool registerEnum_##Type();                                                 \
        bool Type##_enum_registered = registerEnum_##Type();                        \
        bool registerEnum_##Type(){                                                 \
		Serialization::CEnumDescription& description = Serialization::EnumDescriptionImpl<Type>::the();

#define SERIALIZATION_ENUM_BEGIN_NESTED(Class, Enum, label)                                  \
    namespace {                                                                     \
	bool registerEnum_##Class##_##Enum();                                           \
		bool Class##_##Enum##_enum_registered = registerEnum_##Class##_##Enum();    \
		bool registerEnum_##Class##_##Enum(){                                       \
			Serialization::CEnumDescription& description = Serialization::EnumDescriptionImpl<Class::Enum>::the();

#define SERIALIZATION_ENUM_BEGIN_NESTED2(Class, Class1, Enum, label)                                  \
	namespace {                                                                     \
	bool registerEnum_##Class##Class1##_##Enum();                                           \
	bool Class##Class1##_##Enum##_enum_registered = registerEnum_##Class##Class1##_##Enum();    \
	bool registerEnum_##Class##Class1##_##Enum(){                                       \
	Serialization::CEnumDescription& description = Serialization::EnumDescriptionImpl<Class::Class1::Enum>::the();

                                                                                    
#define SERIALIZATION_ENUM_VALUE(value, label)                                              \
		description.add(int(value), #value, label);                                      
#define SERIALIZATION_ENUM(value, name, label)                                              \
		description.add(int(value), name, label);                                      

#define SERIALIZATION_ENUM_VALUE_NESTED(Class, value, label)                                       \
	description.add(int(Class::value), #value, label);                                      

#define SERIALIZATION_ENUM_VALUE_NESTED2(Class, Class1, value, label)                                       \
	description.add(int(Class::Class1::value), #value, label);                                      


#define SERIALIZATION_ENUM_END()													        \
            return true;                                                            \
        };                                                                          \
    };

#include "Serialization/EnumImpl.h"
// vim:ts=4 sw=4:
