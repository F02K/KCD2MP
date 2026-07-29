////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2005.
// -------------------------------------------------------------------------
//  File name:   MultiThread.h
//  Version:     v1.00
//  Compilers:   Visual Studio.NET 2003
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MultiThread_h__
#define __MultiThread_h__
#pragma once

#if defined(PS3) && !defined(__SPU__)
#include <semaphore.h>
#endif

#include "CryAssert.h"

#define WRITE_LOCK_VAL (1<<16)

#if defined(PS3) && defined(PS3_PROFILE_LOCKS)
extern int g_nLockOpCount;
extern int g_nLockContentionCount;
extern int g_nLockContentionCost;
#endif

//as PowerPC operates via cache line reservation, lock variables should reside ion their own cache line
template <class T>
struct SAtomicVar
{
	T val;
#if defined(PS3) || defined(XENON)
	char pad[128-sizeof(T)];
#endif
	inline operator T()const{return val;}
	inline operator T()volatile const{return val;}
	inline void operator =(const T& rV){val = rV;return *this;}
	inline void Assign(const T& rV){val = rV;}
	inline void Assign(const T& rV)volatile{val = rV;}
	inline T* Addr() {return &val;}
	inline volatile T* Addr() volatile {return &val;}

	inline bool operator<(const T& v)const{return val < v;}
	inline bool operator<(const SAtomicVar<T>& v)const{return val < v.val;}
	inline bool operator>(const T& v)const{return val > v;}
	inline bool operator>(const SAtomicVar<T>& v)const{return val > v.val;}
	inline bool operator<=(const T& v)const{return val <= v;}
	inline bool operator<=(const SAtomicVar<T>& v)const{return val <= v.val;}
	inline bool operator>=(const T& v)const{return val >= v;}
	inline bool operator>=(const SAtomicVar<T>& v)const{return val >= v.val;}
	inline bool operator==(const T& v)const{return val == v;}
	inline bool operator==(const SAtomicVar<T>& v)const{return val == v.val;}
	inline bool operator!=(const T& v)const{return val != v;}
	inline bool operator!=(const SAtomicVar<T>& v)const{return val != v.val;}
	inline T operator*(const T& v)const{return val * v;}
	inline T operator/(const T& v)const{return val / v;}
	inline T operator+(const T& v)const{return val + v;}
	inline T operator-(const T& v)const{return val - v;}

	inline bool operator<(const T& v)volatile const{return val < v;}
	inline bool operator<(const SAtomicVar<T>& v)volatile const{return val < v.val;}
	inline bool operator>(const T& v)volatile const{return val > v;}
	inline bool operator>(const SAtomicVar<T>& v)volatile const{return val > v.val;}
	inline bool operator<=(const T& v)volatile const{return val <= v;}
	inline bool operator<=(const SAtomicVar<T>& v)volatile const{return val <= v.val;}
	inline bool operator>=(const T& v)volatile const{return val >= v;}
	inline bool operator>=(const SAtomicVar<T>& v)volatile const{return val >= v.val;}
	inline bool operator==(const T& v)volatile const{return val == v;}
	inline bool operator==(const SAtomicVar<T>& v)volatile const{return val == v.val;}
	inline bool operator!=(const T& v)volatile const{return val != v;}
	inline bool operator!=(const SAtomicVar<T>& v)volatile const{return val != v.val;}
	inline T operator*(const T& v)volatile const{return val * v;}
	inline T operator/(const T& v)volatile const{return val / v;}
	inline T operator+(const T& v)volatile const{return val + v;}
	inline T operator-(const T& v)volatile const{return val - v;}
}
#if defined(PS3)
	_ALIGN(128)
#elif defined(XENON)
//	__declspec(align(128))
#endif
;

typedef SAtomicVar<int> TIntAtomic;
typedef SAtomicVar<unsigned int> TUIntAtomic;
typedef SAtomicVar<float> TFloatAtomic;

// SNC doesn't support inline assembly
#if !defined(__SNC__)
	#define USE_INLINE_ASM
#endif

#define __add_db16cycl__ NIntrinsics::YieldFor16Cycles();

#if !defined(__SPU__)
	void CrySpinLock(volatile int *pLock,int checkVal,int setVal);
  void CryReleaseSpinLock	(volatile int*, int);
	#if !defined(PS3)
		LONG   CryInterlockedIncrement( int volatile *lpAddend );
		LONG   CryInterlockedDecrement( int volatile *lpAddend );
		LONG   CryInterlockedExchangeAdd(LONG volatile * lpAddend, LONG Value);
		LONG	 CryInterlockedCompareExchange(LONG volatile * dst, LONG exchange, LONG comperand);

		void*	 CryInterlockedCompareExchangePointer(void* volatile * dst, void* exchange, void* comperand);
	#endif
	void*  CryCreateCriticalSection();
  void   CryCreateCriticalSectionInplace(void*);
	void   CryDeleteCriticalSection( void *cs );
  void   CryDeleteCriticalSectionInplace( void *cs );
	void   CryEnterCriticalSection( void *cs );
	bool   CryTryCriticalSection( void *cs );
	void   CryLeaveCriticalSection( void *cs );

#ifdef ORBIS
	LONG   CryInterlockedExchange(LONG volatile *addr, LONG exchange);
	#define InterlockedExchange CryInterlockedExchange
#endif

