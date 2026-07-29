#ifndef _CRY_PTR_ARRAY_H_
#define _CRY_PTR_ARRAY_H_
#pragma once

#include "CryArray.h"

//---------------------------------------------------------------------------
template<class T, class P = T*>
struct PtrArray: DynArray<P>
{
	typedef DynArray<P> super;

	// Overrides.
	typedef T value_type;

	ILINE ~PtrArray(){}

	inline T&	operator [](int i) const
		{ return *super::operator[](i); }

	// Iterators.
	struct iterator
	{
		iterator(P* p)
			: _ptr(p)
			{}

		operator P* () const
			{ return _ptr; }
		void operator++()
			{ _ptr++; }
		void operator--()
			{ _ptr--; }
		T& operator*() const
			{ assert(_ptr); return **_ptr; }
		T* operator->() const
			{ assert(_ptr); return *_ptr; }

	protected:
		P* _ptr;
	};

	struct const_iterator
	{
		const_iterator(const P* p)
			: _ptr(p)
			{}

		operator const P* () const
			{ return _ptr; }
		void operator++()
			{ _ptr++; }
		void operator--()
			{ _ptr--; }
		T& operator*() const
			{ assert(_ptr); return **_ptr; }
		T* operator->() const
			{ assert(_ptr); return *_ptr; }

	protected:
		const P* _ptr;
	};

	void GetMemoryUsage(ICrySizer* pSizer) const
	{
		pSizer->AddObject( this->begin(), this->get_alloc_size() );
		for(int i = 0 ; i < this->size() ; ++i )
			pSizer->AddObject(this->super::operator [](i));
	}
};

//---------------------------------------------------------------------------
template<class T>
struct SmartPtrArray: PtrArray< T, _smart_ptr<T> >
{
};

#endif
