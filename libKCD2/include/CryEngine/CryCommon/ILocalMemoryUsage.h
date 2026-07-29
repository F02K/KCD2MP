////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2008.
// -------------------------------------------------------------------------
//  File name:   ILocalMemoryUsage.h
//  Version:     v1.00
//  Created:     Istvan Kiss (Spidy) [11/5/2008]
//  Description: Interface for LocalMemoryUsage
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef _ILOCALMEMORYUSAGE_H_
#define _ILOCALMEMORYUSAGE_H_

class CCamera;
struct IRenderer;

struct ILocalMemoryUsage
{
	virtual ~ILocalMemoryUsage(){}
	virtual void OnRender(IRenderer* pRenderer, const CCamera *camera) = 0;
	virtual void OnUpdate() = 0;
	virtual void DeleteGlobalData() = 0;
};

#endif