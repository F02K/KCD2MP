////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   IReverbManager.h
//  Version:     v1.00
//  Created:     15/8/2005 by Tomas.
//  Compilers:   Visual Studio.NET
//  Description: ReverbManager offers an abstraction layer to handle reverb.
//							 New environments can be activated and automatically blending
//							 into old ones.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IREVERBMANAGER_H__
#define __IREVERBMANAGER_H__

#include "SerializeFwd.h"

#pragma once

#define REVERB_PRESETS_FILENAME_LUA	"Libs/ReverbPresets/ReverbPresetDB.lua"
#define REVERB_PRESETS_FILENAME_XML	"Libs/ReverbPresets/ReverbPresets.xml"

#define REVERB_TYPE_NONE 0
#define REVERB_TYPE_SOFTWARE 1
#define REVERB_TYPE_SOFTWARE_PREALLOC_BUFFERS 2

// Reverb min, max and defaults
// Master level
#define CRYVERB_ROOM_MAX                  0
#define CRYVERB_ROOM_MIN                  -10000
#define CRYVERB_ROOM_DEFAULT              -1000

// HF gain
#define CRYVERB_ROOM_HF_MAX               0
#define CRYVERB_ROOM_HF_MIN               -10000
#define CRYVERB_ROOM_HF_DEFAULT           -100

// LF gain
#define CRYVERB_ROOM_LF_MAX               0
#define CRYVERB_ROOM_LF_MIN               -10000
#define CRYVERB_ROOM_LF_DEFAULT           0

// Decay time
#define CRYVERB_DECAY_TIME_MAX            20.0f
#define CRYVERB_DECAY_TIME_MIN            0.1f
#define CRYVERB_DECAY_TIME_DEFAULT        1.49f

// Decay ratio
#define CRYVERB_DECAY_HF_RATIO_MAX        2.0f
#define CRYVERB_DECAY_HF_RATIO_MIN        0.1f
#define CRYVERB_DECAY_HF_RATIO_DEFAULT    0.83f

// Early reflections
#define CRYVERB_REFLECTIONS_MAX           1000
#define CRYVERB_REFLECTIONS_MIN           -10000
#define CRYVERB_REFLECTIONS_DEFAULT       -2602

// Pre delay
#define CRYVERB_REFLECTIONS_DELAY_MAX     0.3f
#define CRYVERB_REFLECTIONS_DELAY_MIN     0.0f
#define CRYVERB_REFLECTIONS_DELAY_DEFAULT 0.007f

// Late reflections
#define CRYVERB_REVERB_MAX                2000
#define CRYVERB_REVERB_MIN                -10000
#define CRYVERB_REVERB_DEFAULT            200

// Late delay
#define CRYVERB_REVERB_DELAY_MAX          0.1f
#define CRYVERB_REVERB_DELAY_MIN          0.0f
#define CRYVERB_REVERB_DELAY_DEFAULT      0.011f

// HF crossover
#define CRYVERB_HF_REFERENCE_MAX          20000.0f
#define CRYVERB_HF_REFERENCE_MIN          20.0f
#define CRYVERB_HF_REFERENCE_DEFAULT      5000.0f

// LF crossover
#define CRYVERB_LF_REFERENCE_MAX          1000.0f
#define CRYVERB_LF_REFERENCE_MIN          20.0f
#define CRYVERB_LF_REFERENCE_DEFAULT      250.0f

// Diffusion
#define CRYVERB_DIFFUSION_MAX             100.0f
#define CRYVERB_DIFFUSION_MIN             0.0f
#define CRYVERB_DIFFUSION_DEFAULT         100.0f

// Density
#define CRYVERB_DENSITY_MAX               100.0f
#define CRYVERB_DENSITY_MIN               0.0f
#define CRYVERB_DENSITY_DEFAULT           100.0f

// Reverb Presets
//////////////////////////////////////////////////////////////////////////////////////////////
enum SOUND_REVERB_PRESETS{
	REVERB_PRESET_OFF=0,              
	REVERB_PRESET_GENERIC,          
	REVERB_PRESET_PADDEDCELL,       
	REVERB_PRESET_ROOM, 	           
	REVERB_PRESET_BATHROOM, 	       
	REVERB_PRESET_LIVINGROOM,       
	REVERB_PRESET_STONEROOM,        
	REVERB_PRESET_AUDITORIUM,       
	REVERB_PRESET_CONCERTHALL,      
	REVERB_PRESET_CAVE,             
	REVERB_PRESET_ARENA,            
	REVERB_PRESET_HANGAR,           
	REVERB_PRESET_CARPETTEDHALLWAY, 
	REVERB_PRESET_HALLWAY,          
	REVERB_PRESET_STONECORRIDOR,    
	REVERB_PRESET_ALLEY, 	       
	REVERB_PRESET_FOREST, 	       
	REVERB_PRESET_CITY,             
	REVERB_PRESET_MOUNTAINS,        
	REVERB_PRESET_QUARRY,           
	REVERB_PRESET_PLAIN,            
	REVERB_PRESET_PARKINGLOT,       
	REVERB_PRESET_SEWERPIPE,        
	REVERB_PRESET_UNDERWATER,
	REVERB_PRESET_DRUGGED,
	REVERB_PRESET_DIZZY,
};

