////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   IEntityClass.h
//  Version:     v1.00
//  Created:     18/5/2004 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(IEntityClass.h)

#ifndef __IEntityClass_h__
#define __IEntityClass_h__
#pragma once

struct IEntity;
struct IEntityProxy;
struct SEntitySpawnParams;
struct IEntityScript;

struct SEditorClassInfo
{
	SEditorClassInfo():
		sIcon(""),
		sHelper(""),
		sCategory("")
	{
	}

	const char* sIcon;
	const char* sHelper;
	const char* sCategory;
};

enum EEntityClassFlags
{
	ECLF_INVISIBLE = 0x0001, // If set this class will not be visible in editor,and entity of this class cannot be placed manually in editor.
	ECLF_DEFAULT   = 0x0002, // If this is default entity class.
};

// Description:
//    Custom interface that can be used to enumerate/set an entity class' property.
//	  Used for comunication with the editor, mostly.
// See Also:
//    IEntityClassRegistry::SEntityClassDesc
struct IEntityPropertyHandler
{
	virtual ~IEntityPropertyHandler(){}
	//////////////////////////////////////////////////////////////////////////
	// Property info for entity class.
	//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage( ICrySizer *pSizer ) const {/*REPLACE LATER*/}

	enum EPropertyType
	{
		Bool,
		Int,
		Float,
		Vector,
		String,
		Entity,
		FolderBegin,
		FolderEnd
	};

	struct SPropertyInfo
	{
		const char* name;			// Name of the property.
		EPropertyType type;			// Type of the property value.
		const char* editType;		// Type of edit control to use.
		const char* description;	// Description of the property.
		uint32 flags;				// Property flags.

		struct SLimits				// Limits
		{
			float min;
			float max;
		} limits;
	};

	// Description:
	//    Refresh the class' properties.
	virtual void RefreshProperties() = 0;

	// Description:
	//    Load properties into the entity.
	virtual void LoadEntityXMLProperties(IEntity* entity, const XmlNodeRef& xml) = 0;

	// Description:
	//    Load archetype properties.
	virtual void LoadArchetypeXMLProperties(const char* archetypeName, const XmlNodeRef& xml) = 0;

	// Description:
	//    Init entity with archetype properties.
	virtual void InitArchetypeEntity(IEntity* entity, const char* archetypeName, const SEntitySpawnParams& spawnParams) = 0;

	// Description:
	//    Returns number of properties for this entity.
	// See Also:
	//    GetPropertyInfo
	// Returns:
	//    Return Number of properties.
	virtual int GetPropertyCount() const = 0;

	// Description:
	//    Retrieve information about properties of the entity.
	// See Also:
	//    SPropertyInfo, GetPropertyCount
	// Arguments:
	//    nIndex - Index of the property to retrieve, must be in 0 to GetPropertyCount()-1 range.
	// Returns:
	//    Specified event description in SPropertyInfo structure.
	virtual bool GetPropertyInfo(int index, SPropertyInfo& info ) const = 0;

	// Description:
	//    Set a property in a entity of this class.
	// Arguments:
	//    index - Index of the property to set, must be in 0 to GetPropertyCount()-1 range.
	virtual void SetProperty(IEntity* entity, int index, const char* value) = 0;

	// Description:
	//    Get a property in a entity of this class.
	// Arguments:
	//    index - Index of the property to get, must be in 0 to GetPropertyCount()-1 range.
	virtual const char* GetProperty(IEntity* entity, int index) const = 0;

	// Description:
	//    Get a property in a entity of this class.
	// Arguments:
	//    index - Index of the property to get, must be in 0 to GetPropertyCount()-1 range.
	virtual const char* GetDefaultProperty(int index) const = 0;

	// Description:
	//    Inform the implementation that properties have changed. Called at the end of a change block.
	virtual void PropertiesChanged(IEntity* entity) = 0;
};

// Description:
//    Custom interface that can be used to reload an entity's script.
//	  Used by the editor, only.
// See Also:
//    IEntityClassRegistry::SEntityClassDesc
struct IEntityScriptFileHandler
{
	virtual ~IEntityScriptFileHandler(){}
	// Description:
	//    Reloads the specified entity class' script.
	virtual void ReloadScriptFile() = 0;

	// Description:
	//    Returns the class' script file name, if any.
	virtual const char* GetScriptFile() const = 0;

	virtual void GetMemoryUsage( ICrySizer *pSizer ) const {/*REPLACE LATER*/}
};


struct IEntityEventHandler
{
	virtual ~IEntityEventHandler(){}
	virtual void GetMemoryUsage( ICrySizer *pSizer ) const {/*REPLACE LATER*/}

	//////////////////////////////////////////////////////////////////////////
	// Events info for this entity class.
	//////////////////////////////////////////////////////////////////////////
	enum EventValueType
	{
		Bool,
		Int,
		Float,
		Vector,
		Entity,
		String,
	};

