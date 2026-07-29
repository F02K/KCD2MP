#ifndef _SPU_UTIL_H
#define _SPU_UTIL_H

// this file consits of various classes and macros which should help and ease the integration of PPU code to SPU, while
// keeping the PPU code as original and not clustered with #ifdef SPU.
// A full description of each class/macro can be found in the wiki

// forward declaration of traits to allow compile time implemetation selection
struct IsNotSPU{};
struct IsSPU{};

// ======================== //
// == Usefull SPU makros == //
// ======================== //

// SPU_CONST_MAIN_PTR		used to tell CryCG that a constant value(like 0) is a pointer and points to main memory
// SPU_PTR_SELECT				used to allow selecting of an different pointer an SPU than on PPU
// SPU_REF_SELECT				used as the same as SPU_PTR_SELECT but for references
#if defined(__SPU__)
	#define SPU_CONST_MAIN_PTR( value ) SpuConstMainPtr<value>()
	#define SPU_CONST_LOCAL_PTR( value ) SpuConstLocalPtr<value>()
	#define SPU_PTR_SELECT( ppu, spu ) SPU_LOCAL_PTR(spu)
	#define	SPU_REF_SELECT( ppu, spu ) SPU_LOCAL_REF(spu)

	typedef IsSPU SystemTrait;
#else
	#define SPU_CONST_MAIN_PTR( value ) value
	#define SPU_CONST_LOCAL_PTR( value ) value
	#define SPU_PTR_SELECT( ppu, spu ) SPU_LOCAL_PTR(ppu)
	#define	SPU_REF_SELECT( ppu, spu ) SPU_LOCAL_REF(ppu)

	typedef IsNotSPU SystemTrait;
#endif

// ================================================================ //
// == forward declarations of the various implementation classes == //
// ================================================================ //
template<typename Type, bool WriteBack, bool UseMemcpy, typename Trait>				struct SpuStackValueImpl;
template<typename Type, typename Trait>																				struct SpuAyncPreFetchImpl;
template<int NumChunks, typename Trait>																				class SPUChunkTransferFromMainImpl;
template<int ChunkSize, typename Trait>																				class SPUChunkTransferToMainImpl;


// ======================= //
// == Interface classes == //
// ======================= //

// SpuStackValue is a convient template to transfer data on the spu from main memory to the stack,
// which is useful in two cases:
// First to get rid of a second variant of a function, for this just use SpuStackValue to copy a main memory
// value onto the stack to make both function work on spu local (stack) memory.
// Another use is to spare cache lookups if the value is frequently used.
// When using SpuStackValue keep in mind that the stack on the spu is precious since it only has 256kb for code, data, driver and stack!!
//
// using stack value works as following:
// 1. Create a StackValue:							SpuStackValue<MyType> stack_copy( original_object );
// 2. Create a Reference to the value:	MyType &ref = stack_copy;
// 3. Or pass the SpuStackValue Object to a function as a reference, then the implicit conversation operator is used.
// The refence will then point the the copy of the original_object on the stack.
// When a StackValue object goes out of scope the modified object is automaticly copied back to the original.
//
// To futher customize the stackvalue, some policies can be used.
// SpuStackValue<Type, WriteBack, UseMemcpy>
// When Writeback is set to false, no writeback will happen when the stackvalue goes out of scope, this can be used to spare some copy
// operation if the object is accesed read only.
// When UseMemcpy copy is set to true, the object is transfered with a simple memcpy instead of invoking the object copy constructur.
//
// As default Writback is set to true, and UseMemcpy is set to false.
//
// Also if the StackValue is compiled for non-spu system, the copies are spared and only a refence to the original object is forward.
template<typename Type, bool WriteBack = true, bool UseMemcpy = false>
class SpuStackValue
{
public:
	// constructor, obj is a reference to object which should be copied
	SpuStackValue( Type &obj ) : m_impl(obj) {}
	// return a reference to the object(or stack copy in case of SPU)
	operator Type&()		{ return m_impl.getObj(); }
	// access a member of the holded object
	Type* operator->()	{ return &(m_impl.getObj()); }

private:
	// prevent copy and assign
	SpuStackValue( const SpuStackValue &other );
	SpuStackValue& operator=( const SpuStackValue &other );
	
	SpuStackValueImpl<Type, WriteBack, UseMemcpy, SystemTrait> m_impl;
};



