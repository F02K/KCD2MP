////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   CryName.h
//  Version:     v1.00
//  Created:     6/10/2004 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(CryName.h)

#ifndef __CryName_h__
#define __CryName_h__
#pragma once

#include <ISystem.h>
#include <StlUtils.h>
#include <CrySizer.h>

#if defined PS3OPT
	#define USE_WRONLY_NAMETABLE 1
#endif

class CNameTable;


UNIQUE_IFACE struct INameTable
{
	virtual ~INameTable(){}
#if defined USE_WRONLY_NAMETABLE
	struct SNameEntry
	{
    const char *GetStr() { return reinterpret_cast<const char*>(this); }
    void AddRef() { }
    int Release() { return 1; }
    int GetMemoryUsage() { return strlen(GetStr()); }
		int	GetLength(){return strlen(GetStr());}
  };
#else
	// Name entry header, immediately after this header in memory starts actual string data.
	struct SNameEntry
	{
		// Reference count of this string.
		int nRefCount;
		// Current length of string.
		int nLength;
		// Size of memory allocated at the end of this class.
		int nAllocSize;
		// Here in memory starts character buffer of size nAllocSize.
		//char data[nAllocSize]

		const char* GetStr() { return (char*)(this+1); }
		void  AddRef() { nRefCount++; /*InterlockedIncrement(&_header()->nRefCount);*/};
		int   Release() { return --nRefCount; };
    int   GetMemoryUsage() { return sizeof(SNameEntry)+strlen(GetStr()); }
		int		GetLength(){return nLength;}
	};
#endif

	// Finds an existing name table entry, or creates a new one if not found.
	virtual INameTable::SNameEntry* GetEntry( const char *str ) = 0;
	// Only finds an existing name table entry, return 0 if not found.
	virtual INameTable::SNameEntry* FindEntry( const char *str ) = 0;
	// Release existing name table entry.
	virtual void Release( SNameEntry *pEntry ) = 0;
  virtual int GetMemoryUsage() = 0;
  virtual int GetNumberOfEntries() = 0;

	// Output all names from the table to log.
	virtual void LogNames() = 0;

	virtual void GetMemoryUsage( ICrySizer *pSizer ) const  =0;
};

#if defined USE_WRONLY_NAMETABLE


#ifdef PS3
	#define CNAMETABLE_BLOCKSIZE (64<<10)
#else
	#define CNAMETABLE_BLOCKSIZE (4096)
#endif

// If a block has less than CLOSE_THRESHOLD bytes left, then we'll close it
// (move it to the closed block list).
#define CNAMETABLE_CLOSE_THRESHOLD (64)
#include "STLPoolAllocator.h"

class CNameTable : public INameTable
{
  typedef std::set<char*, stl::less_stricmp<char*>, stl::STLPoolAllocator<char*> > TableT;
	static inline int compare(const char *s1, const char *s2)
	{ return stricmp(s1, s2); }

	TableT m_Table;

  struct SNameTableBlock
  {
    SNameTableBlock *m_pNext;
    size_t m_Size;
    char *Data() { return reinterpret_cast<char *>(this + 1); }
  };

  SNameTableBlock *m_pBlocks;
  SNameTableBlock *m_pClosedBlocks;

