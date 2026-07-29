////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2004.
// -------------------------------------------------------------------------
//  File name:   OrbisSpecific.h
//  Version:     v0.01
//  Created:     05/09/2012 by Charlie Cole
//  Compilers:   Clang
//  Description: Specific to Orbis declarations, inline functions etc.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef _CRY_COMMON_ORBIS_SPECIFIC_HDR_
#define _CRY_COMMON_ORBIS_SPECIFIC_HDR_

//////////////////////////////////////////////////////////////////////////
// Standard includes.
//////////////////////////////////////////////////////////////////////////
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <float.h>
#include <xmmintrin.h>
#include <kernel.h>
#include <sce_atomic.h>
#include <sdk_version.h>
#include <BaseTypes.h>
#undef MIN
#undef MAX

#if SCE_ORBIS_SDK_VERSION<0x00990060u
#error Currently supported SDK is 0.990.060 Please upgrade and recompile.
#endif

#define _CPU_AMD64
#define _CPU_SSE

#ifndef _isnan
	#define _isnan isnan
#endif

#if defined(_LIB)
	extern unsigned short GetKeyState(int vKey);
#endif
#define CryGetAsyncKeyState GetAsyncKeyState

#define __debugbreak() _SCE_BREAK()
#define DebugBreak() __debugbreak()

//-------------------------------------socket stuff------------------------------------------

#define stricmp strcasecmp


//////////////////////////////////////////////////////////////////////////
// Define platform independent types.
//////////////////////////////////////////////////////////////////////////
typedef void*								LPVOID;
#define VOID            		void
#define PVOID								void*

typedef signed char         int8;
typedef signed char         INT8;
typedef signed short        int16;
typedef signed short        INT16;
typedef signed int					int32;
typedef int									INT32;
typedef signed long long		int64;
typedef signed long long		INT64;
typedef unsigned char				uint8;
typedef unsigned char				UINT8;
typedef unsigned short			uint16;
typedef unsigned short			UINT16;
typedef unsigned int				uint32;
typedef unsigned int				UINT32;
typedef unsigned long long	uint64;

typedef float               f32;
typedef double              f64;

//typedef double              real;  //biggest float-type on this machine
typedef float              real;  //biggest float-type on this machine
#define REAL_IS_FLOAT 1

typedef __intptr_t intptr_t ;
typedef __uintptr_t uintptr_t;
typedef intptr_t INT_PTR, *PINT_PTR;
typedef uintptr_t UINT_PTR, *PUINT_PTR;

typedef char *LPSTR, *PSTR;

typedef INT_PTR LONG_PTR, *PLONG_PTR, *PLONG;
typedef UINT_PTR ULONG_PTR, *PULONG_PTR;

typedef _Uint32t						uint32_t;
typedef _Int32t							int32_t;

typedef uint32_t						DWORD;
typedef uintptr_t						DWORD_PTR;
typedef unsigned long*			LPDWORD;
typedef unsigned char				BYTE;
typedef unsigned short			WORD;
typedef int                 INT;
typedef unsigned int        UINT;
typedef float               FLOAT;
typedef void*								HWND;
typedef UINT_PTR 						WPARAM;
typedef LONG_PTR 						LPARAM;
typedef LONG_PTR 						LRESULT;
#define PLARGE_INTEGER LARGE_INTEGER*
typedef const char *LPCSTR, *PCSTR;
typedef long long						LONGLONG;
typedef	ULONG_PTR						SIZE_T;
typedef int32_t							LONG;

typedef int32 __int32;
typedef uint32 __uint32;
typedef int64 __int64;
typedef uint64 __uint64;

#define THREADID_NULL -1
typedef uint64 threadID;

typedef union _LARGE_INTEGER
{
  struct
	{
      DWORD LowPart;
      LONG HighPart;
  };
  struct
	{
      DWORD LowPart;
      LONG HighPart;
  } u;
	uint64 QuadPart;
} LARGE_INTEGER;

//shortens alignment declaration
#define _PACK __attribute__ ((packed))

#undef ILINE
#if !defined(_DEBUG) && !defined(NO_ILINE)
#define ILINE __attribute__((always_inline)) inline
#else
#define ILINE inline
#endif

