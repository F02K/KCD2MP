////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   IDataProbe.h
//  Version:     v1.00
//  Created:     19/1/2004 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description: Defines IDataProbe interface.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(IDataProbe.h)

#ifndef __IDataProbe_h__
#define __IDataProbe_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetHelpers.h"

enum EDataProbeCodeInfo
{
  DATAPROBE_CRC32 = 0,
  DATAPROBE_CRC64 = 1,
  DATAPROBE_ADLER32 = 2,
  DATAPROBE_PURE_CRC32 = 3,
  DATAPROBE_MD5 = 4,
};

enum EDataProbeContextType
{
  EDataProbeContextType_FileCheck = 1,
  EDataProbeContextType_CvarCheck = 2,
};

enum EProtectedFileFlag
{
	ePFF_HeaderOnly = BIT( 0 ),
	ePFF_AllowClientAbsent = BIT( 1 ),
	ePFF_LateResolve = BIT( 2 ),
	ePFF_Recurse = BIT( 3 )
};

struct SDefenseFileInfo
{
	SDefenseFileInfo(): m_fileName( "" ), m_flags( 0 ) {}

	string								m_fileName;
	uint32								m_flags;
};

inline bool operator==( const SDefenseFileInfo& lhs, const SDefenseFileInfo& rhs )
{
	return lhs.m_fileName == rhs.m_fileName;
}

inline bool operator<( const SDefenseFileInfo& lhs, const SDefenseFileInfo& rhs )
{
	return lhs.m_fileName < rhs.m_fileName;
}

// how long to wait for client response before giving up
#define DATAPROBE_TIMEOUT_SECONDS 60

struct SDataProbeContext
{
	// Folder name to check. Can't use string as it may not have thread safe ref counting
	CryStringLocalT<char> sFolder;
	// Filename to check.
	CryStringLocalT<char> sFilename;
	void *pBuffer;
	void *pModuleBaseAddress;
	unsigned int nOffset;
	unsigned int nSize;
	// Algorithm used fo code.
	unsigned int nCodeInfo;
	// Resulting code.
	uint64 nCode;
	uint64 nCodeLowSpec;
  // context type
  unsigned int nCtxType;
	// indicates we only want header offsets for this file
	bool bHeaderOnly;
	bool bAllowClientAbsent;
	bool bLateResolve;

	SDataProbeContext()
	{
		pBuffer = 0;
		pModuleBaseAddress = 0;
		nOffset = nSize = nCodeInfo = 0;
		nCode = 0;
    nCtxType = 0;
		bHeaderOnly=false;
		bAllowClientAbsent = false;
		bLateResolve = false;
	}
};

UNIQUE_IFACE struct IDataProbe
{
	struct SModuleInfo
	{
		string filename;
		void* handle;
	};

	// <interfuscator:shuffle>
	virtual ~IDataProbe() {};
	virtual bool Dummy1( SDataProbeContext &ctx ) = 0;
	virtual int Dummy2( void *pBuf,int aa,SDataProbeContext &ctx ) = 0;
	virtual bool GetCode( SDataProbeContext &ctx ) = 0;
	virtual bool Dummy3( SDataProbeContext &ctx ) = 0;

	virtual bool GetRandomFileProbe( SDataProbeContext &ctx,bool bWholeFile )=0;
	virtual bool GetRandomModuleProbe( SDataProbeContext &ctx ) = 0;
	virtual bool GetModuleProbe( SDataProbeContext &ctx ) = 0;

	// Hash of ASCII string.
	virtual uint32 GetHash( const char *sString ) = 0;
	// Hash of any buffer.
	virtual uint32 GetHash( const void *buffer,int len ) = 0;

	// Return array of loaded modules information, returns number of modules.
	virtual int GetLoadedModules( SModuleInfo **pModules ) = 0;
	virtual void AddModule( SModuleInfo &moduleInfo ) = 0;

	virtual void RandomAlloc() = 0;
	
	// Compress block of data with zlib.
	virtual int Compress( void *dest,unsigned int &destLen,const void *source, unsigned int sourceLen,int level=6 ) = 0;
	// Uncompress block of data with zlib.
	virtual int Uncompress( void *dest,unsigned int &destLen,const void *source, unsigned int sourceLen ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// MD5 Digest.
	//////////////////////////////////////////////////////////////////////////
	virtual void GetMD5( const char *pSrcBuffer,int nSrcSize,char signatureMD5[16] ) = 0;

	// Decrypt buffer.
	// Support in place decryption, (pSrcBuffer == pDestBuffer).
	// Output buffer should be at least the same size as source buffer.
	// Key must be at least 32 bytes long.
	virtual void AESDecryptBuffer( const char *pSrcBuffer,int nSrcSize,char *pDestBuffer,int &nDestSize,const char *sKey ) = 0;

	virtual	void RandSeed( uint32 seed ) = 0;
	virtual	uint32 GetRand() = 0;
	virtual	float GetRand( float fMin,float fMax ) = 0;
	// </interfuscator:shuffle>
};

struct IDefenceContext : public INetMessageSink
{
	// <interfuscator:shuffle>
  inline virtual ~IDefenceContext() {}
  virtual void AddProtectedFile( const SDefenseFileInfo& ) = 0;
  virtual void ClearProtectedFiles() = 0;
  virtual bool HasRemoteDef( const SNetMessageDef * pDef ) = 0;

  // true once there's no outstanding validation requests
  virtual bool CanRemove() = 0;

  virtual void StartCvarRequests() = 0;
  virtual void EndCvarRequests() = 0;
	virtual void SendNextRequestSplitCvarRange(uint32 nOffset, uint32 nSize) = 0;

	virtual void Abort() = 0;
	// </interfuscator:shuffle>
};

#endif // __IDataProbe_h__
