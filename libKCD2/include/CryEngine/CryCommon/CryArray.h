//---------------------------------------------------------------------------
// Copyright 2005 Crytek GmbH
// Created by: Scott Peter
//---------------------------------------------------------------------------

#ifndef _CRY_ARRAY_H_
#define _CRY_ARRAY_H_
#pragma once

#if defined __CRYCG__
	#ifndef SPU_NO_INLINE
			#define SPU_NO_INLINE __attribute__ ((crycg_attr ("noinline")))
	#endif
#else
	#ifndef SPU_NO_INLINE
		#define SPU_NO_INLINE 
	#endif
#endif

#include <IGeneralMemoryHeap.h> // <> required for Interfuscator

//---------------------------------------------------------------------------
// Convenient iteration macros
#define for_iter(IT, it, b, e)			for (IT it = (b), _e = (e); it != _e; ++it)
#define for_container(CT, it, cont)	for_iter (CT::iterator, it, (cont).begin(), (cont).end())

#define for_ptr(T, it, b, e)				for (T* it = (b), *_e = (e); it != _e; ++it)
#define for_array_ptr(T, it, arr)		for_ptr (T, it, (arr).begin(), (arr).end())

#define for_array(i, arr)						for (int i = 0; i < (arr).size(); i++)
#define for_all(cont)								for_array (_i, cont) cont[_i]

//---------------------------------------------------------------------------
// Stack array helper
#define STACK_ARRAY(T, name, size) \
	PREFAST_SUPPRESS_WARNING(6255) \
	T* name = SPU_LOCAL_PTR( (T*) alloca((size) * sizeof(T)) );
	
#define ALIGNED_STACK_ARRAY(T, name, size, alignment) \
	PREFAST_SUPPRESS_WARNING(6255) \
	T* name = SPU_LOCAL_PTR( (T*) alloca((size) * sizeof(T) + alignment) ); \
	name = SPU_LOCAL_PTR( (T*)((size_t)name + (alignment-1) & (alignment-1)) );

//---------------------------------------------------------------------------
// Specify semantics for moving objects.
// If raw_movable() is true, objects will be moved with memmove().
// If false, with the templated move_init() function.
template<class T>
bool raw_movable(T const& dest)
{
	return false; 
}

// Generic move function: transfer an existing source object to uninitialised dest address.
// Addresses must not overlap (requirement on caller).
// May be specialised for specific types, to provide a more optimal move.
// For types that can be trivially moved (memcpy), do not specialise move_init, rather specialise raw_movable to return true.
template<class T>
void move_init(T& dest, T& source)
{
	assert(&dest != &source);
	new(&dest) T(source);
	source.~T();
}

/*---------------------------------------------------------------------------
Public classes:

		Array<T, I, STORAGE>
		StaticArray<T, I, nSIZE>
		DynArrayRef<T, I>
		DynArray<T, I>
		FixedDynArray<T, I>
		StaticDynArray<T, I, nSIZE>

Support classes are placed in namespaces NArray and NAlloc to reduce global name usage.
---------------------------------------------------------------------------*/

namespace NArray
{
	// We should never have these defined as macros.
	#undef min
	#undef max

	// Define our own min/max here, to avoid including entire <algorithm>.
	template<class T> inline T min( T a, T b )
		{ return a < b ? a : b; }
	template<class T> inline T max( T a, T b )
		{ return a > b ? a : b; }

	// Automatic inference of signed from unsigned int type.
	template<class T> struct IntTraits
	{
		typedef T TSigned;
	};

	template<> struct IntTraits<uint>
	{
		typedef int TSigned;
	};
	template<> struct IntTraits<uint64>
	{
		typedef int64 TSigned;
	};
#if !defined(LINUX) && !defined(APPLE)
	template<> struct IntTraits<unsigned long>
	{
		typedef long TSigned;
	};
#endif

	//---------------------------------------------------------------------------
	// Move array elements, with proper direction and move semantics.
	template<class T>
	void move_init(T* dest, T* source, size_t count)
	{
		if (source != dest)
		{
			assert(source > dest || source + count <= dest);
			if (raw_movable(*source))
			{
				memcpy(dest, source, count * sizeof(T));
			}
			else
			{
				for (T* dest_end = dest+count; dest < dest_end; )
					::move_init(*dest++, *source++);
			}
		}
	}

	/*---------------------------------------------------------------------------
	// STORAGE prototype for Array<T,I,STORAGE>.
	struct Storage<typename T>
	{
		typedef size_type;

		[const] T* begin() [const];
		size_type size() const;
	};
	---------------------------------------------------------------------------*/

