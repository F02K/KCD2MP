//////////////////////////////////////////////////////////////////////
//
//  Crytek (C) 2001
//
//  CrySound Source Code
//
//  File: ISound.h
//  Description: Sound interface.
// 
//  History:
//  - August 28, 2001: Created by Marco Corbetta
//  - Januar 10, 2005: Taken over by Tomas Neumann
//  - October 1, 2009: Taken over by Thomas Wollenzin
//
//////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(ISound.h)

#ifndef CRYSOUND_ISOUND_H
#define CRYSOUND_ISOUND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerializeFwd.h"
#include "CryName.h"

// Forward declarations.
class		CCamera;
struct	IMusicSystem;
struct  IAudioDevice;
class		ICrySizer;
struct	IVisArea;
struct	ptParam;
struct	ISoundMoodManager;
struct	IReverbManager;
struct	ISound;
struct	ISoundBuffer;
struct	IMicrophone;
struct	INetworkSoundListener;
struct	IMicrophoneStream;
struct	IEntitySoundProxy;
struct  ISound_Extended;
struct  ISound_Deprecated;
struct	ISoundSystem_Extended;
struct	ISoundSystem_Deprecated;
struct	ISoundSystem_WorkInProgress;
struct	SSoundBufferInfo;
struct	IVisArea;

#ifndef EXCLUDE_SCALEFORM_SDK
namespace CryVideoSoundSystem
{
	struct ISoundDelegate;
	struct IChannelDelegate;
	struct IPlayerDelegate;
	struct IAllocatorDelegate;
}
#endif //#ifndef EXCLUDE_SCALEFORM_SDK

// Note:
//	 Need this explicit here to prevent circular includes to IEntity.
// Summary:
//	 Unique identifier for each entity instance.
typedef unsigned int EntityId;	


// Unique ID of a sound.
typedef uint32	tSoundID;
#define INVALID_SOUNDID 0

#define DECIBELTODISTANCE(_DB)	float(cry_powf(10,(_DB-11)/30.0f));

#define AUDIO_FILES_PATH      "Libs/GameAudio/"
#define AUDIO_FILES_FILE_PATH "Libs/GameAudio/AudioFiles.xml"

#define PROCESS_SOUND_OBSTRUCTION

#if defined(PROCESS_SOUND_OBSTRUCTION)
  #define SOUND_MAX_SKIPPED_ENTITIES	5	// Maximum number of skipped entities for obstruction calculation. (Defines cache size)
  #define SOUND_MAX_OBSTRUCTION_TESTS 10
#else
  #define SOUND_MAX_SKIPPED_ENTITIES	0	// Maximum number of skipped entities for obstruction calculation. (Defines cache size)
  #define SOUND_MAX_OBSTRUCTION_TESTS 0
#endif

#define SOUND_INDOOR_TAILNAME "indoor"
#define SOUND_OUTDOOR_TAILNAME "outdoor"

//////////////////////////////////////////////////////////////////////
#define MAX_SFX													1024

//////////////////////////////////////////////////////////////////////
// Valid Event Sound Flags (only use these on a event)
//////////////////////////////////////////////////////////////////////
//#define FLAG_SOUND_RELATIVE							BIT(6)	// Sound position moves relative to player.
//#define FLAG_SOUND_OUTDOOR							BIT(11)	// Play the sound only if the listener is in outdoor.
//#define FLAG_SOUND_INDOOR	 							BIT(12)	// Play the sound only if the listener is in indoor.
//#define FLAG_SOUND_CULLING		 					BIT(14)	// The sound uses sound occlusion (based on VisAreas).
//#define FLAG_SOUND_LOAD_SYNCHRONOUSLY					BIT(15)	// The loading of this sound will be synchronous (asynchronously by default).
//#define FLAG_SOUND_OBSTRUCTION						BIT(18)	// The sound uses sound obstruction (based on ray-world-intersects).
//#define FLAG_SOUND_SELFMOVING							BIT(19)	// Sounds will be automatically moved controlled by direction vector in m/sec.
//#define FLAG_SOUND_START_PAUSED						BIT(20)	// Start the sound paused, so an additional call to unpause is needed.
//#define FLAG_SOUND_VOICE      						BIT(21)	// Sound used as a voice (sub-titles and lip sync can be applied).

//////////////////////////////////////////////////////////////////////
// Valid Wave Sound Flags
//////////////////////////////////////////////////////////////////////
//#define FLAG_SOUND_LOOP								BIT(0)	 
//#define FLAG_SOUND_2D									BIT(1)	
//#define FLAG_SOUND_3D									BIT(2)	 
//#define FLAG_SOUND_STEREO								BIT(3)	 
//#define FLAG_SOUND_16BITS								BIT(4)	 
//#define FLAG_SOUND_STREAM								BIT(5)	// Streamed wav.
//#define FLAG_SOUND_RELATIVE							BIT(6)	// Sound position moves relative to player.
//#define FLAG_SOUND_DOPPLER							BIT(8)	// Use doppler effect for this sound.	
//#define FLAG_SOUND_NO_SW_ATTENUATION					BIT(9)	// Doesn't use SW attenuation for this sound.
//#define FLAG_SOUND_MUSIC								BIT(10)	// Pure music sound, to use to set pure music volume.
//#define FLAG_SOUND_OUTDOOR							BIT(11)	// Play the sound only if the listener is in outdoor.
//#define FLAG_SOUND_INDOOR	 							BIT(12)	// Play the sound only if the listener is in indoor.
//#define FLAG_SOUND_UNUSED								BIT(13)	// Free
//#define FLAG_SOUND_CULLING		 					BIT(14)	// The sound uses sound occlusion (based on VisAreas).
//#define FLAG_SOUND_LOAD_SYNCHRONOUSLY					BIT(15)	// The loading of this sound will be synchronous (asynchronously by default).
//#define FLAG_SOUND_MANAGED							BIT(16) // Managed sounds life time is controlled by the sound system, when sound stops it will be deleted.
//#define FLAG_SOUND_EDITOR								BIT(17)	// Mark sound as being only used within the Editor (eg. Facial Editor).
//#define FLAG_SOUND_OBSTRUCTION						BIT(18)	// The sound uses sound obstruction (based on ray-world-intersects).
//#define FLAG_SOUND_SELFMOVING							BIT(19)	// Sounds will be automatically moved controlled by direction vector in m/sec.
//#define FLAG_SOUND_START_PAUSED						BIT(20) // Start the sound paused, so an additional call to unpause is needed.
//#define FLAG_SOUND_VOICE      						BIT(21) // Sound used as a voice (sub-titles and lip sync can be applied).


//////////////////////////////////////////////////////////////////////
// Internal Sound Flags
//////////////////////////////////////////////////////////////////////

#define FLAG_SOUND_LOOP												BIT(0)	 
#define FLAG_SOUND_2D													BIT(1)	
#define FLAG_SOUND_3D													BIT(2)	 
#define FLAG_SOUND_STEREO											BIT(3)	 
#define FLAG_SOUND_16BITS											BIT(4)	 
#define FLAG_SOUND_STREAM											BIT(5)	// Streamed wav.
#define FLAG_SOUND_RELATIVE										BIT(6)	// Sound position moves relative to player.
#define FLAG_SOUND_RADIUS											BIT(7)	// Sound has a radius, custom attenuation calculation.
#define FLAG_SOUND_DOPPLER										BIT(8)	// Use doppler effect for this sound.
#define FLAG_SOUND_NO_SW_ATTENUATION					BIT(9)	// Doesn't use SW attenuation for this sound.
#define FLAG_SOUND_MUSIC											BIT(10)	// Pure music sound, to use to set pure music volume.
#define FLAG_SOUND_OUTDOOR										BIT(11)	// Play the sound only if the listener is in outdoor.
#define FLAG_SOUND_INDOOR											BIT(12)	// Play the sound only if the listener is in indoor.
#define FLAG_SOUND_ONLY_UPDATE_DIST_ON_START	BIT(13)	// Optimisation, avoid updating disance on some sounds (dsp effects - main thread blocking on fmod)
#define FLAG_SOUND_CULLING										BIT(14)	// The sound uses sound occlusion (based on VisAreas).
#define FLAG_SOUND_LOAD_SYNCHRONOUSLY					BIT(15)	// The loading of this sound will be synchronous (asynchronously by default).
#define FLAG_SOUND_ADD_TO_CACHE								BIT(16) // Marks sound to be added to the CRC-Cache for early culling
#define FLAG_SOUND_EDITOR											BIT(17)	// Mark sound as being only used within the Editor (eg. Facial Editor).
#define FLAG_SOUND_OBSTRUCTION								BIT(18)	// The sound uses sound obstruction (based on ray-world-intersects).
#define FLAG_SOUND_SELFMOVING									BIT(19)	// Sounds will be automatically moved controlled by direction vector in m/sec.
#define FLAG_SOUND_START_PAUSED								BIT(20) // Start the sound paused, so an additional call to unpause is needed.
#define FLAG_SOUND_VOICE											BIT(21) // Sound used as a voice (sub-titles and lip sync can be applied).
#define FLAG_SOUND_EVENT											BIT(22) // This sound is a sound event.
#define FLAG_SOUND_UNUSED                     BIT(23)	// This flag is currently unused.
#define FLAG_SOUND_PARAM_DOPPLER_ABS          BIT(24) // This sound has a doppler parameter (relative to world).
#define FLAG_SOUND_PARAM_DOPPLER_REL					BIT(25)	// This sound has a doppler parameter (relative to listener).
#define FLAG_SOUND_PARAM_SPREAD								BIT(26) // This sound has a spread parameter.
#define FLAG_SOUND_PARAM_SQUELCH							BIT(27) // This sound has a radio squelch parameter.
#define FLAG_SOUND_PARAM_FRONTLEFTREAR				BIT(28) // This sound has a front_left_rear FLR_degree parameter, describing rotation around z.
#define FLAG_SOUND_PARAM_TOPREARDOWN					BIT(29) // This sound has a top_read_down TRD_degree parameter, describing rotation around x.
#define FLAG_SOUND_PARAM_TOPLEFTDOWN					BIT(30) // This sound has a top_left_down TLD_degree parameter, describing rotation around y.
#define FLAG_SOUND_MOVIE											BIT(31) // this is a movie sound