  char *Alloc(const char *str)
  {
    SNameTableBlock *pBlock = m_pBlocks, *pPrevBlock = NULL;
    size_t len = strlen(str) + 1;

    if (len >= CNAMETABLE_BLOCKSIZE)
    {
      pBlock = reinterpret_cast<SNameTableBlock *>(
      malloc(sizeof(SNameTableBlock) + len));
      pBlock->m_pNext = m_pClosedBlocks;
      m_pClosedBlocks = pBlock;
      pBlock->m_Size = len;
      char *data = pBlock->Data();
      memcpy(data, str, len);
      return data;
    }
    while (pBlock != NULL)
    {
      if (len <= CNAMETABLE_BLOCKSIZE - pBlock->m_Size)
        break;
      pPrevBlock = pBlock;
      pBlock = pBlock->m_pNext;
    }
    if (pBlock == NULL)
    {
      pBlock = reinterpret_cast<SNameTableBlock *>(
      malloc(sizeof(SNameTableBlock) + CNAMETABLE_BLOCKSIZE));
      pBlock->m_pNext = m_pBlocks;
      m_pBlocks = pBlock;
      pBlock->m_Size = len;
      char *data = pBlock->Data();
      memcpy(data, str, len);
      return data;
    }
    size_t size = pBlock->m_Size;
    assert(pBlock != NULL && len <= CNAMETABLE_BLOCKSIZE - size);
    char *data = pBlock->Data() + size;
    size += len;
    pBlock->m_Size = size;
    if (CNAMETABLE_BLOCKSIZE - size < CNAMETABLE_CLOSE_THRESHOLD)
    {
      if (pPrevBlock != NULL)
			{
				assert(pPrevBlock->m_pNext == pBlock);
        pPrevBlock->m_pNext = pBlock->m_pNext;
			}
      else
			{
				assert(m_pBlocks == pBlock);
        m_pBlocks = pBlock->m_pNext;
			}
      pBlock->m_pNext = m_pClosedBlocks;
      m_pClosedBlocks = pBlock;
    }
    memcpy(data, str, len);
    return data;
  }

public:
  CNameTable() : m_pBlocks(NULL), m_pClosedBlocks(NULL) { }

	VIRTUAL ~CNameTable()
	{
		for (SNameTableBlock *pBlock = m_pBlocks; pBlock != NULL; )
		{
			void *p = (void*)pBlock;
			pBlock = pBlock->m_pNext;
			free(p);
		}
		for (SNameTableBlock *pBlock = m_pClosedBlocks; pBlock != NULL; )
		{
			void *p = (void*)pBlock;
			pBlock = pBlock->m_pNext;
			free(p);
		}
	}

  VIRTUAL INameTable::SNameEntry *FindEntry(const char *str)
  {
    TableT &table = m_Table;

    if (table.empty())
      return NULL;
    TableT::const_iterator it = table.find(const_cast<char *>(str));
    if (it == table.end())
      return NULL;
    return reinterpret_cast<INameTable::SNameEntry *>(*it);
  }

  VIRTUAL INameTable::SNameEntry *GetEntry(const char *str)
  {
		ScopedSwitchToGlobalHeap useGlobalHeap;

    TableT &table = m_Table;

    if (table.empty())
    {
      char *entry = Alloc(str);
      table.insert(entry);
      return reinterpret_cast<INameTable::SNameEntry *>(entry);
    }
    TableT::iterator it = table.lower_bound(const_cast<char *>(str));
    if (it == table.end() || compare(str, *it))
    {
      char *entry = Alloc(str);
      table.insert(it, entry);
      return reinterpret_cast<INameTable::SNameEntry *>(entry);
    }
    return reinterpret_cast<INameTable::SNameEntry *>(*it);
  }

  VIRTUAL void Release(INameTable::SNameEntry *pEntry) 
	{
		if (!m_Table.empty())	m_Table.erase((char*)pEntry);
	}

  VIRTUAL int GetNumberOfEntries()
  {
    return m_Table.size();
  }