	//---------------------------------------------------------------------------
	// ArrayStorage: Default STORAGE Array<T,I,STORAGE>.
	// Simply contains a pointer and count to an existing array,
	// performs no allocation or deallocation.

	template<class T, class I = int>
	struct ArrayStorage
	{
		typedef T value_type;
		typedef I size_type;

		// Construction.
		inline ArrayStorage()
			: m_aElems(0), m_nCount(0)
			{}
		inline ArrayStorage(T* elems, size_type count)
			: m_aElems(elems), m_nCount(count)
			{}
		inline ArrayStorage(T* start, T* finish)
			: m_aElems(start), m_nCount(check_cast<size_type>(finish-start))
			{}

		void set(T* elems, size_type count)
		{
			m_aElems = elems;
			m_nCount = count;
		}

		// Basic storage.
		CONST_VAR_FUNCTION( T* begin(),
			{ return m_aElems; } )
		inline size_type size() const	
			{ return m_nCount; }

		// Modifiers, alter range in place.
		void erase_front(size_type count = 1)
		{ 
			assert(count <= m_nCount);
			m_nCount -= count;
			m_aElems += count;
		}

		void erase_back(size_type count = 1)
		{ 
			assert(count <= m_nCount);
			m_nCount -= count;
		}

		void resize(size_type count)
		{
			assert(count <= m_nCount);
			m_nCount = count;
		}
		
	protected:
		T*				m_aElems;
		size_type	m_nCount;
	};

	//---------------------------------------------------------------------------
	// StaticArrayStorage: STORAGE scheme with a statically sized member array.

	template<class T, int nSIZE, class I = int>
	struct StaticArrayStorage
	{
		typedef T value_type;
		typedef I size_type;

		// Basic storage.
		CONST_VAR_FUNCTION( T* begin(),
			{ return m_aElems; } )
		inline static size_type size()
			{ return (size_type)nSIZE; }

	protected:
		T				m_aElems[nSIZE];
	};
};

//---------------------------------------------------------------------------
// Array<T,I,S>: Non-growing array.
// S serves as base class, and implements storage scheme: begin(), size()

template< class T, class I = int, class S = NArray::ArrayStorage<T,I> >
struct Array: S
{
	// Tedious redundancy.
	using_type(S, size_type)

	using S::size;
	using S::begin;

	// STL-compatible typedefs.
	typedef typename NArray::IntTraits<size_type>::TSigned 
												difference_type;
	typedef T							value_type;
	typedef T*						pointer;
	typedef const T*			const_pointer;
	typedef T&						reference;
	typedef const T&			const_reference;

	typedef T*						iterator;
	typedef const T*			const_iterator;

	// Construction.
	Array()
		{}

	// Forward single- and double-argument constructors.
	template<class In>
	explicit Array(const In& i)
		: S(i)
		{}

	template<class In1, class In2>
	Array(const In1& i1, const In2& i2)
		: S(i1, i2)
		{}

	// Accessors.
	inline bool empty() const
		{ return size() == 0; }
	inline size_type size_mem() const
		{ return size() * sizeof(T); }

	CONST_VAR_FUNCTION( T* end(),								
		{ return begin()+size(); } )

	CONST_VAR_FUNCTION( T* rbegin(),
		{ return begin()+size()-1; } )

	CONST_VAR_FUNCTION( T* rend(),
		{ return begin()-1; } )

	CONST_VAR_FUNCTION( T& front(),
	{ 
		assert(!empty()); 
		return *begin(); 
	} )

	CONST_VAR_FUNCTION( T& back(),
	{ 
		assert(!empty()); 
		return *rbegin();
	} )

	CONST_VAR_FUNCTION( T& at(size_type i),
	{
		CRY_ASSERT_TRACE(i >= 0 && i < size(), ("Index %lld is out of range (array size is %lld)", (long long int) i, (long long int) size()));
		return begin()[i];
	} )

	CONST_VAR_FUNCTION( T& operator [](size_type i), 
	{
		CRY_ASSERT_TRACE(i >= 0 && i < size(), ("Index %lld is out of range (array size is %lld)", (long long int) i, (long long int) size()));
		return begin()[i];
	} )

	// Conversion to canonical array type.
	operator Array<T,I>()
		{ return Array<T,I>(begin(), size()); }
	operator Array<const T,I>() const
		{ return Array<const T,I>(begin(), size()); }

	// Additional conversion via operator() to full or sub array.
	Array<T,I> operator ()(size_type i = 0, size_type count = -1)
	{
		assert(i >= 0 && i+count <= size());
		return Array<T,I>( SPU_MAIN_PTR(begin()+i), count >= 0 ? count : size()-i);
	}
	Array<const T,I> operator ()(size_type i = 0, size_type count = -1) const
	{
		assert(i >= 0 && i+count <= size());
		return Array<const T,I>(begin()+i, count >= 0 ? count : size()-i);
	}

