////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2011.
// -------------------------------------------------------------------------
//  File name:   IJobManager.h
//  Version:     v1.00
//  Created:     1/8/2011 by Christopher Bolte
//  Description: JobManager interface (developed from the SPU only JobManager)
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#if !defined(__SPU__) // don't devirtualize on SPU
#include DEVIRTUALIZE_HEADER_FIX(IJobManager.h)
#endif

#ifndef __IJOBMAN_SPU_H
#define __IJOBMAN_SPU_H
#pragma once

#if !defined(__SPU__)
#include <ITimer.h>
#endif

#if defined(PS3)
#if !defined(__SPU__) 
#include <stdio.h>
#include <stdlib.h>
#include "sn/libsn.h"	
#include <ppu_intrinsics.h>
#include <ISystem.h>
#endif
#endif

#if !defined(__SPU__) 
#include "CryArray.h"
#endif

// include global variables which are shared with SPUBackend
#include <JobManager_SPUDriver.h>

#if !defined(_SPU_DRIVER_PERSISTENT) && !defined(_LIB_DRIVER)
#include <CryThread.h>
#endif


#if !defined(_ALIGN)
// needed for SPU compilation (should be removed as soon as the spus doesn't depend on this header anymore
#define _ALIGN(x) __attribute__((aligned(x)))
#endif

#if !defined(_MS_ALIGN)
// needed for SPU compilation (should be removed as soon as the spus doesn't depend on this header anymore
#define _MS_ALIGN(x)
#endif

// for some spu/crycg compile path, STATIC_CHECK is not defined, but for those it doesn't really matter
#if !defined(STATIC_CHECK)
	#define STATIC_CHECK(a, b)
#endif

#if !defined(CHAR_BIT)
	#define CHAR_BIT 8
#endif

#if defined(__SPU__)
// add types requiered for SPUs
#include "BaseTypes.h"
#endif

#if defined(__SPU__)
	#define SPU_DRIVER_INT_PTR uint32
#else
	#define SPU_DRIVER_INT_PTR INT_PTR
#endif
// temp workaround for SPU include
#if !defined(ILINE) && defined(__SPU__)
#define ILINE
#endif

#if !defined(RESTRICT_REFERENCE) && defined(__SPU__)
#define RESTRICT_REFERENCE __restrict
#endif

#if !defined(UNIQUE_IFACE) && defined(__SPU__)
#define UNIQUE_IFACE 
#endif

#if defined(PS3)
typedef long LONG;
#endif

// ==============================================================================
// forward declarations
// ==============================================================================
struct ILog;
struct CellGcmContextData;
struct CellSpurs;


class SJobFinishedConditionVariable;

#if !defined(__SPU__)
// implementation of mutex/condition variable
// used in the job manager for yield waiting in corner cases
// like waiting for a job to finish/jobqueue full and so on
class SJobFinishedConditionVariable
{
public:
	SJobFinishedConditionVariable() :
			m_nRefCounter(0),
			m_pOwner(NULL)
	{
		m_nFinished = 1;
	}

	void Acquire()
	{
		//wait for completion of the condition
		m_Notify.Lock();
		while(m_nFinished == 0)
			m_CondNotify.Wait(m_Notify);
		m_Notify.Unlock();
	}

	void Release()
	{		
		m_Notify.Lock();
		m_nFinished = 1;
		m_CondNotify.Notify();
		m_Notify.Unlock();	
	}

	void SetRunning()
	{
		m_nFinished = 0;
	}

	bool IsRunning()
	{
		return m_nFinished == 0;
	}

	void SetStopped()
	{
		m_nFinished = 1;
	}

	void SetOwner(volatile const void *pOwner)
	{
		if(m_pOwner!=NULL)
			__debugbreak();
		m_pOwner = pOwner;		
	}
	
	void ClearOwner(volatile const void *pOwner)
	{
		if(m_pOwner != pOwner)		
			__debugbreak();

		m_pOwner = NULL;		
	}

	bool AddRef(volatile const void *pOwner) 
	{ 
		if(m_pOwner != pOwner)
			return false;

		++m_nRefCounter; 
		return true;
	}
	uint32 DecRef(volatile const void *pOwner) 
	{ 
		if(m_pOwner != pOwner)
			__debugbreak();

		return --m_nRefCounter; 			
	}

	bool HasOwner() const { return m_pOwner != NULL; }

private:
	CryMutex							m_Notify;
	CryConditionVariable	m_CondNotify;	
	volatile uint32				m_nFinished;
	volatile uint32				m_nRefCounter;
	volatile const void*	m_pOwner;
};
#endif


///////////////////////////////////////////////////////////////////////////////
//forward declarations
namespace NSPU{ namespace NDriver{ struct SJobPerfStats; } }

namespace JobManager { class CJobBase; }
namespace JobManager { struct ISPUBackend; }
namespace JobManager { class CJobDelegator; }
namespace JobManager { struct SJobQueueSlotState; }

///////////////////////////////////////////////////////////////////////////////
// function forward declarations for friend declaration
namespace NSPU { namespace NDriver { void ProcessJob(const uint32, const uint32, const uint32); } }


///////////////////////////////////////////////////////////////////////////////
// Wrapper for Vector4 Uint
struct SVEC4_UINT
{
#if defined(PS3)
	vec_uint4 v;
#if !defined(__SPU__)
	SVEC4_UINT& operator=( const SVEC4_UINT &rOther ) { __stvx(__lvlx(alias_cast<float*>(&rOther.v), 0), alias_cast<float*>(&this->v), 0); return *this; }
#endif
#elif defined(XENON)
	__vector4 v;
	SVEC4_UINT& operator=( const SVEC4_UINT &rOther ) { __stvx(__lvlx(&rOther.v, 0), &this->v, 0); return *this; }
#elif defined(LINUX) || defined(APPLE) || defined(CAFE)
	uint32 v[4];
#else
	__m128 v;	
#endif
} _ALIGN(16);


// ==============================================================================
// global values
// ==============================================================================
// front end values
namespace JobManager
{	
	/////////////////////////////////////////////////////////////////////////////
	// type to reprensent a semaphore handle of the jobmanager
	typedef uint16 TSemaphoreHandle;

	/////////////////////////////////////////////////////////////////////////////
	// magic value to reprensent an invalid job handle
	enum { INVALID_JOB_HANDLE = ((unsigned int)-1) };

	/////////////////////////////////////////////////////////////////////////////
	//return results for AddJob
	enum EAddJobRes
	{
		eAJR_Success,						//success of adding job
		eAJR_NoElf,							//spu job is no elf file
		eAJR_NoSPUElf,					//spu job is no spu elf file
		eAJR_ElfTooBig,					//spu job elf is too big
		eAJR_NoQWAddress,				//spu job image start is not on a quadword address
		eAJR_EnqueueTimeOut,		//spu job was not added due to timeout (SPU were occupied for too long)
		eAJR_EnqueueTimeOutPushJob,		//spu job was not added due to timeout of a push job slot (SPU was occupied by one particular job for too long)
		eAJR_NotInitialized,	//SPU were not initialized
		eAJR_JobTooLarge,				//spu job cannot fit into a single SPU local store
		eAJR_JobSetupViolation,	//spu job has some invalid setup for packets/depending jobs
		eAJR_InvalidJobHandle,	//spu job was invoked with an invalid job handle (job has not been found in spu repository)		
		eAJR_NeedFallbackJobInfoBlock, 
		eAJR_UnknownError,			//unknown error
	};

	// Functions to use to allocation, deallocation
	typedef void (*TSPUFreeFunc)(void*);
	typedef void* (*TSPUMallocFunc)(unsigned int, unsigned int);

	/////////////////////////////////////////////////////////////////////////////
	// BackEnd Type
	enum EBackEndType
	{
		eBET_SPU,
		eBET_Thread,
		eBET_Fallback,
		eBET_Blocking
	};

// SPUBackEnd values
namespace SPUBackend
{	

	enum { PAGE_MODE_MASK = 3 };

	enum { CACHE_MODE_MASK = ~7 };
	enum { JOBTYPE_MODE_MASK = 4 };
	enum { CACHE_MODE_SIZE_SHIFT = 3 };

	enum { PROF_ID_RESERVED = 8 };
	enum { MAX_PROF_ID = 1024 };						//max num of supported cache lookup id's
	enum { SPU_PRINTF_BUF_SIZE = 512	 };

	//maximum cache size, 
	enum ECacheMode
	{
		eCM_None = 0,			//no cache, bypassing
		eCM_4		 = 8,			//max cache size 4 KB
		eCM_8		 = 16,		//max cache size 8 KB
		eCM_16	 = 32,		//max cache size 16 KB
		eCM_32	 = 64,		//max cache size 32 KB
		eCM_64	 = 128		//max cache size 64 KB, default
	};

	// type of job
	enum EJobType
	{
		eGeneralJob = 0,	// general job, put into worker thread
		eOffLoadJob = 4	// offload job, only put into worker, if worker is a different cpu, like SPUs
	};

	#define CACHE_MODE_DEFAULT JobManager::SPUBackend::eCM_64
	#define PAGE_MODE_DEFAULT  JobManager::SPUBackend::ePM_Single
	
} // namespace JobManager::SPUBackend

namespace Fiber
{
	// The alignment of the fibertask stack (currently set to 128 kb)
	enum {FIBERTASK_ALIGNMENT = 128U<<10 };
	// The number of switches the fibertask records (default: 32) 
	enum {FIBERTASK_RECORD_SWITCHES = 32U };

} // namespace JobManager::Fiber

} // namespace JobManager


// ==============================================================================
// Used structures of the JobManager
// ==============================================================================
namespace JobManager
{	
	struct SJobStringHandle
	{
		const char *cpString;			//points into repository, must be first element
		unsigned int strLen;			//string length
		uint32 jobHandle;					//job address (corresponding to NBinPage::SJob)
		int jobId;								//index (also acts as id) of job
		uint32 nJobInvokerIdx;		//index of the jobInvoker (used to job switching in the prod/con queue)

		inline bool operator==(const SJobStringHandle& crOther) const;		
		inline bool operator<(const SJobStringHandle& crOther) const;
		
	};

	//handle retrieved by name for job invocation
	typedef SJobStringHandle* TJobHandle;	

	inline bool IsValidJobHandle(const JobManager::TJobHandle cJobHandle)
	{
		return cJobHandle->jobHandle != JobManager::INVALID_JOB_HANDLE;
	}

	bool SJobStringHandle::operator==(const SJobStringHandle& crOther) const
	{
#if !defined(__SPU__)
		return strcmp(cpString,crOther.cpString) == 0;
#else
		snPause();
		return true;
#endif
	}

	bool SJobStringHandle::operator<(const SJobStringHandle& crOther) const
	{
#if !defined(__SPU__)
		return strcmp(cpString,crOther.cpString) < 0;
#else
		snPause();
		return true;
#endif
	}

	// struct to collect profiling informations about job invocations and sync times
	struct SJobProfilingData
	{	
#if defined(__SPU__)
		typedef uint64 TimeValueT;
#else		
		typedef CTimeValue TimeValueT;
#endif

		TimeValueT nStartTime;
		TimeValueT nEndTime;

		uint32 nWorkerThread;
		uint32 nTimeCacheLookups;		// only used on PS3
		uint32 nTimeCodePaging;			// only used on PS3
		uint32 nTimeFnResolve;			// only used on PS3
		uint32 nTimeMemtransferSync; // only used on PS3
		uint32 pad[3];

		// first 48 bytes are written by SPU		
		TimeValueT nWaitBegin;
		TimeValueT nWaitEnd;

		TJobHandle jobHandle;		
		uint32 nThreadId;
	} _ALIGN(16); // 48 bytes

	struct SJobProfilingDataContainer
	{
		enum {nCapturedFrames = 4};
		enum {nCapturedEntriesPerFrame = 4096 };
		uint32 nFrameIdx;
		uint32 nProfilingDataCounter[nCapturedFrames];
				
		ILINE uint32 GetFillFrameIdx() { return nFrameIdx%nCapturedFrames; }
		ILINE uint32 GetPrevFrameIdx() { return (nFrameIdx-1)%nCapturedFrames;}
		ILINE uint32 GetRenderFrameIdx() { return (nFrameIdx-2)%nCapturedFrames;}
		ILINE uint32 GetPrevRenderFrameIdx() { return (nFrameIdx-3)%nCapturedFrames;}

		SJobProfilingData m_DymmyProfilingData _ALIGN(128);
		SJobProfilingData arrJobProfilingData[nCapturedFrames][nCapturedEntriesPerFrame];
	} _ALIGN(128);

	// special combination of a volatile spinning variable combined with a semaphore
	// which is used if the running state is not yet set to finish during waiting
	struct SJobSyncVariable
	{		
#if !defined(_SPU_DRIVER_PERSISTENT) // no constructor for SPU driver, to spare code size
		SJobSyncVariable();
#endif
		bool IsRunning() const volatile;
	
