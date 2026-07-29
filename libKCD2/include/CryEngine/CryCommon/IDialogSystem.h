////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   IDialogSystem.h
//  Version:     v1.00
//  Created:     26/7/2006 by AlexL.
//  Compilers:   Visual Studio.NET 2003
//  Description: Interface of the DialogSystem [mainly for Editor->CryAction]
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(IDialogSystem.h)

#ifndef __IDIALOGSYSTEM_H__
#define __IDIALOGSYSTEM_H__
#pragma once

struct IDialogScriptIterator
{
	struct SDialogScript
	{
		const char* id;
		const char* desc;
		int numRequiredActors;
		int numLines;
		bool bIsLegacyExcel;
	};
	// <interfuscator:shuffle>
	virtual ~IDialogScriptIterator(){}
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual bool Next(SDialogScript& ) = 0;
	// </interfuscator:shuffle>
};
typedef _smart_ptr<IDialogScriptIterator> IDialogScriptIteratorPtr;


UNIQUE_IFACE struct IDialogSystem
{
	// <interfuscator:shuffle>
	virtual ~IDialogSystem(){}
	virtual bool Init() = 0;
	virtual void Update(const float dt) = 0;
	virtual void Reset(bool bUnload) = 0;
	virtual bool ReloadScripts(const char *levelName) = 0;
	virtual IDialogScriptIteratorPtr CreateScriptIterator() = 0;

	// Returns if entity is currently playing a dialog
	virtual bool IsEntityInDialog(EntityId entityId) const = 0;
	// </interfuscator:shuffle>
};

#endif
