////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   Win32specific.h
//  Version:     v1.00
//  Created:     31/03/2003 by Sergiy.
//  Compilers:   Visual Studio.NET
//  Description: Specific to Win32 declarations, inline functions etc.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef _CRY_COMMON_WIN64_SPECIFIC_HDR_
#define _CRY_COMMON_WIN64_SPECIFIC_HDR_

#define _CPU_AMD64
//#define _CPU_SSE
#define ILINE __forceinline

#define DEBUG_BREAK CryDebugBreak()
#define RC_EXECUTABLE "rc.exe"
#define DEPRICATED __declspec(deprecated)
#define TYPENAME(x) typeid(x).name()
#define SIZEOF_PTR 8

#ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x501
#endif

#pragma warning( disable : 4267 ) //warning C4267: 'initializing' : conversion from 'size_t' to 'unsigned int', possible loss of data

//////////////////////////////////////////////////////////////////////////
// Standard includes.
//////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <io.h>
#include <new.h>
#include <direct.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////////

// Special intrinsics
#include <math.h> // Should be included before intrin.h
#include <intrin.h>

//////////////////////////////////////////////////////////////////////////
// Define platform independent types.
//////////////////////////////////////////////////////////////////////////
#include "BaseTypes.h"

#define THREADID_NULL -1
typedef long LONG;
typedef unsigned char BYTE;
typedef unsigned long threadID;
typedef unsigned long DWORD;
typedef double        real;  //biggest float-type on this machine
typedef long          LONG;

typedef void *THREAD_HANDLE;
typedef void *EVENT_HANDLE;

typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

int64 CryGetTicks();

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#define _MS_ALIGN(num) __declspec(align(num))

#define DEFINE_ALIGNED_DATA( type, name, alignment ) _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_STATIC( type, name, alignment ) static _declspec(align(alignment)) type name;
#define DEFINE_ALIGNED_DATA_CONST( type, name, alignment ) const _declspec(align(alignment)) type name;

#define SIZEOF_PTR 8

#ifndef FILE_ATTRIBUTE_NORMAL 
	#define FILE_ATTRIBUTE_NORMAL 0x00000080
#endif

#define TARGET_DEFAULT_ALIGN (0x8U)

#define PLATFORM_64BIT

#endif
