////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   platform.h
//  Version:     v1.00
//  Created:     11/12/2002 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: Platform dependend stuff.
//               Include this file instead of windows.h
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#pragma once

#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0600
#endif

#if (_MSC_VER >= 1700)
	#include <Xkeycheck.h>
#endif

// Alignment support.
#if defined(_MSC_VER)
	#if (_MSC_VER >= 1700)
		#define _ALLOW_KEYWORD_MACROS
	#endif

	#define alignof _alignof
#elif defined(__GNUC__) || defined(CAFE)
	#define alignof __alignof__
#endif

#include <STLPortConfig.h>

#if defined(LINUX) || defined(APPLE)
#define _FILE_OFFSET_BITS 64 // define large file support > 2GB
#endif

// When using cry_rand() we need to make sure the value of RAND_MAX is always 0x7fff. 
// On UNIX platforms this is not the case.
#define CRY_RAND_MAX 0x7fff

// This variable is windows specific, yet used by some code.
// We just define it for Mac OS X and Linux
#if defined(LINUX64) || defined(MAC)
#define MEMORY_ALLOCATION_ALIGNMENT 16
#elif defined(LINUX32)
#define MEMORY_ALLOCATION_ALIGNMENT 8
#endif

#if !defined(PS3) && !defined(ORBIS)
	#define __FUNC__ __FUNCTION__
#else
	#define __FUNC__ __PRETTY_FUNCTION__
#endif

#if defined(_DEBUG) && !defined(PS3) && !defined(LINUX) && !defined(CAFE) && !defined(APPLE) && !defined(ORBIS)
	#include <crtdbg.h>
#endif

#if defined(PS3)
	// use a special heap for big (>64kb) allocations which can be physicaly freed to prevent fragmentation
	#define PS3_USE_SYSTEM_MEM_CONTAINER

	// use this define to disable the node allocator in CryMemoryManager.cpp
	#define PS3_DONT_USE_NODEALLOC

#ifndef _RELEASE
	#define MEM_MAN_ADD_SIZE_BLOCK_VMEM
#endif

//	#define CHECK_SIMD_ALIGNMENT_P(p) if(((unsigned int)(p) & 15) != 0) abort();
//	#define CHECK_SIMD_ALIGNMENT if(((unsigned int)this & 15) != 0) abort();
	#define CHECK_SIMD_ALIGNMENT_P(p)
	#define CHECK_SIMD_ALIGNMENT
	#ifndef __SPU__
		#define ppu_volatile volatile
		#define spu_volatile
	#else
		#define ppu_volatile
		#define spu_volatile volatile
	#endif
#else
	#define CHECK_SIMD_ALIGNMENT_P(p)
	#define CHECK_SIMD_ALIGNMENT
	#define ppu_volatile volatile
#endif

#ifdef __SPU__
	#define ppu_virtual
#else
	#define ppu_virtual virtual
#endif

#undef GCC411_OR_LATER
// we have to use it because of VS doesn't support restrict reference variables
#if defined(PS3) || defined(APPLE) || defined(LINUX)
	#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)
		#define GCC411_OR_LATER
	#endif
	#define RESTRICT_REFERENCE __restrict
	#define FLATTEN __attribute__ ((flatten))
#else
	#define __db16cycl__
	#define RESTRICT_REFERENCE
	#define FLATTEN
#endif


#if !defined(GCC411_OR_LATER) || defined(__SPU__)
	#define __vecreg
#endif

#ifndef CHECK_REFERENCE_COUNTS //define that in your StdAfx.h to override per-project
# define CHECK_REFERENCE_COUNTS 0 //default value
#endif

#if CHECK_REFERENCE_COUNTS
# define CHECK_REFCOUNT_CRASH(x) { if(!(x)) *((int*)0)=0;}
#else
# define CHECK_REFCOUNT_CRASH(x)
#endif

#ifndef GARBAGE_MEMORY_ON_FREE //define that in your StdAfx.h to override per-project
# define GARBAGE_MEMORY_ON_FREE 0 //default value
#endif

#if GARBAGE_MEMORY_ON_FREE
# ifndef GARBAGE_MEMORY_RANDOM			//define that in your StdAfx.h to override per-project
#  define GARBAGE_MEMORY_RANDOM 1	//0 to change it to progressive pattern
# endif
#endif

//////////////////////////////////////////////////////////////////////////
// Available predefined compiler macros for Visual C++.
//		_MSC_VER										// Indicates MS Visual C compiler version
//		_WIN32, _WIN64, _XBOX_VER		// Indicates target OS
//		_M_IX86, _M_PPC							// Indicates target processor
//		_DEBUG											// Building in Debug mode
//		_DLL												// Linking with DLL runtime libs
//		_MT													// Linking with multi-threaded runtime libs
//////////////////////////////////////////////////////////////////////////

//
// Translate some predefined macros.
//

// NDEBUG disables std asserts, etc.
// Define it automatically if not compiling with Debug libs, or with ADEBUG flag.
#if !defined(_DEBUG) && !defined(ADEBUG) && !defined(NDEBUG) && !defined(PS3)
	#define NDEBUG
#endif

#if defined(PS3) && defined(PS3OPT) && !defined(__SPU__)
	#define MATH_H <fastmath.h>
#else
	#define MATH_H <math.h>
#endif

#ifdef PS3
// Workaround for missing C++11 keyword on PS3
#define nullptr (void*)0
#endif

// Xenon target. (We generally use _XBOX but should really use XENON).
#if defined(_XBOX_VER)// && _XBOX_VER == 200
	#define XENON
#ifndef _XBOX
	#define _XBOX