  VIRTUAL int GetMemoryUsage()
  {
    const TableT &table = m_Table;
    int size = sizeof(TableT) + table.size() * sizeof(char *);
    for (SNameTableBlock *pBlock = m_pBlocks;pBlock != NULL; pBlock = pBlock->m_pNext)
    {
      size += sizeof *pBlock
        + std::max(
            pBlock->m_Size,
            static_cast<size_t>(CNAMETABLE_BLOCKSIZE));
    }
    for (SNameTableBlock *pBlock = m_pClosedBlocks;
        pBlock != NULL; pBlock = pBlock->m_pNext)
    {
      size += sizeof *pBlock
        + std::max(
            pBlock->m_Size,
            static_cast<size_t>(CNAMETABLE_BLOCKSIZE));
    }
    return size;
  }
	VIRTUAL void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->AddObject(this, sizeof(*this));
		pSizer->AddContainer(m_Table);		
		for (SNameTableBlock *pBlock = m_pBlocks;pBlock != NULL; pBlock = pBlock->m_pNext)
		{
			pSizer->AddObject(pBlock, sizeof *pBlock + std::max(pBlock->m_Size, static_cast<size_t>(CNAMETABLE_BLOCKSIZE)));			
		}
		for (SNameTableBlock *pBlock = m_pClosedBlocks;
			pBlock != NULL; pBlock = pBlock->m_pNext)
		{
			pSizer->AddObject(pBlock, sizeof *pBlock + std::max(pBlock->m_Size, static_cast<size_t>(CNAMETABLE_BLOCKSIZE)));			
		}
	}

	// Log all names inside CryName table.
	VIRTUAL void LogNames()
	{
#ifndef __SPU__
    TableT &table = m_Table;
		for (TableT::const_iterator it=table.begin(); it!=table.end(); ++it)
		{
			INameTable::SNameEntry *const pNameEntry = reinterpret_cast<INameTable::SNameEntry*>(*it);
			CryLog( "[%4d] %s",pNameEntry->GetLength(),pNameEntry->GetStr() );
		}
#endif
	}
};
#else // else if defined USE_WRONLY_NAMETABLE

//////////////////////////////////////////////////////////////////////////
class CNameTable : public INameTable
{
private:
	typedef stl::hash_map<const char*,SNameEntry*,stl::hash_stricmp<const char*>, stl::STLGlobalAllocator<std::pair<const char*,SNameEntry*> > > NameMap;
	NameMap m_nameMap;

public:
	CNameTable() {}

	~CNameTable()
	{
		for( NameMap::iterator it = m_nameMap.begin() ; it != m_nameMap.end() ; ++it )
		{
			free(it->second);
		}				
	}

	// Only finds an existing name table entry, return 0 if not found.
	VIRTUAL INameTable::SNameEntry* FindEntry( const char *str )
	{
		SNameEntry *pEntry = stl::find_in_map( m_nameMap,str,0 );
		return pEntry;
	}

	// Finds an existing name table entry, or creates a new one if not found.
	VIRTUAL INameTable::SNameEntry* GetEntry( const char *str )
	{
		SNameEntry *pEntry = stl::find_in_map( m_nameMap,str,0 );
		if (!pEntry)
		{
			// Create a new entry.
			unsigned int nLen = strlen(str);
			unsigned int allocLen = sizeof(SNameEntry) + (nLen+1)*sizeof(char);
			pEntry = (SNameEntry*)malloc( allocLen );
			assert(pEntry!=NULL);
			pEntry->nRefCount = 0;
			pEntry->nLength = nLen;
			pEntry->nAllocSize = allocLen;
			// Copy string to the end of name entry.
			char *pEntryStr = const_cast<char*>(pEntry->GetStr());
			memcpy( pEntryStr,str,nLen+1 );
			// put in map.
			//m_nameMap.insert( NameMap::value_type(pEntry->GetStr(),pEntry) );
			m_nameMap[pEntry->GetStr()] = pEntry;
		}
		return pEntry;
	}

