////////////////////////////////////////////////////////////////////////////
//
//  CryEngine Source File.
//  Copyright (C), Crytek Studios, 2009.
// -------------------------------------------------------------------------
//  File name:   STLPortConfig.h
//  Created:     14/7/2009 by Timur.
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __STLPortConfig_h__
#define __STLPortConfig_h__
#pragma once

// Temporary here
#ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
# define _CRT_NONSTDC_NO_DEPRECATE
#endif

#if !defined(RESOURCE_COMPILER_IMAGE)
// Microsoft Debug STL turned off so we can use intermixed debug/release versions of DLL.
#undef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING 0
#undef _SECURE_SCL
#define _SECURE_SCL 0
#undef _SECURE_SCL_THROWS
#define _SECURE_SCL_THROWS 0
#endif

//////////////////////////////////////////////////////////////////////////
// STL Port User config settings.
//////////////////////////////////////////////////////////////////////////

// disable exception handling code
#define _STLP_DONT_USE_EXCEPTIONS 1

// Needed for STLPort so we use our own terminate function in platform_impl.h
#define _STLP_DEBUG_TERMINATE 1
// Needed for STLPort so we use our own debug message function in platform_impl.h
#define _STLP_DEBUG_MESSAGE 1

/*
* Set _STLP_DEBUG to turn the "Debug Mode" on.
* That gets you checked iterators/ranges in the manner
* of "Safe STL". Very useful for debugging. Thread-safe.
* Please do not forget to link proper STLport library flavor
* (e.g libstlportstlg.so or libstlportstlg.a) when you set this flag
* in STLport iostreams mode, namespace customization guaranty that you
* link to the right library.
*/
#if (defined(_DEBUG) || defined(FORCE_ASSERTS_IN_PROFILE)) && !defined(PS3) && !defined(__SPU__) // PS3_STLPORT_FIX: Don't use DEBUG for PS3

#if !defined(_STLP_DEBUG)
# define _STLP_DEBUG 1
#endif
# define _STLP_THREADS
# define _STLP_WIN32THREADS

# if defined(_XBOX_VER)
#  if defined(_WIN32_WINNT)
#   if _WIN32_WINNT != 0x0600
#    error unexpected _WIN32_WINNT definition: This code should be updated.
#   endif //_WIN32_WINNT != 0x0600
#  else
//[AlexMc|02.02.10] stlport\stl\_threads.c only uses the more efficient SwitchToThread() if _WIN32_WINNT is >= 0x0400
#   define _WIN32_WINNT 0x0600
#  endif //_WIN32_WINNT
//# elif defined(_WIN32) // _XBOX_VER else
//
//   //[AlexMc|02.02.10] Forward declare the windows threading primitives so stlport can use them
//
//   //[AlexMc|02.02.10] Copied from windef.h
//#  ifdef _MAC
//#   define WINAPI      CDECL
//#  elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
//#   define WINAPI      __stdcall
//#  else
//#   define WINAPI
//#  endif
//
//   //[AlexMc|02.02.10] Copied from winbase.h
//#  if (defined(_M_IX86) || defined(_M_IA64) || defined(_M_AMD64)) && !defined(MIDL_PASS)
//#   define DECLSPEC_IMPORT __declspec(dllimport)
//#  else
//#   define DECLSPEC_IMPORT
//#  endif
//
//#  if !defined(_KERNEL32_)
//#   define WINBASEAPI DECLSPEC_IMPORT
//#  else
//#   define WINBASEAPI
//#  endif
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//   WINBASEAPI int WINAPI SwitchToThread(void);
//#ifdef __cplusplus
//}
//#endif
//
//#undef WINAPI
//#undef DECLSPEC_IMPORT
//#undef WINBASEAPI

# endif //_XBOX_VER else _WIN32
#endif // _DEBUG && !PS3 && !__SPU__

/*
* To reduce the famous code bloat trouble due to the use of templates STLport grant
* a specialization of some containers for pointer types. So all instantiations
* of those containers with a pointer type will use the same implementation based on
* a container of void*. This feature has shown very good result on object files size
* but after link phase and optimization you will only experiment benefit if you use
* many container with pointer types.
* There are however a number of limitation to use this option:
*   - with compilers not supporting partial template specialization feature, you won't
*     be able to access some nested container types like iterator as long as the
*     definition of the type used to instanciate the container will be incomplete
*     (see IncompleteClass definition in test/unit/vector_test.cpp).
*   - you won't be able to use complex Standard allocator implementations which are
*     allocators having pointer nested type not being a real C pointer.
*/
#define _STLP_USE_PTR_SPECIALIZATIONS 1

#endif //__STLPortConfig_h__