	// Basic element assignment functions.

	// Copy values to existing elements.
	void fill(const T& val)
	{
		for_array_ptr (T, it, *this)
			*it = val;
	}

	void copy(Array<const T,I> array)
	{
		assert(array.size() == size());
		const T* src = array.begin();
		for_array_ptr (T, it, *this)
			*it = *src++;
	}
};

// Type-inferring constructor.

template<class T, class I>
inline Array<T,I> ArrayT(T* elems, I count)
{
	return Array<T,I>(elems, count);
}

// StaticArray<T,I,nSIZE>
// A superior alternative to static C arrays.
// Provides standard STL-like Array interface, including bounds-checking.
//		standard:		Type array[256];
//		structured:	StaticArray<Type,256> array;

template<class T, int nSIZE, class I = int> 
struct StaticArray: Array< T, I, NArray::StaticArrayStorage<T,nSIZE,I> >
{
};

//---------------------------------------------------------------------------
// Specify allocation for dynamic arrays
namespace NAlloc
{
	// Adds prefix bytes to allocation, preserving alignment
	template<class A, int nPrefixSize>
	struct AllocPrefix : private A
	{
		AllocPrefix()
			{}

		AllocPrefix(const A& a)
			: A(a) {}

		void* alloc( size_t& nNewBytes, bool bSlack, int nAlign )
		{
			assert(nNewBytes);
			size_t nPrefixBytes = Align(nPrefixSize, nAlign);
			nNewBytes += nPrefixBytes;
			void* pNew = A::alloc( nNewBytes, bSlack, nAlign );
			if (nNewBytes)
				nNewBytes -= nPrefixBytes;
			if (pNew)
				pNew = (char*)pNew + nPrefixBytes;
			return pNew;
		}

		void dealloc( void* pMem, int nAlign )
		{
			assert(pMem);
			size_t nPrefixBytes = Align(nPrefixSize, nAlign);
			pMem = (char*)pMem - nPrefixBytes;
			A::dealloc( pMem, nAlign );
		}

		size_t alloc_size( void* pMem, size_t nMemBytes, int nAlign = 1 ) const
		{
			if (!pMem)
				return 0;
			size_t nPrefixBytes = Align(nPrefixSize, nAlign);
			nMemBytes += nPrefixBytes;
			pMem = (char*)pMem - nPrefixBytes;
			return A::alloc_size(pMem, nMemBytes, nAlign);
		}
	};

	struct StandardAlloc;

	inline size_t ReallocSize( size_t nMinSize );	// KCD-RE patch: fwd-decl (defined below) for WHDynStorage

	template<class T, class I, class A>
	T* reallocate( A& a, T* old_elems, I old_size, I new_size, I& new_cap, bool allow_slack = false, I alignment = 1 )
	{
		assert(new_size <= new_cap);
		size_t new_bytes = new_cap * sizeof(T);

		T* new_elems = (T*) SPU_MAIN_PTR( new_cap ? a.alloc( new_bytes, allow_slack, alignment ) : 0 );
		assert(IsAligned(new_elems, alignment));

		assert(new_bytes >= new_cap*sizeof(T));
		new_cap = I(new_bytes / sizeof(T));

		if (old_elems)
		{
			if (new_elems)
				// Move elements.
				NArray::move_init(new_elems, old_elems, NArray::min(old_size, new_size));

			// Dealloc old.
			a.dealloc( old_elems, alignment );
		}

		return new_elems;
	}
};

//---------------------------------------------------------------------------
// Storage schemes for dynamic arrays
namespace NArray
{
	/*---------------------------------------------------------------------------
	// STORAGE prototype for DynArray<T,I,STORAGE,ALLOC>
	// Extends ArrayStorage with resizing functionality.

	struct DynStorage<T,I>: Storage<T,I>
	{
		size_type capacity();
		size_type max_size();
		void resize_raw( size_type new_size );
		void resize_raw( size_type new_size, size_type new_cap );
		void destroy( T* );
		void init( T* );
		void init( T*, const T& val );
		~DynStorage();
	};
	---------------------------------------------------------------------------*/

	// Base class specifying init/destroy behavior for uninitialised storage.
	template<class T>
	struct RawStorage
	{
		static void destroy(T* obj)
			{ obj->~T(); }
		static void init(T* obj)
			{ new(obj) T; }
		static void init(T* obj, const T& val)
			{ new(obj) T(val); }
	};

