////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   MSVCSpecific.h
//  Version:     v1.00
//  Created:     5/4/2005 by Scott
//  Compilers:   Visual Studio.NET 2003
//  Description: Settings for all builds under MS Visual C++ compiler
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(WIN32) && !defined(_RELEASE)
#pragma optimize( "y", off ) // Generate frame pointers on the program stack. (Disable Omit Frame Pointers optimization, call stack unwinding cannot work with it)
#endif

// Disable (and enable) specific compiler warnings.
// MSVC compiler is very confusing in that some 4xxx warnings are shown even with warning level 3,
// and some 4xxx warnings are NOT shown even with warning level 4.

#pragma warning(disable: 4018)	// signed/unsigned mismatch
#pragma warning(disable: 4127)	// conditional expression is constant
#pragma warning(disable: 4201)	// nonstandard extension used : nameless struct/union
#pragma warning(disable: 4512)	// assignment operator could not be generated (in STLPort with const constructs)
#pragma warning(disable: 4530)  // C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#pragma warning(disable: 4996)	// 'stricmp' was declared deprecated
#pragma warning(disable: 4503)  // decorated name length exceeded, name was truncated
#pragma warning(disable: 6255)  // _alloca indicates failure by raising a stack overflow exception. Consider using _malloca instead. (Note: _malloca requires _freea.)


// Turn on the following very useful warnings.
#pragma warning(3: 4264)				// no override available for virtual member function from base 'class'; function is hidden
#pragma warning(3: 4266)				// no override available for virtual member function from base 'type'; function is hidden

#include <float.h>

struct ScopedSetFloatExceptionMask
{
	ScopedSetFloatExceptionMask(unsigned int disable = _EM_INEXACT | _EM_UNDERFLOW | _EM_OVERFLOW | _EM_DENORMAL |_EM_INVALID)
	{
		_clearfp();
		_controlfp_s(&oldMask, 0, 0);
		unsigned temp;
		_controlfp_s(&temp, disable, _MCW_EM);
	}
	~ScopedSetFloatExceptionMask()
	{
		_clearfp();
		unsigned temp;
		_controlfp_s(&temp, oldMask, _MCW_EM);
	}
	unsigned oldMask;
};

#define SCOPED_ENABLE_FLOAT_EXCEPTIONS ScopedSetFloatExceptionMask scopedSetFloatExceptionMask(0)
#define SCOPED_DISABLE_FLOAT_EXCEPTIONS ScopedSetFloatExceptionMask scopedSetFloatExceptionMask
