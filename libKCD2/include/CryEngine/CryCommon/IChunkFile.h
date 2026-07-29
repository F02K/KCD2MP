////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2005.
// -------------------------------------------------------------------------
//  File name:   IChunkFile.h
//  Version:     v1.00
//  Created:     22/11/2004 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IChunkFile_h__
#define __IChunkFile_h__
#pragma once

#include "CryHeaders.h"

//////////////////////////////////////////////////////////////////////////
// Description:
//    Interface to the Chunked File, (CGF,CHR,...)
//////////////////////////////////////////////////////////////////////////
struct IChunkFile : _reference_target_t
{
	typedef FILE_HEADER FileHeader;
	typedef CHUNK_HEADER ChunkHeader;

	//////////////////////////////////////////////////////////////////////////
	// Chunk Description.
	//////////////////////////////////////////////////////////////////////////
	struct ChunkDesc
	{
		CHUNK_HEADER hdr;
		void *data;
		int size;
		bool bSwapEndian;

		//////////////////////////////////////////////////////////////////////////
		ChunkDesc() : data(0), size(0), bSwapEndian(false) {}
		inline bool operator<( const ChunkDesc &d2 ) const { return hdr.ChunkID < d2.hdr.ChunkID; }
		inline bool operator>( const ChunkDesc &d2 ) const { return hdr.ChunkID > d2.hdr.ChunkID; }
		inline bool operator==( const ChunkDesc &d2 ) const { return hdr.ChunkID == d2.hdr.ChunkID; }
		inline bool operator!=( const ChunkDesc &d2 ) const { return hdr.ChunkID != d2.hdr.ChunkID; }

		void GetMemoryUsage(ICrySizer *pSizer) const{/*nothing*/}
	};

	// <interfuscator:shuffle>
	virtual void GetMemoryUsage(ICrySizer *pSizer) const=0;

	// Summary:
	//	 Releases chunk file interface.
	virtual void Release() = 0;

	virtual bool IsReadOnly() const = 0;
	virtual bool IsLoaded() const = 0;

	virtual bool Read( const char *filename ) = 0;
	virtual bool ReadFromMemBlock( const void * pData, int nDataSize ) = 0;

	// Summary:
	//	 Writes chunks to file.
	virtual bool Write( const char *filename ) = 0;
	// Description:
	//	 Writes chunks to the memory file, returns pointers to the allocated memory.
	//	 Memory will be released on destruction of the ChunkFile object.
	virtual void WriteToMemory( void **pData,int *nSize ) = 0;

	virtual int AddChunk( const CHUNK_HEADER &hdr,void *chunkData,int chunkSize ) = 0;
	virtual void SetChunkData( int nChunkId,void *chunkData,int chunkSize ) = 0;
	virtual void DeleteChunkId( int nChunkId ) = 0;

	virtual ChunkDesc* FindChunkByType( ChunkTypes nChunkType ) = 0;
	virtual ChunkDesc* FindChunkById( int nChunkId ) = 0;

	// Summary:
	//	 Returns the file header.
	virtual const FileHeader& GetFileHeader() const = 0;

	// Summary:
	//	 Returns the raw data of the i-th chunk.
	virtual const void* GetChunkData(int nIndex ) const  = 0;
	// Summary:
	//	 Retrieves the raw chunk header, as it appears in the file.
	virtual const ChunkHeader& GetChunkHeader( int nIndex ) const  = 0;
	// Summary:
	//	 Gets chunk description at i-th index.
	virtual ChunkDesc* GetChunk( int nIndex ) = 0;
	virtual const ChunkDesc* GetChunk( int nIndex ) const = 0;
	// Summary:
	//	 Calculates the chunk size, based on the very next chunk with greater offset
	//	 or the end of the raw data portion of the file.
	virtual int GetChunkSize( int nIndex ) const = 0;
	// Summary:
	//	 Gets the number of chunks.
	virtual int NumChunks() const = 0;
	// Summary:
	//	 Gets the number of chunks of the specified type.
	virtual int NumChunksOfType (ChunkTypes nChunkType) const = 0;
	virtual const char* GetLastError() const = 0;
	// </interfuscator:shuffle>
};

#endif // __IChunkFile_h__