		// interface, should only be used by the job manager or the job state classes
		void Wait() volatile;		
		void SetRunning() volatile;
		bool SetStopped() volatile;		

#if defined(__SPU__)
		ILINE void SetRunningForce()		{ syncVar.wordValue = 0; syncVar.nRunningCounter = 1; } // sets the sync var to 1, without any atomic checks, used for SPU job submission
#endif

	private:
		friend class CJobManager;		
		friend void NSPU::NDriver::ProcessJob(const uint32, const uint32, const uint32);		

		// union used to combine the semaphore and the running state in a single word
		union SyncVar
		{
			volatile uint32 wordValue;
			struct
			{
				uint16 nRunningCounter;
				TSemaphoreHandle semaphoreHandle;
			};
		};
		 
		// members
		SyncVar syncVar;		// sync-variable which contain the running state or the used semaphore
#if defined(PLATFORM_64BIT)
		char padding[4];
#endif
	};


	//condition variable like struct to be used for polling if a job has been finished
	//since it is set by the SPU by DMA, it has to be on a 16 byte boundary (alignment ensured by padding in SJobData)
	struct SJobStateBase
	{
	public:		
		ILINE bool IsRunning() const		{	return syncVar.IsRunning(); }
		ILINE void SetRunning( )				
		{
#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
			__spu_set_job_state_running( (SJobSyncVariable*)this);
#else
			syncVar.SetRunning(); 
#endif
		}
		ILINE bool SetStopped()					
		{ 
#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
			return __spu_set_job_state_stopped( (SJobSyncVariable*)this);
#else
			return syncVar.SetStopped(); 
#endif
		}			
		ILINE void Wait()								
		{ 
#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
			__spu_job_wait( (SJobSyncVariable*)this);
#else
			syncVar.Wait(); 
#endif
		}

#if defined(__SPU__)
		ILINE void SetRunningForce()		{ syncVar.SetRunningForce(); } // sets the sync var to 1, without any atomic checks, used for SPU job submission
#endif

	private:
		friend class CJobManager;			

		SJobSyncVariable syncVar;		
	};

	// for speed use 16 byte aligned job state
	struct SJobState : SJobStateBase 
	{ 
		// when doing profiling, intercept the SetRunning() and SetStopped() functions for profiling informations

		ILINE SJobState() 
			: m_pFollowUpJob(NULL)
#if defined(JOBMANAGER_SUPPORT_PROFILING)
			, nProfilerIndex(~0)
#endif
		{}

		ILINE void SetRunning();		
		void SetStopped();	

		ILINE void RegisterPostJob(CJobBase *pFollowUpJob) { m_pFollowUpJob = pFollowUpJob; }

#if defined(JOBMANAGER_SUPPORT_PROFILING)	
		uint16 nProfilerIndex;
#endif

		CJobBase *m_pFollowUpJob;
	}_ALIGN(16);

	// Stores worker utilization stats for a frame
	class CWorkerFrameStats
	{
	public:
		struct SWorkerStats
		{
			float  nUtilPerc;						// Utilization percentage this frame [0.f..100.f]
			uint32 nExecutionPeriod;		// Total execution time on this worker in usec
			uint32 nNumJobsExecuted;		// Number of jobs executed contributing to nExecutionPeriod
		};

	public:
		ILINE CWorkerFrameStats(uint8 workerNum)
			:numWorkers(workerNum)
		{
			workerStats = new SWorkerStats[workerNum];
			Reset();
		}

	ILINE ~CWorkerFrameStats()
	{
			delete[] workerStats;
	}

	ILINE void Reset()
	{
		for(int i=0; i < numWorkers; ++i)
		{
			workerStats[i].nUtilPerc = 0.f;
			workerStats[i].nExecutionPeriod = 0;
			workerStats[i].nNumJobsExecuted = 0;
		}

		nSamplePeriod = 0;
	}

	public:
		uint8					numWorkers;
		uint32				nSamplePeriod;
		SWorkerStats* workerStats;
	};

	struct SWorkerFrameStatsSummary
	{
		uint32 nSamplePeriod;					  // Worker sample period
		uint32 nTotalExecutionPeriod;		// Total execution time on this worker in usec
		uint32 nTotalNumJobsExecuted;		// Total number of jobs executed contributing to nExecutionPeriod
		float  nAvgUtilPerc;						// Average worker utilization [0.f ... 100.f]
		uint8	 nNumActiveWorkers;				// Active number of workers for this frame
	};

	//per frame rsx data emitted by DXPS thread
	struct SFrameProfileRSXData
	{
		unsigned int frameTime;
		unsigned int rsxWaitTime;
		unsigned int psTime;
		unsigned int vsTime;
		unsigned int flushTime;
		unsigned int inputLayoutTime;
		unsigned int pad[2];
	} _ALIGN(16);


	//per frame and job specific 
	struct SJobFrameStats
	{
		unsigned int usec;				//last frame's job time in microseconds (written and accumulated by SPU/Thread)
		unsigned int count;				//number of calls this frame (written by PPU and SPU)
		const char* cpName;				//job name (written by PPU)

		unsigned int usecLast;		//last but one frames SPU time in microseconds (written and accumulated by SPU/Thread)

		inline SJobFrameStats();
		inline SJobFrameStats(const char* cpJobName);		

		inline void Reset();
		inline void operator=(const SJobFrameStats& crFrom);						
		inline bool operator<(const SJobFrameStats& crOther) const;

		static bool sort_lexical ( const JobManager::SJobFrameStats& i, const JobManager::SJobFrameStats& j)
		{
			return strcmp(i.cpName, j.cpName) < 0; 
		}

		static bool sort_time_high_to_low ( const JobManager::SJobFrameStats& i, const JobManager::SJobFrameStats& j)
		{
			return i.usec > j.usec;
		}

		static bool sort_time_low_to_high ( const JobManager::SJobFrameStats& i, const JobManager::SJobFrameStats& j)
		{
			return i.usec < j.usec;
		}
	} _ALIGN(16);

	_MS_ALIGN(16) struct SJobFrameStatsSummary
	{
		unsigned int nTotalExecutionTime;					// Total execution time of all jobs in microseconds
		unsigned int nNumJobsExecuted;						// Total Number of job executions this frame 
		unsigned int nNumIndividualJobsExecuted;	// Total Number of individual jobs
	} _ALIGN(16);

	SJobFrameStats::SJobFrameStats(const char* cpJobName) : usec(0), count(0), cpName(cpJobName), usecLast(0)
	{}

	SJobFrameStats::SJobFrameStats() : cpName("Uninitialized"), usec(0), count(0), usecLast(0)
	{}

	void SJobFrameStats::operator=(const SJobFrameStats& crFrom)
	{
		usec = crFrom.usec;
		count = crFrom.count;
		cpName = crFrom.cpName;
		usecLast = crFrom.usecLast;
	}

	void SJobFrameStats::Reset()
	{
		usecLast = usec;
		usec = count = 0;
	}

	bool SJobFrameStats::operator<(const SJobFrameStats& crOther) const
	{
		//sort from large to small
		return usec > crOther.usec;
	}

	//binary representation of a job
	struct SJob
	{
		unsigned short totalJobSize;		//total size of job in 4 bytes (header plus executable code and function table size) (size >> 2)
		short initialPages[4];					//id of first 4 pages, entry point points into initialPages[0], other might be -1
		unsigned short destPageOff;			//relative offset of the entry point into the first page (in 4 byte units)
		unsigned short branchOff;				//offset of the branch (relative to the job start (not text start)) (in 4 byte units)
		unsigned short bhOff;						//offset of the branch hint (relative to the job start (not text start)) (in 4 byte units)

		unsigned short funcTableOffset;	//offset of the function table relative to SJob, 0 if funcTableEntryCount = 0
		unsigned short funcTableEntryCount;//number of entries in function table
		unsigned short funcTableDebugOffset;//offset of the function table PPU addresses relative to SJob, will be same as funcTableOffset if not present
		unsigned short pageMode;				//page mode set by JobGen, encoded in upper 2 bite, lower bits contain max page size
		unsigned short bssOffset;				//offset of bss relative to job start (in multiple of 16 bytes)
		unsigned short bssSize;					//size of bss(in multiple of 16 bytes)
		unsigned short funcTableBinOff;	//binary offset of function table (without bss), in multiple of 4 bytes
		unsigned short funcTableSize;		//function table size, in multiple of 4 bytes

		unsigned short funcProfCount;		//number of function profile sections
		unsigned short funcProfIndStart;//index string table starts for function profile sections
		unsigned short funcProfTimingOff128;//offset for job timing area in cacheline units(allocated at startup)
		unsigned short pad0;

		unsigned int	 jobNameEA;				//set up during loading to map from SJob to its name
		unsigned short pad1[2];

		static const unsigned int scPageModeMask	= (1<<15 | 1<<14);
		static const unsigned int scPageModeShift = (14);

		unsigned int GetMaxPageSize()
		{
			return (pageMode & ~scPageModeMask) * 128;
		}
		unsigned int GetPageMode()
		{
			return (pageMode & scPageModeMask) >> scPageModeShift;
		}
		void SetMaxPageSize(const unsigned int cMaxSize)
		{
			pageMode &= scPageModeMask; pageMode |= (unsigned short)(cMaxSize / 128);
		}
		void SetPageMode(const unsigned int cPageMode)
		{
			pageMode &= ~scPageModeMask; pageMode |= (unsigned short)((unsigned int)cPageMode << scPageModeShift);
		}

		//here follows the executable text
		//	followed by the function pointer table
	} _ALIGN(16);	


	// struct to represent a packet for the consumer producer queue
	_MS_ALIGN(16) struct SAddPacketData
	{
		unsigned char opMode;
		unsigned char nInvokerIndex; // to keep the struct 16 byte big, we use a index into the info block
		unsigned short profilerIndex;
		SPU_DRIVER_INT_PTR eaJobState;
		SPU_DRIVER_INT_PTR binJobEA;
		SPU_DRIVER_INT_PTR jobEA;
		ILINE void SetPageMode(const JobManager::SPUBackend::EPageMode cPageMode)
		{
			opMode = (opMode & ~JobManager::SPUBackend::PAGE_MODE_MASK) | (unsigned char)cPageMode;
		}
		ILINE void SetCacheMode(const JobManager::SPUBackend::ECacheMode cCacheMode)
		{
			opMode = (opMode & ~JobManager::SPUBackend::CACHE_MODE_MASK) | (unsigned char)cCacheMode;
		}
		ILINE JobManager::SPUBackend::EPageMode GetPageMode() const
		{
			return (JobManager::SPUBackend::EPageMode)(opMode & JobManager::SPUBackend::PAGE_MODE_MASK);
		}
		ILINE JobManager::SPUBackend::ECacheMode GetCacheMode() const
		{
			return (JobManager::SPUBackend::ECacheMode)(opMode & JobManager::SPUBackend::CACHE_MODE_MASK);
		}
	} _ALIGN(16);

	//callback information
	struct SCallback
	{
		typedef void (*TSPUCallbackFunc)(void*);

		TSPUCallbackFunc pCallbackFnct;				//callback function, null if no
		void* __restrict pArg;							//argument to pCallbackFnct

		ILINE SCallback() : pCallbackFnct(0), pArg(0){}
	};

	// delegator function, takes a pointer to a params structure
	// and does the decomposition of the parameters, then calls the
	// job entry function
	typedef void (*Invoker)(void*);

