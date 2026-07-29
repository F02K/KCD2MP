// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#include "Serialization/STL.h"
#include "ClassFactory.h"
#include "Serialization/IArchive.h"
#include "Serialization/STLImpl.h"

#include "Serialization/Strings.h"

namespace Serialization{

inline TypeLibrary& TypeLibrary::the()
{
    static TypeLibrary typeLibrary;
    return typeLibrary;
}

inline TypeLibrary::TypeLibrary()
{

}

inline const TypeDescription* TypeLibrary::find(TypeID typeID) const
{
    if(!typeID)
		return 0;
    TypeToDescriptionMap::const_iterator it = typeToDescriptionMap_.find(typeID);
    if(it != typeToDescriptionMap_.end())
        return it->second;
    else
        return 0;
}

inline const TypeDescription* TypeLibrary::findByName(const char* name) const
{
    YASLI_ASSERT(name && strlen(name));
    TypeToDescriptionMap::const_iterator it;

    for(it = typeToDescriptionMap_.begin(); it != typeToDescriptionMap_.end(); ++it)
        if(strcmp(it->second->name(), name) == 0)
            return it->second;

    return 0;
}

inline const TypeDescription* TypeLibrary::registerType(const TypeDescription* description){
    typeToDescriptionMap_[description->typeID()] = description;
    return description;
}

// ----------------------------------------------------------------------------

inline bool TypeID::registered() const{
    return TypeLibrary::the().find(*this) != 0;
}

}

inline bool Serialize(Serialization::IArchive& ar, Serialization::TypeIDWithFactory& value, const char* name, const char* label)
{
	using Serialization::string;
	string typeName;
	if(ar.IsOutput()){
		typeName = value.type.name();
		return ar(typeName, name);
	}
	else{
		if(ar(typeName, name)){
			if(!typeName.empty())
				value.type = value.factory->findTypeByName(typeName.c_str());
			else
				value.type = Serialization::TypeID();
			if(!value.type){
				char msg[128];
				_snprintf_s(msg, sizeof(msg), _TRUNCATE, "Unable to read TypeID: unregistered type name: \'%s\'", typeName.c_str());
				ar.Warning(msg);
				return false;
			}
			else
				return true;
		}
		else
			return false;
	}
}
