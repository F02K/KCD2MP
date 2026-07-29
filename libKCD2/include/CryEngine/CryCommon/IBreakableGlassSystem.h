////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2011.
// -------------------------------------------------------------------------
//  File name:   IBreakableGlassSystem.h
//  Version:     v1.00
//  Created:     11/11/2011 by ChrisBu.
//  Compilers:   Visual Studio 2010
//  Description: Interface to the Breakable Glass System
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(IBreakableGlassSystem.h)

#ifndef __IBREAKABLEGLASSSYSTEM_H__
#define __IBREAKABLEGLASSSYSTEM_H__
#pragma once

struct EventPhysCollision;

UNIQUE_IFACE struct IBreakableGlassSystem 
{
	virtual ~IBreakableGlassSystem() {}

	virtual void	Update(const float frameTime) = 0;
	virtual bool	BreakGlassObject(const EventPhysCollision& physEvent, const bool forceGlassBreak = false) = 0;
	virtual void	ResetAll() = 0;
	virtual void	GetMemoryUsage(ICrySizer* pSizer) const = 0;
};

#endif // __IBREAKABLEGLASSSYSTEM_H__
