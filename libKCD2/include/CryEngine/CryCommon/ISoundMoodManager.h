////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   ISoundMoodManager.h
//  Version:     v1.00
//  Created:     13/5/2005 by Tomas.
//  Compilers:   Visual Studio.NET
//  Description: SoundManager is responsible for organising groups of sounds,
//							 creating new sounds by combinging sound settings and assets
//							 and caching sound settings and distance curves 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(ISoundMoodManager.h)

#ifndef __ISOUNDMOODMANAGER_H__
#define __ISOUNDMOODMANAGER_H__

#include "SerializeFwd.h"

#pragma once

#include <IMemory.h> // <> required for Interfuscator

#define SOUNDMOOD_FILENAME	"Libs/SoundMoods/SoundMoods.xml"

#define FLAG_SOUNDMOOD_MUTED						1				//1<<1
#define FLAG_SOUNDMOOD_SOLO							2				//1<<2 

#define FLAG_SOUNDMOOD_MOODTYPE					256			//1<<8 	
#define FLAG_SOUNDMOOD_CATEGORYTYPE			512			//1<<9 	
#define FLAG_SOUNDMOOD_CATEGORYSUBTYPE	1024		//1<<10 

enum enumSG_SEARCHDIRECTION
{
	eELEMENTONLY,
	eSEARCH_PARENTS,
	eSEARCH_CHILDREN
};

enum enumGroupParamSemantics
{
	gspNONE,
	gspFLAGS,
	gspFVOLUME,
	gspFPITCH,
	gspFLOWPASSCUTOFF,
	gspFHIGHPASSCUTOFF,
	gspFHIGHPASSRESONANCE,
	gspFCOMPRESSORTHRESHOLD,
	gspFCOMPRESSORATTACK,
	gspFCOMPRESSORRELEASE,
	gspFCOMPRESSORGAINMAKEUP,
	gspFPARAMEQ1CENTER,
	gspFPARAMEQ1BANDWIDTH,
	gspFPARAMEQ1GAIN,
	gspFPARAMEQ2CENTER,
	gspFPARAMEQ2BANDWIDTH,
	gspFPARAMEQ2GAIN,
	gspMUTED,
	gspSOLO,
	gspLOOPED,
	gspLAST
};

UNIQUE_IFACE struct IReadPCMDataListener 
{
	// <interfuscator:shuffle>
	virtual ~IReadPCMDataListener(){}
	virtual void OnReadPCMData(float const* const pfBuffer, int unsigned const nLengthInSamples, int const nChannels, void const* const pUserData = NULL) = 0;
	// </interfuscator:shuffle>
};

// forward declaration

struct ISoundMoodManager;

#define MAXCHARSOUNDMOODNAME 256
typedef CryFixedStringT<MAXCHARSOUNDMOODNAME>	TFixedSoundMoodName;

