///////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   WinBase.cpp
//  Version:     v1.00 
//  Created:     10/3/2004 by Michael Glueck
//  Compilers:   GCC
//  Description: Linux/Mac port support for Win32API calls
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#if defined(PS3)
  #include <sdk_version.h>
	#include <fenv.h>
#endif
#include <CryAssert.h>
#if !defined(PS3) && !defined(ORBIS)
	#include <signal.h>
#endif

#ifndef ORBIS
#include <pthread.h>
#endif
#include <sys/types.h>
#include <fcntl.h>

#ifdef APPLE
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/sysctl.h>						// for total physical memory on Mac
#include <CoreFoundation/CoreFoundation.h>	// for CryMessageBox
#include <mach/vm_statistics.h>				// host_statistics
#include <mach/mach_types.h>		
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#endif

#ifndef USE_FILE_HANDLE_CACHE
	#if defined(PS3) || defined(ORBIS)
		#define USE_FILE_HANDLE_CACHE
	#endif
#endif

#ifdef USE_FILE_HANDLE_CACHE
	#include "md5.h"
#endif

#if !defined(PATH_MAX)
	#define PATH_MAX MAX_PATH
#endif

#ifdef ORBIS
	#define ORBIS_USE_NATIVE_FILE_IO
#endif

#ifndef CAFE
#include <sys/time.h>
#endif


#if defined(ORBIS)
#include "platform.h"
#include "OrbisPThreads.h"
#endif

#if defined(PS3)

#include <sys/ppu_thread.h>
#include <sys/synchronization.h>
#include <sys/time_util.h>
#include <sys/sys_time.h>
#include <sys/select.h>
#include <sys/paths.h>
#include <cell/fs/cell_fs_file_api.h>
#include <sysutil/sysutil_sysparam.h>
#include <sysutil/sysutil_common.h>
#include <sysutil/sysutil_gamecontent.h>
#include <sys/timer.h>
#include <sys/memory.h>
#include <sys/dbg.h>
#include <sysutil/sysutil_syscache.h>
#include <arpa/inet.h>
#include <map>
#include <vector>

static bool g_EnableFP = false;

#endif//PS3

#if !defined(_RELEASE) || defined(_DEBUG)
#include <set>
unsigned int g_EnableMultipleAssert = 0;//set to something else than 0 if to enable already reported asserts
#endif

#ifdef USE_FILE_HANDLE_CACHE
void OpenFromFileHandleCacheAsFD( const char *adjustedFilename, int flags, int &fd, int dummy, int &err);
void CloseFromFileHandleCacheAsFD( int fd );
#endif

#if defined(LINUX) || defined(APPLE)
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>
#include <dirent.h>
#include "CryLibrary.h"
#endif


// File I/O compatibility macros
#if defined(PS3)
	#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
	#define FS_ERRNO_TYPE CellFsErrno
	#define FS_ENOENT CELL_FS_ENOENT
	#define FS_EINVAL CELL_FS_EINVAL
  #define FS_EISDIR CELL_FS_ERROR_EISDIR
	#define FS_DIRENT_TYPE CellFsDirent
	#define FS_DIR_TYPE int
	#define FS_DIR_NULL (-1)
	#define FS_STAT_TYPE CellFsStat
	#define FS_CLOSEDIR(dir, err) \
	(((err) = cellFsClosedir((dir))))
	#define FS_CLOSEDIR_NOERR(dir) \
	((cellFsClosedir((dir))))
	#define FS_OPENDIR(dirname, dir, err) \
		(((err) = cellFsOpendir((dirname), &(dir))))
	#define FS_READDIR(dir, ent, entsize, err) \
		((err) = cellFsReaddir((dir), &(ent), (std::uint64_t*)&(entsize)))
	#define FS_TYPE_DIRECTORY CELL_FS_TYPE_DIRECTORY
	#define FS_STAT(filename, buffer, err) \
		((err) = cellFsStat((filename), &(buffer)))
	#define FS_FSTAT(fd, buffer, err) \
		((err) = cellFsFstat((fd), &(buffer)))
	#define FS_OPEN(filename, flags, fd, mode, err) \
	(((err) = cellFsOpen((filename), (flags), &(fd), (mode), 0)))
	#define FS_CLOSE(fd, err) \
		 (((err) = cellFsClose((fd))))
	#define FS_CLOSE_NOERR(fd) \
		(cellFsClose((fd)))
	#define FS_O_RDWR CELL_FS_O_RDWR
	#define FS_O_RDONLY CELL_FS_O_RDONLY
	#define FS_O_WRONLY CELL_FS_O_WRONLY
#endif //PS3

#if defined(LINUX) || defined(APPLE) || defined(ORBIS)
typedef int FS_ERRNO_TYPE;
#if defined(APPLE) || defined(ORBIS)
typedef struct stat FS_STAT_TYPE;
#else
typedef struct stat64 FS_STAT_TYPE;
#endif
static const int FS_O_RDWR = O_RDWR;
static const int FS_O_RDONLY = O_RDONLY;
static const int FS_O_WRONLY = O_WRONLY;
static const FS_ERRNO_TYPE FS_EISDIR = EISDIR;

ILINE void FS_OPEN(const char* szFileName, int iFlags, int &iFileDesc, mode_t uMode, FS_ERRNO_TYPE& rErr)
{
	rErr = ((iFileDesc = open(szFileName, iFlags, uMode)) != -1) ? 0 : errno;
}

ILINE void FS_CLOSE(int iFileDesc, FS_ERRNO_TYPE& rErr)
{
	rErr = close(iFileDesc) != -1 ? 0 : errno;
}

ILINE void FS_CLOSE_NOERR(int iFileDesc)
{
	close(iFileDesc);
}
#endif

#if defined(APPLE) || defined(LINUX)
typedef DIR* FS_DIR_TYPE;
typedef dirent FS_DIRENT_TYPE;
static const FS_ERRNO_TYPE FS_ENOENT = ENOENT;
static const FS_ERRNO_TYPE FS_EINVAL = EINVAL;
static const FS_DIR_TYPE FS_DIR_NULL = NULL;
static const unsigned char FS_TYPE_DIRECTORY = DT_DIR;

ILINE void FS_OPENDIR(const char* szDirName, FS_DIR_TYPE& pDir, FS_ERRNO_TYPE& rErr)
{
	rErr = (pDir = opendir(szDirName)) != NULL ? 0 : errno;
}

ILINE void FS_READDIR(FS_DIR_TYPE pDir, FS_DIRENT_TYPE& kEnt, uint64_t& uEntSize, FS_ERRNO_TYPE& rErr)
{
	errno = 0;	// errno is used to determine if readdir succeeds after
	FS_DIRENT_TYPE* pDirent(readdir(pDir));
	if (pDirent == NULL)
	{
		uEntSize = 0;
		rErr = (errno == FS_ENOENT) ? 0 : errno;
	}
	else
	{
		kEnt = *pDirent;
		uEntSize = static_cast<uint64_t>(sizeof(FS_DIRENT_TYPE));
		rErr = 0;
	}
}

ILINE void FS_STAT(const char* szFileName, FS_STAT_TYPE& kStat, FS_ERRNO_TYPE& rErr)
{
#if defined(APPLE)
	rErr = stat(szFileName, &kStat) != -1 ? 0 : errno;
#else 
	rErr = stat64(szFileName, &kStat) != -1 ? 0 : errno;
#endif
}

ILINE void FS_FSTAT(int iFileDesc, FS_STAT_TYPE& kStat, FS_ERRNO_TYPE& rErr)
{
#if defined(APPLE)
	rErr = fstat(iFileDesc, &kStat) != -1 ? 0 : errno;
#else 
    rErr = fstat64(iFileDesc, &kStat) != -1 ? 0 : errno;
#endif

}

ILINE void FS_CLOSEDIR(FS_DIR_TYPE pDir, FS_ERRNO_TYPE& rErr)
{
	errno = 0;
	rErr = closedir(pDir) == 0 ?  0 : errno;
}

ILINE void FS_CLOSEDIR_NOERR(FS_DIR_TYPE pDir)
{
	closedir(pDir);
}

#endif

#if defined(ORBIS)
	class CReadDirectoryEntries
	{
	public:
		CReadDirectoryEntries(int dir)
		{
			m_dir=dir;
			m_next=NULL;
			m_buffer=NULL;
		}
		int ReadDirectory()
		{
			SceKernelStat stats;
			int err=sceKernelFstat(m_dir, &stats);
			if (err==SCE_OK)
			{
				m_buffer=malloc(stats.st_blksize+sizeof(SceKernelDirent));
				err=-1;
				if (m_buffer)
				{
					// Make sure we will have a null terminated entry at the end. Docs aren't clear.
					memset(m_buffer, 0, stats.st_blksize+sizeof(SceKernelDirent));
					err=sceKernelGetdents(m_dir, (char*)m_buffer, stats.st_blksize);
					m_next=(char*)m_buffer;
					if (err>0)
						err=0;
				}
			}
			return err;
		}
		bool GetNext(SceKernelDirent *pDirEnt)
		{
			SceKernelDirent *pCurEnt=(SceKernelDirent*)m_next;
			memcpy(pDirEnt, m_next, min(pCurEnt->d_reclen,(uint16)sizeof(*pDirEnt)));
			do 
			{
				m_next=m_next+pCurEnt->d_reclen;
			} while (((SceKernelDirent*)m_next)->d_fileno==0 && ((SceKernelDirent*)m_next)->d_reclen!=0);
			return pCurEnt->d_reclen!=0;
		}
		int Delete()
		{
			int err=sceKernelClose(m_dir);
			delete this;
			return err;
		}

	private:
		CReadDirectoryEntries();
		~CReadDirectoryEntries()
		{
			free(m_buffer);
		}
		int m_dir;
		void *m_buffer;
		char *m_next;
	};
	#define FS_TYPE_DIRECTORY SCE_KERNEL_DT_DIR
	#define FS_ENOENT SCE_KERNEL_ERROR_ENOTDIR
	#define FS_EINVAL SCE_KERNEL_ERROR_EINVAL
	#define FS_DIRENT_TYPE SceKernelDirent
	#define FS_DIR_TYPE void*
	#define FS_DIR_NULL NULL
	#define FS_CLOSEDIR(dir, err) err=((CReadDirectoryEntries*)dir)->Delete()
	#define FS_CLOSEDIR_NOERR(dir) ((CReadDirectoryEntries*)dir)->Delete()

	void FS_OPENDIR(const char* dirname, FS_DIR_TYPE& dir, FS_ERRNO_TYPE& err)
	{
		int dh = sceKernelOpen(dirname, SCE_KERNEL_O_RDONLY|SCE_KERNEL_O_DIRECTORY, 0);
		err = dh;
		dir = FS_DIR_NULL;
		if (dh >= 0)
		{
			CReadDirectoryEntries *pEntry = new CReadDirectoryEntries(dh);
			dir = (void*)pEntry;
			err = pEntry->ReadDirectory();
			if (err != 0) 
			{ 
				pEntry->Delete(); 
				dir = FS_DIR_NULL; 
			}
		}
	}
	void FS_READDIR(FS_DIR_TYPE& dir, FS_DIRENT_TYPE& ent, size_t& entsize, FS_ERRNO_TYPE& err)
	{
		CReadDirectoryEntries *pEntry = (CReadDirectoryEntries*)dir;
		err = 0;
		entsize = pEntry->GetNext(&ent) ? ent.d_reclen : 0;
	}
	void FS_STAT(const char* filename, FS_STAT_TYPE& buffer, FS_ERRNO_TYPE& err)
	{
		if (stat(filename, &buffer) == -1)
			err = errno;
		else
			err = 0;
	}
	void FS_FSTAT(int fd, FS_STAT_TYPE& buffer, FS_ERRNO_TYPE& err)
	{
		if (fstat(fd, &buffer) == -1)
			err = errno;
		else
			err = 0;
	}
#endif

#ifdef ORBIS_USE_NATIVE_FILE_IO
	int ConvertFileToHandle(FILE *f) { if (f) return (int)((UINT_PTR)f-1); return -1; }
	FILE* ConvertHandleToFile(int h) { return (FILE*)(EXPAND_PTR)((h>=0)?h+1:0); }
	int ConvertModeToFlags(const char *pMode)
	{
		int flags=0;
		if (pMode[0]=='r')
		{
			if (strchr(pMode, '+'))
				flags=SCE_KERNEL_O_RDWR;
			else
				flags=SCE_KERNEL_O_RDONLY;
		}
		else if (pMode[0]=='w')
		{
			if (strchr(pMode, '+'))
				flags=SCE_KERNEL_O_RDWR;
			else
				flags=SCE_KERNEL_O_WRONLY;
			flags|=SCE_KERNEL_O_TRUNC|SCE_KERNEL_O_CREAT;
		}
		else if (pMode[0]=='a')
		{
			if (strchr(pMode, '+'))
				flags=SCE_KERNEL_O_RDWR;
			else
				flags=SCE_KERNEL_O_WRONLY;
			flags|=SCE_KERNEL_O_CREAT|SCE_KERNEL_O_APPEND;
		}
		else
		{
			ORBIS_TO_IMPLEMENT;
		}
		return flags;
	}
#endif



#if (defined(PS3) || defined(LINUX) || defined(APPLE) || defined(ORBIS)) && (!defined(_RELEASE) || defined(_DEBUG))
	struct SAssertData
	{
		int line;
		char fileName[256-sizeof(int)];
		const bool operator==(const SAssertData& crArg) const
		{
			return crArg.line == line && (strcmp(fileName, crArg.fileName) == 0);
		}

		const bool operator<(const SAssertData& crArg) const
		{
			if(line == crArg.line)
				return strcmp(fileName, crArg.fileName) < 0;
			else
				return line < crArg.line;
		}

		SAssertData() : line(-1){}
		SAssertData(const int cLine, const char* cpFile) : line(cLine)
		{
			strcpy(fileName, cpFile);
		}

		SAssertData(const SAssertData& crAssertData)
		{
			memcpy((void*)this, &crAssertData, sizeof(SAssertData));
		}

		void operator=(const SAssertData& crAssertData)
		{
			memcpy((void*)this, &crAssertData, sizeof(SAssertData));
		}
	};


//#define OUTPUT_ASSERT_TO_FILE

	void HandleAssert(const char* cpMessage, const char* cpFunc, const char* cpFile, const int cLine)
	{
#if defined(OUTPUT_ASSERT_TO_FILE)
#if defined(PS3)
		static FILE *pAssertLogFile = cellFsOpen(SYS_APP_HOME"/Assert.log", "w+");
#else   
        static FILE *pAssertLogFile = fopen("Assert.log", "w+");
#endif
#endif
		bool report = true;
		static std::set<SAssertData> assertSet;
		SAssertData assertData(cLine, cpFile);
		if(!g_EnableMultipleAssert)
		{
			std::set<SAssertData>::const_iterator it = assertSet.find(assertData);
			if(it != assertSet.end())
				report = false;
			else
				assertSet.insert(assertData);
		}
		else
			assertSet.insert(assertData);
		if(report)
		{
			//added function to be able to place a breakpoint here or to print out to other consoles
			printf("ASSERT: %s in %s (%s : %d)\n",cpMessage, cpFunc, cpFile, cLine);
#if defined(OUTPUT_ASSERT_TO_FILE)
			if(pAssertLogFile)
			{
				fprintf(pAssertLogFile, "ASSERT: %s in %s (%s : %d)\n",cpMessage, cpFunc, cpFile, cLine);
				fflush(pAssertLogFile);
			}
#endif
		}
	}
#endif

#if defined PS3
	#define fopenwrapper_basedir_maxsize \
		((size_t)gPS3Env->nFopenWrapperBasedirMax)
	#define fopenwrapper_basedir ((char *)gPS3Env->pFopenWrapperBasedir + 0)
	#define fopenwrapper_trace_fopen (gPS3Env->bFopenWrapperTrace)
#endif // PS3

#if defined LINUX || defined(APPLE)
	extern size_t __attribute__((visibility("default"))) fopenwrapper_basedir_maxsize;
	extern char * fopenwrapper_basedir __attribute__((visibility("default")));
	extern bool __attribute__((visibility("default"))) fopenwrapper_trace_fopen;
#endif

void InitFOpenWrapper()
{
#if defined(PS3) && defined(PS3_CRYENGINE)
	gPS3Env->nFopenWrapperBasedirMax = MAX_PATH;
	
	static char basedirBuffer[MAX_PATH]	= "\0";	

	gPS3Env->pFopenWrapperBasedir = basedirBuffer;
	gPS3Env->bFopenWrapperTrace = 1;

#endif
}

#if defined(PS3) && defined(PS3_CRYENGINE)
//main conversion routine for PS3 and hard disk usage, user provides buffer to provide thread safety
//converts slashes, lowers plus adds the system user path
const char* const ConvertFileName(char *const pBuf, const char* const inCpName)
{
	//init hard drive access
	const char* cpName = inCpName;

	if (cpName[1] == ':' && cpName[0] == 'h')
	{
		strcpy(pBuf, g_GameStat.pGameDataUsrPath);
		cpName = inCpName + 2;
	}
	else
	if (cpName[1] == ':' && cpName[0] == 'd')
	{
		strcpy(pBuf, g_GameStat.pSourceMediaUsrPath );
		cpName = inCpName + 2;
	}
	else
	if (cpName[1] == ':' && cpName[0] == 'a')
	{
		if (!gPS3Env->bAppHomeReadOnly)
		{
			strcpy(pBuf, SYS_APP_HOME"/");
		}
		else
		{
			strcpy(pBuf, SYS_DEV_HDD0"/game_debug/");		
			strcat(pBuf, gPS3Env->sTitleID);
			strcat(pBuf, "/");
		}
		cpName = inCpName + 2;
	}
	else
	if (cpName[1] == ':' && cpName[0] == 'p')
	{
		strcpy(pBuf, g_GameStat.pPatchUsrdirPath);
		cpName = inCpName + 2;
	}
	else
	if (cpName[1] == ':' && cpName[0] == 'z')
	{
		strcpy(pBuf, gPS3Env->pDlcUsrdirPath);
		cpName = inCpName + 2;
		//PS3_WriteToGameLog("Convert '%s' -> '%s'", inCpName, pBuf);
	}
	else
	if (cpName[1] == ':' && ((cpName[0] >= '0') && (cpName[0] <= '9')))
	{
		char dlcNumber[2];
		dlcNumber[0] = cpName[0];
		dlcNumber[1] = '\0';

		//Mount DLC data as virtual drive e.g. 0:, 1:, 2:, 9:
		//DLC is in a fixed location e.g. USRDIR/dlc/dlc<n>/
		strcpy(pBuf, gPS3Env->pDlcUsrdirPath);
		strcat(pBuf, "/dlc");
		strcat(pBuf, dlcNumber);
		//PS3_WriteToGameLog("Convert '%s' -> '%s'", inCpName, pBuf);
		cpName = inCpName + 2;
	}
	else
		strcpy(pBuf, gPS3Env->pFopenWrapperBasedir);

	// Convert path slashes and remove caps
	const int cStrLen = strlen(cpName);
	char *__restrict pCurDest	= &pBuf[strlen(pBuf)];
	const char *__restrict pCurSrc = cpName;
	for(int i=0; i<cStrLen; ++i)
	{
		char c = *pCurSrc++;
		if(c == '\\') 
			c = '/';
		*pCurDest++ = tolower(c);
	}
	*pCurDest = '\0';
	return pBuf;
}
#elif defined(ORBIS)
	char sOrbisRootDir[MAX_PATH] = "/data/sdk/";

	const char* const ConvertFileName(char *const pBuf, const char* const cpName)
	{
		if (cpName[0] != '/')
		{
			// Convert relative path
			strcpy( pBuf, sOrbisRootDir );	
			strcat( pBuf, cpName );
			return pBuf;
		}
		// Return absolute path
		pBuf[0] = '\0';
		return cpName;
	}