#define _inline ILINE
#define __cdecl
#define _cdecl
#define _stdcall
#define __stdcall
#define _fastcall
#define __fastcall

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; } }
#endif

#ifndef SAFE_RELEASE_FORCE
	#define SAFE_RELEASE_FORCE(p)			{ if(p) { (p)->ReleaseForce();	(p)=NULL; } }
#endif

#define DEFINE_ALIGNED_DATA( type, name, alignment ) \
	type __attribute__ ((aligned(alignment))) name;
#define DEFINE_ALIGNED_DATA_STATIC( type, name, alignment ) \
	static type __attribute__ ((aligned(alignment))) name;
#define DEFINE_ALIGNED_DATA_CONST( type, name, alignment ) \
	const type __attribute__ ((aligned(alignment))) name;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))

#define CALLBACK
#define WINAPI

#ifndef __cplusplus
#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define TCHAR wchar_t;
#define _WCHAR_T_DEFINED
#endif
#endif
typedef char CHAR;
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef const WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;
typedef WCHAR *LPUWSTR, *PUWSTR;

typedef const WCHAR *LPCWSTR, *PCWSTR;
typedef const WCHAR *LPCUWSTR, *PCUWSTR;

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#define FILE_ATTRIBUTE_NORMAL               0x00000080

typedef int							BOOL;
typedef uint32_t				ULONG;
typedef int32_t					HRESULT;

#define _PTRDIFF_T_DEFINED

#define TRUE 1
#define FALSE 0

#undef _MAX_PATH 
#undef MAX_PATH
#define MAX_PATH 1024
#define _MAX_PATH MAX_PATH

#ifdef __cplusplus
template<typename S, typename T>
inline const S __min(const S rS, const T rT)
{
	return (rS <= rT)?rS : (const S)rT;
}

template<typename S, typename T>
inline const S __max(const S rS, const T rT)
{
	return (rS >= rT)?rS : (const S)rT;
}
#endif

//////////////////////////////////////////////////////////////////////////
// Multi platform Hi resolution ticks function, should only be used for profiling.
//////////////////////////////////////////////////////////////////////////
// TODO

//conditional selects

//implements branch free: 
//return (cA > cB)?cA : cB;
#if !defined(__SPU__)
template <class T>
T CondSelMax(const T cA, const T cB);
template <class T>
T CondSelMin(const T cA, const T cB);

template <class T>
__attribute__((always_inline))
inline T CondSelMax(const T cA, const T cB)
{
	const uint32 cMinMask = (uint32)(((int32)(cA - cB)) >> 31);
	return ((cA & ~cMinMask) | (cB & cMinMask));
}

//implements branch free for integer types: 
//return (cA < cB)?cA : cB;
template <class T>
__attribute__((always_inline))
inline T CondSelMin(const T cA, const T cB)
{
	const uint32 cMinMask = (uint32)(((int32)(cB - cA)) >> 31);
	return ((cA & ~cMinMask) | (cB & cMinMask));
}

template <>
__attribute__((always_inline))
inline float CondSelMin<float>(const float cA, const float cB)
{
	return (cA < cB)?cA : cB;
}

template <>
__attribute__((always_inline))
inline float CondSelMax<float>(const float cA, const float cB)
{
	return (cA > cB)?cA : cB;
}
#endif

#define _A_RDONLY       (0x01)
#define _A_HIDDEN       (0x02)
#define _A_SUBDIR       (0x10)

//temp definitions for GetAsynchKeyState, should not be used on Orbis
#define VK_SCROLL      0xFF
#define VK_PAUSE       0xFF
#define VK_UP          0xFF
#define VK_DOWN				 0xFF
#define VK_CONTROL     0
#define VK_RIGHT       0xFF
#define VK_LEFT        0xFF
#define VK_SPACE       0xFF
#define VK_ESCAPE			 0xFF
#define VK_LBUTTON		 0xFF	

#define __forceinline ILINE
#define _finite isfinite
#define TARGET_DEFAULT_ALIGN (0x10U)

#define ORBIS_TO_IMPLEMENT	__debugbreak()

extern size_t _msize(void *p);