	//info block transferred first for each job, size if passed to in CJobManager::EnqueueSPUJob
	//we have to transfer the info block, parameter block, input memory needed for execution, data and text of job
	//first 16 bytes gets overwritten for job state from SPU, dont store some post job persistent data there
	//parameter data, packet chain ptr and dma list data are transferred at once since they got allocated consecutively
	_MS_ALIGN(128) struct SInfoBlock
	{
		// struct to represent the state of a SInfoBlock
		struct SInfoBlockState
		{
			// union of requiered members and uin32 to allow easy atomic operations
			union
			{
				struct
				{
					TSemaphoreHandle	nSemaphoreHandle;			// handle for the conditon variable to use in case some thread is waiting
					uint16						nRoundID;							// number of rounds over the job queue, to prevent a race condition with SInfoBlock reuse (we have 15 bits, thus 32k full job queue iterations are safe)
				};
				volatile LONG nValue;											// value for easy Compare and Swap
			};

			void IsInUse(uint32 nRoundIdToCheckOrg, bool &rWait, bool &rRetry, uint32 nMaxValue) volatile const 
			{ 
				uint32 nRoundIdToCheck = nRoundIdToCheckOrg & 0x7FFF; // clamp nRoundID to 15 bit
				uint32 nLoadedRoundID = nRoundID;
				uint32 nNextLoadedRoundID = ((nLoadedRoundID+1) & 0x7FFF);
				nNextLoadedRoundID = nNextLoadedRoundID >= nMaxValue ? 0 : nNextLoadedRoundID;
				
				if(nLoadedRoundID == nRoundIdToCheck) // job slot free
				{
					rWait = false;
					rRetry = false;					
				}else if( nNextLoadedRoundID == nRoundIdToCheck ) // job slot need to be worked on by worker
				{
					rWait = true;
					rRetry = false;
				}
				else // producer was suspended long enough that the worker overtook it
				{
					rWait = false;
					rRetry = true;
				}				
			}
		};

		// data needed to run the job and it's functionality		
		volatile SInfoBlockState				jobState;							// state of the SInfoBlock in job queue, should only be modified by access functions
		JobManager::SCallback						callBackData;					// struct to contain information about a possible callback when the job finished( 8 bytes)
		Invoker													jobInvoker;						// callback function to job invoker (extracts parameters and calls entry function)
		SPU_DRIVER_INT_PTR							eaExtJobStateAddress;	//external job state address /shared with address of prod-cons queue		
		JobManager::SInfoBlock*					pNext;								// single linked list for fallback jobs in the case the queue is full, and a worker wants to push new work		
		SPU_DRIVER_INT_PTR							eaDMAJobAddress;			//source address of job program data
		
		// per job settings like cache size and so on
		unsigned char firstPageIndex;							//index of first page job branches into, added to fetch page fast
		unsigned char frameProfIndex;							//index of SJobFrameStats*				
		unsigned char nflags;											
		unsigned char opMode;											//page mode and cache mode		
		unsigned char paramSize;									//size in total of parameter block in 16 byte units
		unsigned char jobId;											//corresponding job ID, needs to track jobs		

		//	SPU-setting to know how to initialize the job	
		unsigned short bssSize;										//size of bss segment in 16 bytes
		unsigned short bssOff;										//offset of bss segment relativ to job start in 16 bytes
		unsigned short funcTableBinOff;						//binary offset of function table (without bss), in multiple of 4 bytes
		unsigned short funcTableSize;							//function table size, in multiple of 4 bytes
		unsigned short maxPageSize;								//max page size in 128 bytes		
		unsigned short jobSize;										//size of job to transfer (contains .data and .text section) (multiple of 4 bytes)	

		// two bytes: DO_SPU_FUNCPROFILING is PS3 only, profilerIndex on all other platforms, therefore share the memory
		// could also use a union, but this solution is hopefully more clear
#if defined(JOBMANAGER_SUPPORT_PROFILING)
		unsigned short profilerIndex;							//index for the job system profiler
#elif defined(PS3) // needed for DO_SPU_FUNCPROFILING on PS3
		unsigned short funcProfTimingCount;				//size of function profiler area to add to bss
#else
		unsigned short pad;											// padding
#endif

#if defined(LINUX) || defined(MAC)
		static const uint64 scNoThreadId = THREADID_NULL;//indicates that spu index acts as thread id, we have to define it as int64 instead of threadID for avoinding double definitions
#else
		static const threadID scNoThreadId = THREADID_NULL;//indicates that spu index acts as thread id, we have to define it as int64 instead of threadID for avoinding double definitions
#endif
		//bits used for flags
		static const unsigned int scDedicatedThreadOnly		= 0x1;
		static const unsigned int scHasQueue							= 0x4;
		static const unsigned int scDebugEnabled					= 0x8;		
		static const unsigned int scTransferProfDataBack	= 0x10;
		static const unsigned int scKeepQueueCache				= 0x20;
		
		// size of the SInfoBlock struct and how much memory we have to store parameters
#if defined(PLATFORM_64BIT)
		static const unsigned int scSizeOfSJobQueueEntry			= 512;
		static const unsigned int scSizeOfJobQueueEntryHeader	= 88; // Please adjust when adding/removing members
		static const unsigned int scAvailParamSize						= scSizeOfSJobQueueEntry - scSizeOfJobQueueEntryHeader;
#else
		static const unsigned int scSizeOfSJobQueueEntry			= 256;
		static const unsigned int scSizeOfJobQueueEntryHeader	= 48; // Please adjust when adding/removing members
		static const unsigned int scAvailParamSize						= scSizeOfSJobQueueEntry - scSizeOfJobQueueEntryHeader;
#endif
		

		//parameter data are enclosed within to save a cache miss
		_MS_ALIGN(16) unsigned char paramData[scAvailParamSize] _ALIGN(16);	//is 16 byte aligned, make sure it is kept aligned
		
		ILINE void AssignMembersTo(SInfoBlock* const pDest) const
		{
			SVEC4_UINT *const __restrict pDst = (SVEC4_UINT *const __restrict)(pDest);
			SVEC4_UINT *const __restrict pSrc = (SVEC4_UINT *const __restrict)(this);
						
			pDest->callBackData = callBackData;
			pDest->jobInvoker = jobInvoker;		
			pDest->eaExtJobStateAddress = eaExtJobStateAddress;
			pDst[1] = pSrc[1];
			pDst[2] = pSrc[2];		

#if defined(PLATFORM_64BIT) // copy more data, since the block itself is bigger
			pDst[3] = pSrc[3];		
			pDst[4] = pSrc[4];		
#endif
		}

		ILINE unsigned char GetFirstPageIndex() const
		{
			return firstPageIndex;
		}

		ILINE void SetPageMode(const JobManager::SPUBackend::EPageMode cPageMode)
		{
			opMode = (opMode & ~JobManager::SPUBackend::PAGE_MODE_MASK) | (unsigned char)cPageMode;
		}

		ILINE JobManager::SPUBackend::EPageMode GetPageMode() const
		{
			return (JobManager::SPUBackend::EPageMode)(opMode & JobManager::SPUBackend::PAGE_MODE_MASK);
		}

		ILINE void SetPageMaxSize(const unsigned int cMaxPageSize)
		{
			assert(cMaxPageSize < 230*1024);
			assert((cMaxPageSize & 127) == 0);
			maxPageSize = (unsigned short)(cMaxPageSize / 128);//store in 128 byte amounts
		}

		ILINE unsigned int GetMaxPageSize() const
		{
			return (unsigned int)(maxPageSize * 128);
		}

		ILINE void SetCacheMode(const JobManager::SPUBackend::ECacheMode cCacheMode)
		{
			opMode = (opMode & ~JobManager::SPUBackend::CACHE_MODE_MASK) | (unsigned char)cCacheMode;
		}
				
		ILINE bool IsOffLoadJob() const 
		{
			return (opMode & JobManager::SPUBackend::eOffLoadJob) != 0;
		}
		ILINE void SetOpMode(const unsigned int cMode)
		{
			assert(cMode < 255);
			opMode = (unsigned char)cMode;
		}

		ILINE unsigned int GetMaxCacheSize() const
		{
			return ((unsigned int)(opMode & JobManager::SPUBackend::CACHE_MODE_MASK) >> JobManager::SPUBackend::CACHE_MODE_SIZE_SHIFT) << 12;//in KB
		}

		ILINE bool HasQueue() const
		{
			return (nflags & (unsigned char)scHasQueue) != 0;
		}
		

		ILINE void EnableDebug()
		{
			nflags |= scDebugEnabled;
		}

		ILINE int IsDebugEnabled() const
		{
			return (int)((nflags & (unsigned char)scDebugEnabled) != 0);
		}

		ILINE void SetDedicatedThreadOnly(const bool cDedicatedThreadOnly)
		{
			if(cDedicatedThreadOnly)
				nflags |= scDedicatedThreadOnly;
			else
				nflags &= ~scDedicatedThreadOnly;
		}

		ILINE bool IsDedicatedThreadOnly() const
		{
			return (nflags & (unsigned char)scDedicatedThreadOnly) != 0;
		}

		ILINE void SetTransferProfDataBack(const bool cTransfer)
		{
			if(cTransfer)
				nflags |= scTransferProfDataBack;
			else
				nflags &= ~scTransferProfDataBack;
		}

		ILINE bool TransferProfDataBack() const
		{
			return (nflags & (unsigned char)scTransferProfDataBack) != 0;
		}

		ILINE int KeepCache() const
		{
			return (nflags & (unsigned char)scKeepQueueCache);
		}
		
		ILINE void SetExtJobStateAddress(const SPU_DRIVER_INT_PTR cAddr)
		{
			assert(!HasQueue());
			eaExtJobStateAddress = cAddr;
		}

		ILINE SPU_DRIVER_INT_PTR GetExtJobStateAddress() const
		{
			assert(!HasQueue());
			return eaExtJobStateAddress;
		}

		ILINE SPU_DRIVER_INT_PTR GetQueue() const
		{
			assert(HasQueue());
			return eaExtJobStateAddress;
		}

		ILINE unsigned char* const __restrict GetParamAddress()
		{
			assert(!HasQueue());
			return &paramData[0];
		}

		// functions to access jobstate of SInfoBlock		
		ILINE void IsInUse(uint32 nRoundId, bool &rWait, bool &rRetry, uint32 nMaxValue) const { return jobState.IsInUse(nRoundId,rWait,rRetry,nMaxValue); }
		
		void Release(uint32 nMaxValue);
		void Wait(uint32 nRoundID,uint32 nMaxValue);

	} _ALIGN(128);

	//state information for the fill and empty pointers of the job queue
	//this is usable as shared memory with concurrent accesses from several SPUs at once
	struct SJobQueuePos
	{		
		unsigned int										lockObtained;																// keeps track if the consumer state is locked (and by which SPUid, only pull and on SPU only)		
		JobManager::SInfoBlock*					jobQueue[JobManager::eNumPriorityLevel];		// base of job queue per priority level, duplicated for more efficient SPU fetches
		JobManager::SJobQueueSlotState*	jobQueueStates[JobManager::eNumPriorityLevel];		// base of job queue per priority level, duplicated for more efficient SPU fetches
		volatile uint64									index;																			// index use for job slot publishing (is mapped into the job queue and checked by SPU), each priority level is encoded in this single value

		// util function to increase counter for their priority level only
		static const uint64 eBitsInIndex = sizeof(uint64) * CHAR_BIT;
		static const uint64 eBitsPerPriorityLevel = eBitsInIndex / JobManager::eNumPriorityLevel;
		static const uint64 eMaskStreamPriority		= (1ULL<<eBitsPerPriorityLevel)-1ULL;
		static const uint64 eMaskLowPriority			= (((1ULL<<eBitsPerPriorityLevel)<<eBitsPerPriorityLevel)-1ULL) & ~eMaskStreamPriority;
		static const uint64 eMaskRegularPriority	= ((((1ULL<<eBitsPerPriorityLevel)<<eBitsPerPriorityLevel)<<eBitsPerPriorityLevel)-1ULL) & ~(eMaskStreamPriority | eMaskLowPriority);
		static const uint64 eMaskHighPriority			= (((((1ULL<<eBitsPerPriorityLevel)<<eBitsPerPriorityLevel)<<eBitsPerPriorityLevel)<<eBitsPerPriorityLevel)-1ULL) & ~(eMaskRegularPriority | eMaskStreamPriority | eMaskLowPriority);
		
		ILINE static uint64 ExtractIndex( uint64 currentIndex, uint32 nPriorityLevel )
		{
			switch(nPriorityLevel)
			{
				case eHighPriority:			return ((((currentIndex & eMaskHighPriority) >> eBitsPerPriorityLevel) >> eBitsPerPriorityLevel)>> eBitsPerPriorityLevel);
				case eRegularPriority:	return (((currentIndex & eMaskRegularPriority) >> eBitsPerPriorityLevel)>> eBitsPerPriorityLevel);
				case eLowPriority:			return ((currentIndex & eMaskLowPriority)>> eBitsPerPriorityLevel);
				case eStreamPriority:		return (currentIndex & eMaskStreamPriority);
				default: return ~0;
			}
		}
		