// Extended sound flags
enum ESoundFlagsExtended
{
	eSFE_ENVIRONMENT_SOUND        = BIT(0), // Is set when the sound is interested in the environment setting at the sound's position.
	eSFE_ENVIRONMENT_LISTENER     = BIT(1), // Is set when the sound is interested in the environment setting at the listener's position.
	eSFE_ENVIRONMENT_MIX_SOUND    = BIT(2), // Is set when the sound is interested in the mixed environment setting at the sound's position.
	eSFE_ENVIRONMENT_MIX_LISTENER = BIT(3), // Is set when the sound is interested in the mixed environment setting at the listener's position.
	eSFE_POS_CHANGE_SOUND         = BIT(4), // This flag indicates that the sound object changed its position.
	eSFE_POS_CHANGE_LISTENER      = BIT(5), // This flag indicates that the listener object changed its position.
	eSFE_LISTENER_ANGLE           = BIT(6), // Is set when the sound is interested in the "(listener angle)" parameter setting.
	eSFE_CACHE_IS_VALID           = BIT(7), // Is set during sound initialization to indicate that the cache entry has successfully been created and that all values are set.
	eSFE_PLATFORM_CACHE_IS_VALID  = BIT(8), // Is set during platform sound creation to indicate that the platform sound specific cache data has successfully been set.
};

#define FLAG_SOUND_DEFAULT_3D ( FLAG_SOUND_CULLING | FLAG_SOUND_OBSTRUCTION )

#define FLAG_SOUND_ACTIVELIST	 	(FLAG_SOUND_RADIUS | FLAG_SOUND_CULLING | FLAG_SOUND_INDOOR | FLAG_SOUND_OUTDOOR)
#define SOUNDBUFFER_FLAG_MASK	(FLAG_SOUND_LOOP | FLAG_SOUND_2D | FLAG_SOUND_3D | FLAG_SOUND_STEREO | FLAG_SOUND_16BITS | FLAG_SOUND_STREAM | FLAG_SOUND_LOAD_SYNCHRONOUSLY)	// Flags affecting the sound-buffer, not its instance.


//////////////////////////////////////////////////////////////////////
// Precache Flags

#define FLAG_SOUND_PRECACHE_LOAD_SOUND				BIT(0)	 
#define FLAG_SOUND_PRECACHE_LOAD_GROUP				BIT(1)	 
#define FLAG_SOUND_PRECACHE_LOAD_PROJECT			BIT(2)
#define FLAG_SOUND_PRECACHE_STAY_IN_MEMORY		BIT(3)	
#define FLAG_SOUND_PRECACHE_UNLOAD_AFTER_PLAY	BIT(4)	
#define FLAG_SOUND_PRECACHE_UNLOAD_NOW				BIT(5)	


#define FLAG_SOUND_PRECACHE_EVENT_DEFAULT (FLAG_SOUND_PRECACHE_LOAD_SOUND)
#define FLAG_SOUND_PRECACHE_DIALOG_DEFAULT (FLAG_SOUND_PRECACHE_LOAD_SOUND | FLAG_SOUND_PRECACHE_UNLOAD_AFTER_PLAY)
#define FLAG_SOUND_PRECACHE_READABILITY_DEFAULT (FLAG_SOUND_PRECACHE_LOAD_SOUND)

// Update rates in MS = 1000/Hz
#define UPDATE_FILE_CACHE_MANAGER_IN_MS		100
#define UPDATE_SOUND_ASSET_MANAGER_IN_MS	250
#define UPDATE_SOUND_REVERB_MANAGER_IN_MS	250
#define UPDATE_AUDIO_DEVICE_IN_MS					10
#define UPDATE_SOUNDS_IN_MS								15
#define UPDATE_SOUND_AUDIODEVICE_IN_MS		15

#define SONIC_SPEED_METER_PER_SEC	343.0f
#define FMOD_MAX_NUM_NONBLOCKING_LOADING_THREADS 5

enum ESoundSystemErrorCode
{
	eSoundSystemErrorCode_None                    = 0x00000000,
	eSoundSystemErrorCode_ProjectNotReady         = BIT(0),
	eSoundSystemErrorCode_SoundCreateFailed       = BIT(1),
	eSoundSystemErrorCode_SoundIDRangeFull        = BIT(2),
	eSoundSystemErrorCode_SoundGotCulled          = BIT(3),
	eSoundSystemErrorCode_SoundNotKnown           = BIT(4),
	eSoundSystemErrorCode_SoundNotRegistered      = BIT(5),
	eSoundSystemErrorCode_SoundFlagsBad           = BIT(6),
	eSoundSystemErrorCode_NotCalledFromMainThread = BIT(7),
	eSoundSystemErrorCode_SoundCRCNotFoundInCache = BIT(8),
	eSoundSystemErrorCode_Max                     = BIT(31)
};

// Summary:
//	 Sound Semantics flags.
enum ESoundSemantic
{
	eSoundSemantic_None											= 0x00000000,
	eSoundSemantic_OnlyVoice								= BIT(0),
	eSoundSemantic_NoVoice									= BIT(1),
	eSoundSemantic_Sandbox									= BIT(2),
	eSoundSemantic_Sandbox_Browser          = BIT(3),

	eSoundSemantic_Unused1									= BIT(4),
	eSoundSemantic_Unused2									= BIT(5),
	eSoundSemantic_Ambience									= BIT(6), //a
	eSoundSemantic_Ambience_OneShot					= BIT(7), //b

	eSoundSemantic_Physics_Collision        = BIT(8), //c
	eSoundSemantic_Dialog                   = BIT(9), //d
	eSoundSemantic_MP_Chat                  = BIT(10), //e
	eSoundSemantic_Physics_Footstep         = BIT(11), //f

	eSoundSemantic_Physics_General					= BIT(12), //g
	eSoundSemantic_HUD											= BIT(13), //h
	eSoundSemantic_Replay										= BIT(14), //i
	eSoundSemantic_FlowGraph								= BIT(15), //j

	eSoundSemantic_Player_Foley_Voice       = BIT(16), //k
	eSoundSemantic_Living_Entity            = BIT(17), //l
	eSoundSemantic_Mechanic_Entity          = BIT(18), //m
	eSoundSemantic_NanoSuit                 = BIT(19), //n

	eSoundSemantic_SoundSpot                = BIT(20), //o
	eSoundSemantic_Particle                 = BIT(21), //p
	eSoundSemantic_AI_Pain_Death            = BIT(22), //q
	eSoundSemantic_AI_Readability           = BIT(23), //r

	eSoundSemantic_AI_Readability_Response  = BIT(24), //s
	eSoundSemantic_TrackView                = BIT(25), //t
	eSoundSemantic_Projectile               = BIT(26), //u
	eSoundSemantic_Vehicle                  = BIT(27), //v

	eSoundSemantic_Weapon                   = BIT(28), //w
	eSoundSemantic_Explosion                = BIT(29), //x
	eSoundSemantic_Player_Foley             = BIT(30), //y
	eSoundSemantic_Animation                = BIT(31) //z
};

enum ESoundActiveState
{
	eSoundState_None,
	eSoundState_Active,
	eSoundState_Ending,
	eSoundState_Inactive,
	eSoundState_Stopped,
	eSoundState_ProgrammerSoundReleased,
};

enum EFadeState
{
	eFadeState_None,
	eFadeState_FadingIn,
	eFadeState_FadingOut,
	eFadeState_JustFinished,
};

enum ESoundStopMode
{
	ESoundStopMode_AtOnce,
	ESoundStopMode_EventFade,
	// Note:
	//	 Fallback: will be stopped after 0.1 sec or set spSYNCTIMEOUTINSEC before.
	ESoundStopMode_OnSyncPoint, 
};

enum ESoundUpdateMode
{
	eSoundUpdateMode_None					 = 0x00000000,
	eSoundUpdateMode_Listeners		 = BIT(0),
	eSoundUpdateMode_Sounds				 = BIT(1),
	eSoundUpdateMode_Rest					 = BIT(2),
	eSoundUpdateMode_LostFocus		 = BIT(3),
	eSoundUpdateMode_FromMenu      = BIT(4),
	eSoundUpdateMode_CleanInactive = BIT(7),
	eSoundUpdateMode_All					 = 0x0000000F
};

enum EOutputHandle
{
	eOUTPUT_WINMM,			// Pointer to type HWAVEOUT is returned.
	eOUTPUT_DSOUND,			// Pointer to type DIRECTSOUND is returned.
	eOUTPUT_WASAPI,			// Pointer to type WASAPI is returned.
	eOUTPUT_OPENAL,			// Pointer to type OPENAL is returned.
	eOUTPUT_ASIO,				// NULL / 0 is returned.
	eOUTPUT_OSS,				// File handle is returned, (cast to int).
	eOUTPUT_ESD,				// Handle of type int is returned, as returned by so_esd_open_sound (cast to int). 
	eOUTPUT_ALSA,				// Pointer to type snd_pcm_t is returned.
	eOUTPUT_MAC,				// Handle of type SndChannelPtr is returned.
	eOUTPUT_Xbox,				// Pointer to type DIRECTSOUND is returned.
	eOUTPUT_Xbox360,		// Pointer to type IXAudio2 is returned.
	eOUTPUT_PS2,				// NULL / 0 is returned.
	eOUTPUT_PS3,				// NULL / 0 is returned.
	eOUTPUT_PSP,				// NULL / 0 is returned.
	eOUTPUT_GC,					// NULL / 0 is returned.
	eOUTPUT_WII,				// NULL / 0 is returned.
	eOUTPUT_NOSOUND,		// NULL / 0 is returned.
	eOUTPUT_WAVWRITER,	// NULL / 0 is returned.
	eOUTPUT_MAX,
};

enum EPrecacheResult
{
	ePrecacheResult_None,
	ePrecacheResult_OK,
	ePrecacheResult_Disabled,
	ePrecacheResult_ProjectNotReady,
	ePrecacheResult_Error,
	ePrecacheResult_Delayed,
	ePrecacheResult_Max,
};

enum ESoundLogType
{
	eSLT_Message,
	eSLT_Warning,
	eSLT_Error,
	eSLT_Always,
};


typedef struct 
{
	float fCurrentVolumeRatio;
	float fTargetVolumeRatio;

	float fCurrentObstructionRatio;
	float fTargetObstructionRatio;
	
	float fCurrentReverbFeed;
	float fTargetReverbFeed;
	
	float fTimeleft;
	bool  bHasChanged;
} SSoundGroupProperties;

