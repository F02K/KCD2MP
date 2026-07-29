////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek GmbH, 2012.
// -------------------------------------------------------------------------
//  File name:   MacSpecific.h
//  Version:     v1.00
//  Created:     Mathieu Pinard based on the LinuxSpecifc files
//  Compilers:   Visual Studio.NET, GCC
//  Description: Mac declarations
// -------------------------------------------------------------------------
//  History:
//  - Jul 30, 2013: Leander Beernaert - Common definitions moved to AppleSpecific.h
////////////////////////////////////////////////////////////////////////////
#ifndef __MACSPECIFIC_H__
#define __MACSPECIFIC_H__

#include "AppleSpecific.h"
#include <xmmintrin.h>
//#define _CPU_X86
#define _CPU_AMD64
#define _CPU_SSE
#define PLATFORM_64BIT

#define USE_CRT 1
#define SIZEOF_PTR 8

typedef uint64_t threadID;

// curses.h stubs for PDcurses keys
#define PADENTER	KEY_MAX + 1
#define CTL_HOME	KEY_MAX + 2
#define CTL_END		KEY_MAX + 3
#define CTL_PGDN	KEY_MAX + 4
#define CTL_PGUP	KEY_MAX + 5

// stubs for virtual keys, isn't used on Mac
#define VK_UP				0
#define VK_DOWN			0
#define VK_RIGHT		0
#define VK_LEFT			0
#define VK_CONTROL	0
#define VK_SCROLL		0

#define MAC_NOT_IMPLEMENTED assert(false);

#endif //__MACSPECIFIC_H__
