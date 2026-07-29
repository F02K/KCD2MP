////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   IEntityProxy.h
//  Version:     v1.00
//  Created:     28/9/2004 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description: Definition of all proxy interfaces used by an Entity.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#ifndef __IEntityProxy_h__
#define __IEntityProxy_h__
#pragma once

#include <SerializeFwd.h>
#include <ISound.h> // <> required for Interfuscator. Needed for enum ESoundSemantic

#include <BoostHelpers.h>
#include <IComponent.h>

struct SEntitySpawnParams;
struct SEntityEvent;
struct IShaderPublicParams;
struct IFlowGraph;
struct IEntityEventListener;
struct SSGHandle;
struct a2DPoint;

typedef uint32 tSoundID;


//////////////////////////////////////////////////////////////////////////
// Entity proxies that can be hosted by the entity.
//////////////////////////////////////////////////////////////////////////
enum EEntityProxy
{
	ENTITY_PROXY_RENDER,
	ENTITY_PROXY_PHYSICS,
	ENTITY_PROXY_SCRIPT,
	ENTITY_PROXY_SOUND,
	ENTITY_PROXY_AI,
	ENTITY_PROXY_AREA,
	ENTITY_PROXY_BOIDS,
	ENTITY_PROXY_BOID_OBJECT,
	ENTITY_PROXY_CAMERA,
	ENTITY_PROXY_FLOWGRAPH,
	ENTITY_PROXY_SUBSTITUTION,
	ENTITY_PROXY_TRIGGER,
	ENTITY_PROXY_ROPE,
	ENTITY_PROXY_ENTITYNODE,

	ENTITY_PROXY_USER,

	// Always the last entry of the enum.
	ENTITY_PROXY_LAST
};

//////////////////////////////////////////////////////////////////////////
// Description:
//    Base interface to access to various entity proxy objects.
//////////////////////////////////////////////////////////////////////////
struct IEntityProxy : public IComponent
{
	// <interfuscator:shuffle>
	virtual ~IEntityProxy(){}

	virtual ComponentEventPriority GetEventPriority( const int eventID ) const { return ENTITY_PROXY_LAST - const_cast<IEntityProxy*> (this)->GetType(); }

	virtual void GetMemoryUsage(ICrySizer *pSizer) const{};

	virtual EEntityProxy GetType() = 0;

	// Called when the subsystem initialize.
	virtual bool Init( IEntity *pEntity,SEntitySpawnParams &params ) = 0;

	// Called when the subsystem is reloaded.
	virtual void Reload( IEntity *pEntity,SEntitySpawnParams &params ) = 0;

	// Called when the entity is destroyed! At this point, all proxies are valid! No memory should be deleted here!
	virtual void Done() = 0;

	// Description:
	//    When host entity is destroyed every proxy will be called with the Release method to delete itself.
	virtual void Release() = 0;

	// Description:
	//    Update will be called every time the host Entity is updated.
	// Arguments:
	//    ctx - Update context of the host entity, provides all the information needed to update the entity.
	virtual	void Update( SEntityUpdateContext &ctx ) = 0;

	// Description:
	//    By overriding this function proxy will be able to handle events sent from the host Entity.
	// Arguments:
	//    event - Event structure, contains event id and parameters.
	virtual	void ProcessEvent( SEntityEvent &event ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Serialize proxy to/from XML.
	virtual void SerializeXML( XmlNodeRef &entityNode,bool bLoading ) = 0;

	// Description:
	//    Serialize proxy with a TSerialize
	// Arguments:
	//    ser - the object to serialize with
	virtual void Serialize( TSerialize ser ) = 0;

	// Description:
	//    Returns true if this proxy need to be saved during serialization.
	// Returns:
	//    true - If proxy needs to be serialized
	virtual bool NeedSerialize() = 0;

	// Description:
	//    Builds a signature to describe the dynamic hierarchy of the parent Entity container
	// Arguments:
	//    ser - the object to serialize with, forming the signature
	// Returns:
	//    true - If the signature is thus far valid
	// Note:
	//    It's the responsibility of the proxy to identify its internal state which may complicate the hierarchy 
	//    of the parent Entity i.e., sub-proxies and which actually exist for this instantiation.
	virtual bool GetSignature( TSerialize signature ) = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityProxy );

struct IEntityScript;

//////////////////////////////////////////////////////////////////////////
// Script proxy interface.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityScriptProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	virtual void SetScriptUpdateRate( float fUpdateEveryNSeconds ) = 0;
	virtual IScriptTable* GetScriptTable() = 0;
	virtual void CallEvent( const char *sEvent ) = 0;
	virtual void CallEvent( const char *sEvent,float fValue ) = 0;
	virtual void CallEvent( const char *sEvent,bool bValue ) = 0;
	virtual void CallEvent( const char *sEvent,const char *sValue ) = 0;
	virtual void CallEvent( const char *sEvent,const Vec3 &vValue ) = 0;
	virtual void CallEvent( const char *sEvent,EntityId nEntityId ) = 0;