#define MAX_VIS_AREAS 64 // Maximum of visarea cache.

struct IAudioListener 
{
	virtual ~IAudioListener(){}

  virtual void Init() = 0;
  virtual EntityId GetID() const = 0;
  virtual bool GetActive() const = 0;
	virtual void SetActive(bool const bActive) = 0;
  virtual bool GetMoved() const = 0;
  virtual bool GetInside() const = 0;
  virtual void SetInside(bool const bInside) = 0;
	virtual void SetRecordLevel(float const fRecordLevel) = 0;
	virtual float GetRecordLevel() const = 0;
	virtual Vec3 GetPosition() const = 0;
	virtual void SetPosition(Vec3 const& rPosition) = 0;
	virtual Vec3 const& GetForward() const = 0;
	virtual Vec3 GetTop() const = 0;
	virtual Vec3 const& GetVelocity() const = 0;
	virtual void SetVelocity(Vec3 const& vVel) = 0;
	virtual void SetMatrix(Matrix34 const& newTransformation) = 0;
	virtual Matrix34 const& GetMatrix() const = 0;
	virtual float GetUnderwater() const = 0;
	virtual void SetUnderwater(const float fUnder) = 0;
	virtual IVisArea* GetVisArea() const = 0;
	virtual void SetVisArea(IVisArea* const pVArea) = 0;
};
	// </interfuscator:shuffle>
// Description:
//	 Sound events sent to callback that can registered to every sound.
enum ESoundCallbackEvent
{
	SOUND_EVENT_ON_LOADED,            // Fired once the sound has been loaded.
	SOUND_EVENT_ON_LOAD_FAILED,       // Fired once the sound failed loading.
	SOUND_EVENT_ON_START,             // Fired once the sound is started.
	SOUND_EVENT_ON_PLAYBACK_STARTED,  // Fired once the sound's playback started.
	SOUND_EVENT_ON_PLAYBACK_UNPAUSED, // Fired once the sound's playback unpaused.
	SOUND_EVENT_ON_STOP,              // Fired once the sound stops.
	SOUND_EVENT_ON_PLAYBACK_STOPPED,  // Fired once the sound's playback stops.
	SOUND_EVENT_ON_PLAYBACK_PAUSED,   // Fired once the sound's playback paused.
	SOUND_EVENT_ON_SYNCHPOINT,        // Fired once the sound reaches a syncpoint.
	SOUND_EVENT_ON_INFO,              // Fired once the sound's definition is loaded.
};

// Description:
//	 Soundsystem events sent to callback that registered to soundsystem.
// Note:
//	 Important the first elements need to be the same as in ESoundCallbackEvent
// See also:
//	 ESoundCallbackEvent
enum ESoundSystemCallbackEvent
{
	SOUNDSYSTEM_EVENT_ON_LOADED,						// Fired when sound is loaded.
	SOUNDSYSTEM_EVENT_ON_LOAD_FAILED,				// Fired if sound loading is failed.
	SOUNDSYSTEM_EVENT_ON_START,							// Fired when sound is started.
	SOUNDSYSTEM_EVENT_ON_PLAYBACK_STARTED,	// Fired when sound's playback started.
	SOUNDSYSTEM_EVENT_ON_PLAYBACK_UNPAUSED,	// Fired when sound's playback unpaused.
	SOUNDSYSTEM_EVENT_ON_STOP,							// Fired when sound stops.
	SOUNDSYSTEM_EVENT_ON_PLAYBACK_STOPPED,	// Fired when sound's playback stops.
	SOUNDSYSTEM_EVENT_ON_PLAYBACK_PAUSED,		// Fired when sound's playback paused.
	SOUNDSYSTEM_EVENT_ON_SYNCHPOINT,				// Fired when sound reaches a syncpoint.
	SOUNDSYSTEM_EVENT_ON_UNLOAD							// Fired when sound system will be unloaded.
};

// Description:
//	 AFCM file cache entry events sent to listeners that registered to a particular file.
enum EFileCacheEntryCallbackEvent
{
	FILECACHEENTRY_NO_EVENT,
	FILECACHEENTRY_EVENT_ON_FILE_CACHED,		// Fired when this file entry got cached.
	FILECACHEENTRY_EVENT_ON_FILE_UNCACHED,	// Fired when this file entry got uncached.
	FILECACHEENTRY_EVENT_ON_FILE_PRELOADED,	// Fired when this file entry got preloaded.
	FILECACHEENTRY_EVENT_ON_FILE_UNLOADED,	// Fired when this file entry got unloaded.
};

// Description:
//	 Structure for sound obstruction
//	 a single obstruction test.
struct SObstructionTest
{
	SObstructionTest()
	: vOrigin(ZERO),
	  vDirection(ZERO),
	  nHits(0),
		nPierceability(0),
		nTestForTest(0),
		SoundID(INVALID_SOUNDID),
		fDistance(0.0f),
		bDirect(false),
		bResult(true)
	{}

	Vec3 vOrigin;
	Vec3 vDirection;
	int  nHits;
	int  nPierceability;
	int  nTestForTest;
	tSoundID SoundID;
	float fDistance;
	bool bDirect;
	bool bResult;
};

// Summary:
//	 Obstruction test.
struct SObstruction
{
	public: 

		// Constructor.
		SObstruction()
		: nObstructionSkipEnts(0),
		  nRaysShot(0),
			fLastObstructionUpdate(0.0f),
			bProcessObstruction(false),
			bProcessed(true),
			bAssigned(false),
			bDelayPlayback(false),
			bFirstTime(true),
			fDirectOcclusion(0.0f),
			fDirectOcclusionAccu(0.0f),
			fReverbOcclusion(0.0f),
			fReverbOcclusionAccu(0.0f)
		{
			memset(pOnstructionSkipEntIDs, (EntityId)0, sizeof(pOnstructionSkipEntIDs));
		}

		float GetDirect() const
		{
			return fDirectOcclusion;
		}

		void SetDirect(const float fObstruct)
		{
			if (fObstruct > FLT_EPSILON)
			{
				fDirectOcclusion = min(max(fObstruct, 0.0f), 1.0f);
			}
			else
			{
				fDirectOcclusion = 0.0f;
			}
		}

		void AddDirect(const float fObstruct)
		{
			fDirectOcclusionAccu += fObstruct;
		}

		float GetAccumulatedDirect() const
		{
			return fDirectOcclusionAccu;
		}

		void ResetDirect() 
		{
			fDirectOcclusion			= 0.0f;
			fDirectOcclusionAccu	= 0.0f;
		}

		float GetReverb() const
		{
			return fReverbOcclusion;
		}

		void SetReverb(const float fObstruct)
		{
			if (fObstruct > FLT_EPSILON)
			{
				fReverbOcclusion = min(max(fObstruct, 0.0f), 1.0f);
			}
			else
			{
				fReverbOcclusion = 0.0f;
			}
		}

		void AddReverb(const float fObstruct)
		{
			fReverbOcclusionAccu += fObstruct;
		}

		float GetAccumulatedReverb() const
		{
			return fReverbOcclusionAccu;
		}

		void ResetReverb() 
		{
			fReverbOcclusion			= 0.0f;
			fReverbOcclusionAccu	= 0.0f;
		}
	
	int nObstructionSkipEnts;
	int nRaysShot;

	float fLastObstructionUpdate;

	bool bProcessObstruction;
	bool bProcessed;
	bool bAssigned;
	bool bDelayPlayback;
	bool bFirstTime;

	SObstructionTest ObstructionTests[SOUND_MAX_OBSTRUCTION_TESTS];
	EntityId         pOnstructionSkipEntIDs[SOUND_MAX_SKIPPED_ENTITIES];

private:
	
	float fDirectOcclusion;				// level of obstruction from 0 = none, to 1 = full obstructed
	float fDirectOcclusionAccu;		// accumulates level of obstruction from 0 = none, to 1 = full obstructed
	float fReverbOcclusion;				// level of obstruction from 0 = none, to 1 = full obstructed
	float fReverbOcclusionAccu;		// accumulates level of obstruction from 0 = none, to 1 = full obstructed
};

// Structure to query cached information about a sound name via GetCullingByCache
struct SSoundCacheInfo
{
	SSoundCacheInfo()
	:	fMaxRadiusSq(0.0f),
		bCanBeCulled(false),
		nFlags(0),
		nFlagsExtended(0)
	{}

	float		fMaxRadiusSq;
	bool		bCanBeCulled;
	uint32	nFlags;
	uint32	nFlagsExtended;
};

