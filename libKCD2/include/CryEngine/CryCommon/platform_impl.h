////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   platform_impl.h
//  Version:     v1.00
//  Created:     23/7/2004 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description: This file should only be included Once in DLL module.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __platform_impl_h__
#define __platform_impl_h__
#pragma once

#include <StringUtils.h>
#include <platform.h>
#include <ISystem.h>
//#include <CryUnitTest.h>
#include <ITestSystem.h>
#include <CryExtension/Impl/RegFactoryNode.h>
#include <CryExtension/Impl/ICryFactoryRegistryImpl.h>

#ifdef XENON_INTRINSICS
#ifndef _RELEASE
#if defined(XENON)
#if _XDK_VER < 8276
const XMVECTOR  g_XMIdentityR0          = {1.0f, 0.0f, 0.0f, 0.0f};
const XMVECTOR  g_XMIdentityR1          = {0.0f, 1.0f, 0.0f, 0.0f};
const XMVECTOR  g_XMIdentityR2          = {0.0f, 0.0f, 1.0f, 0.0f};
const XMVECTOR  g_XMIdentityR3          = {0.0f, 0.0f, 0.0f, 1.0f};
#endif
#if _XDK_VER < 11626
const XMVECTOR  g_XMZero                = {0.0f, 0.0f, 0.0f, 0.0f};
const XMVECTOR  g_XMOne                 = {1.0f, 1.0f, 1.0f, 1.0f};
#endif
#elif defined(PS3)
extern const XMVECTOR  g_XMIdentityR0;
extern const XMVECTOR  g_XMIdentityR1;
extern const XMVECTOR  g_XMIdentityR2;
extern const XMVECTOR  g_XMIdentityR3;
extern const XMVECTOR  g_XMZero;
extern const XMVECTOR  g_XMOne;
#else
const XMVECTOR  g_XMIdentityR0          = XMVECTOR(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR  g_XMIdentityR1          = XMVECTOR(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR  g_XMIdentityR2          = XMVECTOR(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR  g_XMIdentityR3          = XMVECTOR(0.0f, 0.0f, 0.0f, 1.0f);
const XMVECTOR  g_XMZero                = XMVECTOR(0.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR  g_XMOne                 = XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f);
#endif
#endif
#endif


#if defined(SYS_ENV_AS_STRUCT)
#	if defined(_LAUNCHER)
DLL_EXPORT SSystemGlobalEnvironment gEnv;
#	else
extern SSystemGlobalEnvironment gEnv;
#	endif
#else
SC_API DLL_EXPORT struct SSystemGlobalEnvironment* gEnv = NULL;
#endif


#if defined(_LAUNCHER) && (defined(PS3) || defined(_RELEASE) || defined(LINUX) || defined(APPLE) || defined(ORBIS)) || !defined(PS3) && !defined(_LIB)
//The reg factory is used for registering the different modules along the whole project
struct SRegFactoryNode* g_pHeadToRegFactories = 0;
#endif


#if defined(_LIB) && !defined(_LAUNCHER)
extern CRndGen g_random_generator;
#else //_LIB

//////////////////////////////////////////////////////////////////////////
// If not in static library.
#include <CryThreadImpl.h>
#include <CryCommon.cpp>

/*
 #ifdef CRY_STRING
 int sEmptyStringBuffer[] = { -1, 0, 0, 0 };
 template <>
 string::StrHeader* string::m_emptyStringData = (string::StrHeader*)&sEmptyStringBuffer;
 template <>
 wstring::StrHeader* wstring::m_emptyStringData = (wstring::StrHeader*)&sEmptyStringBuffer;
 #endif //CRY_STRING
 */

// Define UnitTest static variables
CryUnitTest::Test* CryUnitTest::Test::m_pFirst = 0;
CryUnitTest::Test* CryUnitTest::Test::m_pLast = 0;

#if defined(WIN32) || defined(WIN64) || defined(XENON)
void CryPureCallHandler()
{
	CryFatalError("Pure function call");
}

void CryInvalidParameterHandler(
                                const wchar_t * expression,
                                const wchar_t * function,
                                const wchar_t * file,
                                unsigned int line,
                                uintptr_t pReserved
                                )
{
	//size_t i;
	//char sFunc[128];
	//char sExpression[128];
	//char sFile[128];
	//wcstombs_s( &i,sFunc,sizeof(sFunc),function,_TRUNCATE );
	//wcstombs_s( &i,sExpression,sizeof(sExpression),expression,_TRUNCATE );
	//wcstombs_s( &i,sFile,sizeof(sFile),file,_TRUNCATE );
	//CryFatalError( "Invalid parameter detected in function %s. File: %s Line: %d, Expression: %s",sFunc,sFile,line,sExpression );
	CryFatalError( "Invalid parameter detected in CRT function" );
}

void InitCRTHandlers()
{
	_set_purecall_handler(CryPureCallHandler);
	_set_invalid_parameter_handler(CryInvalidParameterHandler);
}
#else
void InitCRTHandlers() {}
#endif

#ifndef SOFTCODE
//////////////////////////////////////////////////////////////////////////
// This is an entry to DLL initialization function that must be called for each loaded module
//////////////////////////////////////////////////////////////////////////
extern "C" DLL_EXPORT void ModuleInitISystem( ISystem *pSystem,const char *moduleName )
{
    
	if (gEnv) // Already registered.
		return;
    
	InitCRTHandlers();
    
#if !defined(SYS_ENV_AS_STRUCT)
	if (pSystem) // DONT REMOVE THIS. ITS FOR RESOURCE COMPILER!!!!
		gEnv = pSystem->GetGlobalEnvironment();
#endif
#if !defined(PS3) && !defined(_LIB) && !(defined(DURANGO) && defined(_LIB))
	if (pSystem)
	{
		ICryFactoryRegistryImpl* pCryFactoryImpl = static_cast<ICryFactoryRegistryImpl*>(pSystem->GetCryFactoryRegistry());
        pCryFactoryImpl->RegisterFactories(g_pHeadToRegFactories);
	}
#endif
#ifdef CRY_UNIT_TESTING
	// Register All unit tests of this module.
	if(pSystem)
	{
		CryUnitTest::Test *pTest = CryUnitTest::Test::m_pFirst;
		for (; pTest != 0; pTest = pTest->m_pNext)
		{
			CryUnitTest::IUnitTestManager *pTestManager = pSystem->GetITestSystem()->GetIUnitTestManager();
			if (pTestManager)
			{
				pTest->m_unitTestInfo.module = moduleName;
				pTestManager->CreateTest( pTest->m_unitTestInfo );
			}
		}
	}
#endif //CRY_UNIT_TESTING
}
#endif

bool g_bProfilerEnabled = false;
int g_iTraceAllocations = 0;

//////////////////////////////////////////////////////////////////////////
extern "C" {
	CRYSYSTEM_API unsigned int CryRandom(); // Exported by CrySystem
}

CRndGen g_random_generator;
uint32 cry_rand32()
{
	return g_random_generator.GenerateUint32();
}
unsigned int cry_rand()
{
	//return CryRandom(); // Return in range from 0 to RAND_MAX
	return g_random_generator.GenerateUint32() & CRY_RAND_MAX;
}
float cry_frand()
{
	return g_random_generator.GenerateFloat();
}

//////////////////////////////////////////////////////////////////////////

// when using STL Port _STLP_DEBUG and _STLP_DEBUG_TERMINATE - avoid actually
// crashing (default terminator seems to kill the thread, which isn't nice).
#ifdef _STLP_DEBUG_TERMINATE

# ifdef __stl_debug_terminate
#  undef __stl_debug_terminate
# endif

void __stl_debug_terminate(void)
{
	assert( 0 && "STL Debug Error" );
}
#endif

#ifdef _STLP_DEBUG_MESSAGE

# ifdef __stl_debug_message
#  undef __stl_debug_message
# endif

void __stl_debug_message(const char * format_str, ...)
{
	va_list __args;
	va_start( __args, format_str );
#ifdef WIN32
	char __buffer [4096];
	vsnprintf(__buffer, sizeof(__buffer) / sizeof(char), format_str, __args);
	OutputDebugStringA(__buffer);
#endif
	gEnv->pLog->LogV( ILog::eErrorAlways,format_str,__args );
	va_end(__args);
}
#endif //_STLP_DEBUG_MESSAGE


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#endif

#if defined(WIN32) || defined(WIN64)
#include <intrin.h>
#endif

// If we use cry memory manager this should be also included in every module.
#if defined(USING_CRY_MEMORY_MANAGER) && !defined(__SPU__)
#include <CryMemoryManager_impl.h>
#endif

#if defined(MAC)
#include "CryAssert_impl.h"
#endif

#if defined(LINUX) && !defined(ANDROID)
#include "CryAssert_impl.h"
#endif


#if defined (_WIN32) || defined (XENON)

#include "CryAssert_impl.h"

//////////////////////////////////////////////////////////////////////////
void CryDebugBreak()
{
#if defined(WIN32) && !defined(RELEASE)
	if (IsDebuggerPresent())
#endif
	{
		DebugBreak();
	}
}

//////////////////////////////////////////////////////////////////////////
void CrySleep( unsigned int dwMilliseconds )
{
	Sleep( dwMilliseconds );
}

//////////////////////////////////////////////////////////////////////////
void CryLowLatencySleep( unsigned int dwMilliseconds )
{
#if defined(DURANGO)
	if(dwMilliseconds>32) // just do an OS sleep for long periods, because we just assume that if we sleep for this long, we don't need a accurate latency (max diff is likly 15ms)
		Sleep( dwMilliseconds );
	else // do a more accurate "sleep" by yielding this CPU to other threads
	{
		LARGE_INTEGER frequency;
		LARGE_INTEGER currentTime;
		LARGE_INTEGER endTime;

		QueryPerformanceCounter(&endTime);

		// Ticks in microseconds (1/1000 ms)
		QueryPerformanceFrequency(&frequency);
		endTime.QuadPart += (dwMilliseconds * frequency.QuadPart) / (1000ULL);

		do
		{
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();

			QueryPerformanceCounter(&currentTime);
		} while (currentTime.QuadPart < endTime.QuadPart);
	}
#else
	CrySleep(dwMilliseconds);
#endif
}

//////////////////////////////////////////////////////////////////////////
int CryMessageBox( const char *lpText,const char *lpCaption,unsigned int uType)
{
#ifdef WIN32
#if !defined(RESOURCE_COMPILER)
	ICVar* const pCVar = gEnv->pConsole ? gEnv->pConsole->GetCVar("sys_no_crash_dialog") : NULL;
	if ((pCVar && pCVar->GetIVal() != 0) || gEnv->bNoAssertDialog)
	{
		return 0;
	}
#endif
	return MessageBox( NULL,lpText,lpCaption,uType );
#else
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
int CryCreateDirectory( const char *lpPathName,void *lpSecurityAttributes )
{
	// Convert from UTF-8 to UNICODE
#if defined(WIN32) || defined(WIN64)  || defined(DURANGO)
	return CreateDirectoryW( CryStringUtils::UTF8ToWStr(lpPathName).c_str(),(LPSECURITY_ATTRIBUTES)lpSecurityAttributes );
#else
	return CreateDirectory( lpPathName,(LPSECURITY_ATTRIBUTES)lpSecurityAttributes );
#endif
}

//////////////////////////////////////////////////////////////////////////
int CryGetCurrentDirectory( unsigned int nBufferLength,char *lpBuffer )
{
#if defined(WIN32) || defined(WIN64)  || defined(DURANGO)
	return GetCurrentDirectoryA(nBufferLength, lpBuffer);
#else
	return 0;
#endif
}

#if defined(WIN32) || defined(WIN64)  || defined(DURANGO)
// Initializes root folder of the game, optionally returns exe name.
void InitRootDir(char szExeFileName[] = 0, uint nSize = 0)
{
	WCHAR szExePathName[_MAX_PATH];
	size_t nLen = GetModuleFileNameW(GetModuleHandle(NULL), szExePathName, _MAX_PATH);
    
	// Find path above exe name and deepest folder.
	int nCount = 0;
	for (size_t n = nLen-1; n > 0; n--)
	{
		if (szExePathName[n] == '\\')
		{
			nLen = n;
			if (++nCount == 2)
				break;
		}
	}
	if (nCount > 0)
	{
		szExePathName[nLen++] = 0;
        
		// Switch to upper folder.
		SetCurrentDirectoryW(szExePathName);
        
		// Return exe name and relative folder, assuming it's ASCII.
		if (szExeFileName)
			wcstombs(szExeFileName, szExePathName+nLen, nSize);
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
short CryGetAsyncKeyState( int vKey )
{
#ifdef WIN32
	return GetAsyncKeyState(vKey);
#else
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
LONG  CryInterlockedIncrement( int volatile *lpAddend )
{
	return InterlockedIncrement((volatile LONG*)lpAddend);
}

//////////////////////////////////////////////////////////////////////////
LONG  CryInterlockedDecrement( int volatile *lpAddend )
{
	return InterlockedDecrement((volatile LONG*)lpAddend);
}

//////////////////////////////////////////////////////////////////////////
LONG	CryInterlockedExchangeAdd(LONG volatile * lpAddend, LONG Value)
{
	return InterlockedExchangeAdd(lpAddend, Value);
}

LONG	CryInterlockedCompareExchange(LONG volatile * dst, LONG exchange, LONG comperand)
{
	return InterlockedCompareExchange(dst, exchange, comperand);
}

void*	CryInterlockedCompareExchangePointer(void* volatile * dst, void* exchange, void* comperand)
{
	return InterlockedCompareExchangePointer(dst, exchange, comperand);
}

void CryInterlockedAdd(volatile size_t *pVal, ptrdiff_t iAdd)
{
#if defined (PLATFORM_64BIT)
	size_t v = (size_t)InterlockedAdd64((volatile int64*)pVal,iAdd);
#else
	size_t v = (size_t)CryInterlockedExchangeAdd((volatile long*)pVal,(long)iAdd);
	v += iAdd;
#endif
	assert((iAdd == 0) || (iAdd < 0 && v < v - (size_t)iAdd) || (iAdd > 0 && v > v - (size_t)iAdd));
}

//////////////////////////////////////////////////////////////////////////
void* CryCreateCriticalSection()
{
	CRITICAL_SECTION *pCS = new CRITICAL_SECTION;
	InitializeCriticalSection(pCS);
	return pCS;
}

void  CryCreateCriticalSectionInplace(void* pCS)
{
    InitializeCriticalSection((CRITICAL_SECTION *)pCS);
}
//////////////////////////////////////////////////////////////////////////
void  CryDeleteCriticalSection( void *cs )
{
	CRITICAL_SECTION *pCS = (CRITICAL_SECTION*)cs;
	if (pCS->LockCount >= 0)
		CryFatalError("Critical Section hanging lock" );
	DeleteCriticalSection(pCS);
	delete pCS;
}

//////////////////////////////////////////////////////////////////////////
void  CryDeleteCriticalSectionInplace( void *cs )
{
    CRITICAL_SECTION *pCS = (CRITICAL_SECTION*)cs;
    if (pCS->LockCount >= 0)
        CryFatalError("Critical Section hanging lock" );
    DeleteCriticalSection(pCS);
}

//////////////////////////////////////////////////////////////////////////
void  CryEnterCriticalSection( void *cs )
{
	EnterCriticalSection((CRITICAL_SECTION*)cs);
}

//////////////////////////////////////////////////////////////////////////
bool  CryTryCriticalSection( void *cs )
{
	return TryEnterCriticalSection((CRITICAL_SECTION*)cs) != 0;
}

//////////////////////////////////////////////////////////////////////////
void  CryLeaveCriticalSection( void *cs )
{
	LeaveCriticalSection((CRITICAL_SECTION*)cs);
}

//////////////////////////////////////////////////////////////////////////
uint32 CryGetFileAttributes( const char *lpFileName )
{
	// Normal GetFileAttributes not available anymore in non desktop applications (eg Durango)
	WIN32_FILE_ATTRIBUTE_DATA data;
	BOOL res;
#if defined(DURANGO)
	res = GetFileAttributesExA( lpFileName, GetFileExInfoStandard, &data);
#else
	res = GetFileAttributesEx( lpFileName, GetFileExInfoStandard, &data);
#endif
	return res ? data.dwFileAttributes : -1;
}

//////////////////////////////////////////////////////////////////////////
bool CrySetFileAttributes( const char *lpFileName,uint32 dwFileAttributes )
{
#if defined(DURANGO)
	return SetFileAttributesA( lpFileName,dwFileAttributes ) != 0;
#else
	return SetFileAttributes( lpFileName,dwFileAttributes ) != 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
threadID CryGetCurrentThreadId()
{
	return GetCurrentThreadId();
}

#else // WIN32

// These are implemented in WinBase.cpp























#endif // _WIN32

#endif //_LIB


#ifndef _LIB
#ifdef XENON

static int startStart = GetTickCount();

HMODULE XenonLoadLibrary( const char *libName )
{
	char s[256];
	MEMORYSTATUS MemoryStatus;
	GlobalMemoryStatus(&MemoryStatus);
	
	//sprintf( s,"Memory: %.2f MB\n",(MemoryStatus.dwTotalPhys - MemoryStatus.dwAvailPhys)/(1024.0f*1024.0f) );
	//OutputDebugString( s );
	
	int m0 = MemoryStatus.dwAvailPhys;
	static int total = 0;
    
	int start = GetTickCount();
	HMODULE h = ::LoadLibrary(libName);
	int end = GetTickCount();
	GlobalMemoryStatus(&MemoryStatus);
	int m1 = MemoryStatus.dwAvailPhys;
	if (m0-m1 > 0)
	{
		total += (end-start);
		sprintf_s( s,"Library %s takes %d Kb. Loading time = %is. Total = %is. Overall=%is\n",libName,(m0-m1)/1024, (end-start)/1000, total/1000, (end-startStart)/1000 );
		OutputDebugString( s );
	}
    
#if CAPTURE_REPLAY_LOG
	if (h)
		CryGetIMemReplay()->LoadedModule(h);
#endif
    
	//sprintf( s,"Memory: %.2f MB\n",(MemoryStatus.dwTotalPhys - MemoryStatus.dwAvailPhys)/(1024.0f*1024.0f) );
	//OutputDebugString( s );
    
	return h;
}

#elif defined(DURANGO)

HMODULE DurangoLoadLibrary( const char *libName )
{
	HMODULE h = ::LoadLibraryExA(libName, 0, 0);
	return h;
}

#endif
#endif

#if defined(WIN32) || defined(WIN64)
#if !defined(_LIB) || defined(_LAUNCHER)
int64 CryGetTicks()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	return li.QuadPart;
}
#endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Threads implementation. For static linking it must be declared inline otherwise creating multiple symbols
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_LIB) && !defined(_LAUNCHER)
#define THR_INLINE inline
#else
#define THR_INLINE
#endif

//////////////////////////////////////////////////////////////////////////
inline void CryDebugStr( const char *format,... )
{
	/*
     #ifdef CRYSYSTEM_EXPORTS
     va_list	ArgList;
     char		szBuffer[65535];
     va_start(ArgList, format);
     _vsnprintf_c(szBuffer,sizeof(szBuffer)-1, format, ArgList);
     va_end(ArgList);
     strcat(szBuffer,"\n");
     OutputDebugString(szBuffer);
     #endif
     */
}

#ifdef PS3
#if CAPTURE_REPLAY_LOG
#define TRACKED_SYSMODULE_LOAD(mod) TrackedSysmoduleLoad(mod, #mod)
int TrackedSysmoduleLoad(int module, const char* moduleName);
#else
#define TRACKED_SYSMODULE_LOAD cellSysmoduleLoadModule
#endif
#endif

// load implementation of platform profile marker
#if !defined(_LIB)
#include <CryProfileMarker_impl.h>
#endif

#if !defined(_LIB) || defined(_LAUNCHER)
_MS_ALIGN(64) uint32  BoxSides[0x40*8] = {
	0,0,0,0, 0,0,0,0, //00
	0,4,6,2, 0,0,0,4, //01
	7,5,1,3, 0,0,0,4, //02
	0,0,0,0, 0,0,0,0, //03
	0,1,5,4, 0,0,0,4, //04
	0,1,5,4, 6,2,0,6, //05
	7,5,4,0, 1,3,0,6, //06
	0,0,0,0, 0,0,0,0, //07
	7,3,2,6, 0,0,0,4, //08
	0,4,6,7, 3,2,0,6, //09
	7,5,1,3, 2,6,0,6, //0a
	0,0,0,0, 0,0,0,0, //0b
	0,0,0,0, 0,0,0,0, //0c
	0,0,0,0, 0,0,0,0, //0d
	0,0,0,0, 0,0,0,0, //0e
	0,0,0,0, 0,0,0,0, //0f
	0,2,3,1, 0,0,0,4, //10
	0,4,6,2, 3,1,0,6, //11
	7,5,1,0, 2,3,0,6, //12
	0,0,0,0, 0,0,0,0, //13
	0,2,3,1, 5,4,0,6, //14
	1,5,4,6, 2,3,0,6, //15
	7,5,4,0, 2,3,0,6, //16
	0,0,0,0, 0,0,0,0, //17
	0,2,6,7, 3,1,0,6, //18
	0,4,6,7, 3,1,0,6, //19
	7,5,1,0, 2,6,0,6, //1a
	0,0,0,0, 0,0,0,0, //1b
	0,0,0,0, 0,0,0,0, //1c
	0,0,0,0, 0,0,0,0, //1d
	0,0,0,0, 0,0,0,0, //1e
	0,0,0,0, 0,0,0,0, //1f
	7,6,4,5, 0,0,0,4, //20
	0,4,5,7, 6,2,0,6, //21
	7,6,4,5, 1,3,0,6, //22
	0,0,0,0, 0,0,0,0, //23
	7,6,4,0, 1,5,0,6, //24
	0,1,5,7, 6,2,0,6, //25
	7,6,4,0, 1,3,0,6, //26
	0,0,0,0, 0,0,0,0, //27
	7,3,2,6, 4,5,0,6, //28
	0,4,5,7, 3,2,0,6, //29
	6,4,5,1, 3,2,0,6, //2a
	0,0,0,0, 0,0,0,0, //2b
	0,0,0,0, 0,0,0,0, //2c
	0,0,0,0, 0,0,0,0, //2d
	0,0,0,0, 0,0,0,0, //2e
	0,0,0,0, 0,0,0,0, //2f
	0,0,0,0, 0,0,0,0, //30
	0,0,0,0, 0,0,0,0, //31
	0,0,0,0, 0,0,0,0, //32
	0,0,0,0, 0,0,0,0, //33
	0,0,0,0, 0,0,0,0, //34
	0,0,0,0, 0,0,0,0, //35
	0,0,0,0, 0,0,0,0, //36
	0,0,0,0, 0,0,0,0, //37
	0,0,0,0, 0,0,0,0, //38
	0,0,0,0, 0,0,0,0, //39
	0,0,0,0, 0,0,0,0, //3a
	0,0,0,0, 0,0,0,0, //3b
	0,0,0,0, 0,0,0,0, //3c
	0,0,0,0, 0,0,0,0, //3d
	0,0,0,0, 0,0,0,0, //3e
	0,0,0,0, 0,0,0,0, //3f
};
#endif // !_LIB || _LAUNCHER
#endif // __platform_impl_h__
