#include DEVIRTUALIZE_HEADER_FIX(IEntityRenderState.h)

#ifndef IRenderNodeSTATE_H
#define IRenderNodeSTATE_H

#if (!defined(PS3) && !defined(XENON))
//on PS3 all goes through the shrinked occlusion buffer from GPU directly
#define SUPP_HMAP_OCCL
#define SUPP_HWOBJ_OCCL
#endif

struct IMaterial;
struct IVisArea;
struct SEntityEvent;
struct SRenderingPassInfo;
struct SRendItemSorter;

enum EERType
{
  eERType_NotRenderNode,
  eERType_Brush,
  eERType_Vegetation,
  eERType_Light,
  eERType_Cloud,
  eERType_Dummy_1, // used to be eERType_VoxelObject, preserve order for compatibility
  eERType_FogVolume,
  eERType_Decal,
  eERType_ParticleEmitter,
  eERType_WaterVolume,
  eERType_WaterWave,
  eERType_Road,
  eERType_DistanceCloud,
  eERType_VolumeObject,
  eERType_Dummy_0, // used to be eERType_AutoCubeMap, preserve order for compatibility
  eERType_Rope,
  eERType_PrismObject,
  eERType_Dummy_2, // used to be eERType_IsoMesh, preserve order for compatibility
  eERType_LightPropagationVolume,
  eERType_RenderProxy,
  eERType_GameEffect,
  eERType_BreakableGlass,
  eERType_LightShape,
  eERType_MergedMesh,
  eERType_TypesNum, // MUST BE AT END TOTAL NUMBER OF ERTYPES
};

enum ERNListType
{
  eRNListType_Unknown,
  eRNListType_Brush,
  eRNListType_Vegetation,
  eRNListType_DecalsAndRoads,
  eRNListType_ListsNum,
  eRNListType_First=eRNListType_Unknown, // This should be the last member
  // And it counts on eRNListType_Unknown
  // being the first enum element.
};

enum EOcclusionObjectType
{
  eoot_OCCLUDER,
  eoot_OCEAN,
  eoot_OCCELL,
  eoot_OCCELL_OCCLUDER,
  eoot_OBJECT,
  eoot_OBJECT_TO_LIGHT,
  eoot_TERRAIN_NODE,
  eoot_PORTAL,
};

struct OcclusionTestClient
{
  OcclusionTestClient() : nLastOccludedMainFrameID(0),nLastVisibleMainFrameID(0) 
  {
#ifdef SUPP_HMAP_OCCL
    vLastVisPoint.Set(0,0,0); 
    nTerrainOccLastFrame = 0; 
#endif
#ifdef SUPP_HWOBJ_OCCL
    bOccluded=true;
    pREOcclusionQuery=0; 
#endif
    //		nInstantTestRequested=0; 
  }
#ifdef SUPP_HWOBJ_OCCL
  ~OcclusionTestClient() 
  { 
    if(pREOcclusionQuery)
      pREOcclusionQuery->Release(false); 
  }
#endif
  uint32 nLastVisibleMainFrameID, nLastOccludedMainFrameID;
  uint32 nLastShadowCastMainFrameID, nLastNoShadowCastMainFrameID;
#ifdef SUPP_HMAP_OCCL
  Vec3 vLastVisPoint;
  int nTerrainOccLastFrame;
#endif
#ifdef SUPP_HWOBJ_OCCL
  class CREOcclusionQuery * pREOcclusionQuery;
  uint8 bOccluded;
#endif
  //  uint8 nInstantTestRequested;
};

// RenderNode flags

#define ERF_GOOD_OCCLUDER						0x1
#define ERF_PROCEDURAL							0x2
#define ERF_USE_TERRAIN_COLOR					0x4
#define ERF_CASTSHADOWMAPS						0x8		// if you ever set this flag, be sure also to set ERF_HAS_CASTSHADOWMAPS
#define ERF_RENDER_ALWAYS				        0x10
#define ERF_CASTSHADOWINTORAMMAP				0x20
#define ERF_HIDABLE								0x40
#define ERF_HIDABLE_SECONDARY					0x80
#define ERF_HIDDEN								0x100
#define ERF_SELECTED							0x200
#define ERF_OUTDOORONLY							0x800
#define ERF_EXCLUDE_FROM_TRIANGULATION			0x2000
#define ERF_REGISTER_BY_BBOX					0x4000
#define ERF_PICKABLE				            0x8000
#define ERF_NO_PHYSICS  						0x20000
#define ERF_NO_DECALNODE_DECALS 				0x40000
#define ERF_REGISTER_BY_POSITION				0x80000
#define ERF_SUBSURFSCATTER						0x100000
#define ERF_RECVWIND							0x200000
#define ERF_COLLISION_PROXY						0x400000    // Collision proxy is a special object that is only visible in editor
// and used for physical collisions with player and vehicles.
#define ERF_SPEC_BIT0							0x1000000   // Bit0 of min config specification.
#define ERF_SPEC_BIT1							0x2000000   // Bit1 of min config specification.
#define ERF_SPEC_BIT2							0x4000000   // Bit2 of min config specification.
#define ERF_SPEC_BITS_MASK              (ERF_SPEC_BIT0|ERF_SPEC_BIT1|ERF_SPEC_BIT2)  // Bit mask of the min spec bits.
#define ERF_SPEC_BITS_SHIFT             (24)				// Bit oOffset of the ERF_SPEC_BIT0.
#define ERF_RAYCAST_PROXY					0x8000000   // raycast proxy is only used for raycasting
#define ERF_HUD										0x10000000	// Hud object that can avoid some visibility tests
#define ERF_RAIN_OCCLUDER					0x20000000  // Is used for rain occlusion map
#define ERF_HAS_CASTSHADOWMAPS		0x40000000  // at one point had ERF_CASTSHADOWMAPS set
#define ERF_ACTIVE_LAYER		      0x80000000  // the node is on a currently active layer

