////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2007.
// -------------------------------------------------------------------------
//  File name:   ICombatLog.h
//  Description: Interface of the CombatLog
//    Specifically for logging gameplay data regarding combat
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(ICombatLog.h)

#ifndef __ICOMBAT_LOG_H_
#define __ICOMBAT_LOG_H_
#pragma once

UNIQUE_IFACE struct ICombatLog
{
	// <interfuscator:shuffle>
	virtual	~ICombatLog(){}
  virtual void Log ( EntityId IdShooter, EntityId IdVictim, const char *format, ... )  PRINTF_PARAMS(4, 5) = 0;
	// </interfuscator:shuffle>
};

#endif // __ICOMBAT_LOG_H_
