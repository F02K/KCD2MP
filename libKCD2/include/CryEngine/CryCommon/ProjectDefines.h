////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   ProjectDefines.h
//  Version:     v1.00
//  Created:     3/30/2004 by MartinM.
//  Compilers:   Visual Studio.NET
//  Description: to get some defines available in every CryEngine project 
// -------------------------------------------------------------------------
//  History:
//    July 20th 2004 - Mathieu Pinard
//    Updated the structure to handle more easily different configurations
//
////////////////////////////////////////////////////////////////////////////

#ifndef PROJECTDEFINES_H
#define PROJECTDEFINES_H

#include "BaseTypes.h"

#ifdef _RELEASE
	#define RELEASE
#endif

#define GAME_IS_CRYSIS3

#if defined(LINUX) || defined(APPLE)
#	define EXCLUDE_SCALEFORM_SDK
//  MM: Since mac/linux doesn't get the defines from the VS .props file, just set them here
#	define EXCLUDE_CINEMIZER_SDK
#	define EXCLUDE_OCULUS_SDK
#elif defined(ORBIS)
//#	define EXCLUDE_SCALEFORM_SDK
#	define EXCLUDE_CRI_SDK
#elif defined(XENON)
//#	define EXCLUDE_SCALEFORM_SDK
#	if !defined(_RELEASE) || defined(PERFORMANCE_BUILD)
#		define ENABLE_STATS_AGENT
#	endif
#elif defined(DURANGO)
//#	define EXCLUDE_SCALEFORM_SDK
#	define EXCLUDE_CRI_SDK
#	define EXCLUDE_GPU_PARTICLE_PHYSICS
#	if !defined(_RELEASE)
#		define ENABLE_STATS_AGENT
#	endif
#elif defined(PS3)
//#	define EXCLUDE_SCALEFORM_SDK

#	if !defined(_RELEASE) || defined(PERFORMANCE_BUILD)
#		define ENABLE_STATS_AGENT
#	endif
#	define EXCLUDE_PHYSICS_THREAD
//  MM: Since PS3 doesn't get the defines from the VS .props file, just set them here
#	define EXCLUDE_CINEMIZER_SDK
#	define EXCLUDE_OCULUS_SDK
# if defined(EXCLUDE_PHYSICS_THREAD) && !defined(PS3_CRYSIZER_HEAP_TRAVERSAL)
#  define EMBED_PHYSICS_AS_FIBER 1
# endif
#elif defined(WIN32) || defined(WIN64)
# if defined(DEDICATED_SERVER)
# define EXCLUDE_SCALEFORM_SDK
# endif // defined(DEDICATED_SERVER)
#	if !defined(_RELEASE) || defined(PERFORMANCE_BUILD)
#		define ENABLE_STATS_AGENT
#	endif
#else
#	define EXCLUDE_SCALEFORM_SDK
#endif

// The following definitions are used by Sandbox and RC to determine which platform support is needed
//#define TOOLS_SUPPORT_POWERVR
//#define TOOLS_SUPPORT_PS3
//#define TOOLS_SUPPORT_X360

// The following definitions are used by RC to determine which libraries to link with
#undef   EXCLUDE_PVRTEX_LIB

#if (_MSC_VER >= 1700)
# define EXCLUDE_ATICOMPRESS_LIB
# define EXCLUDE_NVTTCUDA_LIB
#endif

// Type used for vertex indices
#if defined(RESOURCE_COMPILER)
	typedef uint32 vtx_idx;


#else
#	if (defined(WIN32) || defined(WIN64) || defined(LINUX)) && !defined(XENON) && !defined(DURANGO)
	// PC. Both uint16 and uint32 vertex indices are supported. Uncomment one of the two following typedefs:
	//typedef uint32 vtx_idx;  
	typedef uint16 vtx_idx;
#	else
	// Consoles. Only uint16 vertex indices are supported.
	typedef uint16 vtx_idx;
#	endif
#endif


// see http://wiki/bin/view/CryEngine/TerrainTexCompression for more details on this
// 0=off, 1=on
#define TERRAIN_USE_CIE_COLORSPACE 0