struct IShadowCaster
{
	// <interfuscator:shuffle>
  virtual ~IShadowCaster(){}
  virtual bool HasOcclusionmap(int nLod, IRenderNode *pLightOwner ) { return false;}
  virtual void Render(const SRendParams &RendParams, const SRenderingPassInfo &passInfo ) = 0;
  virtual const AABB GetBBoxVirtual() = 0;
	virtual void FillBBox(AABB & aabb) =0;
  virtual struct ICharacterInstance* GetEntityCharacter( unsigned int nSlot, Matrix34A * pMatrix = NULL, bool bReturnOnlyVisible = false ) = 0;
  virtual EERType GetRenderNodeType() = 0;
  virtual bool IsRenderNode() { return true; }
	// </interfuscator:shuffle>
};

struct IOctreeNode 
{
public:
	struct CTerrainNode* GetTerrainNode() const { return (CTerrainNode*) (m_pTerrainNode & ~0x1); }
	void SetTerrainNode(struct CTerrainNode* node) { m_pTerrainNode = (m_pTerrainNode & 0x1) | ((INT_PTR) node); }

// If true - this node needs to be recompiled for example update nodes max view distance.
	bool IsCompiled() const { return (bool) (m_pTerrainNode & 0x1); }
	void SetCompiled(bool compiled) { m_pTerrainNode = ((int) compiled) | (m_pTerrainNode & ~0x1); }

  struct CVisArea * m_pVisArea;

private:
	INT_PTR m_pTerrainNode;
};

struct SLodDistDissolveTransitionState
{
  float fStartDist;
	int8 nOldLod;
	int8 nNewLod;
	bool bFarside;
};

struct SLightInfo
{
  bool operator == (const SLightInfo & other) const 
  { return other.vPos.IsEquivalent(vPos, 0.1f) && fabs(other.fRadius - fRadius)< 0.1f; }
  Vec3 vPos;
  float fRadius;
  bool bAffecting;
};

struct IRenderNode : public IShadowCaster
{
  enum EInternalFlags
  {
    DECAL_OWNER     = BIT(0),				// Owns some decals.
    REQUIRES_NEAREST_CUBEMAP = BIT(1),	// Pick nearest cube map
    UPDATE_DECALS   = BIT(2),				// The node changed geometry - decals must be updated.
    REQUIRES_FORWARD_RENDERING = BIT(3), // Special shadow processing needed.
    WAS_INVISIBLE   = BIT(4),				// Was invisible last frame.
    WAS_IN_VISAREA  = BIT(5),				// Was inside vis-ares last frame.
    WAS_FARAWAY			= BIT(6),			  // Was considered 'far away' for the purposes of physics deactivation.
    HAS_OCCLUSION_PROXY = BIT(7)		// This node has occlusion proxy.
  };

  IRenderNode()
  {
    m_dwRndFlags = 0;
    m_ucViewDistRatio = 100;
    m_ucLodRatio = 100;
    m_pOcNode = 0;
    m_fWSMaxViewDist = 0;
    m_nInternalFlags = 0;
    m_nMaterialLayers = 0;    
    m_pRNTmpData = NULL;
    m_pPrev = m_pNext = NULL;
    m_nSID = 0;
		m_cShadowLodBias = 0;
  }

	// <interfuscator:shuffle>
  // Debug info about object.
  virtual const char* GetName() const = 0;
  virtual const char* GetEntityClassName() const = 0;
  virtual string GetDebugString( char type = 0 ) const { return ""; }
  virtual float GetImportance() const	{ return 1.f; }

  // Description:
  //    Releases IRenderNode.
  virtual void ReleaseNode(bool bImmediate = false) { delete this; }


  virtual IRenderNode * Clone() const { return NULL; }

  // Description:
  //    Sets render node transformation matrix.
  virtual void SetMatrix( const Matrix34& mat ) {}

  // Description:
  //    Gets local bounds of the render node.
  virtual void GetLocalBounds( AABB &bbox ) { AABB WSBBox(GetBBox()); bbox = AABB(WSBBox.min - GetPos(true), WSBBox.max - GetPos(true)); }

  virtual Vec3 GetPos(bool bWorldOnly = true) const = 0;
  virtual const AABB GetBBox() const = 0;
  virtual void FillBBox(AABB & aabb) { aabb = GetBBox(); }
  virtual void SetBBox( const AABB& WSBBox ) = 0;

  // Summary:
  //    Changes the world coordinates position of this node by delta
  //    Don't forget to call this base function when overriding it.
  virtual void OffsetPosition(const Vec3& delta) = 0;