#if defined(PS3)
	#include <sys/ppu_thread.h>
	#include <pthread.h>
	#include <ppu_intrinsics.h>

	extern DWORD Sleep( DWORD dwMilliseconds );

	ILINE LONG CryInterlockedIncrement( int volatile *lpAddend )
	{
/*		register int r;

		__asm__ __volatile__ (
			"0:      lwarx      %0, 0, %1     # load and reserve\n"
			"        addi       %0, %0, 1    \n"
			"        stwcx.     %0, 0, %1     # store if still reserved\n"
			"        bne-       0b            # loop if lost reservation\n"
			: "=r" (r)
			: "r" (lpAddend), "m" (*lpAddend), "0" (r)
			: "r0", "cc", "memory"
			);

		// Notes:
		// - %0 and %1 must be different registers. We're specifying %0 (r) as input
		//   _and_ output to enforce this.
		// - The 'addi' instruction will become 'li' if the second register operand
		//   is r0, so we're listing r0 is clobbered to make sure r0 is not allocated
		//   for %0.
		return r;
*/
#ifdef USE_INLINE_ASM
		uint32_t old, tmp;

		__asm__ volatile(
			".loop%=:\n"
			"	lwarx   %[old], 0, %[lpAddend]\n"
			"	addi    %[tmp], %[old], 1\n"
			"	stwcx.  %[tmp], 0, %[lpAddend]\n"
			"	beq-    .exsuc%=										# check if write successful\n"	
#ifdef ADD_DB16_CYCLES
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
#endif
			"	b    .loop%=											  # loop if lost reservation\n"	
			".exsuc%=:														# exchange written\n"
			: [old]"=&b"(old), [tmp]"=&r"(tmp)
			: [lpAddend]"b"(lpAddend)
			: "cc", "memory");
		__lwsync();
#else
		uint32_t tmp;
		do {
			tmp = __lwarx((volatile void*)lpAddend);
			tmp++;
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
		} while (0 == __stwcx((volatile void*)lpAddend, tmp));
#endif
		return tmp;
	}

	//////////////////////////////////////////////////////////////////////////
	ILINE LONG CryInterlockedDecrement( int volatile *lpAddend )
	{
/*		register int r;

		__asm__ __volatile__ (
			"0:      lwarx      %0, 0, %1     # load and reserve\n"
			"        addi       %0, %0, -1    \n"
			"        stwcx.     %0, 0, %1     # store if still reserved\n"
			"        bne-       0b            # loop if lost reservation\n"
			: "=r" (r)
			: "r" (lpAddend), "m" (*lpAddend), "0" (r)
			: "r0", "cc", "memory"
			);

		return r;
*/
#ifdef USE_INLINE_ASM
		uint32_t old, tmp;
		__asm__ volatile(
			".loop%=:\n"
			"	lwarx   %[old], 0, %[lpAddend]\n"
			"	subi    %[tmp], %[old], 1\n"
			"	stwcx.  %[tmp], 0, %[lpAddend]\n"
			"	beq-    .exsuc%=										# check if write successful\n"	
#ifdef ADD_DB16_CYCLES
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
#endif
			"	b    .loop%=											  # loop if lost reservation\n"	
			".exsuc%=:														# exchange written\n"
			: [old]"=&b"(old), [tmp]"=&r"(tmp)
			: [lpAddend]"b"(lpAddend)
			: "cc", "memory");
		__lwsync();
#else
		uint32_t tmp;
		do {
			tmp = __lwarx((volatile void*)lpAddend);
			tmp--;
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
		} while (0 == __stwcx((volatile void*)lpAddend, tmp));
#endif
		return tmp;
	}
	//////////////////////////////////////////////////////////////////////////

	ILINE LONG CryInterlockedExchangeAdd(LONG volatile * lpAddend, LONG Value)
	{
		//implements atomically: 	long res = *dst; *dst += Value;	return res;
#ifdef USE_INLINE_ASM
		uint32_t old, tmp;
		__asm__ __volatile__ (
			".loop%=:															# loop start\n"
			" lwarx   %[old], 0, %[lpAddend]\n"
			" add     %[tmp], %[Value], %[old]\n"
			" stwcx.  %[tmp], 0, %[lpAddend]\n"
			"	beq-    .exsuc%=										# check if write successful\n"	
#ifdef ADD_DB16_CYCLES
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
#endif
			"	b    .loop%=											  # loop if lost reservation\n"	
			".exsuc%=:														# exchange written\n"
			: [old] "=&r" (old), [tmp] "=&r" (tmp)
			: [lpAddend] "b" (lpAddend), [Value] "r" (Value)
			: "cc", "memory"
			);
		__lwsync();
#else
		uint32_t old;
		do {
			old = __lwarx((volatile void*)lpAddend);
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
		} while (0 == __stwcx((volatile void*)lpAddend, old + Value));
#endif
		return old;
	}

  ILINE LONG CryInterlockedExchange(LONG volatile *addr, LONG exchange)
  {
#ifdef USE_INLINE_ASM
		uint32_t old;
		__asm__ __volatile__ (
			".loop%=:															# loop start\n"
				" lwarx   %[old], 0, %[addr]\n"
				" stwcx.  %[exchange], 0, %[addr]\n"
				"	beq-    .exsuc%=										# check if write successful\n"	
#ifdef ADD_DB16_CYCLES
				" db16cyc											        # give other hardware thread chance to run\n"
				" db16cyc											        # give other hardware thread chance to run\n"
				" db16cyc											        # give other hardware thread chance to run\n"
				" db16cyc											        # give other hardware thread chance to run\n"
#endif
				"	b    .loop%=											  # loop if lost reservation\n"	
				".exsuc%=:														# exchange written\n"
				: [old] "=&r" (old)
				: [addr] "b" (addr), [exchange] "r" (exchange)
				: "cc", "memory");
		__lwsync();
#else
		uint32_t old;
		do {
			old = __lwarx((volatile void*)addr);
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
		} while (0 == __stwcx((volatile void*)addr, exchange));
#endif
		return old;
	}

	#define InterlockedExchange CryInterlockedExchange

	ILINE LONG CryInterlockedCompareExchange(LONG volatile * dst, LONG exchange, LONG comperand)
	{
		//implements atomically: 	LONG res = *dst; if (comperand == res)*dst = exchange;	return res;
		LONG old;
#ifdef USE_INLINE_ASM
		__asm__ __volatile__ (
			".loop%=:															# loop start\n"
			" lwarx   %[old], 0, %[dst]\n"
			" cmpw    %[old], %[comperand]\n"
			" bne-    1f\n"
			" stwcx.  %[exchange], 0, %[dst]\n"
			"	beq-    1f										      # check if write successful\n"	
#ifdef ADD_DB16_CYCLES
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
			" db16cyc											        # give other hardware thread chance to run\n"
#endif
			"	b    .loop%=											  # loop if lost reservation\n"	
			"1:\n"
			: [old] "=&r" (old)
			: [dst] "b" (dst), [comperand] "r" (comperand), [exchange] "r" (exchange)
			: "cc", "memory"
			);
		__lwsync();
#else
		do {
			old = __lwarx((volatile void*)dst);
			if (old != comperand) 
				break;
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
		} while (0 == __stwcx((volatile void*)dst, exchange));
#endif
		return old;
	}

  ILINE int64_t CryInterlockedCompareExchange64(
			volatile int64_t *addr,
			int64_t exchange,
			int64_t compare
			)
	{
#ifdef USE_INLINE_ASM
		int64_t old;
		__asm__ __volatile__ (
				"0:      ldarx   %[old], 0, %[addr]\n"
				"        cmpd    %[old], %[compare]\n"
				"        bne-    1f\n"
				"        stdcx.  %[exchange], 0, %[addr]\n"
				"        bne-    0b\n"
				"1:\n"
				: [old] "=&r" (old)
				: [addr] "b" (addr), [compare] "r" (compare), [exchange] "r" (exchange)
				: "cc", "memory"
				);
		__lwsync();
#else
		int64_t old;
		do {
			old = __ldarx((volatile void*)addr);
			if (old != compare) 
				break;
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
			__add_db16cycl__
		} while (0 == __stdcx((volatile void*)addr, exchange));
#endif
		return old;
	}

	ILINE void*	 CryInterlockedCompareExchangePointer(void* volatile * dst, void* exchange, void* comperand)
	{
#if !defined(PS3)
		assert(sizeof(void*) == sizeof(LONG));
#endif
		return (void*)CryInterlockedCompareExchange((LONG volatile*)dst, (LONG)exchange, (LONG)comperand);
	}

	ILINE void CryInterlockedAdd(volatile size_t *pVal, ptrdiff_t iAdd)
	{
		size_t v = (size_t)CryInterlockedExchangeAdd((volatile long*)pVal,(long)iAdd);
		v += iAdd;
		assert((iAdd == 0) || (iAdd < 0 && v < v - (size_t)iAdd) || (iAdd > 0 && v > v - (size_t)iAdd));
	}