	enum EventType
	{
		Input,
		Output,
	};

	struct SEventInfo
	{
		const char* name;			// Name of event.
		EventType type;				// Input or Output event.
		EventValueType valueType;	// Type of event value.
		const char* description;	// Description of the event.
	};

	// Description:
	//    Refresh the class' event info.
	virtual void RefreshEvents() = 0;

	// Description:
	//    Load event info into the entity.
	virtual void LoadEntityXMLEvents(IEntity* entity, const XmlNodeRef& xml) = 0;

	// Description:
	//    Returns number of events for this entity.
	// See Also:
	//    GetEventInfo
	// Returns:
	//    Return Number of events.
	virtual int GetEventCount() const = 0;

	// Description:
	//    Retrieve information about events of the entity.
	// See Also:
	//    SEventInfo, GetEventCount
	// Arguments:
	//    nIndex - Index of the event to retrieve, must be in 0 to GetEventCount()-1 range.
	// Returns:
	//    Specified event description in SEventInfo structure.
	virtual bool GetEventInfo(int index, SEventInfo& info ) const = 0;

	// Description:
	//    Send the specified event to the entity.
	// Arguments:
	//    entity - The entity to send the event to.
	//    eventName - Name of the event to send.
	virtual void SendEvent(IEntity* entity, const char* eventName) = 0;
};

