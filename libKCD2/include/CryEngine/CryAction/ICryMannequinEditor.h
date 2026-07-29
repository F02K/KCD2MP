////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2011.
//
////////////////////////////////////////////////////////////////////////////
#ifndef __I_CRY_MANNEQUIN_EDITOR_H__
#define __I_CRY_MANNEQUIN_EDITOR_H__

#include "ICryMannequin.h"


enum EFileEntryType { eFET_Database, eFET_ControllerDef, eFET_TagDef };


struct IMannequinWriter
{	
	virtual ~IMannequinWriter() {}
	virtual void SaveFile(const char *szFilename, XmlNodeRef xmlNode, EFileEntryType fileEntryType) = 0;
};


struct IMannequinEditorListener
{
	virtual ~IMannequinEditorListener() {}
	virtual void OnMannequinTagDefInvalidated(const CTagDefinition& tagDefinition) = 0;
};

struct SAnimDBSnapshot
{
	IAnimationDatabase *pDatabase;
	XmlNodeRef xmlData;
};

typedef DynArray<SAnimDBSnapshot> TAnimDBSnapshotCollection;

enum EModifyFragmentIdResult
{
	eMFIR_Success,
	eMFIR_DuplicateName,
	eMFIR_InvalidNameIdentifier,
	eMFIR_UnknownInputTagDefinition,
	eMFIR_InvalidFragmentId,
};

struct IMannequinEditorManager
{
	virtual ~IMannequinEditorManager() {}

	virtual EModifyFragmentIdResult CreateFragmentID(const CTagDefinition &fragmentIds, const char *szFragmentIdName) = 0;
	virtual EModifyFragmentIdResult RenameFragmentID(const CTagDefinition &fragmentIds, FragmentID fragmentID, const char *szFragmentIdName) = 0;
	virtual EModifyFragmentIdResult DeleteFragmentID(const CTagDefinition &fragmentIds, FragmentID fragmentID) = 0;

	virtual bool SetFragmentTagDef(const CTagDefinition &fragmentIds, FragmentID fragmentID, const CTagDefinition *pFragTagDefs) = 0;
	virtual void SetFragmentDef(const SControllerDef &controllerDef, FragmentID fragmentID, const SFragmentDef &fragmentDef) = 0;

	virtual void SaveDatabasesSnapshot(TAnimDBSnapshotCollection &dbSnapshotCollection) const = 0;
	virtual void LoadDatabasesSnapshot(const TAnimDBSnapshotCollection &dbSnapshotCollection) = 0;

	virtual void GetLoadedTagDefs(DynArray<const CTagDefinition*> &tagDefs) = 0;
	virtual void GetLoadedDatabases(DynArray<const IAnimationDatabase*> &animDatabases) const = 0;

	virtual void SaveAll(IMannequinWriter *pWriter) const = 0;
	virtual void RevertDatabase(const char *szFilename) = 0;
	virtual void RevertControllerDef(const char *szFilename) = 0;
	virtual void RevertTagDef(const char *szFilename) = 0;

	virtual bool DeleteFragmentEntry(IAnimationDatabase *pDatabase, FragmentID fragmentID, const SFragTagState &tagState, uint32 optionIdx, bool logWarning = true) = 0;
	virtual uint32 AddFragmentEntry(IAnimationDatabase *pDatabase, FragmentID fragmentID, const SFragTagState &tagState, const CFragment &fragment) = 0;
	virtual void SetFragmentEntry(IAnimationDatabase *pDatabase, FragmentID fragmentID, const SFragTagState &tagState, uint32 optionIdx, const CFragment &fragment) = 0;

	virtual bool IsFileUsedByControllerDef(const SControllerDef &controllerDef, const char *szFilename) const = 0;

	virtual void RegisterListener(IMannequinEditorListener *pListener) = 0;
	virtual void UnregisterListener(IMannequinEditorListener *pListener) = 0;

	virtual void AddSubADBFragmentFilter(IAnimationDatabase *pDatabase, const char *szSubADBFilename, FragmentID fragmentID) = 0;
	virtual void RemoveSubADBFragmentFilter(IAnimationDatabase *pDatabase, const char *szSubADBFilename, FragmentID fragmentID) = 0;
	virtual uint32 GetSubADBFragmentFilterCount(const IAnimationDatabase *pDatabase, const char *szSubADBFilename) const = 0;
	virtual FragmentID GetSubADBFragmentFilter(const IAnimationDatabase *pDatabase, const char *szSubADBFilename, uint32 index) const = 0;

	virtual void SetSubADBTagFilter(IAnimationDatabase *pDatabase, const char *szSubADBFilename, TagState tagState) = 0;
	virtual TagState GetSubADBTagFilter(const IAnimationDatabase *pDatabase, const char *szSubADBFilename) const = 0;
};

struct IMannequinGameListener
{
	virtual ~IMannequinGameListener(){}

	virtual void OnSpawnParticleEmitter(struct IParticleEmitter *pEffect, IActionController &actionController) = 0;
};

#endif
