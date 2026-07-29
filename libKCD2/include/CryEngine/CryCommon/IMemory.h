/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2009.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: 
  
 -------------------------------------------------------------------------
  History:
  - 11:5:2009   : Created by Andrey Honich

*************************************************************************/
#include DEVIRTUALIZE_HEADER_FIX(IMemory.h)

#ifndef __IMEMORY_H__
#define __IMEMORY_H__

#if _MSC_VER > 1000
#	pragma once
#endif

#include <IDefragAllocator.h> // <> required for Interfuscator
#include <IGeneralMemoryHeap.h> // <> required for Interfuscator

struct IMemoryBlock : public CMultiThreadRefCount
{
	// <interfuscator:shuffle>
  virtual void * GetData() = 0;
  virtual int GetSize() = 0;
	// </interfuscator:shuffle>
};
TYPEDEF_AUTOPTR(IMemoryBlock);

//////////////////////////////////////////////////////////////////////////
struct ICustomMemoryBlock : public IMemoryBlock
{
	// Copy region from from source memory to the specified output buffer
	virtual void CopyMemoryRegion( void *pOutputBuffer,size_t nOffset,size_t nSize ) = 0;
};

//////////////////////////////////////////////////////////////////////////
struct ICustomMemoryHeap : public CMultiThreadRefCount
{
	// <interfuscator:shuffle>
	virtual ICustomMemoryBlock* AllocateBlock( size_t nAllocateSize,const char *sUsage ) = 0;
	virtual void AllocateHeap( size_t const nSize,char const* const sUsage ) = 0;
	virtual void GetMemoryUsage( ICrySizer *pSizer ) = 0;
	virtual size_t GetAllocated() = 0;
	virtual void Defrag() = 0;
	// </interfuscator:shuffle>
};

class IMemoryAddressRange
{
public:
	// <interfuscator:shuffle>
	virtual void Release() = 0;

	virtual char* GetBaseAddress() const = 0;
	virtual size_t GetPageCount() const = 0;
	virtual size_t GetPageSize() const = 0;

	virtual void* MapPage(size_t pageIdx) = 0;
	virtual void UnmapPage(size_t pageIdx) = 0;
	// </interfuscator:shuffle>
protected:
	virtual ~IMemoryAddressRange() {}
};

class IPageMappingHeap
{
public:
	// <interfuscator:shuffle>
	virtual void Release() = 0;

	virtual size_t GetGranularity() const = 0;
	virtual bool IsInAddressRange(void* ptr) const = 0;

	virtual size_t FindLargestFreeBlockSize() const = 0;

	virtual void* Map(size_t sz) = 0;
	virtual void Unmap(void* ptr, size_t sz) = 0;
	// </interfuscator:shuffle>
protected:
	virtual ~IPageMappingHeap() {}
};

#endif //__IMEMORY_H__