#elif defined(PS3_CRYENGINE)
	const bool GetFilenameNoCase(const char*,char *,const bool);
	const char* const ConvertFileName(char *const pBuf, const char* const cpName)
	{
		GetFilenameNoCase(cpName, pBuf, false);
		return pBuf;
	}
#else
	const char* const ConvertFileName(char *const pBuf, const char* const cpName)
	{
		// Not supported outside of CryEngine.
		return NULL;
	}
#endif

#if defined(USE_FILE_MAP)
#define FILE_MAP_FILENAME "files.list"
struct FileNode
{
	int m_nEntries;
	int m_nIndex;							// Index relative to the parent, -1 for root.
	char *m_Buffer;			      // Buffer holding all entry strings, separated by
														// null characters.
	const char **m_Entries;		// Node entries, sorted in strcmp() order.
	FileNode **m_Children;    // Children of the node. NULL for non-directories.
	bool m_bDirty;						// Flag indicating that the node has been modified.

	FileNode();
	~FileNode();
	void Dump(int indent = 0);
	int Find(const char *);
	int FindExact(const char *);

	static FileNode *ReadFileList(FILE * = NULL, int = -1);
	static FileNode *BuildFileList(const char *prefix = NULL);
	static void Init(void)
	{
		m_FileTree = ReadFileList();
		if (m_FileTree == NULL)
			m_FileTree = BuildFileList();
	}
	static FileNode *GetTree(void) { return m_FileTree; }
	static FileNode *FindExact(
			const char *, int &, bool * = NULL, bool = false);
	static FileNode *Find(
			const char *, int &, bool * = NULL, bool = false);
	static bool CheckOpen(const char *, bool = false, bool = false);

	static FileNode *m_FileTree;

	struct Entry
	{
		string Name;
		FileNode *Node;
		Entry(const string &Name, FileNode *const Node)
			: Name(Name), Node(Node)
		{ }
	};

	struct EntryCompare
	{
		bool operator () (const Entry &op1, const Entry &op2)
		{
			return op1.Name < op2.Name;
		}
	};
};

FileNode *FileNode::m_FileTree = NULL;

FileNode::FileNode()
	: m_nEntries(0), m_nIndex(-1), m_Buffer(NULL),
	  m_Entries(NULL), m_Children(NULL), m_bDirty(false)
{ }

FileNode::~FileNode()
{
	for (int i = 0; i < m_nEntries; ++i)
	{
		if (m_Children[i]) delete m_Children[i];
	}
	delete[] m_Children;
	delete[] m_Entries;
	delete[] m_Buffer;
}

void FileNode::Dump(int indent)
{
	for (int i = 0; i < m_nEntries; ++i)
	{
		for (int j = 0; j < indent; ++j) putchar(' ');
		printf("%s%s\n", m_Entries[i], m_Children[i] ? "/" : "");
		if (m_Children[i]) m_Children[i]->Dump(indent + 2);
	}
}

// Find the index of the element with the specified name (case insensitive).
// The method returns -1 if the element is not found.
int FileNode::Find(const char *name)
{
	const int n = m_nEntries;

	for (int i = 0; i < n; ++i)
	{
		if (!strcasecmp(name, m_Entries[i]))
			return i;
	}
	return -1;
}

// Find the index of the element with the specified name (case sensitive).
// The method returns -1 if the element is not found.
int FileNode::FindExact(const char *name)
{
	int i, j, k;
	const int n = m_nEntries;
	int cmp;

	if (!n) return -1;
	for (i = 0, j = n / 2, k = n & 1;; k = j & 1, j /= 2) {
		cmp = strcmp(m_Entries[i + j], name);
		if (cmp == 0) return i + j;
		if (cmp < 0) i += j + k;
		if (!j) break;
	}
	return -1;
}

// Find the node for the specified file name. The method performs a case
// sensitive search.
// The name parameter must be absolute (i.e. start with a '/').
// The method returns the node associated with the directory containing the
// specified file or directory. The index of the requested file is stored into
// &index.
// The skipInitial flag indicates if the initial path component should be
// skipped (e.g. "/work" or "/app_home").
// If the containing directory node is found, but the requested file is not,
// then the method returns the node of the containing directory and sets
// index to -1.
// If containing directory is not found, then the method returns NULL and sets
// index to -1.
// If the file and/or containing directory is not found and dirty parameter is
// not NULL, then a flag value will be written to *dirty indicating if the
// search failed on a dirty node. If the failing node is dirty, then the
// requested file might have been created at runtime.
FileNode *FileNode::FindExact(
		const char *name,
		int &index,
		bool *dirty,
		bool skipInitial
		)
{
	const char *sep;
	FileNode *node = m_FileTree;

	assert(name[0] == '/');
	index = -1;
	if (node == NULL)
	{
		// No node tree present.
		if (dirty != NULL)
			*dirty = true;
		node = NULL;
	}
	if (skipInitial)
	{
		do ++name; while (*name && *name != '/');
		assert(!name[0] || name[0] == '/');
	}
	while (*name == '/') ++name;
	if (!name[0])
	{
		// Root directory requested.
		node = NULL;
	}
	while (node != NULL)
	{
		if (dirty != NULL)
			*dirty = node->m_bDirty;
		for (sep = name; *sep && *sep != '/'; ++sep);
		char component[sep - name + 1];
		memcpy(component, name, sep - name);
		component[sep - name] = 0;
		index = node->FindExact(component);
		name = sep;
		while (*name == '/') ++name;
		if (!name[0]) break;
		if (index != -1)
			node = node->m_Children[index];
		else
			node = NULL;
	}
	if (node == NULL)
		index = -1;
	return node;
}

FileNode *FileNode::Find(
		const char *name,
		int &index,
		bool *dirty,
		bool skipInitial
		)
{
	abort();
	// FIXME: Implement
	index = -1;
	return NULL;
}

// Check if a file open operation can be successful.
// The create flag specified if file creation is requested.
// The skipInitial flag indicates if the initial path component should be
// skipped (e.g. "/work" or "/app_home").
bool FileNode::CheckOpen(
		const char *filename,
		bool create,
		bool skipInitial
		)
{
	int index = -1;
	FileNode *node = NULL;
	bool dirty = false;
	bool fail = false;

	node = FileNode::FindExact(filename, index, &dirty, skipInitial);
	if (node == NULL && !dirty)
	{
		// The containing directory does not exist.
		fail = true;
	} else if (!create && index == -1 && !dirty)
	{
		// The file does not exist and file creation is not requested.
		fail = true;
	} else if (index == -1 && create && node != NULL && !dirty)
	{
		// The file does not exist and file creation *is* requested. We'll mark
		// the containing directory dirty.
		node->m_bDirty = true;
	}
	return !fail;
}

#if defined(PS3_CRYENGINE)
static const char* InitListFileName()
{
	static char listFilename[CELL_GAMEDATA_PATH_MAX];
	snprintf(listFilename, sizeof(listFilename),
			"%s/%s", gPS3Env->pCurDirHDD0, FILE_MAP_FILENAME);
	return listFilename;
}
#endif

FileNode *FileNode::ReadFileList(FILE *in, int index)
{
	char line[256];
#if defined(PS3)
		static const char *const listFilename = InitListFileName();	
#else
	static const char listFilename[] = FILE_MAP_FILENAME;
#endif
	int i, count = 0, size = 0;
	const int maxCount = 4096, maxSize = 256 * 4096;
	FileNode *node = NULL;
	bool closeFile = false;
	
	if (in == NULL)
	{
		in = fopen(listFilename, "r");
		if (in == NULL)
		{
#if !defined(LINUX) && !defined(APPLE)
			fprintf(stderr, "can't open file map '%s' for reading: %s\n",
					listFilename, strerror(errno));
#endif
			return NULL;
		}
		closeFile = true;
	}
	if (fgets(line, sizeof line, in) == NULL)
	{
		fprintf(stderr, "unexpected EOF in file map '%s'\n", listFilename);
		fclose(in);
		return NULL;
	}
	line[sizeof line - 1] = 0;
	if (sscanf(line, "%i,%i", &count, &size) != 2
			|| count < 0 || count > maxCount
			|| size < 0 || size > maxSize)
	{
		fprintf(stderr, "syntax error in file map '%s'\n", listFilename);
		fclose(in);
		return NULL;
	}
	node = new FileNode;
	node->m_nEntries = count;
	node->m_nIndex = index;
	node->m_Buffer = new char[size];
	node->m_Entries = new const char *[count];
	node->m_Children = new FileNode *[count];
	char *buffer = node->m_Buffer;
	for (i = 0; i < count; ++i)
	{
		if (fgets(line, sizeof line, in) == NULL)
		{
			fprintf(stderr, "unexpected EOF in file map '%s'\n", listFilename);
			fclose(in);
			delete node;
			return NULL;
		}
		int lineLen = strlen(line);
		while (lineLen > 0 && isspace(line[lineLen - 1])) --lineLen;
		line[lineLen] = 0;
		char *p = strchr(line, '/');
		if (p && p[1])
		{
			fprintf(stderr, "syntax error in file map '%s'\n", listFilename);
			fclose(in);
			delete node;
			return NULL;
		}
		bool isDir = false;
		if (p) 
		{
			*p = 0;
			isDir = true;
			--lineLen;
		}
		if (lineLen + (buffer - node->m_Buffer) >= size)
		{
			fprintf(stderr, "broken file map '%s'\n", listFilename);
			fclose(in);
			delete node;
			return NULL;
		}
		memcpy(buffer, line, lineLen + 1);
		node->m_Entries[i] = buffer;
		buffer += lineLen + 1;
		if (isDir)
		{
			node->m_Children[i] = ReadFileList(in, i);
			if (node->m_Children[i] == NULL)
			{
				fclose(in);
				delete node;
				return NULL;
			}
		}
	}
	if (buffer - node->m_Buffer != size)
	{
		fprintf(stderr, "broken file map '%s'\n", listFilename);
		fclose(in);
		delete node;
		return NULL;
	}
	if (closeFile)
		fclose(in);
	return node;
}

FileNode *FileNode::BuildFileList(const char *prefix)
{
	FileNode *node = NULL;
	FS_DIR_TYPE dir = FS_DIR_NULL;
	FS_ERRNO_TYPE err = 0;
	char curdir[PATH_MAX + 1];
	bool addPrefix = false;

	if (prefix == NULL)
	{
		// Start with the current working directory.
		GetCurrentDirectory(sizeof curdir - 1, curdir);
		curdir[sizeof curdir - 1] = 0;
		prefix = curdir;
		addPrefix = true;
	}
	FS_OPENDIR(prefix, dir, err);
	if (dir == FS_DIR_NULL || err != 0)
	{
		fprintf(stderr, "warning: can't access directory '%s': %s\n",
				prefix, strerror(err));
		return NULL;
	}
	std::vector<Entry> entryVec;
	size_t nameLengthTotal = 0;
	while (true)
	{
		FS_DIRENT_TYPE entry;
		size_t entrySize = 0;
		FS_READDIR(dir, entry, entrySize, err);
		if(err)
		{
			FS_CLOSEDIR_NOERR(dir);
			return NULL;
		}
		if (entrySize == 0)
			break;
		if (!strcmp(entry.d_name, ".") || !strcmp(entry.d_name, ".."))
			continue;
		FileNode *subNode = NULL;
		if (entry.d_type == FS_TYPE_DIRECTORY)
		{
			char path[PATH_MAX + 1];
			snprintf(path, sizeof path - 1, "%s/%s", prefix, entry.d_name);
			path[sizeof path - 1] = 0;
			subNode = BuildFileList(path);
			if (subNode == NULL)
				continue;
		}
		nameLengthTotal += strlen(entry.d_name);
		entryVec.push_back(Entry(entry.d_name, subNode));
	}
	FS_CLOSEDIR_NOERR(dir);
	const int count = entryVec.size();
	node = new FileNode;
	node->m_nEntries = count;
	if (count > 0)
	{
		std::sort(entryVec.begin(), entryVec.end(), EntryCompare());
		node->m_Buffer = new char[nameLengthTotal + count];
		node->m_Entries = new const char *[count];
		node->m_Children = new FileNode *[count];
		char *buffer = node->m_Buffer;
		for (int i = 0; i < count; ++i)
		{
			strcpy(buffer, entryVec[i].Name.c_str());
			node->m_Entries[i] = buffer;
			buffer += strlen(buffer) + 1;
			FileNode *subNode = entryVec[i].Node;
			node->m_Children[i] = subNode; // NULL for non-directories.
			if (subNode != NULL)
				subNode->m_nIndex = i;
		}
		assert(buffer - node->m_Buffer == nameLengthTotal + count);
	}
	if (addPrefix && strcmp(prefix, "/"))
	{
		// Add the file nodes for the default prefix (current working directory).
		// The nodes are marked as dirty, so the on disk directories are scanned
		// whenever one of these directories is accessed.
		const char *p, *q;
		assert(prefix[0] == '/');
		FileNode *rootNode = NULL, *x = NULL;
		for (p = q = prefix + 1;; ++p)
		{
			if (*p == '/' || *p == 0)
			{
				if (rootNode == NULL)
				{
					rootNode = new FileNode;
					x = rootNode;
					rootNode->m_nIndex = -1;
				}
				else
				{
					assert(x != NULL);
					x->m_Children[0] = new FileNode;
					x = x->m_Children[0];
					x->m_nIndex = 0;
				}
				x->m_nEntries = 1;
				x->m_Children = new FileNode *[1];
				x->m_bDirty = true;
				x->m_Buffer = new char[p - q + 1];
				memcpy(x->m_Buffer, q, p - q);
				x->m_Buffer[p - q] = 0;
				x->m_Entries = new const char *[1];
				x->m_Entries[0] = x->m_Buffer;
				q = p + 1;
			}
			if (*p == 0)
				break;
		}
		x->m_Children[0] = node;
		node->m_nIndex = 0;
		node = rootNode;
	}
	return node;
}

void InitFileList(void)
{
	FileNode::Init();






}
#else
void InitFileList(void) { }
#endif