// The SpuAsyncPreFetch class can be used to speed up tight loops on the spu which contain many cache misses. This can come by
// iterating over an array of big classes (size > 128 byte) and reading only a single value like a int from it during each iteration.
// Such a situation can be verified by using the spu profiling utilities which then typically show nearly 100% cachemiss for
// this value.
//
// Such a situation can be improved by using an explicit SpuAsyncPrefetch. With this class a valu can be preloaded and used later,
// the class is designed for all datatypes which are smaller than 128 byte with or without alignment.
// It is also possible to interleave two SpuAsync Operations to further improve the situation(for example when the loop is to tight for
// an effective prefetch in the same iteration) with the SpuAsyncPrefetchInterleaved class.
//
// To prevent nasty problems with parallel writes and ensure correct data, the class only supports read-only access, also it must be ensured that
// no other data write is happening while prefetching the source locations, or else wrong data could be read.
template<typename Type>
struct SpuAyncPreFetch
{
	// mDmaId: The dma-id to use for memory transfers
	SpuAyncPreFetch( uint32 nDmaId ) : m_impl(nDmaId)	{ COMPILE_TIME_ASSERT( (sizeof(Type) <= 128) ); }

	// ptr: pointer to the addr of the value to load
	void prefetch( SPU_DOMAIN_MAIN const Type *ptr ) { m_impl.prefetch( SPU_MAIN_PTR(ptr) ); }

	// returns a spu local reference for a prefetched value, automaticly syncs, so calling it a second time without a preceding prefetch
	// will result in a deadlock
	const Type&	load()  	{ return m_impl.load(); }

private:
	SpuAyncPreFetchImpl<Type, SystemTrait> m_impl;
};



// class to eficiently transfer data to spu in chunks
template<int NumChunks>
class SPUChunkTransferFromMain
{
public:
	SPUChunkTransferFromMain( void *pDst, void *pSrc, uint32 nSize, uint32 nBaseDmaId, const uint32 arrChunks[NumChunks] ) :
		m_impl(pDst,pSrc,nSize,nBaseDmaId,arrChunks)
		{}
	void SyncNeededData( void *pCur )					{		m_impl.SyncNeededData( pCur );	}
	void SyncNeededData( uint32 nByteOffset )	{		m_impl.SyncNeededData( nByteOffset );	}

private:
	SPUChunkTransferFromMainImpl<NumChunks, SystemTrait> m_impl;		// class which implements logic for the current compile target
};

template<int ChunkSize>
class SPUChunkTransferToMain
{
public:
	SPUChunkTransferToMain( void *pDst, void *pSrc, uint32 nSize, uint32 nDmaId ) :
		m_impl( (char*)pDst, (char*)pSrc, nSize, nDmaId)		
	{}
	
	void TransferFinishedData( uint32 nByteOffset ) { m_impl.TransferFinishedData( nByteOffset ); }
	void TransferFinishedData( void *pCur ) { m_impl.TransferFinishedData( (char*)pCur ); }
	char *FlushBuffer( uint32 nCurPosition ) { return m_impl.FlushBuffer(nCurPosition); }

private:
	SPUChunkTransferToMainImpl<ChunkSize, SystemTrait> m_impl;
};


// =========================== //
// == other utility classes == //
// =========================== //

// util class to allow declaring a constant pointer(like 0) and assign it a domain on spu code.
template<int value>
struct SpuConstMainPtr
{
	template<typename T>
	ILINE operator T*()
	{
		return SPU_MAIN_PTR( reinterpret_cast<T*>(value));
	}
};

template<int value>
struct SpuConstLocalPtr
{
	template<typename T>
	ILINE operator T*()
	{
		return SPU_LOCAL_PTR( reinterpret_cast<T*>(value));
	}
};

// The SpuAsyncPrefetchInterleaved class is a wrapper around two instances of SpuAsyncPrefetch.
// As the name suggest it allows easier use of interleaved async data transfers.
// The constructer requires the two dma-ids(one for each prefetch) and the address of the first element to load.
// The remaining usage is like with SpuAsyncPrefetch only that a swap method is provided which switches the prefetch instance into
// which data should be loaded and from which data should be read.
template<typename Type>
struct SpuAsyncPrefetchInterleaved
{
	SpuAsyncPrefetchInterleaved( SPU_DOMAIN_MAIN const Type *ptr, uint32 dma_id_1, uint32 dma_id_2 ) :
		m_idx(0),
		m_prefetch_1( dma_id_1 ),
		m_prefetch_2( dma_id_2 )
	{
		// prefetch first value
		m_prefetch_1.prefetch( ptr );
	}