	// Base class specifying init/destroy behavior for pre-initialised storage.
	template<class T>
	struct InitStorage
	{
		static void destroy(T* obj)
			{}
		static void init(T* obj)
			{ obj->~T(); new(obj) T; }
		static void init(T* obj, const T& val)
			{ *obj = val; }
	};

	//---------------------------------------------------------------------------
	// FixedDynStorage: STORAGE scheme for DynArray<> which simply
	// uses a non-growable array type, and adds a current count.

	template<class S>
	struct FixedDynStorage: InitStorage<typename S::value_type>
	{
		using_type(S, size_type);
		using_type(S, value_type);
		typedef value_type T;

		// Construction.
		FixedDynStorage()
			: m_nCount(0)
		{}

		CONST_VAR_FUNCTION( T* begin(),
			{ return m_Array.begin(); } )
		size_type size() const
			{ return m_nCount; }
		size_type get_alloc_size() const
			{ return 0; }

		size_type capacity() const
			{ return m_Array.size(); }
		size_type max_size() const
			{ return m_Array.size(); }

		void resize_raw( size_type new_size, size_type new_cap = 0 )
		{
			// capacity unchangeable, just assert new_cap within range.
			assert(new_cap <= max_size());
			assert(new_size >= 0 && new_size <= max_size());
			m_nCount = new_size;
			MEMSTAT_USAGE(m_Array.begin(), new_size * sizeof(T));
		}

	protected:
		size_type		m_nCount;
		S						m_Array;
	};

	//---------------------------------------------------------------------------
	// FastDynStorage: STORAGE scheme for DynArray<T,I,STORAGE>.
	// Simple extension to ArrayStorage: size & capacity fields are inline, 3 words storage, fast access.

	template<class T, class I = int, class A = NAlloc::StandardAlloc>
	struct FastDynStorage: ArrayStorage<T,I>, RawStorage<T>, A
	{
		typedef FastDynStorage<T,I,A> self_type;
		typedef ArrayStorage<T,I> super_type;
		using_type(super_type, size_type);

		using super_type::m_aElems;
		using super_type::m_nCount;

		static int alignment()
			{ return alignof(T); }

		// Construction.
		FastDynStorage()
			: m_nCapacity(0)
		{
			MEMSTAT_REGISTER_CONTAINER(this, EMemStatContainerType::MSC_Vector, T);
		}

		~FastDynStorage()
		{
			if (m_aElems)
				A::dealloc( m_aElems, alignment() );

			MEMSTAT_UNREGISTER_CONTAINER(this);
		}

		void swap(self_type& a)
		{
			self_type temp = *this;
			*this = a;
			a = temp;
			MEMSTAT_SWAP_CONTAINERS(this, &a);
		}

		inline size_type capacity() const
			{ return m_nCapacity; }
		inline size_type get_alloc_size() const
			{ return A::alloc_size(m_aElems, capacity() * sizeof(T), alignment()); }

		SPU_NO_INLINE void resize_raw( size_type new_size, size_type new_cap, bool allow_slack = false )
		{
			assert(new_cap >= new_size);
			if (new_cap != capacity())
			{
				m_aElems = NAlloc::reallocate( *this, m_aElems, m_nCount, new_size, new_cap, allow_slack, alignment() );
				m_nCapacity = new_cap;
				MEMSTAT_BIND_TO_CONTAINER(this, m_aElems);
			}
			set_size(new_size);
		}

		void resize_raw( size_type new_size )
		{
			if (new_size <= capacity())
				set_size(new_size);
			else
				resize_raw(new_size, new_size, capacity() != 0);
		}

	protected:
		size_type		m_nCapacity;

		void set_size( size_type new_size )
		{
			assert(new_size <= capacity());
			m_nCount = new_size;
			MEMSTAT_USAGE(m_aElems, new_size * sizeof(T));
		}
	};

	//---------------------------------------------------------------------------
	// SmallDynStorage: STORAGE scheme for DynArray<T,I,STORAGE,ALLOC>.
	// Array is just a single pointer, size and capacity information stored before the array data.
	// Slightly slower than FastDynStorage, optimal for saving space, especially when array likely to be empty.

	template<class T, class I>
	struct StorageHeader
	{
		typedef I size_type;

		static const size_type nCAP_BIT = size_type(1) << (sizeof(size_type)*8 - 1);

		ILINE T* elems() const				
		{
			const StorageHeader* tmp = SPU_MAIN_PTR( this+1 );
			return SPU_MAIN_PTR( (T*)tmp );
		}
		bool is_null() const
			{ return m_nSizeCap == 0; }
		ILINE size_type size() const
			{ return m_nSizeCap & ~nCAP_BIT; }

