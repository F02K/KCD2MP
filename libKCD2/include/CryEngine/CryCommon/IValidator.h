////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   ivalidator.h
//  Version:     v1.00
//  Created:     3/6/2003 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: IValidator interface used to check objects for warnings and errors
//               Report missing resources or invalid files.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(IValidator.h)

#ifndef __ivalidator_h__
#define __ivalidator_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(WIN32) || defined(WIN64) || defined(LINUX) || defined(MAC)
#	define MAX_WARNING_LENGTH	4096
#else
#	define MAX_WARNING_LENGTH	512
// reduce max warning length for consoles (e.g. to prevent overflows on PS3 where thread stack are rather small)
#endif

#if MAX_WARNING_LENGTH<33
#error "MAX_WARNING_LENGTH should be bigger than 32"
#endif

#define ERROR_CANT_FIND_CENTRAL_DIRECTORY "Cannot find Central Directory Record in pak. This is either not a pak file, or a pak file without Central Directory. It does not mean that the data is permanently lost, but it may be severely damaged. Please repair the file with external tools, there may be enough information left to recover the file completely."

enum EValidatorSeverity
{
	VALIDATOR_ERROR,
	VALIDATOR_ERROR_DBGBRK,	// will __debugbreak() if sys_error_debugbreak is 1
	VALIDATOR_WARNING,
	VALIDATOR_COMMENT
};

enum EValidatorModule
{
	VALIDATOR_MODULE_UNKNOWN,
	VALIDATOR_MODULE_RENDERER,
	VALIDATOR_MODULE_3DENGINE,
	VALIDATOR_MODULE_AI,
	VALIDATOR_MODULE_ANIMATION,
	VALIDATOR_MODULE_ENTITYSYSTEM,
	VALIDATOR_MODULE_SCRIPTSYSTEM,
	VALIDATOR_MODULE_SYSTEM,
	VALIDATOR_MODULE_SOUNDSYSTEM,
	VALIDATOR_MODULE_GAME,
	VALIDATOR_MODULE_MOVIE,
	VALIDATOR_MODULE_EDITOR,
	VALIDATOR_MODULE_NETWORK,
	VALIDATOR_MODULE_PHYSICS,
	VALIDATOR_MODULE_FLOWGRAPH,
	VALIDATOR_MODULE_ONLINE,
	VALIDATOR_MODULE_GAMESTREAM
};

enum EValidatorFlags
{
	VALIDATOR_FLAG_FILE			= 0x0001,		// Indicate that required file was not found or file was invalid.
	VALIDATOR_FLAG_TEXTURE	= 0x0002,		// Problem with texture.
	VALIDATOR_FLAG_SCRIPT		= 0x0004,		// Problem with script.
	VALIDATOR_FLAG_SOUND		= 0x0008,		// Problem with sound.
	VALIDATOR_FLAG_AI				= 0x0010,		// Problem with AI.
	VALIDATOR_FLAG_LOG_ASSET_SCOPE = 0x0020, // Log asset scope with the warning.
	VALIDATOR_FLAG_IGNORE_IN_EDITOR = 0x0040, // Do not log this with the editor
};

struct SValidatorRecord
{
	//! Severity of this error.
	EValidatorSeverity severity;
	//! In which module error occured.
	EValidatorModule module;
	//! Error Text.
	const char *text;
	//! File which is missing or causing problem.
	const char *file;
	//! Additional description for this error.
	const char *description;
	//! Asset scope sring
	const char *assetScope;
	//! Flags that suggest kind of error.
	int flags;

	//////////////////////////////////////////////////////////////////////////
	SValidatorRecord()
	{
		module = VALIDATOR_MODULE_UNKNOWN;
		text = NULL;
		file = NULL;
		assetScope = NULL;
		description = NULL;
		severity = VALIDATOR_WARNING;
		flags = 0;
	}
};

/*! This interface will be given to Validate methods of engine, for resources and objects validation.
 */
struct IValidator
{
	// <interfuscator:shuffle>
	virtual ~IValidator(){}
	virtual void Report( SValidatorRecord &record ) = 0;
	// </interfuscator:shuffle>
};

#endif // __ivalidator_h__
