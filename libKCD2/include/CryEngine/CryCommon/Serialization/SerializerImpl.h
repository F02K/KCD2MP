// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.
#pragma once

#include "IClassFactory.h"

// IArchive.h is supposed to be pre-included

namespace Serialization {

inline bool SStruct::operator()(IArchive& ar) const{
	YASLI_ESCAPE(serializeFunc_ && object_, return false);
	return serializeFunc_(object_, ar);
}

inline bool SStruct::operator()(IArchive& ar, const char* name, const char* label) const{
	return ar(*this, name, label);
}


inline void IPointer::Serialize(IArchive& ar) const
{
	TypeID baseTypeID = baseType();
	TypeID oldTypeID = type();
	IClassFactory* factory = this->factory();

	if(ar.IsOutput()){
		if(oldTypeID){
			TypeIDWithFactory pair(oldTypeID, factory);
			if(ar(pair, "")){
				ar(serializer(), "");
			}
			else
				ar.Warning("Unable to write typeID!");
		}
	}
	else{
		TypeIDWithFactory pair(TypeID(), factory);
		if(!ar(pair, "")){
			if(oldTypeID){
				create(TypeID()); // 0
			}
			return;
		}

		if(oldTypeID && (!pair.type || (pair.type != oldTypeID)))
			create(TypeID()); // 0

		if(pair.type){
			if(!get())
				create(pair.type);
			ar(serializer(), "");
		}
	}	
}

}
// vim:sw=4 ts=4:
