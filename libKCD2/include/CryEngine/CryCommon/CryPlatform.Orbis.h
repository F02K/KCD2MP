////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2013.
// -------------------------------------------------------------------------
//  File name:   CryPlatform.h
//  Version:     v1.00
//  Created:     31/01/2013 by Christopher Bolte.
//  Compilers:   Visual Studio.NET
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef _CRY_PLATFORM_ORBIS_H_
#define _CRY_PLATFORM_ORBIS_H_


////////////////////////////////////////////////////////////////////////////
// check that we are allowed to be included
#if !defined(CRYPLATFROM_ALLOW_DETAIL_INCLUDES)
#	error Please include CryPlatfrom.h instead of this private implementation header
#endif


////////////////////////////////////////////////////////////////////////////
// size and alignment settings for platfrom specific primities
////////////////////////////////////////////////////////////////////////////
// Interlocked singled linked list settings
#define CRYPLATFORM_INTERLOCKEDSLIST_HEADER_SIZE				16
#define CRYPLATFORM_INTERLOCKEDSLIST_HEADER_ALIGNMENT		16
#define CRYPLATFORM_INTERLOCKEDSLIST_ELEMENT_SIZE				8
#define CRYPLATFORM_INTERLOCKEDSLIST_ELEMENT_ALIGNMENT	16


////////////////////////////////////////////////////////////////////////////
// macros for platfrom specific functionality which cannot be expressed as a C++ function
////////////////////////////////////////////////////////////////////////////
// mark a structure as aligned
#define ALIGN_STRUCTURE( alignment, structure ) structure __attribute__ ((aligned(alignment)))
// cause a the debugger to break if executed
#define DEBUG_BREAK do { __debugbreak(); } while(0)

#endif // _CRY_PLATFORM_ORBIS_H_