	void prefetch( const Type *ptr )
	{
		m_idx == 1 ? m_prefetch_1.prefetch(ptr) : m_prefetch_2.prefetch(ptr);
	}
	
	const Type& load()
	{
		const Type& ret = (m_idx == 0 ? m_prefetch_1.load() : m_prefetch_2.load() );
		return ret;
	}

	void swap()
	{
		m_idx ^= 1;
	}

private:
	uint32 m_idx;
	SpuAyncPreFetch<Type>		m_prefetch_1;
	SpuAyncPreFetch<Type>		m_prefetch_2;
};


// Double Buffer class for PS3
// allows having two instances of the same object and switching between these.
// On all other architectures only one buffer is provides so that current and next point to the same location.
template<typename Type>
class DoubleBuffer
{
public:
	DoubleBuffer() : m_counter(0) {}

	// for PS3 get the right buffer, all other architectures use only one entry
#if defined(PS3)
	Type& 			getCurrent() 				{	return m_data[ m_counter & 1]; }
	const Type& getCurrent() const	{	return m_data[ m_counter & 1]; }

	Type& getNext()									{ return m_data[ (m_counter+1) & 1]; }
	const Type& getNext() const			{ return m_data[ (m_counter+1) & 1]; }
#else
	Type& 			getCurrent() 				{	return m_data[0]; }
	const Type& getCurrent() const	{	return m_data[0]; }

	Type& getNext()									{	return m_data[0]; }
	const Type& getNext() const			{	return m_data[0]; }
#endif

	void swap()
	{
		++m_counter;
	}

private:
	uint32 m_counter;

	// provide enough memory to store double buffer (or only one object on non ps3 architectures)
#if defined(PS3)
	Type m_data[2];
#else
	Type m_data[1];
#endif

};

// Util class to manage the type of data transfer between SPU and PPU
// to use it, initialize the class, then start the transfer. After starting you must call
// the sync method.
// For the data transfer it tries to use the most effective way.
// When all requiements are meet an full dma asyncon memtransfer will happen. 
// For very small arrays an object-wise copy is used.
// All other cases are using the synchronous memcopy.
template<typename Type>
struct MemcpyManager
{
	MemcpyManager( Type *pSpu, Type *pPpu, uint32 size , uint32 tag )
	{
		m_pSpu = pSpu;
		m_pPpu = pPpu;
		m_size = size;
		m_tag = tag;
		
		// check if size of multiple of 16 and if src/dst is aligned to 16 byte boundaries
		m_useAsyncCopy = true;
		m_useAsyncCopy &= ( ( reinterpret_cast<uint32>(m_pSpu) & 0xF ) == 0 );
		m_useAsyncCopy &= ( ( reinterpret_cast<uint32>(m_pPpu) & 0xF ) == 0 );
		m_useAsyncCopy &= ( ( size & 0xF ) == 0 );
		
	}
	
	ILINE void start_transfer_to_spu()
	{
		if( m_useAsyncCopy )
		{
			memtransfer_from_main( SPU_LOCAL_PTR(m_pSpu), m_pPpu, m_size, m_tag );
		}
		else
		{
			memcpy( SPU_LOCAL_PTR(m_pSpu), m_pPpu, m_size );		
		}
	}
	
	ILINE void start_transfer_from_spu()
	{
		if( m_useAsyncCopy )
		{
			memtransfer_to_main( m_pPpu, SPU_LOCAL_PTR(m_pSpu), m_size, m_tag );
		}
		else
		{
			memcpy( m_pPpu, SPU_LOCAL_PTR(m_pSpu), m_size );
		}
	}
	
	void sync_transfer()
	{
		if( m_useAsyncCopy )
		{
			memtransfer_sync( m_tag );
		}
	}
	
private:
	
	Type *m_pSpu;
	Type *m_pPpu;
	uint32 m_size;
	uint32 m_tag;
	bool m_useAsyncCopy;
};