#endif

ILINE void CrySpinLock(volatile int *pLock,int checkVal,int setVal)
{ 
#ifdef XENON    
  while(InterlockedCompareExchangeAcquire((volatile LONG*)pLock,setVal,checkVal)!=checkVal)
		Sleep(0);
#else

#ifdef _CPU_X86
# ifdef __GNUC__
	register int val;
	__asm__ __volatile__ (
		"0:     mov %[checkVal], %%eax\n"
		"       lock cmpxchg %[setVal], (%[pLock])\n"
		"       jnz 0b"
		: "=m" (*pLock)
		: [pLock] "r" (pLock), "m" (*pLock),
		  [checkVal] "m" (checkVal),
		  [setVal] "r" (setVal)
		: "eax", "cc", "memory"
		);
# else //!__GNUC__
	__asm
	{
		mov edx, setVal
		mov ecx, pLock
Spin:
		// Trick from Intel Optimizations guide
#ifdef _CPU_SSE
		pause
#endif 
		mov eax, checkVal
		lock cmpxchg [ecx], edx
		jnz Spin
	}
# endif //!__GNUC__
#else // !_CPU_X86
# if defined(PS3)
#  if defined(PS3_PROFILE_LOCKS)
	register int val, loop;
	__asm__ __volatile__ (
		"        li      %[loop], 0\n"
		"0:      lwarx   %[val], 0, %[pLock]\n"
		"        addi    %[loop], %[loop], 1\n"
		"        cmpw    %[checkVal], %[val]\n"
		"        bne-    0b\n"
		"        stwcx.  %[setVal], 0, %[pLock]\n"
		"        bne-    0b\n"
		: [val] "=&r" (val), "=m" (*pLock), [loop] "=&r" (loop)
		: [pLock] "r" (pLock), "m" (*pLock), "r" (loop),
			[checkVal] "r" (checkVal),
			[setVal] "r" (setVal)
		: "r0", "cc", "memory"
		);
	__lwsync();
	// Note: We're listing "r0" as clobbered to make sure r0 is not allocated
	// for %[loop] (otherwise the "addi" becomes "li").
	if (loop > 1)
	{
		g_nLockContentionCount += 1;
		g_nLockContentionCost += loop - 1;
	}
	g_nLockOpCount += 1;
#  else // PS3_PROFILE_LOCKS
/*#ifdef USE_INLINE_ASM
	register int val;
	__asm__ __volatile__ (
		".loop%=:															# loop start\n"
		" lwarx   %[val], 0, %[pLock]\n"
		" cmpw    %[checkVal], %[val]\n"
		"	beq-    .lockobtained%=						  # \n"	
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		"	b    .loop%=											  # loop if lost reservation\n"	
		".lockobtained%=:											# lock obtained\n"
		" stwcx.  %[setVal], 0, %[pLock]\n"
		"	beq-    .exsuc%=										# check if write successful\n"	
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		"	b    .loop%=											  # loop if lost reservation\n"	
		".exsuc%=:														# exchange written\n"
		: [val] "=&r" (val), "=m" (*pLock)
		: [pLock] "r" (pLock), "m" (*pLock),
		  [checkVal] "r" (checkVal),
		  [setVal] "r" (setVal)
		: "cc", "memory"
		);
#else
	while(CryInterlockedCompareExchange((volatile long*)pLock,setVal,checkVal)!=checkVal)
	{
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
	}
#endif
	*/
	
	IF(CryInterlockedCompareExchange((volatile LONG*)pLock,setVal,checkVal)!=checkVal, 0)
	{
		uint64 startTime = rdtsc();
		uint64 endTime = startTime + 500;//~6 usecs
		while(CryInterlockedCompareExchange((volatile LONG*)pLock,setVal,checkVal)!=checkVal)
		{
			IF(rdtsc() > endTime,0)
				Sleep(1);
			else
			{
				for(int i=0; i<50; ++i)
				{
					__db16cyc();
					__db16cyc();
					__db16cyc();
					__db16cyc();
				}
			}
		}
	}
#  endif // PS3_PROFILE_LOCKS
# elif defined(ORBIS)
	while (sceAtomicCompareAndSwap32((volatile int32_t*)pLock, (int32_t)checkVal, (int32_t)setVal)!=checkVal);

# elif defined(MAC) || defined(LINUX64)
    register int val;
	__asm__ __volatile__ (
		"0:     mov %[checkVal], %%eax\n"
		"       lock cmpxchg %[setVal], (%[pLock])\n"
		"       jnz 0b"
		: "=m" (*pLock)
		: [pLock] "r" (pLock), "m" (*pLock),
		  [checkVal] "m" (checkVal),
		  [setVal] "r" (setVal)
		: "eax", "cc", "memory"
		);
    //while(CryInterlockedCompareExchange((volatile long*)pLock,setVal,checkVal)!=checkVal) ;
# else
	// NOTE: The code below will fail on 64bit architectures!
	while(_InterlockedCompareExchange((volatile LONG*)pLock,setVal,checkVal)!=checkVal) ;
# endif
#endif
#endif
}

