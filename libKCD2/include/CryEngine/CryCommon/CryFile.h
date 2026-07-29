////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   cryfile.h
//  Version:     v1.00
//  Created:     3/7/2003 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: File wrapper.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __cryfile_h__
#define __cryfile_h__
#pragma once

#include <ISystem.h>
#include <ICryPak.h>
#include <IConsole.h>
#include "CryPath.h"

//////////////////////////////////////////////////////////////////////////
// Defines for CryEngine filetypes extensions.
//////////////////////////////////////////////////////////////////////////
#define CRY_GEOMETRY_FILE_EXT                    "cgf"
#define CRY_SKEL_FILE_EXT                        "chr" //will be a SKEL soon
#define CRY_SKIN_FILE_EXT												 "skin"
#define CRY_CHARACTER_ANIMATION_FILE_EXT         "caf"
#define CRY_CHARACTER_DEFINITION_FILE_EXT        "cdf"
#define CRY_CHARACTER_LIST_FILE_EXT              "cid"
#define CRY_ANIM_GEOMETRY_FILE_EXT               "cga"
#define CRY_ANIM_GEOMETRY_ANIMATION_FILE_EXT     "anm"
#define CRY_COMPILED_FILE_EXT                    "(c)"
#define CRY_BINARY_XML_FILE_EXT                  "binxml"
#define CRY_XML_FILE_EXT                         "xml"
#define CRY_CHARACTER_PARAM_FILE_EXT             "chrparams"
//////////////////////////////////////////////////////////////////////////
#define CRYFILE_MAX_PATH                         260
//////////////////////////////////////////////////////////////////////////

inline const char* CryGetExt( const char *filepath )
{
	const char *str = filepath;
	size_t len = strlen(filepath);
	for (const char* p = str + len-1; p >= str; --p)
	{
		switch(*p)
		{
		case ':':
		case '/':
		case '\\':
			// we've reached a path separator - it means there's no extension in this name
			return "";
		case '.':
			// there's an extension in this file name
			return p+1;
		}
	}
	return "";
}

// Summary:
//	 Checks if specified file name is a character file.
// Summary:
//	 Checks if specified file name is a character file.
inline bool IsCharacterFile( const char *filename )
{
	const char *ext = CryGetExt(filename);
	if (stricmp(ext,CRY_SKEL_FILE_EXT)==0 || stricmp(ext,CRY_SKIN_FILE_EXT)==0 ||	stricmp(ext,CRY_CHARACTER_DEFINITION_FILE_EXT)==0 || stricmp(ext,CRY_ANIM_GEOMETRY_FILE_EXT)==0)
	{
		return true;
	}
	else
		return false;
}

// Description:
//	 Checks if specified file name is a static geometry file.
inline bool IsStatObjFile( const char *filename )
{
	const char *ext = CryGetExt(filename);
	if (stricmp(ext,CRY_GEOMETRY_FILE_EXT) == 0)
	{
		return true;
	}
	else
		return false;
}

// Summary:
//	 Wrapper on file system.
class CCryFile
{
public:
	CCryFile();
	CCryFile( ICryPak * pIPak ); // allow an alternative ICryPak interface
	CCryFile( const char *filename, const char *mode );
	~CCryFile();

	bool Open( const char *filename, const char *mode,int nOpenFlagsEx=0 );
	void Close();

	// Summary:
	//	 Writes data in a file to the current file position.
	size_t Write( const void *lpBuf,size_t nSize );
	// Summary:
	//	 Reads data from a file at the current file position.
	size_t ReadRaw( void *lpBuf, size_t nSize );
	// Summary:
	//	 Template version, for automatic size support.
	template<class T>
		inline size_t ReadTypeRaw( T *pDest, size_t nCount = 1 )
		{
			return ReadRaw( pDest, sizeof(T)*nCount );
		}

	// Summary:
 	//	 Automatic endian-swapping version.
	template<class T>
		inline size_t ReadType( T *pDest, size_t nCount = 1 )
		{
			size_t nRead = ReadRaw( pDest, sizeof(T)*nCount );
			SwapEndian(pDest, nCount);
			return nRead;
		}

	// Summary:
	//	 Retrieves the length of the file.
	size_t GetLength();

	// Summary:
	//	 Moves the current file pointer to the specified position.
	size_t Seek( size_t seek, int mode );
	// Summary:
	//	 Moves the current file pointer at the beginning of the file.
	void SeekToBegin();
	// Summary:
	//	 Moves the current file pointer at the end of the file.
	size_t SeekToEnd();
	// Summary:
	//	 Retrieves the current file pointer.
	size_t GetPosition();

	// Summary:
	//	 Tests for end-of-file on a selected file.
	bool IsEof();

	// Summary:
	//	 Flushes any data yet to be written.
	void Flush();

	// Summary:
	//	 Gets a handle to a pack object.
	FILE* GetHandle() const { return m_file; };

	// Description:
	//    Retrieves the filename of the selected file.
	const char* GetFilename() const { return m_filename; };

	// Description:
	//    Retrieves the filename after adjustment to the real relative to engine root path.
	// Example:
	//    Original filename "textures/red.dds" adjusted filename will look like "game/textures/red.dds"
	// Return:
	//    Adjusted filename, this is a pointer to a static string, copy return value if you want to keep it.
	const char* GetAdjustedFilename() const;

	// Summary:
	//	 Checks if file is opened from pak file.
	bool IsInPak() const;