  // Summary:
  //	 Renders node geometry
  virtual void Render(const struct SRendParams & EntDrawParams, const SRenderingPassInfo &passInfo ) = 0;

  // Hides/disables node in renderer.
  virtual void Hide( bool bHide ) { SetRndFlags( ERF_HIDDEN, bHide ); }

  // Gives access to object components.
  virtual struct IStatObj * GetEntityStatObj( unsigned int nPartId = 0, unsigned int nSubPartId = 0, Matrix34A * pMatrix = NULL, bool bReturnOnlyVisible = false);
  virtual IMaterial * GetEntitySlotMaterial( unsigned int nPartId, bool bReturnOnlyVisible = false, bool * pbDrawNear = NULL ) { return NULL; }
  virtual void SetEntityStatObj( unsigned int nSlot, IStatObj * pStatObj, const Matrix34A * pMatrix = NULL ) {};
  virtual struct ICharacterInstance* GetEntityCharacter( unsigned int nSlot, Matrix34A * pMatrix = NULL, bool bReturnOnlyVisible = false ) { return 0; }
  virtual int GetSlotCount() const { return 1; }

	// Summary:
	//   Returns IRenderMesh of the object.
	virtual struct IRenderMesh * GetRenderMesh(int nLod) { return 0; };

	// Description:
	//	 Allows to adjust default lod distance settings, 
	//	 if fLodRatio is 100 - default lod distance is used.
	virtual void SetLodRatio(int nLodRatio) { m_ucLodRatio = min(255,max(0,nLodRatio)); }

	// Summary:
	//	 Gets material layers mask.
	virtual uint8 GetMaterialLayers() const { return m_nMaterialLayers; }

	// Summary:
	//	 Gets/sets physical entity.
	virtual struct IPhysicalEntity* GetPhysics() const = 0;
	virtual void SetPhysics( IPhysicalEntity* pPhys ) = 0;

	// Summary
	//	 Physicalizes if it isn't already.
	virtual void CheckPhysicalized() {};

	// Summary:
	//	 Physicalizes node.
	virtual void Physicalize(bool bInstant=false) {} 
	// Summary:
	//	 Physicalize stat object's foliage.
	virtual bool PhysicalizeFoliage(bool bPhysicalize=true, int iSource=0, int nSlot=0) { return false; } 
	// Summary:
	//	 Gets physical entity (rope) for a given branch (if foliage is physicalized).
	virtual IPhysicalEntity* GetBranchPhys(int idx, int nSlot=0) { return 0; }
	// Summary:
	//	 Returns physicalized foliage, if it *is* physicalized.
	virtual struct IFoliage* GetFoliage(int nSlot=0) { return 0; }

	// Make sure I3DEngine::FreeRenderNodeState(this) is called in destructor of derived class.
	virtual ~IRenderNode() { assert(!m_pRNTmpData); };

	// Summary:
	//	 Sets override material for this instance.
	virtual void SetMaterial( IMaterial *pMat ) = 0;
	// Summary:
	//	 Queries override material of this instance.
	virtual IMaterial* GetMaterial(Vec3 * pHitPos = NULL) = 0;
	virtual IMaterial* GetMaterialOverride() = 0;

	// Used by the editor during export
	virtual void SetCollisionClassIndex(int tableIndex) {}

	virtual int GetEditorObjectId() { return 0; }
	virtual void SetEditorObjectId(int nEditorObjectId) {}
	virtual void SetStatObjGroupId(int nVegetationanceGroupId) { }
	virtual int GetStatObjGroupId() const { return -1; }
	virtual void SetLayerId(uint16 nLayerId) { }
	virtual uint16 GetLayerId() { return 0; }
	virtual float GetMaxViewDist() = 0;

	virtual EERType GetRenderNodeType() = 0;
	virtual bool IsAllocatedOutsideOf3DEngineDLL() { return GetRenderNodeType() == eERType_RenderProxy; }
	virtual void Dephysicalize( bool bKeepIfReferenced=false ) {}
	virtual void Dematerialize( ) {}
	virtual void GetMemoryUsage(ICrySizer * pSizer) const = 0;

	virtual void Precache() {};

	virtual const AABB GetBBoxVirtual() { return GetBBox(); }

	//	virtual float GetLodForDistance(float fDistance) { return 0; }

	virtual void OnRenderNodeBecomeVisible(const SRenderingPassInfo &passInfo) {}
	virtual void OnEntityEvent( IEntity* pEntity, SEntityEvent const& event ) {}
	virtual void OnPhysAreaChange() {}

	virtual	bool IsMovableByGame() const { return false; }

	virtual uint8 GetSortPriority() { return 0; }

	virtual void SetMinSpec( int nMinSpec ) { m_dwRndFlags &= ~ERF_SPEC_BITS_MASK; m_dwRndFlags |= (nMinSpec << ERF_SPEC_BITS_SHIFT) & ERF_SPEC_BITS_MASK; };

	// Description:
	//	Allows to adjust default max view distance settings, 
	//	if fMaxViewDistRatio is 100 - default max view distance is used.
	virtual void SetViewDistRatio(int nViewDistRatio);
	// </interfuscator:shuffle>