	// Description:
	//    Change current state of the entity script.
	// Return:
	//    If state was successfully set.
	virtual bool GotoState( const char *sStateName ) = 0;

	// Description:
	//    Change current state of the entity script.
	// Return:
	//    If state was successfully set.
	virtual bool GotoStateId( int nStateId ) = 0;

	// Description:
	//    Check if entity is in specified state.
	// Arguments:
	//    sStateName - Name of state table within entity script (case sensetive).
	// Return:
	//    If entity script is in specified state.
	virtual bool IsInState( const char *sStateName ) = 0;

	// Description:
	//    Retrieves name of the currently active entity script state.
	// Return:
	//    Name of current state.
	virtual const char* GetState() = 0;

	// Description:
	//    Retrieves the id of the currently active entity script state.
	// Return:
	//    Index of current state.
	virtual int GetStateId() = 0;

	// Description:
	//     Fires an event in the entity script.
	//     This will call OnEvent(id,param) Lua function in entity script, so that script can handle this event.
	// See Also:
	//     EScriptEventId, IScriptObject
	virtual void SendScriptEvent( int Event, IScriptTable *pParamters, bool *pRet=NULL) = 0;
	virtual void SendScriptEvent( int Event, const char *str, bool *pRet=NULL )=0;
	virtual void SendScriptEvent( int Event, int nParam, bool *pRet=NULL )=0;

	// Description:
	//    Change the Entity Script used by the Script Proxy. 
	//    Caller is responsible for making sure new script is initialised and script bound as required
	// Arguments:
	//    pScript - an entity script object that has already been loaded with the new script
	//    params - parameters used to set the properties table if required
	virtual void ChangeScript( IEntityScript* pScript, SEntitySpawnParams *params ) = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityScriptProxy );

//////////////////////////////////////////////////////////////////////////
// Parameters passed to IEntity::Physicalize function.
//////////////////////////////////////////////////////////////////////////
struct SEntityPhysicalizeParams
{
	//////////////////////////////////////////////////////////////////////////
	SEntityPhysicalizeParams() : type(0),density(-1),mass(-1),nSlot(-1),nFlagsOR(0),nFlagsAND(UINT_MAX),
		pAttachToEntity(NULL),nAttachToPart(-1),fStiffnessScale(0), bCopyJointVelocities(false),
		pParticle(NULL),pBuoyancy(NULL),pPlayerDimensions(NULL),pPlayerDynamics(NULL),pCar(NULL),pAreaDef(NULL),nLod(0),szPropsOverride(0) {};
	//////////////////////////////////////////////////////////////////////////
	// Physicalization type must be one of pe_type ennums.
	// See Also: pe_type
	int type; // Always must be filled.

	// Index of object slot. -1 if all slots should be used.
	int nSlot;
	// Only one either density or mass must be set, parameter set to 0 is ignored.
	float density;
	float mass;
	// Optional physical flags.
	int nFlagsAND;
	int nFlagsOR;
	// When physicalizing geometry can specify to use physics from different LOD.
	// Used for characters that have ragdoll physics in Lod1
	int nLod;

	// Physical entity to attach this physics object (Only for Soft physical entity).
	IPhysicalEntity *pAttachToEntity;
	// Part ID in entity to attach to (Only for Soft physical entity).
	int nAttachToPart;
	// Used for character physicalization (Scale of force in character joint's springs).
	float fStiffnessScale;

	// Copy joints velocities when converting a character to ragdoll.
	bool bCopyJointVelocities;

	struct pe_params_particle   *pParticle;
	struct pe_params_buoyancy   *pBuoyancy;
	struct pe_player_dimensions *pPlayerDimensions;
	struct pe_player_dynamics   *pPlayerDynamics;
	struct pe_params_car        *pCar;     

	//////////////////////////////////////////////////////////////////////////
	// This parameters are only used when type == PE_AREA
	//////////////////////////////////////////////////////////////////////////
	struct AreaDefinition
	{
		enum EAreaType {
			AREA_SPHERE,        // Physical area will be sphere.
			AREA_BOX,						// Physical area will be box.
			AREA_GEOMETRY,			// Physical area will use geometry from the specified slot.
			AREA_SHAPE,			    // Physical area will points to specify 2D shape.
			AREA_CYLINDER,      // Physical area will be a cylinder
			AREA_SPLINE,				// Physical area will be a spline-tube
		};

		EAreaType areaType;
		float fRadius;      // Must be set when using AREA_SPHERE or AREA_CYLINDER area type or an AREA_SPLINE.
		Vec3 boxmin,boxmax; // Min,Max of bounding box, must be set when using AREA_BOX area type.
		Vec3 *pPoints;      // Must be set when using AREA_SHAPE area type or an AREA_SPLINE.
		int nNumPoints;     // Number of points in pPoints array.
		float zmin,zmax;    // Min/Max of points.
		Vec3 center;
		Vec3 axis;
		