// a very crycg friendly(super easy code) dynamic memory stack only for plain old data (POD types)
// should be used to store parameters for function calles which are deferred so that they are executed after the spu
#if defined(__SPU__)
SPU_NO_INLINE
#else
inline
#endif
void* SpuStackPushImpl( void *pElems, uint16 &rCapacity, uint16 &rSize, const void *pElement, uint32 nElementSize, uint32 nStartSize )
{
	// ensure that enough memory for push is avaible
	IF( !rCapacity, false )
	{			
		pElems		= CryModuleMalloc(nElementSize*nStartSize);
		rCapacity = nStartSize;
	}

	IF( rSize == rCapacity, false )
	{
		// do manual realloc to prevent running into an assertion
		// triggered by too near memory locations and a conservative memcpy
		void *tmp = CryModuleMalloc(nElementSize * (rCapacity+nStartSize));
		memcpy( SPU_MAIN_PTR(tmp), SPU_MAIN_PTR(pElems), rCapacity * nElementSize);
		CryModuleFree(pElems);
		pElems = tmp;

		rCapacity += nStartSize;
	}

	assert(pElems != NULL );
	assert( rSize < rCapacity );
	char *pDst = (char*)pElems + (rSize*nElementSize);
	memcpy(pDst, pElement, nElementSize);
	rSize += 1;
	return pElems;
}

template<typename Type, int nStartSize = 8>
class SpuDataStack
{
public:
	SpuDataStack() : m_pElems(NULL), m_nSize(0), m_nCapacity(0) {}
	~SpuDataStack() { IF( m_pElems!=0, true ) CryModuleFree(m_pElems); }

	bool empty() const { return m_nSize == 0; }
	void push( const Type &element )
	{
		m_pElems = (Type*)SpuStackPushImpl( m_pElems, m_nCapacity, m_nSize, &element, sizeof(Type), nStartSize );		
	}

	Type&				pop()				{ return m_pElems[(m_nSize--) - 1]; }
#if defined(__clang__) || defined(__GNUC__)
	const Type& pop() const {
		uint16* sizePtr=const_cast<uint16*>(&m_nSize);
		(*sizePtr)--;
		return m_pElems[*sizePtr];
	}
#else
	const Type& pop() const { return m_pElems[(const_cast<uint16>(m_nSize)--) - 1]; }
#endif
  const Type& operator [] (int i) const { assert( i>=0 && i<m_nSize ); return m_pElems[i]; }
  uint16 size() const	{ return m_nSize; }
	
	void clear() { IF( m_pElems!=0, true ) CryModuleFree(m_pElems); m_pElems = NULL; m_nSize = 0; m_nCapacity = 0;}

private:
	Type		*m_pElems;
	uint16	m_nSize;
	uint16	m_nCapacity;	
};


// ==================================== //
// == implementation classes for SPU == //
// ==================================== //

// SpuStackValue Implementation for SPU which uses Memcpy
// Should only be used with structures which are mem-copyable.
template<typename Type, bool WriteBack>
struct SpuStackValueImpl<Type, WriteBack, true, IsSPU>
{
	SpuStackValueImpl( Type &obj ) : m_obj(&obj)	
	{ 
		memcpy( m_spu_obj, m_obj, sizeof(Type) );
	}
	~SpuStackValueImpl()													
	{ 
		if( WriteBack ) 
		{ 
			memcpy( m_obj, m_spu_obj, sizeof(Type) );
		} 
	}	
	Type& getObj()																{ return *reinterpret_cast<Type*>(m_spu_obj); }
	
private:
	// use maximum (128byte boundaries) aligned memory to improve memory transfers and overcome a gcc limitation (template parameters can`t be passed to to align attribute)
	char m_spu_obj[sizeof(Type)] _ALIGN(128);
	Type *m_obj;
};


// SpuStackValue Implementation for SPU
// Uses the objects copy and assign operators
template<typename Type, bool WriteBack>
struct SpuStackValueImpl<Type, WriteBack, false, IsSPU>
{
	SpuStackValueImpl( Type &obj ) : m_obj(&obj), m_spu_obj(obj)	{ }
	~SpuStackValueImpl()																					{ if( WriteBack ) *m_obj = m_spu_obj; }
	Type& getObj()																								{ return m_spu_obj; }
	
private:
	Type m_spu_obj;
	Type *m_obj;
};