struct IAudioDevice;
struct ISound;
class CSoundSystem;

typedef struct CRYSOUND_REVERB_PROPERTIES /* MIN     MAX    DEFAULT   DESCRIPTION */
{
  int   nRoom;             /* [in/out] -10000, 0,        -1000,   master level */
	int   nRoomHF;           /* [in/out] -10000, 0,        -100,    hf gain */
	int   nRoomLF;           /* [in/out] -10000, 0,        0,       lf gain */
	float fDecayTime;        /* [in/out] 0.1f,   20.0f,    1.49f,   decay time */
	float fDecayHFRatio;     /* [in/out] 0.1f,   2.0f,     0.83f,   decay ratio */
	int   nReflections;      /* [in/out] -10000, 1000,     -2602,   early reflections */
	float fReflectionsDelay; /* [in/out] 0.0f,   0.3f,     0.007f,  pre delay */
	int   nReverb;           /* [in/out] -10000, 2000,     200,     late reflections */
	float fReverbDelay;      /* [in/out] 0.0f,   0.1f,     0.011f,  late delay */
	float fHFReference;      /* [in/out] 20.0f,  20000.0f, 5000.0f, hf crossover */
  float fLFReference;      /* [in/out] 20.0f,  1000.0f,  250.0f,  lf crossover */
  float fDiffusion;        /* [in/out] 0.0f,   100.0f,   100.0f,  diffusion */
  float fDensity;          /* [in/out] 0.0f,   100.0f,   100.0f,  density */
}CRYSOUND_REVERB_PROPERTIES;

typedef struct CRYSOUND_ECHO_PROPERTIES /* MIN     MAX    DEFAULT   DESCRIPTION */
{
	CRYSOUND_ECHO_PROPERTIES()
		:	fDelay(0.0f),
			fDecayRatio(0.0f),
			fDryMix(0.0f),
			fWetMix(0.0f){}

	float	fDelay;				/* [in/out] 10.0f,	5000.0f,	500.0f,	Echo delay in ms.*/
	float	fDecayRatio;	/* [in/out] 0.0f,		1.0f,			0.5f,		Echo decay per delay. 0.0f to 1.0f, 1.0f = No decay, 0.0f = Total decay (ie simple 1 line delay).*/
	float	fDryMix;			/* [in/out] 0.0f,		1.0f,			1.0f,		Volume of original signal to pass to output.*/
	float	fWetMix;			/* [in/out] 0.0f,		1.0f,			1.0f,		Volume of echo signal to pass to output.*/
}CRYSOUND_ECHO_PROPERTIES;

#define CRYSOUND_REVERB_PRESET_ZERO    {0, 0, 0, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
#define CRYSOUND_REVERB_PRESET_OFF     {-10000, -10000, -10000, 0.1f, 0.1f, -10000, 0.0f, -10000, 0.0f, 20.0f, 20.0f, 0.0f, 0.0f}
#define CRYSOUND_REVERB_PRESET_INDOOR  {-1000, -100, 0, 1.49f, 0.83f, -2602, 0.007f, 200, 0.011f, 5000.0f, 250.0f, 100.0f, 100.0f}
#define CRYSOUND_REVERB_PRESET_OUTDOOR {-2000, -100, 0, 0.2f, 0.83f, -2602, 0.015f, -500, 0.011f, 5000.0f, 250.0f, 100.0f, 100.0f}
#define CRYSOUND_REVERB_UNDERWATER     {-1000, -4000, 0, 1.49f, 0.1f, -449, 0.007f, 1700, 0.011f, 5000.0f, 250.0f, 100.0f, 100.0f}

#define MAXCHARBUFFERSIZE 512
typedef CryFixedStringT<MAXCHARBUFFERSIZE>	TFixedResourceName;

// Summary:
//	 Reverb interface structures.
struct SReverbInfo
{
	//struct SReverbProps
	//{
	//	CRYSOUND_REVERB_PROPERTIES EAX;
	//	int nPreset;
	//};

	struct SReverbPreset
	{
		CryFixedStringT<64> sPresetName;
		CRYSOUND_REVERB_PROPERTIES EAX;
		CRYSOUND_ECHO_PROPERTIES EchoDSP;
		void GetMemoryUsage(ICrySizer *pSizer ) const{}
	};

	struct SWeightedReverbPreset
	{
		SWeightedReverbPreset()
			:	pReverbPreset(NULL),
				fWeight(0.0f),
				bFullEffectWhenInside(false),
				RegID(0){}

