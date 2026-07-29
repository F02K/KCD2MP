#ifndef _JOBMANAGER_SPUDRIVER_H_
#define _JOBMANAGER_SPUDRIVER_H_

// this is the central configuration file for the jobmanager, normally these defines/enums should live in IJobManager.h,
// but due includes from SPU compiled code as well as includes from ASM files, these were extraced into this header

#if !defined(_ALIGN)
// needed for SPU compilation (should be removed as soon as the spus doesn't depend on this header anymore
#if defined(_MSC_VER)
	#define _ALIGN(x)
#else
	#define _ALIGN(x) __attribute__((aligned(x)))
#endif
#endif

#if !defined(_MS_ALIGN)
	#define _MS_ALIGN(x)
#endif
// ==============================================================================
// Job manager settings
// ==============================================================================
//enable to obtain stats of spu usage each frame
#define JOBMANAGER_SUPPORT_FRAMEPROFILER

// collect per job informations about dispatch, start, stop and sync times
#if ! defined(DEDICATED_SERVER) && !defined(PS3)
#define JOBMANAGER_SUPPORT_PROFILING
#endif

// store the latest fnresolv, stackframe and dma access(disabled since it turned out to not be so useful)
//#define JOBMANAGER_STORE_DEBUG_HELP_INFORMATIONS

// support capturing SPU-Threads in tuner.
#define JOBMANAGER_SUPPORT_TUNER_CAPTURES

// in release disable features which cost performance
#if defined(_RELEASE)
#undef JOBMANAGER_SUPPORT_FRAMEPROFILER
#undef JOBMANAGER_SUPPORT_PROFILING
#undef JOBMANAGER_STORE_DEBUG_HELP_INFORMATIONS

#if !defined(PERFORMANCE_BUILD) // keep tuner SPU captures for performance builds
	#undef JOBMANAGER_SUPPORT_TUNER_CAPTURES
#endif
#endif


// ==============================================================================
// MessageIDs for events send by SPU which request PPU work
// ==============================================================================
#define JOBMANAGER_SPU_REQUEST_CONDITION_NOTIFY 1
#define JOBMANAGER_SPU_REQUEST_CONDITION_DESTROY 2
#define JOBMANAGER_SPU_REQUEST_JOBSTATE_SET_STOPPED 3
#define JOBMANAGER_SPU_REQUEST_JOBSTATE_SET_STOPPED_NO_DEC 4
#define JOBMANAGER_SPU_REQUEST_RELEASE_JOB_FINISHED_STATE 5
#define JOBMANAGER_SPU_REQUEST_CUSTOM_CALLBACK 6
#define JOBMANAGER_SPU_REQUEST_MEMORY_HANDLING 7
#define JOBMANAGER_SPU_REQUEST_MEMORY_CLEANUP 8
#define JOBMANAGER_SPU_REQUEST_ACQUIRE_SEMAPHORE 9
#define JOBMANAGER_SPU_REQUEST_RELEASE_SEMAPHORE 10
#define JOBMANAGER_SPU_REQUEST_ACQUIRE_FAST_SEMAPHORE 11
#define JOBMANAGER_SPU_REQUEST_RELEASE_FAST_SEMAPHORE 12
#define JOBMANAGER_SPU_REQUEST_SET_EVENT 13
#define JOBMANAGER_SPU_REQUEST_CLEAR_BUCKET_ALLOCATOR 14

// ==============================================================================
// Constant addresses used for SPU/PPU communication over local store
// uses the padding of the SPU GUID, so we can use the address range of [0x10-0x7c]
// ==============================================================================
#define JOBMANAGER_LS_PROFILING_TRACE_BUFFER_FIELD 0x10
#define JOBMANAGER_LS_PROFILING_TRACE_BUFFER_SIZE_FIELD 0x14
#define JOBMANAGER_LS_PROFILING_TRACE_BUFFER_CONTINOUS_FIELD 0x18
#define JOBMANAGER_LS_PROFILING_TRACE_RUNNING_FIELD 0x1c
#define JOBMANAGER_LS_PRINTF_WORKER_ID_ADDRESS_FIELD 0x20
#define JOBMANAGER_LS_PRINTF_INFOBLOCK_ADDRESS_FIELD 0x24
#define JOBMANAGER_LS_JOB_IS_PROCESSING 0x28
#define JOBMANAGER_LS_JOB_WAS_SUSPENDED_FIELD 0x2c
#define JOBMANAGER_LS_FRAMEPROFILER_CURRENT_FRAME_INDEX 0x30
#define JOBMANAGER_LS_FRAMEPROFILER_ENABLED 0x34
#define JOBMANAGER_LS_JOBMANAGER_FORCE_BUCKET_CLEANUP 0x38