// Structure to query information about memory
typedef struct SSoundMemoryInfo 
{
public:
	
	SSoundMemoryInfo()
		: //fSoundBucketPoolSizeInMB(0.0f),
			fSoundBucketPoolUsedInMB(0.0f),
			fSoundBucketPoolMaxInMB(0.0f),
			nSoundBucketPoolAllocCountMax(0),
			fSoundPrimaryPoolSizeInMB(0.0f),
			fSoundPrimaryPoolUsedInMB(0.0f),
			fSoundPrimaryPoolMaxInMB(0.0f),
			fSoundPrimaryPoolLargestBlockInMB(0.0f),
			nSoundPrimaryPoolAllocCountMax(0),
			fSoundSecondaryPoolSizeInMB(0.0f),
			fSoundSecondaryPoolUsedInMB(0.0f),
			fSoundSecondaryPoolMaxInMB(0.0f),
			nSoundSecondaryPoolAllocCountMax(0),
			nBufferTrashedCount(0),
			nMemTypeNormalCount(0),
			nMemTypeStreamFileCount(0),
			nMemTypeStreamDecodeCount(0),
			nMemTypeXBox360PhysicalCount(0),
			nMemTypePersistentCount(0),
			nMemTypeSecondaryCount(0)
	{
		for (size_t i = 0; i < eAllocationsArraySize; ++i)
			anAllocations[i] = 0;
	}

	// Methods
	inline void Reset()
	{
		//fSoundBucketPoolSizeInMB					= 0.0f;
		fSoundBucketPoolUsedInMB					= 0.0f;
		fSoundBucketPoolMaxInMB						= 0.0f;
		nSoundBucketPoolAllocCountMax			= 0;
		fSoundPrimaryPoolSizeInMB					= 0.0f;
		fSoundPrimaryPoolUsedInMB					= 0.0f;
		fSoundPrimaryPoolMaxInMB					= 0.0f;
		fSoundPrimaryPoolLargestBlockInMB	= 0.0f;
		nSoundPrimaryPoolAllocCountMax		= 0;
		fSoundSecondaryPoolSizeInMB				= 0.0f;
		fSoundSecondaryPoolUsedInMB				= 0.0f;
		fSoundSecondaryPoolMaxInMB				= 0.0f;
		nSoundSecondaryPoolAllocCountMax	= 0;
		nBufferTrashedCount								= 0;
		nMemTypeNormalCount								= 0;
		nMemTypeStreamFileCount						= 0;
		nMemTypeStreamDecodeCount					= 0;
		nMemTypeXBox360PhysicalCount			= 0;
		nMemTypePersistentCount						= 0;
		nMemTypeSecondaryCount						= 0;

		for (size_t i = 0; i < eAllocationsArraySize; ++i)
			anAllocations[i] = 0;
	}

	// BucketAllocator
	//float					fSoundBucketPoolSizeInMB; needs implementation
	float					fSoundBucketPoolUsedInMB;
	float					fSoundBucketPoolMaxInMB;
	size_t				nSoundBucketPoolAllocCountMax;
	
	//Primary
	float					fSoundPrimaryPoolSizeInMB;
	float					fSoundPrimaryPoolUsedInMB;
	float					fSoundPrimaryPoolMaxInMB;
	float					fSoundPrimaryPoolLargestBlockInMB;
	size_t				nSoundPrimaryPoolAllocCountMax;
	
	//Secondary
	float					fSoundSecondaryPoolSizeInMB;
	float					fSoundSecondaryPoolUsedInMB;
	float					fSoundSecondaryPoolMaxInMB;
	size_t				nSoundSecondaryPoolAllocCountMax;
	
	//Other
	unsigned int	nBufferTrashedCount;
	size_t				nMemTypeNormalCount;
	size_t				nMemTypeStreamFileCount;
	size_t				nMemTypeStreamDecodeCount;
	size_t				nMemTypeXBox360PhysicalCount;
	size_t				nMemTypePersistentCount;
	size_t				nMemTypeSecondaryCount;
	
	enum{eAllocationsArraySize = 30};
	size_t				anAllocations[eAllocationsArraySize];
} SSoundMemoryInfo;

// Summary:
//	 Listener interface for the sound.
struct ISoundEventListener
{
	// <interfuscator:shuffle>
	virtual ~ISoundEventListener(){}
	// Summary:
	//	 Callback event.
	virtual void OnSoundEvent( ESoundCallbackEvent event,ISound *pSound ) = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 Listener interface for the soundsystem.
struct ISoundSystemEventListener
{
	// <interfuscator:shuffle>
	virtual ~ISoundSystemEventListener(){}
	// Summary:
	//	 Callback event.
	virtual void OnSoundSystemEvent( ESoundSystemCallbackEvent event,ISound *pSound ) = 0;
	// </interfuscator:shuffle>
};

struct SFileCacheEntryEventInfo
{
	SFileCacheEntryEventInfo():eEvent(FILECACHEENTRY_NO_EVENT){}
	EFileCacheEntryCallbackEvent eEvent;
};

// Summary:
//	 Listener interface for the audio file cache entry.
struct IFileCacheEntryEventListener
{
	// <interfuscator:shuffle>
	virtual ~IFileCacheEntryEventListener(){}

	// Summary:
	//	 Callback event.
	virtual void OnFileCacheEntryEvent(SFileCacheEntryEventInfo const& rFileCacheEntryEventInfo) = 0;
	// </interfuscator:shuffle>
};

// Note:
//	 Marco's NOTE: this is a redefine of the EAX preset OFF, since it seems
//	 that audigy cards are having problems when the default EAX off preset.
#define MY_CS_PRESET_OFF  {0,	1.0f,	0.00f, -10000, -10000, -10000,   0.1f,  0.1f, 0.1f,  -10000, 0.0f, { 0.0f,0.0f,0.0f }, -10000, 0.0f, { 0.0f,0.0f,0.0f }, 0.0750f, 0.00f, 0.04f, 0.000f, 0.0f, 1000.0f, 20.0f, 0.0f,   0.0f,   0.0f, 0 }


// Summary:
//	 Wavebank interface.
struct IWavebank
{
	struct SWavebankInfo
	{
	public: 
		SWavebankInfo()
		{
			nTimesAccessed			= 0;
			nFileSize						= 0;
			nMemCurrentlyInByte = 0;
			nMemPeakInByte			= 0;
		}
		
		uint32 nTimesAccessed;
		uint32 nFileSize;
		uint32 nMemCurrentlyInByte;
		uint32 nMemPeakInByte;
	};
	// <interfuscator:shuffle>
	virtual ~IWavebank(){}
	virtual const char *GetName() = 0;
	virtual const char* GetPath() = 0;
	virtual void SetPath(const char* sWavebankPath) = 0;
	virtual IWavebank::SWavebankInfo* GetInfo() = 0;
	virtual void AddInfo(SWavebankInfo &WavebankInfo) = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct ISoundProfileInfo
{
	struct SSoundProfileInfo
	{
	public: 
		SSoundProfileInfo()
		{
			nTimesPlayed					= 0;
			nTimesPlayedOnChannel	= 0;
			nMemorySize						= 0;
			nPeakSpawn						= 0;
			nChannelsUsed					= 0;
			nSoundIDsUsed					= 0;
			bWavebanksLogged			= false;
		}

		uint32 nTimesPlayed;
		uint32 nTimesPlayedOnChannel;
		uint32 nMemorySize;
		uint32 nPeakSpawn;
		uint32 nChannelsUsed;
		uint32 nSoundIDsUsed;
		bool   bWavebanksLogged;
	};
	// <interfuscator:shuffle>
	virtual ~ISoundProfileInfo(){}
	virtual const char *GetName() = 0;
	virtual ISoundProfileInfo::SSoundProfileInfo* GetInfo() = 0;
	virtual void AddInfo(SSoundProfileInfo &SoundInfo) = 0;
	// </interfuscator:shuffle>
};

// Type of file cached in the FileCacheManager
enum EAudioFileCacheType
{
	eAFCT_NOTCACHED,
	eAFCT_GLOBAL,
	eAFCT_LEVEL_SPECIFIC,
	eAFCT_GAME_HINT,
	eAFCT_MUSIC,
	eAFCT_FSB_HEADER,
	eAFCT_ALL
};

// State of file cached in the FileCacheManager
enum EAudioFileCacheState
{
	eAFCS_CACHED                   = BIT(0),
	eAFCS_NOTCACHED                = BIT(1),
	eAFCS_NOTFOUND                 = BIT(2),
	eAFCS_MEMALLOCFAIL             = BIT(3),
	eAFCS_REMOVABLE                = BIT(4),
	eAFCS_LOADING                  = BIT(5),
	eAFCS_QUEUED_FOR_CACHING       = BIT(6),
	eAFCS_QUEUED_FOR_PRELOAD       = BIT(7),
	eAFCS_QUEUED_FOR_UNLOAD        = BIT(8),
	eAFCS_PRELOADED                = BIT(9),
	eAFCS_VIRTUAL                  = BIT(10),
	eAFCS_UNCACHE_AFTER_UNLOAD     = BIT(11),
	eAFCS_SOUND_NOT_READY          = BIT(12),
	eAFCS_UNLOAD_WHEN_SOUND_READY  = BIT(13),
	eAFCS_UNCACHE_WHEN_SOUND_READY = BIT(14),
  eAFCS_NEEDED_RECREATION        = BIT(15),
};

// Summary:
//	 Sound system interface.
struct ISoundSystem
{
	// <interfuscator:shuffle>
	virtual ~ISoundSystem(){}
	//////////////////////////////////////////////////////////////////////////
	// CORE FUNCTIONALITY
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Called after all engine modules have been initialized.
	virtual void PostInit() = 0;

	// Summary:
	//	 Has to be called regularly.	
	virtual void Update(const ESoundUpdateMode UpdateMode, bool const bIgnoreTimers = false) = 0;

	// Summary:
	//	 Saves and restores the state of the complete SoundSystem
	virtual void Serialize(TSerialize ser) = 0;

	// Summary:
	//	 Centrally outputs log information using an internal Spam filter
	virtual void Log( const ESoundLogType eType, const char *szFormat,... ) = 0;

	// Summary:
	//	 Queries the SoundSystem for the last thrown errors, eg. after getting a NULL pointer from CreateSound
	virtual ESoundSystemErrorCode GetError() = 0;
	
	// Summary:
	//	 Releases the SoundSystem
	virtual void Release() = 0;

	// Summary:
	//	 Registers listener to the sound.
	virtual void AddEventListener( ISoundSystemEventListener *pListener, bool bOnlyVoiceSounds ) = 0;

	// Summary:
	//	 Removes listener to the sound.
	virtual void RemoveEventListener( ISoundSystemEventListener *pListener ) = 0;

	// Summary:
	//	 Locks all sound buffer resources to prevent them from unloading (when restoring checkpoint).
	virtual void LockResources() = 0;
	// Summary:
	//	 Unlocks all sound buffer resources to prevent them from unloading.
	virtual void UnlockResources() = 0;

	// Summary:
	//	 Reset the sound system, unloading all loaded asset
	//   Usually called at unloading of the level
	virtual void Reset(bool bUnload) = 0;

	// Summary:
	//   Sets a global parameter value to all events which have a parameter with this name
	virtual void SetGlobalParameter( const char *sGlobalParameter, const float fValue) = 0;

	// Description:
	//	 Parses the AudioPreloads.xml and caches either just globals if parameter sLevelName is set to NULL
	//   or level specific files for the level passed in parameter sLevelName.
	// Arguments:
	//   sLevelName - Name of the level to cache files for.
	// Return Value:
	//	 void
	virtual void CacheAudioData( char const* const sLevelName, uint8 const nStage ) = 0;

	// Description:
	//	 Clears the entire cache if nothing is parsed or the all files of the parsed type.
	// Arguments:
	//	 eAudioFileCacheType - Type of files to clear out, if set to eAFCT_ALL will clear everything.
	// Return Value:
	//	 void
	virtual void ClearAudioDataCache( EAudioFileCacheType const eAudioFileCacheType = eAFCT_ALL, bool bDefrag = false ) = 0;

