// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#include "StringList.h"
#include "Serialization/STL.h"
#include "Serialization/IArchive.h"
#include "Serialization/STLImpl.h"
#include "Serialization/Strings.h"

namespace Serialization{

// ---------------------------------------------------------------------------
inline void splitStringList(StringList* result, const char *str, char delimeter)
{
    result->clear();

    const char* ptr = str;
    for(; *ptr; ++ptr)
	{
        if(*ptr == delimeter){
			result->push_back(string(str, ptr));
            str = ptr + 1;
        }
	}
	result->push_back(string(str, ptr));
}

inline void joinStringList(string* result, const StringList& stringList, char sep)
{
    YASLI_ESCAPE(result != 0, return);
    result->clear();
    for(StringList::const_iterator it = stringList.begin(); it != stringList.end(); ++it)
    {
        if(!result->empty())
            result += sep;
        result->append(*it);
    }
}

inline void joinStringList(string* result, const StringListStatic& stringList, char sep)
{
    YASLI_ESCAPE(result != 0, return);
    result->clear();
    for(StringListStatic::const_iterator it = stringList.begin(); it != stringList.end(); ++it)
    {
        if(!result->empty())
            (*result) += sep;
        YASLI_ESCAPE(*it != 0, continue);
        result->append(*it);
    }
}
}

inline bool Serialize(Serialization::IArchive& ar, Serialization::StringList& value, const char* name, const char* label)
{
	return ar(static_cast<std::vector<Serialization::string>&>(value), name, label);
}

inline bool Serialize(Serialization::IArchive& ar, Serialization::StringListValue& value, const char* name, const char* label)
{
	using Serialization::string;
    if(ar.IsEdit()){
        return ar(Serialization::SStruct(value), name, label);
    }
    else{
        string str;
        if(ar.IsOutput())
            str = value.c_str();
        if(ar(str, name, label) && ar.IsInput()){
            value = str.c_str();
            return true;
        }
        return false;
    }
}

inline bool Serialize(Serialization::IArchive& ar, Serialization::StringListStaticValue& value, const char* name, const char* label)
{
	using Serialization::string;
    if(ar.IsEdit())
        return ar(Serialization::SStruct(value), name, label);
    else{
        string str;
        if(ar.IsOutput())
            str = value.c_str();
        if(ar(str, name, label) && ar.IsInput()){
            value = str.c_str();
            return true;
        }
        return true;
    }
}
