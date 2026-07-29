/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2004.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: The Editor->Game communication interface.
  
 -------------------------------------------------------------------------
  History:
  - 30:8:2004   11:11 : Created by Márcio Martins

*************************************************************************/
#include DEVIRTUALIZE_HEADER_FIX(IEditorGame.h)

#ifndef __IEDITORGAME_H__
#define __IEDITORGAME_H__

#if _MSC_VER > 1000
# pragma once
#endif

struct IFlowSystem;
struct IGameTokenSystem;
namespace Telemetry { struct ITelemetryRepository; }

// For game to access Editor functionality.
struct IGameToEditorInterface
{
	// <interfuscator:shuffle>
	virtual	~IGameToEditorInterface(){}
	virtual void SetUIEnums( const char *sEnumName,const char **sStringsArray,int nStringCount ) = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IEquipmentSystemInterface
{
	struct IEquipmentItemIterator
	{
		struct SEquipmentItem
		{
			const char* name;
			const char* type;
		};
		// <interfuscator:shuffle>
		virtual	~IEquipmentItemIterator(){}
		virtual void AddRef() = 0;
		virtual void Release() = 0;
		virtual bool Next(SEquipmentItem& outItem) = 0;
		// </interfuscator:shuffle>
	};
	typedef _smart_ptr<IEquipmentItemIterator> IEquipmentItemIteratorPtr;
	
	// <interfuscator:shuffle>
	virtual	~IEquipmentSystemInterface(){}

	// return iterator with available equipment items of a certain type
	// type can be empty to retrieve all items
	virtual IEquipmentSystemInterface::IEquipmentItemIteratorPtr CreateEquipmentItemIterator(const char* type="") = 0;
	virtual IEquipmentSystemInterface::IEquipmentItemIteratorPtr CreateEquipmentAccessoryIterator(const char* type) = 0;

	// delete all equipment packs
	virtual void DeleteAllEquipmentPacks() = 0;

	// load a single equipment pack from an XmlNode
	// Equipment Pack is basically
	// <EquipPack name="BasicPack">
	//   <Items>
	//      <Scar type="Weapon" />
	//      <SOCOM type="Weapon" />
	//   </Items>
	//   <Ammo Scar="50" SOCOM="70" />
	// </EquipPack>

	virtual bool LoadEquipmentPack(const XmlNodeRef& rootNode) = 0;	

	// set the players equipment pack. maybe we enable this, but normally via FG only
	// virtual void SetPlayerEquipmentPackName(const char *packName) = 0;
	// </interfuscator:shuffle>
};

// Summary
//		Interface used by the Editor to interact with the GameDLL
UNIQUE_IFACE struct IEditorGame
{
	typedef IEditorGame *(*TEntryFunction)();
	
	// <interfuscator:shuffle>
	virtual	~IEditorGame(){}
	virtual bool Init( ISystem *pSystem,IGameToEditorInterface *pEditorInterface ) = 0;
	virtual int Update(bool haveFocus, unsigned int updateFlags) = 0;
	virtual void Shutdown() = 0;
	virtual bool SetGameMode(bool bGameMode) = 0;
	virtual IEntity * GetPlayer() = 0;
	virtual void SetPlayerPosAng(Vec3 pos,Vec3 viewDir) = 0;
	virtual void HidePlayer(bool bHide) = 0;
	virtual void OnBeforeLevelLoad() = 0;
	virtual void OnAfterLevelLoad(const char *levelName, const char *levelFolder) = 0;
	virtual void OnCloseLevel() = 0;
	virtual bool BuildEntitySerializationList(XmlNodeRef output) = 0;
	virtual bool GetAdditionalMinimapData(XmlNodeRef output) = 0;

	virtual IFlowSystem * GetIFlowSystem() = 0;
	virtual IGameTokenSystem* GetIGameTokenSystem() = 0;

	virtual IEquipmentSystemInterface* GetIEquipmentSystemInterface() = 0;

	virtual bool SupportsMultiplayerGameRules() { return false; }
	virtual void ToggleMultiplayerGameRules() {}

	// telemetry functions: possibly should find a better place for these
	virtual void RegisterTelemetryTimelineRenderers(Telemetry::ITelemetryRepository* pRepository) = 0;

	virtual void OnDisplayRenderUpdated( bool displayHelpers ) = 0;
	// </interfuscator:shuffle>
};


#endif //__IEDITORGAME_H__