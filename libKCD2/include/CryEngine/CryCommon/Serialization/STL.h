// Copyright (c) 2012 Crytek GmbH
// Authors: Evgeny Andreeshchev, Alexander Kotliar
// Based on: Yasli - the serialization library.

#pragma once

#include <vector>
#include <list>

#include "Serialization/Serializer.h"

namespace Serialization{ class IArchive; }

template<class T, class Alloc>
bool Serialize(Serialization::IArchive& ar, std::vector<T, Alloc>& container, const char* name, const char* label);

template<class T, class Alloc>
bool Serialize(Serialization::IArchive& ar, std::list<T, Alloc>& container, const char* name, const char* label);

bool Serialize(Serialization::IArchive& ar, Serialization::string& value, const char* name, const char* label);
bool Serialize(Serialization::IArchive& ar, Serialization::wstring& value, const char* name, const char* label);