  void CopyIRenderNodeData(IRenderNode * pDest) const
  {
    pDest->m_fWSMaxViewDist			= m_fWSMaxViewDist;
    pDest->m_dwRndFlags					= m_dwRndFlags;
    //pDest->m_pOcNode						= m_pOcNode;		// Removed to stop the registering from earlying out.
    pDest->m_ucViewDistRatio		= m_ucViewDistRatio;
    pDest->m_ucLodRatio					= m_ucLodRatio;
		pDest->m_cShadowLodBias			= m_cShadowLodBias;
    pDest->m_nInternalFlags			= m_nInternalFlags;
    pDest->m_nMaterialLayers		= m_nMaterialLayers;
    //pDestBrush->m_pRNTmpData				//If this is copied from the source render node, there are two
    //	pointers to the same data, and if either is deleted, there will
    //	be a crash when the dangling pointer is used on the other
  }

  // Rendering flags.
  ILINE void SetRndFlags(unsigned int dwFlags) { m_dwRndFlags = dwFlags; }
  ILINE void SetRndFlags(unsigned int dwFlags, bool bEnable) { if(bEnable) SetRndFlags(m_dwRndFlags|dwFlags); else SetRndFlags(m_dwRndFlags&(~dwFlags)); }
  ILINE unsigned int GetRndFlags() const { return m_dwRndFlags; }

  // Object draw frames (set if was drawn).
  ILINE void SetDrawFrame( int nFrameID, int nRecursionLevel ) 
  { 
    assert(m_pRNTmpData);
    int * pDrawFrames = (int *)m_pRNTmpData;
    pDrawFrames[nRecursionLevel] = nFrameID; 
  }

  ILINE int GetDrawFrame( int nRecursionLevel = 0 ) const
  { 
    IF(!m_pRNTmpData, 0)
      return 0;

    int * pDrawFrames = (int *)m_pRNTmpData;
    return pDrawFrames[nRecursionLevel]; 
  }

  // Returns:
  //   Current VisArea or null if in outdoors or entity was not registered in 3dengine.
  ILINE IVisArea * GetEntityVisArea() const { return m_pOcNode ? (IVisArea*)(m_pOcNode->m_pVisArea) : NULL; }

  // Returns:
  //   Current VisArea or null if in outdoors or entity was not registered in 3dengine.
  struct CTerrainNode * GetEntityTerrainNode() const { return (m_pOcNode && !m_pOcNode->m_pVisArea) ? m_pOcNode->GetTerrainNode() : NULL; }

  // Summary:
  //	 Makes object visible at any distance.
  ILINE void SetViewDistUnlimited() { SetViewDistRatio(255); }

  // Summary:
  //   Retrieves the view distance settings.
  ILINE int GetViewDistRatio() const { return (m_ucViewDistRatio==255) ? 1000l : m_ucViewDistRatio; }

  // Summary:
  //   Retrieves the view distance settings without any value interpretation.
  ILINE int GetViewDistRatioVal() const { return m_ucViewDistRatio; }

  // Summary:
  //	 Returns max view distance ratio.
  ILINE float GetViewDistRatioNormilized() const 
  { 
    if(m_ucViewDistRatio==255)
      return 100.f;
    return (float)m_ucViewDistRatio * 0.01f;
  }

  // Summary:
  //	 Returns lod distance ratio.
  ILINE int GetLodRatio() const { return m_ucLodRatio; }

  // Summary:
  //	 Returns lod distance ratio
  ILINE float GetLodRatioNormalized() const { return 0.01f*m_ucLodRatio; }

	// Description:
	//	Bias value to add to the regular lod
	virtual void SetShadowLodBias(int8 nShadowLodBias) { m_cShadowLodBias = nShadowLodBias; }

	// Summary:
	//	 Returns lod distance ratio.
	ILINE int GetShadowLodBias() const { return m_cShadowLodBias; }

  // Summary:
  //	 Sets material layers mask.
  ILINE void SetMaterialLayers(uint8 nMtlLayers) { m_nMaterialLayers = nMtlLayers; }

	ILINE int  GetMinSpec() const { return (m_dwRndFlags&ERF_SPEC_BITS_MASK) >> ERF_SPEC_BITS_SHIFT; };