#endif
	// Build static library when compiling release on Xenon
	#ifdef _RELEASE
		#define _LIB
	#endif
#endif

// Durango target.
#if defined(DURANGO)// && _XBOX_VER == 200

	// Build static library when compiling release on Durango
	#ifdef _RELEASE
		#define _LIB
	#endif
#endif

#if defined(ORBIS)
	#define _LIB
#endif

#if defined(DURANGO) || defined(ORBIS) || defined(PS3) || defined(XENON)
#define CONSOLE
#endif

//render thread settings, as this is accessed inside 3dengine and renderer and needs to be compile time defined, we need to do it here
//enable this macro to strip out the overhead for render thread
//	#define STRIP_RENDER_THREAD
#ifdef STRIP_RENDER_THREAD
	#define RT_COMMAND_BUF_COUNT 1
#else
	//can be enhanced to triple buffering, FlushFrame needs to be adjusted and RenderObj would become 132 bytes
	#define RT_COMMAND_BUF_COUNT 2
#endif


// We use WIN macros without _.
#if defined(_WIN32) && !defined(XENON) && !defined(DURANGO) && !defined(LINUX32) && !defined(LINUX64) && !defined(APPLE) && !defined(WIN32)
	#define WIN32
#endif
#if defined(_WIN64) && !defined(WIN64) && !defined(DURANGO)
	#define WIN64
#endif

// In Win32 Release we use static linkage
#ifdef WIN32
	#if defined(_RELEASE) && !defined(RESOURCE_COMPILER) && !defined(_FORCEDLL)
		#define _LIB
	#else //#if defined(_RELEASE) && !defined(RESOURCE_COMPILER) && !defined(_FORCEDLL)

		// All windows targets not in Release built as DLLs.
		#ifndef _USRDLL
			#define _USRDLL
		#endif
	#endif

#endif //WIN32

#if defined(XENON)
	#define __passinreg __declspec(passinreg)
	#define __passinreg_vec __declspec(passinreg)
#elif defined(PS3) && !defined(__CRYCG__) && !defined(__SNC__) && !defined(__SPU__)
	#define __passinreg
	#define __passinreg_vec __attribute__((d64_abi)) __attribute__((vecreturn))
#else
	#define __passinreg
	#define __passinreg_vec
#endif

#if defined(LINUX) || defined(APPLE) || defined(PS3) || defined(CAFE)
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#if defined(APPLE) || defined(LINUX64)
    // int64 is not the same type as the operating system's int64_t 
    #undef PRIX64
    #undef PRIx64
    #undef PRId64
    #undef PRIu64
    #define PRIX64 "llX"
    #define PRIx64 "llx"
    #define PRId64 "lld"
    #define PRIu64 "llu"    
#endif
#define PLATFORM_I64(x) x##ll
#elif defined(ORBIS)
#define PRIX64 "llX"
#define PRIx64 "llx"
#define PRId64 "lld"
#define PRIu64 "llu"
#define PLATFORM_I64(x) x##ll
#else
#define PRIX64 "I64X"
#define PRIx64 "I64x"
#define PRId64 "I64d"
#define PRIu64 "I64u"
#define PLATFORM_I64(x) x##i64
#endif

#if !defined(PRISIZE_T)
    #if defined(WIN64)
        #define PRISIZE_T "I64u" //size_t defined as unsigned __int64
    #elif defined(WIN32) || defined(XENON) || defined(PS3) || defined(LINUX32)
        #define PRISIZE_T "u"
    #elif defined(MAC) || defined(LINUX64) || defined(ORBIS) || defined (DURANGO)
        #define PRISIZE_T "lu"
    #else
        #error "Please defined PRISIZE_T for this platform"
    #endif
#endif
#if !defined(PRI_THREADID)
    #if defined(MAC) || defined(ORBIS)
        #define PRI_THREADID "lld" 
    #elif defined(LINUX64) 
        #define PRI_THREADID "ld"
    #else
        #define PRI_THREADID "d"
    #endif 
#endif
#include "ProjectDefines.h"							// to get some defines available in every CryEngine project
#include "ExtensionDefines.h"

//////////////////////////////////////////////////////////////////////////
// Include standard CRT headers used almost everywhere.
//////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
//////////////////////////////////////////////////////////////////////////

// Function attribute for printf/scanf-style parameters.
// This enables extended argument checking by GCC.
//
// Usage:
// Put this after the function or method declaration (not the definition!),
// between the final closing parenthesis and the semicolon.
// The first parameter indicates the 1-based index of the format string
// parameter, the second parameter indicates the 1-based index of the first
// variable parameter.  Example:
//   void foobar(int a, const char *fmt, ...) PRINTF_PARAMS(2, 3);
//
// For va_list based printf style functions, specfy 0 as the second parameter.
// Example:
//   void foobarv(int a, const char *fmt, va_list ap) PRINTF_PARAMS(2, 0);
//
// Note that 'this' is counted as a method argument. For non-static methods,
// add 1 to the indices.

#if !defined(__clang__) && defined(__GNUC__) && !defined(__SPU__) && !defined(_RELEASE)
  #define PRINTF_PARAMS(...) __attribute__ ((format (printf, __VA_ARGS__)))
  #define SCANF_PARAMS(...) __attribute__ ((format (scanf, __VA_ARGS__)))
#else
  #define PRINTF_PARAMS(...)
	#define SCANF_PARAMS(...)
#endif

// Storage class modifier for thread local storage.
#if defined(CAFE)
	#define THREADLOCAL __unsupported_on_wiiu__ 
#elif defined(APPLE)
	#define THREADLOCAL __unsupported_on_mac__