		size_type	capacity() const
		{
			size_type cap = m_nSizeCap;
			if (cap & nCAP_BIT)
			{
				cap &= ~nCAP_BIT;
				uint8* pCap = SPU_MAIN_PTR( (uint8*)(elems() + cap) );
				if (*pCap)
					// Stored as byte.
					cap += *pCap;
				else
				{
					// Stored in next aligned word.
					pCap = SPU_MAIN_PTR( Align(pCap+1, alignof(size_type)) );
					cap += *(size_type*)pCap;
				}
			}
			return cap;
		}

		void set_sizes( size_type s, size_type c )
		{
			// Store size, and assert against overflow.
			m_nSizeCap = s;
			if (c > s)
			{
				// Store extra capacity after elements.
				m_nSizeCap |= nCAP_BIT;
				uint8* pCap = SPU_MAIN_PTR( (uint8*)(elems() + s) );
				size_type nExtra = c - s;
				if (nExtra < 256)
					// If it fits in a byte, store it there.
					*pCap = nExtra;
				else
				{
					// Otherwise, mark it 0, store it in next aligned word.
					*pCap++ = 0;
					pCap = SPU_MAIN_PTR( Align(pCap, alignof(size_type)) );
					*(size_type*)pCap = nExtra;
				}
			}
			assert(size() == s);
			assert(capacity() == c);
		}

	protected:
		size_type		m_nSizeCap;				// Store allocation size, with last bit indicating extra capacity.
	};

	template<class T, class I = int, class A = NAlloc::StandardAlloc>
	// KCD-RE patch: __declspec(empty_bases) so MSVC collapses the two empty bases
	// (RawStorage + AllocPrefix) and this matches the retail single-pointer (8-byte) layout.
	struct __declspec(empty_bases) SmallDynStorage: RawStorage<T>, NAlloc::AllocPrefix<A, sizeof(StorageHeader<T,I>)>
	{
		typedef SmallDynStorage<T,I,A> self_type;
		typedef StorageHeader<T,I> Header;
		typedef NAlloc::AllocPrefix<A, sizeof(Header)> AP;
		typedef I size_type;

		// Construction.
		SmallDynStorage()
		{
			set_null(); 
			MEMSTAT_REGISTER_CONTAINER(this, EMemStatContainerType::MSC_Vector, T);
		}

		SmallDynStorage(const A& a)
			: AP(a)
		{
			set_null(); 
			MEMSTAT_REGISTER_CONTAINER(this, EMemStatContainerType::MSC_Vector, T);
		}

		~SmallDynStorage()
		{
			if (!is_null())
				AP::dealloc( m_aElems, alignment() );
			MEMSTAT_UNREGISTER_CONTAINER(this);
		}

		void swap(self_type& a)
		{
			T* pTemp = m_aElems;
			m_aElems = a.m_aElems;
			a.m_aElems = pTemp;
			MEMSTAT_SWAP_CONTAINERS(this, &a);
		}

		// Basic storage.
		CONST_VAR_FUNCTION( T* begin(),
			{ return m_aElems; } )
		inline size_type size() const
			{ return header()->size(); }
		inline size_type capacity() const
			{ return header()->capacity(); }
		inline size_type get_alloc_size() const
			{ return is_null() ? 0 : AP::alloc_size( m_aElems, capacity() * sizeof(T), alignment() ); }

		SPU_NO_INLINE void resize_raw( size_type new_size, size_type new_cap, bool allow_slack = false )
		{
			assert(new_cap >= new_size);
			if (new_cap != capacity())
			{
				m_aElems = NAlloc::reallocate( static_cast<AP&>(*this), header()->is_null() ? 0 : m_aElems, size(), new_size, new_cap, allow_slack, alignment() );
				MEMSTAT_BIND_TO_CONTAINER(this, m_aElems);

				if (!m_aElems)
				{
					set_null();
					return;
				}
			}
			header()->set_sizes(new_size, new_cap);
			MEMSTAT_USAGE(begin(), new_size * sizeof(T));
		}

		void resize_raw( size_type new_size )
		{
			size_type cap = capacity();
			if (new_size <= cap)
			{
				header()->set_sizes(new_size, cap);
				MEMSTAT_USAGE(begin(), new_size * sizeof(T));
			}
			else
				resize_raw(new_size, new_size, cap != 0);
		}

	protected:

		T*				m_aElems;

		static int alignment()
		{
			return NArray::max(alignof(T), alignof(Header));
		}

		CONST_VAR_FUNCTION( Header* header(),
		{
			assert(m_aElems);
			return ((Header*)m_aElems)-1;
		} )