// for consoles every bit of memory is important so files for documentation purpose are excluded
// they are part of regular compiling to verify the interface
#if defined(PS3) || defined(XENON) || defined(ORBIS)
#	define EXCLUDE_DOCUMENTATION_PURPOSE
#endif

#if defined(PS3) || defined(XENON)
#	define RENDERNODES_LEAN_AND_MEAN
#endif

#if defined(WIN32) || defined(WIN64)
#define RELEASE_LOGGING
#if defined(_RELEASE)
#define CVARS_WHITELIST
#endif // defined(_RELEASE)
#endif

#if defined(_RELEASE) && !defined(RELEASE_LOGGING) && !defined(DEDICATED_SERVER)
#define EXCLUDE_NORMAL_LOG
#endif

#if defined(WIN32) || defined(WIN64)
#if defined(DEDICATED_SERVER)
// enable/disable map load slicing functionality from the build
#define MAP_LOADING_SLICING
#endif
#endif

#if defined(MAP_LOADING_SLICING)
#if defined(__cplusplus)
#define SLICE_AND_SLEEP() do { GetISystemScheduler()->SliceAndSleep(__FUNC__, __LINE__); } while (0)
#define SLICE_SCOPE_DEFINE() CSliceLoadingMonitor sliceScope
#else
extern void SliceAndSleep( const char* pFunc, int line );
#define SLICE_AND_SLEEP() SliceAndSleep( __FILE__, __LINE__ )
#endif
#else
#define SLICE_AND_SLEEP()
#define SLICE_SCOPE_DEFINE()
#endif

#if (defined(PS3) || defined(XENON)) && !defined(_DEBUG)
#	define EXCLUDE_CVARHELP
#endif

// Compile with unit testing enabled when not in RELEASE
#if !defined(_RELEASE)
	#define CRY_UNIT_TESTING

	// configure the unit testing framework, if we have exceptions or not
	#if !(defined(PS3) || defined(XENON) || defined(APPLE) || defined(LINUX) || defined(ORBIS))
		#define CRY_UNIT_TESTING_USE_EXCEPTIONS
	#endif

#endif

#if (!defined(_RELEASE) || defined(PERFORMANCE_BUILD))
	#define USE_HTTP_WEBSOCKETS 1
#endif

#if ((defined(XENON) && !defined(_LIB)) || defined(WIN32) || defined(ORBIS) || defined(DURANGO)) && !defined(RESOURCE_COMPILER)
  #define CAPTURE_REPLAY_LOG 1
#endif

#ifdef PS3
  #if defined(ENABLE_MEMREPLAY) && !defined(CRY_DXPS_SINGLEFLUSHVALIDATE)
    #define CAPTURE_REPLAY_LOG 1
  #endif
  #if defined(JOB_LIB_COMP) || defined(__SPU__)
    #undef CAPTURE_REPLAY_LOG
  #endif
	
#endif

#if CAPTURE_REPLAY_LOG && defined(PS3_CRYSIZER_HEAP_TRAVERSAL)
	#undef CAPTURE_REPLAY_LOG
#endif

#if defined(RESOURCE_COMPILER) || defined(_RELEASE)
  #undef CAPTURE_REPLAY_LOG
#endif

#ifndef CAPTURE_REPLAY_LOG
  #define CAPTURE_REPLAY_LOG 0
#endif

#if (defined(PS3) || defined(XENON) || defined(LINUX) || defined(APPLE) ||  defined(WIN32) || defined(DURANGO) || defined(ORBIS)) && !defined(PS3_CRYSIZER_HEAP_TRAVERSAL)
	#define USE_GLOBAL_BUCKET_ALLOCATOR
#endif

#if (defined(PS3) || defined(XENON)) && !defined(PS3_CRYSIZER_HEAP_TRAVERSAL)
#define USE_LEVEL_HEAP 1
#endif

#define OLD_VOICE_SYSTEM_DEPRECATED













#	define TAGES_EXPORT

// test -------------------------------------
//#define EXCLUDE_CVARHELP

#define _DATAPROBE
#if defined(_RELEASE) && !defined(XENON) && !defined(PS3) && !defined(DURANGO)
#define ENABLE_COPY_PROTECTION
#endif // #if defined(_RELEASE) && !defined(XENON) && !defined(PS3)