		ILINE static bool IncreasePullIndex( uint64 currentPullIndex, uint64 currentPushIndex, uint64 &rNewPullIndex, uint32 &rPriorityLevel,
			uint32 nJobQueueSizeHighPriority, uint32 nJobQueueSizeRegularPriority, uint32 nJobQueueSizeLowPriority, uint32 nJobQueueSizeStreamPriority)
		{
			uint32 nPushExtractedHighPriority			= static_cast<uint32>(ExtractIndex(currentPushIndex,eHighPriority));
			uint32 nPushExtractedRegularPriority	= static_cast<uint32>(ExtractIndex(currentPushIndex,eRegularPriority));
			uint32 nPushExtractedLowPriority			= static_cast<uint32>(ExtractIndex(currentPushIndex,eLowPriority));
			uint32 nPushExtractedStreamPriority		= static_cast<uint32>(ExtractIndex(currentPushIndex,eStreamPriority));

			uint32 nPullExtractedHighPriority			= static_cast<uint32>(ExtractIndex(currentPullIndex,eHighPriority));
			uint32 nPullExtractedRegularPriority	= static_cast<uint32>(ExtractIndex(currentPullIndex,eRegularPriority));
			uint32 nPullExtractedLowPriority			= static_cast<uint32>(ExtractIndex(currentPullIndex,eLowPriority));
			uint32 nPullExtractedStreamPriority		= static_cast<uint32>(ExtractIndex(currentPullIndex,eStreamPriority));

			uint32 nRoundPushHighPriority				= nPushExtractedHighPriority / nJobQueueSizeHighPriority;
			uint32 nRoundPushRegularPriority		= nPushExtractedRegularPriority / nJobQueueSizeRegularPriority;
			uint32 nRoundPushLowPriority				= nPushExtractedLowPriority / nJobQueueSizeLowPriority;
			uint32 nRoundPushStreamPriority			= nPushExtractedStreamPriority / nJobQueueSizeStreamPriority;

			uint32 nRoundPullHighPriority				= nPullExtractedHighPriority / nJobQueueSizeHighPriority;
			uint32 nRoundPullRegularPriority		= nPullExtractedRegularPriority / nJobQueueSizeRegularPriority;
			uint32 nRoundPullLowPriority				= nPullExtractedLowPriority / nJobQueueSizeLowPriority;
			uint32 nRoundPullStreamPriority			= nPullExtractedStreamPriority / nJobQueueSizeStreamPriority;

			uint32 nPushJobSlotHighPriority			= nPushExtractedHighPriority & (nJobQueueSizeHighPriority-1);
			uint32 nPushJobSlotRegularPriority	= nPushExtractedRegularPriority & (nJobQueueSizeRegularPriority-1);
			uint32 nPushJobSlotLowPriority			= nPushExtractedLowPriority & (nJobQueueSizeLowPriority-1);
			uint32 nPushJobSlotStreamPriority		= nPushExtractedStreamPriority & (nJobQueueSizeStreamPriority-1);

			uint32 nPullJobSlotHighPriority			= nPullExtractedHighPriority & (nJobQueueSizeHighPriority-1);
			uint32 nPullJobSlotRegularPriority	= nPullExtractedRegularPriority & (nJobQueueSizeRegularPriority-1);
			uint32 nPullJobSlotLowPriority			= nPullExtractedLowPriority & (nJobQueueSizeLowPriority-1);
			uint32 nPullJobSlotStreamPriority		= nPullExtractedStreamPriority & (nJobQueueSizeStreamPriority-1);

			//NOTE: if this shows in the profiler, find a lockless variant of it
			if( ((nRoundPushHighPriority == nRoundPullHighPriority) && nPullJobSlotHighPriority < nPushJobSlotHighPriority) || 
					((nRoundPushHighPriority != nRoundPullHighPriority) && nPullJobSlotHighPriority >= nPushJobSlotHighPriority) )
			{				
				rPriorityLevel = eHighPriority;
				rNewPullIndex = IncreaseIndex(currentPullIndex,eHighPriority);
			}
			else if (	((nRoundPushRegularPriority == nRoundPullRegularPriority) && nPullJobSlotRegularPriority < nPushJobSlotRegularPriority) || 
								((nRoundPushRegularPriority != nRoundPullRegularPriority) && nPullJobSlotRegularPriority >= nPushJobSlotRegularPriority) )
			{				
				rPriorityLevel = eRegularPriority;
				rNewPullIndex = IncreaseIndex(currentPullIndex,eRegularPriority);
			}
			else if (	((nRoundPushLowPriority == nRoundPullLowPriority) && nPullJobSlotLowPriority < nPushJobSlotLowPriority ) || 
								((nRoundPushLowPriority != nRoundPullLowPriority) && nPullJobSlotLowPriority >= nPushJobSlotLowPriority ) )
			{
				rPriorityLevel = eLowPriority;
				rNewPullIndex = IncreaseIndex(currentPullIndex,eLowPriority);
			}
			else if (	((nRoundPushStreamPriority == nRoundPullStreamPriority) && nPullJobSlotStreamPriority < nPushJobSlotStreamPriority ) || 
								((nRoundPushStreamPriority != nRoundPullStreamPriority) && nPullJobSlotStreamPriority >= nPushJobSlotStreamPriority ) )
			{
				rPriorityLevel = eStreamPriority;
				rNewPullIndex = IncreaseIndex(currentPullIndex,eStreamPriority);
			}
			else
			{
				rNewPullIndex = ~0;
				return false;
			}

			return true;
		}

		static uint64 IncreasePushIndex( uint64 currentPushIndex, uint32 nPriorityLevel )
		{
			return IncreaseIndex(currentPushIndex,nPriorityLevel);
		}

		static uint64 IncreaseIndex( uint64 currentIndex, uint32 nPriorityLevel )
		{
			uint64 nIncrease = 1ULL;
			uint64 nMask = 0;
			switch(nPriorityLevel) // unrolled loop, high priority two shifts, regular one, else no shift
			{
				case eHighPriority:			
					nIncrease <<= eBitsPerPriorityLevel; 
					nIncrease <<= eBitsPerPriorityLevel; 
					nIncrease <<= eBitsPerPriorityLevel; 
					nMask = eMaskHighPriority;
					break;
				case eRegularPriority:						
					nIncrease <<= eBitsPerPriorityLevel; 
					nIncrease <<= eBitsPerPriorityLevel; 
					nMask = eMaskRegularPriority;
					break;
				case eLowPriority:
					nIncrease <<= eBitsPerPriorityLevel; 
					nMask = eMaskLowPriority;
					break;
				case eStreamPriority:
					nMask = eMaskStreamPriority;
					break;
			}
			
			// increase counter while preventing overflow
			uint64 nCurrentValue					= currentIndex & nMask;							// extract all bits for this priority only
			uint64 nCurrentValueCleared		= currentIndex & ~nMask;						// extract all bits of other priorities (they shouldn't change)
			nCurrentValue += nIncrease;																				// increase value by one (already at the right bit position)
			nCurrentValue &= nMask;																						// mask out again to handle overflow
			uint64 nNewCurrentValue = nCurrentValueCleared | nCurrentValue;		// add new value to other priorities
			return nNewCurrentValue;
		}
		
	} _ALIGN(16);

	//struct covers DMA data setup common to all jobs and packets
	class CCommonDMABase
	{
	public:
		CCommonDMABase();
		void SetJobParamData(void* pParamData);
		const void* GetJobParamData() const;
		uint16 GetProfilingDataIndex() const;
		// in case of persistent job objects, we have to reset the profiling data
		void ForceUpdateOfProfilingDataIndex();

	protected:
		void*	m_pParamData;					//parameter struct, not initialized to 0 for performance reason
		uint16 nProfilerIndex;			// index handle for Jobmanager Profiling Data
	};
		
	//delegation class for each job
	class CJobDelegator : public CCommonDMABase
	{
	public:	

		typedef volatile CJobBase* TDepJob;

		CJobDelegator();
		const JobManager::EAddJobRes RunJob( const unsigned int cOpMode, const JobManager::TJobHandle cJobHandle);
		void RegisterQueue(const void* const cpQueue, const bool cKeepCache = false);		
		const void* GetQueue() const;
		const bool KeepCache() const;		
		void RegisterCallback(void (*pCallbackFnct)(void*), void *pArg);
		void RegisterJobState(JobManager::SJobState* __restrict pJobState);
		
		const SCallback& RESTRICT_REFERENCE GetCallbackdata() const;		
		//return a copy since it will get overwritten
		void SetJobPerfStats(volatile NSPU::NDriver::SJobPerfStats* pPerfStats);		
		void SetParamDataSize(const unsigned int cParamDataSize);
		const unsigned int GetParamDataSize() const;
		void SetCurrentThreadId(const threadID cID);		
		const threadID GetCurrentThreadId() const;
		
		JobManager::SJobState*GetJobState() const;		
		void SetRunning();
								
		ILINE void SetDelegator( Invoker pGenericDelecator )
		{			
			m_pGenericDelecator = pGenericDelecator;		
		}

		Invoker GetGenericDelegator()
		{
			return m_pGenericDelecator;
		}

		unsigned int GetPriorityLevel() const { return m_nPrioritylevel; }
		bool IsBlocking() const { return m_bIsBlocking; };

		void SetPriorityLevel( unsigned int nPrioritylevel ) { m_nPrioritylevel = nPrioritylevel; }
		void SetBlocking() { m_bIsBlocking = true; }

		bool IsDedicatedThreadOnly() const { return m_bDedicatedThreadOnly; }
		void SetDedicatedThreadOnly() { m_bDedicatedThreadOnly = true; }

	protected:
		volatile NSPU::NDriver::SJobPerfStats* m_pJobPerfData;	//job performance data pointer
		JobManager::SJobState*m_pJobState;					// extern job state
		SCallback	m_Callbackdata;										// callback data
		const void*	m_pQueue;												// consumer/producer queue
		bool m_KeepCache;														// if true then the cache is kept between packets
		unsigned int m_nPrioritylevel;							// enum represent the priority level to use
		bool m_bIsBlocking;													// if true, then the job could block on a mutex/sleep
		bool m_bDedicatedThreadOnly;								// if true, can only be executed on Raw SPUs
		unsigned int		m_ParamDataSize;						// sizeof parameter struct 		
		threadID		m_CurThreadID;							// current thread id
		Invoker m_pGenericDelecator;
	};

	//base class for jobs
	class CJobBase
	{
	public:
		ILINE CJobBase() : m_OpMode(PAGE_MODE_DEFAULT | CACHE_MODE_DEFAULT)
		{
			m_pJobProgramData = 0;
		}

		ILINE void RegisterCallback(void (*pCallbackFnct)(void*), void *pArg) volatile
		{
			((CJobBase*)this)->m_JobDelegator.RegisterCallback(pCallbackFnct, pArg);
		}

		ILINE void RegisterJobState( JobManager::SJobState* __restrict pJobState) volatile
		{
			((CJobBase*)this)->m_JobDelegator.RegisterJobState(pJobState);
		}

		ILINE void RegisterQueue(const void* const cpQueue, const bool cKeepCache = false) volatile
		{
			((CJobBase*)this)->m_JobDelegator.RegisterQueue(cpQueue, cKeepCache);
		}
		ILINE const JobManager::EAddJobRes Run() volatile
		{
			return ((CJobBase*)this)->m_JobDelegator.RunJob(m_OpMode, m_pJobProgramData);
		}
		
		ILINE const JobManager::TJobHandle GetJobProgramData()
		{	
			return m_pJobProgramData;
		}

		ILINE const unsigned int GetOpMode() const
		{	
			return m_OpMode;
		}

		ILINE void SetCacheMode(const JobManager::SPUBackend::ECacheMode cCacheMode) volatile
		{	
			m_OpMode = (m_OpMode & (unsigned int)~JobManager::SPUBackend::CACHE_MODE_MASK) | (unsigned char)cCacheMode;
		}

		ILINE void SetCacheMode(const JobManager::SPUBackend::ECacheMode cCacheMode)
		{	
			m_OpMode = (m_OpMode & (unsigned int)~JobManager::SPUBackend::CACHE_MODE_MASK) | (unsigned char)cCacheMode;
		}
		
		ILINE void SetJobType(const JobManager::SPUBackend::EJobType cJobTope)
		{
			m_OpMode = (m_OpMode & ~JobManager::SPUBackend::JOBTYPE_MODE_MASK) | (unsigned char)cJobTope;
		}

		ILINE void SetCurrentThreadId(const threadID cID)
		{
			((CJobBase*)this)->m_JobDelegator.SetCurrentThreadId(cID);
		}

		ILINE const threadID GetCurrentThreadId() const
		{
			return ((CJobBase*)this)->m_JobDelegator.GetCurrentThreadId();
		}

		ILINE void SetJobPerfStats(volatile NSPU::NDriver::SJobPerfStats* pPerfStats) volatile
		{
			((CJobBase*)this)->m_JobDelegator.SetJobPerfStats(pPerfStats);
		}

		ILINE CJobDelegator* GetJobDelegator()
		{
			return &m_JobDelegator;
		}

	protected:
		CJobDelegator	m_JobDelegator;				//delegation implementation, all calls to job manager are going through it
		JobManager::TJobHandle				m_pJobProgramData;		//handle to program data to run
		unsigned int			m_OpMode;							//cache and code paging mode for the job

		//sets the job program data
		ILINE void SetJobProgramData(const JobManager::TJobHandle pJobProgramData, const JobManager::SPUBackend::ECacheMode cCacheMode = CACHE_MODE_DEFAULT)
		{
			assert(pJobProgramData != 0);
			m_pJobProgramData = pJobProgramData;
			m_OpMode					= cCacheMode;
		}

	};
		
} // namespace JobManager


// ==============================================================================
// Interface of the JobManager
// ==============================================================================

// Create a Producer consumer queue for a job type
#define PROD_CONS_QUEUE_TYPE(name, size) JobManager::CProdConsQueue<name, (size)>

// Declaration for the InvokeOnLinkedStacked util function
extern "C" 
{
	void InvokeOnLinkedStack(void (*proc)(void *), void *arg, void *stack, size_t stackSize);

} // extern "C"

namespace JobManager
{
	class IWorkerBackEndProfiler;

	// base class for the various backends the jobmanager supports
	struct IBackend
	{
		// <interfuscator:shuffle>
		virtual ~IBackend(){}

		virtual bool Init(uint32 nSysMaxWorker) = 0;
		virtual bool ShutDown() = 0;
		virtual void Update() = 0;