// SpuAyncPreFetch implementation class for SPU.
template<typename Type>
struct SpuAyncPreFetchImpl<Type, IsSPU>
{
	SpuAyncPreFetchImpl( uint32 nDmaId ) : m_nDmaId(nDmaId), m_bNeedSync(false) {}
	
	~SpuAyncPreFetchImpl()
	{
		// sync all transfers which all still open, to prevent old pending transfers
		if( m_bNeedSync )
		{			
			memtransfer_sync( m_nDmaId );
		}
	}

	void prefetch( const Type *ptr )
	{
		// compute base addresse
		uint32 base = (uint32)ptr & ~0x7F;
		m_offset = (uint32)ptr & 0x7F;
		
		// start transfer
		memtransfer_from_main( SPU_LOCAL_PTR(m_buffer), SPU_MAIN_PTR( (void*)(EXPAND_PTR)base ), 256, m_nDmaId );
		
		m_bNeedSync = true;
	}
	
	const Type& load()
	{
		//NOTE: this also works when the Type has alignment since the buffer is aligned and the offset is
		// the same as in the main memory
		memtransfer_sync( m_nDmaId );
		m_bNeedSync = false;
		Type *value = (Type*)&m_buffer[m_offset];
		return *value;
	}
	
private:
	// use 128 bye aligned buffer for maximum transfer speed and to guarantee alignment.
	char m_buffer[256] _ALIGN(128);
	uint32 m_offset;
	uint32 m_nDmaId;
	bool m_bNeedSync;
};


// SPUChunkTransferToMain implementation for SPUs
template<int ChunkSize>
class SPUChunkTransferToMainImpl<ChunkSize, IsSPU>
{
public:
	SPUChunkTransferToMainImpl( char *pDst, char *pSrc, uint32 nSize, uint32 nDmaId ) :
		m_pDst(pDst),
		m_pSrc(pSrc),
		m_nSize(nSize),
		m_nDmaId(nDmaId),
		m_nOffset(0),
		m_nVirtualSrcOffset(0)
	{}

	~SPUChunkTransferToMainImpl()
	{
		// transfer the remaining data syncronly
		memcpy( SPU_MAIN_PTR(m_pDst + m_nOffset), SPU_LOCAL_PTR(m_pSrc + m_nOffset - m_nVirtualSrcOffset), m_nSize - m_nOffset );

		// sync all remaining async transfers
		memtransfer_sync( m_nDmaId );
	}

	void TransferFinishedData( uint32 nByteOffset )
	{
		if( nByteOffset > m_nOffset + ChunkSize )
		{
			memtransfer_to_main( SPU_MAIN_PTR(m_pDst + m_nOffset), SPU_LOCAL_PTR(m_pSrc + m_nOffset - m_nVirtualSrcOffset), ChunkSize, m_nDmaId );
			m_nOffset += ChunkSize;
		}
	}

	char *FlushBuffer( uint32 nCurPosition )
	{		
		// sync all transfers
		memtransfer_sync( m_nDmaId );
		
		// copy remaining data to the buffer beginning
		memcpy( SPU_LOCAL_PTR(m_pSrc), SPU_LOCAL_PTR(m_pSrc + m_nOffset - m_nVirtualSrcOffset), nCurPosition - m_nOffset );
		
		// special offset to produce "virtual" ptrs before our valid memory
		// the client code will use an offset directly into this one
		m_nVirtualSrcOffset += m_nOffset - m_nVirtualSrcOffset;
		
		return m_pSrc - m_nVirtualSrcOffset;		
	}
private:
	char *m_pDst;
	char *m_pSrc;
	uint32 m_nSize;
	uint32 m_nDmaId;
	uint32 m_nOffset;
	uint32 m_nVirtualSrcOffset;
};