#elif defined(__GNUC__)
	#define THREADLOCAL __thread
#else
	#define THREADLOCAL __declspec(thread)
#endif



//////////////////////////////////////////////////////////////////////////
// define Read Write Barrier macro needed for lockless programming
//////////////////////////////////////////////////////////////////////////
#if (defined(PS3) && !defined(__SPU__)) || defined(XENON)
	#define READ_WRITE_BARRIER {__lwsync();}
	//#pragma intrinsic(_ReadWriteBarrier)
	//#define READ_WRITE_BARRIER \
		//_ReadWriteBarrier() \
		//AcquireLockBarrier()
#else
	#define READ_WRITE_BARRIER
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// define macro to prevent memory reoderings of reads/and writes
#if defined(PS3) && !defined(__SPU__)	
		#define MEMORY_RW_REORDERING_BARRIER do{ NIntrinsics::MemoryFence(); }while(0)	
#else
	#define MEMORY_RW_REORDERING_BARRIER do{/*not implemented*/}while(0)
#endif

//default stack size for threads, currently only used on pthread platforms
#ifdef ORBIS
#define SIMPLE_THREAD_STACK_SIZE_KB (256)
//default stack size for threads, currently only used on PS3
#elif defined(LINUX) || defined(APPLE)
#if !defined(_DEBUG)
#define SIMPLE_THREAD_STACK_SIZE_KB (256)
#else
#define SIMPLE_THREAD_STACK_SIZE_KB (256 * 4)
#endif
#else
#define SIMPLE_THREAD_STACK_SIZE_KB (32)
#endif


// DLL import / export
#if defined(PS3)
	#undef _MSC_VER
	#undef WIN32
	#undef WIN64
#endif

#if defined(__GNUC__) || defined(PS3)
	#define DLL_EXPORT __attribute__ ((visibility("default")))
	#define DLL_IMPORT __attribute__ ((visibility("default")))
#elif defined(XENON) && !defined(SOFTCODE_ENABLED)
	#define DLL_EXPORT 
	#define DLL_IMPORT 
#else
	#ifdef _LIB
		#define DLL_EXPORT
		#define DLL_IMPORT
	#else //_LIB
		#define DLL_EXPORT __declspec(dllexport)
		#define DLL_IMPORT __declspec(dllimport)
	#endif //_LIB
#endif

//////////////////////////////////////////////////////////////////////////
// Define BIT macro for use in enums and bit masks.
#define BIT(x) (1<<(x))
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Help message, all help text in code must be wrapped in this define.
// Only include these in non RELEASE builds
#if !defined(_RELEASE)
	#define _HELP(x) x
#else
	#define _HELP(x) ""
#endif
//////////////////////////////////////////////////////////////////////////

//will be defined for SPUs and PS3 therefore only
#ifndef SPU_DEBUG_BREAK
	#define SPU_DEBUG_BREAK
#endif

//////////////////////////////////////////////////////////////////////////
// Globally Used Defines.
//////////////////////////////////////////////////////////////////////////
// CPU Types: _CPU_X86,_CPU_AMD64,_CPU_G5
// Platform: WIN23,WIN64,LINUX32,LINUX64,MAC,_XBOX
// CPU supported functionality: _CPU_SSE
//////////////////////////////////////////////////////////////////////////

#ifdef XENON
  #define PREFAST_SUPPRESS_WARNING(W) __pragma(warning(suppress:W))
#else
  #if defined(_MSC_VER)
    #include "MSVCspecific.h"
    #define PREFAST_SUPPRESS_WARNING(W) __pragma(warning(suppress:W))
  #else
    #define PREFAST_SUPPRESS_WARNING(W) 
  #endif
#endif


#if defined(WIN32) && !defined(WIN64) && !defined(DURANGO)
#include "Win32specific.h"
#endif

#if defined(WIN64) && !defined(DURANGO)
#include "Win64specific.h"
#endif

#if defined(LINUX64)
#include "Linux64Specific.h"
#endif

#if defined(LINUX32)
#include "Linux32Specific.h"
#endif

#if defined(XENON)
#include "XenonSpecific.h"
#endif

#if defined(PS3)
#include "PS3Specific.h"
#endif

#if defined(DURANGO)
#include "Durangospecific.h"
#endif

#if defined(ORBIS)
#define CRYPLATFROM_ALLOW_DETAIL_INCLUDES
#include "OrbisSpecific.h"
#include "CryPlatform.Orbis.h"
#endif 

#if defined(APPLE)
#include "MacSpecific.h"
#endif

#if !defined(TARGET_DEFAULT_ALIGN)
# error "No default alignment specified for target architecture"
#endif 

// Indicates potentially dangerous cast on 64bit machines
typedef	UINT_PTR TRUNCATE_PTR;
typedef	UINT_PTR EXPAND_PTR;

#if !defined(PS3)
//dummy definitions to avoid ifdef's
ILINE void SPUAddCacheWriteRangeAsync(const unsigned int, const unsigned int){}
#define __cache_range_write_async(a,b)
#define __spu_flush_cache_line(a)
#define __flush_cache_range(a,b)
#define __flush_cache()

#undef IF
#undef WHILE
#undef IF_UNLIKELY	
#undef IF_LIKELY
#define IF(a, b) if((a))
#define WHILE(a, b) while((a))
#define IF_UNLIKELY(a) if((a))
#define IF_LIKELY(a) if((a))
#endif //!defined(PS3)

#include <stdio.h>

// Includes core CryEngine modules definitions.
#include "CryModuleDefs.h"


