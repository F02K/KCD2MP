// Copyright (c) 2012 Crytek GmbH
#pragma once

#include "STLImpl.h"

template<class T, class I, class S>
bool Serialize(Serialization::IArchive& ar, DynArray<T, I, S>& container, const char* name, const char* label)
{
	Serialization::ContainerSTL<DynArray<T, I, S>, T> ser(&container);
	return ar(static_cast<Serialization::IContainer&>(ser), name, label);
}