	// Summary:
	//	 Gets path of archive this file is in.
	const char* GetPakPath() const;

private:
	char m_filename[CRYFILE_MAX_PATH];
	FILE *m_file;
	ICryPak *m_pIPak;
};

#define IfPak(PakFunc, stdfunc, args) (m_pIPak ? m_pIPak->PakFunc args : stdfunc args)

#ifndef __SPU__
// Summary:
// CCryFile implementation.
inline CCryFile::CCryFile()
{
	m_file = 0;
	m_pIPak = gEnv ? gEnv->pCryPak : NULL;
}

inline CCryFile::CCryFile( ICryPak * pIPak )
{
	m_file = 0;
	m_pIPak = pIPak;
}

//////////////////////////////////////////////////////////////////////////
inline CCryFile::CCryFile( const char *filename, const char *mode )
{
	m_file = 0;
	m_pIPak = gEnv ? gEnv->pCryPak : NULL;
	Open( filename,mode );
}

//////////////////////////////////////////////////////////////////////////
inline CCryFile::~CCryFile()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////
// Notes:
//	 For nOpenFlagsEx see ICryPak::EFOpenFlags
// See also:
//	 ICryPak::EFOpenFlags
inline bool CCryFile::Open( const char *filename, const char *mode,int nOpenFlagsEx )
{
	char tempfilename[CRYFILE_MAX_PATH];
	strncpy_s( tempfilename,filename,_TRUNCATE );

#if !defined ( _RELEASE )
	if ( gEnv && gEnv->IsEditor() )
	{
		ICVar * const pCvar = gEnv->pConsole->GetCVar("ed_lowercasepaths");
		if ( pCvar )
		{
			const int lowercasePaths = pCvar->GetIVal();
			if ( lowercasePaths )
			{
				const string lowerString = PathUtil::ToLower(tempfilename);
				strncpy_s( tempfilename,lowerString.c_str(),_TRUNCATE );
			}
		}
	}
#endif
	if (m_file)
	{
		Close();
	}
	strncpy_s( m_filename,tempfilename,_TRUNCATE );

	m_file = m_pIPak ? m_pIPak->FOpen( tempfilename,mode,nOpenFlagsEx ) : fopen( tempfilename,mode );
	return m_file != NULL;
}

//////////////////////////////////////////////////////////////////////////
inline void CCryFile::Close()
{
	if (m_file)
	{
		IfPak( FClose, fclose, (m_file) );
		m_file = 0;
		strcpy_s( m_filename,"" );
	}
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::Write( const void *lpBuf,size_t nSize )
{
	assert( m_file );
	return IfPak( FWrite, fwrite, (lpBuf,1,nSize,m_file));
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::ReadRaw( void *lpBuf,size_t nSize )
{
	assert( m_file );
	return IfPak( FReadRaw, fread, (lpBuf,1,nSize,m_file) );
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::GetLength()
{
	assert( m_file );
	if (m_pIPak)
		return m_pIPak->FGetSize(m_file);
	long curr = ftell(m_file);
	if (fseek(m_file,0,SEEK_END) != 0)
		return 0;
	long size = ftell(m_file);
	if (fseek(m_file,curr,SEEK_SET) != 0)
		return 0;
	return size;
}

#ifdef WIN64
#pragma warning( push )									//AMD Port
#pragma warning( disable : 4267 )
#endif

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::Seek( size_t seek, int mode )
{
	assert( m_file );
	return IfPak( FSeek, fseek, (m_file, long(seek), mode) );
}

#ifdef WIN64
#pragma warning( pop )									//AMD Port
#endif

//////////////////////////////////////////////////////////////////////////
inline void CCryFile::SeekToBegin()
{
	Seek( 0,SEEK_SET );
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::SeekToEnd()
{
	return Seek( 0,SEEK_END );
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::GetPosition()
{
	assert(m_file);
	return IfPak( FTell, ftell, (m_file) );
}

//////////////////////////////////////////////////////////////////////////
inline bool CCryFile::IsEof()
{
	assert(m_file);
	return IfPak( FEof, feof, (m_file) ) != 0;
}

//////////////////////////////////////////////////////////////////////////
inline void CCryFile::Flush()
{
	assert( m_file );
	IfPak( FFlush, fflush, (m_file) );
}

//////////////////////////////////////////////////////////////////////////
inline bool CCryFile::IsInPak() const
{
	if (m_file && m_pIPak)
		return m_pIPak->GetFileArchivePath(m_file) != NULL;
	return false;
}

//////////////////////////////////////////////////////////////////////////
inline const char* CCryFile::GetPakPath() const
{
	if (m_file && m_pIPak)
	{
		const char *sPath = m_pIPak->GetFileArchivePath(m_file);
		if (sPath != NULL)
			return sPath;
	}
	return "";
}


//////////////////////////////////////////////////////////////////////////
inline const char* CCryFile::GetAdjustedFilename() const
{
	static char szAdjustedFile[ICryPak::g_nMaxPath];
	assert(m_pIPak);
	if (!m_pIPak)
		return "";

	// Gets mod path to file.
	const char *gameUrl = m_pIPak->AdjustFileName( m_filename, szAdjustedFile, 0 );

	// Returns standard path otherwise.
	strncpy_s(szAdjustedFile, gameUrl,_TRUNCATE);
	return szAdjustedFile;
}
#endif//__SPU__
#endif // __cryfile_h__