ILINE void CryReleaseSpinLock(volatile int *pLock,int setVal)
{ 
  *pLock = setVal;
#ifdef PS3
	__lwsync();
#endif
}

//////////////////////////////////////////////////////////////////////////
#if defined(APPLE) || defined(LINUX64)
// Fixes undefined reference to CryInterlockedAdd(unsigned long volatile*, long) on
// Mac and linux.
ILINE void CryInterLockedAdd(volatile LONG *pVal, LONG iAdd)
{
    (void) CryInterlockedExchangeAdd(pVal,iAdd); 
}


/*
ILINE void CryInterLockedAdd(volatile unsigned long *pVal, long iAdd)
{
    long r;
	__asm__ __volatile__ (
	#if defined(LINUX64) || defined(MAC)  // long is 64 bits on amd64.
		"lock ; xaddq %0, (%1) \n\t"
	#else
		"lock ; xaddl %0, (%1) \n\t"
	#endif
		: "=r" (r)
		: "r" (pVal), "0" (iAdd)
		: "memory"
	);
    (void) r;
}*/
/*ILINE void CryInterlockedAdd(volatile size_t *pVal, ptrdiff_t iAdd) {
   //(void)CryInterlockedExchangeAdd((volatile long*)pVal,(long)iAdd);
    (void) __sync_fetch_and_add(pVal,iAdd);
}*/

#endif
ILINE void CryInterlockedAdd(volatile int *pVal, int iAdd)
{
#ifdef _CPU_X86
# ifdef __GNUC__
	__asm__ __volatile__ (
		"        lock add %[iAdd], (%[pVal])\n"
		: "=m" (*pVal)
		: [pVal] "r" (pVal), "m" (*pVal), [iAdd] "r" (iAdd)
		);
# else
	__asm
	{
		mov edx, pVal
		mov eax, iAdd
		lock add [edx], eax
	}
# endif
#else
# if defined(PS3)
#ifdef USE_INLINE_ASM
	register int val;
	__asm__ __volatile__ (
		".loop%=:															# loop start\n"
		" lwarx   %[val], 0, %[pVal]\n"
		" add     %[val], %[val], %[iAdd]\n"
		" stwcx.  %[val], 0, %[pVal]\n"
		"	beq-    .exsuc%=										# check if write successful\n"	
#ifdef ADD_DB16_CYCLES
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
		" db16cyc											        # give other hardware thread chance to run\n"
#endif
		"	b    .loop%=											  # loop if lost reservation\n"	
		".exsuc%=:														# exchange written\n"
		: [val] "=&r" (val), "=m" (*pVal)
		: [pVal] "r" (pVal), "m" (*pVal),
			[iAdd] "r" (iAdd) 
		);
	__lwsync();
#else
	int old;
	do {
		old = __lwarx((volatile void*)pVal);
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
		__add_db16cycl__
	} while (0 == __stwcx((volatile void*)pVal, old + iAdd));
#endif
# else
	// NOTE: The code below will fail on 64bit architectures!
#ifdef XENON
	MemoryBarrier();
  InterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
  MemoryBarrier();
#elif defined(_WIN64)
  _InterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
# elif defined(ORBIS)
	sceAtomicAdd32((int32_t*)pVal, (int32_t)iAdd);
#elif defined(APPLE) || defined(LINUX)
  CryInterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
#else
  InterlockedExchangeAdd((volatile LONG*)pVal,iAdd);
#endif
#endif
#endif
}

