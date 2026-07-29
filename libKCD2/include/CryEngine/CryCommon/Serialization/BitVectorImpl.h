// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once
#include "Serialization/BitVector.h"
#include "Serialization/IArchive.h"
#include "Serialization/Enum.h"

namespace Serialization{

struct BitVectorWrapper
{
	int* valuePointer;
	int value;
    const CEnumDescription* description;

	explicit BitVectorWrapper(int* _value = 0, const CEnumDescription* _description = 0)
    : valuePointer(_value)
    , description(_description)
    {
		if(valuePointer)
			value = *valuePointer;
    }
    BitVectorWrapper(const BitVectorWrapper& _rhs)
    : value(_rhs.value)
    , description(0)
	, valuePointer(0)
    {

    }

	~BitVectorWrapper()
	{
		if(valuePointer)
			*valuePointer = value;
	}
	BitVectorWrapper& operator=(const BitVectorWrapper& rhs){
		value = rhs.value;
		return *this;
	}


    void Serialize(IArchive& ar)
    {
		ar(value, "value", "Value");
    }
};

template<class Enum>
void BitVector<Enum>::Serialize(IArchive& ar)
{
    ar(value_, "value", "Value");
}

}

template<class Enum>
bool Serialize(Serialization::IArchive& ar, Serialization::BitVector<Enum>& value, const char* name, const char* label)
{
    using namespace Serialization;
    CEnumDescription &desc = getEnumDescription<Enum>();
    if(ar.IsEdit())
        return ar(BitVectorWrapper(&static_cast<int&>(value), &desc), name, label);
    else
    {
        return desc.serializeBitVector(ar, static_cast<int&>(value), name, label);
    }
}