	// Description:
	//	 Caches a single *.fsb file or *.fsb files of a game hint.
	// Arguments:
	//	 sName - Name of the hint or *.fsb file path.
	//   nFileCacheType - file cache type of type EAudioFileCacheType.
	// Return Value:
	//	 void
	virtual void	CacheAudioFile( char const* const sName, EAudioFileCacheType const eAudioFileCacheType ) = 0;

	// Description:
	//	 Removes a single *.fsb file or *.fsb files of a game hint.
	// Arguments:
	//	 sName - Name of the hint or *.fsb file path.
	//   bNow - Indicates if the file(s) is(are) supposed to be removed immediately or gracefully when the sound system decides so.
	// Return Value:
	//	 void
	virtual void	RemoveCachedAudioFile( char const* const sName, bool const bNow ) = 0;

	// Description:
	//	Returns whether a particular *.fsb file or the *.fsb files of a game hint have the flags set.
	//	All *.fsb files in the hint must have the given flags set for the function to return true.
	// Arguments:
	//	sName - Name of the hint or *.fsb file path.
	//	flags - The flags to check.
	// Return Value:
	//	bool true if the flags are set on the fsb file or fsb files specified, false otherwise.
	virtual bool const EntryOrHintHasFlags(char const* const sName, const EAudioFileCacheState eFlags) = 0;

	// Description:
	//	Adds a listener to a particular file cache entry
	// Arguments:
	//	rNameCRC - Const reference to the file cache entry's name CRC.
	//	pListener - Pointer to the listener object to add.
	// Return Value:
	//	void
	virtual void AddListenerToFileCacheEntry(CCryNameCRC const& rNameCRC, IFileCacheEntryEventListener* const pListener) = 0;

	// Description:
	//	Removes a listener from a particular file cache entry
	// Arguments:
	//	rNameCRC - Const reference to the file cache entry's name CRC.
	//	pListener - Pointer to the listener object to remove.
	// Return Value:
	//	void
	virtual void RemoveListenerFromFileCacheEntry(CCryNameCRC const& rNameCRC, IFileCacheEntryEventListener* const pListener) = 0;

	//////////////////////////////////////////////////////////////////////////
	// SOUND CREATION & HANDLING
	//////////////////////////////////////////////////////////////////////////

	// Description:
	//	 Provides a quick check if a sound effect should be even created.
	// Arguments:
	//   sGroupAndSoundName - Name of the sound to be played
	//	 vPosition - Position where the sound should play.
	// Return Value:
	//	 SystemError none, if a valid result could be retrieved, eSoundSystemErrorCode_SoundCRCNotFoundInCache if the result is invalid
	//	 bool true, if this sound effect can be safely be culled, false if it should not be culling
	virtual ESoundSystemErrorCode const GetCullingByCache(char const* const sGroupAndSoundName, Vec3 const& rPosition, SSoundCacheInfo& rCullInfo) = 0;

	// Description:
	//	 Provides a quick check if a sound effect should be even created.
	// Arguments:
	//   nGroupAndSoundNameCRC - CRC of the name of the sound to be played
	//	 vPosition - Position where the sound should play.
	// Return Value:
	//	 SystemError none, if a valid result could be retrieved, eSoundSystemErrorCode_SoundCRCNotFoundInCache if the result is invalid
	//	 bool true, if this sound effect can be safely be culled, false if it should not be culling
	virtual ESoundSystemErrorCode const GetCullingByCache(uint32 const nGroupAndSoundNameCRC, Vec3 const& rPosition, SSoundCacheInfo& rCullInfo) = 0;

	// HACK HACK HACK FOR CRYSIS 2
	virtual void HackFixupName(const char *soundName, char *output_buffer, size_t output_buffer_size) = 0;

	// Description:
	//	 Creates a sound object by combining the definition of the sound and the buffer of sound data.
	// Arguments:
	//	 nFlags - Sound flags.
	//   nFlagsExtended - Extended sound flags.
	// Return Value:
	//	 ISound* is a Ptr to a Sound, this will change to the unique Sound ID later.
	virtual ISound* const CreateSound       (char const* const sGroupAndSoundName, uint32 nFlags, uint32 nFlagsExtended = 0, uint32 const nPrecacheFlags = 0) = 0;

	// Description:
	//	 Creates a sound object by combining the definition of the sound and the buffer of sound data.
	// Arguments:
	//	 nFlags - Sound flags.
	// Return Value:
	//	 ISound* is a Ptr to a Sound, this will change to the unique Sound ID later.
	virtual ISound* const CreateLineSound   (char const* const sGroupAndSoundName, uint32 nFlags, uint32 nFlagsExtended, const Vec3 &vStart, const Vec3 &VEnd ) = 0;

	// Description:
	//	 Creates a sound object by combining the definition of the sound and the buffer of sound data.
	// Arguments:
	//	 nFlags - Sound flags.
	// Return Value:
	//	 ISound* is a Ptr to a Sound, this will change to the unique Sound ID later.
	virtual ISound* const CreateSphereSound (char const* const sGroupAndSoundName, uint32 nFlags, uint32 nFlagsExtended, const float fRadius ) = 0;

	// Summary:
	//	 Gets a sound interface from the sound system.
	// Arguments:
	//	 nSoundId - Sound id.
	virtual struct ISound* GetSound(tSoundID nSoundID) const = 0;

	// Description:
	//	 Precaches the definition and the buffer of a group, subgroup or a sound.
	// Arguments:
	//	 sGroupAndSoundName - Name of group (with /path/) and name of sound split by ":".
	//	 nSoundFlags		- some Flags that can be set, needs to be defined yet.
	//	 nPrecacheFlags		- Some Flags that can be set to change precaching behaviour.
	// Return Value:
	//	 EPrecacheResult result code.
	virtual EPrecacheResult Precache( const char *sGroupAndSoundName, uint32 nSoundFlags, uint32 nPrecacheFlags ) = 0;

	// Description:
	//	 Precaches a dialog line plus dialog project and buffer if needed.
	//
	// Parameters:
	//	 sKeyName       - dialog line key name
	//
	// Return Value:
	//	 EPrecacheResult result enum
	virtual EPrecacheResult const PrecacheDialogLine(char const* const sKeyName) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	// VOLUME CONTROL
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Sets master volume.
	// Arguments:
	//	 fVol - Volume (0.0f - 1.0f)		
	virtual void SetMasterVolume(float fVol) = 0;

	// Summary:
	//	 Gets master volume.
	// Arguments:
	//	 fVol - Volume (0.0f - 1.0f)		
	virtual float GetMasterVolume() const = 0;

	// Summary:
	//	 Sets the volume scale for all sounds.
	// Arguments:
	//	 fScale - Volume scale (default 1.0)			
	virtual void SetMasterVolumeScale(float fScale, bool bForceRecalc=false) = 0;

	// Summary:
	//	 Gets SFX volume.
	// Arguments:
	//	 fVol - Volume (0.0f - 1.0f)
	virtual float GetSFXVolume() = 0;

	// SetMovieFadeoutVolume
	// @param movieFadeoutVolume fadeout volume (0.0f - 1.0f)
	virtual void SetMovieFadeoutVolume(const float movieFadeoutVolume) = 0;

	// GetMovieFadeoutVolume
	// @param movieFadeoutVolume fadeout volume (0.0f - 1.0f)
	virtual float GetMovieFadeoutVolume() const = 0;

	// Description:
	//	 Sets parameters for directional attenuation (for directional microphone effect); set fConeInDegree to 0 to disable the effect.
	virtual void CalcDirectionalAttenuation(const Vec3 &Pos, const Vec3 &Dir, const float fConeInRadians) = 0;

	// Description:
	//	 Returns the maximum sound-enhance-factor to use it in the binoculars as "graphical-equalizer"...
	virtual float GetDirectionalAttenuationMaxScale() = 0;

	// Summary:
	//	 Returns if directional attenuation is used.
	virtual bool UsingDirectionalAttenuation() = 0;

	//////////////////////////////////////////////////////////////////////////
	// PLAYBACK FUNCTIONALITY
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Pauses all sounds.
	virtual void	Pause(bool bPause, bool bResetVolume=false, unsigned int nFadeOutInMS=0) = 0; 
	// Summary:
	//	 Checks if the SoundSystem is paused.
	virtual bool  IsPaused() = 0;

	// Summary:
	//	 Sets Muted/unmuted all sounds.
	virtual void	Mute(bool bMute)=0;

	// Summary:
	//	 Stops all sounds and music.
	virtual bool	Silence(bool bStopLoopingSounds, bool bUnloadData) = 0;

	// Summary:
	//	 Filter out any sounds which don't match the given semantic mask and stop any existing sounds.
	virtual void  SetPlaybackFilter(const uint32 nMask) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Listener
	//////////////////////////////////////////////////////////////////////////

	// Description:
	//	 Set the listener transformation matrix, velocity and record level for a special listener.
	// Arguments:
	//	 nListenerID	- ListenerID of the listener.
	//	 matOrientation - Transformation matrix.
	//	 vVel			- Velocity vector.
	//	 fRecordLevel	- Level of input sensitivity.
	// Return Value:
	//	 Bool value if listener was successfully found and updated.
	virtual bool SetListener(EntityId const nListenerID, Matrix34 const& matOrientation, Vec3 const& vVel, bool const bActive, float const fRecordLevel) = 0;

	//virtual void SetListenerEntity( ListenerID nListenerID, EntityId nEntityID ) = 0;

	// Summary:
	//	 Creates a new listener.
	// Return Value:
	//	 ListenerID of new created Listener or INVALID_LISTENERID on failure.
	virtual EntityId CreateListener() = 0;

	// Summary:
	//	 Removes a listener.
	// Arguments:
	//	 nListenerID - ListenerID of the listener which should be removed.
	// Return Value:
	//	 Bool if Listener was removed successfully .
	virtual bool RemoveListener(EntityId const nListenerID, bool const bRemoveEntity = true) = 0;

	// Description:
	//	 Retrieves the ListenerID of the closest listener to a 3D Point.
	// Arguments:
	//	 vPosition - Position in 3D
	// Return Value:
	//	 nListenerID - ListenerID of the active listener which is closest.
	virtual EntityId GetClosestActiveListener(Vec3 const& vPosition) const = 0;