// ==============================================================================
// Common Job manager enums (needed to be ifdefs since this file is included by asm files
// ==============================================================================
// don't include enums and structures when compiling asm files
#if !defined(SPU_DRIVER_ASM_FILE)
namespace JobManager {
	enum { SPU_EVENT_QUEUE_CHANNEL = 42 }; // channel id used for SPU -> PPU communication
	enum { SPU_EVENT_QUEUE_NUM_ENTRIES = 32 } ; // number of slots in the event queue from SPU to PPU

	// priority settings used for jobs
	enum TPriorityLevel {
		eHighPriority			= 0,
		eRegularPriority	= 1,
		eLowPriority			= 2,
		eStreamPriority		= 3,
		eNumPriorityLevel = 4
	};
	
namespace SPUBackend {
	
	enum { USER_DMA_TAG_BASE = 12 };				// base tag available to custom usage
	enum { MEM_TRANSFER_DMA_TAG_BASE	= USER_DMA_TAG_BASE+2 };	//tag available for memtransfer_from_main/memtransfer_to_main

	//page mode
	enum EPageMode
	{
		ePM_Single = 0,		//single page mode, job occupies as much as it takes
		ePM_Dual	 = 1,		//2 pages
		ePM_Quad	 = 2,		//4 pages
	};
}
}

namespace JobManager {

	// Stores worker utilization 
	// One instance per Worker/SPU
	// One instance per cache line to avoid thread synchronization issues
	_MS_ALIGN(128) struct SWorkerStats
	{
		unsigned int nExecutionPeriod;	// Accumulated job execution period in micro seconds
		unsigned int nNumJobsExecuted;	// Number of jobs executed
	} _ALIGN(128); 

namespace SPUBackend {
	enum { scMaxWorkerSPU	= 5 };			// Maximum number of SPUs to manage as worker threads
	enum { scMaxSPU	= 6 };						// Maximum number of SPUs to manage (one extra SPU dedicated for DXPS)
	}

	///////////////////////////////////////////////////////////////////////////////
	// util structure for acks/data send back to SPU
	// since event queue try_recieve is very expensive on SPU
	// we are using a atomic cacheline read
	struct SSPUEventAcknowledgeLine
	{
		enum { nAcknowledgeCleared = 0};
		enum { nAcknowledgeSend = 1};

		unsigned int nAck;					// ack value, value is returned in SPU SendPPUThread (used nAcknowledgeSend if nothing is to return)
		
	} _ALIGN(128);


	///////////////////////////////////////////////////////////////////////////////
	// struct to collect all parameters passed to SPU
	struct SSPUDriverParameter
	{
		// spu number parameters
		unsigned int nNumAllowedSPUs;
		unsigned int nSPUIndex;
		unsigned int nWorkerIndex;
		unsigned int bIsSharedWithOS;

		// driver information
		unsigned int nDriverSize;

		// main memory addresses
		unsigned int eaSPUJobQueuePull;
		unsigned int eaSPUJobQueuePush;

		unsigned int eaBlockingJobQueuePull;
		unsigned int eaBlockingJobQueuePush;
		unsigned int eaBlockingQueueSemaphore;

		unsigned int eaJobSlotStateBase;
		unsigned int eaJobSlotStateBaseBlocking;
		unsigned int eaProfilingData;

		unsigned int eaInvokerListEA;

		unsigned int eaMemoryAreaManager;
		unsigned int eaPageInfo;

		unsigned int eaCustomCallbackArea;
		unsigned int eaAcknowledgeCacheLine;

		unsigned int eaBucketAllocatorList;

#if defined(JOBMANAGER_SUPPORT_FRAMEPROFILER)
		unsigned int eaJobStatsProfData;
		unsigned int eaWorkerStatsProfData;
#endif

		unsigned int eaAddJobToQueueModule;
		unsigned int sizeAddJobToQueueModule;

		unsigned int eaFallbackInfoBlockBlockingList;
	} _ALIGN(16);

} // namespace JobManager

#endif

#endif // _JOBMANAGER_SPUDRIVER_H_