	static const ERNListType GetRenderNodeListId(const EERType eRType) 
  { 
    switch(eRType)
    {
    case eERType_Vegetation:
      return eRNListType_Vegetation;
    case eERType_Brush:
      return eRNListType_Brush;
    case eERType_Decal:
    case eERType_Road:
      return eRNListType_DecalsAndRoads;
    default:
      return eRNListType_Unknown;
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // Variables
  //////////////////////////////////////////////////////////////////////////

public:

  // Every sector has linked list of IRenderNode objects.
  IRenderNode * m_pNext, * m_pPrev;

  // Current objects tree cell.
  IOctreeNode * m_pOcNode;

  // Pointer to temporary data allocated only for currently visible objects.
  struct CRNTmpData * m_pRNTmpData;

  // Max view distance.
  float m_fWSMaxViewDist;

  // Render flags.
  int m_dwRndFlags; 

	// Shadow LOD bias
	int8 m_cShadowLodBias;

  // Segment Id
  int m_nSID;

  // Max view distance settings.
  unsigned char m_ucViewDistRatio;

  // LOD settings.
  unsigned char m_ucLodRatio;

  // Flags for render node internal usage, one or more bits from EInternalFlags.
  unsigned char m_nInternalFlags;

  // Material layers bitmask -> which material layers are active.
  unsigned char m_nMaterialLayers;

};

inline void IRenderNode::SetViewDistRatio(int nViewDistRatio) 
{ 
	nViewDistRatio = SATURATEB(nViewDistRatio);
	if(m_ucViewDistRatio != nViewDistRatio)
	{
		m_ucViewDistRatio = nViewDistRatio; 
		if(m_pOcNode) 
			m_pOcNode->SetCompiled(false); 
	}
}

#if !defined(__SPU__)
///////////////////////////////////////////////////////////////////////////////
// function is copied to ObjectsTree_SPU.cpp for SPUs to prevent CryCG issues
inline IStatObj * IRenderNode::GetEntityStatObj( unsigned int nPartId, unsigned int nSubPartId, Matrix34A * pMatrix, bool bReturnOnlyVisible)
{ 
	return 0; 
}
#endif

//We must use interfaces instead of unsafe type casts and unnecessary includes
UNIQUE_IFACE struct IVegetation : public IRenderNode
{
  virtual float GetScale(void) const = 0;
};

UNIQUE_IFACE struct IBrush : public IRenderNode
{
  virtual const Matrix34& GetMatrix() const = 0;
	virtual void SetDrawLast(bool enable) = 0;
};

struct SVegetationSpriteInfo
{
  Sphere sp;
  float fScaleH;
  float fScaleV;
  struct SSectorTextureSet * pTerrainTexInfo;
  struct SVegetationSpriteLightInfo * pLightInfo;
  uint8 ucSunDotTerrain;
  uint8 ucAlphaTestRef;
	uint8 ucDissolveOut;
  uint8 ucShow3DModel;

	ColorB terrainColor;

  // Used only in case of sprite texture update.
  uint8 ucSlotId;
  struct IStatInstGroup * pStatInstGroup;

  // Used only by 3DEngine.
  const IRenderNode * pVegetation;

  void GetMemoryUsage( ICrySizer *pSizer ) const {/*nothing*/}
};

const int FAR_TEX_COUNT = 12;							// Number of sprites per object
const int FAR_TEX_ANGLE = (360/FAR_TEX_COUNT);
const int FAR_TEX_HAL_ANGLE = (256/FAR_TEX_COUNT)/2;

// Summary:
//	 Groups sprite gen data.
struct SVegetationSpriteLightInfo
{
  SVegetationSpriteLightInfo() { m_vSunDir = Vec3(0,0,0); m_MipFactor=0.0f; }

  float			m_MipFactor;
  IDynTexture * m_pDynTexture;

  // -----------------------------------------

  void SetLightingData( const Vec3 &vSunDir )
  {
    m_vSunDir=vSunDir;
  }

  // Notes:
  //	 vSunDir should be normalized.
  bool IsEqual( const Vec3 &vSunDir, const float fDirThreshold ) const
  {
    return IsEquivalent(m_vSunDir,vSunDir,fDirThreshold);
  }


private: // --------------------------------------------------------

  Vec3			m_vSunDir;								// normalized sun direction
};

UNIQUE_IFACE struct ILightSource : public IRenderNode
{
	// <interfuscator:shuffle>
	virtual void SetLightProperties(const CDLight & light) = 0;
	virtual CDLight &GetLightProperties() = 0;
	virtual const Matrix34& GetMatrix() = 0;
	virtual struct ShadowMapFrustum * GetShadowFrustum(int nId = 0) = 0;
	virtual bool IsLightAreasVisible() = 0;
	virtual void SetCastingException(IRenderNode * pNotCaster) = 0;
	virtual void SetCheapLight(const uint64 entityGUID) = 0;
	virtual bool IsCheapLight(uint64& sourceEntityGUID) const = 0;
	// </interfuscator:shuffle>
};

#if !defined(RENDERNODES_LEAN_AND_MEAN)
struct SCloudMovementProperties
{
  bool m_autoMove;
  Vec3 m_speed;
  Vec3 m_spaceLoopBox;
  float m_fadeDistance;
};

// Summary:
//	 ICloudRenderNode is an interface to the Cloud Render Node object.
struct ICloudRenderNode : public IRenderNode // make unique again for PS3 if feature is used (see RENDERNODES_LEAN_AND_MEAN in CryCommon/ProjectDefines.h)
{
	// <interfuscator:shuffle>
  // Description:
  //    Loads a cloud from a cloud description XML file.
  virtual bool LoadCloud( const char *sCloudFilename ) = 0;
  virtual bool LoadCloudFromXml( XmlNodeRef cloudNode ) = 0;
  virtual void SetMovementProperties(const SCloudMovementProperties& properties) = 0;
	// </interfuscator:shuffle>
};
#endif

// Summary:
//	 ILightShapeRenderNode is an interface to the LightShape render node object.
UNIQUE_IFACE struct ILightShapeRenderNode : public IRenderNode
{
  enum ELightShapeFlags
  {
    elsfDisableGI = 1<<0,
  };

	// <interfuscator:shuffle>
  // Description:
  //		Loads a light shape from cgf filename
  virtual bool LoadLightShape( const char * sLightShape, const uint32 nLSFlags ) = 0;
  virtual bool LoadLightBox( ) = 0;
  virtual void SetLSFlags( const uint32 nNewFlags ) = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 IRoadRenderNode is an interface to the Road Render Node object.
UNIQUE_IFACE struct IRoadRenderNode : public IRenderNode
{
	// <interfuscator:shuffle>
  virtual void SetVertices(const Vec3 *pVerts, int nVertsNum, float fTexCoordBegin, float fTexCoordEnd, float fTexCoordBeginGlobal, float fTexCoordEndGlobal) = 0;
  virtual void SetSortPriority(uint8 sortPrio) = 0;
  virtual void SetIgnoreTerrainHoles(bool bVal) = 0;
  virtual void GetClipPlanes(Plane * pPlanes, int nPlanesNum, int nVertId=0) = 0;
  virtual void GetTexCoordInfo(float * pTexCoordInfo) = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 IBreakableGlassRenderNode is an interface to the Breakable Glass Render Node object.
struct SBreakableGlassInitParams;
struct SBreakableGlassUpdateParams;
struct SBreakableGlassState;
struct SBreakableGlassCVars;
struct SGlassPhysFragment;

UNIQUE_IFACE struct IBreakableGlassRenderNode : public IRenderNode
{
	// <interfuscator:shuffle>
	virtual bool		InitialiseNode(const SBreakableGlassInitParams& params, const Matrix34& matrix) = 0;
	virtual void		SetId(const uint16 id) = 0;
	virtual uint16	GetId() = 0;

	virtual void		Update(SBreakableGlassUpdateParams& params) = 0;
	virtual bool		HasGlassShattered() = 0;
	virtual bool		HasActiveFragments() = 0;
	virtual void		ApplyImpactToGlass(const EventPhysCollision* pPhysEvent) = 0;
	virtual void		ApplyExplosionToGlass(const EventPhysCollision* pPhysEvent) = 0;

	virtual void		DestroyPhysFragment(SGlassPhysFragment* pPhysFrag) = 0;

	virtual void		SetCVars(const SBreakableGlassCVars* pCVars) = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 IVoxelObject is an interface to the Voxel Object Render Node object.
UNIQUE_IFACE struct IVoxelObject : public IRenderNode
{	
  DEVIRTUALIZATION_VTABLE_FIX

	// <interfuscator:shuffle>
  virtual struct IMemoryBlock * GetCompiledData(EEndian eEndian) = 0;
  virtual void SetCompiledData(void * pData, int nSize, uint8 ucChildId, EEndian eEndian) = 0;
  virtual void SetObjectName( const char * pName ) = 0;
  virtual void SetMatrix( const Matrix34& mat ) = 0;
  virtual bool ResetTransformation() = 0;
  virtual void InterpolateVoxelData() = 0;
  virtual void SetFlags(int nFlags) = 0;
  virtual void Regenerate() = 0;
  virtual void CopyHM() = 0;
  virtual bool IsEmpty() = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 IFogVolumeRenderNode is an interface to the Fog Volume Render Node object.
struct SFogVolumeProperties
{	
  // Common parameters.
  // Center position & rotation values are taken from the entity matrix.

  int			m_volumeType;
  Vec3		m_size; 
  ColorF		m_color;
  bool		m_useGlobalFogColor;
  float		m_globalDensity;
  float		m_densityOffset;
  float		m_softEdges;
  float		m_fHDRDynamic;				// 0 to get the same results in LDR, <0 to get darker, >0 to get brighter.
  float		m_nearCutoff;

  float m_heightFallOffDirLong;			// Height based fog specifics.
  float m_heightFallOffDirLati;			// Height based fog specifics.
  float m_heightFallOffShift;				// Height based fog specifics.
  float m_heightFallOffScale;				// Height based fog specifics.
};

UNIQUE_IFACE struct IFogVolumeRenderNode : public IRenderNode
{
	// <interfuscator:shuffle>
  virtual void SetFogVolumeProperties( const SFogVolumeProperties& properties ) = 0;
  virtual const Matrix34& GetMatrix() const = 0;

  virtual void FadeGlobalDensity(float fadeTime, float newGlobalDensity) = 0;
	// </interfuscator:shuffle>
};


struct SDecalProperties
{
  SDecalProperties()
  {
    m_projectionType = ePlanar;
    m_sortPrio = 0;
    m_deferred = false;
    m_pos = Vec3(0.0f,0.0f,0.0f);
    m_normal = Vec3(0.0f,0.0f,1.0f);
    m_explicitRightUpFront = Matrix33::CreateIdentity();
    m_radius = 1.0f;
    m_depth = 1.0f;
  }

  enum EProjectionType
  {
    ePlanar,
    eProjectOnStaticObjects,
    eProjectOnTerrain,
    eProjectOnTerrainAndStaticObjects
  };

  EProjectionType m_projectionType;
  uint8 m_sortPrio;
  uint8 m_deferred;
  Vec3 m_pos;
  Vec3 m_normal;
  Matrix33 m_explicitRightUpFront;
  float m_radius;
  float m_depth;
  const char* m_pMaterialName;
};

// Description:
//	 IDecalRenderNode is an interface to the Decal Render Node object.
UNIQUE_IFACE struct IDecalRenderNode : public IRenderNode
{
	// <interfuscator:shuffle>
  virtual void SetDecalProperties( const SDecalProperties& properties ) = 0;
  virtual const SDecalProperties* GetDecalProperties() const = 0;
  virtual const Matrix34& GetMatrix( ) = 0;
  virtual void CleanUpOldDecals() = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 IWaterVolumeRenderNode is an interface to the Water Volume Render Node object.
UNIQUE_IFACE struct IWaterVolumeRenderNode : public IRenderNode
{
  enum EWaterVolumeType
  {
    eWVT_Unknown,
    eWVT_Ocean,
    eWVT_Area,
    eWVT_River
  };

	// <interfuscator:shuffle>
  // Description:
  // Sets if the render node is attached to a parent entity
  // This must be called right after the object construction if it is the case
  // Only supported for Areas (not rivers or ocean)
  virtual void SetAreaAttachedToEntity( ) = 0;
  
  virtual void SetFogDensity( float fogDensity ) = 0;
  virtual float GetFogDensity() const = 0;
  virtual void SetFogColor( const Vec3& fogColor ) = 0;
  virtual void SetFogColorAffectedBySun(bool enable) = 0;
  virtual void SetFogShadowing(float fogShadowing) = 0;

  virtual void SetCapFogAtVolumeDepth(bool capFog) = 0;
  virtual void SetVolumeDepth(float volumeDepth) = 0;
  virtual void SetStreamSpeed(float streamSpeed) = 0;

  virtual void SetCaustics(bool caustics) = 0;
  virtual void SetCausticIntensity(float causticIntensity) = 0;
  virtual void SetCausticTiling(float causticTiling) = 0;
  virtual void SetCausticHeight(float causticHeight) = 0;
	virtual void SetAuxPhysParams(pe_params_area *) = 0;

  virtual void CreateOcean( uint64 volumeID, /* TBD */ bool keepSerializationParams = false ) = 0;
  virtual void CreateArea( uint64 volumeID, const Vec3* pVertices, unsigned int numVertices, const Vec2& surfUVScale, const Plane& fogPlane, bool keepSerializationParams = false, int nSID = -1 ) = 0;
  virtual void CreateRiver( uint64 volumeID, const Vec3* pVertices, unsigned int numVertices, float uTexCoordBegin, float uTexCoordEnd, const Vec2& surfUVScale, const Plane& fogPlane, bool keepSerializationParams = false, int nSID = -1 ) = 0;	

  virtual void SetAreaPhysicsArea( const Vec3* pVertices, unsigned int numVertices, bool keepSerializationParams = false ) = 0;
  virtual void SetRiverPhysicsArea( const Vec3* pVertices, unsigned int numVertices, bool keepSerializationParams = false ) = 0;

  virtual IPhysicalEntity* SetAndCreatePhysicsArea( const Vec3* pVertices, unsigned int numVertices ) = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 SWaterWaveParams is an interface to the Water Wave Render Node object.
struct SWaterWaveParams
{
  SWaterWaveParams():m_fSpeed( 5.0f ) , m_fSpeedVar( 2.0f ), m_fLifetime( 8.0f ) , m_fLifetimeVar( 2.0f ), 
    m_fHeight(0.75f), m_fHeightVar(0.4f), m_fPosVar( 5.0f ), m_fCurrLifetime(-1.0f), m_fCurrFrameLifetime(-1.0f), 
    m_fCurrSpeed(0.0f), m_fCurrHeight(1.0f)
  {

  }

  Vec3 m_pPos;
  float m_fSpeed, m_fSpeedVar;
  float m_fLifetime, m_fLifetimeVar;
  float m_fHeight, m_fHeightVar;
  float m_fPosVar;

  float m_fCurrLifetime;    
  float m_fCurrFrameLifetime;    
  float m_fCurrSpeed;
  float m_fCurrHeight;
};

UNIQUE_IFACE struct IWaterWaveRenderNode : public IRenderNode
{
	// <interfuscator:shuffle>
  virtual void Create( uint64 nID, const Vec3 *pVertices, uint32 nVertexCount, const Vec2 &pUVScale, const Matrix34 &pWorldTM ) = 0;	    
  virtual void SetParams( const SWaterWaveParams &pParams ) = 0;
  virtual const SWaterWaveParams &GetParams() const = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 IDistanceCloudRenderNode is an interface to the Distance Cloud Render Node object.
struct SDistanceCloudProperties
{
  Vec3 m_pos;
  float m_sizeX;
  float m_sizeY;
  float m_rotationZ;
  const char* m_pMaterialName;
};

UNIQUE_IFACE struct IDistanceCloudRenderNode : public IRenderNode
{
  virtual void SetProperties( const SDistanceCloudProperties& properties ) = 0;
};

#if !defined(RENDERNODES_LEAN_AND_MEAN)
struct SVolumeObjectProperties
{
};

struct SVolumeObjectMovementProperties
{
  bool m_autoMove;
  Vec3 m_speed;
  Vec3 m_spaceLoopBox;
  float m_fadeDistance;
};

// Description:
//	 IVolumeObjectRenderNode is an interface to the Volume Object Render Node object.
struct IVolumeObjectRenderNode : public IRenderNode // make unique again for PS3 if feature is used (see RENDERNODES_LEAN_AND_MEAN in CryCommon/ProjectDefines.h)
{
	// <interfuscator:shuffle>
  virtual void LoadVolumeData(const char* filePath) = 0;
  virtual void SetProperties(const SVolumeObjectProperties& properties) = 0;
  virtual void SetMovementProperties(const SVolumeObjectMovementProperties& properties) = 0;
	// </interfuscator:shuffle>
};
#endif

#if !defined(EXCLUDE_DOCUMENTATION_PURPOSE)
struct IPrismRenderNode :public IRenderNode
{
};
#endif // EXCLUDE_DOCUMENTATION_PURPOSE

//////////////////////////////////////////////////////////////////////////
UNIQUE_IFACE struct IRopeRenderNode : public IRenderNode
{
  enum ERopeParamFlags
  {
    eRope_BindEndPoints   = 0x0001,			// Bind rope at both end points.
    eRope_CheckCollisinos = 0x0002,			// Rope will check collisions.
    eRope_Subdivide       = 0x0004,			// Rope will be subdivided in physics.
    eRope_Smooth          = 0x0008,			// Rope will be smoothed after physics.
    eRope_NoAttachmentCollisions = 0x0010,	// Rope will ignore collisions against the objects it's attached to .
    eRope_Nonshootable		= 0x0020,		// Rope cannot be broken by shooting.
    eRope_Disabled				= 0x0040,		// simulation is completely disabled
    eRope_NoPlayerCollisions = 0x0080,		// explicit control over collisions with players
    eRope_StaticAttachStart  = 0x0100,		// attach start point to the 'world'
    eRope_StaticAttachEnd    = 0x0200,		// attach end point to the 'world'
    eRope_CastShadows				 = 0x0400,    // self-explanatory
    eRope_Awake							 = 0x0800,    // Rope will be awake initially
  };
  struct SRopeParams
  {
    int nFlags; // ERopeParamFlags.

    float fThickness;
    // Radius for the end points anchors that bind rope to objects in world.
    float fAnchorRadius;

    //////////////////////////////////////////////////////////////////////////
    // Rendering/Tessellation.
    //////////////////////////////////////////////////////////////////////////

    int nNumSegments;
    int nNumSides;
    float fTextureTileU;
    float fTextureTileV;

    //////////////////////////////////////////////////////////////////////////
    // Rope Physical/Params.
    //////////////////////////////////////////////////////////////////////////

    int nPhysSegments;
    int nMaxSubVtx;


    // Rope Physical parameters.

    float mass;        // Rope mass, if mass is 0 it will be static.
    float tension;
    float friction;
    float frictionPull;

    Vec3 wind;
    float windVariance;
    float airResistance;
    float waterResistance;

    float jointLimit;
    float maxForce;

    int nMaxIters;
    float maxTimeStep;
    float stiffness;
		float hardness;
    float damping;
    float sleepSpeed;
  };
  struct SEndPointLink
  {
    IPhysicalEntity *pPhysicalEntity;
    Vec3 offset;
    int nPartId;
  };

	// <interfuscator:shuffle>
  virtual void SetName( const char *sNodeName ) = 0;

  virtual void SetParams( const SRopeParams& params ) = 0;
  virtual const IRopeRenderNode::SRopeParams& GetParams() const = 0;

  virtual void   SetEntityOwner( uint32 nEntityId ) = 0;
  virtual uint32 GetEntityOwner() const = 0;

  virtual void SetPoints( const Vec3 *pPoints,int nCount ) = 0;
  virtual int GetPointsCount() const = 0;
  virtual const Vec3* GetPoints() const = 0;

  virtual void LinkEndPoints() = 0;
  virtual uint32 GetLinkedEndsMask() = 0;

  virtual void LinkEndEntities(IPhysicalEntity* pStartEntity, IPhysicalEntity* pEndEntity) = 0;
  // Description:
  //	 Retrieves information about linked objects at the end points, links must be a pointer to the 2 SEndPointLink structures.
  virtual void GetEndPointLinks( SEndPointLink *links ) = 0;

  // Summary:
  //	 Callback from physics.
  virtual void OnPhysicsPostStep() = 0;


  virtual void ResetPoints() = 0;

  // Sound related
  virtual void SetRopeSound( char const* const pcSoundName, int unsigned const nSegmentToAttachTo, float const fOffset ) = 0;
  virtual void StopRopeSound() = 0;
  virtual void ResetRopeSound() = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 ILPVRenderNode is an interface to the Light Propagation Volume Render Node object.
struct ILPVRenderNode : public IRenderNode // // make unique again for PS3 if feature is used (see RENDERNODES_LEAN_AND_MEAN in CryCommon/ProjectDefines.h)
{
	// <interfuscator:shuffle>
  virtual bool TryInsertLight( const CDLight &light ) = 0;
  virtual void UpdateMetrics(const Matrix34& mx, const bool recursive = false) = 0;
  virtual void SetDensity(const float fDensity) = 0;
  virtual float GetDensity() const = 0;
  virtual void EnableSpecular(const bool bEnabled) = 0;
  virtual bool IsSpecularEnabled() const = 0;
  virtual void GetMatrix(Matrix34& mxGrid) const = 0;
  virtual bool AutoFit(const DynArray<CDLight>& lightsToFit) = 0;
	// </interfuscator:shuffle>
};

#endif // IRenderNodeSTATE_H