		// pGravityParams must be a valid pointer to the area gravity params structure.
		struct pe_params_area *pGravityParams;

		AreaDefinition() : areaType(AREA_SPHERE),fRadius(0),boxmin(0,0,0),boxmax(0,0,0),
			pPoints(NULL),nNumPoints(0),pGravityParams(NULL),zmin(0),zmax(0),center(0,0,0),axis(0,0,0) {}
	};
	// When physicalizing with type == PE_AREA this must be a valid pointer to the AreaDefinition structure.
	AreaDefinition *pAreaDef;
	// an optional string with text properties overrides for CGF nodes
	const char *szPropsOverride;
};

//////////////////////////////////////////////////////////////////////////
// Physical proxy interface.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityPhysicalProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	// Description:
	//    Assign a pre-created physical entity to this proxy.
	// Arguments:
	//    pPhysEntity - The pre-created physical entity.
	//		nSlot - Slot Index to which the new position will be taken from.
	virtual void AssignPhysicalEntity( IPhysicalEntity *pPhysEntity, int nSlot=-1 ) = 0;
	// Description:
	//    Get world bounds of physical object.
	// Arguments:
	//    bounds - Returns Bounding box in world space.
	virtual void GetWorldBounds( AABB &bounds ) = 0;

	// Description:
	//    Get local space physical bounding box.
	// Arguments:
	//    bounds - Returns Bounding box in local entity space.
	virtual void GetLocalBounds( AABB &bounds ) = 0;

	virtual void Physicalize( SEntityPhysicalizeParams &params ) = 0;
	virtual void ReattachSoftEntityVtx( IPhysicalEntity *pAttachToEntity, int nAttachToPart ) = 0;
	virtual IPhysicalEntity* GetPhysicalEntity() const = 0;

	virtual void SerializeTyped( TSerialize ser, int type, int flags ) = 0;
	
	// Enable or disable physical simulation.
	virtual void EnablePhysics( bool bEnable ) = 0;
	// Is physical simulation enabled?
	virtual bool IsPhysicsEnabled() const = 0;
	// Add impulse to physical entity.
	virtual void AddImpulse( int ipart, const Vec3 &pos,const Vec3 &impulse,bool bPos,float fAuxScale, float fPushScale = 1.0f ) = 0;

	// Description:
	//    Creates a trigger bounding box.
	//    When physics will detect collision with this bounding box it will send an events to the entity.
	//    If entity have script OnEnterArea and OnLeaveArea events will be called.
	// Arguments:
	//    bbox - Axis aligned bounding box of the trigger in entity local space (Rotation and scale of the entity is ignored).
	//           Set empty bounding box to disable trgger.
	virtual void SetTriggerBounds( const AABB &bbox ) = 0;
	
	// Description:
	//    Retrieve trigger bounding box in local space.
	// Return:
	//    Axis aligned bounding box of the trigger in the local space.
	virtual void GetTriggerBounds( AABB &bbox ) = 0;

	// Description:
	//		physicalizes the foliage of StatObj in slot iSlot
	virtual bool PhysicalizeFoliage( int iSlot ) = 0;

	// Description:
	//		dephysicalizes the foliage in slot iSlot
  virtual void DephysicalizeFoliage( int iSlot ) = 0;

	// Description:
	//		returns foliage object in slot iSlot
  virtual struct IFoliage *GetFoliage( int iSlot ) = 0;

  // Description:
  //    retrieve current partid offset
  virtual int GetPartId0() = 0;


  // Description:
  //    Enable/disable network serialisation of the physics aspect
  virtual void EnableNetworkSerialization(bool enable) = 0;

  // Description:
  //    Have the physics ignore the XForm event
  virtual void IgnoreXFormEvent(bool ignore) = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityPhysicalProxy );

//////////////////////////////////////////////////////////////////////////
// Proximity trigger proxy interface.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityTriggerProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	// Description:
	//    Creates a trigger bounding box.
	//    When physics will detect collision with this bounding box it will send an events to the entity.
	//    If entity have script OnEnterArea and OnLeaveArea events will be called.
	// Arguments:
	//    bbox - Axis aligned bounding box of the trigger in entity local space (Rotation and scale of the entity is ignored).
	//           Set empty bounding box to disable trgger.
	virtual void SetTriggerBounds( const AABB &bbox ) = 0;

	// Description:
	//    Retrieve trigger bounding box in local space.
	// Return:
	//    Axis aligned bounding box of the trigger in the local space.
	virtual void GetTriggerBounds( AABB &bbox ) = 0;

	// Description:
	//    Forward enter/leave events to this entity
	virtual void ForwardEventsTo( EntityId id ) = 0;

	// Description:
	//    Invalidate the trigger, so it gets recalculated and catches things which are already inside
	//		when it gets enabled.
	virtual void InvalidateTrigger( ) = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityTriggerProxy );

//////////////////////////////////////////////////////////////////////////
// Sound proxy interface.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntitySoundProxy : public IEntityProxy
{
	enum ESoundProxyFlags
	{
		FLAG_DELEGATE_SOUND_ANIM_EVENTS = BIT(1) // When set it does not process 'sound' animation events 
	};

	// <interfuscator:shuffle>
	virtual void SetFlags( int nSoundProxyFlags ) = 0;
	virtual int  GetFlags() const = 0;

	virtual ISound* const GetSound(tSoundID const nSoundId) = 0;
	virtual bool PlaySound( ISound *pSound, const Vec3 &vPos=Vec3(0,0,0),const Vec3 &vDirection=FORWARD_DIRECTION,float fSoundScale=1.0f, bool bLipSync=true ) = 0;
	// new calls
	virtual tSoundID PlaySound( const char *sSoundOrEventName, const Vec3 &vOffset,const Vec3 &vDirection, uint32 nSoundFlags, uint32 nSoundFlagsExtended, ESoundSemantic eSemantic, EntityId *pSkipEnts,int nSkipEnts) = 0;
	virtual tSoundID PlaySound( const char *sSoundOrEventName, const Vec3 &vOffset,const Vec3 &vDirection, uint32 nSoundFlags, uint32 nSoundFlagsExtended, ESoundSemantic eSemantic) = 0;
	virtual tSoundID PlaySoundEx( const char *sSoundOrEventName, const Vec3 &vOffset,const Vec3 &vDirection, uint32 nSoundFlags, uint32 nSoundFlagsExtended, float fVolume, float fMinRadius,float fMaxRadius, ESoundSemantic eSemantic) = 0;
	virtual tSoundID PlaySoundEx( const char *sSoundOrEventName, const Vec3 &vOffset,const Vec3 &vDirection, uint32 nSoundFlags, uint32 nSoundFlagsExtended, float fVolume, float fMinRadius,float fMaxRadius, ESoundSemantic eSemantic, EntityId *pSkipEnts,int nSkipEnts) = 0;
	virtual bool SetStaticSound(tSoundID nSoundId, bool bStatic) = 0;
	virtual bool GetStaticSound(const tSoundID nSoundId) = 0;
	virtual void StopSound(tSoundID const nSoundId, ESoundStopMode const eStopMode = ESoundStopMode_EventFade, bool const bLoopedOnly = false) = 0;
	virtual void PauseSound( tSoundID nSoundId,bool bPause ) = 0;
	virtual void StopAllSounds() = 0;
	virtual void SetSoundPos( tSoundID nSoundId,const Vec3 &vPos ) = 0;
	virtual Vec3 GetSoundPos( tSoundID nSoundId ) = 0;
	virtual void SetSoundDirection( tSoundID nSoundId,const Vec3 &dir ) = 0;
	virtual Vec3 GetSoundDirection( tSoundID nSoundId ) = 0;
	virtual void SetEffectRadius( float fEffectRadius ) = 0;
	virtual float GetEffectRadius() = 0;
	virtual const char* GetTailName() = 0;
	virtual void CheckVisibilityForTailName(const float fLength, const float fDistanceToRecalculate) = 0;

	virtual void UpdateSounds() = 0;

	virtual IEntity* GetEntity() const = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntitySoundProxy );

//////////////////////////////////////////////////////////////////////////
// Description:
//    Flags the can be set on each of the entity object slots.
//////////////////////////////////////////////////////////////////////////
enum EEntitySlotFlags
{
	ENTITY_SLOT_RENDER = 0x0001,  // Draw this slot.
	ENTITY_SLOT_RENDER_NEAREST = 0x0002,  // Draw this slot as nearest. [Rendered in camera space]
	ENTITY_SLOT_RENDER_WITH_CUSTOM_CAMERA = 0x0004,  // Draw this slot using custom camera passed as a Public ShaderParameter to the entity.
	ENTITY_SLOT_IGNORE_PHYSICS = 0x0008,  // This slot will ignore physics events sent to it.
	ENTITY_SLOT_BREAK_AS_ENTITY = 0x0010,
	ENTITY_SLOT_RENDER_AFTER_POSTPROCESSING = 0x0020,
	ENTITY_SLOT_BREAK_AS_ENTITY_MP = 0x0040, // In MP this an entity that shouldn't fade or participate in network breakage
};

