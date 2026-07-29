/*************************************************************************
	Crytek Source File.
	Copyright (C), Crytek Studios, 2001-2004.
 -------------------------------------------------------------------------
	$Id$
	$DateTime$
	Description:	This is the interface which the Launcher.exe will interact
								with the game dll. For an implementation of this interface
								refer to the GameDll project of the title or MOD you are
								working	on.

 -------------------------------------------------------------------------
	History:
		- 23:7:2004   15:17 : Created by Marco Koegler
		- 30:7:2004   12:00 : Taken-over by Márcio Martins

*************************************************************************/
#include DEVIRTUALIZE_HEADER_FIX(IGameStartup.h)

#ifndef __IGAMESTARTUP_H__
#define __IGAMESTARTUP_H__

#if _MSC_VER > 1000
#	pragma once
#endif


#include <ISystem.h>
#include <IGame.h> // <> required for Interfuscator
#include "IGameRef.h"


struct ILog;
struct ILogCallback;
struct IValidator;
struct ISystemUserCallback;

// Summary
//   Interfaces used to initialize a game using CryEngine
UNIQUE_IFACE struct IGameStartup
{
	// Summary
	//   Entry function to the game
	// Returns
	//   a new instance of the game startup
	// Description
	//   Entry function used to create a new instance of the game
	typedef IGameStartup *(*TEntryFunction)();

	// <interfuscator:shuffle>
	virtual ~IGameStartup(){}

	// Description:
	//		Initialize the game and/or any MOD, and get the IGameMod interface.
	//		The shutdown method, must be called independent of this method's return value.
	// Arguments:
	//		startupParams - Pointer to SSystemInitParams structure containing system initialization setup!
	// Return Value:
	//		Pointer to a IGameMod interface, or 0 if something went wrong with initialization.
	virtual IGameRef Init(SSystemInitParams &startupParams) = 0;

	// Description:
	//		Shuts down the game and any loaded MOD and delete itself.
	virtual void Shutdown() = 0;

	// Description:
	//		Updates the game.
	// Arguments:
	//		haveFocus - Boolean describing if the game has the input focus or not.
	// Return Value:
	//		0 to terminate the game (i.e. when quitting), non-zero to continue
	virtual int Update(bool haveFocus, unsigned int updateFlags) = 0;

	// Description:
	//		Returns a restart level and thus triggers a restart. 
	// Return Value:
	//		NULL to quit the game, a levelname to restart with that level
	virtual bool GetRestartLevel(char** levelName) = 0;

	// Description:
	//		Returns whether a patch needs installing
	//	Return Value:
	//	path+filename if patch to install, null otherwise
	virtual const char* GetPatch() const = 0;
	//		Retrieves the next mod to use in case the engine got a restart request.
	// Return Value:
	//		true will indicates that a mod is requested
	virtual bool GetRestartMod(char* pModName, int nameLenMax) = 0;

	// Description:
	//		Initiate and control the game loop!
	// Arguments:
	//    autoStartLevelName - name of the level to jump into, or NULL for normal behaviour
	// Return Value:
	//		0 when the game terminated normally, non-zero otherwise
	virtual int Run( const char * autoStartLevelName ) = 0;

	// Description:
	//		Returns the RSA Public Key used by the engine to decrypt pak files which are encrypted by an offline tool.
	//		Part of the tools package includes a key generator, which will generate a header file with the public key which can be easily included in code.
	//		This *should* ideally be in IGame being game specific info, but paks are loaded very early on during boot and this data is needed to init CryPak.
	virtual const uint8* GetRSAKey(uint32 *pKeySize) const { *pKeySize = 0; return NULL; }
	// </interfuscator:shuffle>
};

#endif //__IGAMESTARTUP_H__