//////////////////////////////////////////////////////////////////////////
// Provide special cast function which mirrors C++ style casts to support aliasing correct type punning casts in gcc with strict-aliasing enabled
template<typename DestinationType, typename SourceType>
ILINE DestinationType alias_cast( SourceType pPtr )
{
	//STATIC_CHECK(sizeof(DestinationType) == sizeof(SourceType), ALIAS_CAST_BOTH_TYPES_NEED_TO_BE_OF_THE_SAME_SIZE );
#if defined(__SPU__)
	return reinterpret_cast<DestinationType>(pPtr);
#else
	union { 
		SourceType pSrc;
		DestinationType pDst;
	} conv_union;
	conv_union.pSrc = pPtr;
	return conv_union.pDst;
#endif
}

#if defined(__SPU__)
template<>
ILINE uint32 alias_cast<uint32, float>( float pPtr )
{
	return *reinterpret_cast<uint32*>(&pPtr);
}
#endif

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// CryModule memory manager routines must always be included.
// They are used by any module which doesn't define NOT_USE_CRY_MEMORY_MANAGER
// No Any STL includes must be before this line.
//////////////////////////////////////////////////////////////////////////
#if 1  //#ifndef NOT_USE_CRY_MEMORY_MANAGER
#define USE_NEWPOOL
#include "CryMemoryManager.h"
#else
inline int IsHeapValid()
{
#if defined(_DEBUG) && !defined(RELEASE_RUNTIME) && !defined(XENON) && !defined(PS3)
	return _CrtCheckMemory();
#else
	return true;
#endif
}
#endif // NOT_USE_CRY_MEMORY_MANAGER

// Memory manager breaks strdup
// Use something higher level, like CryString
// PS3 headers require this, does not compile otherwise
#if !defined(PS3)
	#undef strdup
	#define strdup dont_use_strdup
	#undef snPause
	#define snPause()
#endif

#ifdef PS3
//defines necessary stuff for SPU Software Cache
//needs to be included for all platforms (mostly empty decls. there)
#include "PS3CryCache.h"
#else
#define __CRYCG_NOINLINE__
#define __CRYCG_IGNORE_PARAM_MISMATCH__
#define SPU_MAIN_PTR(PTR) (PTR)
#define SPU_MAIN_REF(REF) (REF)
#define SPU_LOCAL_PTR(PTR) (PTR)
#define SPU_LOCAL_REF(REF) (REF)
#define SPU_LINK_PTR(PTR, LINK) (PTR)
#define SPU_LINK_REF(REF, LINK) (PTR)
#define SPU_DOMAIN_MAIN
#define SPU_DOMAIN_LOCAL
#define SPU_DOMAIN_LINK(ID)
#define SPU_VERBATIM_BLOCK(X) ((void)0)
#define SPU_FRAME_PROFILER(X){}
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef DEPRICATED
#define DEPRICATED
#endif

