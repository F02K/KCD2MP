// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once

#include <vector>
#include "Serialization/Strings.h"
#include <string.h>
#include "Serialization/Assert.h"

namespace Serialization{

class IArchive;
class StringListStatic : public std::vector<const char*>{
public:
    enum { npos = -1 };
    int find(const char* value) const{
		int numItems = int(size());
		for(int i = 0; i < numItems; ++i){
            if(strcmp((*this)[i], value) == 0)
                return i;
        }
        return npos;
    }
};

class StringListStaticValue{
public:
	StringListStaticValue(const StringListStaticValue& original)
	: stringList_(original.stringList_)
	, index_(original.index_)
	{
	}
    explicit StringListStaticValue(const StringListStatic& stringList = StringListStatic(), int value = StringListStatic::npos)
    : stringList_(&stringList)
    , index_(value)
    {
    }
    StringListStaticValue(const StringListStatic& stringList, const char* value)
    : stringList_(&stringList)
    , index_(stringList.find(value))
    {
        YASLI_ASSERT(index_ != StringListStatic::npos);
    }
    StringListStaticValue& operator=(const char* value){
        index_ = stringList_->find(value);
		return *this;
    }
    StringListStaticValue& operator=(int value){
		YASLI_ASSERT(value >= 0 && std::size_t(value) < stringList_->size());
        YASLI_ASSERT(this != 0);
        index_ = value;
		return *this;
    }
    StringListStaticValue& operator=(const StringListStaticValue& rhs){
        stringList_ = rhs.stringList_;
		index_ = rhs.index_;
        return *this;
    }
    const char* c_str() const{
        if(index_ >= 0 && std::size_t(index_) < stringList_->size())
			return (*stringList_)[index_];
		else
			return "";
    }
    int index() const{ return index_; }
    const StringListStatic& stringList() const{ return *stringList_; }
    template<class IArchive>
    void Serialize(IArchive& ar) {
        ar(index_, "index");
    }
protected:
    const StringListStatic* stringList_;
    int index_;
};

class StringList: public std::vector<string>{
public:
    StringList() {}
    StringList(const StringList& rhs){
        *this  = rhs;
    }
    StringList(const StringListStatic& rhs){
        const int size = int(rhs.size());
        resize(size);
        for(int i = 0; i < int(size); ++i)
            (*this)[i] = rhs[i];
    }
    enum { npos = -1 };
    int find(const char* value) const{
		const int numItems = int(size());
		for(int i = 0; i < numItems; ++i){
            if((*this)[i] == value)
                return i;
        }
        return npos;
    }
};

class StringListValue{
public:
    explicit StringListValue(const StringListStaticValue &value)
	{
        stringList_.insert( stringList_.end(), value.stringList().begin(), value.stringList().end() );
        index_ = value.index();
	}
    StringListValue(const StringListValue &value)
	{
		stringList_ = value.stringList_;
        index_ = value.index_;
	}
    explicit StringListValue(const StringList& stringList = StringList(), int value = StringList::npos)
    : stringList_(stringList)
    , index_(value)
    {
    }
    StringListValue(const StringList& stringList, const char* value)
    : stringList_(stringList)
    , index_(stringList.find(value))
    {
        YASLI_ASSERT(index_ != StringList::npos);
    }
    StringListValue(const StringListStatic& stringList, const char* value)
    : stringList_(stringList)
    , index_(stringList.find(value))
    {
        YASLI_ASSERT(index_ != StringList::npos);
    }
    StringListValue& operator=(const char* value){
        index_ = stringList_.find(value);
		return *this;
    }
    StringListValue& operator=(int value){
		YASLI_ASSERT(value >= 0 && std::size_t(value) < stringList_.size());
        YASLI_ASSERT(this != 0);
        index_ = value;
		return *this;
    }
    const char* c_str() const{
        if(index_ >= 0 && std::size_t(index_) < stringList_.size())
			return stringList_[index_].c_str();
		else
			return "";
    }
    int index() const{ return index_; }
    const StringList& stringList() const{ return stringList_; }
    template<class IArchive>
    void Serialize(IArchive& ar) {
        ar(index_, "index");
		ar(stringList_, "stringList");
    }
protected:
    StringList stringList_;
    int index_;
};

class IArchive;

void splitStringList(StringList* result, const char *str, char sep);
void joinStringList(string* result, const StringList& stringList, char sep);
void joinStringList(string* result, const StringListStatic& stringList, char sep);

}

bool Serialize(Serialization::IArchive& ar, Serialization::StringList& value, const char* name, const char* label);
bool Serialize(Serialization::IArchive& ar, Serialization::StringListValue& value, const char* name, const char* label);
bool Serialize(Serialization::IArchive& ar, Serialization::StringListStaticValue& value, const char* name, const char* label);

#include "StringListImpl.h"