		virtual JobManager::EAddJobRes AddJob( JobManager::CJobDelegator& RESTRICT_REFERENCE crJob, const uint32 cOpMode, const JobManager::TJobHandle cJobHandle, JobManager::SInfoBlock &rInfoBlock ) = 0;
		virtual uint32 GetNumWorkerThreads() const = 0;
		// </interfuscator:shuffle>

#if defined(JOBMANAGER_SUPPORT_FRAMEPROFILER)
		virtual IWorkerBackEndProfiler* GetBackEndWorkerProfiler() const = 0;
#endif
	};

	// singleton managing the job queues and/for the SPUs
	UNIQUE_IFACE struct IJobManager
	{
		// <interfuscator:shuffle>
		// === front end interface ===
		virtual ~IJobManager(){}

		virtual void Init( JobManager::TSPUFreeFunc FreeFunc, JobManager::TSPUMallocFunc MallocFunc, bool bEnablePrintf, uint32 nSysMaxWorker ) = 0;

		//adds a job
		virtual const JobManager::EAddJobRes AddJob( JobManager::CJobDelegator& RESTRICT_REFERENCE crJob, const unsigned int cOpMode, const JobManager::TJobHandle cJobHandle ) = 0;

		//polls for a spu job (do not use is a callback has been registered)
		//returns false if a time out has occurred
		virtual const bool WaitForJob( JobManager::SJobState& rJobState, const int cTimeOutMS=-1) const = 0;

		//obtain job handle from name
		virtual const JobManager::TJobHandle GetJobHandle(const char* cpJobName, const unsigned int cStrLen, JobManager::Invoker pInvoker)= 0;
		virtual const JobManager::TJobHandle GetJobHandle(const char* cpJobName, JobManager::Invoker pInvoker) = 0;

		virtual void* Allocate(uint32 size, uint32 alignment = 8) = 0;
		virtual void Free(void*) = 0;
		
		//shuts down spu job manager
		virtual void ShutDown() = 0;
	
		virtual const uint32 GetAllocatedMemory() const = 0;
		
		virtual JobManager::IBackend* GetBackEnd(JobManager::EBackEndType backEndType) = 0;

		virtual bool InvokeAsJob(const char *cJobHandle) const =0;
		virtual bool InvokeAsJob(const JobManager::TJobHandle cJobHandle) const =0;
		
		virtual bool InvokeAsSPUJob(const char *cJobHandle) const =0;
		virtual bool InvokeAsSPUJob(const JobManager::TJobHandle cJobHandle) const =0;

		virtual void SetJobFilter( const char *pFilter ) = 0;
		virtual void SetJobSystemEnabled( int nEnable ) = 0;

		virtual uint32 GetWorkerThreadId() const = 0;
		
		virtual JobManager::SJobProfilingData* GetProfilingData(uint16 nProfilerIndex) = 0;
		virtual uint16 ReserveProfilingData() = 0;

		virtual void Update(int nJobSystemProfiler) = 0;
		virtual void PushProfilingMarker( const char *pName ) =0;
		virtual void PopProfilingMarker() =0;


		virtual uint32 GetNumWorkerThreads() const = 0;

		// get a free semaphore handle from the Job Manager pool
		virtual JobManager::TSemaphoreHandle AllocateSemaphore( volatile const void* pOwner ) = 0;

		// return a semaphore handle to the Job Manager pool
		virtual void DeallocateSemaphore( JobManager::TSemaphoreHandle nSemaphoreHandle, volatile const void* pOwner ) =0;

		// increase the refcounter of a semaphore, but only if it is > 0, else returns false
		virtual bool AddRefSemaphore( JobManager::TSemaphoreHandle nSemaphoreHandle, volatile const void* pOwner ) = 0;

		// get the actual semaphore object for aquire/release calls
		virtual SJobFinishedConditionVariable* GetSemaphore( JobManager::TSemaphoreHandle nSemaphoreHandle, volatile const void* pOwner ) =0;

		virtual void DumpJobList() = 0;

		
		virtual void EnableSuspendWorkerForMP() =0;
		virtual void DisableSuspendWorkerForMP() =0;

		// </interfuscator:shuffle>

#if !defined(__SPU__)
		virtual void SetFrameStartTime( const CTimeValue &rFrameStartTime ) = 0;
#endif
	};

	// specialized interface for PS3 with a SPU backend
	UNIQUE_IFACE struct ISPUBackend : public IBackend
	{		
#if defined(JOBMANAGER_SUPPORT_FRAMEPROFILER)
		JobManager::IWorkerBackEndProfiler* GetBackEndWorkerProfiler() const = 0;
#endif

		// <interfuscator:shuffle>
		virtual bool Init(uint32 nSysMaxWorker) =0;
		virtual void Update() =0;

		//returns number of SPUs allowed for job scheduling
		virtual const unsigned int GetSPUsAllowed() const = 0;

		//returns spu driver size, all data must be placed behind it
		virtual const unsigned int GetDriverSize() const  = 0;

		//initializes all allowed SPUs
		virtual const bool InitSPUs(const char* cpSPURepo, const int cSPUWorkerCount ) = 0;

		//print performance stats
		virtual void PrintPerfStats(const volatile NSPU::NDriver::SJobPerfStats* pPerfStats, const char* cpJobName) const = 0;
							
		//enables spu debugging for a particular job
		virtual void EnableSPUJobDebugging(const void*) = 0;
		//registers a variable to check if the profiling data should be transferred back
		virtual void RegisterProfileStatVar(int *pVar) = 0;
		//enables/disables spu profiling
		virtual void EnableSPUProfiling(const uint8=0) = 0;
	
		//obtains and resets the SPU function profiling stats of the last frame
		virtual void GetAndResetSPUFuncProfStats(const JobManager::SJobFrameStats*& rpCurFuncProfStatVec, uint32& rCount, const uint32 cThresholdUSecs=100) = 0;
			
		//initialize values required for spu-libgcm usage
		virtual void GcmInit
		(
			const uint32,
			void *const __restrict,
			const uint32, 
			CellGcmContextData *const __restrict, 
			const uint32,
			const uint32,
			const uint32
		) = 0;
		
		// set the text filer, if a job matches this filer, it is not executed in a thread
		virtual void SetJobFilter( const char *) =0;

		// returns true in case a SPU repository was loaded successful
		virtual bool HasValidSPURepository() const = 0;
		
		// suspends a shared worker thread, to allow other SPU threads to run
		virtual void SuspendSharedWorkerThread() =0;

		// resume execution of a previously suspended worker thread
		virtual void ResumeSharedWorkerThread() =0;

		// is the shared worker thread currently suspended
	  virtual bool IsSharedWorkerThreadSuspended() =0;

		// forces clearing of all per worker preallocated buckets
		virtual void ForceClearAllSPUBuckets() =0;

		// </interfuscator:shuffle>
	};
	