// GPU pass timers are enabled here for Release builds as well
// Disable them before shipping, otherwise game is linked against instrumented libraries on 360
//#ifndef PURE_XENON_RELEASE
//#define ENABLE_SIMPLE_GPU_TIMERS
//#endif

#if defined(XENON) && defined(_RELEASE) && !defined(ENABLE_PROFILING_CODE)
//#ifndef ENABLE_SIMPLE_GPU_TIMERS
//	#define PURE_XENON_RELEASE
//#endif
#undef ENABLE_FRAME_PROFILER
#endif

//This feature allows automatic crash submission to JIRA, but does not work outside of CryTek
//Note: This #define will be commented out during code export
#define ENABLE_CRASH_HANDLER

#if !defined(PHYSICS_STACK_SIZE)
# define PHYSICS_STACK_SIZE (128U<<10)
#endif 
#if !defined(EMBED_PHYSICS_AS_FIBER)
# define EMBED_PHYSICS_AS_FIBER 0
#endif 
#if EMBED_PHYSICS_AS_FIBER && !defined(EXCLUDE_PHYSICS_THREAD)
# error cannot embed physics as fiber if the physics timestep is threaded!
#endif 

#if !defined(USE_LEVEL_HEAP)
#define USE_LEVEL_HEAP 0
#endif

#if USE_LEVEL_HEAP && !defined(_RELEASE)
#define TRACK_LEVEL_HEAP_USAGE 1
#endif

#ifndef TRACK_LEVEL_HEAP_USAGE
#define TRACK_LEVEL_HEAP_USAGE 0
#endif

#if (!defined(_RELEASE) || defined(PERFORMANCE_BUILD)) && !defined(RESOURCE_COMPILER)
#ifndef ENABLE_PROFILING_CODE
	#define ENABLE_PROFILING_CODE
#endif
//lightweight profilers, disable for submissions, disables displayinfo inside 3dengine as well
#ifndef ENABLE_LW_PROFILERS
	#define ENABLE_LW_PROFILERS
#endif
#endif

#if !defined(_RELEASE)
#define USE_FRAME_PROFILER      // comment this define to remove most profiler related code in the engine
#define CRY_TRACE_HEAP
#endif

#undef ENABLE_STATOSCOPE
#if defined(ENABLE_PROFILING_CODE)
  #define ENABLE_STATOSCOPE 1
#endif

#if defined(ENABLE_PROFILING_CODE)
//  #define ENABLE_SIMPLE_GPU_TIMERS
#endif

#if defined(PS3) && defined(ENABLE_PROFILING_CODE)
  #define ENABLE_ACCURATE_RSX_PROFILING
#endif

#if defined(ENABLE_PROFILING_CODE)
  #define USE_PERFHUD
#endif

#if defined(ENABLE_PROFILING_CODE)
#define ENABLE_ART_RT_TIME_ESTIMATE
#endif

#if defined(ENABLE_STATOSCOPE) && !defined(_RELEASE)
	#define FMOD_STREAMING_DEBUGGING 1
#endif

#if (defined(PS3) || defined(XENON)) && !defined(ENABLE_LW_PROFILERS)
#ifndef USE_NULLFONT
#define USE_NULLFONT 1
#endif
#define USE_NULLFONT_ALWAYS 1
#endif

#if defined(XENON) || defined(PS3)
#	define ENABLE_FLASH_LOCKLESS_RENDERING_API
#endif

#if defined(WIN32) || defined(WIN64)
#define FLARES_SUPPORT_EDITING
#endif

// Reflect texture slot information - only used in the editor
#if defined(WIN32) || defined(WIN64)
#define SHADER_REFLECT_TEXTURE_SLOTS 1
#else
#define SHADER_REFLECT_TEXTURE_SLOTS 0
#endif

#if (defined(WIN32) || defined(WIN64)) && !defined(_LIB)
#define CRY_ENABLE_RC_HELPER 1
#endif

#if !defined(_RELEASE) && (defined(XENON) || defined(PS3))
// create a watchdog thread on PS3/360 which will crash the game if the main update loop takes more than 5 minutes
#define ENABLE_WATCHDOG_THREAD
#endif