	// Description:
	//	 Retrieves a pointer to a listener struct of the listener defined by EntityID.
	// Arguments:
	//	 nEntityID - EntityID of a valid Listener.
	// Return Value:
	//	 IAudioListener* Pointer to Listener struct or NULL if not found.
	virtual IAudioListener* GetListener(EntityId const nEntityID) const = 0;

	// Description:
	//	 Retrieves a pointer to a listener struct of the listener who has a higher ID than provided.
	// Arguments:
	//	 nListenerID - ListenerID or LISTENERID_STANDARD to start.
	// Return Value:
	//	 IAudioListener* Pointer to listener struct or NULL if no listener has a higher ID.
	virtual IAudioListener* GetNextListener(EntityId const nListenerID) = 0;

	// Description:
	//	 Retrieves the number of active listeners.
	// Return Value:
	//	 uint32 Number of active Listeners
	virtual uint32 GetNumActiveListeners() const = 0;

	//////////////////////////////////////////////////////////////////////////
	// VOIP
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Checks for multiple record devices and expose them.
	virtual bool GetRecordDeviceInfo(const int nRecordDevice, char* sName, int nNameLength) = 0;

	// Summary:
	//	 Creates a new microphone.
	virtual IMicrophone* CreateMicrophone(const unsigned int nRecordDevice, const unsigned int nBitsPerSample, const unsigned int nSamplesPerSecond, const unsigned int nBufferSizeInSamples) = 0;

	// Summary:
	//	 Removes a microphone.
	virtual bool RemoveMicrophone( IMicrophone *pMicrophone ) = 0;

	// Description:
	//	 Creates a 3D sound emitting the talk of PlayerName that belongs to PlayerMicro.
	//	 Probably it even gets routed to the headset's speaker, so we need to identify target player.
	virtual ISound* CreateNetworkSound(	INetworkSoundListener *pNetworkSoundListener, const unsigned int nBitsPerSample, const unsigned int nSamplesPerSecond, const unsigned int nBufferSizeInSamples, EntityId PlayerID) = 0;

	// Description:
	//	 Removes this network sound, probably ISound:Release can replace this later.
	virtual void RemoveNetworkSound (ISound *pSound) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Interfaces
	//////////////////////////////////////////////////////////////////////////

	virtual ISoundSystem_Extended* GetInterfaceExtended() = 0;
	virtual ISoundSystem_Deprecated* GetInterfaceDeprecated() = 0;
	virtual ISoundSystem_WorkInProgress* GetInterfaceWorkInProgress() = 0;

	// Summary:
	//	 Returns IAudioDevice Interface.
	virtual IAudioDevice*		GetIAudioDevice()	const = 0;

	// Summary:
	//	 Returns ISoundMoodManager Interface.
	virtual ISoundMoodManager* GetIMoodManager() const = 0;

	// Summary:
	//	 Returns IReverbManager Interface.
	virtual IReverbManager* GetIReverbManager() const = 0;

	//main update timing
	virtual float		GetUpdateMilliseconds() = 0;

	virtual void		SetAllowReadRequests(bool bValue ) = 0;

	// Description:
	//	 Retrieves the count of all known FMOD parameters.
	virtual size_t	GetNumParameterNames() const = 0;
	// </interfuscator:shuffle>

#if !defined(EXCLUDE_SCALEFORM_SDK)
	virtual CryVideoSoundSystem::IPlayerDelegate* CreateVideoSoundSystem(CryVideoSoundSystem::IAllocatorDelegate* pAllocator) const = 0;
#endif
};

// Extended ISoundSystem Interface for use in Sandbox
// Functions calls in the Extended Interface should only be used after fully understanding the impact
// and the functionality. When using the data-driven SoundSystem most of these calls should not be called
// by the game
struct ISoundSystem_Extended : public ISoundSystem
{
	// <interfuscator:shuffle>
	//////////////////////////////////////////////////////////////////////////
	// SYSTEM CONTROL
	//////////////////////////////////////////////////////////////////////////

	// Description:
	//	 Need to call this after the AudioDevice was set and initialized.
	virtual bool Init() = 0;

	// Description:
	// Creates a music-system. You should only create one music-system at a time.
	virtual IMusicSystem* CreateMusicSystem() = 0;

	// Summary:
	//	 Added to set and get music effect value for volume.
	virtual void SetMusicEffectsVolume(float v ) = 0;

	// Summary:
	//	 Gets music-volume.
	virtual float GetMusicVolume() const = 0;

	// Summary:
	//	 Sets master pitch.
	// Arguments:
	//	 fPitch - Pitch (in octaves).	
	virtual void SetMasterPitch(float fPitch) = 0;

	// Description:
	//	 Called to be able to overwrite sound files and clear any loaded dependency.
	virtual bool DeactivateAudioDevice() = 0;
	// Description:
	//	 Called to be able to overwrite sound files and clear any loaded dependency.
	virtual bool ActivateAudioDevice() = 0;

	//////////////////////////////////////////////////////////////////////////
	// PROFILING
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 This one is to debug some of FMOD memory allocation.
	virtual void TraceMemoryUsage(int nMemUsage) = 0;

	// Summary:
	//	 Gets memory usage.
	virtual void GetMemoryUsage(class ICrySizer* pSizer) const = 0;

	// Summary:
	//	 Gets memory usage info.
	virtual void	GetMemoryInfo(SSoundMemoryInfo *pMemInfo)const = 0;

	// Summary:
	//	 Gets memory usage in MB.
	virtual int  GetMemoryUsageInMB(bool bGetPoolSize = false) = 0;

	// Summary:
	//	 Gets number of voices playing.
	virtual int	GetUsedVoices() const = 0;

	// Summary:
	//	 Gets cpu-usuage.
	virtual float	GetCPUUsage() const = 0;

	// Summary:
	//		Profiling sounds.
	virtual ISoundProfileInfo* GetSoundInfo(int nIndex) = 0;
	virtual ISoundProfileInfo* GetSoundInfo(const char* sSoundName) = 0;
	virtual int GetSoundInfoCount() = 0;
	
	//////////////////////////////////////////////////////////////////////////
	// INFORMATION
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Returns true if the NULLSoundSystem was initialized.
	virtual bool IsNullImplementation() const = 0;

	//////////////////////////////////////////////////////////////////////////
	// FUNCTIONALITY
	//////////////////////////////////////////////////////////////////////////
	
	// Description:
	//	 Sets minimal priority for sounds to be played.
	//	 Sound`s with priority less then that will not be played.
	// Return Value:
	//	 Previous minimal priority..
	virtual int SetMinSoundPriority( int nPriority ) = 0;
	
	// Description:
	//	 To be called when something changes in the environment which could affect
	//	 sound occlusion, for example a door closes etc.
	// Arguments:
	//	 bRecomputeListener - Recomputes the listener vis area.
	//	 bForceRecompute	- Forces to recompute the vis area connections even if
	// 	 the listener didn't move (useful for moving objects that can occlude).
	virtual void	RecomputeSoundOcclusion(bool bRecomputeListener, bool bForceRecompute, bool bReset=false) = 0;

	// Description:
	//	 Converts a dialog sound name reference to a file into a dialog key syntax
	//	 which is compatible with the LocalizationManager
	// Return Value:
	//	 name of the dialog key
	virtual const char* MakeDialogKey(const char* sDialogFileName) = 0;
	// </interfuscator:shuffle>
};

// ISoundSystem Interface extention with functions which will be deprecated
struct ISoundSystem_Deprecated
{
	// <interfuscator:shuffle>
	virtual ~ISoundSystem_Deprecated(){}
	virtual void SetSoundActiveState(ISound *pSound, ESoundActiveState State) = 0;

	// Summary:
	//	 Checks for EAX support.
	virtual bool IsEAX(void) = 0;
	// </interfuscator:shuffle>
};

// ISoundSystem Interface extention with functions which are in development and are not fully supported yet
struct ISoundSystem_WorkInProgress
{
	// <interfuscator:shuffle>
	virtual ~ISoundSystem_WorkInProgress(){}
	// Summary:
	//	 Sets the weather condition that affect acoustics.
	virtual bool SetWeatherCondition(float fWeatherTemperatureInCelsius, float fWeatherHumidityAsPercent, float fWeatherInversion) = 0;

	// Summary:
	//	 Gets the weather condition that affect acoustics.
	virtual bool GetWeatherCondition(float &fWeatherTemperatureInCelsius, float &fWeatherHumidityAsPercent, float &fWeatherInversion) = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 Special asset ParamSemantics.
enum enumSoundParamSemantics
{
	// review and clean this list, check implementation

	spNONE,
	spINITIALIZED,
	spSOUNDID,
	spSOUNDTYPE, // ?
	spSAMPLETYPE, //?
	spFREQUENCY,
	spPITCH,

	spVOLUME,
	spISPLAYING,	
	spPAUSEMODE,
	spLOOPMODE,
	spLENGTHINMS,
	spLENGTHINBYTES,
	spLENGTHINSAMPLES,
	spSAMPLEPOSITION,
	spTIMEPOSITION,
	sp3DPOSITION,
	sp3DVELOCITY,
	spMINRADIUS,
	spMAXRADIUS,
	spPRIORITY,
	spFXEFFECT,
	spAMPLITUDE,
	spSPEAKERPAN,
	spREVERBWET,
	spREVERBDRY,
	spSYNCTIMEOUTINSEC,
	spLISTENERANGLE,
};

enum eUnloadDataOptions
{
	sbUNLOAD_NONE,
	sbUNLOAD_ALL_DATA,
	sbUNLOAD_ALL_DATA_NOW,
	sbUNLOAD_UNNEEDED_DATA,
	sbUNLOAD_UNNEEDED_DATA_NOW
};

// Summary
// Basic Interface to work with a Sound. All functionality listed here is sufficient for Game Code
struct ISound
{
	// <interfuscator:shuffle>
	virtual ~ISound(){}
	virtual void GetMemoryUsage( ICrySizer *pSizer ) const=0;

	//////////////////////////////////////////////////////////////////////////
	// CORE FUNCTIONALITY
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Every Sound has to have a Semantic assigned to it before it can be played back
	virtual void	SetSemantic(const ESoundSemantic eSemantic) = 0;
	virtual const ESoundSemantic	GetSemantic()const = 0;