//////////////////////////////////////////////////////////////////////////
// Description:
//    Entity class defines what is this entity, what script it uses,
//    what user proxy will be spawned with the entity,etc...
//    IEntityClass unique identify type of the entity, Multiple entities
//    share the same entity class.
//    Two entities can be compared if they are of the same type by
//    just comparing their IEntityClass pointers.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityClass
{
	//////////////////////////////////////////////////////////////////////////
	// UserProxyCreateFunc is a function pointer type, 
	// by calling this function EntitySystem can create user defined UserProxy class for an entity in SpawnEntity.
	// Ex:
	// IEntityProxy* CreateUserProxy( IEntity *pEntity,SEntitySpawnParams &params )
	// { 
	//   return new CUserProxy( pEntity,params );
	// }
	typedef IEntityProxyPtr (*UserProxyCreateFunc)( IEntity *pEntity, SEntitySpawnParams &params, void *pUserData );

	//////////////////////////////////////////////////////////////////////////
	// Events info for this entity class.
	//////////////////////////////////////////////////////////////////////////
	enum EventValueType
	{
		EVT_INT,
		EVT_FLOAT,
		EVT_BOOL,
		EVT_VECTOR,
		EVT_ENTITY,
		EVT_STRING
	};

	struct SEventInfo
	{
		const char *name;			// Name of event.
		EventValueType type;  // Type of event value.
		bool bOutput;					// Input or Output event.
	};

	// <interfuscator:shuffle>
	virtual ~IEntityClass(){}
	// Description:
	//    Destroy IEntityClass object, do not call directly, only EntityRegisty can destroy entity classes.
	virtual void Release() = 0;

	// Description:
	//    Returns the name of the entity class, Class name must be unique among all the entity classes.
	//    If this entity also uses a script, this is the name of the Lua table representing the entity behavior.
	virtual const char* GetName() const = 0;

	// Description:
	//    Returns entity class flags.
	// See Also:
	//    EEntityClassFlags
	virtual uint32 GetFlags() const = 0;

	// Description:
	//    Set entity class flags.
	// See Also:
	//    EEntityClassFlags
	virtual void SetFlags( uint32 nFlags ) = 0;

	// Description:
	//    Returns the Lua script file name.
	// Returns:
	//    Lua Script filename, return empty string if entity does not use script.
	virtual const char* GetScriptFile() const = 0;

	// Description:
	//    Returns the IEntityScript interface assigned for this entity class.
	// Returns:
	//    IEntityScript interface if this entity have script, or NULL if no script defined for this entity class.
	virtual IEntityScript* GetIEntityScript() const = 0;

	// Description:
	//    Returns the IScriptTable interface assigned for this entity class.
	// Returns:
	//    IScriptTable interface if this entity have script, or NULL if no script defined for this entity class.
	virtual IScriptTable* GetScriptTable() const = 0;

	virtual IEntityPropertyHandler* GetPropertyHandler() const = 0;
	virtual IEntityEventHandler* GetEventHandler() const = 0;
	virtual IEntityScriptFileHandler* GetScriptFileHandler() const = 0;

	virtual const SEditorClassInfo& GetEditorClassInfo() const = 0;
	virtual void SetEditorClassInfo(const SEditorClassInfo& editorClassInfo) = 0;
	
	// Description:
	//    Loads the script.
	//    It is safe to call LoadScript multiple times, only first time the script will be loaded, if bForceReload is not specified.
	virtual bool LoadScript( bool bForceReload ) = 0;

	// Description:
	//    Returns pointer to the user defined function to create UserProxy.
	// Returns:
	//    Return ContainerCreateFunc function pointer.
	virtual IEntityClass::UserProxyCreateFunc GetUserProxyCreateFunc() const = 0;
	
	// Description:
	//    Returns pointer to the user defined function to create UserProxy.
	// Returns:
	//    Return ContainerCreateFunc function pointer.
	virtual void* GetUserProxyData() const = 0;
	
	// Description:
	//    Returns number of input and output events defiend in the entity script.
	// See Also:
	//    GetEventInfo
	// Returns:
	//    Return Number of events.
	virtual int GetEventCount() = 0;

	// Description:
	//    Retrieve information about input/output event of the entity.
	// See Also:
	//    SEventInfo, GetEventCount
	// Arguments:
	//    nIndex - Index of the event to retrieve, must be in 0 to GetEventCount()-1 range.
	// Returns:
	//    Specified event description in SEventInfo structure.
	virtual IEntityClass::SEventInfo GetEventInfo( int nIndex ) = 0;

	// Description:
	//    Find event by name.
	// See Also:
	//    SEventInfo, GetEventInfo
	// Arguments:
	//    sEvent - Name of the event.
	//    event - Output parameter for event.
	// Returns:
	//    True if event found and event parameter is initialized.
	virtual bool FindEventInfo( const char *sEvent,SEventInfo &event ) = 0;

	virtual void GetMemoryUsage( ICrySizer *pSizer ) const = 0;
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////
// Description:
//    This interface is the repository of the the various entity classes, it allows
//    creation and modification of entities types.
//    There`s only one IEntityClassRegistry interface can exist per EntitySystem.
//    Every entity class that can be spawned must be registered in this interface.
// See Also:
//    IEntitySystem::GetClassRegistry
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityClassRegistry
{
	struct SEntityClassDesc
	{
		SEntityClassDesc()
			: flags(0)
			, sName("")
			, sScriptFile("")
			, editorClassInfo()
			, pUserProxyCreateFunc(NULL)
			, pUserProxyData(NULL)
			, pPropertyHandler(NULL)
			, pEventHandler(NULL)
			, pScriptFileHandler(NULL)
		{
		};

		int           flags;
		const char*   sName;
		const char*   sScriptFile;

		SEditorClassInfo editorClassInfo;

		IEntityClass::UserProxyCreateFunc pUserProxyCreateFunc;
		void*         pUserProxyData;

		IEntityPropertyHandler* pPropertyHandler;
		IEntityEventHandler* pEventHandler;
		IEntityScriptFileHandler* pScriptFileHandler;
	};

	virtual ~IEntityClassRegistry(){}
	// Description:
	//    Register a new entity class.
	// Returns:
	//    true if successfully registered.
	virtual bool RegisterClass( IEntityClass *pClass ) = 0;
	
	// Description:
	//    Unregister an entity class.
	// Returns:
	//    true if successfully unregistered.
	virtual bool UnregisterClass( IEntityClass *pClass ) = 0;

	// Description:
	//    Retrieves pointer to the IEntityClass interface by entity class name.
	// Returns:
	//    Pointer to the IEntityClass interface, or NULL if class not found.
	virtual IEntityClass* FindClass( const char *sClassName ) const = 0;

	// Description:
	//    Retrieves pointer to the IEntityClass interface for a default entity class.
	// Returns:
	//    Pointer to the IEntityClass interface, It can never return NULL.
	virtual IEntityClass* GetDefaultClass() const = 0;

	// Description:
	//    Load all entity class description xml files with extension ".ent" from specified directory.
	// Arguments:
	//    sPath - Path where to search for .ent files.
	virtual void LoadClasses( const char *sPath,bool bOnlyNewClasses=false ) = 0;

	// Description:
	//    Register standard entity class, if class id not specified (is zero), generate a new class id.
	// Returns:
	//    Pointer to the new created and registered IEntityClass interface, or NULL if failed.
	virtual IEntityClass* RegisterStdClass( const SEntityClassDesc &entityClassDesc ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Registry iterator.
	//////////////////////////////////////////////////////////////////////////

	// Description:
	//    Move the entity class iterator to the begin of the registry.
	//    To iterate over all entity classes, ex:
	//    ...
	//    IEntityClass *pClass = NULL;
	//    for (pEntityRegistry->IteratorMoveFirst(); pClass = pEntityRegistry->IteratorNext();;)
	//    {
	//       pClass
	//    ...
	//    }
	virtual void IteratorMoveFirst() = 0;
	
	// Description:
	//    Get the next entity class in the registry.
	// Returns:
	//    Return a pointer to the next IEntityClass interface, or NULL if is the end
	virtual IEntityClass* IteratorNext() = 0;
	
	// Description:
	//    Return the number of entity classes in the registry.
	// Returns:
	//    Return a pointer to the next IEntityClass interface, or NULL if is the end
	virtual int GetClassCount() const = 0;
	// </interfuscator:shuffle>
};


#endif // __IEntityClass_h__