		struct EmptyHeader
		{
			Header	head;
			char		pad[alignof(T)];
		}; 

		// workaround for SPUS which can't handle function static variables:
		// use a global empty header var and assign ptr to function static
		// on spu use the global var
#if defined(PS3)
		static T* gEmptyHeaderPtr;
#endif

		void set_null()
		{
#if !defined(__SPU__)
			// m_aElems is never 0, for empty array points to a static empty header.
			// Declare a big enough static var to account for alignment.
			static EmptyHeader s_EmptyHeader;

			// The actual header pointer can be anywhere in the struct, it's all initialised to 0.
			static T* s_EmptyElems = (T*)Align(s_EmptyHeader.pad, alignment());

			m_aElems = s_EmptyElems;

#if defined(PS3) // assign to global header
			gEmptyHeaderPtr = s_EmptyElems;
#endif

#else // on spu only use the global ptr
			m_aElems = gEmptyHeaderPtr;
#endif
		}
		inline bool is_null() const
			{ return header()->is_null(); }
	};

#if defined(PS3)
	template<class T, class I, class A>
	T* SmallDynStorage<T,I,A>::gEmptyHeaderPtr;
#endif

};

// KCD-RE patch: the game's own DynArray storage policy (used as the default below).
#include "WHDynStorage.h"

//---------------------------------------------------------------------------

// DynArrayRef<T,STORAGE>: Non-growing access to a DynArray (see below).
// Does not specify allocation scheme, any DynArray can convert to it.
// Simply an Array alias.
// KCD-RE patch: default storage = WHDynStorage (the game's block format).
// SmallDynStorage (stock CryEngine, module-local heap) kept for reference.

template< class T, class I = int, class S = NArray::WHDynStorage<T,I> >
struct DynArrayRef: Array<T,I,S>
{
};

//---------------------------------------------------------------------------
// DynArray<T,A,S>: Extension of Array allowing dynamic allocation.
// S specifies storage scheme, as with Array, but adds resize(), capacity(), ...
// A specifies the actual memory allocation function: alloc()

template< class T, class I = int, class S = NArray::WHDynStorage<T,I> >
struct DynArray: DynArrayRef<T,I,S>
{
	typedef DynArray<T,I,S> self_type;
	typedef DynArrayRef<T,I,S> super_type;

	// Tedious redundancy for GCC.
	using_type(super_type, size_type);
	using_type(super_type, iterator);
	using_type(super_type, const_iterator);

	using super_type::size;
	using super_type::capacity;
	using super_type::empty;
	using super_type::begin;
	using super_type::end;
	using super_type::back;
	using super_type::fill;
	using super_type::copy;

	//
	// Construction.
	//
	DynArray()
		{}

	DynArray(size_type count)
	{
		grow(count);
	}
	DynArray(size_type count, const T& val)
	{
		grow(count, val);
	}

	// Copying from a generic array type.
	DynArray(Array<const T,I> a)
	{
		push_back(a);
	}
	SPU_NO_INLINE self_type& operator =(Array<const T,I> a)
	{
		if (a.begin() >= begin() && a.end() <= end())
		{
			// Assigning from (partial) self; remove undesired elements.
			erase((T*)a.end(), end());
			erase(begin(), (T*)a.begin());
		}
		else
		{
			// Assert no overlap.
			assert(a.end() <= begin() || a.begin() >= end());
			if (a.size() == size())
			{
				// If same size, perform element copy.
				copy(a);
			}
			else
			{
				// If different sizes, destroy then copy init elements.
				pop_back(size());
				S::resize_raw(a.size(), a.size());
				init(*this, a.begin());
			}
		}
		return *this;
	}

	// Copy init/assign.
	inline DynArray(const self_type& a)
	{
		push_back(a());
	}
	inline self_type& operator =(const self_type& a)
	{
		return *this = a();
	}

	// Init/assign from basic storage type.
	inline DynArray(const S& a)
	{
		push_back(Array<const T,I>(a.begin(), a.size()));
	}
	inline self_type& operator =(const S& a)
	{
		return *this = Array<const T,I>(a.begin(), a.size());
	}

	inline ~DynArray()
	{
		destroy();
	}

	inline size_type available() const
	{ 
		return capacity() - size(); 
	}

	//
	// Allocation modifiers.
	//

	void reserve(size_type count)
	{
		if (count > capacity())
			S::resize_raw(size(), count);
	}

	// Grow array if needed, return iterator to new raw elems.
	iterator grow_raw(size_type count = 1)
	{
		return expand_raw(end(), count);
	}
	Array<T,I> append_raw(size_type count = 1)
	{
		return Array<T,I>(expand_raw(end(), count), count);
	}
	iterator grow(size_type count)
	{
		return init(append_raw(count));
	}
	iterator grow(size_type count, const T& val)
	{
		return init(append_raw(count), val);
	}

