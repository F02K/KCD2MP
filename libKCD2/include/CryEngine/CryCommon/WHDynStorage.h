#pragma once
// KCD-RE patch: WHGame.dll's retail DynArray storage policy.  Included by CryArray.h
// (between the NArray storage schemes and DynArrayRef); not standalone -- it uses
// NArray::RawStorage / move_init / min and NAlloc::ReallocSize from there.
#ifndef _CRY_ARRAY_H_
#	error "WHDynStorage.h is internal to CryArray.h -- include CryArray.h instead"
#endif

// WHGame.dll bridge (defined in CrySDKStubs.cpp).
extern "C"
{
	void* WHDynArrayAlloc( size_t nBytes, size_t* nActualBytes );
	void  WHDynArrayFree( void* pBlockBase );
	void* WHDynArrayDefaultAllocFn();
}

namespace NArray
{
	//---------------------------------------------------------------------------
	// WHDynStorage: WHGame.dll's retail DynArray storage (NArray store-allocator).
	// Single pointer member; heap block:
	//   [D-16: realloc-fn ptr][D-8: capacity u32][D-4: count u32][D: elements]
	// bucket-allocated and stamped with the game's default allocator fn, so blocks are
	// growable/freeable by either side (e.g. the game fills SUIArguments::m_FlashVars
	// inside our stack objects; our dtor frees it).

	template<class T, class I = int>
	struct WHDynStorage: RawStorage<T>
	{
		typedef WHDynStorage<T,I> self_type;
		typedef I size_type;

		static_assert(alignof(T) <= 8, "game block puts elements at base+16 (8-aligned)");

		struct Prefix
		{
			void*		pAllocFn;		// game realloc fn (default sub_1804A2C50)
			uint32	nCap;
			uint32	nCount;
		};

		WHDynStorage()
			{ set_null(); }

		~WHDynStorage()
		{
			if (!is_null())
				WHDynArrayFree(prefix());
		}

		void swap(self_type& a)
		{
			T* pTemp = m_aElems;
			m_aElems = a.m_aElems;
			a.m_aElems = pTemp;
		}

		// Basic storage.
		CONST_VAR_FUNCTION( T* begin(),
			{ return m_aElems; } )
		inline size_type size() const
			{ return (size_type)prefix()->nCount; }
		inline size_type capacity() const
			{ return (size_type)prefix()->nCap; }
		inline size_type get_alloc_size() const
			{ return is_null() ? 0 : (size_type)(sizeof(Prefix) + prefix()->nCap * sizeof(T)); }

		SPU_NO_INLINE void resize_raw( size_type new_size, size_type new_cap, bool allow_slack = false )
		{
			assert(new_cap >= new_size);
			if (new_cap != capacity())
			{
				T* old_elems = is_null() ? 0 : m_aElems;
				size_type old_size = size();

				T* new_elems = 0;
				if (new_cap > 0)
				{
					size_t nBytes = sizeof(Prefix) + (size_t)new_cap * sizeof(T);
					if (allow_slack)
						nBytes = NAlloc::ReallocSize(nBytes);
					size_t nActual = 0;
					if (Prefix* pNew = (Prefix*)WHDynArrayAlloc(nBytes, &nActual))
					{
						pNew->pAllocFn = WHDynArrayDefaultAllocFn();
						new_cap = (size_type)((nActual - sizeof(Prefix)) / sizeof(T));	// keep bucket slack, like the game
						new_elems = (T*)(pNew + 1);
						if (old_elems)
							NArray::move_init(new_elems, old_elems, NArray::min(old_size, new_size));
					}
				}
				if (old_elems)
					WHDynArrayFree((Prefix*)old_elems - 1);
				if (!new_elems)
				{
					set_null();
					return;
				}
				m_aElems = new_elems;
			}
			set_sizes(new_size, new_cap);
		}

		void resize_raw( size_type new_size )
		{
			size_type cap = capacity();
			if (new_size <= cap)
				set_sizes(new_size, cap);
			else
				resize_raw(new_size, new_size, cap != 0);
		}

	protected:

		T*				m_aElems;

		CONST_VAR_FUNCTION( Prefix* prefix(),
			{ return ((Prefix*)m_aElems)-1; } )

		// Empty test is cap==0 (the game's own: it masks such blocks out of every
		// free/realloc).  Never compare the ADDRESS -- the game parks its own sentinel
		// (&unk_185665498 family) in our member when it empties an array.
		inline bool is_null() const
			{ return prefix()->nCap == 0; }

		void set_sizes( size_type s, size_type c )
		{
			if (c)		// never write a shared empty sentinel (ours or the game's)
			{
				prefix()->nCap = (uint32)c;
				prefix()->nCount = (uint32)s;
			}
		}

		void set_null()
		{
			static Prefix s_EmptyPrefix;		// zero block: fn 0, cap 0, count 0
			m_aElems = (T*)(&s_EmptyPrefix + 1);
		}
	};
};