// Description of the contents of the entity slot.
struct SEntitySlotInfo
{
	// Slot flags.
	int nFlags;
	// Index of parent slot, (-1 if no parent)
	int nParentSlot;
	// Hide mask used by breakable object to indicate what index of the CStatObj sub-object is hidden.
	uint64 nSubObjHideMask;
	// Slot local transformation matrix.
	const Matrix34 *pLocalTM;
	// Slot world transformation matrix.
	const Matrix34 *pWorldTM;
	// Objects that can binded to the slot.
	EntityId                   entityId;
	struct IStatObj*           pStatObj;
	struct ICharacterInstance*   pCharacter;
	struct IParticleEmitter*   pParticleEmitter;
	struct ILightSource*      pLight;
	struct IRenderNode*      pChildRenderNode;
	// Custom Material used for the slot.
	IMaterial* pMaterial;
};

struct IShaderParamCallback;
DECLARE_BOOST_POINTERS(IShaderParamCallback);

//////////////////////////////////////////////////////////////////////////
// Render proxy interface.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Interface to the entity Render proxy.
//////////////////////////////////////////////////////////////////////////
struct IEntityRenderProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	// Description:
	//    Get world bounds of render proxy.
	// Arguments:
	//    bounds - Returns Bounding box in world space.
	virtual void GetWorldBounds( AABB &bounds ) = 0;

	// Description:
	//    Get local space int the entity bounds of render proxy.
	// Arguments:
	//    bounds - Returns Bounding box in local entity space.
	virtual void GetLocalBounds( AABB &bounds ) = 0;

	// Description:
	//    Force local bounds.
	// Arguments:
	//    bounds - Bounding box in local space.
	//    bDoNotRecalculate - when set to true entity will never try to recalculate local bounding box set by this call.
	virtual void SetLocalBounds( const AABB &bounds,bool bDoNotRecalculate ) = 0;

	// Invalidates local or world space bounding box.
	virtual void InvalidateLocalBounds() = 0;

	// Description:
	//     Retrieve an actual material used for rendering specified slot.
	///    Will return custom applied material or if custom material not set will return an actual material assigned to the slot geometry.
	// Arguments:
	//     nSlot - Slot to query used material from, if -1 material will be taken from the first renderable slot.
	// Return:
	//     Material used for rendering, or NULL if slot is not rendered.
	virtual IMaterial* GetRenderMaterial( int nSlot=-1 ) = 0;

	// Description:
	//     Assign custom material to the slot.
	// Arguments:
	//     nSlot - Slot to apply material to.
	virtual void SetSlotMaterial( int nSlot,IMaterial *pMaterial ) = 0;

	// Description:
	//     Retrieve slot's custom material (This material Must have been applied before with the SetSlotMaterial).
	// Arguments:
	//     nSlot - Slot to query custom material from.
	// Return:
	//     Custom material applied on the slot.
	virtual IMaterial* GetSlotMaterial( int nSlot ) = 0;

	// Description:
	//    Retrieve engine render node, used to render this entity.
	virtual IRenderNode* GetRenderNode() = 0;

	// Description:
	//    Retrieve and optionally create a shaders public params for this render proxy.
	// Arguments:
	//    bCreate - If Shader public params are not created for this entity, they will be created.
	virtual IShaderPublicParams* GetShaderPublicParams( bool bCreate=true ) = 0;
		
	// Description:
	//    Add a render proxy callback
	virtual void AddShaderParamCallback(IShaderParamCallbackPtr pCallback) = 0;

	// Description:
	//    Remove a render proxy callback
	virtual bool RemoveShaderParamCallback(IShaderParamCallbackPtr pCallback) = 0;

	// Description
	//		Check which shader param callback should be activated or disactived depending on 
	//		the current entity state
	virtual void CheckShaderParamCallbacks() = 0;
	
	// Description
	//		Clears all the shader param callback from this entity
	virtual void ClearShaderParamCallbacks() = 0;

	// Description:
	//     Assign sub-object hide mask to slot.
	// Arguments:
	//     nSlot - Slot to apply hide mask to.
	virtual void SetSubObjHideMask( int nSlot,uint64 nSubObjHideMask ) = 0;
	virtual uint64 GetSubObjHideMask( int nSlot ) const = 0;

	// Description:
	//    updates indirect lighting for children
	virtual void UpdateIndirLightForChildren(){};

  // Description:
  //    sets material layers masks
  virtual void SetMaterialLayersMask( uint8 nMtlLayersMask ) = 0;
  virtual uint8 GetMaterialLayersMask( ) const = 0;

  // Description:
  //    overrides material layers blend amount
  virtual void SetMaterialLayersBlend( uint32 nMtlLayersBlend ) = 0;
  virtual uint32 GetMaterialLayersBlend( ) const = 0;

	// Description:
	//    set/get cloak interference state
	virtual void SetCloakInterferenceState( bool bHasCloakInterference ) = 0;
	virtual bool GetCloakInterferenceState( ) const = 0;

	// Description:
	//    set cloak highlight strength
	virtual void SetCloakHighlightStrength( float highlightStrength ) = 0;

	// Description:
	//    set/get cloak color channel
	virtual void SetCloakColorChannel( uint8 nCloakColorChannel ) = 0;
	virtual uint8 GetCloakColorChannel( ) const = 0;

	// Description:
	//    set/get cloak fade by distance
	virtual void SetCloakFadeByDistance( bool bCloakFadeByDistance ) = 0;
	virtual bool DoesCloakFadeByDistance( ) const = 0;

	// Description:
	//    sets timescale for cloak blending (range 1 - 4)
	virtual void SetCloakBlendTimeScale( float fCloakBlendTimeScale ) = 0;
	virtual float GetCloakBlendTimeScale( ) const =0;

	// Description:
	//    set/get ignore cloak refraction color
	virtual void SetIgnoreCloakRefractionColor(bool bIgnoreCloakRefractionColor) = 0;
	virtual bool DoesIgnoreCloakRefractionColor() const = 0;

	// Description:
	//    sets custom post effect
	virtual void SetCustomPostEffect( const char* pPostEffectName ) = 0;

	// Description:
	//    sets object to be rendered in post 3d pass
	//		bPost3dRenderObject - flag object to render in post 3d render pass
	//		groupId - assign group ID's to groups of objects that render in the same screen rect
	//		groupScreenRect - screen rect that the grouped objects render in (0.0->1.0)
	virtual void SetAsPost3dRenderObject( bool bPost3dRenderObject, uint8 groupId, float groupScreenRect[4] ) = 0;
	
	// Description:
	//    sets hud render proxy to ignore hud interference filter
	virtual void SetIgnoreHudInterferenceFilter(const bool bIgnoreFiler) = 0;

	// Description:
	//    sets whether 3D HUD Objects require to be rendered at correct depth (i.e. behind weapon)
	virtual void SetHUDRequireDepthTest(const bool bRequire) = 0;

	// Description:
	//    sets whether 3D HUD Object should have bloom disabled (Required to avoid overglow and cutoff with alien hud ghosted planes)
	virtual void SetHUDDisableBloom(const bool bDisableBloom) = 0;

	// Description:
	//    set render proxy to ignore heat value of object
	virtual void SetIgnoreHeatAmount( bool bIgnoreHeat ) = 0;

  // Description:
 //    set/get vision params (thermal/sonar/etc)
  virtual void SetVisionParams( float r, float g, float b, float a ) = 0;
  virtual uint32 GetVisionParams() const = 0;
  
	// Description:
	//    set/get hud silhouetes params 
	virtual void SetHUDSilhouettesParams( float r, float g, float b, float a ) = 0;
	virtual uint32 GetHUDSilhouettesParams() const = 0;
  
	// Description:
	//    set/get shadow dissolving (fade out for phantom perk etc) 
	virtual void SetShadowDissolve( bool enable ) = 0;
	virtual bool GetShadowDissolve() const = 0;

	// Description:
	//    set/get effect layer params (used for game related layer effects - eg: nanosuit effects)
	virtual void SetEffectLayerParams( const Vec4 &pParams ) = 0;
	virtual void SetEffectLayerParams( uint32 nEncodedParams ) = 0;
	virtual const uint32 GetEffectLayerParams() const = 0;

  // Description:
  //    set/get opacity
  virtual void SetOpacity(float fAmount) = 0;
  virtual float GetOpacity() const = 0;

	// Description:
	//	return the last time (as set by the system timer) when the renderproxy was last seen.
	virtual	float	GetLastSeenTime() const = 0;

	// Description:
	//	return true if entity visarea was visible during last frames
	virtual	bool IsRenderProxyVisAreaVisible() const = 0;

	// Description:
	//	Removes all slots from the render proxy
	virtual void ClearSlots() = 0;
	// Description:
	//	Enables / Disables motion blur
	virtual void SetMotionBlur(bool enable) = 0;

	// Description:
	//    Set the viewDistRatio on the render node.
	virtual void SetViewDistRatio(int nViewDistRatio) = 0;
	
	// Description:
	//    Sets the LodRatio on the render node.
	virtual void SetLodRatio(int nLodRatio) = 0;

	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityRenderProxy );

// Description:
//		Type of an area managed by IEntityAreaProxy.
enum EEntityAreaType
{
	ENTITY_AREA_TYPE_SHAPE,     // Area type is a closed set of points forming shape.
	ENTITY_AREA_TYPE_BOX,       // Area type is a oriented bounding box.
	ENTITY_AREA_TYPE_SPHERE,    // Area type is a sphere.
	ENTITY_AREA_TYPE_GRAVITYVOLUME,    // Area type is a volume around a bezier curve.
	ENTITY_AREA_TYPE_SOLID			// Area type is a solid which can have any geometry figure.
};


// Description:
//     Area proxy allow for entity to host an area trigger.
//     Area can be shape, box or sphere, when marked entities cross this area border,
//     it will send ENTITY_EVENT_ENTERAREA,ENTITY_EVENT_LEAVEAREA,ENTITY_EVENT_AREAFADE, 
//     events to the target entities.
UNIQUE_IFACE struct IEntityAreaProxy : public IEntityProxy
{
	enum EAreaProxyFlags
	{
		FLAG_NOT_UPDATE_AREA = BIT(1), // When set points in the area will not be updated.
		FLAG_NOT_SERIALIZE   = BIT(2)  // Areas with this flag will not be serialized
	};

	// <interfuscator:shuffle>
	// Area flags.
	virtual void SetFlags( int nAreaProxyFlags ) = 0;
	// Area flags.
	virtual int  GetFlags() = 0;

	// Description:
	//    Retrieve area type.
	// Return:
	//    One of EEntityAreaType enumerated types.
	virtual EEntityAreaType GetAreaType() const = 0;

	// Description:
	//    Sets area to be a shape, and assign points to this shape.
	//    Points are specified in local entity space, shape will always be constructed in XY plane,
	//    lowest Z of specified points will be used as a base Z plane.
	//    If fHeight parameter is 0, shape will be considered 2D shape, and during intersection Z will be ignored
	//    If fHeight is not zero shape will be considered 3D and will accept intersection within vertical range from baseZ to baseZ+fHeight.
	// Arguments:
	//    vPoints					- Array of 3D vectors defining shape vertices.
	//		vObstructSound	- Array of corresponding booleans that indicate sound obstruction
	//    nPoints					- Number of vertices in vPoints array.
	//    fHeight					- Height of the shape .
	// See Also: SetSphere,SetBox,GetSphere,GetBox,GetPoints
	virtual void	SetPoints( const Vec3* const vPoints, const bool* const pabSoundObstructionSegments, int const nPointsCount, float const fHeight ) = 0;

	// Description:
	//    Sets area to be a Box, min and max must be in local entity space.
	//    Host entity orientation will define the actual world position and orientation of area box.
	// See Also: SetSphere,SetPoints,GetSphere,GetBox,GetPoints
	virtual void	SetBox( const Vec3& min,const Vec3& max, const bool* const pabSoundObstructionSides, size_t const nSideCount ) = 0;

	// Description:
	//    Sets area to be a Sphere, center and radius must be specified in local entity space.
	//    Host entity world position will define the actual world position of the area sphere.
	// See Also: SetBox,SetPoints,GetSphere,GetBox,GetPoints
	virtual void	SetSphere( const Vec3& vCenter,float fRadius ) = 0;
	
	// Description:
	//		This function need to be called before setting convexhulls for a AreaSolid,
	//		And then AddConvexHullSolid() function is called as the number of convexhulls consisting of a geometry.
	//	See Also : AddConvexHullToSolid,EndSettingSolid
	virtual void	BeginSettingSolid( const Matrix34& worldTM ) = 0;

	// Description:
	//		Add a convexhull to a solid. This function have to be called after calling BeginSettingSolid()
	//	See Also : BeginSettingSolid,EndSettingSolid
	virtual void	AddConvexHullToSolid( const Vec3* verticesOfConvexHull, bool bObstruction, int numberOfVertices ) = 0;

	// Description:
	//		Finish setting a solid geometry. Generally the BSPTree based on convexhulls which is set before is created in this function.
	//	See Also : BeginSettingSolid,AddConvexHullToSolid
	virtual void	EndSettingSolid() = 0;

	// Description:
	//    Retrieve number of points for shape area, return 0 if not area type is not shape.
	// See Also: SetPoints,GetPoints
	virtual int   GetPointsCount() = 0;
	// Description:
	//    Retrieve array of points for shape area, will return NULL for all other area types.
	// See Also: SetSphere,SetBox,SetPoints,GetSphere,GetBox
	virtual const Vec3* GetPoints() = 0;
	// Description:
	//    Retrieve shape area height, if height is 0 area is 2D.
	// See Also: SetPoints
	virtual float GetHeight() = 0;
	// Description:
	//    Retrieve min and max in local space of area box.
	// See Also: SetSphere,SetBox,SetPoints,GetSphere,GetBox,GetPoints
	virtual void	GetBox( Vec3& min,Vec3& max ) = 0;
	// Description:
	//    Retrieve center and radius of the sphere area in local space.
	// See Also: SetSphere,SetBox,SetPoints,GetSphere,GetBox,GetPoints
	virtual void	GetSphere( Vec3& vCenter,float &fRadius ) = 0;

	virtual void SetGravityVolume(const Vec3 * pPoints, int nNumPoints, float fRadius, float fGravity, bool bDontDisableInvisible, float fFalloff, float fDamping) = 0;

	// Description:
	//    Set area ID, this id will be provided to the script callback OnEnterArea , OnLeaveArea.
	// See Also: GetID
	virtual void	SetID( const int id ) = 0;
	// Description:
	//    Retrieve area ID.
	// See Also: SetID
	virtual int		GetID() const = 0;

	// Description:
	//    Set area group id, areas with same group id act as an exclusive areas,
	//    If 2 areas with same group id overlap, entity will be considered in the most internal area (closest to entity).
	// See Also: GetGroup
	virtual void	SetGroup( const int id) = 0;

	// Description:
	//    Retrieve area group id.
	// See Also: SetGroup
	virtual int		GetGroup( ) const = 0;

	// Description:
	//    Set priority defines the individual priority of an area,
	//    Area with same group ideas will depend on which has the higher priority
	// See Also: GetPriority
	virtual void	SetPriority( const int nPriority) = 0;

	// Description:
	//    Retrieve area priority.
	// See Also: SetPriority
	virtual int		GetPriority( ) const = 0;

	// Description:
	//    Sets sound obstruction depending on area type
	virtual void	SetSoundObstructionOnAreaFace( int unsigned const nFaceIndex, bool const bObstructs ) = 0;

	// Description:
	//    Add target entity to the area.
	//    When someone enters/leaves an area, it will send ENTERAREA,LEAVEAREA,AREAFADE, events to these target entities.
	// See Also: ClearEntities
	virtual void	AddEntity( EntityId id ) = 0;

	// Description:
	//    Add target entity to the area.
	//    When someone enters/leaves an area, it will send ENTERAREA,LEAVEAREA,AREAFADE, events to these target entities.
	// See Also: ClearEntities
	virtual void	AddEntity( EntityGUID guid ) = 0;

	// Description:
	//    Removes all added target entities.
	// See Also: AddEntity
	virtual void	ClearEntities() = 0;

	// Description:
	//    Set area proximity region near the border.
	//    When someone is moving within this proximity region from the area outside border
	//    Area will generate ENTITY_EVENT_AREAFADE event to the target entity, with a fade ratio from 0, to 1.
	//    Where 0 will be at he area outside border, and 1 inside the area in distance fProximity from the outside area border.
	// See Also: GetProximity
	virtual void	SetProximity( float fProximity ) = 0;

	// Description:
	//    Retrieves area proximity.
	// See Also: SetProximity
	virtual float	GetProximity() = 0;

	// Description:
	//    computes and returned squared distance to a point which is outside
	//    OnHull3d is the closest point on the hull of the area
	virtual float CalcPointNearDistSq(EntityId const nEntityID, Vec3 const& Point3d, Vec3& OnHull3d) = 0;

	// Description:
	//    computes and returned squared distance from a point to the hull of the area
	//    OnHull3d is the closest point on the hull of the area
	//		This function is not sensitive of if the point is inside or outside the area
	virtual float ClosestPointOnHullDistSq(EntityId const nEntityID, Vec3 const& Point3d, Vec3& OnHull3d) = 0;

	// Description:
	//    checks if a given point is inside the area
	//    ignoring the height speeds up the check
	virtual bool	CalcPointWithin(EntityId const nEntityID, Vec3 const& Point3d, bool const bIgnoreHeight = false) const = 0;
  // </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityAreaProxy );

//////////////////////////////////////////////////////////////////////////
// Description:
//     Boids proxy allow entity to host flocks of birds or fishes.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityBoidsProxy : public IEntityProxy
{

};

DECLARE_COMPONENT_POINTERS( IEntityBoidsProxy );

//////////////////////////////////////////////////////////////////////////
// Description:
//     Flow Graph proxy allows entity to host reference to the flow graph.
//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityFlowGraphProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	virtual void SetFlowGraph( IFlowGraph *pFlowGraph ) = 0;
	virtual IFlowGraph* GetFlowGraph() = 0;

	virtual void AddEventListener( IEntityEventListener *pListener ) = 0;
	virtual void RemoveEventListener( IEntityEventListener *pListener ) = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityFlowGraphProxy );

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//     Substitution proxy remembers IRenderNode this entity substitutes and unhides it upon deletion
/////////////////////////////////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntitySubstitutionProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	virtual void SetSubstitute(IRenderNode *pSubstitute) = 0;
	virtual IRenderNode *GetSubstitute() = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntitySubstitutionProxy );

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//     Represents entity camera.
/////////////////////////////////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityCameraProxy : public IEntityProxy
{
	// <interfuscator:shuffle>
	virtual void SetCamera( CCamera &cam ) = 0;
	virtual CCamera& GetCamera() = 0;
	// </interfuscator:shuffle>
};

DECLARE_COMPONENT_POINTERS( IEntityCameraProxy );

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
//     Proxy for the entity rope.
/////////////////////////////////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IEntityRopeProxy : public IEntityProxy
{
	virtual struct IRopeRenderNode* GetRopeRenderNode() = 0;
};

DECLARE_COMPONENT_POINTERS( IEntityRopeProxy );

#endif // __IEntityProxy_h__