	// Release existing name table entry.
	VIRTUAL void Release( SNameEntry *pEntry )
	{
		assert(pEntry);
		m_nameMap.erase( pEntry->GetStr() );
		free(pEntry);
	}
  VIRTUAL int GetMemoryUsage()
  {
    int nSize = 0;
    NameMap::iterator it;
    int n = 0;
    for (it=m_nameMap.begin(); it!=m_nameMap.end(); it++)
    {
      nSize += strlen(it->first);
      nSize += it->second->GetMemoryUsage();
      n++;
    }
    nSize += n*8;

    return nSize;
  }
	VIRTUAL void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->AddObject(this, sizeof(*this));
		pSizer->AddObject(m_nameMap);
	}
  VIRTUAL int GetNumberOfEntries()
  {
    return m_nameMap.size();
  }

	// Log all names inside CryName table.
	VIRTUAL void LogNames()
	{
		NameMap::iterator it;
		for (it=m_nameMap.begin(); it!=m_nameMap.end(); ++it)
		{
			SNameEntry *pNameEntry = it->second;
			CryLog( "[%4d] %s",pNameEntry->nLength,pNameEntry->GetStr() );
		}
	}

};
#endif // end else if defined USE_WRONLY_NAMETABLE

///////////////////////////////////////////////////////////////////////////////
// Class CCryName.
//////////////////////////////////////////////////////////////////////////
class	CCryName
{
public:
	CCryName();
	CCryName( const CCryName& n );
	explicit CCryName( const char *s );
	CCryName( const char *s,bool bOnlyFind );
	~CCryName();

	CCryName& operator=( const CCryName& n );
	CCryName& operator=( const char *s );

	bool	operator==( const CCryName &n ) const;
	bool	operator!=( const CCryName &n ) const;

	bool	operator==( const char *s ) const;
	bool	operator!=( const char *s ) const;

	bool	operator<( const CCryName &n ) const;
	bool	operator>( const CCryName &n ) const;

	bool	empty() const { return !m_str || !m_str[0]; }
	void	reset()	{	_release(m_str);	m_str = 0; }
  void	addref()	{	_addref(m_str); }
	
	const	char*	c_str() const { 
#if defined(__SPU__)
		// to create an empty in in main memory, assume that the object itself lies there, than cast the 
		// 0 ptr to a char array containing '\0' (the empty string)
		return (m_str) ? m_str: SPU_MAIN_PTR( (char*)&(m_str) );
#else
		return (m_str) ? m_str: ""; 
#endif
	}
	int	length() const { return _length(); };

	static bool find( const char *str ) { return GetNameTable()->FindEntry(str) != 0; }
	void GetMemoryUsage( ICrySizer *pSizer ) const  
	{
		//pSizer->AddObject(m_str);
		pSizer->AddObject( GetNameTable() ); // cause for slowness?
	}
  static int GetMemoryUsage()
  {
#ifdef USE_STATIC_NAME_TABLE
    CNameTable *pTable = GetNameTable();
#else
    INameTable *pTable = GetNameTable();
#endif
    return pTable->GetMemoryUsage();
  }
  static int GetNumberOfEntries()
  {
#ifdef USE_STATIC_NAME_TABLE
    CNameTable *pTable = GetNameTable();
#else
    INameTable *pTable = GetNameTable();
#endif
    return pTable->GetNumberOfEntries();
  }

	// Compare functor for sorting CCryNames lexically.
	struct CmpLex
	{
		bool operator () (const CCryName &n1, const CCryName &n2) const
		{
			return strcmp(n1.c_str(), n2.c_str()) < 0;
		}
	};

private:
	typedef INameTable::SNameEntry SNameEntry;

#ifdef USE_STATIC_NAME_TABLE
	static CNameTable* GetNameTable()
	{
    // Note: can not use a 'static CNameTable sTable' here, because that
    // implies a static destruction order depenency - the name table is
    // accessed from static destructor calls.
		static CNameTable *table = NULL;

    if (table == NULL)
      table = new CNameTable();
    return table;
	}
#else
	//static INameTable* GetNameTable() { return GetISystem()->GetINameTable(); }
	static INameTable* GetNameTable() 
  { 
    assert(gEnv && gEnv->pNameTable); 
    return gEnv->pNameTable; 
  }
#endif

#ifdef USE_WRONLY_NAMETABLE
  SNameEntry *_entry( const char *pBuffer ) const
  {
    assert(pBuffer);
    return reinterpret_cast<SNameEntry *>(const_cast<char *>(pBuffer));
  }
  void _release( const char *pBuffer) { }
	int  _length() const { return (m_str) ? strlen(m_str) + 1 : 0; };
	void _addref( const char *pBuffer ) { }
#else
	SNameEntry* _entry( const char *pBuffer ) const { assert(pBuffer); return ((SNameEntry*)pBuffer)-1; }
	void _release( const char *pBuffer ) {
		if (pBuffer && _entry(pBuffer)->Release() <= 0)
			GetNameTable()->Release(_entry(pBuffer));
	}
	int  _length() const { return (m_str) ? _entry(m_str)->nLength : 0; };
	void _addref( const char *pBuffer ) { if (pBuffer) _entry(pBuffer)->AddRef(); }
#endif