//////////////////////////////////////////////////////////////////////////
// Win32 FileAttributes.
//////////////////////////////////////////////////////////////////////////
#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000
#define FILE_WRITE_ATTRIBUTES								FILE_ATTRIBUTE_NORMAL //not entirely correct but shoudl work for now

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define EISCONNECTED  EISCONN
#define ENOTCONNECTED ENOTCONN
	
#define WSAEINTR EINTR
#define WSAEBADF EBADF
#define WSAEACCES EACCES
#define WSAEFAULT EFAULT
#define WSAEACCES EACCES
#define WSAEFAULT EFAULT
#define WSAEINVAL EINVAL
#define WSAEMFILE EMFILE
#define WSAEWOULDBLOCK EWOULDBLOCK
#define WSAEINPROGRESS EINPROGRESS
#define WSAEALREADY EALREADY
#define WSAENOTSOCK ENOTSOCK 
#define WSAEDESTADDRREQ EDESTADDRREQ
#define WSAEMSGSIZE EMSGSIZE
#define WSAEPROTOTYPE EPROTOTYPE
#define WSAENOPROTOOPT ENOPROTOOPT
#define WSAEPROTONOSUPPORT EPROTONOSUPPORT
#define WSAESOCKTNOSUPPORT ESOCKTNOSUPPORT
#define WSAEOPNOTSUPP EOPNOTSUPP
#define WSAEPFNOSUPPORT EPFNOSUPPORT
#define WSAEAFNOSUPPORT EAFNOSUPPORT
#define WSAEADDRINUSE EADDRINUSE
#define WSAEADDRNOTAVAIL EADDRNOTAVAIL
#define WSAENETDOWN ENETDOWN
#define WSAENETUNREACH ENETUNREACH
#define WSAENETRESET ENETRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAECONNRESET ECONNRESET
#define WSAENOBUFS ENOBUFS
#define WSAEISCONN EISCONN
#define WSAENOTCONN ENOTCONN
#define WSAESHUTDOWN ESHUTDOWN
#define WSAETOOMANYREFS ETOOMANYREFS
#define WSAETIMEDOUT ETIMEDOUT
#define WSAECONNREFUSED ECONNREFUSED
#define WSAELOOP ELOOP
#define WSAENAMETOOLONG ENAMETOOLONG
#define WSAEHOSTDOWN EHOSTDOWN
#define WSAEHOSTUNREACH EHOSTUNREACH
#define WSAENOTEMPTY ENOTEMPTY
#define WSAEPROCLIM EPROCLIM
#define WSAEUSERS EUSERS
#define WSAEDQUOT EDQUOT
#define WSAESTALE ESTALE
#define WSAEREMOTE EREMOTE

#define WSAHOST_NOT_FOUND (1024 + 1)
#define WSATRY_AGAIN			EAGAIN
#define WSANO_RECOVERY		(1024 + 3)
#define WSANO_DATA				ENODATA
#define WSANO_ADDRESS			(WSANO_DATA)

typedef int SOCKET;
#define closesocket			close
#define TCP_NODELAY			SCE_NET_TCP_NODELAY
#define SO_NBIO					SCE_NET_SO_NBIO
#define sys_net_errno		sce_net_errno
typedef int TErrorType;

in_addr_t inet_addr(const char *);
char *inet_ntoa(struct in_addr);

#define FP16_MESH 1
#define PLATFORM_64BIT

namespace VirtualAllocator
{
	void* AllocateVirtualAddressSpace(size_t size);
	int FreeVirtualAddressSpace(void *ptr, size_t sz=~0ul);
	void* MapPage(void* ptr, size_t pageSize=PAGE_SIZE);
	int UnmapPage(void* ptr, size_t pageSize=PAGE_SIZE);
	void* MapPageBlock(void* ptr, size_t size, size_t pageSize=PAGE_SIZE);
	void UnmapPageBlock(void* ptr, size_t size, size_t pageSize=PAGE_SIZE);
	bool InAllocatedSpace(void *ptr);
	void RegisterVideoMemoryAllocated(size_t size);
	void CalibrateUntrackedMemory();
	void QueryMemory(size_t &mainMemory, size_t &videoMemory);
	void VerifyList();
	void* GetLastTrackedAddress();
};

#endif //_CRY_COMMON_ORBIS_SPECIFIC_HDR_