	void shrink()
	{
		// Realloc memory to exact array size.
		S::resize_raw(size(), size());
	}

	SPU_NO_INLINE void resize(size_type new_size)
	{
		size_type s = size();
		if (new_size > s)
			grow(new_size - s);
		else
			pop_back(s-new_size);
	}
	SPU_NO_INLINE void resize(size_type new_size, const T& val)
	{
		size_type s = size();
		if (new_size > s)
			grow(new_size - s, val);
		else
			pop_back(s-new_size);
	}

	void assign(size_type n, const T& val)
	{
		resize(n);
		fill(val);
	}

  void assign(const_iterator start, const_iterator finish)
	{
		*this = Array<const T,I>(start, finish);
	}

	iterator push_back()
	{
		return grow(1);
	}
	iterator push_back(const T& val)
	{ 
		return grow(1, val);
	}
	iterator push_back(Array<const T,I> array)
	{
		return init(append_raw(array.size()), array.begin());
	}

	SPU_NO_INLINE Array<T,I> insert_raw(iterator pos, size_type count = 1)
	{
		// Grow array if needed, return iterator to inserted raw elems.
		pos = expand_raw(pos, count);

		// Copy-init to end elements.
		T* dest = end();
		T* dest_start = NArray::max(end()-count, pos+count);
		while (dest-- > dest_start)
			S::init(dest, *(dest-count));

		// Copy moved elements.
		dest_start = pos+count;
		dest++;
		while (dest-- > dest_start)
			*dest = *(dest-count);

		// Destroy first copied elements.
		dest = NArray::min(dest+1, end() - count);
		while (dest-- > pos)
			S::destroy(dest);

		return Array<T,I>(pos, count);
	}

	iterator insert(iterator it, size_type count = 1)
	{
		return init(insert_raw(it, count));
	}
	iterator insert(iterator it, const T& val)
	{
		return init(insert_raw(it, 1), val);
	}
	iterator insert(iterator it, size_type count, const T& val)
	{
		return init(insert_raw(it, count), val);
	}
	iterator insert(iterator it, Array<const T,I> array)
	{
		return init(insert_raw(it, array.size()), array.begin());
	}
	iterator insert(iterator it, const_iterator start, const_iterator finish)
	{
		return insert( it, Array<T,I>(start, check_cast<size_type>(finish-start)) );
	}

	void pop_back(size_type count = 1)
	{
		// Destroy erased elems, change size without reallocing.
		assert(count <= size());
		size_type new_size = size()-count;
		destroy((*this)(new_size));
		S::resize_raw(new_size);
	}

	SPU_NO_INLINE iterator erase(iterator start, iterator finish)
	{
		assert(start >= begin() && finish >= start && finish <= end());

		// Copy over erased elems, destroy those at end.
		iterator it = start, e = end();
		while (finish < e)
			*it++ = *finish++;
		pop_back(check_cast<size_type>(finish - it));
		return start;
	}

	iterator erase(iterator it)
	{
		return erase(it, it+1);
	}

	iterator erase(size_type pos, size_type count = 1)
	{
		return erase(begin()+pos, begin()+pos+count);
	}

	void clear()
	{
		if (capacity())
		{
			destroy();
			S::resize_raw(0, 0);
		}
	}

protected:

	// Grow array if needed, return iterator to inserted raw elems.
	SPU_NO_INLINE iterator expand_raw(iterator pos, size_type count)
	{
		assert(pos >= begin() && pos <= end());
		T* old = begin();
		S::resize_raw(size() + count);
		return begin() + (pos - old);
	}

	//
	// Raw element construct/destruct functions.
	//
	static void destroy(Array<T,I> range)
	{
		// Destroy in reverse order, to complement construction order.
		for (iterator it = range.end(); it-- > range.begin(); )
			S::destroy(it);
	}

	void destroy()
	{
		destroy(*this);
	}

	static iterator init(Array<T,I> range)
	{
		for_array_ptr (T, it, range)
			S::init(it);
		return range.begin();
	}

	static iterator init(Array<T,I> range, const T& val)
	{
		for_array_ptr (T, it, range)
			S::init(it, val);
		return range.begin();
	}

	static iterator init(Array<T,I> range, const T* source)
	{
		assert(source + range.size() <= range.begin() || source >= range.end());
		for_array_ptr (T, it, range)
			S::init(it, *source++);
		return range.begin();
	}
};

