#ifndef __PAGED_BUFFER_H__
#define __PAGED_BUFFER_H__

// Fast implementation of POD buffer. It grows when a write off the end of the buffer
// causes an page fault meaning the writing code doesn't need to do any checks at all.
// Buffer also automatically trims if rewound PAGED_TICKS_UNTIL_TRIM times
// without using the whole of the buffer
// 2010 Charlie Cole

#ifdef PS3
#define PAGED_TICKS_UNTIL_TRIM	128
#define PAGED_PAGE_SIZE					(64*1024)
#define PAGED_SEGMENT_SIZE			(256*1024*1024)
#define PAGED_BUFFER_MAX_PAGES	(PAGED_SEGMENT_SIZE/PAGED_PAGE_SIZE)

class CPagedBufferManager
{
public:
	CPagedBufferManager() { m_bInitialised=false; }
	bool Init();
	void* AllocAddressSpace(size_t size);
	void FreeAddressSpace(uint8 *vAddress, size_t size);
	bool MapPhysicalPages(uint8 *vAddress, size_t size);
	bool UnmapPhysicalPages(uint8 *vAddress, size_t size);
	bool RegisterPageFaultCallback(class CPagedBuffer *pagedBuffer);
	bool DeregisterPageFaultCallback(class CPagedBuffer *pagedBuffer);

private:
	static void PagedBufferHandler(uint64 arg);
	uint8 PageMapped(int idx)
	{
		return m_allocatedPagesBitMap[idx/8]&(1<<(idx&7));
	}
	void SetPageMapped(int idx)
	{
		m_allocatedPagesBitMap[idx/8]|=(1<<(idx&7));
	}
	void ClearPageMapped(int idx)
	{
		m_allocatedPagesBitMap[idx/8]&=~(1<<(idx&7));
	}

	bool m_bInitialised;
	sys_ppu_thread_t m_pagedBufferThread;
	sys_event_queue_t	m_pagedBufferQueue;
	static CPagedBuffer* m_pagedBufferCallback[1024];
	static int m_numPagedBufferCallbacks;
	void *m_pagedBufferBaseAddress;
	uint8 m_allocatedPagesBitMap[512];
};

class CPagedBuffer
{
public:
	explicit CPagedBuffer(size_t maxSize, size_t initialSize=0, size_t growDelta=PAGED_PAGE_SIZE)
	{
		s_pbm.Init();
		m_startAddress=(uint8*)s_pbm.AllocAddressSpace(maxSize+PAGED_PAGE_SIZE); // Unmapped guard page needed to trap overrun
		if (!m_startAddress)
		{
			CryFatalError("Couldn't allocate address space for paged buffer");
		}
		if (!s_pbm.MapPhysicalPages(m_startAddress, initialSize))
		{
			CryFatalError("Couldn't map default size for paged buffer");
		}
		m_maxAddress=m_startAddress+maxSize;
		m_endAddress=m_startAddress+initialSize;
		m_ptr=m_startAddress;
		m_growDelta=growDelta;
		m_maxUsedAddress=m_startAddress;
		m_ticksToTrim=PAGED_TICKS_UNTIL_TRIM;
		if (!s_pbm.RegisterPageFaultCallback(this))
		{
			CryFatalError("Couldn't register page fault callback");
		}
	}

	~CPagedBuffer()
	{
		if (!s_pbm.DeregisterPageFaultCallback(this))
		{
			CryFatalError("Couldn't register page fault callback");
		}
		free();
		s_pbm.FreeAddressSpace(m_startAddress, m_maxAddress-m_startAddress);
	}

	void rewind()
	{
		trim();
		m_ptr=m_startAddress;
	}
	
	size_t offset()
	{
		return m_ptr-m_startAddress;
	}
	
	size_t capacity()
	{
		return m_endAddress-m_startAddress;
	}

	template<typename T>
	void write(const T &d)
	{
		*(T*)m_ptr=d;
		m_ptr+=sizeof(T);
	}
	
	const void* writeData(const void *pSrc, size_t size)
	{
		memcpy(m_ptr, pSrc, size);
		m_ptr+=size;
	}

	template<typename T>
	const T& read()
	{
		uint8* ptr=m_ptr;
		m_ptr+=sizeof(T);
		return *(T*)ptr;
	}
	
	const void* readData(size_t size)
	{
		uint8* ptr=m_ptr;
		m_ptr+=size;
		return (const void*)ptr;
	}

	void free()
	{
		if (!s_pbm.UnmapPhysicalPages(m_startAddress, m_endAddress-m_startAddress))
		{
			CryFatalError("Error unmapping paged buffer");
		}
		m_endAddress=m_startAddress;
		m_ptr=m_startAddress;
	}

	static bool PageFaultCallback(void* address, void* userData)
	{
		CPagedBuffer *buffer=(CPagedBuffer*)userData;
		if (address<buffer->m_startAddress || address>=buffer->m_maxAddress)
		{
			return false;
		}
		while (buffer->m_endAddress<=address)
		{
			if (!s_pbm.MapPhysicalPages(buffer->m_endAddress, buffer->m_growDelta))
			{
				printf("Couldn't allocate space to deal with page fault");
				return false;
			}
			buffer->m_endAddress+=buffer->m_growDelta;
		}
		return true;
	}

private:

	CPagedBuffer(const CPagedBuffer &buffer);
	CPagedBuffer& operator =(const CPagedBuffer &buffer);
	
	void trim()
	{
		if (m_ptr>m_maxUsedAddress)
		{
			m_maxUsedAddress=m_ptr;
		}
		if ((--m_ticksToTrim)==0)
		{
			m_maxUsedAddress-=(UINT_PTR)m_startAddress;
			m_maxUsedAddress=(uint8*)(((UINT_PTR)m_maxUsedAddress+(m_growDelta-1))&~(m_growDelta-1));
			m_maxUsedAddress+=(UINT_PTR)m_startAddress;
			if (m_maxUsedAddress<m_endAddress)
			{
				if (!s_pbm.UnmapPhysicalPages(m_maxUsedAddress, m_endAddress-m_maxUsedAddress))
				{
					CryFatalError("Error trimming paged buffer");
				}
				m_endAddress=m_maxUsedAddress;
			}
			m_maxUsedAddress=m_startAddress;
			m_ticksToTrim=PAGED_TICKS_UNTIL_TRIM;
		}
	}

	uint8* __restrict m_ptr;
	uint8 *m_startAddress;
	uint8 *m_endAddress;
	uint8 *m_maxAddress;
	uint8 *m_maxUsedAddress;
	int m_ticksToTrim;
	size_t m_growDelta;

	static CPagedBufferManager s_pbm;
};
#endif

#endif // __PAGED_BUFFER_H__