#endif //__SPU__

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CryInterlocked*SList Function, these are specialized C-A-S
// functions for single-linked lists which prevent the A-B-A problem there
// there are implemented in the platform specific CryThread_*.h files
// TODO clean up the interlocked function the same was the CryThread_* header are

//TODO somehow get their real size on WIN (without including windows.h...)
//NOTE: The sizes are verifyed at compile-time in the implementation functions, but this is still ugly

#if !defined(_SPU_JOB) || defined(JOB_LIB_COMP)
#if defined(WIN64) || defined(DURANGO) 
_MS_ALIGN(16)
#elif defined(WIN32)
_MS_ALIGN(4)
#elif defined(XENON)
_MS_ALIGN(4)
#endif
struct SLockFreeSingleLinkedListEntry
{
	volatile SLockFreeSingleLinkedListEntry *pNext;
}
#if defined(PS3) 
_ALIGN(4)
#elif defined(LINUX32)
_ALIGN(8)
#elif defined(MAC) || defined(LINUX64) 
_ALIGN(16)
#endif
;

#if defined(WIN64) || defined(DURANGO)
_MS_ALIGN(16)
#elif defined(WIN32)
_MS_ALIGN(8)
#elif defined(XENON)
_MS_ALIGN(8)
#endif
struct SLockFreeSingleLinkedListHeader
{
	volatile SLockFreeSingleLinkedListEntry *pNext;
#if defined(APPLE) || defined(LINUX) || defined(ORBIS)
    volatile uint64 salt;
#endif
}
#if defined(PS3) 
_ALIGN(4)
#elif defined(ORBIS)
__attribute__ ((aligned(16)))
#elif defined(LINUX32)
_ALIGN(8)
#elif defined(MAC) || defined(LINUX64)
_ALIGN(16)
#endif
;


// push a element atomically onto a single linked list
void CryInterlockedPushEntrySList( SLockFreeSingleLinkedListHeader& list, SLockFreeSingleLinkedListEntry &element );

// push a element atomically from a single linked list
void* CryInterlockedPopEntrySList(  SLockFreeSingleLinkedListHeader& list );

// initialzied the lock-free single linked list
void CryInitializeSListHead(SLockFreeSingleLinkedListHeader& list);
  
// flush the whole list
void* CryInterlockedFlushSList(SLockFreeSingleLinkedListHeader& list);
#endif


//special define to guard SPU driver compilation
#if !defined(JOB_LIB_COMP) && !defined(_SPU_JOB)

ILINE void CryReadLock(volatile int *rw, bool yield)
{
	CryInterlockedAdd(rw,1);
#ifdef NEED_ENDIAN_SWAP
# ifdef PS3_PROFILE_LOCKS
	int loop = 0;
	volatile char *pw=(volatile char*)rw+1; 
#if defined(XENON) || defined(PS3)
	if (yield)
	{
		for(;*pw;) {++loop; Sleep(0); }
	}
	else
#endif
	{
		for(;*pw;) {++loop;}
	}

	if (loop > 0)
	{
		g_nLockContentionCount += 1;
		g_nLockContentionCost += loop;
	}
# else
	volatile char *pw=(volatile char*)rw+1;
#if defined(XENON) || defined(PS3)
	if (yield)
	{
		for(;*pw;) Sleep(0);
	}
	else
#endif
	{
		for(;*pw;);
	}
# endif
#else
	volatile char *pw=(volatile char*)rw+2;
#if defined(XENON) || defined(PS3)
	if (yield)
	{
		for(;*pw;) Sleep(0);
	}
	else
#endif
	{
		for(;*pw;);
	}
#endif
}

ILINE void CryReleaseReadLock(volatile int* rw)
{
	CryInterlockedAdd(rw,-1);
}

ILINE void CryWriteLock(volatile int* rw)
{
	CrySpinLock(rw,0,WRITE_LOCK_VAL);
}

ILINE void CryReleaseWriteLock(volatile int* rw)
{
	CryInterlockedAdd(rw,-WRITE_LOCK_VAL);
}

//////////////////////////////////////////////////////////////////////////
struct ReadLock
{
	ILINE ReadLock(volatile int &rw)
	{
		CryInterlockedAdd(prw=&rw,1);
#ifdef NEED_ENDIAN_SWAP
# ifdef PS3_PROFILE_LOCKS
		int loop = 0;
		volatile char *pw=(volatile char*)&rw+1; for(;*pw;) ++loop;
		if (loop > 0)
		{
			g_nLockContentionCount += 1;
			g_nLockContentionCost += loop;
		}
# else
		volatile char *pw=(volatile char*)&rw+1; for(;*pw;);
# endif
#else
		volatile char *pw=(volatile char*)&rw+2; for(;*pw;);
#endif
	}
	ILINE ReadLock(volatile int &rw, bool yield)
	{
		CryReadLock(prw=&rw, yield);
	}
	~ReadLock()
	{
		CryReleaseReadLock(prw);
	}
private:
	volatile int *prw;
};