inline void WrappedF_InitCWD()
{
#if defined(LINUX) || defined(APPLE)
#if !defined(APPLE_BUNDLE)
	if (getcwd(fopenwrapper_basedir, fopenwrapper_basedir_maxsize) == NULL)
	{
		fprintf(stderr, "getcwd(): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fopenwrapper_basedir[fopenwrapper_basedir_maxsize - 1] = 0;
#endif

	// Move up to parent of BinXX directory.
	// /home/user/depot/Main/Build/Linux-Arc/Bin -> /home/user/depot/Main
	// [K01]: comment below code till real build
	/*
	char *ptr = strrchr(fopenwrapper_basedir, '/');
	if (ptr != NULL)
	{
		bool upOneDir = false;
		if (strcasecmp(ptr, "/Bin32") == 0)
			upOneDir = true;
		else if (strcasecmp(ptr, "/Bin64") == 0)
			upOneDir = true;
			
		if (upOneDir)
		{
			*ptr = '\0';
			chdir(fopenwrapper_basedir);
		}
	}*/
	SetModulePath(fopenwrapper_basedir);
	char *ptr = strstr(fopenwrapper_basedir, "/Build");
	if (ptr != NULL)
	{
		*ptr = '\0';
		chdir(fopenwrapper_basedir);
	}
#endif
}

bool IsBadReadPtr(void* ptr, unsigned int size )
{
	//too complicated to really support it
	return ptr? false : true;
}

//////////////////////////////////////////////////////////////////////////
char* _strtime(char* date)
{
	strcpy( date,"0:0:0" );
	return date;
}

//////////////////////////////////////////////////////////////////////////
char* _strdate(char* date)
{
	strcpy( date,"0" );
	return date;
}

//////////////////////////////////////////////////////////////////////////
char* strlwr (char * str)
{
  char *cp;               /* traverses string for C locale conversion */

  for (cp=str; *cp; ++cp)
  {
    if ('A' <= *cp && *cp <= 'Z')
      *cp += 'a' - 'A';
  }
  return str;
}

char* strupr (char * str)
{
  char *cp;               /* traverses string for C locale conversion */

  for (cp=str; *cp; ++cp)
  {
    if ('a' <= *cp && *cp <= 'z')
      *cp += 'A' - 'a';
  }
  return str;
}

char *ltoa ( long i , char *a , int radix )
{
	if ( a == NULL ) return NULL ;
	strcpy ( a , "0" ) ;
	if ( i && radix > 1 && radix < 37 ) {
		char buf[35] ;
		unsigned long u = i , p = 34 ;
		buf[p] = 0 ;
		if ( i < 0 && radix == 10 ) u = -i ;
		while ( u ) {
			unsigned int d = u % radix ;
			buf[--p] = d < 10 ? '0' + d : 'a' + d - 10 ;
			u /= radix ;
		}
		if ( i < 0 && radix == 10 ) buf[--p] = '-' ;
		strcpy ( a , buf + p ) ;
	}
	return a ;
}

#if defined(PS3)
BOOL CopyFile(const char* cpSrcOrg, const char* cpDstOrg, bool/* bFailIfExists*/)
{
	char bufferSource[512];
	char bufferDestination[512];
	const char* cpSrc = ConvertFileName(bufferSource, cpSrcOrg);
	const char* cpDst = ConvertFileName(bufferDestination, cpDstOrg);
	//this copies a file, no file name adjustments are performed
	int rd = 0, wr = 0;

	BOOL retValue = true;

	uint64_t numRead;
	char fileBuf[32 * 1024];

	if(cellFsOpen(cpSrc, CELL_FS_O_RDONLY, &rd, NULL, 0) != CELL_FS_SUCCEEDED) 
		return false;

	if(cellFsOpen(cpDst, CELL_FS_O_WRONLY | CELL_FS_O_CREAT, &wr, NULL, 0) != CELL_FS_SUCCEEDED)
		goto done;
	
	do
	{
		if(cellFsRead(rd, fileBuf, sizeof(fileBuf), &numRead) != CELL_FS_SUCCEEDED) 
		{
			retValue = false;
			break;
		}
		if(numRead == 0) 
			break;
		if(cellFsWrite(wr, fileBuf, numRead, NULL) != CELL_FS_SUCCEEDED)  
		{
			retValue = false;
			break;
		}
	} 
	while(numRead > 0);
done:
	if(rd) 
		cellFsClose(rd);
	if(wr)
		cellFsClose(wr);
	return retValue;
}


// declared in PS3_Win32Wrapper.h for PS3.
// For Linux it's redefined to wcscasecmp and wcsncasecmp'
int wcsicmp (const wchar_t* s1, const wchar_t* s2)
{
	wint_t c1, c2;

	if (s1 == s2)
		return 0;

	do
	{
		c1 = towlower(*s1++);
		c2 = towlower(*s2++);
	}
	while (c1 && c1==c2);

	return (int) (c1-c2);
}

int wcsnicmp (const wchar_t* s1, const wchar_t* s2, size_t count)
{
	wint_t c1,c2;
	if (s1 == s2 || count == 0)
		return 0;

	do 
	{
		c1 = towlower(*s1++);
		c2 = towlower(*s2++);
	} 
	while((--count) && c1 && (c1==c2));
	return (int) (c1-c2);
}
#endif

void _makepath(char * path, const char * drive, const char *dir, const char * filename, const char * ext)
{
  char ch;
  char tmp[MAX_PATH];
  if ( !path )
	  return;
  tmp[0] = '\0';
  if (drive && drive[0])
  {
    tmp[0] = drive[0];
    tmp[1] = ':';
    tmp[2] = 0;
  }
  if (dir && dir[0])
  {
    strcat(tmp, dir);
    ch = tmp[strlen(tmp)-1];
    if (ch != '/' && ch != '\\')
	    strcat(tmp,"\\");
  }
  if (filename && filename[0])
  {
    strcat(tmp, filename);
    if (ext && ext[0])
    {
      if ( ext[0] != '.' )
				strcat(tmp,".");
      strcat(tmp,ext);
    }
  }
  strcpy( path, tmp );
}

char * _ui64toa(unsigned long long value,	char *str, int radix)
{
	if(str == 0)
		return 0;

	char buffer[65];
	char *pos;
	int digit;

	pos = &buffer[64];
	*pos = '\0';

	do 
	{
		digit = value % radix;
		value = value / radix;
		if (digit < 10) 
		{
			*--pos = '0' + digit;
		} else 
		{
			*--pos = 'a' + digit - 10;
		} /* if */
	} while (value != 0L);

	memcpy(str, pos, &buffer[64] - pos + 1);
	return str;
}

long long _atoi64( const char *str )
{
	if(str == 0)
		return -1;
	unsigned long long RunningTotal = 0;
	char bMinus = 0;
	while (*str == ' ' || (*str >= '\011' && *str <= '\015')) 
	{
		str++;
	} /* while */
	if (*str == '+') 
	{
		str++;
	} else if (*str == '-') 
	{
		bMinus = 1;
		str++;
	} /* if */
	while (*str >= '0' && *str <= '9') 
	{
		RunningTotal = RunningTotal * 10 + *str - '0';
		str++;
	} /* while */
	return bMinus? ((long long)-RunningTotal) : (long long)RunningTotal;
}
 
#ifdef PS3
int gettimeofday(timeval *__restrict tp, void *restrict)
{
	sys_time_sec_t sec;
	sys_time_nsec_t nsec;
	sys_time_get_current_time(&sec, &nsec);
	tp->tv_sec  = sec;
	tp->tv_usec = nsec / 1000;
	return 0;
}
#endif

#if !defined(PS3) && !defined(ORBIS)
bool QueryPerformanceCounter(LARGE_INTEGER *counter)
{
#if defined(LINUX)
	// replaced gettimeofday
	// http://fixunix.com/kernel/378888-gettimeofday-resolution-linux.html
	timespec tv;
	clock_gettime(CLOCK_MONOTONIC, &tv);
	counter->QuadPart = (uint64)tv.tv_sec * 1000000 + tv.tv_nsec / 1000;
	return true;
#elif defined(APPLE)
  counter->QuadPart = mach_absolute_time();
  return true;
#elif defined(CAFE)
	counter->QuadPart=OSGetTime();
	return true;
#else
	return false;
#endif
}

bool QueryPerformanceFrequency(LARGE_INTEGER *frequency)
{
#if defined(LINUX)
	// On Linux we'll use gettimeofday().  The API resolution is microseconds,
	// so we'll report that to the caller.
	frequency->u.LowPart  = 1000000;
	frequency->u.HighPart = 0;
	return true;
#elif defined(APPLE)
	static mach_timebase_info_data_t s_kTimeBaseInfoData;
	if (s_kTimeBaseInfoData.denom == 0)
        mach_timebase_info(&s_kTimeBaseInfoData);
	// mach_timebase_info_data_t expresses the tick period in nanoseconds
	frequency->QuadPart = 1e+9 * (uint64_t)s_kTimeBaseInfoData.denom / (uint64_t)s_kTimeBaseInfoData.numer;
	return true;
#elif defined(CAFE)
	frequency->QuadPart=OS_TIMER_CLOCK;
	return true;
#else
	return false;
#endif
}
#endif//PS3

void _splitpath(const char* inpath, char * drv, char * dir, char* fname, char * ext )
{	
	if (drv) 
		drv[0] = 0;

  typedef CryStackStringT<char, PATH_MAX> path_stack_string;

	const path_stack_string inPath(inpath);
	string::size_type s = inPath.rfind('/', inPath.size());//position of last /
	path_stack_string fName;
	if(s == string::npos)
	{
		if(dir)
			dir[0] = 0;
		fName = inpath;	//assign complete string as rest
	}
	else
	{
		if(dir)
			strcpy(dir, (inPath.substr((string::size_type)0,(string::size_type)(s+1))).c_str());	//assign directory
		fName = inPath.substr((string::size_type)(s+1));					//assign remaining string as rest
	}
	if(fName.size() == 0)
	{
		if(ext)
			ext[0] = 0;
		if(fname)
			fname[0] = 0;
	}
	else
	{
		//dir and drive are now set
		s = fName.find(".", (string::size_type)0);//position of first .
		if(s == string::npos)
		{
			if(ext)
				ext[0] = 0;
			if(fname)
				strcpy(fname, fName.c_str());	//assign filename
		}
		else
		{
			if(ext)
				strcpy(ext, (fName.substr(s)).c_str());		//assign extension including .
			if(fname)
			{
				if(s == 0)
					fname[0] = 0;
				else
					strcpy(fname, (fName.substr((string::size_type)0,s)).c_str());	//assign filename
			}
		}  
	} 
}

DWORD GetFileAttributes(LPCSTR lpFileName)
{
#if defined(PS3)
	CRY_ASSERT_MESSAGE(0, "GetFileAttributes not implemented yet");
	return FILE_ATTRIBUTE_READONLY;
#else	
	struct stat fileStats;
	const int success = stat(lpFileName, &fileStats);
	if(success == -1)
	{
		char adjustedFilename[MAX_PATH];
		GetFilenameNoCase(lpFileName, adjustedFilename);
		if(stat(adjustedFilename, &fileStats) == -1)
			return (DWORD)INVALID_FILE_ATTRIBUTES;
	}
	DWORD ret = 0; 

	const int acc = (fileStats.st_mode & S_IWRITE);

	if(acc != 0)
	if(S_ISDIR(fileStats.st_mode) != 0)
		ret |= FILE_ATTRIBUTE_DIRECTORY;
	return (ret == 0)?FILE_ATTRIBUTE_NORMAL:ret;//return file attribute normal as the default value, must only be set if no other attributes have been found
#endif
}
 
#if defined PS3
const char* GetCellFsErrString(int cRetCode)
{
	switch(cRetCode)
	{
	case CELL_FS_SUCCEEDED:
		return "Succeeded";
	case CELL_FS_ENOTMOUNTED:
		return "Path not mounted";
	case CELL_FS_ENOENT:
		return "File does not exist and CELL_FS_O_CREAT is not specified";
	case CELL_FS_EINVAL:
		return "Specified path is invalid";
	case CELL_FS_EMFILE:
		return "Max number of file descriptors exceeded";
	case CELL_FS_EISDIR:
		return "Specified file path is a directory";
	case CELL_FS_EIO:
		return "I/O error has occured";
	case CELL_FS_ENOMEM:
		return "Memory is insufficient";
	case CELL_FS_ENOTDIR:
		return "Components in path contain something other than a directory";
	case CELL_FS_ENAMETOOLONG:
		return "File path exceeds max number of length";
	case CELL_FS_EFSSPECIFIC:
		return "File system specific internal error has occurred";
	case CELL_FS_EFAULT:
		return "Path or fd is NULL";
	case CELL_FS_EPERM:
		return "Permission is invalid";
	case CELL_FS_EACCES:
		return "Permission is invalid (Only CFS and FAT)";
	case CELL_FS_EEXIST:
		return "File does already exist and CELL_FS_O_CREAT is specified";
	case CELL_FS_ENOSPC:
		return "No area to create new file";
	}
	return "Unknown";
}
#endif // defined PS3

int _mkdir(const char *dirname)
{
#if defined(ORBIS)
	char buf[512];
	const char *path = ConvertFileName(buf, dirname);
	int error = sceKernelMkdir(path, SCE_KERNEL_S_IRWU);
	if (error == SCE_OK || error == SCE_KERNEL_ERROR_EEXIST)
	{
		return 0; // OK
	}
	//printf( "sceKernelMkdir error 0x%x with arg %s\n", error, buf );
	return -1;
#elif !defined(PS3)
	const mode_t mode = 0x0777;
	const int suc = mkdir(dirname, mode);
	return (suc == 0)? 0 : -1;
#else
	//do not create those folders
	//assert(strlen(dirname) > 1 && strcmp(dirname, PathUtil::GetGameFolder().c_str()) && strstr(dirname, "haders"));
	char buf[512];
	const char* const cpConvName = ConvertFileName(buf, dirname);
#if defined(_DEBUG)
	//test if exists already 
	int dirHandle;
	if(cellFsOpendir(cpConvName, &dirHandle) == CELL_FS_SUCCEEDED)
	{
		fprintf(stderr, "_mkdir: tried to create an existing directory for \"%s\" \n", dirname);
		cellFsClosedir(dirHandle);
		return 0;
	}
#endif
	CellFsErrno Err	=	cellFsMkdir(cpConvName, CELL_FS_DEFAULT_CREATE_MODE_1);
	if(Err != CELL_FS_SUCCEEDED && Err != CELL_FS_EEXIST)
	{
		char errorBuf[512];
		sprintf(errorBuf, "_mkdir for \"%s\" failed with error: %s\n", dirname, GetCellFsErrString(Err));
//		CryFatalError(errorBuf);
		return -1;
	}
	else 
		return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
int memicmp( LPCSTR s1, LPCSTR s2, DWORD len )
{
  int ret = 0;
  while (len--)
  {
      if ((ret = tolower(*s1) - tolower(*s2))) break;
      s1++;
      s2++; 
  }
  return ret; 
}

//////////////////////////////////////////////////////////////////////////
int strcmpi( const char *str1, const char *str2 )
{
	for (;;)
	{
		int ret = tolower(*str1) - tolower(*str2);
		if (ret || !*str1) return ret;
		str1++;
		str2++;
	}
}

//-----------------------------------------other stuff-------------------------------------------------------------------

void GlobalMemoryStatus(LPMEMORYSTATUS lpmem)
{
		//not complete implementation
#if defined(PS3)
	memset(lpmem, 0,  sizeof(MEMORYSTATUS));
	sys_memory_info_t memInfo;
	if(sys_memory_get_user_memory_size(&memInfo) == CELL_OK)
	{
		lpmem->dwTotalPhys = memInfo.total_user_memory;
		lpmem->dwAvailPhys = memInfo.available_user_memory;
	}
#elif defined(ORBIS)
	memset(lpmem, 0,  sizeof(MEMORYSTATUS));
	lpmem->dwAvailPhys=lpmem->dwTotalPhys=sceKernelGetDirectMemorySize();
	off_t direct=0;
	// Potentially very slow loop over all physical memory to determine if it's allocated or not
	while (direct<lpmem->dwTotalPhys)
	{
		int type;
		off_t rStart, rEnd;
		if (sceKernelGetDirectMemoryType(direct, &type, &rStart, &rEnd)==SCE_OK)
		{
			INT_PTR diff=rEnd-rStart;
			lpmem->dwAvailPhys-=diff;
			direct=rEnd;
		}
		else
		{
			direct+=64*1024;
		}
	}
#elif defined(APPLE)
		
	// Retrieve dwTotalPhys 
	int kMIB[] = {CTL_HW, HW_MEMSIZE};
	size_t ulength = sizeof(lpmem->dwTotalPhys);
	if (sysctl(kMIB, 2, &lpmem->dwTotalPhys, &ulength, NULL, 0) != 0)
	{
		gEnv->pLog->LogError("sysctl failed\n");
	}
	
	// Get the page size
	mach_port_t kHost(mach_host_self());
	vm_size_t uPageSize;
	if (host_page_size(kHost, &uPageSize) != 0)
	{
		gEnv->pLog->LogError("host_page_size failed\n");
	}
	else
	{
		// Get memory statistics
		vm_statistics_data_t kVMStats;
		mach_msg_type_number_t uCount(sizeof(kVMStats) / sizeof(natural_t));
		if (host_statistics(kHost, HOST_VM_INFO, (host_info_t)&kVMStats, &uCount) != 0)
		{
			gEnv->pLog->LogError("host_statistics failed\n");
		}
		else
		{
			// Calculate dwAvailPhys
			lpmem->dwAvailPhys = uPageSize * kVMStats.free_count;
		}
	}
#else
	FILE *f;
	lpmem->dwMemoryLoad    = 0;
	lpmem->dwTotalPhys     = 16*1024*1024;
	lpmem->dwAvailPhys     = 16*1024*1024;
	lpmem->dwTotalPageFile = 16*1024*1024;
	lpmem->dwAvailPageFile = 16*1024*1024;
	f = ::fopen( "/proc/meminfo", "r" );
	if (f)
	{
		char buffer[256];
		memset(buffer, '0', 256);
		int total, used, free, shared, buffers, cached;

		lpmem->dwLength = sizeof(MEMORYSTATUS);
		lpmem->dwTotalPhys = lpmem->dwAvailPhys = 0;
		lpmem->dwTotalPageFile = lpmem->dwAvailPageFile = 0;
		while (fgets( buffer, sizeof(buffer), f ))
		{
				if (sscanf( buffer, "Mem: %d %d %d %d %d %d", &total, &used, &free, &shared, &buffers, &cached ))
				{
						lpmem->dwTotalPhys += total;
						lpmem->dwAvailPhys += free + buffers + cached;
				}
				if (sscanf( buffer, "Swap: %d %d %d", &total, &used, &free ))
				{
						lpmem->dwTotalPageFile += total;
						lpmem->dwAvailPageFile += free;
				}
				if (sscanf(buffer, "MemTotal: %d", &total))
						lpmem->dwTotalPhys = total*1024;
				if (sscanf(buffer, "MemFree: %d", &free))
						lpmem->dwAvailPhys = free*1024;
				if (sscanf(buffer, "SwapTotal: %d", &total))
						lpmem->dwTotalPageFile = total*1024;
				if (sscanf(buffer, "SwapFree: %d", &free))
						lpmem->dwAvailPageFile = free*1024;
				if (sscanf(buffer, "Buffers: %d", &buffers))
						lpmem->dwAvailPhys += buffers*1024;
				if (sscanf(buffer, "Cached: %d", &cached))
						lpmem->dwAvailPhys += cached*1024;
		}
		fclose( f );
		if (lpmem->dwTotalPhys)
		{
				DWORD TotalPhysical = lpmem->dwTotalPhys+lpmem->dwTotalPageFile;
				DWORD AvailPhysical = lpmem->dwAvailPhys+lpmem->dwAvailPageFile;
				lpmem->dwMemoryLoad = (TotalPhysical-AvailPhysical)  / (TotalPhysical / 100);
		}
  }
#endif
}

static const int YearLengths[2] = {DAYSPERNORMALYEAR, DAYSPERLEAPYEAR};
static const int MonthLengths[2][MONSPERYEAR] =
{ 
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static int IsLeapYear(int Year)
{
	return Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0) ? 1 : 0;
}

static void NormalizeTimeFields(short *FieldToNormalize, short *CarryField, int Modulus)
{
	*FieldToNormalize = (short) (*FieldToNormalize - Modulus);
	*CarryField = (short) (*CarryField + 1);
}

bool TimeFieldsToTime(PTIME_FIELDS tfTimeFields, PLARGE_INTEGER Time)
{
	#define SECSPERMIN         60
	#define MINSPERHOUR        60
	#define HOURSPERDAY        24
	#define MONSPERYEAR        12

	#define EPOCHYEAR          1601

	#define SECSPERDAY         86400
	#define TICKSPERMSEC       10000
	#define TICKSPERSEC        10000000
	#define SECSPERHOUR        3600

	int CurYear, CurMonth;
	LONGLONG rcTime;
	TIME_FIELDS TimeFields = *tfTimeFields;

	rcTime = 0;
	while (TimeFields.Second >= SECSPERMIN)
	{
		NormalizeTimeFields(&TimeFields.Second, &TimeFields.Minute, SECSPERMIN);
	}
	while (TimeFields.Minute >= MINSPERHOUR)
	{
		NormalizeTimeFields(&TimeFields.Minute, &TimeFields.Hour, MINSPERHOUR);
	}
	while (TimeFields.Hour >= HOURSPERDAY)
	{
		NormalizeTimeFields(&TimeFields.Hour, &TimeFields.Day, HOURSPERDAY);
	}
	while (TimeFields.Day > MonthLengths[IsLeapYear(TimeFields.Year)][TimeFields.Month - 1])
	{
		NormalizeTimeFields(&TimeFields.Day, &TimeFields.Month, SECSPERMIN);
	}
	while (TimeFields.Month > MONSPERYEAR)
	{
		NormalizeTimeFields(&TimeFields.Month, &TimeFields.Year, MONSPERYEAR);
	}
	for (CurYear = EPOCHYEAR; CurYear < TimeFields.Year; CurYear++)
	{ rcTime += YearLengths[IsLeapYear(CurYear)];
	}
	for (CurMonth = 1; CurMonth < TimeFields.Month; CurMonth++)
	{ rcTime += MonthLengths[IsLeapYear(CurYear)][CurMonth - 1];
	}
	rcTime += TimeFields.Day - 1;
	rcTime *= SECSPERDAY;
	rcTime += TimeFields.Hour * SECSPERHOUR + TimeFields.Minute * SECSPERMIN + TimeFields.Second;

	rcTime *= TICKSPERSEC;
	rcTime += TimeFields.Milliseconds * TICKSPERMSEC;

	Time->QuadPart = rcTime;

	return true;
}

BOOL SystemTimeToFileTime( const SYSTEMTIME *syst, LPFILETIME ft )
{
	TIME_FIELDS tf;
	LARGE_INTEGER t;

	tf.Year = syst->wYear;
	tf.Month = syst->wMonth;
	tf.Day = syst->wDay;
	tf.Hour = syst->wHour;
	tf.Minute = syst->wMinute;
	tf.Second = syst->wSecond;
	tf.Milliseconds = syst->wMilliseconds;

	TimeFieldsToTime(&tf, &t);
	ft->dwLowDateTime = t.u.LowPart;
	ft->dwHighDateTime = t.u.HighPart;
	return TRUE;
}

//begin--------------------------------findfirst/-next implementation----------------------------------------------------

void __finddata64_t::CopyFoundData(const char *rMatchedFileName)
{
	FS_STAT_TYPE fsStat;
	FS_ERRNO_TYPE fsErr = 0;
	bool isDir = false, isReadonly = false;

	memset(&fsStat, 0, sizeof(fsStat));
	const int cCurStrLen = strlen(name);
#if !defined(PS3)
	if(cCurStrLen > 0 && name[cCurStrLen-1] != '/')
#else
	if(name[cCurStrLen-1] != '/')
#endif	
	{
		name[cCurStrLen] = '/';
		name[cCurStrLen+1] = 0;
	}
	strncpy(name, rMatchedFileName, sizeof name - 1);
	name[sizeof name - 1] = 0;
	// Remove trailing slash for directories.
	if (name[0] && name[strlen(name) - 1] == '/')
	{
		name[strlen(name) - 1] = 0;
		isDir = true;
	}

	//more eff.impl for: 	string filename = string(m_DirectoryName) + "/" + name;
	char filename[MAX_PATH];
	const int cDirStrLen = strlen(m_DirectoryName);
	char *pDirCur = m_DirectoryName;
	char *pDst = filename;
	for(int i=0; i<cDirStrLen; ++i)
		*pDst++ = *pDirCur++;
  if(pDirCur[-1] != '/')
		*pDirCur = '/';
	const int cNameLen = strlen(name);
	pDirCur = name;
	for(int i=0; i<cNameLen; ++i)
		*pDst++ = *pDirCur++;
	*pDirCur = 0;
	*pDst = 0;

	// Check if the file is a directory and/or is read-only.
	if (!isDir)
	{
		FS_STAT(filename, fsStat, fsErr);
		if (fsErr) return;
	}
	(void)fsErr;

#if defined(LINUX) || defined(APPLE) || defined(ORBIS)
	// This is how it should be done. However, the st_mode field of
	// CellFsStat is currently unimplemented by the current version of the
	// Cell SDK (0.5.0).
	if (S_ISDIR(fsStat.st_mode)) isDir = true;
	if (!(fsStat.st_mode & S_IWRITE)) isReadonly = true;
#else
	{ 
/*
		// HACK
		int fd;
		if (!isDir) 
		{
			FS_OPEN(filename, FS_O_RDWR, fd, 0, fsErr);
			if (fsErr)
				isReadonly = true;
			else
				FS_CLOSE_NOERR(fd);
		}
*/
		isReadonly = false;
	}
#endif
	if (isDir)
		attrib |= _A_SUBDIR;
	else
		attrib &= ~_A_SUBDIR;
	if (isReadonly)
		attrib |= _A_RDONLY;
	else
		attrib &= ~_A_RDONLY;
	if (!isDir)
	{
		size = fsStat.st_size;
		time_access = fsStat.st_atime;
		time_write = fsStat.st_mtime;
		time_create = fsStat.st_ctime;
	} else
	{
    size = 0;
		time_access = time_write = time_create = 0;
	}
}

// Map all path components to the correct case.
// Slashes must be normalized to fwdslashes (/).
// Trailing path component is ignored (i.e. dirnames must end with a /).
// Returns 0 on success and -1 on error.
// In case of an error, all matched path components will have been case
// corrected.
int FixDirnameCase
(
	char *path, int index = 0
#if defined(USE_FILE_MAP)
	, FileNode *node = NULL
#endif
)
{
#if defined(PS3) && defined(PS3_CRYENGINE)
	//lower file name for everything behind the initial path
	const int cLen = strlen(path);
	for(int i=gPS3Env->nCurDirHDD0Len; i<cLen; ++i)
		path[i] = tolower(path[i]);
	return 0;
#else
	FS_ERRNO_TYPE fsErr = 0;
  char *slash;
	FS_DIR_TYPE dir = FS_DIR_NULL;
	bool pathOk = false;
	char *parentSlash;
#if defined(USE_FILE_MAP)
	FileNode *node1 = NULL;
#endif

	slash = strchr(path + index + 1, '/');
	if (!slash) return 0;
	*slash = 0;
	parentSlash = strrchr(path, '/');

#if defined(PS3)
	// On PS3 the first path component is *always* ok.
	// FIXME  remove the skipInitial semantics for PS3!
	if (index == 0)
		pathOk = true;
#endif

#if defined(USE_FILE_MAP)
	if (index == 0)
		node = FileNode::GetTree();
	if (node != NULL && !pathOk)
	{
		int i = node->FindExact(path + index + 1);
		if (i != -1)
		{
			node1 = node->m_Children[i];
			if (node1 != NULL)
			{
				assert(node1->m_nIndex == i);
				pathOk = true;
			}
		} else
		{
			i = node->Find(path + index + 1);
			if (i != -1)
			{
				node1 = node->m_Children[i];
				assert(node1 == NULL || node1->m_nIndex == i);
			}
		}
		if (!pathOk && node1 == NULL && !node->m_bDirty)
		{
			*slash = '/';
			return -1;
		}
	}
	if (node1 == NULL && !pathOk)
	{
#endif
		// Check if path is a valid directory.
		FS_OPENDIR(path, dir, fsErr);
		if (!fsErr)
		{
			pathOk = true;
			FS_CLOSEDIR_NOERR(dir);
			dir = FS_DIR_NULL;
		} else if (fsErr != FS_ENOENT && fsErr != FS_EINVAL)
		{
			*slash = '/';
			return -1;
		}
#if defined(USE_FILE_MAP)
	}

	if (node1 != NULL && !pathOk)
	{
		const char *name = node->m_Entries[node1->m_nIndex];
		if (parentSlash)
			memcpy(parentSlash + 1, name, strlen(name));
		else
			memcpy(path, name, strlen(name));
		pathOk = true;
	}
#endif

	if (!pathOk) {
		// Get the parent dir.
		const char *parent;
		char *name;
		if (parentSlash) {
			*parentSlash = 0;
			parent = path;
			if (!*parent) parent = "/";
			name = parentSlash + 1;
		} else
		{
			parent = ".";
			name = path;
		}

		// Scan parent.
		FS_OPENDIR(parent, dir, fsErr);
		if (fsErr)
		{
			if (parentSlash) *parentSlash = '/';
			*slash = '/';
			return -1;
		}
		FS_DIRENT_TYPE dirent;
		uint64_t direntSize = 0;
		while (true)
		{
			FS_READDIR(dir, dirent, direntSize, fsErr);
			if (fsErr)
			{
				FS_CLOSEDIR_NOERR(dir);
				if (parentSlash) *parentSlash = '/';
				*slash = '/';
				return -1;
			}
			if (direntSize == 0) break;
#if defined(ORBIS)
			size_t len = dirent.d_namlen;
			const char *dirEntName=dirent.d_name;
			if (len > 0 && dirEntName[len - 1] == '/') len -= 1;
			if (!strncasecmp(dirEntName, name, len))
			{
				pathOk = true;
				if (parentSlash)
					memcpy(parentSlash + 1, dirEntName, len);
				else
					memcpy(path, dirEntName, len);
				break;
			}
#else
#if defined(PS3)
			size_t len = dirent.d_namlen;
#else
			size_t len = strlen(dirent.d_name);
#endif
			if (len > 0 && dirent.d_name[len - 1] == '/') len -= 1;
			if (!strncasecmp(dirent.d_name, name, len))
			{
				pathOk = true;
				if (parentSlash)
					memcpy(parentSlash + 1, dirent.d_name, len);
				else
					memcpy(path, dirent.d_name, len);
				break;
			}
#endif
		}
		FS_CLOSEDIR(dir, fsErr);
		if (parentSlash) *parentSlash = '/';
		if (fsErr)
		{
			*slash = '/';
			return -1;
		}
	}
	*slash = '/';

	// Recurse.
	if (pathOk)
	{
#if defined(USE_FILE_MAP)
		return FixDirnameCase(path, slash - path, node1);
#else
		return FixDirnameCase(path, slash - path);
#endif
	}
#endif
	return -1;
}

// Match the specified name against the specified glob-pattern.
// Returns true iff name matches pattern.
static bool matchPattern(const char *name, const char *pattern)
{
  while (true)
	{
		if (!*pattern) return !*name;
		switch (*pattern)
		{
		case '?':
			if (!*name) return false;
			++name;
			++pattern;
			break;
		case '*':
			++pattern;
			while (true)
			{
				if (matchPattern(name, pattern)) return true;
				if (!*name) return false;
				++name;
			}
			break; // Not reached.
		default:
			if (strnicmp(name, pattern, 1)) return false;
			++name;
			++pattern;
			break;
		}
	}
}

intptr_t _findfirst64(const char *pFileName, __finddata64_t *pFindData)
{
#if defined(PS3) || defined(ORBIS)
	//path is still relative, convert to absolute path
	char buf[512];
	pFileName = ConvertFileName(buf, pFileName);
	//printf("_findfirst64:'%s' -> '%s'\n", pFileName, buf);
#endif
	FS_ERRNO_TYPE fsErr = 0;
	char filename[MAX_PATH];
	size_t filenameLength = 0;
	const char *dirname = 0;
	const char *pattern = 0;

	pFindData->m_LastIndex = -1;
	strcpy(filename, pFileName);
	filenameLength = strlen(filename);

	// Normalize ".*" and "*.*" suffixes to "*".
	if (!strcmp(filename + filenameLength - 3, "*.*"))
		filename[filenameLength - 2] = 0;
	else if (!strcmp(filename + filenameLength - 2, ".*"))
	{
		filename[filenameLength - 2] = '*';
		filename[filenameLength - 1] = 0;
	}

	// Map backslashes to fwdslashes.
#if !defined(PS3)
	const int cLen = strlen(pFileName);
	for (int i = 0; i<cLen; ++i)
		if (filename[i] == '\\') filename[i] = '/';
#endif
	// Get the dirname.
	char *slash = strrchr(filename, '/');
	if (slash)
	{
#if !defined(PS3) && !defined(ORBIS)
		if (FixDirnameCase(filename) == -1)
			return -1;
#endif
		pattern = slash + 1;
		dirname = filename;
		*slash = 0;
	} 
	else 
	{
		dirname = "./";
		pattern = filename;
	}
	strncpy(pFindData->m_ToMatch, pattern, sizeof pFindData->m_ToMatch - 1);

	// Close old directory descriptor.
	if (pFindData->m_Dir != FS_DIR_NULL)
	{
		FS_CLOSEDIR(pFindData->m_Dir, fsErr);
		pFindData->m_Dir = FS_DIR_NULL;
		if (fsErr)
			return -1;
	}

	bool readDirectory = true;
#if defined(USE_FILE_MAP)
#if defined(PS3)
	const bool skipInitial = true;
#else
	const bool skipInitial = false;
#endif
	// Check if an up to date directory listing can be extracted from the file
	// map.
	int dirIndex = -1;
	bool dirDirty = false;
	FileNode *dirNode = FileNode::FindExact(
			dirname, dirIndex, &dirDirty, skipInitial);
	if ((dirNode == NULL || dirIndex == -1) && !dirDirty)
		return -1;
	if (dirNode != NULL && !dirNode->m_bDirty)
	{
		FileNode *const dirNode1 = dirNode->m_Children[dirIndex];
		if (dirNode1 == NULL)
			return -1;
		if (!dirNode1->m_bDirty)
		{
			// Copy the directory listing from the file node.
			strncpy(pFindData->m_DirectoryName, dirname,
					sizeof pFindData->m_DirectoryName - 1);
			pFindData->m_DirectoryName[sizeof pFindData->m_DirectoryName - 1] = 0;
			const int n = dirNode1->m_nEntries;
			for (int i = 0; i < n; ++i)
			{
				if (dirNode1->m_Children[i])
				{
					// Directory. We'll add a trailing slash to the name to identify
					// directory entries.
					char name[strlen(dirNode1->m_Entries[i]) + 2];
					strcpy(name, dirNode1->m_Entries[i]);
					strcat(name, "/");
					pFindData->m_Entries.push_back(name);
				} else
					pFindData->m_Entries.push_back(dirNode1->m_Entries[i]);
			}
			readDirectory = false;
		}
	}
#endif

	if (readDirectory)
	{
		// Open and read directory.
 		FS_OPENDIR(dirname, pFindData->m_Dir, fsErr);


		if (fsErr)
			return -1;
		strncpy(pFindData->m_DirectoryName, dirname,
				sizeof pFindData->m_DirectoryName - 1);
		pFindData->m_DirectoryName[sizeof pFindData->m_DirectoryName - 1] = 0;
		FS_DIRENT_TYPE dirent;
		uint64_t direntSize = 0;
		while(true)
		{
			FS_READDIR(pFindData->m_Dir, dirent, direntSize, fsErr);
			if (fsErr)
			{
				FS_CLOSEDIR_NOERR(pFindData->m_Dir);
				pFindData->m_Dir	=	FS_DIR_NULL;
				return -1;
			}
			if (direntSize == 0)
				break;
			if (!strcmp(dirent.d_name, ".")
					|| !strcmp(dirent.d_name, "./")
					|| !strcmp(dirent.d_name, "..")
					|| !strcmp(dirent.d_name, "../"))
				continue;
			// We'll add a trailing slash to the name to identify directory
			// entries.
			char d_name[MAX_PATH];
			strcpy(d_name, dirent.d_name);
            // On Mac OS X Sometimes the \0 dilimeter does not get updated to the correct position.
			#if defined(CAFE)
			// not supported yet
			#elif !defined(LINUX)
			d_name[dirent.d_namlen] = '\0';
			#endif
			if (dirent.d_type == FS_TYPE_DIRECTORY)
			{
				const int cLen = strlen(d_name);
				if(d_name[0] || d_name[cLen - 1] != '/')
					strcat(d_name, "/");
			}
			pFindData->m_Entries.push_back(d_name);
		}
		FS_CLOSEDIR_NOERR(pFindData->m_Dir);
		pFindData->m_Dir	=	FS_DIR_NULL;
	}

	// Locate first match.
	int i = 0;
	const std::vector<string>::const_iterator cEnd = pFindData->m_Entries.end();
	for(std::vector<string>::const_iterator iter = pFindData->m_Entries.begin(); iter != cEnd; ++iter)
	{
		const char *cpEntry = iter->c_str();
		if (matchPattern(cpEntry, pattern))
		{
			pFindData->CopyFoundData(cpEntry);
			pFindData->m_LastIndex = i;
			break;
		}
		++i;
	}
	return pFindData->m_LastIndex;
};

int _findnext64(intptr_t last, __finddata64_t *pFindData)
{
	if (last == -1 || pFindData->m_LastIndex == -1)
		return -1;
	if (pFindData->m_LastIndex + 1 >= pFindData->m_Entries.size())
		return -1;

	int found = -1;
	int i = pFindData->m_LastIndex + 1;
	pFindData->m_LastIndex = -1;
	for (
		std::vector<string>::const_iterator iter = pFindData->m_Entries.begin() + i;
		iter != pFindData->m_Entries.end();
	  ++iter)
	{
		if (matchPattern(iter->c_str(), pFindData->m_ToMatch))
		{
			pFindData->CopyFoundData(iter->c_str());
			pFindData->m_LastIndex = i;
			found = 0;
			break;
		}
		++i;
	}
	return found;
}

//////////////////////////////////////////////////////////////////////////
int _findclose( intptr_t handle )
{
	return 0;//we dont need this
}

__finddata64_t::~__finddata64_t()
{
	if (m_Dir != FS_DIR_NULL)
	{
		FS_CLOSEDIR_NOERR(m_Dir);
		m_Dir = FS_DIR_NULL;
	}
}

//end--------------------------------findfirst/-next implementation----------------------------------------------------
#if !defined(PS3)
void adaptFilenameToLinux(string& rAdjustedFilename)
{
	//first replace all \\ by /
	string::size_type loc = 0;
	while((loc = rAdjustedFilename.find( "\\", loc)) != string::npos)
	{
		rAdjustedFilename.replace(loc, 1, "/");
	}	
	loc = 0;
	//remove /./
	while((loc = rAdjustedFilename.find( "/./", loc)) != string::npos)
	{
		rAdjustedFilename.replace(loc, 3, "/");
	}
}
#endif

#if !defined(PS3)
	void replaceDoublePathFilename(char *szFileName)
	{
		//replace "\.\" by "\"
		string s(szFileName);
		string::size_type loc = 0;
		//remove /./
		while((loc = s.find( "/./", loc)) != string::npos)
		{
			s.replace(loc, 3, "/");
		}
		loc = 0;
		//remove "\.\"
		while((loc = s.find( "\\.\\", loc)) != string::npos)
		{
			s.replace(loc, 3, "\\");
		}
		strcpy((char*)szFileName, s.c_str());
	}
#endif

#if !defined(PS3)
	const int comparePathNames(const char* cpFirst, const char* cpSecond, unsigned int len)
	{
		//create two strings and replace the \\ by / and /./ by /
		string first(cpFirst);
		string second(cpSecond);
		adaptFilenameToLinux(first);
		adaptFilenameToLinux(second);
		if(strlen(cpFirst) < len || strlen(cpSecond) < len)
			return -1;
		unsigned int length = std::min(std::min(first.size(), second.size()), (size_t)len);//make sure not to access invalid memory
		return memicmp(first.c_str(), second.c_str(), length);
	}
#else
	const int comparePathNames(const char* __restrict pFirst, const char* __restrict pSecond, unsigned int cLen)
	{
		//do ignore any . and slashes to save the /./ and \.\ replacement
		//skip disc prefix
		if(pFirst[0] == 'd' && pFirst[1] == ':')
		{
			cLen -= 2;
			pFirst += 2;
		}
		for(int i=0; i<cLen; ++i)
		{
			const char cSecond	= *pSecond;
			const char cFirst		= *pFirst;
			if(cSecond == 0)
				return 1;
			const int cIgnoreFirst = (cFirst == '/') | (cFirst == '.') | (cFirst == '\\');
			if(cIgnoreFirst)
			{
				++pFirst;
				continue;
			}
			const int cIgnoreSecond = (cSecond == '/') | (cSecond == '.') | (cSecond == '\\');
			if(cIgnoreSecond)
			{
				++pSecond;
				continue;
			}
			if(cFirst != cSecond)
				return -1;//do not care if 1 or -1
			++pFirst;
			++pSecond;
		}
		return 0;
	}
#endif

#if defined(LINUX) || defined(APPLE)
static bool FixOnePathElement(char *path)
{
	if (*path == '\0')
		return true;
		
	if ((path[0] == '/') && (path[1] == '\0'))
		return true;  // root dir always exists.
		
	if (strchr(path, '*') || strchr(path, '?'))
		return true; // wildcard...stop correcting path.
		
		struct stat statbuf;
		if (stat(path, &statbuf) != -1)  // current case exists.
			return true;
			
		char *name = path;
		char *ptr = strrchr(path, '/');
		if (ptr)
		{
			name = ptr+1;
			*ptr = '\0';
		}
		
		if (*name == '\0')  // trailing '/' ?
		{
			*ptr = '/';
			return true;
		}
		
		const char *parent;
		if (ptr == path)
			parent = "/";
		else if (ptr == NULL)
			parent = ".";
		else
			parent = path;
			
		DIR *dirp = opendir(parent);
		if (ptr) *ptr = '/';
		
		if (dirp == NULL)
			return false;
			
		struct dirent *dent;
		bool found = false;
		while ((dent = readdir(dirp)) != NULL)
		{
			if (strcasecmp(dent->d_name, name) == 0)
			{
				strcpy(name, dent->d_name);
				found = true;
				break;
			}
		}
		
		closedir(dirp);
		return found;
	}
#endif

#if !defined(PS3) && !defined(ORBIS)
const bool GetFilenameNoCase
(
	const char *file,
	char *pAdjustedFilename,
	const bool cCreateNew
)
{
	assert(file);
	assert(pAdjustedFilename);
	strcpy(pAdjustedFilename, file);
#ifndef PS3
	// Fix the dirname case.
	const int cLen = strlen(file);
	for (int i = 0; i<cLen; ++i) 
		if(pAdjustedFilename[i] == '\\') 
			pAdjustedFilename[i] = '/';
#endif
	char *slash;
	const char *dirname;
	char *name;
	FS_ERRNO_TYPE fsErr = 0;
	FS_DIRENT_TYPE dirent;
	uint64_t direntSize = 0;
	FS_DIR_TYPE fd = FS_DIR_NULL;

	if (
		(pAdjustedFilename) == (char*)-1)
		return false;

	slash = strrchr(pAdjustedFilename, '/');
	if (slash)
	{
		dirname = pAdjustedFilename;
		name = slash + 1;
		*slash = 0;
	} else
	{
	  dirname = ".";
		name = pAdjustedFilename;
	}

#if !defined(LINUX) && !defined(APPLE)		// fix the parent path anyhow.
	// Check for wildcards. We'll always return true if the specified filename is
	// a wildcard pattern.
	if (strchr(name, '*') || strchr(name, '?'))
	{
		if (slash) *slash = '/';
		return true;
	}
#endif

	// Scan for the file.
	bool found = false;
	bool skipScan = false;
#if defined(USE_FILE_MAP)
#if defined(PS3)
	const bool skipInitial = true;
#else
	const bool skipInitial = false;
#endif
	bool dirty = false;
	int dirIndex = -1;
	FileNode *dirNode = NULL;
	if (strrchr(dirname, '/') > dirname)
	{
		FileNode *parentDirNode = FileNode::FindExact(
				dirname, dirIndex, &dirty, skipInitial);
		if (dirty)
		{
			if (parentDirNode != NULL && dirIndex != -1)
				dirNode = parentDirNode->m_Children[dirIndex];
		} else
		{
			if (parentDirNode == NULL || dirIndex == -1)
				return false;
			dirNode = parentDirNode->m_Children[dirIndex];
			dirty = dirNode->m_bDirty;
		}
	}
	else
	{
		// The requested file is in the root directory.
		dirNode = FileNode::GetTree();
		if (dirNode != NULL)
			dirty = dirNode->m_bDirty;
		else
			dirty = true;
	}
	if (dirNode != NULL)
	{
		int index = dirNode->FindExact(name);
		if (index == -1)
		{
			index = dirNode->Find(name);
			if (index != -1)
			{
				strcpy(name, dirNode->m_Entries[index]);
				found = true;
			}
		} else
			found = true;
	}
	if (!dirty || found)
		skipScan = true;

	if (!skipScan)
	{
		FS_OPENDIR(dirname, fd, fsErr);
		if (fsErr)
			return false;
		while (true)
		{
			FS_READDIR(fd, dirent, direntSize, fsErr);
			if (fsErr)
			{
				FS_CLOSEDIR_NOERR(fd);
				return false;
			}
			if (direntSize == 0) break;
			if (!stricmp(dirent.d_name, name))
			{
				strcpy(name, dirent.d_name);
				found = true;
				break;
			}
		}
		FS_CLOSEDIR(fd, fsErr);
		if (fsErr)
			return false;
	}

	if (slash)
		*slash = '/';
	//if (!found && !cCreateNew) return false;
	return true;

#else //USE_FILE_MAP

	#if defined(LINUX) || defined(APPLE)
		if (slash) *slash = '/';
		char *path = pAdjustedFilename;
		char *sep;
		while ((sep = strchr(path, '/')) != NULL)
		{
			*sep = '\0';
			const bool exists = FixOnePathElement(pAdjustedFilename);
			*sep = '/';
			if (!exists)
				return false;
			path = sep + 1;
		}
	  return FixOnePathElement(pAdjustedFilename);  // catch last filename.
	#endif
	
#endif//USE_FILE_MAP
	return true;
}
#endif//PS3

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
HANDLE CreateFile(
													const char* lpFileName,
													DWORD dwDesiredAccess,
													DWORD dwShareMode,
													void* lpSecurityAttributes,
													DWORD dwCreationDisposition,
													DWORD dwFlagsAndAttributes,
													HANDLE hTemplateFile
												 )
{
	int flags = 0;
	int fd = -1;
	FS_ERRNO_TYPE fserr;
	bool create = false;
	HANDLE h;
#if !defined(PS3)
	if ((dwDesiredAccess & GENERIC_READ) == GENERIC_READ
			&& (dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE)
		flags = O_RDWR;
	else if ((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE)
		flags = O_WRONLY;
	else
	{
		// On Windows files can be opened with no access. We'll map no access
		// to read only.
		flags = O_RDONLY;
	}
	if ((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE)
#endif
	{
		switch (dwCreationDisposition)
		{
		case CREATE_ALWAYS:
			flags |= O_CREAT | O_TRUNC;
			create = true;
			break;
		case CREATE_NEW:
		  flags |= O_CREAT | O_EXCL;
			create = true;
			break;
		case OPEN_ALWAYS:
			flags |= O_CREAT;
			create = true;
			break;
		case OPEN_EXISTING:
			flags = O_RDONLY;
			break;
		case TRUNCATE_EXISTING:
			flags |= O_TRUNC;
			break;
		default:
			assert(0);
		}
	}

#if defined(PS3) || defined(ORBIS)
	char buf[512];
	const char *adjustedFilename = ConvertFileName(buf, lpFileName);
#else
	char adjustedFilename[MAX_PATH];
	GetFilenameNoCase(lpFileName, adjustedFilename, create);
#endif

	bool failOpen = false;
#if defined(USE_FILE_MAP)
#if defined(LINUX) || defined(APPLE)
	const bool skipInitial = false;
#else
	const bool skipInitial = true;
#endif
	if (!FileNode::CheckOpen(adjustedFilename, create, skipInitial))
		failOpen = true;
#endif
	(void)create;

#if defined(FILE_MAP_DEBUG)
	fd = open(adjustedFilename, flags, mode);
	if (fd != -1 && failOpen)
	{
		puts("FileNode::CheckOpen error");
		assert(0);
	}
#else
	if (failOpen)
	{
		fd = -1;
		fserr = ENOENT;
	} 
	else
	{
#if defined(USE_FILE_HANDLE_CACHE)
		OpenFromFileHandleCacheAsFD(adjustedFilename, flags, fd, 0, errno);
#else
		FS_OPEN(adjustedFilename, flags, fd, 0, errno);		
#endif
	}
#endif
	(void)fserr;
	if (fd == -1)
	{
		h = INVALID_HANDLE_VALUE;
	} else
	{
		h = (HANDLE)fd;
	}
	return h;
}

#define Int32x32To64(a, b) ((uint64)((uint64)(a)) * (uint64)((uint64)(b)))

#if defined(PS3)
	#ifndef MAX_FILE_HANDLE_INDEX
		#define MAX_FILE_HANDLE_INDEX 256
	#endif
#endif//PS3

/*
//////////////////////////////////////////////////////////////////////////
BOOL SetFileTime(
												HANDLE hFile,
												const FILETIME *lpCreationTime,
												const FILETIME *lpLastAccessTime,
												const FILETIME *lpLastWriteTime )
{
	CRY_ASSERT_MESSAGE(0, "SetFileTime not implemented yet");
	return FALSE;
}
*/
BOOL SetFileTime(const char* lpFileName, const FILETIME *lpLastAccessTime)
{
#if defined(PS3) || defined(ORBIS)
	char buf[512];
	const char* const adjustedFilename = ConvertFileName(buf, lpFileName);
#else
	// Craig: can someone get a better impl here?
	char adjustedFilename[MAX_PATH];
	GetFilenameNoCase(lpFileName, adjustedFilename, false);
#endif
#if defined(PS3)
	CellFsUtimbuf timeBuf;
	// the incoming time is in windows format(msecs since 1601)
	// also the set file functions seem to have some (undocumented?) requierments
	// that the time is a valid date, or else a default date(xx,xx,1979) is choosen
	// so we must compute the unix file time from the windows format
	uint64 winTime = *(uint64*)lpLastAccessTime;
	uint64 unixTime = (winTime - 116444736000000000ll) / 10000000;
	
	timeBuf.actime = *(time_t*)&unixTime;
	timeBuf.modtime = *(time_t*)&unixTime;
	const CellFsErrno fileErr = cellFsUtime(adjustedFilename, &timeBuf);//local host file
	if(fileErr != CELL_FS_SUCCEEDED)
	{
		CryFatalError("SetFileTime for \"%s\" failed with error code: 0x%08x\n",adjustedFilename,fileErr);
		return false;
	}
	return true;
#elif defined(ORBIS)
	ORBIS_TO_IMPLEMENT;
	return false;
#elif defined(LINUX) || defined(APPLE)
	struct utimbuf timeBuf;
	memset(&timeBuf, 0, sizeof timeBuf);
	timeBuf.actime = *(time_t *)lpLastAccessTime;
	timeBuf.modtime = timeBuf.actime;
	return utime(adjustedFilename, &timeBuf) == 0;
#endif
}

//returns modification time of file
//for testing we do just store the time as is without conversion to win32 system time
const uint64 GetFileModifTime(FILE * hFile)
{
	FS_ERRNO_TYPE fsErr = 0;
	FS_STAT_TYPE st;
	FS_FSTAT(fileno(hFile), st, fsErr);

	(void)fsErr;
	// UnixTimeToFileTime
	// since the cache filesystem uses fat32 and has a 2 seconds resolution time, and the
	// cellfs has a 1 seconds resolution timer, it is necessary to mask out the last second 
	time_t fixed_time = st.st_mtime & ~1ull;
	
	return Int32x32To64(fixed_time, 10000000) + 116444736000000000ll;
}

//////////////////////////////////////////////////////////////////////////
/*
BOOL GetFileTime(HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
{
	FS_ERRNO_TYPE err = 0;
	FS_STAT_TYPE st;
	int fd = (int)hFile;
	uint64 t;
	FILETIME creationTime, lastAccessTime, lastWriteTime;

	FS_FSTAT(fd, st, err);
	if (err != 0)
		return FALSE;
	t = st.st_ctime * 10000000UL + 116444736000000000ULL;
	creationTime.dwLowDateTime = (DWORD)t;
	creationTime.dwHighDateTime = (DWORD)(t >> 32);
	t = st.st_atime * 10000000UL + 116444736000000000ULL;
	lastAccessTime.dwLowDateTime = (DWORD)t;
	lastAccessTime.dwHighDateTime = (DWORD)(t >> 32);
	t = st.st_mtime * 10000000UL + 116444736000000000ULL;
	lastWriteTime.dwLowDateTime = (DWORD)t;
	lastWriteTime.dwHighDateTime = (DWORD)(t >> 32);
	if (lpCreationTime) *lpCreationTime = creationTime;
	if (lpLastAccessTime) *lpLastAccessTime = lastAccessTime;
	if (lpLastWriteTime) *lpLastWriteTime = lastWriteTime;
	return TRUE;
}
*/

//////////////////////////////////////////////////////////////////////////
#ifndef PS3
BOOL SetFileAttributes(
															LPCSTR lpFileName,
															DWORD dwFileAttributes )
{
//TODO: implement
	printf("SetFileAttributes not properly implemented yet, should not matter\n");
	return TRUE;
}
#endif

//////////////////////////////////////////////////////////////////////////
DWORD GetFileSize(HANDLE hFile,DWORD *lpFileSizeHigh )
{
	FS_ERRNO_TYPE err = 0;
	FS_STAT_TYPE st;
	int fd = (int)hFile;
	DWORD r;

	FS_FSTAT(fd, st, err);
	if (err != 0)
		return INVALID_FILE_SIZE;
	if (lpFileSizeHigh)
		*lpFileSizeHigh = (DWORD)(st.st_size >> 32);
	r = (DWORD)st.st_size;
	return r;
}

//////////////////////////////////////////////////////////////////////////
BOOL CloseHandle( HANDLE hObject )
{
	int fd = (int)hObject;

	if (fd != -1)
	{
#if defined(USE_FILE_HANDLE_CACHE)
		CloseFromFileHandleCacheAsFD(fd);
#else
		close(fd);
#endif
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL CancelIo( HANDLE hFile )
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "CancelIo not implemented yet");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
HRESULT GetOverlappedResult( HANDLE hFile,void* lpOverlapped,LPDWORD lpNumberOfBytesTransferred, BOOL bWait )
{
#if defined PS3
	// There is nothing like 'GetOverlappedResult()' on PS3, so we'll always
	// report that no data has been read so far.
	if (lpNumberOfBytesTransferred != NULL)
		*lpNumberOfBytesTransferred = 0;
	return 1;
#else
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "GetOverlappedResult not implemented yet");
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
BOOL ReadFile
(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
)
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "ReadFile not implemented yet");
	abort();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL ReadFileEx
(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
#if defined(PS3)
	CellFsAio *const pAio = lpOverlapped;
#ifdef ENABLE_FSAIO
	int id = 0;
	pAio->fd = static_cast<int>(hFile.Handle());
	if(pAio->fd == -1)
		return FALSE;
	pAio->buf = lpBuffer;
	pAio->size = static_cast<uint64_t>(nNumberOfBytesToRead);
	CellFsErrno err = cellFsAioRead(pAio, &id, lpCompletionRoutine);
	pAio->user_data = (uint64_t)id;//store id to be able cancel io later on
	return err == CELL_OK;
#else
	uint64 read = 0;
	uint64 pos = 0;
	int err = -1;
	int h = static_cast<int>(hFile.Handle());
	pAio->fd = h;
	if(h >= 0)
	{
		err = cellFsLseek(h, pAio->offset, SEEK_SET, &pos);
		if(err == CELL_OK)
			err = cellFsRead(h, lpBuffer, nNumberOfBytesToRead, &read);
	}
	lpCompletionRoutine(pAio,err, 0, read);
	return err==CELL_OK;
#endif//ENABLE_FSAIO
#else
	CRY_ASSERT_MESSAGE(0, "ReadFileEx not implemented yet");
	return TRUE;
#endif
}

//////////////////////////////////////////////////////////////////////////
DWORD SetFilePointer
(
	HANDLE hFile,
	LONG lDistanceToMove,
	PLONG lpDistanceToMoveHigh,
	DWORD dwMoveMethod
)
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "SetFilePointer not implemented yet");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
#if !defined(PS3) && !defined(ORBIS)
threadID GetCurrentThreadId()
{
  return threadID(pthread_self());
}
#endif

//////////////////////////////////////////////////////////////////////////
HANDLE CreateEvent
(
	LPSECURITY_ATTRIBUTES lpEventAttributes,
	BOOL bManualReset,
	BOOL bInitialState,
	LPCSTR lpName
)
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "CreateEvent not implemented yet");
	return 0;
}


//////////////////////////////////////////////////////////////////////////
#ifndef PS3
DWORD Sleep(DWORD dwMilliseconds)
{
#if defined(LINUX) || defined(APPLE)
	timespec req;
	timespec rem;

	memset(&req, 0, sizeof(req));
	memset(&rem, 0, sizeof(rem));

	time_t sec = (int)(dwMilliseconds/1000);
	req.tv_sec = sec;
	req.tv_nsec = (dwMilliseconds - (sec*1000))*1000000L;
	if (nanosleep(&req, &rem) == -1)
		nanosleep(&rem, 0);

	return 0;
#elif ORBIS
	sceKernelUsleep(dwMilliseconds*1000);
	return 0;
#else
	timeval tv, start, now;
	uint64 tStart;

	memset(&tv, 0, sizeof tv);
	memset(&start, 0, sizeof start);
	memset(&now, 0, sizeof now);
	gettimeofday(&now, NULL);
	start = now;
	tStart = (uint64)start.tv_sec * 1000000 + start.tv_usec;
	while (true)
	{
		uint64 tNow, timePassed, timeRemaining;
		tNow = (uint64)now.tv_sec * 1000000 + now.tv_usec;
		timePassed = tNow - tStart;
		if (timePassed >= dwMilliseconds)
			break;
		timeRemaining = dwMilliseconds * 1000 - timePassed;
		tv.tv_sec = timeRemaining / 1000000;
		tv.tv_usec = timeRemaining % 1000000;
		select(1, NULL, NULL, NULL, &tv);
		gettimeofday(&now, NULL);
	}
	return 0;
#endif
}
#endif

//////////////////////////////////////////////////////////////////////////
DWORD SleepEx( DWORD dwMilliseconds,BOOL bAlertable )
{
//TODO: implement
//	CRY_ASSERT_MESSAGE(0, "SleepEx not implemented yet");
	printf("SleepEx not properly implemented yet\n");
	Sleep(dwMilliseconds);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
DWORD WaitForSingleObjectEx(HANDLE hHandle,	DWORD dwMilliseconds,	BOOL bAlertable)
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "WaitForSingleObjectEx not implemented yet");
	return 0;
}















//////////////////////////////////////////////////////////////////////////
DWORD WaitForSingleObject( HANDLE hHandle,DWORD dwMilliseconds )
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "WaitForSingleObject not implemented yet");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
BOOL SetEvent( HANDLE hEvent )
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "SetEvent not implemented yet");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL ResetEvent( HANDLE hEvent )
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "ResetEvent not implemented yet");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
HANDLE CreateMutex
(
	LPSECURITY_ATTRIBUTES lpMutexAttributes,
	BOOL bInitialOwner,
	LPCSTR lpName
)
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "CreateMutex not implemented yet");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
BOOL ReleaseMutex( HANDLE hMutex )
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "ReleaseMutex not implemented yet");
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////


