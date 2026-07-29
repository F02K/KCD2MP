//#include DEVIRTUALIZE_HEADER_FIX(IBackendService.h)

#ifndef __IGAMESTREAM_H__
#define __IGAMESTREAM_H__

#pragma once

#include "VectorMap.h"
struct ICVar;

struct IGameStream 
{
	enum DirEntStatus
	{
		INLOCAL		= 1<<0,		// file is present in a local folder, i.e. gametau
		INCACHE		= 1<<1,		// file is present in cache
		MODIFIED	= 1<<2,		// file is modified
		READONLY	= 1<<3,		// file is read only
		ISDIR			= 1<<4		// This entry is a directory
	};

	enum Error
	{
		SUCCESS = 0,
		ERR_INIT,						// not properly initialized
		ERR_DLNOTFOUND,			// cannot download, not found
		ERR_DLNOTALLOWED,		// cannot download, permission denied
		ERR_CREATEFILE,		  // cannot create local file
		ERR_SETTIME,				// cannot set file time
	};

	struct DirEntryInfo;
	typedef VectorMap<string, DirEntryInfo, stl::hash_strcmp<string> > DirHash;

	struct DirEntryInfo
	{
		char packedMD5[16];
		union {
			//If pdh is set, this is a dir and doesn't need time. Can be unioned to save space
			DirHash *pdh;	//Directory hash containing children
			uint32 time;
		};
		uint32 size;
		char ver;
		char status;
		char opperc;	// opened percent
		DirEntryInfo() { time = size = 0; ver = status = opperc = 0; pdh = NULL; }
		inline bool dir() const {return ( status & IGameStream::ISDIR ) != 0;}
	};

	typedef void (*FSyncCallback)(const char *pcRoot, const char *pcFile, void *pCallbackData);
	typedef void (*FileFoundCallback)(const char *szName, const DirEntryInfo *pDirInfo, const void *pUserData);
	typedef bool (*WildcardMatch)(const char* szString, const char* szWildcard);
	typedef bool (*FileFilterCallback)(const char *pcFile);

	// sync flags
	static const int STOCACHE = 1;	// sync to cache instead to real folders
	static const int DELFILES	= 2;	// delete files not in list
	static const int IGNORECHANGES = 4;	// ignore changed files
	static const int PRELOADSTATS = 8;	// preload most opened files only
	static const int NOEDITOR = 16;			// skip everything editor related

	// info
	enum EDirType {
		DIR_BIN32,
		DIR_GAME
	};

	// <interfuscator:shuffle>
	//Destructor
	virtual ~IGameStream() {};

	// initializing
	virtual bool Init(const bool bFailIfNoDataCFG) = 0;

	virtual void GetBaseURI(char *szBuffer, const uint32 bufferSize) const = 0;
	virtual void GetDirName(EDirType eType, char *szBuffer, const uint32 bufferSize) const = 0;
	virtual void GetWorkDir(char *szBuffer, const uint32 bufferSize) const = 0;
	virtual void GetInfoValue(const char *pcKey, char *szValueBuffer, const uint32 bufferSize) = 0;

	// file management
	virtual __int64 GetFilesRev() = 0;
	virtual __int64 GetLauncherRev() = 0;
	virtual bool DownloadFile(const char *pcSrcFile, const char *pcLocalFile) = 0;
	virtual bool DownloadFile(const char *pcSrcFile, const char *pcLocalFile, const IGameStream::DirEntryInfo *pDirEntryInfo, const __int64 dirEntryInfoQueryTicks = 0) = 0;
	virtual bool GetOpenedFilesStats(const char *pcRoot, int nMinPerc, const char *pcOutFile) = 0;
	virtual bool SyncFiles(const char *root, int flags, FSyncCallback fn, void *pCallbackData, FileFilterCallback filterFunction = NULL) = 0;
	virtual const DirEntryInfo *GetFileInfo(const char *pcFile) const = 0;
	virtual int FindFileInfos(const char *pcPath, FileFoundCallback fnCallback, const void *pUserData, WildcardMatch fnWildcardMatch) const = 0;
	virtual bool LoadFilesInfo(const char *pcRoot) = 0;

	// message and status handling
	virtual void ReportErrorMessage(const char *pcMessage) = 0;
	virtual Error GetErrorCode() const = 0;
	virtual string GetErrorText() const = 0;
	virtual void SetError(Error code, const char *text) = 0;

	virtual void SetLoggingCVarPointer(ICVar *pLoggingCVar) = 0;

	virtual void LogDownloadTimer(const char *szFilename, __int64 nFileInfoTicks, __int64 nCheckExistingTicks, __int64 nDownloadingTicks, __int64 nSettingModTimeTicks, uint32 fileSize, const char *szResult) = 0;
	// </interfuscator:shuffle>
};

#endif