#if !defined(_RELEASE) && !defined(PS3) && !defined(LINUX) && !defined(APPLE) && !defined(DURANGO) && !defined(ORBIS)
	#define SOFTCODE_SYSTEM_ENABLED
#endif

// Is SoftCoding enabled for this module? Usually set by the SoftCode AddIn in conjunction with a SoftCode.props file.
#ifdef SOFTCODE_ENABLED

	// Is this current compilation unit part of a SOFTCODE build?
	#ifdef SOFTCODE
		// Import any SC functions from the host module
		#define SC_API __declspec(dllimport)
	#else
		// Export any SC functions from the host module
		#define SC_API __declspec(dllexport)
	#endif

#else	// SoftCode disabled
	
	#define SC_API

#endif

#if defined(PS3) || defined(_LIB)
#	define SYS_ENV_AS_STRUCT
#endif

#if !defined(_RELEASE) && !defined(PS3) && !defined(RESOURCE_COMPILER)
//For the time being, only compile Gamestream on profile PC & Xenon
#define USE_GAMESTREAM
#endif

// these enable and disable certain net features to give compatibility between PCs and consoles / profile and performance builds
#define PC_CONSOLE_NET_COMPATIBLE 0
#define PROFILE_PERFORMANCE_NET_COMPATIBLE 0

#if (!defined(_RELEASE) || defined(PERFORMANCE_BUILD)) && !PROFILE_PERFORMANCE_NET_COMPATIBLE
#define USE_LAGOMETER (1)
#else
#define USE_LAGOMETER (0)
#endif

// Enable additional structures and code for sprite motion blur. Currently non-functional and disabled
// #define PARTICLE_MOTION_BLUR

// a special ticker thread to run during load and unload of levels
#define USE_NETWORK_STALL_TICKER_THREAD

#if !defined(XENON) && !defined(PS3)
	//---------------------------------------------------------------------
	// Enable Tessellation Features
	// (displacement mapping, subdivision, water tessellation)
	//---------------------------------------------------------------------
	// Modules   : 3DEngine, Renderer
	// Depends on: DX11
	
	// Global tessellation feature flag
	#define TESSELLATION

	#ifdef TESSELLATION
		// Specific features flags
		#define WATER_TESSELLATION
		#define PARTICLES_TESSELLATION

		#ifndef ORBIS // Causes memory wastage in RenderMesh.cpp
			// Mesh tessellation (displacement, smoothing, subd)
			#define MESH_TESSELLATION
			// Mesh tessellation also in motion blur passes
			#define MOTIONBLUR_TESSELLATION
		#endif

		// Dependencies
		#ifdef MESH_TESSELLATION
			#define MESH_TESSELLATION_ENGINE
		#endif
		#ifdef DIRECT3D10
			#ifdef WATER_TESSELLATION
				#define WATER_TESSELLATION_RENDERER
			#endif
			#ifdef PARTICLES_TESSELLATION
				#define PARTICLES_TESSELLATION_RENDERER
			#endif
			#ifdef MESH_TESSELLATION_ENGINE
				#define MESH_TESSELLATION_RENDERER
			#endif

			#if defined(WATER_TESSELLATION_RENDERER) || defined(PARTICLES_TESSELLATION_RENDERER) || defined(MESH_TESSELLATION_RENDERER)
				// Common tessellation flag enabling tessellation stages in renderer
				#define TESSELLATION_RENDERER
			#endif
		#endif
	#endif //#ifdef TESSELLATION
#endif //#if !defined(XENON) && !defined(PS3)






#if !defined(_RELEASE)
#	define ENABLE_DYNTEXSRC_PROFILING
#endif

#if defined(ENABLE_PROFILING_CODE)
//#	define USE_DISK_PROFILER
//#	define ENABLE_LOADING_PROFILER
#endif

#if !defined(_DEBUG) && !defined(APPLE) && !defined(LINUX) && !defined(IS_FREESDK) && !defined(IS_PROSDK)
//#define CRY_PROFILE_MARKERS_USE_GPA
#endif

#if defined(WIN32) || defined(WIN64)
	//#define SEG_WORLD
#endif

