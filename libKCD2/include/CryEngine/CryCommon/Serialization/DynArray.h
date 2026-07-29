// Copyright (c) 2012 Crytek GmbH
#pragma once

namespace Serialization { class IArchive; }

template<class T, class I, class S>
bool Serialize(Serialization::IArchive& ar, DynArray<T, I, S>& container, const char* name, const char* label);
