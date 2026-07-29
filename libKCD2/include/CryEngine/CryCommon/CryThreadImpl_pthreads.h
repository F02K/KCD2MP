/////////////////////////////////////////////////////////////////////////////
//
// Crytek Source File
// Copyright (C), Crytek Studios, 2001-2006.
//
// History:
// Jun 20, 2006: Created by Sascha Demetrio
//
/////////////////////////////////////////////////////////////////////////////

#include "CryThread_pthreads.h"

#ifndef __SPU__
#if PLATFORM_SUPPORTS_THREADLOCAL
THREADLOCAL CrySimpleThreadSelf
	*CrySimpleThreadSelf::m_Self = NULL;
#else
TLS_DEFINE(CrySimpleThreadSelf*, g_CrySimpleThreadSelf)
#endif
#endif
// vim:ts=2


//////////////////////////////////////////////////////////////////////////
// CryEvent(Timed) implementation
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CryEventTimed::Reset()
{
	m_lockNotify.Lock();
	m_flag = false;
	m_lockNotify.Unlock();
}

//////////////////////////////////////////////////////////////////////////
void CryEventTimed::Set()
{
	m_lockNotify.Lock();
	m_flag = true;
	m_cond.Notify();
	m_lockNotify.Unlock();
}

//////////////////////////////////////////////////////////////////////////
void CryEventTimed::Wait()
{
	m_lockNotify.Lock();
	if (!m_flag)
		m_cond.Wait(m_lockNotify);
	m_flag	=	false;
	m_lockNotify.Unlock();
}

//////////////////////////////////////////////////////////////////////////
bool CryEventTimed::Wait( const uint32 timeoutMillis )
{
	bool bResult = true;
	m_lockNotify.Lock();
	if (!m_flag)
		bResult = m_cond.TimedWait(m_lockNotify,timeoutMillis);
	m_flag	=	false;
	m_lockNotify.Unlock();
	return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// CryCriticalSection implementation
///////////////////////////////////////////////////////////////////////////////
typedef CryLockT<CRYLOCK_RECURSIVE> TCritSecType;

void  CryDeleteCriticalSection( void *cs )
{
	delete ((TCritSecType *)cs);
}

void  CryEnterCriticalSection( void *cs )
{
	((TCritSecType*)cs)->Lock();
}

bool  CryTryCriticalSection( void *cs )
{
	return false;
}

void  CryLeaveCriticalSection( void *cs )
{
	((TCritSecType*)cs)->Unlock();
}

void  CryCreateCriticalSectionInplace(void* pCS)
{
	new (pCS) TCritSecType;
}

void CryDeleteCriticalSectionInplace( void *)
{
}

void* CryCreateCriticalSection()
{
	return (void*) new TCritSecType;
}

#if defined(PS3) && !defined (ORBIS)
#if !defined(__SPU__)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list,  SLockFreeSingleLinkedListEntry &element )
{
	STATIC_CHECK(sizeof(SLockFreeSingleLinkedListHeader) == sizeof(SLockFreeSingleLinkedListEntry*), CRY_INTERLOCKED_SLIST_HEADER_HAS_WRONG_SIZE);
	STATIC_CHECK(sizeof(SLockFreeSingleLinkedListEntry) == sizeof(SLockFreeSingleLinkedListEntry*), CRY_INTERLOCKED_SLIST_ENTRY_HAS_WRONG_SIZE);		

	SLockFreeSingleLinkedListEntry *pCurrentTop = NULL;
	volatile void *pHeader = alias_cast<volatile void*>(&list);
	__lwsync();
	do
	{
		pCurrentTop = (SLockFreeSingleLinkedListEntry*)( __lwarx(pHeader) );
		element.pNext = pCurrentTop;
	}
	while (!__stwcx(pHeader, (long)&element));
}

//////////////////////////////////////////////////////////////////////////
void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list )
{
	STATIC_CHECK(sizeof(SLockFreeSingleLinkedListHeader) == sizeof(SLockFreeSingleLinkedListEntry*), CRY_INTERLOCKED_SLIST_HEADER_HAS_WRONG_SIZE);

	struct SFreeList{ SFreeList *pNext; };
	volatile SLockFreeSingleLinkedListEntry *pCurrentTop = NULL;
	volatile SLockFreeSingleLinkedListEntry *pNext = NULL;
	volatile void *pHeader = alias_cast<volatile void*>(&list);
	do
	{
		pCurrentTop = (SLockFreeSingleLinkedListEntry*)( __lwarx(pHeader) );
		if(pCurrentTop == NULL)
			return NULL;

		pNext = pCurrentTop->pNext;
	}
	while (!__stwcx(pHeader, (long)pNext));
	__lwsync();
	return (void*)pCurrentTop;
}

//////////////////////////////////////////////////////////////////////////
void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list)
{
#if !defined(__SPU__)
	STATIC_CHECK(sizeof(SLockFreeSingleLinkedListHeader) == sizeof(SLockFreeSingleLinkedListEntry*), CRY_INTERLOCKED_SLIST_HEADER_HAS_WRONG_SIZE);
	list.pNext = NULL;
#endif 
}