		SReverbPreset* pReverbPreset;
		float fWeight;
		bool bFullEffectWhenInside;
		EntityId RegID;		// Unique ID to identify who registered the preset.
		void GetMemoryUsage(ICrySizer *pSizer ) const{}
	};

	struct Data
	{
		SReverbPreset *pPresets;
		int nPresets;
		//Mood *pMoods;
		//int nMoods;
		//Pattern *pPatterns;
		//int nPatterns;
	};

};

struct IReverbManager
{
	// <interfuscator:shuffle>
	virtual ~IReverbManager(){}
	//////////////////////////////////////////////////////////////////////////
	// Initialization
	//////////////////////////////////////////////////////////////////////////

	//
	virtual void Init(IAudioDevice *pAudioDevice, CSoundSystem* pSoundSystem) = 0;
	virtual bool SelectReverb(int nReverbType) = 0;

	virtual void Reset() = 0;
	virtual void Release() = 0;

	virtual bool SetData( SReverbInfo::Data *pReverbData ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Information
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	// Writes output to screen in debug.
	virtual void DrawInformation(IRenderer* pRenderer, float xpos, float ypos) = 0;

	// Draws the ray cast lines plus spheres at the collision points
	virtual void DrawDynamicReverbDebugInfo(IRenderer* const pRenderer) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Management
	//////////////////////////////////////////////////////////////////////////

	// Note:
	//	 Needs to be called regularly.
	virtual void Update(bool const bInside) = 0;

	virtual bool SetListenerReverb(SOUND_REVERB_PRESETS nPreset, CRYSOUND_REVERB_PROPERTIES *tpProps=NULL, uint32 nFlags=0) = 0;
	
	// Preset management
	//##@{
	virtual bool RegisterReverbPreset(char const* const pcPresetName=NULL, CRYSOUND_REVERB_PROPERTIES const* const pReverbProperties=NULL, CRYSOUND_ECHO_PROPERTIES const* const pEchoProperties=NULL) = 0;
	virtual bool UnregisterReverbPreset(const char *sPresetName=NULL) = 0;
	//##@}

	// Description:
	//	 Registers an EAX Preset Area with the current blending weight (0-1) as being active
	//	 morphing of several EAX Preset Areas is done internally.
	// Note:
	//	 Registering the same Preset multiple time will only overwrite the old one
	virtual bool RegisterWeightedReverbEnvironment(const char *sPreset=NULL, EntityId RegID=0, bool bFullEffectWhenInside=false, uint32 nFlags=0) = 0;
	//virtual bool RegisterWeightedReverbEnvironment(const char *sPreset=NULL, CRYSOUND_REVERB_PROPERTIES *pProps=NULL, bool bFullEffectWhenInside=false, uint32 nFlags=0) = 0;
	//virtual bool RegisterWeightedReverbEnvironment(const char *sPreset=NULL, SOUND_REVERB_PRESETS nPreset, bool bFullEffectWhenInside=false, uint32 nFlags=0) = 0;

	// Description:
	//	 Updates an EAX Preset Area with the current blending ratio (0-1).
	virtual bool UpdateWeightedReverbEnvironment(const char *sPreset=NULL, EntityId RegID=0, float fWeight=0.0) = 0;

	// Summary:
	//	 Unregisters an active EAX Preset Area.
	virtual bool UnregisterWeightedReverbEnvironment(const char *sPreset=NULL, EntityId RegID=0) = 0;

	// Summary:
	//	 Gets current EAX listener environment.
	virtual bool GetCurrentReverbEnvironment(SOUND_REVERB_PRESETS &nPreset, CRYSOUND_REVERB_PROPERTIES &Props) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Dependency with loading a sound
	//////////////////////////////////////////////////////////////////////////

	virtual float GetEnvironment(EntityId const nEntityID, Vec3 const& rPosition) = 0;

	//virtual bool RegisterSound(ISound *pSound) = 0;
	//virtual int  GetReverbInstance(ISound *pSound) const = 0;

	//virtual bool RegisterPlayer(EntityId PlayerID) = 0;
	//virtual bool UnregisterPlayer(EntityId PlayerID) = 0;

	// virtual bool EvaluateSound(ISound *pSound);

	//virtual SSGHandle ImportXMLFile(const string sGroupName) = 0;

	virtual void SerializeState(TSerialize ser) = 0;


	virtual void GetMemoryUsage( ICrySizer *pSizer ) const =0;
	//////////////////////////////////////////////////////////////////////////
	// ISoundEventListener implementation
	//////////////////////////////////////////////////////////////////////////
//	virtual void OnSoundEvent( ESoundCallbackEvent event,ISound *pSound );

	// Summary:
	// Saves the current reverb preset setup and removes reverb completely on passing true.
	// Restores the previous reverb preset setup on passing false.
	virtual void TogglePause(bool const bPause) = 0;
	// </interfuscator:shuffle>
};
#endif
