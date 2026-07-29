////////////////////////////////////////////////////////////////////////////
//
//  CryEngine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   ResourceCompilerHelper.h
//  Version:     v1.00
//  Created:     12/07/2004 by MartinM.
//  Compilers:   Visual Studio.NET
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef NOT_USE_CRY_MEMORY_MANAGER
	#include "CryModuleDefs.h"												// ECryModule
#endif

#ifndef __RESOURCECOMPILERHELPER_H__
#define __RESOURCECOMPILERHELPER_H__
#pragma once

#include "SettingsManagerHelpers.h"
#include <stdio.h>     // strlen()

enum ERcExitCode
{
	eRcExitCode_Success = 0,   // must be 0
	eRcExitCode_Error = 1,
	eRcExitCode_FatalError = 100,
	eRcExitCode_Crash = 101,
	eRcExitCode_UserFixing = 200,
	eRcExitCode_Pending = 666,
};

////////////////////////////////////////////////////////////////////////////
// Listener for synchronous resource-compilation.
// Connects the listener to the output pipe of the RC.
struct IResourceCompilerListener
{
public:
	enum MessageSeverity
	{
		MessageSeverity_Debug,
		MessageSeverity_Info,
		MessageSeverity_Warning,
		MessageSeverity_Error
	};

	virtual void OnRCMessage(MessageSeverity severity, const char* text) = 0;
	virtual ~IResourceCompilerListener(){}
};

//////////////////////////////////////////////////////////////////////////
// Provides settings and functions to make calls to RC.
class CResourceCompilerHelper
{
public:
	enum ERcCallResult
	{
		eRcCallResult_success,
		eRcCallResult_notFound,
		eRcCallResult_error,
		eRcCallResult_crash,
		eRcCallResult_queued
	};

	enum ERcExePath
	{
		eRcExePath_currentFolder,
		eRcExePath_registry,
		eRcExePath_settingsManager,
		eRcExePath_customPath
	};

#if (CRY_ENABLE_RC_HELPER)				// run compiler only on developer platform

	//
	// Arguments:
	//   szFileName null terminated ABSOLUTE file path or 0 can be used to test for rc.exe existance, relative path needs to be relative to bin32/rc directory
	//   szAdditionalSettings - 0 or e.g. "/refresh" or "/refresh /xyz=56"
	//
	static ERcCallResult CallResourceCompiler(
		const char* szFileName=0, 
		const char* szAdditionalSettings=0, 
		IResourceCompilerListener* listener=0, 
		bool bMayShowWindow=true, 
		ERcExePath rcExePath=eRcExePath_registry, 
		bool bSilent=false,
		bool bNoUserDialog=false,
		const wchar_t* szWorkingDirectory=0,
		const wchar_t* szRootPath=0);

	static void* AsyncCallResourceCompiler(
		const char* szFileName=0, 
		const char* szAdditionalSettings=0, 
		bool bMayShowWindow=true, 
		ERcExePath rcExePath=eRcExePath_registry, 
		bool bSilent=false,
		bool bNoUserDialog=false,
		const wchar_t* szWorkingDirectory=0,
		const wchar_t* szRootPath=0);

	static ERcExitCode InvokeResourceCompiler(
		const char* szSrcFilePath,
		const char* szDstFilePath, 
		const bool bUserDialog);
	static const char* GetCallResultDescription(ERcCallResult result);
#endif // CRY_ENABLE_RC_HELPER

public:
	// little helper function (to stay independent)
	static const char* GetExtension(const char* in)
	{
		const size_t len = strlen(in);
		for(const char* p = in + len-1; p >= in; --p)
		{
			switch(*p)
			{
			case ':':
			case '/':
			case '\\':
				// we've reached a path separator - it means there's no extension in this name
				return 0;
			case '.':
				// there's an extension in this file name
				return p+1;
			}
		}
		return 0;
	}	

	// little helper function (to stay independent)
	static void ReplaceExtension(const char* path, const char* new_ext, char* buffer, size_t bufferSizeInBytes)
	{
		const char* const ext = GetExtension(path);
    
		SettingsManagerHelpers::CFixedString<char, 512> p;
		if(ext)
		{
			p.set(path, ext - path);
			p.append(new_ext);
		}
		else
		{
			p.set(path);
			p.append(".");
			p.append(new_ext);
		}
    
		strncpy_s(buffer, bufferSizeInBytes, p.c_str(), _TRUNCATE);
	}
	
	// little helper function (to stay independent)
	static void RemovePath(const char* szFilePath, char* buffer, size_t bufferSizeInBytes)
	{
		const char* out = szFilePath;
		const char* chk = strrchr(out, '\\');
		if (chk)
		{
			out = chk + 1;
		}

		chk = strrchr(out, '/');
		if (chk)
		{
			out = chk + 1;
		}

		strncpy_s(buffer, bufferSizeInBytes, out, _TRUNCATE);
	}	

	// little helper function (to stay independent)
	static void RemoveFilename(const char* szFilePath, char* buffer, size_t bufferSizeInBytes)
	{
		strncpy_s(buffer, bufferSizeInBytes, szFilePath, _TRUNCATE);

		char* out = buffer;
		char* chk = strrchr(out, '\\');
		if (chk)
		{
			out = chk;
		}

		chk = strrchr(out, '/');
		if (chk)
		{
			out = chk;
		}

		*out = '\0';
	}	

public:
	// Arguments:
	//   szFilePath - could be source or destination filename
	static void GetOutputFilename(const char* szFilePath, char* buffer, size_t bufferSizeInBytes)
	{
		const char* const ext = GetExtension(szFilePath);

		if (ext)
		{
			if (_stricmp(ext, "tif") == 0 ||
			    _stricmp(ext, "hdr") == 0 ||
			    _stricmp(ext, "srf") == 0)
			{
				ReplaceExtension(szFilePath, "dds", buffer, bufferSizeInBytes);
				return;
			}
		}

		strncpy_s(buffer, bufferSizeInBytes, szFilePath, _TRUNCATE);
	}
};

#endif // __RESOURCECOMPILERHELPER_H__