	// Summary:
	//	 Registers event listener to the sound.
	virtual void AddEventListener( ISoundEventListener *pListener, const char *sWho ) = 0;

	// Summary:
	//	 Remove event listener to the sound.
	virtual void RemoveEventListener( ISoundEventListener *pListener ) = 0;

	// Summary:
	//	 Adds / removes reference counts.
	// Note:
	//	 This is used in the smart_ptr automatically.
	virtual int	AddRef() = 0;
	virtual int	Release() = 0;

	//////////////////////////////////////////////////////////////////////////
	// PLAYBACK CONTROL
	//////////////////////////////////////////////////////////////////////////

	virtual void Play(const float fVolumeScale=1.0f, const bool bForceActiveState=true, const bool bSetRatio=true, IEntitySoundProxy *pEntitySoundProxy=NULL) = 0;
	virtual void Stop(const ESoundStopMode eStopMode=ESoundStopMode_EventFade) = 0; // Sound will be invalid after stopped, unless being static on a soundproxy
	
	virtual void SetPaused(const bool bPaused) = 0;
	virtual const bool GetPaused()const = 0;

	// Summary:
	//	 Fading In/Out - 0.0f is Out 1.0f is In.
	virtual void				SetFade(const float fFadeGoal, const int nFadeTimeInMS) = 0;
	virtual EFadeState	GetFadeState() const = 0;

	virtual SSoundBufferInfo* GetSoundBufferInfo() { return 0; };

	//////////////////////////////////////////////////////////////////////////
	// SPATIALIZATION
	//////////////////////////////////////////////////////////////////////////
	
	// Summary:
	//	 Sets sound source position.
	// Note:
	//	 Position gets automatically updated if the sound is played on a SoundProxy
	virtual void	SetPosition(const Vec3 &pos) = 0;

	// Summary:
	//	 Gets sound source position.
	virtual Vec3 GetPosition() const = 0;	
	
	virtual void			SetMatrix(Matrix34 const& newTransformation) = 0;
	virtual Matrix34	GetMatrix() const = 0;
	virtual void			SetDirection(const Vec3 &vDir) = 0; // overwrites transformation matrix
	virtual Vec3			GetForward() const = 0;
	
	//////////////////////////////////////////////////////////////////////////
	// MANIPULATION
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Gets parameter defined in the enumAssetParam list.
	virtual bool			GetParam(enumSoundParamSemantics eSemantics, ptParam* pParam) const = 0;
	// Summary:
	//	 Sets parameter defined in the enumAssetParam list.
	virtual bool			SetParam(enumSoundParamSemantics const eSemantics, ptParam const* const pParam, bool const bDisableAutomation = false) = 0;

	// Summary:
	//	 Gets parameter defined by string and float value, returns the index of that parameter
	virtual int				GetParam(const char *sParameter, float *fValue, bool bOutputWarning=true) const = 0;
	// Summary:
	//	 Sets parameter defined by string and float value, returns the index of that parameter
	virtual int				SetParam(const char *sParameter, float fValue, bool bOutputWarning=true) = 0;

	// Summary:
	//	 Gets parameter defined by index and float value.
	virtual bool			GetParam(int nIndex, float* const pfValue, float* const pfRangeMin=NULL, float* const pfRangeMax=NULL, char const** const ppcParameterName=NULL, bool const bOutputWarning=true) const = 0;
	// Summary:
	// Sets parameter defined by index and float value.
	virtual bool			SetParam(int nIndex, float fValue, bool bOutputWarning=true) = 0;

	// Description:
	//	 Sets a distance multiplier so sound event's distance can be tweak (sadly pretty hack feature).
	virtual void			SetDistanceMultiplier(const float fMultiplier) = 0;

	//////////////////////////////////////////////////////////////////////////
	// INFORMATION
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Gets unique id of sound.
	virtual const tSoundID	GetId()const = 0;

	// Summary:
	//	 Gets name of sound file.
	virtual const char*	GetName() = 0;

	// Summary:
	//	 Gets the Sound Flags to query information about the sound
	virtual uint32 GetFlags() const = 0;

	// Summary:
	//	 Gets the extended sound flags to query information about the sound.
	virtual uint32 GetFlagsExtended() const = 0;

	virtual bool IsPlaying() const = 0;
	virtual bool IsPlayingVirtual() const = 0;

	// Summary:
	//	 Returns true if sound is now in the process of asynchronous loading of sound buffer.
	virtual bool IsLoading()const = 0;
	// Summary:
	//	 Returns true if sound have already loaded sound buffer.
	virtual bool IsLoaded() const = 0;
	// Summary:
	//	 Returns true if sound is initialized and other queries can be trusted.
	virtual bool IsInitialized() const = 0;

	// Summary:
	//	 Returns the size in ms.
	// Return Value:
	//	 0 on looping sound or if buffer is not loaded (dialog).
	virtual int GetLengthMs() const = 0; 

	// Summary:
	//	 Returns the size of the stream in bytes.
	virtual int GetLengthInBytes() const = 0; // will soon change to:
	//virtual int GetLengthInBytes()=0;

	// Summary:
	//	 Retrieves the maximum distance of a sound.
	virtual float GetMaxDistance() const = 0;

	// Summary:
	//	 Retrieves the current distance multiplier of a sound.
	virtual float GetDistanceMultiplier() const = 0;

	virtual bool IsRelative() const = 0;

	//////////////////////////////////////////////////////////////////////////
	// EXTENDED CONTROL
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Modifies a line sound.
	virtual void SetLineSpec(const Vec3 &vStart, const Vec3 &vEnd) = 0;
	virtual bool GetLineSpec(  Vec3 &vStart,   Vec3 &vEnd) = 0;

	// Summary:
	//	 Modifies a sphere sound
	virtual void SetSphereSpec(const float fRadius) = 0;
	
	// Summary:
	//	 Sets to-be-skipped entities for obstruction calculation.
	virtual void SetPhysicsToBeSkipObstruction(EntityId *pSkipEnts,int nSkipEnts) = 0;

	// Summary:
	//	 Unloads the soundbuffer of this sound.
	virtual bool UnloadBuffer(eUnloadDataOptions UnloadOption) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	// INTERFACES
	//////////////////////////////////////////////////////////////////////////
	virtual ISound_Extended* GetInterfaceExtended() = 0;
	virtual ISound_Deprecated* GetInterfaceDeprecated() = 0;
	// </interfuscator:shuffle>
};

// Extended ISound Interface for use in Sandbox
// Functions calls in the Extended Interface should only be used after fully understanding the impact
// and the functionality. When using the data-driven SoundSystem most of these calls should not be called
// by the game
struct ISound_Extended : public ISound
{
	// <interfuscator:shuffle>
	//////////////////////////////////////////////////////////////////////////
	// PLAYBACK CONTROL
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Sets sound volume. Better control pitch using a parameter
	// Note:
	//	 Range: 0-1
	virtual	void	SetVolume( const float fVolume ) = 0;
	
	// Summary:
	//	 Gets sound volume. 
	virtual	float	GetVolume() const = 0;
  virtual float GetActiveVolume() const = 0;
	
	// Summary:
	//	 Sets the Sound Flags, SetFlags will overwrite all old Flags!
	virtual void SetFlags(uint32 const nFlags) = 0;

	// Summary:
	//	 Sets sound pitch. Better control pitch using a parameter
	// Note:
	//	 1000 is default pitch.
	virtual void SetPitch(int nPitch) = 0; // TODO internal
	
	// Summary:
	//	 Gets sound source velocity.
	virtual Vec3 const&	GetVelocity() const = 0;

	// Summary:
	//	 Sets the volume ratio.
	virtual void SetRatio(float fRatio) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	// MANIPULATION
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Set the currently played sample-pos in bytes or milliseconds.
	virtual void SetCurrentSamplePos(unsigned int nPos,bool bMilliSeconds) = 0;

	// Summary:
	//	 Retrieves the currently played sample-pos, in milliseconds or bytes.
	virtual unsigned int GetCurrentSamplePos(bool bMilliSeconds=false) const = 0;

	// Summary:
	//	 Sets sound priority (0-255).
	virtual void	SetSoundPriority(int nSoundPriority) = 0;	

	virtual bool Preload() = 0;

	// Summary:
	//	 Sets panning values (-1.0 left, 0.0 center, 1.0 right).
	virtual void	SetPan(float fPan) = 0; // TODO legacy
	virtual float GetPan() const = 0; // TODO legacy

	// Description:
	//	 Sets 3d panning values (0.0 no directional, 1.0 full 3d directional effect).
	virtual void	Set3DPan(float f3DPan) = 0; // TODO verify
	virtual float	Get3DPan() const = 0; // TODO verify

	//////////////////////////////////////////////////////////////////////////
	// INFORMATION
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Gets sound obstruction struct.
	virtual SObstruction* GetObstruction( void ) = 0;

	virtual bool IsInCategory(const char* sCategory) = 0;

	// Retrieves EntityID if the Sound is played on a SoundProxy
	virtual EntityId GetSoundProxyEntityID() = 0;
	// </interfuscator:shuffle>
};

// ISound Interface extension with functions which will be deprecated
struct ISound_Deprecated
{
	// <interfuscator:shuffle>
	virtual ~ISound_Deprecated(){}
	// Summary:
	//	 Sets automatic pitching amount (0-1000).
	virtual void SetPitching(float fPitching) = 0;  // TODO legacy

	// Description:
	//	 Set Minimal/Maximal distances for sound.
	//	 Sound is not attenuated below minimal distance and not eared outside of max distance.
	virtual void SetMinMaxDistance(float fMinDist, float fMaxDist) = 0; // TODO legacy

	// Summary:
	//	 Enable fx effects for this sound.
	// Note:
	//	 Must be called after each play
	virtual	void	FXEnable(int nEffectNumber)=0;

	virtual	void	FXSetParamEQ(float fCenter,float fBandwidth,float fGain)=0;

	// Summary:
	//	 Sets looping mode of sound.
	virtual void SetLoopMode(bool bLoop) = 0;