UNIQUE_IFACE struct ICategory : public _i_reference_target_t
{
	// <interfuscator:shuffle>
	// Summary:
	//	 Gets the name.
	virtual const char*	GetName() const  = 0;
	// Summary:
	//	 Sets the name.
	virtual void		SetName(const char* sCategoryName) = 0;
	// Summary:
	//	 Gets the mood name.
	virtual const char* GetMoodName() const = 0;

	// Summary:
	//	 Gets the platform category.
	virtual void*				GetPlatformCategory() const = 0;
	// Summary:
	//	 Sets the platform category
	virtual void				SetPlatformCategory(void *pPlatformCategory) = 0;

	// Summary:
	//	 Checks if it is muted.
	virtual bool				GetMuted() const = 0;
	// Summary:
	//	 Sets muted.
	virtual void				SetMuted(bool bMuted) = 0;

	// Summary:
	//	 Gets parent category.
	virtual ICategory*			GetParentCategory() const = 0;
	// Summary:
	//	 Sets parent category
	virtual void				SetParentCategory(ICategory* pParent) = 0;

	// Summary:
	//	 Gets the number of children.
	virtual uint32		GetChildCount() = 0;
	virtual ICategory*	GetCategoryByIndex(uint32 nCategoryCount) const = 0;
	virtual ICategory*	GetCategory(const char* sCategoryName) const = 0;
	virtual ICategory*	GetCategoryByPtr(void *pPlatformCategory) const = 0;
	virtual ICategory*	AddCategory(const char* sCategoryName) = 0;
	virtual bool				RemoveCategory(const char *sCategoryName) = 0;

	// Summary:
	//   Gets the internal flag field for attributes
	virtual uint32 GetAttributes() const = 0;

	// Summary:
	//	 Gets parameter.
	virtual bool				GetParam(enumGroupParamSemantics eSemantics, ptParam* pParam) const = 0;
	// Summary:
	//	 Sets parameter.
	virtual bool				SetParam(enumGroupParamSemantics eSemantics, ptParam* pParam) = 0;

	// Import Export
	//##@{
	virtual bool				Serialize(XmlNodeRef &node, bool bLoading) = 0;
	virtual bool				RefreshCategories(const bool bAddNew) = 0;
	//##@}

	// Functionality.
	virtual bool ApplyCategory(float fFade) = 0;


	// Other.

	// Summary:
	//	 Gets a pointer to the mood manager.
	virtual ISoundMoodManager* GetIMoodManager() = 0;

	virtual void GetMemoryUsage( ICrySizer *pSizer ) const=0;

	virtual bool const	AddReadPCMDataListener(IReadPCMDataListener* const pListener, void const* pUserData = NULL) = 0;
	virtual bool const	RemoveReadPCMDataListener(IReadPCMDataListener* const pListener) = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IMood : public _i_reference_target_t
{
	// <interfuscator:shuffle>
	// Summary:
	//	 Resets to initial state.
	virtual void Reset() = 0;

	// Summary:
	//	 Gets the name.
	virtual const char*	GetName() const = 0;
	// Summary:
	//	 Sets the name.
	virtual void SetName(const char* sMoodName) = 0;

	// Summary:
	//	 Gets master category
	virtual ICategory*	GetMasterCategory() const = 0;
	
	// Summary:
	//	 Gets the number of categories.
	virtual size_t			GetCategoryCount() const = 0;
	// Summary:
	//	 Gets a category from its index.
	virtual ICategory*	GetCategoryByIndex(uint32 nCategoryCount) const = 0;
	// Summary:
	//	 Gets a category from its name.
	virtual ICategory*	GetCategory(const char *sCategoryName)  const = 0;
	// Gets a category from its pointer.
	virtual ICategory*	GetCategoryByPtr(void* pPlatformCategory) const = 0;
	// Summary:
	//	 Adds a category.
	virtual ICategory*	AddCategory(const char *sCategoryName) = 0;
	// Summary:
	//	 Removes a category.
	virtual bool		RemoveCategory(const char *sCategoryName) = 0;

	// Summary:
	//	 Gets the priority.
	virtual float           GetPriority() const = 0;
	// Summary:
	//	 Sets the priority.
	virtual void            SetPriority(float fPriority) = 0;
	// Summary:
	//	 Gets the music volume.
	virtual float           GetMusicVolume() const = 0;
	// Summary:
	//	 Sets the music volume.
	virtual void            SetMusicVolume(float fMusicVolume) = 0;

	// Import Export
	//##@{
	virtual bool Serialize(XmlNodeRef &node, bool bLoading) = 0;
	virtual void SetIsMixMood(const bool bIsMixMood) = 0;
	virtual bool GetIsMixMood() const = 0;
	//##@}


	// Other

	// Summary:
	//	 Gets a pointer to the mood manager.
	virtual ISoundMoodManager* GetIMoodManager() const = 0;

	virtual void GetMemoryUsage(ICrySizer *pSizer) const=0;
	// </interfuscator:shuffle>
};



UNIQUE_IFACE struct ISoundMoodManager
{
	// <interfuscator:shuffle>
	virtual ~ISoundMoodManager(){}
	//////////////////////////////////////////////////////////////////////////
	// Initialization.
	//////////////////////////////////////////////////////////////////////////

	virtual bool RefreshCategories(IMood *pMood) = 0;
	virtual bool RefreshPlatformCategories() = 0;
	virtual void Release() = 0;

	virtual void Reset(bool bUnload) = 0; // removes all soundmoods except "default" and resets the effect

	//////////////////////////////////////////////////////////////////////////
	// Memory Usage.
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Returns current Memory Usage.
	virtual uint32 GetMemUsage(void) const = 0;

	//////////////////////////////////////////////////////////////////////////
	// Information.
	//////////////////////////////////////////////////////////////////////////

	virtual uint32 GetNumberSoundsLoaded(void) const = 0;

	//////////////////////////////////////////////////////////////////////////
	// Management.
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Adds/removes a Mood to the MoodManager database.
	virtual IMood*			AddMood(const char *sMoodName) = 0;
	virtual bool				RemoveMood(const char *sMoodName) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Real Time Manipulation.
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Needs to be updated regularly.
	virtual bool Update() = 0;

	// Summary:
	//	 Registers a Mood to be actively processes (starts with 0 fade value).
	virtual bool	RegisterSoundMood(const char *sMoodName) = 0;

	// Summary:
	//	 Updates the fade value of a registered mood.
	virtual bool	UpdateSoundMood(const char *sMoodName, float fFade, uint32 nFadeTimeInMS, bool bUnregistedOnFadeOut=true) = 0;
	
	// Summary:
	//	 Gets current fade value of a registered mood.
	virtual float	GetSoundMoodFade(const char *sMoodName) = 0;

	// Summary:
	//	 Unregisters a Mood and removes it from the active ones.
	virtual bool	UnregisterSoundMood(const char *sMoodName) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Access.
	//////////////////////////////////////////////////////////////////////////

	virtual IMood*			GetMoodPtr(uint32 nGroupCount) const = 0;			// may be NULL
	virtual IMood*			GetMoodPtr(const char *sMoodName) const = 0;	// may be NULL
	virtual IMood*			GetMixMoodPtr() const = 0;										// may be NULL

	//////////////////////////////////////////////////////////////////////////
	// Import/Export.
	//////////////////////////////////////////////////////////////////////////

	virtual bool Serialize(XmlNodeRef &node, bool bLoading) = 0;
	virtual void SerializeState(TSerialize ser) = 0;

	//////////////////////////////////////////////////////////////////////////
	// VU Meter
	//////////////////////////////////////////////////////////////////////////

	virtual bool const	AddVUListenerCategory(ICategory* const pCategory) = 0;
	virtual bool const	RemoveVUListenerCategory(ICategory* const pCategory) = 0;
	virtual void				ClearVUListenerCategories() = 0;
	virtual float const	GetWaveDataMaxAmplitude(int const nChannel) const = 0;

	//Other

	//
	virtual bool RefreshCategories() = 0;

	virtual void GetMemoryUsage(ICrySizer *pSizer) const=0;
	// </interfuscator:shuffle>
};
#endif