	/////////////////////////////////////////////////////////////////////////////
	ILINE ISPUBackend* GetSPUBackEnd()
	{
#if defined(PS3) && !defined(__SPU__) // do i need this on SPU?
		return gPS3Env->pSPUBackend;
#else
		snPause();
        return NULL;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	// util function to get the worker thread id in a job, returns 0xFFFFFFFF otherwise
	ILINE uint32 GetWorkerThreadId()
	{
#if defined(__SPU__)
		return __spu_get_current_worker_id();
#else
		uint32 nWorkerThreadID = gEnv->GetJobManager()->GetWorkerThreadId();
		return nWorkerThreadID == ~0 ? ~0 : (nWorkerThreadID & ~0x40000000);
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	// util function to find out if a call comes from the mainthread or from a worker thread
	ILINE bool IsWorkerThread()
	{
#if defined(__SPU__)
		return true; // on SPU we are always a worker thread
#else
		return gEnv->GetJobManager()->GetWorkerThreadId() != ~0;
#endif
	}
	
		/////////////////////////////////////////////////////////////////////////////
	// util function to find out if a call comes from the mainthread or from a worker thread
	ILINE bool IsBlockingWorkerThread()
	{
#if defined(__SPU__)
		return false; // on SPU we don't have blocking worker threads
#else
		return (gEnv->GetJobManager()->GetWorkerThreadId() != ~0) && ((gEnv->GetJobManager()->GetWorkerThreadId() & 0x40000000) != 0);
#endif
	}

	
	/////////////////////////////////////////////////////////////////////////////
	// util function to check if a specific job should really run as job
	ILINE bool InvokeAsJob( const char *pJobName )
	{
#if defined(__SPU__) || defined(_RELEASE)
		return true; // on SPU and in release alwasy assume to execute the job
#else
		return gEnv->pJobManager->InvokeAsJob(pJobName);
#endif
	}
	
	/////////////////////////////////////////////////////////////////////////////
	ILINE void SJobState::SetRunning( )
	{			
		SJobStateBase::SetRunning();
	}

#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
	inline void SJobState::SetStopped()
	{	
			__spu_set_job_state_stopped( (SJobSyncVariable*)this); // evil cast, but it works :)
	}
#elif !defined(__SPU__)
	/////////////////////////////////////////////////////////////////////////////
	// special function is provided for SPUs, living in SPUDriver_spu.cpp
	inline void SJobState::SetStopped()
	{		
		CJobBase *pFollowUpJob = m_pFollowUpJob;	// take this pointer early as the job state could be freed by another thread
		bool bStopped = SJobStateBase::SetStopped();

		// start post job if set
		if( pFollowUpJob != NULL && bStopped)		
			pFollowUpJob->Run();
	}
#endif

// ==============================================================================
// Interface of the Producer/Consumer Queue for JobManager
// ==============================================================================
	//	producer (1 PPU thread) - consumer queue (1 SPU)
	//
	//	- all implemented ILINE using a template:
	//		- ring buffer size (num of elements)
	//		- instance type of job
	//		- param type of job
	//	- Factory with macro instantiating the queue (knowing the exact names for a job)
	//	- queue consists of:
	//		- ring buffer consists of 1 instance of param type of job
	//		- for atomic test on finished and push pointer update, the queue is 128 byte aligned and push, pull ptr and 
	//				DMA job state are lying within that first 128 byte
	//		- volatile push (only modified by PPU) /pull (only modified by SPU) pointer, point to ring buffer, both equal in the beginning
	//		- job instance (create with def. ctor)	
	//		- DMA job state (running, finished)
	//		- AddPacket - method to add a packet
	//			- wait on current push/pull if any space is available
	//			- need atomically test if a SPU is running and update the push pointer, if SPU is finished, start new job
	//		- Finished method, returns push==pull
	//		- WaitForSPU method doing a poll with nops
	//
	//	PPU job manager side:
	//		- provide RegisterProdConsumerQueue - method, set flags accordingly
	//	SPU side: 
	//		- check if it has  a prod/consumer queue
	//		- if it is part of queue, it must obtain DMA address for job state and of push/pull (static offset to pointer)
	//		- a flag tells if job state is job state or queue (eaExtJobStateAddress becomes the queue address)
	//		- lock is obtained when current push/pull ptr is obtained, snooping therefore enabled
	//		- before FlushCacheComplete, get next parameter packet if multiple packets needs to be processed
	//		- if all packets were processed, try to write updated pull pointer and set finished state
	//				if it fails, push pointer was updated during processing time, in that case get lock again and with it the new push pointer
	//		- loop till the lock and finished state was updated successfully
	//		- no HandleCallback method, only 1 SPU is permitted to run with queue
	//		- no write back to external job state, always just one inner packet loop
	struct SProdConsQueueBase
	{
		// ---- don't move the state and push ptr to another place in this struct ----
		// ---- they are updated together with an atomic operation
		SJobSyncVariable		m_QueueRunningState;				// waiting state of the queue
		void* m_pPush;																			//push pointer, current ptr to push packets into (written by PPU)
		// ---------------------------------------------------------------------------

		volatile void* m_pPull;															//pull pointer, current ptr to pull packets from (written by SPU)
		uint32 m_PullIncrement _ALIGN(16);					//increment of pull, also accessed by SPU
		uint32 m_AddPacketDataOffset;								//offset of additional data relative to push ptr		
		SPU_DRIVER_INT_PTR m_RingBufferStart;									//start of ring buffer (to swap properly by SPU), also accessed by SPU
		SPU_DRIVER_INT_PTR m_RingBufferEnd;										//end of ring buffer (to swap properly by SPU), also accessed by SPU
		SJobFinishedConditionVariable*				m_pQueueFullSemaphore;			// Semaphore used for yield-waiting when the queue is full
		
	};

	// util functions namespace for the producer consumer queue
	/////////////////////////////////////////////////////////////////////////////////
	namespace ProdConsQueueBase
	{	
		inline SPU_DRIVER_INT_PTR MarkPullPtrThatWeAreWaitingOnASemaphore( SPU_DRIVER_INT_PTR nOrgValue )	{ assert( (nOrgValue&1) == 0 ); return nOrgValue | 1; }		
		inline bool IsPullPtrMarked( SPU_DRIVER_INT_PTR nOrgValue ) { return (nOrgValue & 1) == 1; }
	}

	/////////////////////////////////////////////////////////////////////////////////
	template <class TJobType, unsigned int Size>
	class CProdConsQueue : public SProdConsQueueBase
	{
	public:
		CProdConsQueue(const bool cKeepCache = false);																//default ctor
		~CProdConsQueue();
		
		template <class TAnotherJobType>
		void AddPacket
		(
			const typename TJobType::packet& crPacket, 
			const JobManager::SPUBackend::ECacheMode cCacheMode,
			JobManager::TPriorityLevel nPriorityLevel,
			TAnotherJobType* pJobParam,
			bool differentJob = true
		);	//adds a new parameter packet with different job type (job invocation)
		void AddPacket
		(
			const typename TJobType::packet& crPacket, 
			const JobManager::SPUBackend::ECacheMode cCacheMode = JobManager::SPUBackend::eCM_64,
			JobManager::TPriorityLevel nPriorityLevel = JobManager::eRegularPriority
		);	//adds a new parameter packet (job invocation)
		void WaitFinished();										//wait til all current jobs have been finished and been processed by a SPU
		bool IsEmpty();													//returns true if queue is empty
		uint32 PendingPackets() const;

#if defined(DO_SPU_PROFILING)
		const volatile NSPU::NDriver::SJobPerfStats& GetPerfStats();	//return current performance stats
#endif
	

	private:
		//initializes queue
		void Init(const unsigned int cPacketSize);		
		
		//get incremented ptr, takes care of wrapping
		void* const GetIncrementedPointer();
		
		void* m_pRingBuffer _ALIGN(128);																//the ring buffer
		TJobType m_JobInstance;															//job instance
		int	m_Initialized;
		int m_KeepCache;		

		static const unsigned int IDLE_NOPS = 32;				//number of nops performed in each wait loop iteration

		// compile tinme optinal profiling support
#if defined(DO_SPU_PROFILING)
		volatile NSPU::NDriver::SJobPerfStats g_PerfStatsJob;	//job specific performance stats location
#endif

	} _ALIGN(128);//align for DMA speed and cache line reservation


// ==============================================================================
// Fiber Interface
// ==============================================================================


namespace Fiber {
	// Thread compatible fiber method
	typedef void (*FiberFnc)(void*); 

	enum FiberTaskState
	{ 
		FIBERTASK_STARTED = 0x1, 
		FIBERTASK_JOINED = 0x2, 
		FIBERTASK_FINALIZED = 0x4,
		FIBERTASK_SUSPENDED = 0x8,
	}; 

	// Fiber Task structure definition. 
	// 
	// Note: The beginning of the stack for the fiber task is always located
	// right after the structure instance in memory (aligned to quadword
	// boundary). This means it is easy for a given fiber to find out where the
	// info structure is and for the task info to determine where the task's
	// stack is.
	struct FiberState 
	{ 
		uintptr_t m_ReturnToc;			// The toc address of the return address  
		uintptr_t m_ReturnLR;				// The return link register of the containing thread
		uintptr_t m_ReturnStack;		// The active stack link address of the containing thread
		uintptr_t m_ReturnIP;				// The return address this fiber will jump back to 
	}; 

	// A fiber task context 
	struct FiberTask 
	{ 
		uint64 m_Magic;						// Magic header to ensure that only prepared stacks are switched back and forth
		uint32 m_Flags;						// The state flags of the task fiber 
		uint32 m_CurSwitch;       // The current switch index for rdtsc recording 
		uint64 m_SwitchTimes[JobManager::Fiber::FIBERTASK_RECORD_SWITCHES];   // The rdtsc at the time of the stack switches
		uint64 m_TimeYielded;     // The accumulated cycles outside of the fiber (must be reset outside of jobmanager)
		uint64 m_TimeInFiber;     // The accumulated cycles outside of the fiber (must be reset outside of jobmanager)
		uint64 m_FiberYields;     // The accumulated number of fiber switches 
		FiberState m_State;         // The saved context state of the fiber prior to it's last switch  
	}; 

	// Creates a fiber task and executes it till the first yield 
	FiberTask* CreateFiber(FiberFnc pFnc, void* pArgument, size_t stackSize); 

	// Destroys the fiber and frees all allocated memory 
	void DestroyFiber(FiberTask* pFiber); 

	// Switch the fiber directly (possible from within the fiber or outside)
	void SwitchFiberDirect();

	// Yield from within a fiber task back to the containing thread
	void YieldFiber(); 

	// Execute the fiber until the fiber has completed. 
	void JoinFiber(FiberTask* pFiber); 

	// Jump into the fiber task 
	void SwitchToFiber(FiberTask* pFiber); 

	// Returns true if the current thread is within the fiber 
	bool IsInFiberThread();	

	// Returns the time outside of the current fiber (if present). If called
	// when currently within the fiber, it will return the number of cycles spent
	// outside of the fiber, else the number of cycles within the fiber. This
	// function can be used to obtain an accurate timing of cycles while
	// subtracting the amount of time spent in the 'other stack'. 
	uint64 FiberYieldTime(); 

	inline int64 GetNonFiberTicks()
	{
#if EMBED_PHYSICS_AS_FIBER
		return CryGetTicks() - FiberYieldTime(); 
#else
		return CryGetTicks();
#endif
	}

} // namespace JobManager::Fiber


#if !defined(__SPU__) 
// Interface to track BackEnd worker utilisation and job execution timings
class IWorkerBackEndProfiler
{
public:
	enum EJobSortOrder
	{
		eJobSortOrder_NoSort,
		eJobSortOrder_TimeHighToLow,
		eJobSortOrder_TimeLowToHigh,
		eJobSortOrder_Lexical,
	};

public:
	typedef DynArray<JobManager::SJobFrameStats> TJobFrameStatsContainer;

public:
	virtual ~IWorkerBackEndProfiler(){;}

	virtual void Init(const uint16 numWorkers) = 0;

	// Update the profiler at the beginning of the sample period
	virtual void Update() = 0;
	virtual void Update(const uint32 curTimeSample) = 0;

	// Register a job with the profiler
	virtual void RegisterJob(const uint32 jobId, const char* jobName) = 0;

	// Record execution information for a registered job
	virtual void RecordJob(const uint16 profileIndex, const uint8 workerId, const uint32 jobId, const uint32 runTimeMicroSec) = 0;

	// Get worker frame stats 
	virtual void GetFrameStats(JobManager::CWorkerFrameStats& rStats) const = 0;
	virtual void GetFrameStats(TJobFrameStatsContainer& rJobStats, EJobSortOrder jobSortOrder) const = 0;
	virtual void GetFrameStats(JobManager::CWorkerFrameStats& rStats, TJobFrameStatsContainer& rJobStats, EJobSortOrder jobSortOrder) const = 0;

	// Get worker frame stats summary
	virtual void GetFrameStatsSummary(SWorkerFrameStatsSummary& rStats) const = 0;
	virtual void GetFrameStatsSummary(SJobFrameStatsSummary& rStats) const = 0;

	// Returns the index of the active multi-buffered profile data
	virtual uint16 GetProfileIndex() const = 0;

	// Get the number of workers tracked
	virtual uint32 GetNumWorkers() const = 0;

public:
	// Returns a microsecond sample
	static uint32 GetTimeSample()
	{
#if defined(__SPU__)
		const float cInvTB	= 1000.f / 79800.f;//inverse ticks per usec
		return (unsigned int)((float)rdtsc() * cInvTB); // Convert ticks to microseconds: 1ms = 80000 ticks => 1us = 80 ticks
#else
		return static_cast<uint32>(gEnv->pTimer->GetAsyncTime().GetMicroSecondsAsInt64());
#endif
	}
};
#endif

}// namespace JobManager

// interface to create the JobManager since it is needed before CrySystem on PS3
// needed here for calls from Producer Consumer queue
extern "C" 
{
#if !defined(__SPU__)
	DLL_EXPORT JobManager::IJobManager* GetJobManSPUInterface();
#endif
}

// ==============================================================================
// Implementation of Producer Consumer functions
// currently adding jobs to a producer/consumer queue is not supported
// ==============================================================================
#if !defined(__SPU__)
template <class TJobType, unsigned int Size>
ILINE JobManager::CProdConsQueue<TJobType, Size>::~CProdConsQueue()
{
	if(m_pRingBuffer)
	gEnv->GetJobManager()->Free(m_pRingBuffer);
}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
ILINE JobManager::CProdConsQueue<TJobType, Size>::CProdConsQueue(const bool cKeepCache) : m_KeepCache(cKeepCache), m_Initialized(0), m_pRingBuffer(NULL)
{
	assert(Size > 2);
	m_JobInstance.RegisterQueue((void*)this, cKeepCache);
#if defined(DO_SPU_PROFILING)
	m_JobInstance.SetJobPerfStats(&g_PerfStatsJob);
#endif
}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
ILINE void JobManager::CProdConsQueue<TJobType, Size>::Init(const uint32 cPacketSize)
{
	ScopedSwitchToGlobalHeap heapSwitch;
 	assert((cPacketSize & 15) == 0);
 	m_AddPacketDataOffset = cPacketSize;	
 	m_PullIncrement				= m_AddPacketDataOffset + sizeof(SAddPacketData);
 	m_pRingBuffer =  gEnv->GetJobManager()->Allocate( Size * m_PullIncrement,128);
 	assert(m_pRingBuffer);
 	m_pPush = m_pRingBuffer;
	m_pPull = m_pRingBuffer;
 	m_RingBufferStart	= (SPU_DRIVER_INT_PTR)m_pRingBuffer;
 	m_RingBufferEnd		= m_RingBufferStart + Size * m_PullIncrement;
 	m_Initialized			= 1;
 	((TJobType*)&m_JobInstance)->SetParamDataSize(cPacketSize);
	m_pQueueFullSemaphore = NULL;
}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
ILINE uint32 JobManager::CProdConsQueue<TJobType, Size>::PendingPackets() const
{
	//non atomic retrieval
	const SPU_DRIVER_INT_PTR push = (SPU_DRIVER_INT_PTR)m_pPush;
	const SPU_DRIVER_INT_PTR pull = (SPU_DRIVER_INT_PTR)m_pPull;
	const SPU_DRIVER_INT_PTR pullIncr = m_PullIncrement;
	if(push > pull)
		return (push - pull) / pullIncr;
	if(push < pull)
		return ((SPU_DRIVER_INT_PTR)m_RingBufferEnd-pull + push - (SPU_DRIVER_INT_PTR)m_RingBufferStart) / pullIncr;
	return 0;	
}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
ILINE void JobManager::CProdConsQueue<TJobType, Size>::WaitFinished() 
{
	m_QueueRunningState.Wait();
	// ensure that the pull ptr is set right 
	assert(m_pPull == m_pPush);

}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
ILINE bool JobManager::CProdConsQueue<TJobType, Size>::IsEmpty() 
{
	return (SPU_DRIVER_INT_PTR)m_pPush == (SPU_DRIVER_INT_PTR)m_pPull;
}

///////////////////////////////////////////////////////////////////////////////
#if defined(DO_SPU_PROFILING)
template <class TJobType, unsigned int Size>
ILINE const volatile NSPU::NDriver::SJobPerfStats& JobManager::CProdConsQueue<TJobType, Size>::GetPerfStats()
{
	return g_PerfStatsJob;
}
#endif //DO_SPU_PROFILING

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
ILINE void* const JobManager::CProdConsQueue<TJobType, Size>::GetIncrementedPointer() 
{
	//returns branch free the incremented wrapped aware param pointer
	SPU_DRIVER_INT_PTR cNextPtr = (SPU_DRIVER_INT_PTR)m_pPush + m_PullIncrement;
#if defined(PLATFORM_64BIT)
	if((SPU_DRIVER_INT_PTR)cNextPtr >= (SPU_DRIVER_INT_PTR)m_RingBufferEnd)	cNextPtr = (SPU_DRIVER_INT_PTR)m_RingBufferStart;
	return (void*)cNextPtr;
#else
	const unsigned int cNextPtrMask = (unsigned int)(((int)(cNextPtr - m_RingBufferEnd)) >> 31);
	return (void*)(cNextPtr & cNextPtrMask | m_RingBufferStart & ~cNextPtrMask);
#endif
}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
inline void JobManager::CProdConsQueue<TJobType, Size>::AddPacket
(
  const typename TJobType::packet& crPacket, 
  const JobManager::SPUBackend::ECacheMode cCacheMode,
	JobManager::TPriorityLevel nPriorityLevel
)
{
	AddPacket<TJobType>(crPacket, cCacheMode, nPriorityLevel, (TJobType*)&m_JobInstance, false);
}

///////////////////////////////////////////////////////////////////////////////
template <class TJobType, unsigned int Size>
template <class TAnotherJobType>
inline void JobManager::CProdConsQueue<TJobType, Size>::AddPacket
(
	const typename TJobType::packet& crPacket, 
	const JobManager::SPUBackend::ECacheMode cCacheMode,
	JobManager::TPriorityLevel nPriorityLevel,
	TAnotherJobType* pJobParam,
	bool differentJob
)
{		
	const uint32 cPacketSize = crPacket.GetPacketSize();
	IF(m_Initialized == 0, 0)
		Init(cPacketSize);

	assert(m_RingBufferEnd == m_RingBufferStart + Size * (cPacketSize + sizeof(SAddPacketData)));

	const void* const cpCurPush = m_pPush;

	// don't overtake the pull ptr
	
	SJobSyncVariable curQueueRunningState = m_QueueRunningState;
	IF( (cpCurPush == m_pPull) && (curQueueRunningState.IsRunning()), 0)
	{		
			
		SPU_DRIVER_INT_PTR nPushPtr = (SPU_DRIVER_INT_PTR)cpCurPush;
		SPU_DRIVER_INT_PTR markedPushPtr = JobManager::ProdConsQueueBase::MarkPullPtrThatWeAreWaitingOnASemaphore(nPushPtr);
		TSemaphoreHandle nSemaphoreHandle = gEnv->pJobManager->AllocateSemaphore( this );
		m_pQueueFullSemaphore = gEnv->pJobManager->GetSemaphore( nSemaphoreHandle, this );
		
		bool bNeedSemaphoreWait = false;
#if defined(PLATFORM_64BIT) // for 64 bit, we need to atomicly swap 128 bit
		int64 compareValue[2] = { *alias_cast<int64*>(&curQueueRunningState),(int64)nPushPtr};		
		_InterlockedCompareExchange128((volatile int64*)this, (int64)markedPushPtr, *alias_cast<int64*>(&curQueueRunningState), compareValue);
		// make sure nobody set the state to stopped in the meantime
		bNeedSemaphoreWait = ( compareValue[0] == *alias_cast<int64*>(&curQueueRunningState) && compareValue[1] == (int64)nPushPtr ); 
#else
		// union used to construct 64 bit value for atomic updates
		union T64BitValue
		{
			int64 doubleWord;
			struct  
	{
				uint32 word0;
				uint32 word1;
			};
		};

		// job system is process the queue right now, we need an atomic update
		T64BitValue compareValue;
		compareValue.word0 = *alias_cast<uint32*>(&curQueueRunningState);
		compareValue.word1 = (uint32)nPushPtr;

		T64BitValue exchangeValue;
		exchangeValue.word0 = *alias_cast<uint32*>(&curQueueRunningState);
		exchangeValue.word1 = (uint32)markedPushPtr;

		T64BitValue resultValue;
		resultValue.doubleWord = CryInterlockedCompareExchange64((volatile int64*)this, exchangeValue.doubleWord, compareValue.doubleWord);

		bNeedSemaphoreWait = ( resultValue.word0 == *alias_cast<uint32*>(&curQueueRunningState) && resultValue.word1 == nPushPtr);
#endif
		
		// C-A-S successful, now we need to do a yield-wait
		IF( bNeedSemaphoreWait,0)
			m_pQueueFullSemaphore->Acquire();
		else
			m_pQueueFullSemaphore->SetStopped();

		gEnv->pJobManager->DeallocateSemaphore(nSemaphoreHandle, this );
		m_pQueueFullSemaphore = NULL;
	}

#if !defined(__SPU__)
	if(crPacket.GetJobStateAddress())
	{
		JobManager::SJobState *pJobState = reinterpret_cast<JobManager::SJobState*>(crPacket.GetJobStateAddress());
		pJobState->SetRunning();
	}
#endif

	//get incremented push pointer and check if there is a slot to push it into
	void* const cpNextPushPtr = GetIncrementedPointer();

	const SVEC4_UINT * __restrict pPacketCont = crPacket.GetPacketCont();
	SVEC4_UINT * __restrict pPushCont				 = (SVEC4_UINT*)cpCurPush;

	//copy packet data
	const uint32 cIters = cPacketSize >> 4;
	for(uint32 i=0;i<cIters; ++i)
		pPushCont[i] = pPacketCont[i];


	// setup addpacket data for SPUs
	SAddPacketData* const __restrict pAddPacketData = (SAddPacketData*)((unsigned char*)cpCurPush + m_AddPacketDataOffset);
	pAddPacketData->SetCacheMode(cCacheMode);
	pAddPacketData->jobEA = 0;
	pAddPacketData->eaJobState = crPacket.GetJobStateAddress();

#if defined(JOBMANAGER_SUPPORT_PROFILING)
#if defined(__SPU__) // spawning packet jobs on SPU is not supported right now, thus no profiling needed for packets also
	pAddPacketData->profilerIndex = ~0;
#else
	SJobProfilingData *pJobProfilingData = gEnv->GetJobManager()->GetProfilingData(crPacket.GetProfilerIndex());
	pJobProfilingData->jobHandle = pJobParam->GetJobProgramData();
	pAddPacketData->profilerIndex = crPacket.GetProfilerIndex();	
	if(pAddPacketData->eaJobState) // also store profilerindex in syncvar, so be able to record wait times
	{
		((JobManager::SJobState*)pAddPacketData->eaJobState)->nProfilerIndex = pAddPacketData->profilerIndex;
	}
#endif
#endif

	// used for non-spu backends during job switching
	pAddPacketData->nInvokerIndex = pJobParam->GetProgramHandle()->nJobInvokerIdx;

	if(differentJob)
	{
		SJob *pJob = NULL; 
		TJobHandle handle = pJobParam->GetProgramHandle(); 
		uint32 jobHandle = pJobParam->GetProgramHandle()->jobHandle; 

		pAddPacketData->jobEA =  pJobParam->GetJobAddress();
		pJob	= (SJob*)(EXPAND_PTR)jobHandle;
		pAddPacketData->binJobEA	= (SPU_DRIVER_INT_PTR)pJob;
		if (IsValidJobHandle(handle))
			pAddPacketData->SetPageMode((JobManager::SPUBackend::EPageMode)pJob->GetPageMode());
		
	}

	// new job queue, or empty job queue
	if( !m_QueueRunningState.IsRunning() )
	{
		m_pPush = cpNextPushPtr;//make visible
		m_QueueRunningState.SetRunning();

		pJobParam->RegisterQueue((void*)this, m_KeepCache!=0);
		pJobParam->SetParamDataSize(((TJobType*)&m_JobInstance)->GetParamDataSize());
		pJobParam->SetCacheMode(cCacheMode);
		if(crPacket.IsDedicatedThreadOnly())
			pJobParam->SetDedicatedThreadOnly();
		pJobParam->SetPriorityLevel(nPriorityLevel);
		pJobParam->Run(); 
		
		return;
	}

	bool bAtomicSwapSuccessfull = false;  
	JobManager::SJobSyncVariable newSyncVar; newSyncVar.SetRunning(); 
#if defined(PLATFORM_64BIT) // for 64 bit, we need to atomicly swap 128 bit
	int64 compareValue[2] = { *alias_cast<int64*>(&newSyncVar),(int64)cpCurPush};		
	_InterlockedCompareExchange128((volatile int64*)this, (int64)cpNextPushPtr, *alias_cast<int64*>(&newSyncVar), compareValue);
	// make sure nobody set the state to stopped in the meantime
	bAtomicSwapSuccessfull = ( compareValue[0] > 0 ); 
#else
	// union used to construct 64 bit value for atomic updates
	union T64BitValue
	{
		int64 doubleWord;
		struct  
		{
			uint32 word0;
			uint32 word1;
		};
	};

	// job system is process the queue right now, we need an atomic update
	T64BitValue compareValue;
	compareValue.word0 = *alias_cast<uint32*>(&newSyncVar);
	compareValue.word1 = (uint32)(TRUNCATE_PTR)cpCurPush;

	T64BitValue exchangeValue;
	exchangeValue.word0 = *alias_cast<uint32*>(&newSyncVar);
	exchangeValue.word1 = (uint32)(TRUNCATE_PTR)cpNextPushPtr;

	T64BitValue resultValue;
	resultValue.doubleWord = CryInterlockedCompareExchange64((volatile int64*)this, exchangeValue.doubleWord, compareValue.doubleWord);

	bAtomicSwapSuccessfull = ( resultValue.word0 > 0 );
#endif

	// job system finished in the meantime, next to issue a new job
	if( !bAtomicSwapSuccessfull )
	{
		m_pPush = cpNextPushPtr;//make visible
		m_QueueRunningState.SetRunning();

		pJobParam->RegisterQueue((void*)this, m_KeepCache != 0);
		pJobParam->SetParamDataSize(((TJobType*)&m_JobInstance)->GetParamDataSize());
		pJobParam->SetCacheMode(cCacheMode);
		if(crPacket.IsDedicatedThreadOnly())
			pJobParam->SetDedicatedThreadOnly();
		pJobParam->SetPriorityLevel(nPriorityLevel);
		pJobParam->Run(); 		
	}
}
#endif

// ==============================================================================
// CCommonDMABase Function implementations
// ==============================================================================
#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
extern uint16 ReserveProfilingDataOnSPU();
#endif

inline JobManager::CCommonDMABase::CCommonDMABase()
{
	ForceUpdateOfProfilingDataIndex();
}

///////////////////////////////////////////////////////////////////////////////
inline void JobManager::CCommonDMABase::ForceUpdateOfProfilingDataIndex()
{
#if defined(JOBMANAGER_SUPPORT_PROFILING)
#if defined(__SPU__)	&& !defined(_SPU_DRIVER_PERSISTENT)
	nProfilerIndex = ReserveProfilingDataOnSPU();
#elif	!defined(__SPU__)
	nProfilerIndex = gEnv->GetJobManager()->ReserveProfilingData();
#endif
#endif
}

///////////////////////////////////////////////////////////////////////////////
inline void JobManager::CCommonDMABase::SetJobParamData(void* pParamData)
{
	m_pParamData = pParamData;
}

///////////////////////////////////////////////////////////////////////////////
inline const void* JobManager::CCommonDMABase::GetJobParamData() const
{
	return m_pParamData;
}
		
///////////////////////////////////////////////////////////////////////////////
inline uint16 JobManager::CCommonDMABase::GetProfilingDataIndex() const
{
	return nProfilerIndex;
}

// ==============================================================================
// Job Delegator Function implementations
// ==============================================================================
ILINE JobManager::CJobDelegator::CJobDelegator() : m_ParamDataSize(0), m_CurThreadID(threadID(JobManager::SInfoBlock::scNoThreadId))
{
	m_pJobPerfData = NULL;
	m_pQueue = NULL;
	m_KeepCache = false;
	m_pJobState = NULL;
	m_nPrioritylevel = JobManager::eRegularPriority;
	m_bIsBlocking =false;
	m_bDedicatedThreadOnly = false;
}

#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
extern void AddJobtoJobQueue(JobManager::CJobDelegator& crJob, const unsigned int cOpMode, const JobManager::TJobHandle cJobHandle);
#endif

///////////////////////////////////////////////////////////////////////////////
ILINE const JobManager::EAddJobRes JobManager::CJobDelegator::RunJob( const unsigned int cOpMode,	const JobManager::TJobHandle cJobHandle )
{	
#if defined(__SPU__) && !defined(_SPU_DRIVER_PERSISTENT)
	AddJobtoJobQueue( (CJobDelegator&)*this, cOpMode, cJobHandle );
	return eAJR_Success;
#elif !defined(__SPU__)
	JobManager::IJobManager *const __restrict pJobMan = gEnv->GetJobManager();		
	const JobManager::EAddJobRes cRes =  pJobMan->AddJob((CJobDelegator& RESTRICT_REFERENCE)*this, cOpMode, cJobHandle );			

#if defined(USE_JOB_QUEUE_VERIFICATION)
	OutputJobError(cRes, cJobHandle->cpString, cJobHandle->strLen);
#endif
	return cRes;
#endif

}

///////////////////////////////////////////////////////////////////////////////
ILINE void JobManager::CJobDelegator::RegisterQueue(const void* const cpQueue, const bool cKeepCache)
{
	m_pQueue = cpQueue;
	m_KeepCache = cKeepCache;
}

///////////////////////////////////////////////////////////////////////////////
ILINE const void* JobManager::CJobDelegator::GetQueue() const
{
	return m_pQueue;
}

///////////////////////////////////////////////////////////////////////////////
ILINE const bool JobManager::CJobDelegator::KeepCache() const
{
	return m_KeepCache;
}

///////////////////////////////////////////////////////////////////////////////
ILINE void JobManager::CJobDelegator::RegisterCallback(void (*pCallbackFnct)(void*), void *pArg)
{
	m_Callbackdata.pCallbackFnct = pCallbackFnct;
	m_Callbackdata.pArg = pArg;
}

///////////////////////////////////////////////////////////////////////////////
ILINE void JobManager::CJobDelegator::RegisterJobState( JobManager::SJobState* __restrict pJobState)
{
	assert(pJobState);
	m_pJobState = pJobState;
#if defined(JOBMANAGER_SUPPORT_PROFILING)
	pJobState->nProfilerIndex = this->GetProfilingDataIndex();
#endif
}

///////////////////////////////////////////////////////////////////////////////
ILINE const JobManager::SCallback& RESTRICT_REFERENCE JobManager::CJobDelegator::GetCallbackdata() const
{
	return m_Callbackdata;
}

//return a copy since it will get overwritten
ILINE void JobManager::CJobDelegator::SetJobPerfStats(volatile NSPU::NDriver::SJobPerfStats* pPerfStats)
{
	m_pJobPerfData = pPerfStats;
}

///////////////////////////////////////////////////////////////////////////////
ILINE void JobManager::CJobDelegator::SetParamDataSize(const unsigned int cParamDataSize)
{
	m_ParamDataSize = cParamDataSize;
}

///////////////////////////////////////////////////////////////////////////////
ILINE const unsigned int JobManager::CJobDelegator::GetParamDataSize() const
{
	return m_ParamDataSize;
}

///////////////////////////////////////////////////////////////////////////////
ILINE void JobManager::CJobDelegator::SetCurrentThreadId(const threadID cID)
{
	m_CurThreadID = cID;
}

///////////////////////////////////////////////////////////////////////////////
ILINE const threadID JobManager::CJobDelegator::GetCurrentThreadId() const
{
	return m_CurThreadID;
}

///////////////////////////////////////////////////////////////////////////////
ILINE JobManager::SJobState* JobManager::CJobDelegator::GetJobState() const
{
	return m_pJobState;
}

///////////////////////////////////////////////////////////////////////////////
ILINE void JobManager::CJobDelegator::SetRunning()
{
	m_pJobState->SetRunning();
}


// ==============================================================================
// Implementation of SJObSyncVariable functions
// ==============================================================================
#if !defined(_SPU_DRIVER_PERSISTENT) // no constructor for SPU driver, to spare code size
inline JobManager::SJobSyncVariable::SJobSyncVariable()  
{ 
	syncVar.wordValue = 0; 
#if defined(PLATFORM_64BIT)
	padding[0] = padding[1] = padding[2] = padding[3]  = 0;
#endif
}
#endif

/////////////////////////////////////////////////////////////////////////////////
inline bool JobManager::SJobSyncVariable::IsRunning() const volatile		
{
	return syncVar.nRunningCounter != 0;
}

// provide a special path for SPUs, since they use a different atomic operations
// and require a interrupt thread call to unlock the semaphore
// the SPU version is living in SPUDriver_spu.cpp due to dependency issues
#if !defined(__SPU__)
/////////////////////////////////////////////////////////////////////////////////
inline void JobManager::SJobSyncVariable::Wait() volatile
{
#if defined(__SPU__)
	snPause();
#else
	if( syncVar.wordValue == 0 )
		return;
				
	IJobManager *pJobManager = gEnv->GetJobManager();

	SyncVar currentValue;
	SyncVar newValue;
	SyncVar resValue;
	TSemaphoreHandle semaphoreHandle = gEnv->GetJobManager()->AllocateSemaphore( this );	

retry:
	// volatile read
	currentValue.wordValue = syncVar.wordValue; 

	if( currentValue.wordValue != 0 )
	{
		// there is already a semaphore, wait for this one
		if(currentValue.semaphoreHandle)
		{
			// prevent the following race condition: Thread A Gets the semaphore and waits, then Thread B fails the earlier C-A-S
			// thus will wait for the same semaphore, but Thread A already release the semaphore and now another Thread (could even be A again)
			// is using the same semaphore to wait for the job (executed by thread B) which waits for the samephore -> Deadlock
			// this works since the jobmanager semphore function internally are using locks
			// so if the following line succeeds, we got the lock before the release and have increased the use counter
			// if not, it means that thread A release the semaphore, which also means thread B doesn't have to wait anymore

			if(pJobManager->AddRefSemaphore( currentValue.semaphoreHandle, this ) )
			{			
#if EMBED_PHYSICS_AS_FIBER
				JobManager::Fiber::SwitchFiberDirect();
#endif
				pJobManager->GetSemaphore( currentValue.semaphoreHandle, this )->Acquire();
				pJobManager->DeallocateSemaphore( currentValue.semaphoreHandle, this );
			}
		}
		else // no semaphore found
		{
			newValue = currentValue;
			newValue.semaphoreHandle = semaphoreHandle;
            resValue.wordValue = CryInterlockedCompareExchange((volatile LONG*)&syncVar.wordValue,newValue.wordValue,currentValue.wordValue);
            
			// four case are now possible:
			//a) job has finished -> we only need to free our semaphore
			//b) we succeeded setting our semaphore, thus wait for it
			//c) another waiter has set it's sempahore wait for the other one
			//d) another thread has increased/decreased the num runner variable, we need to do the run again

			if(resValue.wordValue != 0 ) // case a), do nothing
			{
				if(resValue.wordValue == currentValue.wordValue) // case b)
				{
#if EMBED_PHYSICS_AS_FIBER
					JobManager::Fiber::SwitchFiberDirect();
#endif
					pJobManager->GetSemaphore( semaphoreHandle, this )->Acquire();
				}
				else // C-A-S not succeeded, check how to proceed
				{
					if(resValue.semaphoreHandle) // case c)
					{
						// prevent the following race condition: Thread A Gets the semaphore and waits, then Thread B fails the earlier C-A-S
						// thus will wait for the same semaphore, but Thread A already release the semaphore and now another Thread (could even be A again)
						// is using the same semaphore to wait for the job (executed by thread B) which waits for the samephore -> Deadlock
						// this works since the jobmanager semphore function internally are using locks
						// so if the following line succeeds, we got the lock before the release and have increased the use counter
						// if not, it means that thread A release the semaphore, which also means thread B doesn't have to wait anymore

						if(pJobManager->AddRefSemaphore( resValue.semaphoreHandle, this ) )
						{
#if EMBED_PHYSICS_AS_FIBER
							JobManager::Fiber::SwitchFiberDirect();
#endif
							pJobManager->GetSemaphore( resValue.semaphoreHandle, this )->Acquire();					
							pJobManager->DeallocateSemaphore( resValue.semaphoreHandle, this );
						}
					}
					else // case d
					{
						goto retry;
					}
				}
			}
		}
	}

	// mark an old semaphore as stopped, in case we didn't use use	(if we did use it, this is a nop)		
	pJobManager->GetSemaphore( semaphoreHandle, this )->SetStopped(); 	
	pJobManager->DeallocateSemaphore( semaphoreHandle, this );
#endif
}

/////////////////////////////////////////////////////////////////////////////////
inline void JobManager::SJobSyncVariable::SetRunning() volatile
{
	SyncVar currentValue;
	SyncVar newValue;	
		
	do
	{
		// volatile read
		currentValue.wordValue = syncVar.wordValue; 

		newValue = currentValue;
		newValue.nRunningCounter += 1;
	} while( CryInterlockedCompareExchange((volatile LONG*)&syncVar.wordValue,newValue.wordValue,currentValue.wordValue) != currentValue.wordValue );
}

/////////////////////////////////////////////////////////////////////////////////
inline bool JobManager::SJobSyncVariable::SetStopped() volatile
{	
	SyncVar currentValue;
	SyncVar newValue;
	SyncVar resValue;

	// first use atomic operations to decrease the running counter
	do
	{
		// volatile read
		currentValue.wordValue = syncVar.wordValue; 

		newValue = currentValue;
		assert(newValue.nRunningCounter > 0 );
		newValue.nRunningCounter -= 1;

        resValue.wordValue = CryInterlockedCompareExchange((volatile LONG*)&syncVar.wordValue,newValue.wordValue,currentValue.wordValue);

	} while(  resValue.wordValue != currentValue.wordValue );

	// we now successfull decreased our counter, check if we need to signal a semaphore
	if(newValue.nRunningCounter > 0 ) // return if we still have other jobs on this syncvar
		return false;

	// do we need to release a semaphore
	if(currentValue.semaphoreHandle)
	{
		// try to set the running counter to 0 atomically
		bool bNeedSemaphoreRelease = true;
		do
		{
			// volatile read
			currentValue.wordValue = syncVar.wordValue;
			newValue = currentValue;			
			newValue.semaphoreHandle = 0;

			// another thread increased the running counter again, don't call semaphore release in this case
			if(currentValue.nRunningCounter)
				return false;

		}while( CryInterlockedCompareExchange((volatile LONG*)&syncVar.wordValue,newValue.wordValue,currentValue.wordValue) != currentValue.wordValue);
		// set the running successfull to 0, now we can release the semaphore
		gEnv->GetJobManager()->GetSemaphore(resValue.semaphoreHandle, this)->Release();
	}

	return true;
}
#endif
		
#if !defined(__SPU__)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// SPU have their private implemantion in the driver/jobAPi_SPU.cpp
// SInfoBlock State functions


/////////////////////////////////////////////////////////////////////////////////	
inline void JobManager::SInfoBlock::Release(uint32 nMaxValue)
{	
	JobManager::IJobManager *pJobManager = gEnv->GetJobManager();

	SInfoBlockState currentInfoBlockState;
	SInfoBlockState newInfoBlockState;
	SInfoBlockState resultInfoBlockState;

	// use atomic operations to set the running flag
	do
	{
		// volatile read
		currentInfoBlockState.nValue = *(const_cast<volatile LONG*>(&jobState.nValue)); 
		
		newInfoBlockState.nSemaphoreHandle = 0;		
		newInfoBlockState.nRoundID = (currentInfoBlockState.nRoundID + 1) & 0x7FFF;		
		newInfoBlockState.nRoundID = newInfoBlockState.nRoundID >= nMaxValue ? 0 : newInfoBlockState.nRoundID;

        resultInfoBlockState.nValue = CryInterlockedCompareExchange((volatile LONG*)&jobState.nValue, newInfoBlockState.nValue, currentInfoBlockState.nValue);
	} while( resultInfoBlockState.nValue != currentInfoBlockState.nValue );

	// do we need to release a semaphore
	if(currentInfoBlockState.nSemaphoreHandle)	
		gEnv->GetJobManager()->GetSemaphore(currentInfoBlockState.nSemaphoreHandle, this)->Release();
	
}

/////////////////////////////////////////////////////////////////////////////////	
inline void JobManager::SInfoBlock::Wait(uint32 nRoundID,uint32 nMaxValue)
{		
	bool bWait = false;
	bool bRetry = false;
	jobState.IsInUse(nRoundID,bWait,bRetry,nMaxValue);
	if( bWait == false )
		return;
	
	JobManager::IJobManager *pJobManager = gEnv->GetJobManager();
	
	// get a semaphore to wait on
	TSemaphoreHandle semaphoreHandle = pJobManager->AllocateSemaphore( this );

	// try to set semaphore (info block could have finished in the meantime, or another thread has set the semaphore
	SInfoBlockState currentInfoBlockState;
	SInfoBlockState newInfoBlockState;
	SInfoBlockState resultInfoBlockState;
	
	currentInfoBlockState.nValue = *(const_cast<volatile LONG*>(&jobState.nValue));

	// make sure the 
	currentInfoBlockState.IsInUse(nRoundID,bWait,bRetry,nMaxValue);
	if( bWait == true )
	{
		// is a semaphore already set
		if(currentInfoBlockState.nSemaphoreHandle != 0)
		{
			if(pJobManager->AddRefSemaphore( currentInfoBlockState.nSemaphoreHandle, this ) )
			{			
				pJobManager->GetSemaphore(currentInfoBlockState.nSemaphoreHandle, this )->Acquire();
				pJobManager->DeallocateSemaphore( currentInfoBlockState.nSemaphoreHandle, this );
			}
		}
		else
		{
			// try to set the semaphore
			newInfoBlockState.nRoundID = currentInfoBlockState.nRoundID;
			newInfoBlockState.nSemaphoreHandle = semaphoreHandle;

			resultInfoBlockState.nValue = CryInterlockedCompareExchange((volatile LONG*)&jobState.nValue, newInfoBlockState.nValue, currentInfoBlockState.nValue);

				
			// three case are now possible:
			//a) job has finished -> we only need to free our semaphore
			//b) we succeeded setting our semaphore, thus wait for it
			//c) another waiter has set it's sempahore wait for the other one		
			resultInfoBlockState.IsInUse(nRoundID,bWait,bRetry,nMaxValue);
			if( bWait == true ) // case a) do nothing
			{
				if(resultInfoBlockState.nValue == currentInfoBlockState.nValue) // case b)
				{
					pJobManager->GetSemaphore( semaphoreHandle, this )->Acquire();
				}
				else // case c)
				{
					if(pJobManager->AddRefSemaphore( resultInfoBlockState.nSemaphoreHandle, this ) )
					{			
						pJobManager->GetSemaphore( resultInfoBlockState.nSemaphoreHandle, this )->Acquire();
						pJobManager->DeallocateSemaphore( resultInfoBlockState.nSemaphoreHandle, this );
					}
				}
			}						
		}
	}
	
	pJobManager->GetSemaphore( semaphoreHandle, this )->SetStopped(); 	
	pJobManager->DeallocateSemaphore( semaphoreHandle, this );
}
#endif

#endif //__IJOBMAN_SPU_H