struct ReadLockCond
{
	ILINE ReadLockCond(volatile int &rw,int bActive)
	{
		if (bActive)
		{
			CryInterlockedAdd(&rw,1);
			bActivated = 1;
#ifdef NEED_ENDIAN_SWAP
# ifdef PS3_PROFILE_LOCKS
			int loop = 0;
			volatile char *pw=(volatile char*)&rw+1; for(;*pw;) ++loop;
			if (loop > 0)
			{
				g_nLockContentionCount += 1;
				g_nLockContentionCost += loop;
			}
# else
			volatile char *pw=(volatile char*)&rw+1; for(;*pw;);
# endif
#else
			volatile char *pw=(volatile char*)&rw+2; for(;*pw;);
#endif
		}
		else
		{
			bActivated = 0;
		}
		prw = &rw; 
	}
	void SetActive(int bActive=1) { bActivated = bActive; }
	void Release() { CryInterlockedAdd(prw,-bActivated); }
	~ReadLockCond()
	{
#if defined(PS3) || defined(XENON)
		if (bActivated)
#endif
		CryInterlockedAdd(prw,-bActivated);
	}

private:
	volatile int *prw;
	int bActivated;
};

//////////////////////////////////////////////////////////////////////////
struct WriteLock
{
	ILINE WriteLock(volatile int &rw) { CryWriteLock(&rw); prw=&rw; }
	~WriteLock() { CryReleaseWriteLock(prw); }
private:
	volatile int *prw;
};

//////////////////////////////////////////////////////////////////////////
struct WriteAfterReadLock
{
	ILINE WriteAfterReadLock(volatile int &rw) { CrySpinLock(&rw,1,WRITE_LOCK_VAL+1); prw=&rw; }
	~WriteAfterReadLock() { CryInterlockedAdd(prw,-WRITE_LOCK_VAL); }
private:
	volatile int *prw;
};

//////////////////////////////////////////////////////////////////////////
struct WriteLockCond
{
	ILINE WriteLockCond(volatile int &rw,int bActive=1)
	{
		if (bActive)
			CrySpinLock(&rw,0,iActive=WRITE_LOCK_VAL);
		else 
			iActive = 0;
		prw = &rw; 
	}
	ILINE WriteLockCond() { prw=&(iActive=0); }
	~WriteLockCond() { 
#if defined(PS3) || defined(XENON)
		if (iActive)
#endif
		CryInterlockedAdd(prw,-iActive); 
	}
	void SetActive(int bActive=1) { iActive = -bActive & WRITE_LOCK_VAL; }
	void Release() { CryInterlockedAdd(prw,-iActive); }
	volatile int *prw;
	int iActive;
};


#if defined(LINUX64) || defined(APPLE)
ILINE int64 CryInterlockedCompareExchange64( volatile int64 *addr, int64 exchange, int64 comperand )
{
    return __sync_val_compare_and_swap(addr,comperand,exchange);
    // This is OK, because long is signed int64 on Linux x86_64
	//return CryInterlockedCompareExchange((volatile long*)addr, (long)exchange, (long)comperand);
}
#elif !defined(PS3)
ILINE int64 CryInterlockedCompareExchange64( volatile int64 *addr, int64 exchange, int64 compare )
{
	// forward to system call
#if defined(ORBIS)
	return sceAtomicCompareAndSwap64((volatile int64_t*)addr, (int64_t)compare, (int64_t)exchange);
#else
	return _InterlockedCompareExchange64( (volatile int64*)addr, exchange,compare);
#endif
}
#endif
//////////////////////////////////////////////////////////////////////////
#if defined(PS3)

#if !defined(_RELEASE)
  extern uint32 s_dwMainThreadId; 
#endif 

// for SwtichFiberDirect
namespace JobManager {
namespace Fiber {
	void SwitchFiberDirect();
	void YieldFiber();
	uint64_t FiberYieldTime();
}
}
//////////////////////////////////////////////////////////////////////////
struct FiberLockBase 
{ 
protected: 
#if !defined(_RELEASE)
	FiberLockBase() { if (s_dwMainThreadId && CryGetCurrentThreadId() != s_dwMainThreadId) __debugbreak(); } 
	~FiberLockBase() { if (s_dwMainThreadId && CryGetCurrentThreadId() != s_dwMainThreadId) __debugbreak(); } 
#endif 

	void contended() 
	{ 
#if !defined(__SPU__)
		JobManager::Fiber::SwitchFiberDirect();
#endif    
	}
}; 

//////////////////////////////////////////////////////////////////////////
struct FiberReadLock : public FiberLockBase 
{
	ILINE FiberReadLock(volatile int &rw) : prw(&rw) { *prw+=1; char *pw=(char*)&rw+1; for(;*pw;) contended(); }
	ILINE FiberReadLock(volatile int &rw, bool yield) : prw(&rw) { *prw+=1; char *pw=(char*)&rw+1; for(;*pw;) contended(); }
	ILINE ~FiberReadLock() { *prw -= 1; }
private:
	volatile int *prw;
};

//////////////////////////////////////////////////////////////////////////
struct JobFiberReadLock : public FiberLockBase {
	ILINE JobFiberReadLock(volatile int &rw) : prw(&rw) { CryInterlockedAdd(prw,1); volatile char *pw=(volatile char*)&rw+1; for(;*pw;) contended(); }
	ILINE JobFiberReadLock(volatile int &rw, bool yield) : prw(&rw) { CryInterlockedAdd(prw,1); volatile char *pw=(volatile char*)&rw+1; for(;*pw;) contended(); }
	ILINE ~JobFiberReadLock()	{ CryInterlockedAdd(prw,-1); }
private:
	volatile int *prw;
};