//////////////////////////////////////////////////////////////////////////
// compile time error stuff
//////////////////////////////////////////////////////////////////////////
template<bool> struct CompileTimeError;
template<> struct CompileTimeError<true> {};
#undef STATIC_CHECK 
#define STATIC_CHECK(expr, msg) \
	{ CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

// Assert dialog box macros
#include "CryAssert.h"

// Replace standard assert calls by our custom one
// Works only ifdef USE_CRY_ASSERT && _DEBUG && WIN32
#ifndef assert
#define assert CRY_ASSERT
#endif

#include "CompileTimeAssert.h"
//////////////////////////////////////////////////////////////////////////
// Platform dependent functions that emulate Win32 API.
// Mostly used only for debugging!
//////////////////////////////////////////////////////////////////////////
void   CryDebugBreak();
void   CrySleep( unsigned int dwMilliseconds );
void   CryLowLatencySleep( unsigned int dwMilliseconds );
int    CryMessageBox( const char *lpText,const char *lpCaption,unsigned int uType);
int    CryCreateDirectory( const char *lpPathName,void *lpSecurityAttributes );
int    CryGetCurrentDirectory( unsigned int nBufferLength,char *lpBuffer );
short  CryGetAsyncKeyState( int vKey );
unsigned int CryGetFileAttributes( const char *lpFileName );

#if defined(LINUX) || defined(APPLE)
#define CrySwprintf swprintf
#else
#define CrySwprintf _snwprintf
#endif

inline void CryHeapCheck()
{
#if !defined(LINUX) && !defined (PS3) && !defined (DURANGO) && !defined(ORBIS) && !defined(APPLE) // todo: this might be readded with later xdks?
  int Result = _heapchk();
  assert(Result!=_HEAPBADBEGIN);
  assert(Result!=_HEAPBADNODE);
  assert(Result!=_HEAPBADPTR);
  assert(Result!=_HEAPEMPTY);
  assert(Result==_HEAPOK);
#endif
}

//---------------------------------------------------------------------------
// Pointer classification functions 
// 
// Note: Do not use the result of these functions to enable or disable conditional codepaths 
// because they are not available on all platforms. Only use these to verify assumptions on more 
// just one platform (e.g To ensure not to stack pointers into code paths that could be affected by 
// spu jobs because they would cause mfc segment exceptions on their first access)
#if !defined(RELEASE)
#ifndef PLATFORM_HAS_SEGMENT_POINTER_CHECK
# define PLATFORM_HAS_SEGMENT_POINTER_CHECK 0
#endif 
#ifndef PLATFORM_HAS_STACK_POINTER_CHECK
# define PLATFORM_HAS_STACK_POINTER_CHECK 0
#endif 

// Returns not null 0 if the passed pointer comes from the heap.
// Note: currently only available on ps3.
inline unsigned IsPointerFromHeap(void* __ptr)
{
#if PLATFORM_HAS_SEGMENT_POINTER_CHECK && PLATFORM_HAS_STACK_POINTER_CHECK
  return _IsPointerFromSegment(__ptr) && !_IsPointerFromStack(__ptr);
#else
  return 1; 
#endif 
}

// Returns not null if the passed pointer resides on the stack of the
// current thread 
// Note: currently only available on ps3
inline unsigned IsPointerFromStack(void* __ptr)
{
#if PLATFORM_HAS_STACK_POINTER_CHECK
  return _IsPointerFromStack(__ptr);
#else
  return 0; 
#endif 
}

// Returns not null if the passed pointer resides in the code, data or
// bss segment of the executable.
// Note: currently only available on ps3
inline unsigned IsPointerFromSegment(void* __ptr)
{
#if PLATFORM_HAS_SEGMENT_POINTER_CHECK
  return _IsPointerFromSegment(__ptr);
#else
  return 0; 
#endif 
}
#endif 


// Useful function to clean the structure.
template <class T>
inline void ZeroStruct( T &t ) { memset( &t,0,sizeof(t) ); }

// Useful functions to init new memory.
template<class T>
inline void Construct(T& t)		
	{ new(&t) T(); }

template<class T, class U>
inline void Construct(T& t, U const& u)
	{	new(&t) T(u); }

// Cast one type to another, asserting there is no conversion loss.
// Usage: DestType dest = check_cast<DestType>(src);
template<class D, class S>
inline D check_cast(S const& s)
{
	D d = D(s);
	assert(S(d) == s);
	return d;
}

// Convert one type to another, asserting there is no conversion loss.
// Usage: DestType dest;  check_convert(dest, src);
template<class D, class S>
inline D& check_convert(D& d, S const& s)
{
	d = D(s);
	assert(S(d) == s);
	return d;
}

// Convert one type to another, asserting there is no conversion loss.
// Usage: DestType dest;  check_convert(dest) = src;
template<class D>
struct CheckConvert
{
	CheckConvert(D& d)
		: dest(&d) {}

	template<class S>
	D& operator=(S const& s)
	{
		return check_convert(*dest, s);
	}

protected:
	D*	dest;
};

template<class D>
inline CheckConvert<D> check_convert(D& d)
{
	return d;
}

//---------------------------------------------------------------------------
// Use NoCopy as a base class to easily prevent copy init & assign for any class.
struct NoCopy
{
	NoCopy() {}
private:
	NoCopy(const NoCopy&);
	NoCopy& operator =(const NoCopy&);
};

//---------------------------------------------------------------------------
// Quick const-manipulation macros

// Declare a const and variable version of a function simultaneously.
#define CONST_VAR_FUNCTION(head, body) \
	inline head body \
	inline const head const body

template<class T> inline
T& non_const(const T& t)
	{ return const_cast<T&>(t); }

#define using_type(super, type) \
	typedef typename super::type type;

typedef unsigned char	uchar;
typedef unsigned int uint;
typedef const char* cstr;

//---------------------------------------------------------------------------
// Align function works on integer or pointer values.
// Only support power-of-two alignment.
template<typename T> inline
T Align(T nData, size_t nAlign)
{
	assert((nAlign & (nAlign-1)) == 0);
	size_t size = ((size_t)nData + (nAlign-1)) & ~(nAlign-1);
	return T(size);
}

template<typename T> inline
bool IsAligned(T nData, size_t nAlign)
{
	assert((nAlign & (nAlign-1)) == 0);
	return (size_t(nData) & (nAlign-1)) == 0;
}

#if !defined(_SPU_JOB)
#if !defined(NOT_USE_CRY_STRING) 
  #include "CryString.h"
  #if !defined(RESOURCE_COMPILER)
		typedef CryStringT<char> string;	
		typedef CryStringT<wchar_t> wstring;
	#else
		typedef CryStringLocalT<char> string;
		typedef CryStringLocalT<wchar_t> wstring;
	#endif
#else // NOT_USE_CRY_STRING
	#ifndef __SPU__
		#include <string>				// STL string
		typedef std::string string;
		typedef std::wstring wstring;
	#endif
#endif // NOT_USE_CRY_STRING

#include <functional>

// The 'string_less' class below provides less functor implementation for
// 'string' supporting direct comparison against plain C strings and stack
// strings.  This is most effective in combination with STLPORT, where various
// 'find' and related methods are templated on the parameter type.  For
// STLPORT, 'string_less' will be used as a specialization for
// 'std::less<string>'.

struct string_less
{
  bool operator ()(const string &s1, const char *s2) const
  {
    return s1.compare(s2) < 0;
  }
  bool operator ()(const char *s1, const string &s2) const
  {
    return s2.compare(s1) > 0;
  }
  bool operator ()(const string &s1, const string &s2) const
  {
    return s1 < s2;
  }

#if !defined(NOT_USE_CRY_STRING)
  template <size_t S>
  bool operator()(const string &s1, const CryStackStringT<char, S> &s2) const
  {
    return s1.compare(s2.c_str()) < 0;
  }
  template <size_t S>
  bool operator()(const CryStackStringT<char, S> &s1, const string &s2) const
  {
    return s1.compare(s2.c_str()) < 0;
  }
#endif // !defined(NOT_USE_CRY_STRING)
};

#if defined(USING_STLPORT)
namespace std
{
  template <> struct less< ::string > : public string_less { };
}
#endif // defined(USING_STLPORT)

// Include support for meta-type data.
#include "TypeInfo_decl.h"

// Include array.
#include <CryArray.h>

// Wrapper code for non-windows builds.
#if defined(LINUX) || defined(APPLE)
	#include "Linux_Win32Wrapper.h"
#endif
#if defined(PS3)
	#include "PS3_Win32Wrapper.h"
#endif
#if defined(DURANGO)
	#include "Durango_Win32Legacy.h"
#endif

#if defined(ORBIS)
	#include "Orbis_Win32Wrapper.h"
#endif

#if defined(PS3)
	//avoid overhead of yet another function indirection
	#define CryGetCurrentThreadId GetCurrentThreadId
	#define CrySetFileAttributes(lpFileName,dwFileAttributes)
#else
	bool   CrySetFileAttributes( const char *lpFileName,uint32 dwFileAttributes );
	threadID CryGetCurrentThreadId();
#endif

#if !defined(NOT_USE_CRY_STRING) && (!defined(__SPU__) || defined(__CRYCG__))
	// Fixed-Sized (stack based string)
	// put after the platform wrappers because of missing wcsicmp/wcsnicmp functions
	#include "CryFixedString.h"
#endif

#if defined(DURANGO)
// Dec2012 XDK: MoveFileEx still in header but there's no MoveFileExA in kernelx.lib
#undef MoveFileEx

inline BOOL MoveFileEx(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, DWORD dwFlags)
{
	size_t nChars = 0;
	const uint32 STR_LEN = 128;
	CryStackStringT<wchar_t, STR_LEN> wsExistingFileName;
	const size_t existingFileNameLength = strlen(lpExistingFileName);
	wsExistingFileName.resize(existingFileNameLength);
	mbstowcs_s( &nChars, wsExistingFileName.m_str, existingFileNameLength, lpExistingFileName, STR_LEN );
	assert(nChars>0);

	CryStackStringT<wchar_t, STR_LEN> wsNewFileName;
	const size_t newFileNameLength = strlen(lpNewFileName);
	wsNewFileName.resize(newFileNameLength);
	mbstowcs_s( &nChars, wsNewFileName.m_str, newFileNameLength, lpNewFileName,  STR_LEN );
	assert(nChars>0);

	return MoveFileExW(wsExistingFileName.c_str(), wsNewFileName.c_str(), dwFlags);
}
#endif

// need this in a common header file and any other file would be too misleading
enum ETriState
{
	eTS_false,
	eTS_true,
	eTS_maybe
};

#endif // !defined (_SPU_JOB)

#define SAFE_DELETE_VOID_ARRAY(p) { if(p) { delete[] (unsigned char*)(p);   (p)=NULL; } }

//---------------------------------------------------------------------------
// Facilities for collecting options in a struct, and quickly constructing them.
// Used, for example, as construction argument. Safer and more informative than bool arguments.
//
// Example:
//		struct FObjectOpts;
//		struct CObject { CObject(FObjectOpts = ZERO); };
//		CObject object_def();
//		CObject object( FObjectOpts().Size(8).AllowGrowth(true) );

#define OPT_STRUCT(Struc) \
	typedef Struc TThis; \
	Struc(void* p = 0) { ZeroStruct(*this); }

#define VAR_OPT(Type, Var) \
	Type _##Var; \
	Type const& Var() const { return _##Var; } \
	TThis& Var(Type const& val) { _##Var = val; return *this; }

#define BIT_STRUCT(Struc, Int) \
	typedef Struc TThis; typedef Int TInt; \
	TInt Mask() const { return *(const TInt*)this; } \
	TInt& Mask() { return *(TInt*)this; } \
	Struc(TInt init = 0) { COMPILE_TIME_ASSERT(sizeof(TThis) == sizeof(TInt)); Mask() = init; }

#define BIT_OPT(Var) \
	TInt _##Var: 1; \
	TInt Var() const { return _##Var; } \
	TThis& Var(TInt val) { _##Var = val; return *this; }

//---------------------------------------------------------------------------

#if defined __CRYCG__
	#ifndef SPU_ENTRY
		#define SPU_ENTRY(job_name) __attribute__ ((crycg_attr (entry, "job = " #job_name)))	
	#endif
	#ifndef SPU_INDIRECT
		#define SPU_INDIRECT(...) __attribute__ ((crycg_attr (entry, "indirect = " #__VA_ARGS__)))
	#endif
	#ifndef SPU_INDIRECT_TAG
    #define SPU_INDIRECT_TAG(tag) __attribute__ ((crycg_attr (indirect_tag,#tag)))
	#endif
	#ifndef SPU_LOCAL
    #define SPU_LOCAL __attribute__ ((crycg_attr (spu_local,"__ALL__")))
	#endif
	#ifndef SPU_LOCAL_FOR_JOBS
    #define SPU_LOCAL_FOR_JOBS(...) __attribute__ ((crycg_attr (spu_local,__VA_ARGS__)))
	#endif
	#ifndef SPU_EXTERN
		#define SPU_EXTERN __attribute__ ((crycg_attr ("spu_extern"))) extern
	#endif
	#ifndef SPU_DRIVER
		#define SPU_DRIVER(...) __attribute__ ((crycg_attr (driver, #__VA_ARGS__)))
  #endif
	#ifndef SPU_DEBUG
		#define SPU_DEBUG __attribute__ ((crycg_attr ("noinline"))) __attribute__ ((crycg_attr ("spu_debug")))
	#endif
	#ifndef SPU_NO_INLINE
		#define SPU_NO_INLINE __attribute__ ((crycg_attr ("noinline")))
	#endif
#else
	#ifndef SPU_ENTRY
		#define SPU_ENTRY(job_name)
	#endif
	#ifndef SPU_INDIRECT
		#define SPU_INDIRECT(...)
	#endif
	#ifndef SPU_INDIRECT_TAG
		#define SPU_INDIRECT_TAG(tag) 
	#endif
	#ifndef SPU_LOCAL 
		#define SPU_LOCAL 
	#endif
	#ifndef SPU_LOCAL_FOR_JOBS
    #define SPU_LOCAL_FOR_JOBS(...)
	#endif
	#ifndef SPU_EXTERN
		#define SPU_EXTERN extern
	#endif
	#ifndef SPU_DRIVER
		#define SPU_DRIVER(...)
	#endif
	#ifndef SPU_DEBUG
		#define SPU_DEBUG
	#endif
	#ifndef SPU_NO_INLINE
		#define SPU_NO_INLINE
	#endif
	#ifndef SPU_DEFAULT_TO_LOCAL
		#define SPU_DEFAULT_TO_LOCAL(...)
	#endif
#endif

#ifdef PS3
	#ifdef __CRYCG__
		#ifndef SPU_DEFAULT_TO_LOCAL
		  #define SPU_DEFAULT_TO_LOCAL(...) __attribute__ ((crycg_attr(default_domain,#__VA_ARGS__)))
		#endif
		#define NO_INLINE __attribute__ ((noinline)) __attribute__ ((crycg_attr ("noinline")))
	#else
		#define NO_INLINE __attribute__ ((noinline))
		#define SPU_DEFAULT_TO_LOCAL(...)
	#endif
#else
	#ifdef __GNUC__
		#define NO_INLINE __attribute__ ((noinline))
	#else
		#define NO_INLINE _declspec(noinline)
	#endif
#endif

// NO_INLINE_WEAK
// markes a function as no_inline, but also as weak to prevent multiple-defined errors
#ifdef PS3	
	#ifdef __CRYCG__
		#define NO_INLINE_WEAK __attribute__ ((noinline)) __attribute__ ((crycg_attr ("noinline")))
	#else
		#define NO_INLINE_WEAK __attribute__ ((noinline)) __attribute__((weak))
	#endif
#else
	#ifdef __GNUC__
		#define NO_INLINE_WEAK __attribute__ ((noinline)) __attribute__((weak))
	#else
		#define NO_INLINE_WEAK _declspec(noinline) inline
	#endif
#endif

//only for PS3 this will take effect, win32 does not support alignment
#if !defined(_ALIGN)
	#if defined(PS3)
		#define _ALIGN(num) __attribute__ ((aligned(num)))
	#else
		#define _ALIGN(num) 
	#endif
#endif

//Align for Win and Xbox360
#if !defined(_MS_ALIGN)
		#define _MS_ALIGN(num) 
#endif

#if defined(WIN32) || defined(WIN64) || defined(XENON)
#ifndef XENON
	extern "C" {
		__declspec(dllimport) unsigned long __stdcall TlsAlloc();
		__declspec(dllimport) void* __stdcall TlsGetValue(unsigned long dwTlsIndex);
		__declspec(dllimport) int __stdcall TlsSetValue(unsigned long dwTlsIndex, void* lpTlsValue);
	}
#endif

	#define TLS_DECLARE(type,var) extern int var##idx;
	#define TLS_DEFINE(type,var) \
	int var##idx; \
	struct Init##var { \
		Init##var() { var##idx = TlsAlloc(); } \
	}; \
	Init##var g_init##var;
	#define TLS_DEFINE_DEFAULT_VALUE(type,var,value) \
	int var##idx; \
	struct Init##var { \
		Init##var() { var##idx = TlsAlloc(); TlsSetValue(var##idx,(void*)(value));} \
	}; \
	Init##var g_init##var;
	#define TLS_GET(type,var) (type)TlsGetValue(var##idx)
	#define TLS_SET(var,val) TlsSetValue(var##idx,(void*)(val))
#elif defined(LINUX)
	#define TLS_DECLARE(type,var) extern __thread type var;
	#define TLS_DEFINE(type,var) __thread type var = 0;
	#define TLS_DEFINE_DEFAULT_VALUE(type,var,value) __thread type var = value;
	#define TLS_GET(type,var) (var)
	#define TLS_SET(var,val) (var=(val))
#elif defined(APPLE)
	#define TLS_DECLARE(_TYPE, _VAR) extern SCryMacTLS<_TYPE> _VAR##TLSKey;
	#define TLS_DEFINE(_TYPE, _VAR) SCryMacTLS<_TYPE> _VAR##TLSKey;
	#define TLS_DEFINE_DEFAULT_VALUE(_TYPE, _VAR, _DEFAULT) SCryMacTLS<_TYPE> _VAR##TLSKey = _DEFAULT;
	#define TLS_GET(_TYPE, _VAR) _VAR##TLSKey.Get()
	#define TLS_SET(_VAR, _VALUE) _VAR##TLSKey.Set(_VALUE)
#else
	#define TLS_DECLARE(type,var) extern THREADLOCAL type var;
	#define TLS_DEFINE(type,var) THREADLOCAL type var;
	#define TLS_DEFINE_DEFAULT_VALUE(type,var,value) THREADLOCAL type var = value;
	#define TLS_GET(type,var) (var)
	#define TLS_SET(var,val) (var=(val))
#endif // defined(LINUX)

	//provide empty macros for non-ps3 systems which don't need these
#if !defined(PS3) && !defined(ORBIS)
	#define FILE_IO_WRAPPER_NO_PATH_ADJUSTMENT
	#define FILE_IO_WRAPPER_STREAMING_FILE
	#define FILE_IO_WRAPPER_DIRECT_ACCESS_FILE
#endif

#ifndef PS3_NO_TAIL_PADDING
	// Use this macro at the end of a struct/class definition to prevent
	// GCC from overlaying the tail padding of the class with data members
	// from other classes.  The macro adds an empty array member which is
	// aligned to the alignment of the containing class.  Using this macro
	// does _not_ affect the size of the containing class.
	#define PS3_NO_TAIL_PADDING(CLASS)
#endif

#ifndef PS3_ALIGNMENT_BARRIER
	// Enforce the specified alignment onto the next data member of the
	// containing class.  When used at the end of the class, this macro
	// eliminates tail padding up to the specified alignment.
        //
        // Note: At most one alignment barrier may be used per class.
	#define PS3_ALIGNMENT_BARRIER(ALIGN)
#endif

#ifndef UNIQUE_IFACE
	// UNIQUE_IFACE expands to nothing on _all_ platforms, but is recognized by
	// the de-virtualization tool.  If placed in front of an interface
	// struct/class, all methods are assumed to be unique-virtual.
  #define UNIQUE_IFACE
#endif

#ifndef VIRTUAL
	// VIRTUAL should be used for pure virtual methods with only a single
	// implementation - it should be applied to both the method
	// declaration/definition in the interface _and_ the implementation (as an
	// alternative the 'virtual' keyword may be omitted from the
	// implementation).  On platforms not using automatic de-virtualization
	// VIRTUAL simply expands to 'virtual'.
  #define VIRTUAL virtual
#endif

#ifndef UNIQUE_VIRTUAL_WRAPPER
	// Expands to the name if the wrapper include file for an interface
	// containing unique-virtual methods.  Expands to an empty include file on
	// platforms not supporting de-virtualization.
  #define UNIQUE_VIRTUAL_WRAPPER(NAME) <NoIfaceWrapper.h>
	#include <NoIfaceWrapper.h>
#endif

#ifndef DEVIRTUALIZE_HEADER_FIX
	// Expands to hea1er when using devirtualization, this ensures that only
	// devirtualized header files are included. Expands to an empty include file on
	// platforms not supporting de-virtualization.
	#define DEVIRTUALIZE_HEADER_FIX( HEADER ) <NoIfaceWrapper.h>
	#include <NoIfaceWrapper.h>
#endif

#ifndef DEVIRTUALIZATION_VTABLE_FIX
	// Expands  to a dummy function declaration used to force generation of a vtable in a
	// translation unit. Expands to nothing if the plattform doesn't support devirtualization
	#define DEVIRTUALIZATION_VTABLE_FIX
#endif

#ifndef DEVIRTUALIZATION_VTABLE_FIX_IMPL
	// Expands to a dummy function definition to get the compiler to generate a vtable object in the
	// translation unit. Expands to nothing if the plattform doesn't support devirtualization
	#define DEVIRTUALIZATION_VTABLE_FIX_IMPL( CLASS )
#endif

// check if the function is overriding with exactly same signature
#ifndef OVERRIDE
	#if defined(WIN32) || defined(XENON) || defined(DURANGO)
		#define OVERRIDE override
	#else
		#define OVERRIDE
	#endif
#endif

// to prevent inheriting from classes or overriding function in derived class
#ifndef FINAL
	#if defined(WIN32) || defined(XENON) || defined(DURANGO)
		#if _MSC_VER >= 1700
			#define FINAL final
		#else
			#define FINAL sealed
		#endif
	#else
		#define FINAL
	#endif
#endif


//memory transfer operations proxys for non SPU compilation (also for PC)
#ifndef __SPU__ 
	#define memtransfer_from_main(pDest, pSrc, cSize, cSyncPointID) memcpy(pDest, pSrc, cSize)
	#define memtransfer_to_main(pDest, pSrc, cSize, cSyncPointID) memcpy(pDest, pSrc, cSize)
	#define memtransfer_from_main_fenced memtransfer_from_main
	#define memtransfer_to_main_fenced memtransfer_to_main
	#define memtransfer_sync(cSyncPointID) (void)(cSyncPointID)
	#define memtransfer_pending(id) false
	#define __spu_flush_cache()
	#define __spu_cache_barrier()
#endif

#if !defined(LINUX) && !defined(APPLE) && !defined(PS3) && !defined(ORBIS)
	typedef int socklen_t;
#endif


// Include MultiThreading support.
#ifndef __SPU__ 
#  include "CryThread.h"
#endif
#include "MultiThread.h"

//////////////////////////////////////////////////////////////////////////
// Include most commonly used STL headers
// They end up in precompiled header and make compilation faster.
// 
// Note: Don't include them when spu code is compiled!
//////////////////////////////////////////////////////////////////////////
#if !defined(_SPU_JOB)
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <algorithm>
#include <functional>
#include <iterator>
#include "stl/STLAlignedAlloc.h"
#endif
//////////////////////////////////////////////////////////////////////////

// In RELEASE disable printf and fprintf
#if defined(_RELEASE) && (defined(PS3) || defined(XENON) || defined(DURANGO)) && !defined(RELEASE_LOGGING)
  #undef printf
  #define printf(...) (void) 0
  #undef fprintf
  #define fprintf(...) (void) 0
  #undef OutputDebugString
  #define OutputDebugString(...) (void) 0
#endif

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#if defined(WIN32) || defined(WIN64) || defined(XENON) || defined(DURANGO)
#define MESSAGE(msg) message(__FILE__ "(" STRINGIFY(__LINE__) "): " msg)
#else
#define MESSAGE(msg)
#endif

#endif // _PLATFORM_H_