	// Summary:
	//	 Returns frequency of sound.
	virtual int	 GetFrequency() const = 0; // TODO legacy
	// </interfuscator:shuffle>
};

// Summary:
//	 A network sound listener.
UNIQUE_IFACE struct INetworkSoundListener
{
	// <interfuscator:shuffle>
	virtual ~INetworkSoundListener(){}
	// Summary:
	//	 Asks for buffer to be filled with data.
	virtual bool FillDataBuffer( unsigned int nBitsPerSample, unsigned int nSamplesPerSecond, unsigned int nNumSamples, void* pData ) = 0;

	virtual void OnActivate( bool active ) = 0;
	
	// Description:
	//	 Sets the distance at which sound playback switches between 2d and 3d.
	//	 Closer than max3dDistance: sound plays in 3d
	//	 Further than min2dDistance: sound plays in 2d
	//	 Between the two: linear transition.
	// Note:
	//	 NB: asserts max3dDistance >= min2dDistance
	virtual void SetSoundPlaybackDistances(float max3dDistance, float min2dDistance) = 0;

	// Summary:
	//	 Recalculate the sound's 3d pan based on position.
	virtual void UpdateSound3dPan() = 0;
	// </interfuscator:shuffle>
};

// Summary:
// A single microphone structure:
UNIQUE_IFACE struct IMicrophone
{
	// <interfuscator:shuffle>
	virtual ~IMicrophone(){}
	// Summary:
	//	 Starts recording and calling ReadDataBuffer on IMicrophoneStream.
	virtual bool Record(const unsigned int nRecordDevice, const unsigned int nBitsPerSample, const unsigned int nSamplesPerSecond, const unsigned int nBufferSizeInSamples) = 0;

	// Summary:
	//	 Stops recording and calling the callback. Can restart with Record.
	virtual bool Stop() = 0;

	// Summary:
	//	 Regular call to fill internal buffer.
	virtual void Update() = 0;

	// Summary:
	//	 Returns size of data in the internal buffer(valid until next call to Update()).
	virtual int GetDataSize()=0;

	// Summary:
	//	 Returns pointer to internal buffer (valid until next call to Update()).
	virtual int16* GetData()=0;

	// Summary:
	//	 Will tear down this microphone.
	virtual void Release() = 0;
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EXCLUDE_SCALEFORM_SDK

namespace CryVideoSoundSystem
{
	struct IChannelDelegate
	{
		// <interfuscator:shuffle>
		virtual void Release() = 0;

		virtual bool Stop() = 0;
		virtual bool SetPaused(bool pause) = 0;

		virtual bool SetVolume(float volume) = 0;
		virtual bool Mute(bool mute) = 0;

		virtual bool SetBytePosition(unsigned int bytePos) = 0;
		virtual bool GetBytePosition(unsigned int& bytePos) = 0;

		virtual bool SetSpeakerMix(float fl, float fr, float c, float lfe, float bl, float br, float sl, float sr) = 0;
		// </interfuscator:shuffle>

	protected:
		virtual ~IChannelDelegate() {}
	};

	struct ISoundDelegate
	{
		struct LockRange
		{
			void* p0;
			void* p1;
			unsigned int length0;
			unsigned int length1;
		};

		// <interfuscator:shuffle>
		virtual void Release() = 0;

		virtual IChannelDelegate* Play() = 0;

		virtual bool Lock(unsigned int offset, unsigned int length, LockRange& lr) = 0;
		virtual bool Unlock(const LockRange& lr) = 0;
		// </interfuscator:shuffle>

	protected:
		virtual ~ISoundDelegate() {}
	};

	struct IPlayerDelegate
	{
		// <interfuscator:shuffle>
		virtual void Release() = 0;

		virtual ISoundDelegate* CreateSound(unsigned int numChannels, unsigned int sampleRate, unsigned int lengthInBytes) = 0;

		virtual bool MuteMainTrack() const = 0;
		// </interfuscator:shuffle>

	protected:
		virtual ~IPlayerDelegate() {}
	};

	struct IAllocatorDelegate
	{
		// <interfuscator:shuffle>
		virtual void* Allocate(size_t size) = 0;
		virtual void Free(void* p) = 0;
		// </interfuscator:shuffle>

	protected:
		virtual ~IAllocatorDelegate() {}
	};
} // namespace CryVideoSoundSystem

#endif //#ifndef EXCLUDE_SCALEFORM_SDK

//////////////////////////////////////////////////////////////////////////////////////////////
typedef ISoundSystem* (*PFNCREATESOUNDSYSTEM)(struct ISystem*, void*);

#ifdef CRYSOUNDSYSTEM_EXPORTS
	#define CRYSOUND_API DLL_EXPORT
#else
	#define CRYSOUND_API DLL_IMPORT
#endif

extern "C"
{
	CRYSOUND_API ISoundSystem* CreateSoundSystem(struct ISystem*, void *pInitData);
}

//////////////////////////////////////////////////////////////////////////
// interface that passes all parameters via a structures
// this allows having stable interface methods and flexible default arguments system
// see physics code for the original idea, tried to do this a bit nicer
//////////////////////////////////////////////////////////////////////////
// Generic ptParam structure that offers Get/SetValue, which returns bool
// if that query was successful and the type checking was ok
//
// Use this by implementing an abstract interface to get/set parameter
// bool	GetParam(enumParamSemantics eSemantics, ptParam* pParam);
// bool	SetParam(enumParamSemantics eSemantics, ptParam* pParam);
// 
// ptParamINT32 NewParam(pspNumberOfSomething, 42);
// m_pMyImpl->SetParam(&NewParam);
//
// Then internally use the Semantics to tell what the value means
// switch (eSemantics)
//
//	case pspNumberOfSomething:
//		int32 nTemp;
//		if (!(pParam->GetValue(nTemp))) return (false);
//		if (!CS_SetSomethingToFmod(m_nChannel,nTemp)) return (false);
//		break;
//////////////////////////////////////////////////////////////////////////

// Summary:
//	 Some generic parameter types.
enum enumParamType
{
	ptBOOL,
	ptVOIDP,
	ptCHARP,
	ptF32,
	ptINT32,
	ptSIZE_T,
	ptCRYSTRING,
	ptVEC3F32,
};

////////////////////////// Params structures /////////////////////

struct ptParamBOOL;
struct ptParamVOIDP;
struct ptParamCHARP;
struct ptParamF32;
struct ptParamINT32;
struct ptParamSIZE_T;
struct ptParamCRYSTRING;
struct ptParamVEC3F32;

template<typename T> struct Map2ptParam;
template<> struct Map2ptParam<bool>        { typedef bool        Tkey; typedef ptParamBOOL      Tval; enum { TypeId = ptBOOL }; };
template<> struct Map2ptParam<void*>       { typedef void*       Tkey; typedef ptParamVOIDP     Tval; enum { TypeId = ptVOIDP }; };
template<> struct Map2ptParam<char const*> { typedef char const* Tkey; typedef ptParamCHARP     Tval; enum { TypeId = ptCHARP }; };
template<> struct Map2ptParam<f32>         { typedef f32         Tkey; typedef ptParamF32       Tval; enum { TypeId = ptF32 }; };
template<> struct Map2ptParam<int32>       { typedef int32       Tkey; typedef ptParamINT32     Tval; enum { TypeId = ptINT32 }; };
template<> struct Map2ptParam<size_t>      { typedef size_t      Tkey; typedef ptParamSIZE_T    Tval; enum { TypeId = ptSIZE_T }; };
template<> struct Map2ptParam<string>      { typedef string      Tkey; typedef ptParamCRYSTRING Tval; enum { TypeId = ptCRYSTRING }; };
template<> struct Map2ptParam<Vec3>        { typedef Vec3        Tkey; typedef ptParamVEC3F32   Tval; enum { TypeId = ptVEC3F32 }; };

// Todo:
//	 Rename.
struct ptParam
{
protected:
	ptParam(const enumParamType eType) : m_eType(eType) {}

	enumParamType m_eType;

public:
	// Summary:
	//	 Template class to the given type with the value and return if type check was ok.
	template<typename T> bool GetValue(T &outVal) const
	{
		if (m_eType == (enumParamType)Map2ptParam<T>::TypeId)
		{
			outVal = ( *static_cast<const typename Map2ptParam<T>::Tval*>(this) ).m_val;
			return true;
		}
		return false; // type check failed
	}

	// Summary:
	//	 Template class to set the given type with the value and return if type check was ok.
	template<typename T> bool SetValue(const T &inVal)
	{
		if (m_eType == (enumParamType)Map2ptParam<T>::TypeId)
		{
			( *static_cast<typename Map2ptParam<T>::Tval*>(this) ).m_val = inVal;
			return true;
		}
		return false; // type check failed
	}
};

struct ptParamBOOL : ptParam 
{ 
	friend struct ptParam;
	ptParamBOOL(bool newVal) : ptParam(ptBOOL), m_val(newVal) {}
private:
	bool m_val;
};

struct ptParamVOIDP : ptParam
{
	friend struct ptParam;
	ptParamVOIDP(void* newVal) : ptParam(ptVOIDP), m_val(newVal) {}
private:
	void* m_val;
};

struct ptParamCHARP : ptParam
{
	friend struct ptParam;
	ptParamCHARP(char const* newVal) : ptParam(ptCHARP), m_val(newVal) {}
private:
	char const* m_val;
};

struct ptParamF32 : ptParam 
{ 
	friend struct ptParam;
	ptParamF32(f32 newVal) : ptParam(ptF32), m_val(newVal) {} 
private:
	f32 m_val;
};

struct ptParamINT32 : ptParam 
{ 
	friend struct ptParam;
	ptParamINT32(int32 newVal) : ptParam(ptINT32), m_val(newVal) {}
private:
	int32 m_val;
};

struct ptParamSIZE_T : ptParam 
{ 
	friend struct ptParam;
	ptParamSIZE_T(size_t newVal) : ptParam(ptSIZE_T), m_val(newVal) {}
private:
	size_t m_val;
};

struct ptParamCRYSTRING : ptParam 
{ 
	friend struct ptParam;
	ptParamCRYSTRING(const string& newVal) : ptParam(ptCRYSTRING), m_val(newVal) {}
private:
	string m_val;
};

struct ptParamVEC3F32 : ptParam 
{ 
	friend struct ptParam;
	ptParamVEC3F32(const Vec3& newVal) : ptParam(ptVEC3F32), m_val(newVal) {}
private:
	Vec3 m_val;
};

#endif // CRYSOUND_ISOUND_H
