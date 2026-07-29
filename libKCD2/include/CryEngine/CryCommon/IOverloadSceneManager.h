////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 20012.
// -------------------------------------------------------------------------
//  File name:   IOverloadSceneManager.h
//  Version:     v1.00
//  Created:     19/04/2012 by JamesChilvers.
//  Compilers:   Visual Studio.NET 2010
//  Description: IOverloadSceneManager interface declaration.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include DEVIRTUALIZE_HEADER_FIX(IOverloadSceneManager.h)

#ifndef _I_OVERLOAD_SCENE_MANAGER_
#define _I_OVERLOAD_SCENE_MANAGER_
#pragma once

//==================================================================================================
// Name: COverloadSceneManager
// Desc: Manages overload values (eg CPU,GPU etc) 
//			 1.0="everything is ok"  0.0="very bad frame rate"
//			 various systems can use this information and control what is currently in the scene
// Author: James Chilvers
//==================================================================================================
UNIQUE_IFACE struct IOverloadSceneManager
{
public:
	// <interfuscator:shuffle>
	virtual ~IOverloadSceneManager() {}
  
	virtual void Reset() = 0;
	virtual void Update() = 0;
  
  // Override auto-calculated scale to reach targetfps.
  // frameScale is clamped to internal min/max values,
  // dt is the length of time in seconds to transition
  virtual void OverrideScale(float frameScale, float dt) = 0;

  // Go back to auto-calculated scale from an overridden scale
  virtual void ResetScale(float dt) = 0;
	// </interfuscator:shuffle>
};//------------------------------------------------------------------------------------------------

#endif //_I_OVERLOAD_SCENE_MANAGER_
