#ifndef __PLAYERPROFILE_H__
#define __PLAYERPROFILE_H__

#if _MSC_VER > 1000
#	pragma once
#endif

//#include <IPlayerProfiles.h>
#include "PlayerProfileManager.h"

class CPlayerProfile : public IPlayerProfile
{
public:
	static const char* ATTRIBUTES_TAG; // "Attributes";
	static const char* ACTIONMAPS_TAG; // "ActionMaps";
	static const char* VERSION_TAG; // "Version";

	typedef std::map<string, TFlowInputData, std::less<string>, stl::STLGlobalAllocator<std::pair<string, TFlowInputData> > > TAttributeMap;

	CPlayerProfile(CPlayerProfileManager* pManager, const char* name, const char* userId, bool bIsPreview=false);
	virtual ~CPlayerProfile();

	// IPlayerProfile
	VIRTUAL bool Reset(); 

	// is this the default profile? it cannot be modified
	VIRTUAL bool IsDefault() const;

	// override values with console player profile defaults
	void LoadGamerProfileDefaults();

	// name of the profile
	VIRTUAL const char* GetName();

	// Id of the profile user
	VIRTUAL const char* GetUserId();

	// retrieve an action map
	VIRTUAL IActionMap* GetActionMap(const char* name);

	// set the value of an attribute
	VIRTUAL bool SetAttribute(const char* name, const TFlowInputData& value);

	// re-set attribute to default value (basically removes it from this profile)
	VIRTUAL bool ResetAttribute(const char* name);

	//delete an attribute from attribute map (regardless if has a default)
	VIRTUAL void DeleteAttribute(const char* name);

	// get the value of an attribute. if not specified optionally lookup in default profile
	VIRTUAL bool GetAttribute(const char* name, TFlowInputData& val, bool bUseDefaultFallback = true) const;

	// get all attributes available 
	// all in this profile and inherited from default profile
	VIRTUAL IAttributeEnumeratorPtr CreateAttributeEnumerator();

	// save game stuff
	VIRTUAL ISaveGameEnumeratorPtr CreateSaveGameEnumerator();
	VIRTUAL ISaveGame* CreateSaveGame();
	VIRTUAL ILoadGame* CreateLoadGame();
	VIRTUAL bool DeleteSaveGame(const char* name);
  VIRTUAL ILevelRotationFile* GetLevelRotationFile(const char* name);
	// ~IPlayerProfile

	bool SerializeXML(CPlayerProfileManager::IProfileXMLSerializer* pSerializer);

	void SetName(const char* name);
	void SetUserId(const char* userId);
	
	const TAttributeMap& GetAttributeMap() const {
		return m_attributeMap;
	}

	const TAttributeMap& GetDefaultAttributeMap() const;

	void GetMemoryStatistics(ICrySizer * s);

protected:
	CHEAT_PROTECTION_EXPORT bool LoadAttributes(const XmlNodeRef& root, int requiredVersion);
	bool SaveAttributes(const XmlNodeRef& root);

	friend class CAttributeEnumerator;

	CPlayerProfileManager* m_pManager;
	string m_name;
	string m_userId;
	TAttributeMap m_attributeMap;
	int m_attributesVersion;
	bool m_bIsPreview;
};

#endif