// SPUChunkTransferFromMain implementation for SPU
template<int NumChunks>
class SPUChunkTransferFromMainImpl<NumChunks, IsSPU>
{
public:
	SPUChunkTransferFromMainImpl( char *pDst, char *pSrc, uint32 nSize, uint32 nBaseDmaId, const uint32 arrChunks[NumChunks] ) :
		m_pSrc(pSrc),
		m_pDst(pDst),
		m_nSize(nSize),
		m_nBaseDmaId(nBaseDmaId),
		m_nSyncOffset(0),
		m_nCurChunk(0)
	{
		// copy chunks sizes into member variables
		for( uint32 i = 0 ; i < NumChunks ; ++i )
			m_arrChunks[i] = arrChunks[i];

		// start transfer of first chunk (we read always this much data, even when we read over the end of the ppu data)
		memtransfer_from_main( SPU_LOCAL_PTR(m_pDst), SPU_MAIN_PTR(m_pSrc), m_arrChunks[0], m_nBaseDmaId );

		uint32 nSizeOffset =  m_arrChunks[0];

		// start transfers for all needed chunks
		for( uint32 i = 1 ; i < NumChunks ; ++i )
		{
			if( m_nSize > nSizeOffset )
			{
					memtransfer_from_main( SPU_LOCAL_PTR(m_pDst + nSizeOffset), SPU_MAIN_PTR(m_pSrc + nSizeOffset), m_arrChunks[i], m_nBaseDmaId + i );
					nSizeOffset += m_arrChunks[i];
			}
		}

		// start transfer of remaining data(this needs to be seperate to ensure that the size of the remaining data is a multiple of 128)
		if( m_nSize > nSizeOffset )
		{
			uint32 nRemainingSize = m_nSize - nSizeOffset;
	
			// align remaing size to 128
			nRemainingSize += 128;
			nRemainingSize &= ~0x7F;
			memtransfer_from_main( SPU_LOCAL_PTR(m_pDst + nSizeOffset), SPU_MAIN_PTR(m_pSrc + nSizeOffset), nRemainingSize, m_nBaseDmaId + NumChunks );
		}
		
	}

	void SyncNeededData( char *pCur )
	{
		SyncNeededData( pCur - m_pSrc );
	}

	void SyncNeededData( uint32 nByteOffset )
	{
		if( nByteOffset >= m_nSyncOffset )
		{			
			memtransfer_sync( m_nBaseDmaId + m_nCurChunk );

			if( m_nCurChunk == NumChunks )
				m_nSyncOffset = 0xFFFFFFFF; // mark that we synced all chunks
			else
			{
				m_nSyncOffset += m_arrChunks[m_nCurChunk];
				++m_nCurChunk;
			}
		}
	}
private:
	char *m_pSrc;
	char *m_pDst;
	uint32 m_nSize;
	uint32 m_nBaseDmaId;
	uint32 m_nSyncOffset;
	uint32 m_nCurChunk;
	uint32 m_arrChunks[NumChunks];
};


// ====================================================== //
// == Dummy-implementation classes for non-SPU systems == //
// ====================================================== //

// Implementation for non-spu code, just forwards the original object.
template<typename Type, bool UseMemcpy, bool WriteBack>
struct SpuStackValueImpl<Type,WriteBack,UseMemcpy, IsNotSPU>
{
	SpuStackValueImpl( Type &obj ) : m_obj(&obj)		{}
	Type& getObj()																	{ return *m_obj; }

private:
	Type *m_obj;
};

// Spu AsyncPreFetch implementation for non-spu systems.
template<typename Type>
struct SpuAyncPreFetchImpl<Type, IsNotSPU>
{
	SpuAyncPreFetchImpl( uint32 nDmaId	)	{}
	void prefetch( const Type *ptr )			{ m_ptr = ptr; }
	const Type& load()										{ return *m_ptr; }
	
private:
	const Type *m_ptr;
};


// SPUChunkTransferFromMain implementation for non-spu systems.
template<int NumChunks>
class SPUChunkTransferFromMainImpl<NumChunks, IsNotSPU>
{
public:
	SPUChunkTransferFromMainImpl( void *pDst, void *pSrc, uint32 nSize, uint32 nBaseDmaId, const uint32 arrChunks[NumChunks] ) {}
	void SyncNeededData( void *pCur )					{}
	void SyncNeededData( uint32 nByteOffset )	{}
};


// SPUChunkTransferToMain implementation for non-spu systems.
template<int ChunkSize>
class SPUChunkTransferToMainImpl<ChunkSize, IsNotSPU>
{
public:
	SPUChunkTransferToMainImpl( char *pDst, char *pSrc, uint32 nSize, uint32 nDmaId )	{}
	void TransferFinishedData( uint32 nByteOffset )	{}
	void TransferFinishedData( char *pCur ) {}
	char *FlushBuffer( uint32 nCurPosition ) { return NULL; }
};


















































































































































































































































#endif // _SPU_UTIL_H
