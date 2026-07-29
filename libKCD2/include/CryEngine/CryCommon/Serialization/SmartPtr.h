#pragma once

template <class T> class _smart_ptr;

namespace Serialization
{
	class IArchive;
};

template<class T>
bool Serialize(Serialization::IArchive& ar, _smart_ptr<T>& ptr, const char* name, const char* label);