#ifdef SEG_WORLD
#define SW_STRIP_LOADING_MSG
#define SW_ENTITY_ID_USE_GUID
#define SW_NAVMESH_USE_GUID
#endif

#include "ProjectDefinesInclude.h"

#if defined(SOFTCODE_ENABLED) && defined(NOT_USE_CRY_MEMORY_MANAGER)
	#error "SoftCode currently relies on CryMemoryManager being enabled. Either build without SoftCode support, or enable CryMemoryManager."
#endif

#if defined(GAME_IS_CRYSIS3)
#if !defined(XENON) && !defined(PS3) && !PC_CONSOLE_NET_COMPATIBLE
#define ANTI_CHEAT
#endif
#endif

#if defined(ANTI_CHEAT) && defined(DEDICATED_SERVER)
#define SERVER_CHECKS
#endif

//Encryption & security defines

//Defines for various encryption methodologies that we support (or did support at some stage)
#define SUPPORT_UNENCRYPTED_PAKS												//Enable during dev and on consoles to support paks that aren't encrypted in any way
#define SUPPORT_UNSIGNED_PAKS														//Enabled during dev to test release builds easier (remove this to enforce signed paks in release builds)

//#define SUPPORT_XTEA_PAK_ENCRYPTION										//C2 Style. Compromised - do not use
//#define SUPPORT_STREAMCIPHER_PAK_ENCRYPTION						//C2 DLC Style - by Mark Tully
#if !defined(XENON) && !defined(PS3) && !defined(DURANGO)
#define SUPPORT_RSA_AND_STREAMCIPHER_PAK_ENCRYPTION		//C3/Warface Style - By Timur Davidenko and integrated by Rob Jessop
#endif
#if !defined(_RELEASE) || defined(PERFORMANCE_BUILD)
#define SUPPORT_UNSIGNED_PAKS													//Enable to load paks that aren't RSA signed
#endif //!_RELEASE || PERFORMANCE_BUILD
#if !defined(DURANGO)
#define SUPPORT_RSA_PAK_SIGNING												//RSA signature verification
#endif

// Disabled - needs fixing with April XDK
#if defined(DURANGO)
//#define SUPPORT_SMARTGLASS
#endif

#if (defined(WIN32) || defined(WIN64) || defined(ORBIS)) && !defined(DIRECT3D9) && !defined(NULL_RENDERER)
#define GPU_PARTICLES 1
#else
#define GPU_PARTICLES 0
#endif

#if defined(SUPPORT_RSA_AND_STREAMCIPHER_PAK_ENCRYPTION) || defined(SUPPORT_RSA_PAK_SIGNING)
//Use LibTomMath and LibTomCrypt for cryptography
#define INCLUDE_LIBTOMCRYPT
#endif

//This enables checking of CRCs on archived files when they are loaded fully and synchronously in CryPak.
//Computes a CRC of the decompressed data and compares it to the CRC stored in the archive CDR for that file.
//Files with CRC mismatches will return Z_ERROR_CORRUPT and invoke the global handler in the PlatformOS.
#define VERIFY_PAK_ENTRY_CRC
#if !defined(PS3)
//Define for enabling CRC checking in the streaming engine. Can only work if the entire file is streamed and decompressed.
//Currently doesn't work on PS3 due to the complexity of adding it to the SPU code
#define VERIFY_PAK_ENTRY_CRC_STREAMING
#endif	//!PS3
//#define CHECK_CRC_ONLY_ONCE	//Do NOT enable this if using SUPPORT_RSA_AND_STREAMCIPHER_PAK_ENCRYPTION - it will break subsequent decryption attempts for a file as it nulls the stored CRC32
#if defined(XENON) || defined(PS3)
//On consoles we can trust files that have been loaded from optical drives
#define SKIP_CHECKSUM_FROM_OPTICAL_MEDIA
#endif //XENON || PS3

//End of encryption & security defines

// loading patch1.pak during startup
//#define USE_PATCH_PAK

#define EXPOSE_D3DDEVICE

// The maximum number of joints in an animation
#if defined(XENON) || defined(PS3)
#define MAX_JOINT_AMOUNT 256
#else
#define MAX_JOINT_AMOUNT 1024
#endif

#endif // PROJECTDEFINES_H