//////////////////////////////////////////////////////////////////////////
struct FiberReadLockCond : public FiberLockBase 
{
	ILINE FiberReadLockCond(volatile int &rw,int bActive) : prw((int*)&rw) 
	{
		IF (bActive,1) { *prw += 1; bActivated = 1; char *pw=(char*)&rw+1; for(;*pw;) contended(); }
		else bActivated = 0;
	}
	void SetActive(int bActive=1) { bActivated = bActive; }
	void Release() { *prw += -bActivated; }
  ILINE ~FiberReadLockCond()	{ *prw += -bActivated; }
private:
	volatile int *prw;
	int bActivated;
};
struct JobFiberReadLockCond : public FiberLockBase 
{
	ILINE JobFiberReadLockCond(volatile int &rw,int bActive) : prw(&rw) 
	{
		IF (bActive,1) { CryInterlockedAdd(prw,1); bActivated = 1; volatile char *pw=(volatile char*)&rw+1; for(;*pw;) contended(); }
		else bActivated = 0;
	}
	void SetActive(int bActive=1) { bActivated = bActive; }
	void Release() { CryInterlockedAdd(prw,-bActivated); }
	ILINE ~JobFiberReadLockCond()	{ CryInterlockedAdd(prw,-bActivated); }
private:
	volatile int *prw;
	int bActivated;
};

//////////////////////////////////////////////////////////////////////////
struct FiberWriteLock : public FiberLockBase 
{
	ILINE FiberWriteLock(volatile int &rw) : prw(&rw){ WHILE(*prw!=0, 0) contended(); *prw=WRITE_LOCK_VAL; }
	ILINE~FiberWriteLock() { *prw+=-WRITE_LOCK_VAL; } 
private:
	volatile int *prw;
};
struct JobFiberWriteLock : public FiberLockBase {
	ILINE JobFiberWriteLock(volatile int &rw) : prw(&rw) { WHILE(CryInterlockedCompareExchange((volatile LONG*)prw,WRITE_LOCK_VAL,0)!=0,0) contended(); }
	ILINE ~JobFiberWriteLock() { CryInterlockedAdd(prw, -WRITE_LOCK_VAL); }
private:
	volatile int *prw;
};

//////////////////////////////////////////////////////////////////////////
struct FiberWriteLockCond : public FiberLockBase 
{
	ILINE FiberWriteLockCond(volatile int &rw,int bActive=1) : prw((int*)&rw) 
	{
		if (bActive) { WHILE(*prw!=0,0) contended(); *prw=iActive=WRITE_LOCK_VAL; }
		else iActive=0; 
	}
	ILINE FiberWriteLockCond() { prw=&(iActive=0); }
	ILINE ~FiberWriteLockCond() { if(iActive) { *prw+=-iActive; } }
	void SetActive(int bActive=1) { iActive = -bActive&WRITE_LOCK_VAL; }
	void Release() { *prw+=-iActive; }
	volatile int *prw;
	int iActive;
};
struct JobFiberWriteLockCond : public FiberLockBase 
{
	ILINE JobFiberWriteLockCond(volatile int &rw,int bActive=1) : prw(&rw) 
	{ 
		IF (bActive,1) 
		{ 
			WHILE(CryInterlockedCompareExchange((volatile LONG*)prw,WRITE_LOCK_VAL,0)!=0,0) contended();
			iActive=WRITE_LOCK_VAL; 
		}
		else iActive=0; 
	}
	ILINE JobFiberWriteLockCond() { prw=&(iActive=0); }
	ILINE ~JobFiberWriteLockCond() { CryInterlockedAdd(prw,-iActive); }
	void SetActive(int bActive=1) { iActive = -bActive&WRITE_LOCK_VAL; }
	void Release() { CryInterlockedAdd(prw,-iActive); }
	volatile int *prw;
	int iActive;
};
#endif//PS3 

struct Lock_dummy_param{};

struct NoLock {
	NoLock(unsigned int& rw) {}
	NoLock(int& rw, int bActive=1) {}
	NoLock(volatile int& rw, int bActive=1) {}
	NoLock(Lock_dummy_param) {}
  NoLock() { 
#if defined(__SPU__)
    prw=&(iActive=0); 
#endif 
  }
	void SetActive(int = 0) {}
#if defined(__SPU__) || defined(PS3_CRYSIZER_HEAP_TRAVERSAL)
	volatile int *prw;
	int iActive;
#endif 
};

#if defined(EXCLUDE_PHYSICS_THREAD) 
# if EMBED_PHYSICS_AS_FIBER
   ILINE void SolveContention() { 
#if !defined(__SPU__)
		 JobManager::Fiber::SwitchFiberDirect();
#endif 
	 }
  ILINE void SpinLock(volatile int *pLock,int checkVal,int setVal) { *(int*)pLock=setVal; } 
	ILINE void AtomicAdd(volatile int *pVal, int iAdd) {	*(int*)pVal+=iAdd; }
	ILINE void AtomicAdd(volatile unsigned int *pVal, int iAdd) { *(unsigned int*)pVal+=iAdd; }
  ILINE void JobSpinLock(volatile int *pLock,int checkVal,int setVal) { 
    while(CryInterlockedCompareExchange((LONG volatile * )pLock,(LONG)setVal,(LONG)checkVal)!=checkVal) SolveContention(); 
  } 
