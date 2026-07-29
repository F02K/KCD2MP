#pragma once

#include <Serialization/Serializer.h>

// Exposes _smart_ptr<> as serializeable type for Serialization::IArchive
template<class T>
class SmartPtrSerializer : public Serialization::IPointer
{
public:
	SmartPtrSerializer(_smart_ptr<T>& ptr)
	: m_ptr(ptr)
	{}

	Serialization::TypeID type() const
	{
		if (m_ptr)
			return Serialization::ClassFactory<T>::the().getTypeID(m_ptr.get());
		else
			return Serialization::TypeID();
	}

	void create(Serialization::TypeID type) const
	{
		CRY_ASSERT(!m_ptr || m_ptr->NumRefs() == 1);
		if (type)
			m_ptr.reset(Serialization::ClassFactory<T>::the().create(type));
		else
			m_ptr.reset((T*)0);
	}
	Serialization::TypeID baseType() const{ return Serialization::TypeID::get<T>(); }
	virtual Serialization::SStruct serializer() const{ return Serialization::SStruct(*m_ptr); }
	void* get() const{ return reinterpret_cast<void*>(m_ptr.get()); }
	Serialization::IClassFactory* factory() const{ return &Serialization::ClassFactory<T>::the(); }
protected:
	mutable _smart_ptr<T>& m_ptr;
};

template<class T>
bool Serialize(Serialization::IArchive& ar, _smart_ptr<T>& ptr, const char* name, const char* label)
{
	return ar(static_cast<Serialization::IPointer&>(SmartPtrSerializer<T>(ptr)), name, label);
}