template<class T, class I = int>
struct FixedDynArray: DynArray< T, I, NArray::FixedDynStorage< NArray::ArrayStorage<T,I> > >
{
	void set( Array<T,I> array, I count = 0 )
	{
		this->m_Array = array; 
		this->m_nCount = count;
	}
};

template<class T, int nSIZE, class I = int>
struct StaticDynArray: DynArray< T, I, NArray::FixedDynStorage< NArray::StaticArrayStorage<T,nSIZE,I> > >
{
};

/*---------------------------------------------------------------------------
// ALLOC prototype for ArrayStorage.

struct Alloc
{
	static void* alloc( size_t& nNewBytes, bool bSlack = false, int nAlign = 1 );
	static void dealloc( void* pMem, int nAlign = 1 );
	static size_t alloc_size( void* pMem, size_t nUsedBytes, int nAlign = 1 );
};

---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
namespace NAlloc
{
	// since this file is also included for compiling SPU jobs, we get double implementations
	// for all non-template classes, so an ifdef is needed
#if !defined(_SPU_JOB)

	inline size_t ReallocSize( size_t nMinSize )
	{
		// Choose an efficient realloc size, when growing an existing (non-zero) block.
		// Find the next power-of-two, minus a bit of slack for presumed system alloc overhead.
		const size_t nSlack = 16;
		#if defined(PS3) || defined(XENON)
			const size_t nDoubleLimit = 1<<12;
		#else
			const size_t nDoubleLimit = 1<<16;
		#endif
		nMinSize += nSlack;
		size_t nAlloc = 32;
		while (nAlloc < nMinSize)
			nAlloc <<= 1;
		if (nAlloc > nDoubleLimit)
		{
			nAlloc = NArray::max(nAlloc>>3, nDoubleLimit);
			nAlloc = Align(nMinSize, nAlloc);
		}
		return nAlloc - nSlack;
	}

	//---------------------------------------------------------------------------
	// Allocators for DynArray.

	// StandardAlloc: Base DynArray<> allocator.
	// Uses aligned or standard versions of CryModuleMalloc, etc.
	struct StandardAlloc
	{
		union align_type { void* p; double d; };

		static void* alloc( size_t& nNewBytes, bool bSlack = false, int nAlign = 1 )
		{
			assert(nNewBytes);
			if (bSlack)
				nNewBytes = ReallocSize(nNewBytes);
			if (nAlign > alignof(align_type))
				return SPU_MAIN_PTR( CryModuleMemalign(nNewBytes, nAlign) );
			else
				return SPU_MAIN_PTR( CryModuleMalloc(nNewBytes) );
		}

		static void dealloc( void* pMem, int nAlign = 1 )
		{
			if (nAlign > alignof(align_type))
				CryModuleMemalignFree(pMem);
			else
				CryModuleFree(pMem);
		}

		static size_t alloc_size( void* pMem, size_t nMemBytes, int nAlign = 1 )
		{
			if (!pMem)
				return 0;

			nMemBytes = Align(nMemBytes, nAlign);
			if (nAlign > alignof(align_type))
				// Add assumed alignment padding.
				nMemBytes += nAlign;
			return nMemBytes;
		}
	};

	struct GeneralHeapAlloc: StandardAlloc
	{
		IGeneralMemoryHeap* m_pHeap;

		GeneralHeapAlloc()
			: m_pHeap(0)
		{
		}

		explicit GeneralHeapAlloc(IGeneralMemoryHeap* pHeap)
			: m_pHeap(pHeap)
		{
		}

		void* alloc( size_t& nNewBytes, bool bSlack = false, int nAlign = 1 )
		{
			if (!m_pHeap)
				return StandardAlloc::alloc(nNewBytes, bSlack, nAlign);

			assert(nNewBytes);
			if (bSlack)
				nNewBytes = ReallocSize(nNewBytes);
			void* p = NULL;
			if (nAlign > alignof(align_type))
				p = m_pHeap->Memalign(nAlign, nNewBytes, "");
			else
				p = m_pHeap->Malloc(nNewBytes, "");
			return SPU_MAIN_PTR(p);
		}

		void dealloc( void* pMem, int nAlign = 1 )
		{
			if (!m_pHeap)
				return StandardAlloc::dealloc(pMem, nAlign);

			m_pHeap->Free(pMem);
		}
	};

#endif // !_SPU_JOB
};

#if !defined(_SPU_JOB)
template<class T, class I = int>
struct FastDynArray: DynArray<T, I, NArray::FastDynStorage<T,I> >
{
};

#endif // !_SPU_JOB

#if !defined(__SPU__) || defined(__CRYCG__)
	#include "CryPodArray.h"
#endif

#endif