# else
	 ILINE void SpinLock(volatile int *pLock,int checkVal,int setVal) { *(int*)pLock=setVal; } 
	 ILINE void AtomicAdd(volatile int *pVal, int iAdd) {	*(int*)pVal+=iAdd; }
	 ILINE void AtomicAdd(volatile unsigned int *pVal, int iAdd) { *(unsigned int*)pVal+=iAdd; }
   ILINE void JobSpinLock(volatile int *pLock,int checkVal,int setVal) { CrySpinLock(pLock,checkVal,setVal); } 
# endif 
#else
	ILINE void SpinLock(volatile int *pLock,int checkVal,int setVal) { CrySpinLock(pLock,checkVal,setVal); } 
	ILINE void AtomicAdd(volatile int *pVal, int iAdd) {	CryInterlockedAdd(pVal,iAdd); }
	ILINE void AtomicAdd(volatile unsigned int *pVal, int iAdd) { CryInterlockedAdd((volatile int*)pVal,iAdd); }

  ILINE void JobSpinLock(volatile int *pLock,int checkVal,int setVal) { SpinLock(pLock,checkVal,setVal); } 
#endif

ILINE void JobAtomicAdd(volatile int *pVal, int iAdd) {	CryInterlockedAdd(pVal,iAdd); }
ILINE void JobAtomicAdd(volatile unsigned int *pVal, int iAdd) { CryInterlockedAdd((volatile int*)pVal,iAdd); }


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Definitions of default locking primitives for all platforms except PS3 
#if !defined(PS3) 
  typedef ReadLock JobReadLock;
  typedef ReadLockCond JobReadLockCond;
  typedef WriteLock JobWriteLock;
  typedef WriteLockCond JobWriteLockCond;
  typedef ReadLock JobReadLockPlatf1;
  typedef WriteLock JobWriteLockPlatf1;

	#define ReadLockPlatf0 ReadLock
	#define WriteLockPlatf0 WriteLock
	#define ReadLockCondPlatf0 ReadLockCond
	#define WriteLockCondPlatf0 WriteLockCond
	#define ReadLockPlatf1 NoLock
	#define WriteLockPlatf1 NoLock
	#define ReadLockCondPlatf1 NoLock
	#define WriteLockCondPlatf1 NoLock
// Definitions of ps3 locking primitives
#else
  #define ReadLockPlatf0 NoLock
	#define WriteLockPlatf0 NoLock
	#define ReadLockCondPlatf0 NoLock
	#define WriteLockCondPlatf0 NoLock
# if EMBED_PHYSICS_AS_FIBER
   typedef JobFiberReadLock JobReadLock;
   typedef JobFiberReadLockCond JobReadLockCond;
   typedef JobFiberWriteLock JobWriteLock;
   typedef JobFiberWriteLockCond JobWriteLockCond;
   typedef JobFiberReadLock JobReadLockPlatf1;
   typedef JobFiberWriteLock JobWriteLockPlatf1;
#  if !defined(__SPU__)
    #define ReadLock FiberReadLock
    #define ReadLockCond FiberReadLockCond
    #define WriteLock FiberWriteLock
    #define WriteLockCond FiberWriteLockCond
    #define ReadLockPlatf1 FiberReadLock
    #define WriteLockPlatf1 FiberWriteLock
    #define ReadLockCondPlatf1 FiberReadLockCond
    #define WriteLockCondPlatf1 FiberWriteLockCond
#  else
    #define ReadLock NoLock
    #define ReadLockCond NoLock
    #define WriteLock NoLock
    #define WriteLockCond NoLock
    #define ReadLockPlatf1 NoLock
    #define WriteLockPlatf1 NoLock
    #define ReadLockCondPlatf1 NoLock
    #define WriteLockCondPlatf1 NoLock
#  endif
# elif defined(EXCLUDE_PHYSICS_THREAD)
    typedef ReadLock JobReadLock;
    typedef ReadLockCond JobReadLockCond;
    typedef WriteLock JobWriteLock;
    typedef WriteLockCond JobWriteLockCond;
    typedef ReadLock JobReadLockPlatf1;
    typedef WriteLock JobWriteLockPlatf1;

    #define ReadLock NoLock
    #define ReadLockCond NoLock
    #define WriteLock NoLock
    #define WriteLockCond NoLock
    #define ReadLockPlatf1 NoLock
    #define WriteLockPlatf1 NoLock
    #define ReadLockCondPlatf1 NoLock
    #define WriteLockCondPlatf1 NoLock
# else 
    typedef ReadLock JobReadLock;
    typedef ReadLockCond JobReadLockCond;
    typedef WriteLock JobWriteLock;
    typedef WriteLockCond JobWriteLockCond;
    typedef ReadLock JobReadLockPlatf1;
    typedef WriteLock JobWriteLockPlatf1;

    #define ReadLockPlatf1 ReadLock
    #define WriteLockPlatf1 WriteLock
	  #define ReadLockCondPlatf1 ReadLockCond
	  #define WriteLockCondPlatf1 WriteLockCond
#   if defined(__SPU__)
     #define ReadLock NoLock
     #define ReadLockCond NoLock
     #define WriteLock NoLock
     #define WriteLockCond NoLock
     #define ReadLockPlatf1 NoLock
     #define WriteLockPlatf1 NoLock
     #define ReadLockCondPlatf1 NoLock
     #define WriteLockCondPlatf1 NoLock
#   endif 
# endif
#endif
#endif//JOB_LIB_COMP


#endif // __MultiThread_h__