//////////////////////////////////////////////////////////////////////////
void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list)
{
	STATIC_CHECK(sizeof(SLockFreeSingleLinkedListHeader) == sizeof(SLockFreeSingleLinkedListEntry*), CRY_INTERLOCKED_SLIST_HEADER_HAS_WRONG_SIZE);
	STATIC_CHECK(sizeof(SLockFreeSingleLinkedListHeader) == sizeof(SLockFreeSingleLinkedListEntry*), CRY_INTERLOCKED_SLIST_HEADER_HAS_WRONG_SIZE);

	struct SFreeList{ SFreeList *pNext; };
	SLockFreeSingleLinkedListEntry *pCurrentTop = NULL;
	SLockFreeSingleLinkedListEntry *pNext = NULL;
	volatile void *pHeader = alias_cast<volatile void*>(&list);
	do
	{
		pCurrentTop = (SLockFreeSingleLinkedListEntry*)( __lwarx(pHeader) );
		if(pCurrentTop == NULL)
			return NULL;		
	}
	while (!__stwcx(pHeader, (long)0));
	__lwsync();
	return pCurrentTop;
}

#else // for SPU forward to driver implemented functions
inline void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry &element ) 
{ 
	CryInterlockedPushEntrySListSPU( SPU_MAIN_PTR((void*)&list), SPU_MAIN_PTR((void*)&element)); 
}

inline SPU_DOMAIN_MAIN void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list )
{ 
	return SPU_MAIN_PTR( CryInterlockedPopEntrySListSPU( SPU_MAIN_PTR((void*)&list) ) ); 
}

inline void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list) 
{ 
	CryInitializeSListHeadSPU( SPU_MAIN_PTR((void*)&list) ); 
}

inline SPU_DOMAIN_MAIN void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list) 
{
	return SPU_MAIN_PTR( CryInterlockedFlushSListSPU( SPU_MAIN_PTR((void*)&list) ) );
}

#endif
//////////////////////////////////////////////////////////////////////////
// LINUX64 Implementation of Lockless Single Linked List
//////////////////////////////////////////////////////////////////////////
#elif defined(LINUX64) || defined(MAC)
	typedef __uint128_t uint128;

	//////////////////////////////////////////////////////////////////////////
	// Implementation for Linux64 with gcc using __int128_t
	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list,  SLockFreeSingleLinkedListEntry &element )
	{
		uint64 curSetting[2];
		uint64 newSetting[2];
		uint64 newPointer = (uint64)&element;
		do 
		{
			curSetting[0] = (uint64)list.pNext;
			curSetting[1] = list.salt;
			element.pNext = (SLockFreeSingleLinkedListEntry*)curSetting[0];
			newSetting[0] = newPointer; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		// while (false == __sync_bool_compare_and_swap( (volatile uint128*)&list.pNext,*(uint128*)&curSetting[0],*(uint128*)&newSetting[0] ));
	   while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0])); 
    }

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list )
	{
		uint64 curSetting[2];
		uint64 newSetting[2];
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint64)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = *(uint64*)curSetting[0]; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
	    //while (false == __sync_bool_compare_and_swap( (volatile uint128*)&list.pNext,*(uint128*)&curSetting[0],*(uint128*)&newSetting[0] ));
	   while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0])); 
		return (void*)curSetting[0];
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list)
	{
		list.salt = 0;
		list.pNext = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list)
	{
		uint64 curSetting[2];
		uint64 newSetting[2];
		uint64 newSalt;
		uint64 newPointer;
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint64)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = 0;
			newSetting[1] = curSetting[1]+1;
		}
	//	while (false == __sync_bool_compare_and_swap( (volatile uint128*)&list.pNext,*(uint128*)&curSetting[0],*(uint128*)&newSetting[0] ));
	   while (0 == _InterlockedCompareExchange128((volatile int64*)&list.pNext, (int64)newSetting[1], (int64)newSetting[0], (int64*)&curSetting[0])); 
		return (void*)curSetting[0];
	}
	//////////////////////////////////////////////////////////////////////////
#elif defined(LINUX32)
	//////////////////////////////////////////////////////////////////////////
	// Implementation for Linux32 with gcc using uint64
	//////////////////////////////////////////////////////////////////////////
	void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list,  SLockFreeSingleLinkedListEntry &element )
	{
		uint32 curSetting[2];
		uint32 newSetting[2];
		uint32 newPointer = (uint32)&element;
		do 
		{
			curSetting[0] = (uint32)list.pNext;
			curSetting[1] = list.salt;
			element.pNext = (SLockFreeSingleLinkedListEntry*)curSetting[0];
			newSetting[0] = newPointer; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list )
	{
		uint32 curSetting[2];
		uint32 newSetting[2];
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint32)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = *(uint32*)curSetting[0]; // new pointer
			newSetting[1] = curSetting[1]+1; // new salt
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
		return (void*)curSetting[0];
	}

	//////////////////////////////////////////////////////////////////////////
	void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list)
	{
		list.salt = 0;
		list.pNext = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list)
	{
		uint32 curSetting[2];
		uint32 newSetting[2];
		uint32 newSalt;
		uint32 newPointer;
		do 
		{
			curSetting[1]=list.salt;
			curSetting[0]=(uint32)list.pNext;
			if (curSetting[0]==0)
				return NULL;
			newSetting[0] = 0;
			newSetting[1] = curSetting[1]+1;
		}
		while (false == __sync_bool_compare_and_swap( (volatile uint64*)&list.pNext,*(uint64*)&curSetting[0],*(uint64*)&newSetting[0] ));
		return (void*)curSetting[0];
	}
	//////////////////////////////////////////////////////////////////////////
#endif