typedef DWORD (*PTHREAD_START_ROUTINE)( LPVOID lpThreadParameter );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

//////////////////////////////////////////////////////////////////////////
HANDLE CreateThread
(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId
)
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "CreateThread not implemented yet");
	return 0;
}

//////////////////////////////////////////////////////////////////////////








































































//////////////////////////////////////////////////////////////////////////
DWORD GetCurrentDirectory( DWORD nBufferLength, char* lpBuffer )
{
#if defined(PS3_CRYENGINE)
	char *p = fopenwrapper_basedir;
	size_t len;

	if (*p != '/')
		WrappedF_InitCWD();
	assert(*p == '/');

	if (!*p)
	{
		// Yes, we'll return 2 if the buffer is too small and 1 otherwise.
		// Yes, this is stupid, but that's what Microsoft says...
		if (nBufferLength < 2) return 2;
		lpBuffer[0] = '\\';
		lpBuffer[1] = 0;
		return 1;
	}
	len = strlen(p) + 1;
	if (nBufferLength < len) 
		return len;
	strcpy(lpBuffer, p);
#if !defined(PS3)
	for(int i=0; i<len; ++i)
	{
		if (*p == '/') *p = '\\';
		++p;
	}
#endif
	return len - 1;










#else
	// Not supported outside of CryEngine.
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
BOOL DeleteFile(LPCSTR lpFileName)
{
#if defined(PS3)
	char buf[512];
	const char *const cpConvFileName = ConvertFileName(buf, lpFileName);
	return((cellFsUnlink(cpConvFileName) == CELL_FS_SUCCEEDED)?0 : -1);
#elif defined(LINUX) || defined(APPLE)
	int err = unlink(lpFileName);
	return (0 == err);
#else
	CRY_ASSERT_MESSAGE(0, "DeleteFile not implemented yet");
	return TRUE;
#endif
}

//////////////////////////////////////////////////////////////////////////
BOOL MoveFile( LPCSTR lpExistingFileName,LPCSTR lpNewFileName )
{
#if defined(LINUX) || defined(APPLE)
	int err = rename(lpExistingFileName, lpNewFileName);
	return (0 == err);
#else
	CRY_ASSERT_MESSAGE(0, "MoveFile not implemented yet");
#endif
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL CopyFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
{
#if defined(LINUX) || defined(APPLE)
    if (bFailIfExists)
    {
		// return false if file already exists
		FILE* fTemp = fopen(lpNewFileName, "rb");
		if (fTemp != NULL)
		{
	    	fclose(fTemp);
	    	return FALSE;
		}
    }
    
    FILE* fFrom = fopen(lpExistingFileName, "rb");
    if (NULL == fFrom)
		return FALSE;
	
    FILE* fTo = fopen(lpNewFileName, "wb");
    if (NULL == fTo)
    {
		fclose(fFrom);
		return FALSE;
    }
    
    #define COPY_FILE_BUF_SIZE 1024
    char buf[COPY_FILE_BUF_SIZE];
    size_t lenRead;
    size_t lenWrite;
        
    while (!feof(fFrom))
    {
		lenRead = fread(buf, sizeof(char), COPY_FILE_BUF_SIZE, fFrom);
		lenWrite = fwrite(buf, sizeof(char), lenRead, fTo);
		assert(lenWrite == lenRead);
    }
    
    fclose(fFrom);
    fclose(fTo);
    
    return TRUE;
	    
#else
    CRY_ASSERT_MESSAGE(0, "CopyFile not implemented yet");
		return FALSE;
#endif
}

#if defined(LINUX) || defined(APPLE)
BOOL GetComputerName(LPSTR lpBuffer, LPDWORD lpnSize)
{
	if (!lpBuffer || !lpnSize)
		return FALSE;

 	int err = gethostname(lpBuffer, *lpnSize);

	if (-1 == err)
	{
		CryLog("GetComputerName falied [%d]\n", errno);
		return FALSE;
	}
	return TRUE;
}

DWORD GetCurrentProcessId(void)
{
	return (DWORD)getpid();
}
#endif //LINUX APPLE

//////////////////////////////////////////////////////////////////////////
BOOL RemoveDirectory(LPCSTR lpPathName)
{
#if defined(PS3)
	char buf[512];
	const char *const cpConvFileName = ConvertFileName(buf, lpPathName);
	return((cellFsRmdir(cpConvFileName) == CELL_FS_SUCCEEDED)?0 : -1);
#else//PS3
	CRY_ASSERT_MESSAGE(0, "RemoveDirectory not implemented yet");
	return TRUE;
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void CrySleep( unsigned int dwMilliseconds )
{
	Sleep( dwMilliseconds );
}

//////////////////////////////////////////////////////////////////////////
void CryLowLatencySleep( unsigned int dwMilliseconds )
{
#if defined(DURANGO)
	if(dwMilliseconds>32) // just do an OS sleep for long periods, because we just assume that if we sleep for this long, we don't need a accurate latency (max diff is likly 15ms)
		Sleep( dwMilliseconds );
	else // do a more accurate "sleep" by yielding this CPU to other threads
	{
		LARGE_INTEGER frequency;
		LARGE_INTEGER currentTime;
		LARGE_INTEGER endTime;

		QueryPerformanceCounter(&endTime);

		// Ticks in microseconds (1/1000 ms)
		QueryPerformanceFrequency(&frequency);
		endTime.QuadPart += (dwMilliseconds * frequency.QuadPart) / (1000ULL);

		do
		{
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();
			SwitchToThread();

			QueryPerformanceCounter(&currentTime);
		} while (currentTime.QuadPart < endTime.QuadPart);
	}
#else
	CrySleep(dwMilliseconds);
#endif
}

//////////////////////////////////////////////////////////////////////////
#if defined(PS3)
void sys_timer_usleep_nops( unsigned int dwMicroSeconds )
{
	//do loops with nops, 16 nops per iteration
	const int cIterations = dwMicroSeconds * 100;
	for(int i=0; i<cIterations; ++i)
	{
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
int CryMessageBox( const char *lpText,const char *lpCaption,unsigned int uType)
{
#ifdef WIN32
	return MessageBox( NULL,lpText,lpCaption,uType );
#elif defined(APPLE)
	CFStringRef strText = CFStringCreateWithCString(NULL, lpText, strlen(lpText));
	CFStringRef strCaption = CFStringCreateWithCString(NULL, lpCaption, strlen(lpCaption));
		
	CFOptionFlags kResult;
	CFUserNotificationDisplayAlert(
		0, // no timeout
		kCFUserNotificationNoteAlertLevel, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
		NULL, //icon url, use default, you can change it depending message_type flags
		NULL, //not used
		NULL, //localization of strings
		strText, //header text
		strCaption, //message text
		NULL, //default "ok" text in button
		CFSTR("Cancel"), //alternate button title
		NULL, //other button title, null--> no other button
		&kResult //response flags
		);
	
	CFRelease(strCaption);
	CFRelease(strText);
		
	if (kResult == kCFUserNotificationDefaultResponse)
		return 1;	// IDOK on Win32
	else
		return 2;	// IDCANCEL on Win32
#else
	printf("Messagebox: cap: %s  text:%s\n",lpCaption?lpCaption:" ",lpText?lpText:" ");
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
int CryCreateDirectory( const char *lpPathName,void *lpSecurityAttributes )
{
#if defined(LINUX) || defined(APPLE)
	int res = mkdir(lpPathName, S_IRWXU);
#else
	int res = _mkdir(lpPathName);
#endif
	if (res != 0)
	{
		SetLastError( ERROR_PATH_NOT_FOUND );
		return -1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int CryGetCurrentDirectory( unsigned int nBufferLength,char *lpBuffer )
{
	return GetCurrentDirectory(nBufferLength,lpBuffer);
}

#if !defined(PS3) || !defined(_LIB)
//////////////////////////////////////////////////////////////////////////
short CryGetAsyncKeyState( int vKey )
{
//TODO: implement
	CRY_ASSERT_MESSAGE(0, "CryGetAsyncKeyState not implemented yet");
	return 0;
}
#endif

#if defined(LINUX) || defined(APPLE)
//[K01]: http://www.memoryhole.net/kyle/2007/05/atomic_incrementing.html
//http://forums.devx.com/archive/index.php/t-160558.html
//////////////////////////////////////////////////////////////////////////
DLL_EXPORT LONG  CryInterlockedIncrement( LONG volatile *lpAddend )
{
	/*int r;
	__asm__ __volatile__ (
		"lock ; xaddl %0, (%1) \n\t"
		: "=r" (r)
		: "r" (lpAddend), "0" (1)
		: "memory"
	);
	return (LONG) (r + 1); */ // add, since we get the original value back.
    return __sync_fetch_and_add(lpAddend,1) + 1;
}

//////////////////////////////////////////////////////////////////////////
DLL_EXPORT LONG  CryInterlockedDecrement( LONG volatile *lpAddend )
{
	/*int r;
	__asm__ __volatile__ (
		"lock ; xaddl %0, (%1) \n\t"
		: "=r" (r)
		: "r" (lpAddend), "0" (-1)
		: "memory"
	);
	return (LONG) (r - 1);  */// subtract, since we get the original value back.
    return __sync_fetch_and_sub(lpAddend,1) - 1;
}

//////////////////////////////////////////////////////////////////////////
DLL_EXPORT LONG 	 CryInterlockedExchangeAdd(LONG  volatile * lpAddend, LONG  Value)
{
/*	LONG r;
	__asm__ __volatile__ (
	#if defined(LINUX64) || defined(APPLE)  // long is 64 bits on amd64.
		"lock ; xaddq %0, (%1) \n\t"
	#else
		"lock ; xaddl %0, (%1) \n\t"
	#endif
		: "=r" (r)
		: "r" (lpAddend), "0" (Value)
		: "memory"
	);
	return r;*/
    return __sync_fetch_and_add(lpAddend, Value);
}

DLL_EXPORT LONG 	CryInterlockedCompareExchange(LONG  volatile * dst, LONG  exchange, LONG comperand)
{
    return __sync_val_compare_and_swap(dst,comperand,exchange);
	/*LONG r;
	__asm__ __volatile__ (
	#if defined(LINUX64) || defined(APPLE)  // long is 64 bits on amd64.
		"lock ; cmpxchgq %2, (%1) \n\t"
	#else
		"lock ; cmpxchgl %2, (%1) \n\t"
	#endif
		: "=a" (r)
		: "r" (dst), "r" (exchange), "0" (comperand)
		: "memory"
	);
	return r;*/
}


DLL_EXPORT void*	 CryInterlockedCompareExchangePointer(void* volatile * dst, void* exchange, void* comperand)
{
    return __sync_val_compare_and_swap(dst,comperand,exchange);
	//return (void*)CryInterlockedCompareExchange((long volatile*)dst, (long)exchange, (long)comperand);
}

#if defined(LINUX64) || defined(APPLE)
DLL_EXPORT unsigned char _InterlockedCompareExchange128( int64 volatile *dst, int64 exchangehigh, int64 exchangelow, int64* comperand )
{
    bool bEquals;
    __asm__ __volatile__
    (
        "lock cmpxchg16b %1\n\t"
        "setz %0"
        : "=q" (bEquals), "+m" (*dst), "+d" (comperand[1]), "+a" (comperand[0])
        : "c" (exchangehigh), "b" (exchangelow)
        : "cc"
    );
    return (char)bEquals;
}
#endif

threadID CryGetCurrentThreadId()
{
  return GetCurrentThreadId();
}

void CryDebugBreak()
{
	__builtin_trap();
}
#endif//LINUX APPLE

#ifdef PS3
void CryDebugBreak()
{
	__debugbreak();
}
#endif //PS3

//////////////////////////////////////////////////////////////////////////
uint32 CryGetFileAttributes(const char *lpFileName)
{
#if defined(PS3) || defined(ORBIS)
	char buf[512];
	const char* buffer = ConvertFileName(buf, lpFileName);
#elif defined(LINUX) || defined(APPLE)
	string fn = lpFileName;
	adaptFilenameToLinux(fn);
	const char* buffer = fn.c_str();
#else
	const char* buffer = lpFileName;
#endif
	//test if it is a file, a directory or does not exist
	FS_ERRNO_TYPE fsErr = 0;
	int fd = -1;
	FS_OPEN(buffer, FS_O_RDONLY, fd, 0, fsErr);
	if(!fsErr)
  {
    FS_CLOSE_NOERR(fd);
		return FILE_ATTRIBUTE_NORMAL;
  }
	return (fsErr == FS_EISDIR)? FILE_ATTRIBUTE_DIRECTORY : INVALID_FILE_ATTRIBUTES;
}

#if !defined(PS3)
//////////////////////////////////////////////////////////////////////////
bool CrySetFileAttributes( const char *lpFileName,uint32 dwFileAttributes )
{
//TODO: implement
	printf("CrySetFileAttributes not properly implemented yet\n");
	return false;
}
#endif

#if defined(PS3)
//////////////////////////////////////////////////////////////////////////
char *_fullpath(char * absPath, const char* relPath, size_t size)
{
	char szExeFileName[_MAX_PATH];
	// Get the path of the executable
	GetCurrentDirectory(sizeof(szExeFileName), szExeFileName);
	char * __restrict pCurDst = absPath;
	const char * __restrict pCurSrc = szExeFileName;
	while(*pCurSrc)
		*pCurDst++ = *pCurSrc++;
	pCurSrc = relPath;
	while(*pCurSrc)
		*pCurDst++ = *pCurSrc++;
	*pCurDst = '\0';
	return absPath;
}
#endif

#if !defined(PS3)
	int file_counter = 0;
	long file_op_counter = 0;
	long file_op_break = -1;
#endif

static void WrappedF_Break(long op_counter)
{
	printf("WrappedF_Break(op_counter = %li)\n", op_counter);
}

#if defined(PS3)

#if defined PS3_CRYENGINE
typedef std::map<pthread_t, const char*> TThreadMap;
static TThreadMap g_ThreadNameMap;
static std::vector<uint32> g_ThreadIndexVec;
static CryLockT<CRYLOCK_RECURSIVE> g_ThreadIndexLock;

// fp exceptions
inline int EnableFPException(sys_ppu_thread_t id)
{
#ifdef SUPP_FP_EXC
	int ret;
	ret = feclearexcept(FE_DIVBYZERO | FE_INVALID);
	if (ret != 0)
		return ret;
	ret = sys_dbg_enable_floating_point_enabled_exception(id, 0, 0, 0);
	if (ret != CELL_OK)
		return ret;
	ret = fesettrapenable(FE_DIVBYZERO | FE_INVALID);
	if (ret != 0)
		return ret;
#endif
	return CELL_OK;
}

void RegisterThreadName(pthread_t id, const char* name)
{
	g_ThreadIndexLock.Lock();
	ScopedSwitchToGlobalHeap GlobalHeap;
	if(name)
		g_ThreadNameMap.insert(std::pair<pthread_t, const char*>(id, name));	
	g_ThreadIndexVec.push_back((uint32)id);	
	if(g_EnableFP)
		EnableFPException((sys_ppu_thread_t)id);
	g_ThreadIndexLock.Unlock();
};

uint32 MapThreadIDToIndex(uint32 threadId)
{
	g_ThreadIndexLock.Lock();
	uint32 index=0;
	const uint32 cEntryCnt = g_ThreadIndexVec.size();
	for(; index<cEntryCnt; ++index)
	{
		if(g_ThreadIndexVec[index] == threadId)
			break;
	}
	g_ThreadIndexLock.Unlock();
	return index;
}

void UnRegisterThreadName(pthread_t id)
{
	g_ThreadIndexLock.Lock();
	TThreadMap::iterator it = g_ThreadNameMap.find(id);
	if(it != g_ThreadNameMap.end())
		g_ThreadNameMap.erase(it);
	const std::vector<uint32>::iterator cThreadEnd = g_ThreadIndexVec.end();
	for(std::vector<uint32>::iterator it=g_ThreadIndexVec.begin();it!=cThreadEnd; ++it)
	{
		if(*it == (uint32)id)
		{
			g_ThreadIndexVec.erase(it);
			return;
		}
	}
	g_ThreadIndexLock.Unlock();
};

const char* GetRegisteredThreadName(uint32 id)
{
	g_ThreadIndexLock.Lock();
	TThreadMap::iterator it = g_ThreadNameMap.find((pthread_t)id);
	const char* retVal = NULL;
	if(it != g_ThreadNameMap.end())
		retVal = it->second;
	g_ThreadIndexLock.Unlock();
	return retVal;
}

inline void DisableFPException(sys_ppu_thread_t id)
{
#ifdef SUPP_FP_EXC
	sys_dbg_disable_floating_point_enabled_exception(id, 0, 0, 0);
	fesettrapenable(0);
#endif
}

void DisableFPExceptions()
{
#ifdef SUPP_FP_EXC
	sys_ppu_thread_t id;
	int ret = sys_ppu_thread_get_id(&id);
	if(ret >= 0)
		DisableFPException(id);
	g_EnableFP = false;
	const std::vector<uint32>::iterator cThreadEnd = g_ThreadIndexVec.end();
	for(std::vector<uint32>::iterator it=g_ThreadIndexVec.begin();it!=cThreadEnd; ++it)
		DisableFPException(*it);
#endif
}

void EnableFPExceptions()
{
#ifdef SUPP_FP_EXC
	sys_ppu_thread_t id;
	if(CELL_OK == sys_ppu_thread_get_id(&id))
		EnableFPException(id);
	g_EnableFP = true;
	const std::vector<uint32>::iterator cThreadEnd = g_ThreadIndexVec.end();
	for(std::vector<uint32>::iterator it=g_ThreadIndexVec.begin();it!=cThreadEnd; ++it)
		EnableFPException(*it);
#endif
}
#endif
#endif // defined(PS3)

#if defined(PS3) || defined(ORBIS)
// undef the define overloads
#undef fopen
#undef stat
#undef fclose
#undef fseek
#undef ftell
#undef fread
#undef fileno
#undef fwrite
#undef vfprintf
#undef fgets
#undef feof
#undef ferror
#undef getc
#undef ungetc
#undef fputs
#undef fflush
#undef fprintf
#undef fscanf
#undef vfscanf
#undef fstat
#undef fgetc


// LRU cache for filehandles
// The PS3 only allows up to 31 files opened on gamedata
// thus we cache them, reuse older filedescriptors, and close
// files in case we ran out of file descriptors
// to prevent problems with streaming/direct file access, these get their LRU
// value modified, so that they have a higher priority
class CFileHandleCache
{
	enum { nFILEHANDLESALT = 0xFF000000 };	// special salt to build FILE* look alikes
#ifndef PS3
	enum { nFILEENTRIES = 256 };						// number of supported entries in the cache
#if defined(ORBIS)
	enum { nMAXOPENFILES = FOPEN_MAX - 13 };	// maximum of simultanius opened files (minus 3 as includes stdout, stderr and stdin minus 10 for miscellaneous other files the OS seems to have open)
#else
	enum { nMAXOPENFILES = FOPEN_MAX - 3 };	// maximum of simultanius opened files (minus 3 as includes stdout, stderr and stdin)
#endif
#else
	enum { nFILEENTRIES = 64 };							// number of supported entries in the cache
	enum { nMAXOPENFILES = 26 };						// maximum of simultanius opened files
#endif
	enum { nFILEIOBUFFERSIZE = 32 *1024 };		// size of the buffer used for file io

public:

	static CFileHandleCache& Instance()
	{
		static CFileHandleCache singleton;
		return singleton;
	}

	~CFileHandleCache() {}

	// wrapped std file interface
	inline FILE* FOpen(const char* name, const char *mode, FileIoWrapper::FileAccessType type = FileIoWrapper::GENERAL);
	inline int FClose(FILE* pHandle );	
	inline size_t Fileno(FILE *pHandle );
	inline size_t FRead( void *ptr, size_t size, size_t count, FILE *pHandle);
	inline size_t FWrite( const void * ptr, size_t size, size_t count, FILE * stream );
	inline size_t FTell(FILE* pHandle);
	inline size_t FSeek(FILE* pHandle, size_t offset, size_t origin);
	inline int VFprintf ( FILE * stream, const char * format, va_list arg );
	inline char* FGets ( char * str, int num, FILE * stream );
	inline int Feof ( FILE * stream );
	inline int Ferror ( FILE * stream );
	inline int Getc ( FILE * stream );
	inline int Ungetc ( int character, FILE * stream );
	inline int Fputs( const char * str, FILE * stream );
	inline int Fflush(FILE *stream );
	inline int VFscanf( FILE * stream, const char * format, va_list arg );
	inline int Fgetc( FILE * stream );

	// wrapper utils for cellfs functions(they work on fds)
	void OpenFromFileHandleCacheAsFD( const char *adjustedFilename, int flags, int &fd, int &err);
	void CloseFromFileHandleCacheAsFD( int fd );

	// util functions
	void DumpCacheContent();
private:
	CFileHandleCache();

	void TestDiscEjection(FILE *pHandle) 
	{
#ifdef PS3
		if( errno != 0 )
		{
			//For robustness, follow the documented procedure described in "libfs Overview - Handling Disc Ejections"
			FILE *pRealHandle = GetRealFileHandle(pHandle);
			if(pRealHandle)
			{
				int fileNumber = ::fileno(pRealHandle);
				CellFsStat sb;
				CellFsErrno statError = cellFsFstat(fileNumber, &sb);
				if( statError == CELL_FS_EBADF )
				{
					SetDiscIsEjectedFlag(pHandle);
				}
			}
		}
#endif
	}

	void SetDiscIsEjectedFlag(FILE *pHandle )
	{
		uint32 nCachePos = GetCachePos(pHandle);
		SCacheEntry& rEntry = m_Cache[nCachePos];
		rEntry.m_bDiscIsEjected = true;
	}

	// md5 key for fast search of open files
	typedef union
	{
		unsigned char c16[16];		
		uint64 u64[2];
	} TPakNameKey;								

	// entry holding all file handle informations per file for the cache
	struct SCacheEntry
	{
		enum States { Open,					// file handle is open and in use
									Cached,				// file not in use, but file descriptor still open for re-use
									Parked,				// file in use, but file descriptor closed
									Closed  };		// File and file descriptor closed

		// members for LRU-cache
		uint32 m_lru;									//lru counter last set op
		TPakNameKey m_pakNameKey;			//md5 of file name (full path)

		// FILE informations
		FILE* m_pHandle;							//cached pak file handle
		char m_filename[MAX_PATH];		// file-name, in case the file needs to be re-opened
		char m_mode[16];							// access mode used for file io
		uint32 m_nSeekPos;						// store seek position, in case files needs to be reopened
		States m_state;								// state of the cache entrie
		FileIoWrapper::FileAccessType m_type;	// type of access to this file
		char* m_fileIoBuf;						// the allocated file io buffer, only valid when Open or Cached
		bool m_bDiscIsEjected;				// remember per file if the disc was ejected

		SCacheEntry();
	};

	uint32 GetCachePos(FILE*pHandle) const				{ return (uint32)((TRUNCATE_PTR)pHandle) & ~nFILEHANDLESALT; }
	FILE* GetFileHandle(uint32 nCachePos ) const	{ return (FILE*)(EXPAND_PTR)((uint32)nCachePos | nFILEHANDLESALT); }

	FILE* GetRealFileHandle(FILE* pHandle); 
	
	inline void CloseFile(uint32 nCachePos );
	inline FILE* OpenFile(uint32 nCachePos, const char *sName, const char*op, FileIoWrapper::FileAccessType type);
	inline void ReOpenFile(uint32 nCachePos);

	void ActivateEntry( uint32 nCachePos );
	void FreeUpFD( uint32 nCachePosKeep );

	uint32 FindOpenSlot();

	char* AllocateFileIoBuffer() { char *buf = m_FileIoBufferPtrs.back(); m_FileIoBufferPtrs.pop_back(); return buf; }
	void FreeFileIoBuffer(char *buf) { m_FileIoBufferPtrs.push_back(buf); }

	typedef std::map< int, FILE*> FDMap;
	
	uint32 m_LRU;														//current lru counter to replace slots properly
	uint32 m_nFileCount;										// number of open files	
	FDMap	m_FdToFileMap;										// map to find FILE* from fd used by cellfsio
	CryCriticalSection m_Lock;							// Lock to protected the cache against parallel access
	CryCriticalSection m_mapLock;						// Lock to protect the FILE*->fd map from parallel access		
	std::vector<char*> m_FileIoBufferPtrs;	// nMAXOPENFILES pointers into m_FileIoBuffer, wrapped in a vector for convenience

	SCacheEntry m_Cache[nFILEENTRIES];			//file handle cache
	char m_FileIoBuffer[nMAXOPENFILES][nFILEIOBUFFERSIZE]; // static buffer for all file ios
};

CFileHandleCache::CFileHandleCache() :
m_LRU(0),
m_nFileCount(0)
{
	m_FileIoBufferPtrs.reserve(nMAXOPENFILES);

	for (uint32 i=0; i<nMAXOPENFILES; i++)
	{
		m_FileIoBufferPtrs.push_back(&m_FileIoBuffer[i][0]);
	}
}

CFileHandleCache::SCacheEntry::SCacheEntry() : 
m_pHandle(NULL),
m_lru(0),
m_nSeekPos(0),
m_state(Closed),
m_type(FileIoWrapper::GENERAL),
m_fileIoBuf(NULL)
{
	m_pakNameKey.u64[0] = 0;
	m_pakNameKey.u64[1] = 0;	
	m_filename[0] = '\0';
	m_mode[0] = '\0';
}

//retrieve cache one if existing, otherwise open new one
FILE* CFileHandleCache::FOpen(const char *name, const char *mode, FileIoWrapper::FileAccessType type)
{	
	AUTO_LOCK_T(CryCriticalSection, m_Lock);
	// create md5 hash as a search key
	TPakNameKey	key;
	MD5Context context;
	MD5Init(&context);
	MD5Update(&context, (unsigned char*)name, strlen(name) );
	MD5Final(key.c16, &context);

	// check for a matching closed file
	for( uint32 nCachePos = 0 ;nCachePos<nFILEENTRIES;++nCachePos)
	{		
		SCacheEntry& rEntry = m_Cache[nCachePos];
		
		// try to find a cached handle for this file
		if( rEntry.m_state == SCacheEntry::Cached &&
			rEntry.m_pakNameKey.u64[0] == key.u64[0] && rEntry.m_pakNameKey.u64[1] == key.u64[1])
		{			
			// if different mode then, close it first. We never want 2 files open with different modes
			if (strcmp(rEntry.m_mode, mode) == 0)
			{
				ActivateEntry(nCachePos);			
				return GetFileHandle(nCachePos);
			} else {
				CloseFile(nCachePos);
				break;
			}
		}
		
	}
	uint32 nOpenSlot = FindOpenSlot();
	return OpenFile(nOpenSlot, name, mode, type);
}

uint32 CFileHandleCache::FindOpenSlot()
{
	uint32 curLowestCachedSlot		= 0xFFFFFFFF; //invalid slot id
	uint32 curLowestCachedSlotLRU = 0xFFFFFFFF;
	uint32 curLowestClosedSlot		= 0xFFFFFFFF; // invalid slot id
	uint32 curLowestClosedSlotLRU = 0xFFFFFFFF;


	// find slot where we could close an fd
	for( uint32 nCachePos = 0;nCachePos<nFILEENTRIES;++nCachePos)
	{			
		SCacheEntry& rEntry = m_Cache[nCachePos];

		if( rEntry.m_state == SCacheEntry::Cached && rEntry.m_lru < curLowestCachedSlotLRU )
		{
			curLowestCachedSlotLRU = rEntry.m_lru;
			curLowestCachedSlot = nCachePos;
		}

		if( rEntry.m_state == SCacheEntry::Closed && rEntry.m_lru < curLowestClosedSlotLRU )
		{
			curLowestClosedSlotLRU = rEntry.m_lru;
			curLowestClosedSlot = nCachePos;
		}
	}

	if(curLowestClosedSlot != 0xFFFFFFFF ) return curLowestClosedSlot;
	if( curLowestCachedSlot != 0xFFFFFFFF ) return curLowestCachedSlot;

	gEnv->pLog->LogError("[FileHandleCache] Could not find a slot in state Cache or Closed to reused");
	DumpCacheContent();
	__debugbreak();
	return -1;
}

int CFileHandleCache::FClose(FILE* pHandle)
{	
	AUTO_LOCK_T(CryCriticalSection, m_Lock);
	uint32 nCachePos = GetCachePos(pHandle);

	SCacheEntry& rEntry = m_Cache[nCachePos];
	switch(rEntry.m_state )
	{
	case SCacheEntry::Open:
		rEntry.m_state = SCacheEntry::Cached;
		if (strchr(rEntry.m_mode, 'w') == NULL)
		{
#ifdef ORBIS_USE_NATIVE_FILE_IO
			sceKernelFsync(ConvertFileToHandle(rEntry.m_pHandle));
#else
			::fflush(rEntry.m_pHandle);		
#endif
		} else {
			// fully close the file if it was opened for writing
			CloseFile(nCachePos);
		}
		break;
	case SCacheEntry::Parked:
		rEntry.m_state = SCacheEntry::Closed;
		rEntry.m_nSeekPos = 0;
		rEntry.m_filename[0] = '\0';
		rEntry.m_bDiscIsEjected = false;
		break;
	default:
		gEnv->pLog->LogError("[FileHandleCache] Tried to close a file not in state Open or Parked, nCachePos = %d", nCachePos);
		DumpCacheContent();
		__debugbreak();
	}
	
	return 0; 
}

FILE* CFileHandleCache::GetRealFileHandle(FILE *pHandle)
{	
	// handle stdout and stderr :/
	IF(pHandle == stdout || pHandle == stderr, 0 )
		return pHandle;

	AUTO_LOCK_T(CryCriticalSection, m_Lock);
	uint32 nCachePos = GetCachePos(pHandle);

	ActivateEntry(nCachePos);
	SCacheEntry& rEntry = m_Cache[nCachePos];

	if( rEntry.m_bDiscIsEjected )
		return NULL;

	return rEntry.m_pHandle;
}

size_t CFileHandleCache::Fileno(FILE *pHandle )
{
#ifdef ORBIS
	ORBIS_TO_IMPLEMENT; // Broken as of 0.915 SDK
	return 0;
#else
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
	size_t nRes = pRealHandle ? ::fileno(pRealHandle) : 0;
	TestDiscEjection(pHandle);
	return nRes;
#endif
}

size_t CFileHandleCache::FRead( void *ptr, size_t size, size_t count, FILE *pHandle)
{	
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	size_t nRes = pRealHandle ? sceKernelRead(ConvertFileToHandle(pRealHandle), ptr, size*count)/size : 0;
#else
	size_t nRes = pRealHandle ? ::fread(ptr, size, count, pRealHandle) : 0;
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

size_t CFileHandleCache::FWrite( const void * ptr, size_t size, size_t count, FILE * pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	size_t nRes = pRealHandle ? sceKernelWrite(ConvertFileToHandle(pRealHandle), ptr, size*count)/size : 0;
#else
	size_t nRes = pRealHandle ? ::fwrite(ptr, size, count, pRealHandle) : 0;
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

size_t CFileHandleCache::FTell(FILE *pHandle)
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	size_t nRes = pRealHandle ? sceKernelLseek(ConvertFileToHandle(pRealHandle), 0, SCE_KERNEL_SEEK_CUR) : 0;
#else
	size_t nRes = pRealHandle ? ::ftell(pRealHandle) : (size_t)(-1L);
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

size_t CFileHandleCache::FSeek(FILE* pHandle, size_t offset, size_t origin)
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	// SEEK_SET==SCE_KERNEL_SET etc so no need for conversion
	off_t ret=sceKernelLseek(ConvertFileToHandle(pRealHandle), offset, origin);
	size_t nRes=0;
	if (ret<0)
		nRes=-1;
#else
	size_t nRes = pRealHandle ? ::fseek(pRealHandle,offset, origin) : (size_t)(-1L);
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

int CFileHandleCache::VFprintf( FILE * pHandle, const char * format, va_list arg )
{
#ifdef ORBIS_USE_NATIVE_FILE_IO
	char buffer[4096];
	int written=vsnprintf(buffer, sizeof(buffer), format, arg);
	if (written==sizeof(buffer))
		ORBIS_TO_IMPLEMENT;
	return FWrite(buffer, 1, written, pHandle);
#else
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
	int nRes = pRealHandle ? ::vfprintf (pRealHandle,format, arg) : -1;
	TestDiscEjection(pHandle);
	return nRes;
#endif
}

int CFileHandleCache::VFscanf( FILE * pHandle, const char * format, va_list arg )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int nRes=0;
	ORBIS_TO_IMPLEMENT;
#else
	int nRes = pRealHandle ? ::vfscanf( pRealHandle,format, arg): -1;
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

int CFileHandleCache::Fgetc( FILE * pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int nRes=0;
	ORBIS_TO_IMPLEMENT;
#else
	int nRes = pRealHandle ? ::fgetc( pRealHandle): -1;
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

char* CFileHandleCache::FGets( char * str, int num, FILE * pHandle )
{
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int read=FRead(str, 1, num, pHandle);
	if (read>=0)
	{
		str[min(read,num-1)]='\0';
		for (int i=0; i<read; i++)
		{
			if (str[i]=='\n')
			{
				FSeek(pHandle, i+1-read, SEEK_CUR);
				str[i+1]='\0';
				break;
			}
		}
		return str;
	}
	return NULL;
#else
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
	char* nRes = pRealHandle ? ::fgets (str,num, pRealHandle) : NULL;	
	TestDiscEjection(pHandle);
	return nRes;
#endif
}

int CFileHandleCache::Feof ( FILE * pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	off_t cur=sceKernelLseek(ConvertFileToHandle(pRealHandle), 0, SCE_KERNEL_SEEK_CUR);
	off_t end=sceKernelLseek(ConvertFileToHandle(pRealHandle), 0, SCE_KERNEL_SEEK_END);
	int nRes=1;
	if (cur!=end)
	{
		sceKernelLseek(ConvertFileToHandle(pRealHandle), cur, SCE_KERNEL_SEEK_SET);
		nRes=0;
	}
#else
	int nRes = pRealHandle ? ::feof (pRealHandle) : 0;	
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

int CFileHandleCache::Ferror ( FILE * pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int nRes=0;
#else
	int nRes = pRealHandle ? ::ferror (pRealHandle) : 0;
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

int CFileHandleCache::Getc ( FILE * pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int nRes=0;
	ORBIS_TO_IMPLEMENT;
#else
	int nRes = pRealHandle ? ::getc (pRealHandle) : EOF;	
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

int CFileHandleCache::Ungetc ( int character, FILE * pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int nRes=0;
	ORBIS_TO_IMPLEMENT;
#else
	int nRes = pRealHandle ? ::ungetc (character, pRealHandle): EOF;	
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

int CFileHandleCache::Fputs( const char * str, FILE * pHandle )
{
#ifdef ORBIS_USE_NATIVE_FILE_IO
	return FWrite(str, 1, strlen(str), pHandle);
#else
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
	int nRes = pRealHandle ? ::fputs (str, pRealHandle) : EOF;	
	TestDiscEjection(pHandle);
	return nRes;
#endif
}

int CFileHandleCache::Fflush(FILE *pHandle )
{
	FILE *pRealHandle = GetRealFileHandle(pHandle);
	errno = 0; // reset erno to not get wrong results
#ifdef ORBIS_USE_NATIVE_FILE_IO
	int nRes = pRealHandle ? sceKernelFsync(ConvertFileToHandle(pRealHandle)) : EOF;	
#else
	int nRes = pRealHandle ? ::fflush(pRealHandle) : EOF;	
#endif
	TestDiscEjection(pHandle);
	return nRes;
}

// === cellfs wrapper utils
void CFileHandleCache::OpenFromFileHandleCacheAsFD( const char *adjustedFilename, int flags, int &fd, int &err)
{
	AUTO_LOCK_T(CryCriticalSection, m_mapLock);

	FILE* f = FOpen(adjustedFilename,"rb", FileIoWrapper::STREAMING);
	if(f)
	{
		fd = Fileno(f);
		m_FdToFileMap.insert( std::make_pair( fd,f ) );		
		err=0;
	}
	else
	{
		fd = -1;
		err = errno;
	}	
}

void CFileHandleCache::CloseFromFileHandleCacheAsFD( int fd )
{
	AUTO_LOCK_T(CryCriticalSection, m_mapLock);	
	
	FDMap::iterator it = m_FdToFileMap.find( fd );
	if(it != m_FdToFileMap.end() )
	{
		FILE *f = it->second;
		FClose(f);
		m_FdToFileMap.erase(it);
		return;
	}

	gEnv->pLog->LogError("[FileHandleCache] Tried to close invalid FileDescriptor %d",fd);	
}

// === intern FILEHandleCache functions
void CFileHandleCache::ActivateEntry( uint32 nCachePos )
{
	// make sure we can really open the file
	if( m_nFileCount == nMAXOPENFILES )
	{		
		FreeUpFD(nCachePos);		
	}

	SCacheEntry& rEntry = m_Cache[nCachePos];
	
	rEntry.m_lru = ++m_LRU;
	if(m_LRU == 0)
	{
		gEnv->pLog->LogError("[FileHandleCache] LRU OverFlow!");
		__debugbreak();
	}
	
	switch(rEntry.m_state)
	{
	case SCacheEntry::Open:
		return;
	case SCacheEntry::Cached:
#ifdef ORBIS_USE_NATIVE_FILE_IO
		sceKernelLseek(ConvertFileToHandle(rEntry.m_pHandle),0,SCE_KERNEL_SEEK_SET);
#else
		::fseek(rEntry.m_pHandle,0, SEEK_SET);		
#endif
		rEntry.m_state = SCacheEntry::Open;
		rEntry.m_bDiscIsEjected = false;
		return;
	case SCacheEntry::Parked:
		ReOpenFile(nCachePos);
		return;
	case SCacheEntry::Closed:
		gEnv->pLog->LogError("[FileHandleCache Error] Tried to activate a closed entry, nCachePos = %d",nCachePos);
		DumpCacheContent();
		__debugbreak();
		return;
	}

	gEnv->pLog->LogError("[FileHandleCache] Data corruption!");
	DumpCacheContent();
	__debugbreak();
}

void CFileHandleCache::FreeUpFD( uint32 nCachePosKeep )
{

	uint32 curLowestCachedSlot		= 0xFFFFFFFF; //invalid slot id
	uint32 curLowestCachedSlotLRU = 0xFFFFFFFF;
	uint32 curLowestOpenSlot		= 0xFFFFFFFF; // invalid slot id
	uint32 curLowestOpenSlotLRU = 0xFFFFFFFF;
	

	// find slot where we could close an fd
	for( uint32 nCachePos = 0;nCachePos<nFILEENTRIES;++nCachePos)
	{	
		// don't free the slot we want to use
		if(nCachePos==nCachePosKeep) 
			continue;

		SCacheEntry& rEntry = m_Cache[nCachePos];
		
		if( rEntry.m_state == SCacheEntry::Cached && rEntry.m_lru < curLowestCachedSlotLRU )
		{
			curLowestCachedSlotLRU = rEntry.m_lru;
			curLowestCachedSlot = nCachePos;
		}

		if( rEntry.m_state == SCacheEntry::Open && rEntry.m_lru < curLowestOpenSlotLRU )
		{
			curLowestOpenSlotLRU = rEntry.m_lru;
			curLowestOpenSlot = nCachePos;
		}
	}

	// first close Cached files
	if( curLowestCachedSlot != 0xFFFFFFFF )
	{
		CloseFile(curLowestCachedSlot);
		return;
	}

	// we we didn't find a cached one, park an opened one
	if( curLowestOpenSlot != 0xFFFFFFFF )
	{
		CloseFile(curLowestOpenSlot);
		return;
	}

	gEnv->pLog->LogError("[FileHandleCache] Could not find a slot in state Cached or Open to close");
	DumpCacheContent();
	__debugbreak();
}

void CFileHandleCache::CloseFile(uint32 nCachePos )
{
	SCacheEntry& rEntry = m_Cache[nCachePos];
	
	switch(rEntry.m_state)
	{	
	case SCacheEntry::Cached:		
#ifdef ORBIS_USE_NATIVE_FILE_IO
		sceKernelClose(ConvertFileToHandle(rEntry.m_pHandle));
#else
		::fclose(rEntry.m_pHandle);		
#endif
		rEntry.m_pHandle = NULL;
		rEntry.m_filename[0] = '\0';
		rEntry.m_state = SCacheEntry::Closed;	
		rEntry.m_nSeekPos = 0;
		rEntry.m_type = FileIoWrapper::GENERAL;
		FreeFileIoBuffer(rEntry.m_fileIoBuf);
		rEntry.m_fileIoBuf = NULL;
		rEntry.m_bDiscIsEjected = false;
		memset( &rEntry.m_pakNameKey, 0, sizeof(rEntry.m_pakNameKey));
		break;	
	case SCacheEntry::Open:	
#ifdef ORBIS_USE_NATIVE_FILE_IO
		rEntry.m_nSeekPos = sceKernelLseek(ConvertFileToHandle(rEntry.m_pHandle), 0, SCE_KERNEL_SEEK_CUR);
		sceKernelClose(ConvertFileToHandle(rEntry.m_pHandle));
#else
		rEntry.m_nSeekPos = ::ftell(rEntry.m_pHandle);
		::fclose(rEntry.m_pHandle);		
#endif
		rEntry.m_pHandle = NULL;		 
		rEntry.m_state = SCacheEntry::Parked;	
		FreeFileIoBuffer(rEntry.m_fileIoBuf);
		rEntry.m_fileIoBuf = NULL;
		rEntry.m_bDiscIsEjected = false;
		break;
	default:
		gEnv->pLog->LogError("[FileHandleCache] Tried to close a file not in state Cached or Opened, nCachePos = %d",nCachePos);
		DumpCacheContent();
		__debugbreak();
	}	
	
	m_nFileCount -= 1;
}

FILE* CFileHandleCache::OpenFile(uint32 nCachePos, const char *sName, const char*op, FileIoWrapper::FileAccessType type)
{	
	SCacheEntry& rEntry = m_Cache[nCachePos];		

	// in case we are opening a non cached file, make sure we can open it
	if( m_nFileCount == nMAXOPENFILES && rEntry.m_state != SCacheEntry::Cached)
	{
		FreeUpFD(nCachePos);
	}
		
	// if we use a cached file, close it before
	if( rEntry.m_state == SCacheEntry::Cached )
	{
		CloseFile(nCachePos);
	}
	
	{
		ScopedSwitchToGlobalHeap useGlobalHeap;
#ifdef ORBIS_USE_NATIVE_FILE_IO
		rEntry.m_pHandle = ConvertHandleToFile(sceKernelOpen(sName, ConvertModeToFlags(op), SCE_KERNEL_S_IRWU));
#else
		rEntry.m_pHandle = ::fopen(sName, op );	
#endif
	}

	if ( errno == EMFILE )
	{
		gEnv->pLog->LogError("[FileHandleCache] Opened more than 31 files on gamedata");
		DumpCacheContent();
		__debugbreak();
	}

	if( rEntry.m_pHandle == NULL )
		return NULL;
	
	rEntry.m_bDiscIsEjected = false;
#ifndef ORBIS_USE_NATIVE_FILE_IO
	rEntry.m_fileIoBuf = AllocateFileIoBuffer();
	setvbuf( rEntry.m_pHandle, rEntry.m_fileIoBuf, _IOFBF, nFILEIOBUFFERSIZE );
#endif
		
	rEntry.m_state = SCacheEntry::Open;
	strcpy_s(rEntry.m_filename, sName);

	stack_string reopenOp=op;
	reopenOp.replace("wb+", "rb+");
	reopenOp.replace("wt+", "rt+");
	reopenOp.replace("w+", "r+");
	reopenOp.replace("w", "r+");
	strcpy_s(rEntry.m_mode, reopenOp.c_str());

	rEntry.m_type = type;
	// create md5 hash as a search key
	TPakNameKey	key;
	MD5Context context;
	MD5Init(&context);
	MD5Update(&context, (unsigned char*)sName, strlen(sName));
	MD5Final(rEntry.m_pakNameKey.c16, &context);

	rEntry.m_lru = ++m_LRU;
	rEntry.m_lru |= type;

	if(m_LRU == 0)
	{
		gEnv->pLog->LogError("[FileHandleCache] LRU Overflow!");
		__debugbreak();
	}
	
	m_nFileCount += 1;
	
	return GetFileHandle(nCachePos);
}

void CFileHandleCache::ReOpenFile(uint32 nCachePos)
{
	ScopedSwitchToGlobalHeap useGlobalHeap;
	SCacheEntry& rEntry = m_Cache[nCachePos];
	CryStackStringT<char, sizeof(rEntry.m_mode)> mode = rEntry.m_mode;
#ifdef ORBIS_USE_NATIVE_FILE_IO
	rEntry.m_pHandle = ConvertHandleToFile(sceKernelOpen(rEntry.m_filename, ConvertModeToFlags(mode.c_str()), SCE_KERNEL_S_IRWU));
#else
	rEntry.m_pHandle = ::fopen(rEntry.m_filename, mode.c_str());
#endif
	if ( errno == EMFILE )
	{
		gEnv->pLog->LogError("[FileHandleCache] Opened more than 31 files on gamedata");
		DumpCacheContent();
		__debugbreak();
		return;
	}

	if(rEntry.m_pHandle == NULL ) 
	{
		// if we failed to open a file which was open before, assume a disc ejection
		rEntry.m_bDiscIsEjected = true;
		return;
	}
	
#ifdef ORBIS_USE_NATIVE_FILE_IO
	sceKernelLseek(ConvertFileToHandle(rEntry.m_pHandle),rEntry.m_nSeekPos,SCE_KERNEL_SEEK_SET);
#else
	rEntry.m_fileIoBuf = AllocateFileIoBuffer();
	setvbuf( rEntry.m_pHandle, rEntry.m_fileIoBuf, _IOFBF, nFILEIOBUFFERSIZE );
	::fseek(rEntry.m_pHandle,rEntry.m_nSeekPos, SEEK_SET);
#endif
	rEntry.m_state = SCacheEntry::Open;
	rEntry.m_nSeekPos = 0;
	rEntry.m_bDiscIsEjected = false;
	m_nFileCount += 1;
}

void CFileHandleCache::DumpCacheContent()
{	
	uint32 nOpen = 0;
	uint32 nClosed = 0;
	uint32 nParked = 0;
	uint32 nCached = 0;

	gEnv->pLog->Log("\n==== FileDescriptor Cache Dump ===\n");
	for( uint32 nCachePos = 0 ; nCachePos < nFILEENTRIES ; ++nCachePos)
	{
		SCacheEntry& rEntry = m_Cache[nCachePos];
		char buffer[1024] = {0};
		char *pDst = &buffer[0];
		pDst += sprintf(pDst,"Pos %d.\tName \"%s\"\t n, State: ",nCachePos, rEntry.m_filename );
				
		switch(rEntry.m_state)
		{
		case SCacheEntry::Open:		pDst += sprintf(pDst,"Open"); nOpen++; break;
		case SCacheEntry::Closed: pDst += sprintf(pDst,"Closed"); nClosed++; break;
		case SCacheEntry::Parked:	pDst += sprintf(pDst,"Parked"); nParked++; break;
		case SCacheEntry::Cached: pDst += sprintf(pDst,"Cached"); nCached++; break;
		}
		pDst += sprintf(pDst, "\tType: ");
		if(rEntry.m_type & FileIoWrapper::DIRECT) 
			pDst += sprintf(pDst, "Direct");
		else if(rEntry.m_type & FileIoWrapper::STREAMING) 
			pDst += sprintf(pDst, "Streaming");
		else
			pDst += sprintf(pDst, "General");

		pDst += sprintf(pDst, "\tLRU %d",rEntry.m_lru);
		gEnv->pLog->Log(buffer);
	}

	gEnv->pLog->Log("FileDescriptors open %d (Open %d, Closed %d, Parked %d, Cached %d)", m_nFileCount, nOpen, nClosed, nParked, nCached);	
}
void LogOpenFiles(struct IConsoleCmdArgs*)
{
	CFileHandleCache::Instance().DumpCacheContent();
}

void OpenFromFileHandleCacheAsFD( const char *adjustedFilename, int flags, int &fd, int dummy, int &err)
{
	CFileHandleCache::Instance().OpenFromFileHandleCacheAsFD(adjustedFilename,flags, fd, err );	
}

void CloseFromFileHandleCacheAsFD( int fd )
{	
	CFileHandleCache::Instance().CloseFromFileHandleCacheAsFD(fd);
}

namespace std
{
// ==== file io wrapper ==== //
#if defined(USE_FILE_HANDLE_CACHE)
// non-ps3 versions are below
FILE *WrappedFopen(const char *__restrict filename,	const char *__restrict mode, FileIoWrapper::FileAccessType type, bool bSysAppHome )
{		
	char buf[512];
			
#if defined(PS3)
	FILE *fp = CFileHandleCache::Instance().FOpen( bSysAppHome ? filename : ConvertFileName(buf, filename), (const char*)mode, type);	
#elif defined(ORBIS)
	FILE *fp = CFileHandleCache::Instance().FOpen( ConvertFileName(buf, filename), (const char*)mode, type);	
#else
	FILE *fp = CFileHandleCache::Instance().FOpen( filename, (const char*)mode, type);	
#endif
	return fp;
}

int WrappedStat(const char * _Filename, struct stat * _Stat)
{
	if (strchr(_Filename, '*'))
	{
		return -1; // Wildcards should always fail a 'stat'
	}
	char buf[512];
#if defined(PS3) || defined(ORBIS)
	return ::stat(ConvertFileName(buf, _Filename), _Stat);
#else
	return ::stat(_Filename, _Stat);
#endif
}

#ifdef ORBIS
int WrappedFStat(int files, struct stat * _Stat)
{
	return fstat(files, _Stat);
}
#endif

int WrappedFclose(FILE *fp)			
{			
	return CFileHandleCache::Instance().FClose(fp);
}

size_t WrappedFileno(FILE *fp)
{
	return CFileHandleCache::Instance().Fileno(fp);
}

size_t WrappedFtell( FILE *pHandle )
{
	return CFileHandleCache::Instance().FTell(pHandle);	
}

size_t WrappedFSeek(FILE *pHandle, size_t offset, size_t origin)
{
	return CFileHandleCache::Instance().FSeek(pHandle, offset, origin);
}

size_t WrappedFRead(void *ptr, size_t size , size_t count, FILE *pHandle)
{
	return CFileHandleCache::Instance().FRead(ptr, size, count, pHandle);
}

size_t WrappedFWrite( const void * ptr, size_t size, size_t count, FILE * pHandle )
{
	return CFileHandleCache::Instance().FWrite(ptr, size, count, pHandle);
}

int WrappedVFprintf ( FILE * stream, const char * format, va_list arg )
{
	return CFileHandleCache::Instance().VFprintf(stream, format, arg );
}

char * WrappedFGets ( char * str, int num, FILE * stream )
{
	return  CFileHandleCache::Instance().FGets(str,num,stream);
}

int WrappedFeof ( FILE * stream )
{
	return  CFileHandleCache::Instance().Feof(stream);
}

int WrappedFerror ( FILE * stream )
{
	return  CFileHandleCache::Instance().Ferror(stream);
}

int WrappedGetc ( FILE * stream )
{
	return  CFileHandleCache::Instance().Getc(stream);
}

int WrappedUngetc ( int character, FILE * stream )
{
	return CFileHandleCache::Instance().Ungetc(character,stream);
}

int WrappedFputs( const char * str, FILE * stream )
{
	return CFileHandleCache::Instance().Fputs(str,stream);
}

int WrappedFflush( FILE *stream )
{
	return CFileHandleCache::Instance().Fflush(stream);
}

int WrappedFprintf ( FILE * stream, const char * format, ... )
{
	va_list args;
	va_start(args, format);
	int ret = CFileHandleCache::Instance().VFprintf(stream, format, args );
	va_end(args);
	return ret;
}

int WrappedVFscanf ( FILE * stream, const char * format, va_list arg )
{
	return CFileHandleCache::Instance().VFscanf(stream, format, arg );
}

int WrappedFgetc ( FILE * stream )
{
	return CFileHandleCache::Instance().Fgetc(stream);
}

int WrappedFscanf ( FILE * stream, const char * format, ... )
{
	va_list args;
	va_start(args, format);
	int ret = CFileHandleCache::Instance().VFscanf(stream, format, args );
	va_end(args);
	return ret;
}


// ==== non PS3 fopen/fclose wrapper
#else 

extern "C" FILE *WrappedFopen(const char *__restrict filename,	const char *__restrict mode)
{
	bool isWrite = false;
	bool skipOpen = false;
	char buffer[MAX_PATH + 1];
	if (fopenwrapper_basedir[0] != '/')
		WrappedF_InitCWD();

	++file_op_counter;
	if (file_op_counter == file_op_break)
		WrappedF_Break(file_op_counter);

	const bool cContainsWrongSlash = strchr(filename, '\\');
	if((cContainsWrongSlash || filename[0] != '/'))
	{
		char *bp = buffer, *const buffer_end = buffer + sizeof buffer;
		buffer_end[-1] = 0;
		if (filename[0] != '/')
		{
			strncpy(bp, fopenwrapper_basedir, buffer_end - bp - 1);
			bp += strlen(bp);
			if (bp > buffer && bp[-1] != '/' && bp < buffer_end - 2)
				*bp++ = '/';
		}
		strncpy(bp, filename, buffer_end - bp - 1);
		//replace '\\' by '/' and lower it
		const int cLen = strlen(buffer);
		for (int i = 0; i<cLen; ++i)
		{
			buffer[i] = tolower(buffer[i]);
			if (buffer[i] == '\\')
				buffer[i] = '/';
		}
		filename = buffer;
	}
	else
		if(cContainsWrongSlash)
		{
			const int cLen = strlen(filename);
			for (int i = 0; i<cLen; ++i)
			{
				buffer[i] = filename[i];
				if (buffer[i] == '\\')
					buffer[i] = '/';
			}
			filename = buffer;
		}
		// Note: "r+" is not considered to be a write-open, since fopen() will
		// fail if the specified file does not exist.
		if (strchr(mode, 'w') || strchr(mode, 'a')) isWrite = true;
#if defined(LINUX) || defined(APPLE)
		const bool skipInitial = false;
#else
		const bool skipInitial = true;
#endif

		bool failOpen = false;
#if defined(USE_FILE_MAP)
		if (!FileNode::CheckOpen(filename, isWrite, skipInitial))
			failOpen = true;
#endif
		FILE *fp = 0;
#if defined(FILE_MAP_DEBUG)
		if (!skipOpen)
			fp = fopen(filename, mode);
		if (fp && failOpen)
		{
			puts("FileNode::CheckOpen error");
			assert(0);
		}
#else
		if (failOpen)
		{
			fp = NULL;
			errno = ENOENT;
		} else if (!skipOpen)
			fp = fopen(filename, mode);
#endif
		if (fp)
			++file_counter;

		return fp;
}

extern "C" int WrappedFclose(FILE *fp)
{
	const bool error = false;
	int err = 0;
	if (!error)
	{
		err  = fclose(fp);
	}
	if (err == 0)
	{
		if (!error) --file_counter;
	} else
	{
		printf("WrappedFclose: fclose() failed: %s [%i]\n",	strerror(err), err);
		WrappedF_Break(file_op_counter);
	}
	return err;
}
#endif
} // namespace std





#endif


#if defined(APPLE) || defined(LINUX)
// WinAPI debug functions.
DLL_EXPORT void OutputDebugString(const char *outputString)
{
#if _DEBUG
	// There is no such thing as a debug console on XCode
	fprintf(stderr, "debug: %s\n", outputString);
#endif
}


DLL_EXPORT void DebugBreak()
{
	CryDebugBreak();
}

#endif
