////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2003.
// -------------------------------------------------------------------------
//  File name:   ISPULog.h
//  Version:     v1.00
//  Created:     12/9/2003 by Michael Glueck.
//  Compilers:   Visual Studio.NET
//  Description: Transparent Log interface for SPU
//							 non virtual implementation which will always result in printf calls
//							 one instance is kept on SPU, so GetILog will call GetISPULog
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef _CRY_ENGINE_SPU_LOG_HDR_
#define _CRY_ENGINE_SPU_LOG_HDR_

#if defined(__SPU__)
#include <stdarg.h>

struct ILogCallback;

extern void LogMessageV(const char* szFormat, va_list args);

struct ILog
{
	enum ELogType
	{
		eMessage,
		eWarning,
		eError,
		eAlways,
		eWarningAlways,
		eErrorAlways,
		eInput,						
		eInputResponse,		
		eComment
	};

	void Release(){}
	bool SetFileName(const char *command = NULL){}
	const char*	GetFileName(){return NULL;}
	void Log(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogWarning(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogError(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogPlus(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogToFile(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogToFilePlus(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogToConsole(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void LogToConsolePlus(const char *szCommand,...) PRINTF_PARAMS(2, 3)
	{
		va_list args;
		va_start(args,szCommand);
		LogV(szCommand, args);
		va_end(args);
	}
	void UpdateLoadingScreen(const char *szCommand,...) PRINTF_PARAMS(2, 3){}
	void RegisterConsoleVariables(){}
	void UnregisterConsoleVariables() {}
	void SetVerbosity( int verbosity ){}
	int	 GetVerbosityLevel(){return 0;}
	void AddCallback( ILogCallback *){}
	void RemoveCallback( ILogCallback *){}
	//will be implemented in SPU driver lib
	void LogV( const ELogType, const char* szFormat, va_list args )
	{
		LogMessageV(szFormat, args);
	}
	void LogV( const char* szFormat, va_list args )
	{
		LogMessageV(szFormat, args);
	}
	void PushAssetScopeName( const char *sAssetType,const char *sName ) {};
	void PopAssetScopeName() {};
	const char* GetAssetScopeString() { return ""; };
};

extern ILog* GetISPULog();

#define INDENT_LOG_DURING_SCOPE(...) (void)(0)
#define CRY_DEFINE_ASSET_SCOPE( sAssetType,sAssetName ) (void)(0)

#endif//__SPU__
#endif //_CRY_ENGINE_SPU_LOG_HDR_