	const char *m_str;
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CryName
//////////////////////////////////////////////////////////////////////////
inline CCryName::CCryName()
{
	m_str = 0;
}

//////////////////////////////////////////////////////////////////////////
inline CCryName::CCryName( const CCryName& n )
{
	_addref( n.m_str );
	m_str = n.m_str;
}

//////////////////////////////////////////////////////////////////////////
inline CCryName::CCryName( const char *s )
{
	m_str = 0;
	*this = s;
}

//////////////////////////////////////////////////////////////////////////
inline CCryName::CCryName( const char *s,bool bOnlyFind )
{
	assert(s);
	m_str = 0;
	if (*s) // if not empty
	{
		SNameEntry *pNameEntry = GetNameTable()->FindEntry(s);
		if (pNameEntry)
		{
			m_str = pNameEntry->GetStr();
			_addref(m_str);
		}
	}
}

inline CCryName::~CCryName()
{
	_release(m_str);
}

//////////////////////////////////////////////////////////////////////////
inline CCryName&	CCryName::operator=( const CCryName &n )
{
	if (m_str != n.m_str)
	{
		_release(m_str);
		m_str = n.m_str;
		_addref(m_str);
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////
inline CCryName&	CCryName::operator=( const char *s )
{
	assert(s);
	const char *pBuf = 0;
	if (s && *s) // if not empty
	{
		pBuf = GetNameTable()->GetEntry(s)->GetStr();
	}
	if (m_str != pBuf)
	{
		_release(m_str);
		m_str = pBuf;
		_addref(m_str);
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////
inline bool	CCryName::operator==( const CCryName &n ) const {
	return m_str == n.m_str;
}

inline bool	CCryName::operator!=( const CCryName &n ) const {
	return !(*this == n);
}

inline bool	CCryName::operator==( const char* str ) const {
	return m_str && stricmp(m_str,str) == 0;
}

inline bool	CCryName::operator!=( const char* str ) const {
	if (!m_str)
		return true;
	return stricmp(m_str,str) != 0;
}

inline bool	CCryName::operator<( const CCryName &n ) const {
	return m_str < n.m_str;
}

inline bool	CCryName::operator>( const CCryName &n ) const {
	return m_str > n.m_str;
}

inline bool	operator==( const string &s,const CCryName &n ) {
	return n == s;
}
inline bool	operator!=( const string &s,const CCryName &n ) {
	return n != s;
}

inline bool	operator==( const char *s,const CCryName &n ) {
	return n == s;
}
inline bool	operator!=( const char *s,const CCryName &n ) {
	return n != s;
}

#include <crc32.h>

///////////////////////////////////////////////////////////////////////////////
// Class CCryNameCRC.
//////////////////////////////////////////////////////////////////////////
class	CCryNameCRC
{
public:
  CCryNameCRC();
  CCryNameCRC( const CCryNameCRC& n );
  CCryNameCRC( const char *s );
  CCryNameCRC( const char *s,bool bOnlyFind );
  explicit CCryNameCRC( uint32 n ) { m_nID = n; } //  We use "explicit" to prevent comparison of strings with ints due to implicit conversion.
  ~CCryNameCRC();

  CCryNameCRC& operator=( const CCryNameCRC& n );
  CCryNameCRC& operator=( const char *s );

  bool	operator==( const CCryNameCRC &n ) const;
  bool	operator!=( const CCryNameCRC &n ) const;

  bool	operator==( const char *s ) const;
  bool	operator!=( const char *s ) const;

  bool	operator<( const CCryNameCRC &n ) const;
  bool	operator>( const CCryNameCRC &n ) const;

  bool	empty() const { return m_nID == 0; }
  void	reset()	{	m_nID = 0; }
  uint32 get() const	{	return m_nID; }
  void  add(int nAdd) { m_nID += nAdd; }

  AUTO_STRUCT_INFO

    void GetMemoryUsage( ICrySizer *pSizer ) const {/*nothing*/}
private:

  uint32 m_nID;

};

//////////////////////////////////////////////////////////////////////////
// CCryNameCRC
//////////////////////////////////////////////////////////////////////////
inline CCryNameCRC::CCryNameCRC()
{
  m_nID = 0;
}

//////////////////////////////////////////////////////////////////////////
inline CCryNameCRC::CCryNameCRC( const CCryNameCRC& n )
{
  m_nID = n.m_nID;
}

//////////////////////////////////////////////////////////////////////////
inline CCryNameCRC::CCryNameCRC( const char *s )
{
  m_nID = 0;
  *this = s;
}

inline CCryNameCRC::~CCryNameCRC()
{
  m_nID = 0;
}

//////////////////////////////////////////////////////////////////////////
inline CCryNameCRC&	CCryNameCRC::operator=( const CCryNameCRC &n )
{
  m_nID = n.m_nID;
  return *this;
}

//////////////////////////////////////////////////////////////////////////
inline CCryNameCRC&	CCryNameCRC::operator=( const char *s )
{
  assert(s);
  if (*s) // if not empty
  {
    Crc32Gen *pGen = GetISystem()->GetCrc32Gen();
    m_nID = pGen->GetCRC32Lowercase(s);
  }
  return *this;
}


//////////////////////////////////////////////////////////////////////////
inline bool	CCryNameCRC::operator==( const CCryNameCRC &n ) const {
  return m_nID == n.m_nID;
}

inline bool	CCryNameCRC::operator!=( const CCryNameCRC &n ) const {
  return !(*this == n);
}

inline bool	CCryNameCRC::operator==( const char* str ) const
{
  assert(str);
  if (*str) // if not empty
  {
    Crc32Gen *pGen = GetISystem()->GetCrc32Gen();
    uint32 nID = pGen->GetCRC32Lowercase(str);
    return m_nID == nID;
  }
  return m_nID == 0;
}

inline bool	CCryNameCRC::operator!=( const char* str ) const {
  if (!m_nID)
    return true;
  if (*str) // if not empty
  {
    Crc32Gen *pGen = GetISystem()->GetCrc32Gen();
    uint32 nID = pGen->GetCRC32Lowercase(str);
    return m_nID != nID;
  }
  return false;
}

inline bool	CCryNameCRC::operator<( const CCryNameCRC &n ) const {
  return m_nID < n.m_nID;
}

inline bool	CCryNameCRC::operator>( const CCryNameCRC &n ) const {
  return m_nID > n.m_nID;
}

inline bool	operator==( const string &s,const CCryNameCRC &n ) {
  return n == s;
}
inline bool	operator!=( const string &s,const CCryNameCRC &n ) {
  return n != s;
}

inline bool	operator==( const char *s,const CCryNameCRC &n ) {
  return n == s;
}
inline bool	operator!=( const char *s,const CCryNameCRC &n ) {
  return n != s;
}

#endif //__CryName_h__
