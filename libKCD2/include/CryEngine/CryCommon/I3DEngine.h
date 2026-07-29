////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   i3dengine.h
//  Version:     v1.00
//  Created:     28/5/2001 by Vladimir Kajalin
//  Compilers:   Visual Studio.NET
//  Description: 3dengine interface
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(I3DEngine.h)

#ifndef CRY3DENGINEINTERFACE_H
#define CRY3DENGINEINTERFACE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// !!! Do not add any headers here !!!
#include "CryEngineDecalInfo.h" 
#include <IStatObj.h> // <> required for Interfuscator
#include "IRenderer.h"
#include <IProcess.h> // <> required for Interfuscator
#include <IMaterial.h> // <> required for Interfuscator
#include <ISurfaceType.h> // <> required for Interfuscator
#include <IEntityRenderState.h> // <> required for Interfuscator
#include "CryArray.h"
#include <IMemory.h> // <> required for Interfuscator
// !!! Do not add any headers here !!!

struct ISystem;
struct ICharacterInstance;
struct CVars;
struct pe_params_particle;
struct IMaterial;
struct RenderLMData;
struct AnimTexInfo;
struct ISplineInterpolator;
class CContentCGF;
struct SpawnParams;
struct ForceObject;
class I3DSampler;
class ICrySizer;
struct IRenderNode;
struct CRNTmpData;
struct IParticleManager;
class IOpticsManager;
struct IDeferredPhysicsEventManager;

const float HDRDynamicMultiplier = 2.0f;

enum E3DEngineParameter
{
	E3DPARAM_SUN_COLOR,
	E3DPARAM_SKY_COLOR,

	E3DPARAM_SUN_SPECULAR_MULTIPLIER,

	E3DPARAM_AMBIENT_GROUND_COLOR,
	E3DPARAM_AMBIENT_MIN_HEIGHT,
	E3DPARAM_AMBIENT_MAX_HEIGHT,

	E3DPARAM_FOG_COLOR,
	E3DPARAM_FOG_COLOR2,
	E3DPARAM_FOG_RADIAL_COLOR,

	E3DPARAM_VOLFOG_HEIGHT_DENSITY,
	E3DPARAM_VOLFOG_HEIGHT_DENSITY2,

	E3DPARAM_VOLFOG_GRADIENT_CTRL,

	E3DPARAM_VOLFOG_GLOBAL_DENSITY,
	E3DPARAM_VOLFOG_RAMP,

	E3DPARAM_VOLFOG_SHADOW_RANGE,
	E3DPARAM_VOLFOG_SHADOW_DARKENING,
	E3DPARAM_VOLFOG_SHADOW_ENABLE,

	E3DPARAM_SKYLIGHT_SUN_INTENSITY,

	E3DPARAM_SKYLIGHT_KM,
	E3DPARAM_SKYLIGHT_KR,
	E3DPARAM_SKYLIGHT_G,

	E3DPARAM_SKYLIGHT_WAVELENGTH_R,
	E3DPARAM_SKYLIGHT_WAVELENGTH_G,
	E3DPARAM_SKYLIGHT_WAVELENGTH_B,

	E3DPARAM_NIGHSKY_HORIZON_COLOR,
	E3DPARAM_NIGHSKY_ZENITH_COLOR,
	E3DPARAM_NIGHSKY_ZENITH_SHIFT,

	E3DPARAM_NIGHSKY_STAR_INTENSITY,

	E3DPARAM_NIGHSKY_MOON_DIRECTION,
	E3DPARAM_NIGHSKY_MOON_COLOR,
	E3DPARAM_NIGHSKY_MOON_SIZE,
	E3DPARAM_NIGHSKY_MOON_INNERCORONA_COLOR,
	E3DPARAM_NIGHSKY_MOON_INNERCORONA_SCALE,
	E3DPARAM_NIGHSKY_MOON_OUTERCORONA_COLOR,
	E3DPARAM_NIGHSKY_MOON_OUTERCORONA_SCALE,

	E3DPARAM_CLOUDSHADING_MULTIPLIERS,
	E3DPARAM_CLOUDSHADING_SUNCOLOR,
	E3DPARAM_CLOUDSHADING_SKYCOLOR,

	E3DPARAM_CORONA_SIZE,

	E3DPARAM_OCEANFOG_COLOR,
	E3DPARAM_OCEANFOG_DENSITY,
	
	// Sky highlight (ex. From Lightning)
	E3DPARAM_SKY_HIGHLIGHT_COLOR,
	E3DPARAM_SKY_HIGHLIGHT_SIZE,
	E3DPARAM_SKY_HIGHLIGHT_POS,

	E3DPARAM_SKY_MOONROTATION,

	E3DPARAM_SKY_SKYBOX_ANGLE,
	E3DPARAM_SKY_SKYBOX_STRETCHING,

  EPARAM_SUN_SHAFTS_VISIBILITY,

	E3DPARAM_SKYBOX_MULTIPLIER,

	E3DPARAM_DAY_NIGHT_INDICATOR,

  // Tone mapping tweakables
  E3DPARAM_HDR_FILMCURVE_SHOULDER_SCALE,
  E3DPARAM_HDR_FILMCURVE_LINEAR_SCALE,
  E3DPARAM_HDR_FILMCURVE_TOE_SCALE,
  E3DPARAM_HDR_FILMCURVE_WHITEPOINT,

	E3DPARAM_HDR_EYEADAPTATION_PARAMS,

  E3DPARAM_HDR_COLORGRADING_COLOR_SATURATION,
  E3DPARAM_HDR_COLORGRADING_COLOR_BALANCE,

  E3DPARAM_COLORGRADING_FILTERS_PHOTOFILTER_COLOR,
  E3DPARAM_COLORGRADING_FILTERS_PHOTOFILTER_DENSITY,
  E3DPARAM_COLORGRADING_FILTERS_GRAIN
};

enum EShadowMode
{
	ESM_NORMAL = 0,
	ESM_HIGHQUALITY
};


//////////////////////////////////////////////////////////////////////////
// Description:
//     This structure is filled and passed by the caller to the DebugDraw functions of the stat object or entity.
struct SGeometryDebugDrawInfo
{
	Matrix34 tm;        // Transformation Matrix
	ColorB   color;     // Optional color of the lines.
	ColorB   lineColor; // Optional color of the lines.
	
	// Optional flags controlling how to render debug draw information.
	uint32   bNoCull  : 1;
	uint32   bNoLines : 1;
	uint32   bExtrude : 1; // Extrude debug draw geometry alittle bit so it is over real geometry.

	SGeometryDebugDrawInfo() : color(255,0,255,255),lineColor(255,255,0,255),bNoLines(0),bNoCull(0) { tm.SetIdentity(); }
};

// Summary:
//     Physics material enumerator, allows for 3dengine to get material id from game code.
struct IPhysMaterialEnumerator
{
	// <interfuscator:shuffle>
	virtual ~IPhysMaterialEnumerator(){}
  virtual int EnumPhysMaterial(const char * szPhysMatName) = 0;
	virtual bool IsCollidable(int nMatId) = 0;
	virtual int	GetMaterialCount() = 0;
	virtual const char* GetMaterialNameByIndex( int index ) = 0;
	// </interfuscator:shuffle>
};


// Summary:
//	   Physics foreign data flags.
enum EPhysForeignFlags
{
	PFF_HIDABLE                = 0x1,
	PFF_HIDABLE_SECONDARY      = 0x2,
	PFF_EXCLUDE_FROM_STATIC    = 0x4,
	PFF_BRUSH                  = 0x8,
	PFF_VEGETATION             = 0x10,
	PFF_UNIMPORTANT            = 0x20,
	PFF_OUTDOOR_AREA           = 0x40,
	PFF_MOVING_PLATFORM        = 0x80,
};

// Summary:
//	   Ocean data flags.
enum EOceanRenderFlags
{
  OCR_NO_DRAW     =   1<<0,
  OCR_OCEANVOLUME_VISIBLE  =   1<<1,
};

// Summary:
//		Structure to pass vegetation group properties.
UNIQUE_IFACE struct IStatInstGroup
{
	enum EPlayerHideable
	{
		ePlayerHideable_None = 0,
		ePlayerHideable_High,
		ePlayerHideable_Mid,
		ePlayerHideable_Low,

		ePlayerHideable_COUNT,
	};

	IStatInstGroup() 
	{ 
		pStatObj = 0;
		szFileName[0]=0;
		bHideability = 0;
		bHideabilitySecondary = 0;

    bPickable = 0;
		fBending = 0;
		nCastShadowMinSpec = 0;
		bRecvShadow = 0;
		bPrecShadow = true;
		bUseAlphaBlending = 0;
		fSpriteDistRatio = 1.f;
		fShadowDistRatio = 1.f;
		fMaxViewDistRatio = 1.f;
    fLodDistRatio = 1.f;
		fBrightness = 1.f;
		pMaterial = 0;
    bUseSprites = true;
	
		fDensity=1;
		fElevationMax=4096;
		fElevationMin=8;
		fSize=1 ;
		fSizeVar=0 ;
		fSlopeMax=255 ;
		fSlopeMin=0 ;
		fStiffness = 0.5f;
		fDamping = 2.5f;
		fVariance = 0.6f; 
		fAirResistance = 1.f;
		bRandomRotation = false;
    nMaterialLayers = 0;
		bAllowIndoor = false;
		bUseTerrainColor = false;
		bAlignToTerrain = false; 
		bAutoMerged = false;
		minConfigSpec = (ESystemConfigSpec)0;
		nTexturesAreStreamedIn = 0;
		nPlayerHideable = ePlayerHideable_None;
	}

	_smart_ptr<IStatObj> pStatObj;
	char  szFileName[256];
	bool	bHideability;
	bool	bHideabilitySecondary;
  bool  bPickable;
	float fBending;
	//bool	bCastShadow;
	uint8	nCastShadowMinSpec;
	bool	bRecvShadow;
	bool	bPrecShadow;
	bool	bUseAlphaBlending;
	float fSpriteDistRatio;
  float fLodDistRatio;
	float fShadowDistRatio;
	float fMaxViewDistRatio;
	float	fBrightness;
  bool  bUseSprites;
	bool  bRandomRotation;
	bool  bAlignToTerrain;
  bool  bUseTerrainColor;
	bool  bAllowIndoor;
	bool  bAutoMerged; 

	float fDensity;
	float fElevationMax;
	float fElevationMin;
	float fSize;
	float fSizeVar;
	float fSlopeMax;
	float fSlopeMin;
	float fStiffness;
	float fDamping;
	float fVariance; 
	float fAirResistance; 

  float fVegRadius;
  float fVegRadiusVert;
  float fVegRadiusHor;

  int nPlayerHideable;

	// Minimal configuration spec for this vegetation group.
	ESystemConfigSpec minConfigSpec;

	// Override material for this instance group.
	_smart_ptr<IMaterial> pMaterial;

  // Material layers bitmask -> which layers are active.
  uint8 nMaterialLayers;

	// Textures Are Streamed In.
	uint8 nTexturesAreStreamedIn;

	// Flags similar to entity render flags.
	int m_dwRndFlags;
};

// Description:
//     Water volumes should usually be created by I3DEngine::CreateWaterVolume.
// Summary:
//     Interface to water volumes.
struct IWaterVolume
{
	// <interfuscator:shuffle>
//DOC-IGNORE-BEGIN
	virtual ~IWaterVolume(){}
	virtual void UpdatePoints(const Vec3 * pPoints, int nCount, float fHeight) = 0;
	virtual void SetFlowSpeed(float fSpeed) = 0;
	virtual void SetAffectToVolFog(bool bAffectToVolFog) = 0;
	virtual void SetTriSizeLimits(float fTriMinSize, float fTriMaxSize) = 0;
//	virtual void SetMaterial(const char * szShaderName) = 0;
	virtual void SetMaterial( IMaterial *pMaterial ) = 0;
	virtual IMaterial * GetMaterial() = 0;
	virtual const char* GetName() const = 0;
	virtual void SetName(const char * szName) = 0;
//DOC-IGNORE-END

	// Description:
	//     Used to change the water level. Will assign a new Z value to all 
	//     vertices of the water geometry.
	// Arguments:
	//     vNewOffset - Position of the new water level
	// Summary:
	//     Sets a new water level.
	virtual void SetPositionOffset(const Vec3 & vNewOffset) = 0;
	// </interfuscator:shuffle>
};

// Summary:
//     Provides information about the different VisArea volumes.
UNIQUE_IFACE struct IVisArea
{
	// <interfuscator:shuffle>
	virtual ~IVisArea(){}
	// Summary:
	//     Gets the last rendered frame id.
	// Return Value:
	//     An int which contains the frame id.
	virtual int GetVisFrameId() = 0;
	
	// Description:
	//     Gets a list of all the VisAreas which are connected to the current one. 
	// Arguments:
	//     pAreas               - Pointer to an array of IVisArea*
	//     nMaxConnNum          - The maximum of IVisArea to write in pAreas
	//     bSkipDisabledPortals - Ignore portals which are disabled
	// Return Value:
	//     An integer which hold the amount of VisArea found to be connected. If 
	//     the return is equal to nMaxConnNum, it's possible that not all 
	//     connected VisAreas were returned due to the restriction imposed by the 
	//     argument.
	// Summary:
	//     Gets all the areas which are connected to the current one.
	virtual	int GetVisAreaConnections(IVisArea ** pAreas, int nMaxConnNum, bool bSkipDisabledPortals = false) = 0;

	// Summary:
	//     Determines if it's connected to an outdoor area.
	// Return Value:
	//     True if the VisArea is connected to an outdoor area.
	virtual bool IsConnectedToOutdoor() = 0;

	// Summary:
	//     Determines if the visarea ignores Global Illumination inside.
	// Return Value:
	//     True if the VisArea ignores Global Illumination inside.
	virtual bool IsIgnoringGI() const = 0;

	// Summary:
	//     Gets the name.
	// Notes:
	//     The name is always returned in lower case.
	// Return Value:
	//     A null terminated char array containing the name of the VisArea.
	virtual const char * GetName() = 0;

	// Summary:
	//     Determines if this VisArea is a portal.
	// Return Value:
	//     True if the VisArea is a portal, or false in the opposite case.
	virtual bool IsPortal() = 0;

	// Description: 
	//     Searches for a specified VisArea to see if it's connected to the current 
	//     VisArea.
	// Arguments:
	//     pAnotherArea         - A specified VisArea to find
	//     nMaxRecursion        - The maximum number of recursion to do while searching
	//     bSkipDisabledPortals - Will avoid searching disabled VisAreas
	//		 pVisitedAreas				- if not NULL - will get list of all visited areas
	// Return Value:
	//     True if the VisArea was found.
	// Summary:
	//     Searches for a specified VisArea.
	virtual bool FindVisArea(IVisArea * pAnotherArea, int nMaxRecursion, bool bSkipDisabledPortals) = 0;

	// Description: 
	//     Searches for the surrounding VisAreas which connected to the current 
	//     VisArea.
	// Arguments:
	//     nMaxRecursion        - The maximum number of recursion to do while searching
	//     bSkipDisabledPortals - Will avoid searching disabled VisAreas
	//	   pVisitedAreas		- if not NULL - will get list of all visited areas
	// Return Value:
	//     None.
	// Summary:
	//     Searches for the surrounding VisAreas.
	virtual void FindSurroundingVisArea( int nMaxRecursion, bool bSkipDisabledPortals, PodArray<IVisArea*> * pVisitedAreas = NULL, int nMaxVisitedAreas = 0, int nDeepness = 0) = 0;

	// Summary:
	//     Determines if it's affected by outdoor lighting.
	// Return Value:
	//     Returns true if the VisArea if it's affected by outdoor lighting, else
	//     false will be returned.
	virtual bool IsAffectedByOutLights() = 0;

	// Summary:
	//     Determines if the spere can be affect the VisArea.
	// Return Value:
	//     Returns true if the VisArea can be affected by the sphere, else
	//     false will be returned.
	virtual bool IsSphereInsideVisArea(const Vec3 & vPos, const f32 fRadius) = 0;

	// Summary:
	//     Clips geometry inside or outside a vis area.
	// Return Value:
	//     Whether geom was clipped.
	virtual bool ClipToVisArea(bool bInside, Sphere& sphere, Vec3 const& vNormal) = 0;
	
	// Summary:
	//     Gives back the axis aligned bounding box of VisArea.
	// Return Value:
	//     Returns the pointer of a AABB.
	virtual const AABB* GetAABBox() const = 0;

	// Summary:
	//     Gives back the axis aligned bounding box of all static objects in the VisArea.
	//     This AABB can be huger than the ViaArea AABB as some objects might not be completely inside the VisArea.
	// Return Value:
	//     Returns the pointer to the AABB.
	virtual const AABB* GetStaticObjectAABBox() const = 0;

	// Summary:
	//     Determines if the point can be affect the VisArea.
	// Return Value:
	//     Returns true if the VisArea can be affected by the point, else
	//     false will be returned.
	virtual bool IsPointInsideVisArea(const Vec3 & vPos) = 0;

  // Description: 
  //     Return vis area final ambient color (ambient color depends on factors, like if connected to outdoor, is affected by skycolor - etc)
  // Arguments:
  //     none
  // Return Value:
  //     none
  virtual const Vec3 GetFinalAmbientColor() = 0;

	virtual void GetShapePoints(const Vec3 *&pPoints, size_t &nPoints) = 0;
	virtual float GetHeight() = 0;
	// </interfuscator:shuffle>
};


// Water level unknown.
#define WATER_LEVEL_UNKNOWN -1000000.f
#define BOTTOM_LEVEL_UNKNOWN -1000000.f


// float m_SortId		: offseted by +WATER_LEVEL_SORTID_OFFSET if the camera object line is crossing the water surface
// : otherwise offseted by -WATER_LEVEL_SORTID_OFFSET
#define WATER_LEVEL_SORTID_OFFSET			10000000

#ifdef SEG_WORLD
#define DEFAULT_SID -1
#else
#define DEFAULT_SID 0
#endif

// Summary:
//     indirect lighting quadtree definition.
namespace NQT
{
	// Forward declaration
	template <class TLeafContent, uint32 TMaxCellElems, class TPosType, class TIndexType, bool TUseRadius>
	class CQuadTree;
}

#define FILEVERSION_TERRAIN_SHLIGHTING_FILE 5

enum EVoxelBrushShape
{
	evbsSphere = 1,
	evbsBox,
};

enum EVoxelEditTarget
{
	evetVoxelObjects = 1,
};

enum EVoxelEditOperation
{
  eveoNone=0,
  eveoPaintHeightPos,
  eveoPaintHeightNeg,
	eveoCreate,
	eveoSubstract,
	eveoMaterial,
  eveoBaseColor,
	eveoBlurPos,
  eveoBlurNeg,
	eveoCopyTerrainPos,
  eveoCopyTerrainNeg,
  eveoPickHeight,
  eveoIntegrateMeshPos,
  eveoIntegrateMeshNeg,
  eveoForceDepth,
  eveoLimitLod,
  eveoLast,
};

#define COMPILED_HEIGHT_MAP_FILE_NAME          "terrain\\terrain.dat"
#define COMPILED_VISAREA_MAP_FILE_NAME         "terrain\\indoor.dat"
#define COMPILED_TERRAIN_TEXTURE_FILE_NAME     "terrain\\cover.ctc"
#define COMPILED_VOX_MAP_FILE_NAME             "terrain\\voxmap.dat"
#define COMPILED_MERGED_MESHES_BASE_NAME       "terrain\\merged_meshes_sectors\\"
#define COMPILED_MERGED_MESHES_LIST            "mmrm_used_meshes.lst"
#define LEVEL_INFO_FILE_NAME                   "levelinfo.xml"

//////////////////////////////////////////////////////////////////////////

#pragma pack(push,4)

struct STerrainInfo
{
	int nHeightMapSize_InUnits;
	int nUnitSize_InMeters;
	int nSectorSize_InMeters;

	int nSectorsTableSize_InSectors;
	float fHeightmapZRatio;
	float fOceanWaterLevel;

	AUTO_STRUCT_INFO
};

#define TERRAIN_CHUNK_VERSION 25
#define VISAREAMANAGER_CHUNK_VERSION 6

#define SERIALIZATION_FLAG_BIG_ENDIAN 1
#define SERIALIZATION_FLAG_SECTOR_PALETTES 2

#define TCH_FLAG2_AREA_ACTIVATION_IN_USE 1

struct STerrainChunkHeader
{
  int8 nVersion;
  int8 nDummy;
  int8 nFlags;
  int8 nFlags2;
	int32 nChunkSize;
	STerrainInfo TerrainInfo;

	AUTO_STRUCT_INFO
};

struct SVisAreaManChunkHeader
{
  int8 nVersion;
  int8 nDummy;
  int8 nFlags;
  int8 nFlags2;
  int nChunkSize;
	int nVisAreasNum;
	int nPortalsNum;
	int nOcclAreasNum;

	AUTO_STRUCT_INFO
};

struct SOcTreeNodeChunk
{
  int16	nChunkVersion;
  int16 ucChildsMask;
  AABB	nodeBox;
  int32 nObjectsBlockSize;

  AUTO_STRUCT_INFO
};

struct IGetLayerIdAtCallback
{
	// <interfuscator:shuffle>
	virtual ~IGetLayerIdAtCallback(){}
  virtual uint32 GetLayerIdAtPosition( const int x, const int y ) const = 0;
  virtual uint32 GetSurfaceTypeIdAtPosition( const int x, const int y ) const = 0;
  virtual bool GetHoleAtPosition( const int x, const int y ) const = 0;
  virtual ColorB GetColorAtPosition( const float x, const float y, bool bBilinear ) = 0;
  virtual float GetElevationAtPosition( const float x, const float y ) = 0;
  virtual float GetRGBMultiplier( ) = 0;
	// </interfuscator:shuffle>
};

//	Summary:
//     Interface to terrain engine
struct IVoxTerrain
{
	// <interfuscator:shuffle>
	virtual ~IVoxTerrain(){}
	virtual bool SetCompiledData(byte * pData, int nDataSize, bool bUpdateMesh, EEndian eEndian, AABB * pAreaBox, int nSID=0) = 0;
	virtual IMemoryBlock * GetCompiledData(bool bSaveMesh, EEndian eEndian, bool bSaveForEditing, AABB * pAreaBox, int nSID=0) = 0;
	virtual void DrawEditingHelper(const Sphere & sp, EVoxelEditOperation eOperation, IMaterial * pHelperMat) = 0;
	virtual void OnMouse(bool bUp) = 0;
	virtual PodArray<IRenderNode*> * GetNodesForUpdate() = 0;
	virtual void PaintLayerId( const float fpx, const float fpy, const float radius, const float hardness, const uint32 dwLayerId ) = 0;
	virtual void SetLayerData(void * pData, int nDataSize) = 0;
	virtual void GetLayerData(void ** pData, int & nDataSize) = 0;
	virtual void SetTextureArea(Vec3 * pPoints, int nPointsCount, int nShapePartId) = 0;
	// </interfuscator:shuffle>
};

#define TERRAIN_DEFORMATION_MAX_DEPTH 3.f

struct SHotUpdateInfo
{
  SHotUpdateInfo()
  {
    nHeigtmap = 1;
    nObjTypeMask = ~0;
	pVisibleLayerMask = NULL;
	pLayerIdTranslation = NULL;
    areaBox.Reset();
  }

  uint32 nHeigtmap;
  uint32 nObjTypeMask;
  const uint8* pVisibleLayerMask;
  const uint16* pLayerIdTranslation;
  AABB areaBox;

  AUTO_STRUCT_INFO
};

UNIQUE_IFACE struct ITerrain
{
  struct SExportInfo
  {
    SExportInfo()
    {
      bHeigtmap = bObjects = true;
      areaBox.Reset();
    }
    bool bHeigtmap;
    bool bObjects;
    AABB areaBox;
  };

	// <interfuscator:shuffle>
	virtual ~ITerrain(){}
   // Summary:
	//	 Loads data into terrain engine from memory block.
	virtual bool SetCompiledData(byte * pData, int nDataSize, std::vector<struct IStatObj*> ** ppStatObjTable, std::vector<IMaterial*> ** ppMatTable, bool bHotUpdate = false, SHotUpdateInfo * pExportInfo = NULL, int nSID = 0, Vec3 vSegmentOrigin = Vec3(0,0,0)) = 0;

	// Summary: 
	//   Executes one step of streaming the compiled data in pData.
	//   Returns false when the streaming is complete, true if there is still work to do.
	//   You should call this until it returns true.
	virtual bool StreamCompiledData(byte* pData, int nDataSize, int nSID, const Vec3 &vSegmentOrigin) = 0;
	virtual void CancelStreamCompiledData(int nSID) = 0;

	// Summary: 
	//	 Saves data from terrain engine into memory block.
	virtual bool GetCompiledData(byte * pData, int nDataSize, std::vector<struct IStatObj*> ** ppStatObjTable, std::vector<IMaterial*> ** ppMatTable, std::vector<struct IStatInstGroup*> ** ppStatInstGroupTable, EEndian eEndian, SHotUpdateInfo * pExportInfo = NULL, int nSID = 0, const Vec3 &segmentOffset = Vec3(0, 0, 0)) = 0;

	// Summary:
	//	 Returns terrain data memory block size.
	virtual int GetCompiledDataSize(SHotUpdateInfo * pExportInfo = NULL, int nSID = 0) = 0;

	//virtual bool LoadTables(byte * & f, int & nDataSize, std::vector<struct IStatObj*> *& pStatObjTable, std::vector<IMaterial*> *& pMatTable, bool bHotUpdate, bool bSW, EEndian eEndian) = 0;
	virtual int  GetTablesSize (SHotUpdateInfo * pExportInfo, int nSID) = 0;
	virtual void SaveTables(byte *& pData, int & nDataSize, std::vector<struct IStatObj*> *& pStatObjTable, std::vector<IMaterial*> *& pMatTable, std::vector<struct IStatInstGroup*> *& pStatInstGroupTable, EEndian eEndian, SHotUpdateInfo * pExportInfo, int nSID) = 0;
	virtual void GetTables(std::vector<struct IStatObj*> *& pStatObjTable, std::vector<IMaterial*> *& pMatTable, std::vector<struct IStatInstGroup*> *& pStatInstGroupTable, int nSID) = 0;
	virtual void ReleaseTables(std::vector<struct IStatObj*> *& pStatObjTable, std::vector<IMaterial*> *& pMatTable, std::vector<struct IStatInstGroup*> *& pStatInstGroupTable) = 0;
	
	// Summary:
	//	 Creates and place a new vegetation object on the terrain.
	virtual IRenderNode* AddVegetationInstance( int nStaticGroupID,const Vec3 &vPos,const float fScale,uint8 ucBright,uint8 angle, uint8 angleX=0, uint8 angleY=0, int nSID=DEFAULT_SID ) = 0;

	// Summary:
	//	 Sets ocean level.
	virtual void SetOceanWaterLevel( float fOceanWaterLevel ) = 0;

	// Summary:
	//	 Returns whole terrain lightmap texture id.
	virtual int GetTerrainLightmapTexId( Vec4 & vTexGenInfo, int nSID = 0 ) = 0;

	// Summary:
	//	 Updates part of height map.
	// Notes:
	//	 x1, y1, nSizeX, nSizeY are in terrain units
	//   pTerrainBlock points to a square 2D array with dimensions GetTerrainSize()
	//   by default update only elevation.
	virtual void SetTerrainElevation(int x1, int y1, int nSizeX, int nSizeY, float * pTerrainBlock, uint8 * pSurfaceData, int nSurfOrgX, int nSurfOrgY, int nSurfSizeX, int nSurfSizeY, uint32 * pResolMap, int nResolMapSizeX, int nResolMapSizeY, int nSID=DEFAULT_SID) = 0;

	// Summary:
	//	 Checks if it is possible to paint on the terrain with a given surface type ID.
	// Notes:
	//	 Should be called by the editor to avoid overflowing the sector surface type palettes.
	virtual bool CanPaintSurfaceType(int x, int y, int r, uint16 usGlobalSurfaceType) = 0;

	// Summary:
	//	 Returns current amount of terrain textures requests for streaming, if more than 0 = there is streaming in progress.
	virtual int GetNotReadyTextureNodesCount() = 0;

	// Summary:
	//	 Retrieves the resource (mostly texture system memory) memory usage
	//   for a given region of the terrain.
	// Arguments:
	//	pSizer -    Is a pointer to an instance of the CrySizer object. The 
	//				      purpose of this object is making sure each element is
	//              accounted only once.
	//  crstAABB -  Is a reference to the bounding box in which region we
	//							want to analyze the resources.
	virtual void GetResourceMemoryUsage(ICrySizer*	pSizer,const AABB&	crstAABB, int nSID=0)=0;

	// Summary:
  	//	 Return number of used detail texture materials
	//   Fills materials array if materials!=NULL
	virtual int GetDetailTextureMaterials(IMaterial* materials[], int nSID=0) = 0;


	// Description:
	// Deallocate segment data in the deleted array
	virtual void ReleaseInactiveSegments() = 0;

  // Description:
  //   Allocate new world segment
  //   Returns handle of newly created segment (usually it is just id of segment in the list of currently loaded segments)
  virtual int CreateSegment(Vec3 vSegmentSize, Vec3 vSegmentOrigin = Vec3(0, 0, 0), const char *pcPath = 0) = 0;

	// Description:
	//   Changes the segment file path
	//   Returns true if specified segment exist and path was successfully updated
	virtual bool SetSegmentPath(int nSID, const char *pcPath) = 0;

  // Description:
	//   Returns a pointer to the segment file path
	virtual const char* GetSegmentPath(int nSID) = 0;

  // Description:
  //   Set new origin for existing world segment
  //   Returns true if specified segment exist and origin was successfully updated
  virtual bool SetSegmentOrigin(int nSID, Vec3 vSegmentOrigin, bool callOffsetPosition = true) = 0;

  // Description:
  //   Returns the segment origin of the given segment id.
  virtual Vec3 GetSegmentOrigin(int nSID) = 0;
  // Description:
  //   Get origin for existing world segment, really.
  //   Returns Vec3 with position or NaNs if segment ID is invalid
  virtual const Vec3 &GetSegmentOrigin(int nSID) const = 0;

  // Description:
  //   Set new origin for existing world segment
  //   Returns true if specified segment was found and successfully deleted
  virtual bool DeleteSegment(int nSID, bool bDeleteNow) = 0;

	// Description:
	//   Find (first) world segment containing given point (in local world coordinates)
	//   Returns id of the found segment or -1 if not found
	virtual int FindSegment(Vec3 vPt) = 0;

	// Description:
	//   Find (first) world segment containing given point (in heightmap coordinates)
	//   Returns id of the found segment or -1 if not found
	virtual int FindSegment(int x, int y) = 0;

	// Description:
	//   Returns a number bigger than the last valid segment ID
	//   to be used in loops like: for (int nSID = 0; nSID < GetMaxSegmentsCount(); ++nSID)
	virtual int GetMaxSegmentsCount() const = 0;

	// Description:
	//   fills bbox with the bounding box of the specified segment (nSID)
	//   Returns true if succeeded, false if nSID is not valid segment id
	virtual bool GetSegmentBounds(int nSID, AABB &bbox) = 0;

	// Description:
	//   if nSID < 0 finds segment containing vPt (in local world coordinates) and
	//     adjusts vPt to be relative to segment's origin
	//   if nSID >= 0 does nothing
	//   Returns id of the found segment or -1 if not found
	virtual int WorldToSegment(Vec3 &vPt, int nSID = DEFAULT_SID) = 0;

	// Description:
	//   if nSID < 0 finds segment containing given point
	//   on input, (x << nBitShift, y << nBitShift) represents a point in local world coordinates
	//   on output, x and y are adjusted to be relative to found segment
	//   if nSID >= 0 does nothing
	//   Returns id of the found segment or -1 if not found
	virtual int WorldToSegment(int &x, int &y, int nBitShift, int nSID = DEFAULT_SID) = 0;
};

// callbacks interface for higher level segments management
struct ISegmentsManager {
	enum ESegmentLoadFlags
	{
		slfTerrain = BIT(1),
		slfVisArea = BIT(2),
		slfEntity = BIT(3),
		slfNavigation = BIT(4),

		slfAll = slfTerrain | slfVisArea | slfEntity | slfNavigation,
	};
	virtual ~ISegmentsManager(){}
	virtual void WorldVecToGlobalSegVec(const Vec3 &inPos, Vec3 &outPos, Vec2 &outAbsCoords) = 0;
	virtual void GlobalSegVecToLocalSegVec(const Vec3 &inPos, const Vec2 &inAbsCoords, Vec3 &outPos) = 0;
	virtual Vec3 WorldVecToLocalSegVec(const Vec3 &inPos) = 0;
	virtual Vec3 LocalToAbsolutePosition( Vec3 const& vPos, f32 fDir = 1.f) const =0;
	virtual void GetTerrainSizeInMeters(int &x, int &y) = 0;
	virtual bool CreateSegments(ITerrain *pTerrain) = 0;
	virtual bool DeleteSegments(ITerrain *pTerrain) = 0;
	virtual bool FindSegment(ITerrain *pTerrain, const Vec3 &pt, int &nSID) = 0;
	virtual bool FindSegmentCoordByID(int nSID, int &x, int &y) = 0;
	virtual void ForceLoadSegments(unsigned int flags) = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IVisAreaCallback
{
	// <interfuscator:shuffle>
	virtual ~IVisAreaCallback(){}
	virtual void OnVisAreaDeleted( IVisArea* pVisArea ) = 0;
	// </interfuscator:shuffle>
};


UNIQUE_IFACE struct IVisAreaManager
{
	// <interfuscator:shuffle>
	virtual ~IVisAreaManager(){}
	// Summary:
	//	 Loads data into VisAreaManager engine from memory block.
  virtual bool SetCompiledData(uint8 * pData, int nDataSize, std::vector<struct IStatObj*> ** ppStatObjTable, std::vector<IMaterial*> ** ppMatTable, bool bHotUpdate, SHotUpdateInfo * pExportInfo, const Vec3 &vSegmentOrigin = Vec3(0, 0, 0)) = 0;
	
	// Summary:
	//	 Saves data from VisAreaManager engine into memory block.
	virtual bool GetCompiledData(uint8 * pData, int nDataSize, std::vector<struct IStatObj*> ** ppStatObjTable, std::vector<IMaterial*> ** ppMatTable, std::vector<struct IStatInstGroup*> ** ppStatInstGroupTable, EEndian eEndian, SHotUpdateInfo * pExportInfo = NULL, const Vec3 &segment = Vec3(0, 0, 0)) = 0;

	// Summary: 
	//	 Returns VisAreaManager data memory block size.
	virtual int GetCompiledDataSize(SHotUpdateInfo * pExportInfo = NULL) = 0;

	// Summary:
	//	 Returns the accumulated number of visareas and portals.
	virtual int GetNumberOfVisArea() const = 0;

	// Summary:
	//	 Returns the visarea interface based on the id (0..GetNumberOfVisArea()) it can be a visarea or a portal.
	virtual IVisArea* GetVisAreaById( int nID ) const = 0;

	virtual void AddListener( IVisAreaCallback *pListener ) = 0;
	virtual void RemoveListener( IVisAreaCallback *pListener ) = 0;

	virtual void PrepareSegmentData(const AABB &box) = 0;
	virtual void ReleaseInactiveSegments() = 0;
	virtual bool CreateSegment(int nSID) = 0;
	virtual bool DeleteSegment(int nSID, bool bDeleteNow) = 0;
	virtual bool StreamCompiledData(uint8 * pData, int nDataSize, int nSID, std::vector<struct IStatObj*> * pStatObjTable, std::vector<IMaterial*> * pMatTable, std::vector<struct IStatInstGroup*> * pStatInstGroupTable, const Vec3 &vSegmentOrigin, const Vec2 &vIndexOffset) = 0;
	virtual void OffsetPosition(const Vec3& delta) = 0;
	virtual void UpdateConnections() = 0;
	// </interfuscator:shuffle>
};

// Summary:
//	 Manages simple pre-merged mesh instances into pre-baked sectors
// 
struct IMergedMeshesManager
{
	struct SInstanceSector 
	{
		DynArray<uint8> data;   // memory stream of internally compiled data 
		string id;							// unique identifier string identifing this sector
	};

	// <interfuscator:shuffle>
	virtual ~IMergedMeshesManager() {}

	///////////////////////////////////////////////////////////////////////////////////////
	// Compilation
	// Summary:
	//	 Returns the compiled instance sectors as a null-terminated pointer array. Returns false on error.
	// Note:
	//		The caller is responsible for freeing the allocated memory (both the array and the sectors)
	virtual bool CompileSectors(DynArray<SInstanceSector>& pSectors, std::vector<struct IStatInstGroup*> *pVegGroupTable) = 0;

	///////////////////////////////////////////////////////////////////////////////////////
	// Compilation
	// Summary:
	//	 Returns the list of merged mesh geometry currently active. Returns false on error.
	virtual bool GetUsedMeshes(DynArray<string>& pMeshNames) = 0;

	///////////////////////////////////////////////////////////////////////////////////////
	// Statistics
	// Summary:
	//	 Returns the current memory footprint in vram (accumulated vertex and indexbuffer size in bytes)
	virtual size_t CurrentSizeInVram() const = 0; 

	// Summary:
	//	 Returns the current memory footprint in main memory (the accumulated footprint of all merged instances)
	virtual size_t CurrentSizeInMainMem() const = 0;

	// Summary:
	//	 Returns the memory footprint of the prebaked geometry in bytes
	virtual size_t GeomSizeInMainMem() const = 0;

	// Summary:
	//	 Returns the size of the instance map in bytes
	virtual size_t InstanceSize() const = 0;

	// Summary:
	//	 Returns the size of animated instances if they have spines
	virtual size_t SpineSize() const = 0;

	// Summary:
	//	 The instance count 
	virtual size_t InstanceCount() const = 0; 

	// Summary:
	//	 The number of visible instances last frame
	virtual size_t VisibleInstances() const = 0;
	// </interfuscator:shuffle>

	virtual void PrepareSegmentData(const AABB &aabb) = 0;

	virtual int GetSegmentNodeCount() = 0;

	virtual int GetCompiledDataSize(uint32 index) = 0;

	virtual bool GetCompiledData(uint32 index, byte * pData, int nSize, string *pName, std::vector<struct IStatInstGroup*> ** ppStatInstGroupTable, const Vec3 &segmentOffset) = 0;
};


struct ITimeOfDayUpdateCallback;

// Summary:
//	 Interface to the Time Of Day functionality.
UNIQUE_IFACE struct ITimeOfDay
{
	enum ETimeOfDayParamID
	{
		PARAM_HDR_DYNAMIC_POWER_FACTOR,
		PARAM_LIGHTS_HDR_DYNAMIC_POWER_FACTOR,

		PARAM_TERRAIN_OCCL_MULTIPLIER,
		PARAM_SSAO_MULTIPLIER,
		PARAM_SSAO_CONTRAST_MULTIPLIER,
		PARAM_GI_MULTIPLIER,

		PARAM_SUN_COLOR,
		PARAM_SUN_COLOR_MULTIPLIER,
		PARAM_SUN_SPECULAR_MULTIPLIER,

		PARAM_SKY_COLOR,
		PARAM_SKY_COLOR_MULTIPLIER,

		PARAM_AMBIENT_GROUND_COLOR,
		PARAM_AMBIENT_GROUND_COLOR_MULTIPLIER,

		PARAM_AMBIENT_MIN_HEIGHT,
		PARAM_AMBIENT_MAX_HEIGHT,

		PARAM_FOG_COLOR,
		PARAM_FOG_COLOR_MULTIPLIER,
		PARAM_VOLFOG_HEIGHT,
		PARAM_VOLFOG_DENSITY,
		PARAM_FOG_COLOR2,
		PARAM_FOG_COLOR2_MULTIPLIER,
		PARAM_VOLFOG_HEIGHT2,
		PARAM_VOLFOG_DENSITY2,
		PARAM_VOLFOG_HEIGHT_OFFSET,

		PARAM_FOG_RADIAL_COLOR,
		PARAM_FOG_RADIAL_COLOR_MULTIPLIER,
		PARAM_VOLFOG_RADIAL_SIZE,
		PARAM_VOLFOG_RADIAL_LOBE,

		PARAM_VOLFOG_FINAL_DENSITY_CLAMP,

		PARAM_VOLFOG_GLOBAL_DENSITY,
		PARAM_VOLFOG_RAMP_START,
		PARAM_VOLFOG_RAMP_END,
		PARAM_VOLFOG_RAMP_INFLUENCE,

		PARAM_VOLFOG_SHADOW_DARKENING,
		PARAM_VOLFOG_SHADOW_DARKENING_SUN,
		PARAM_VOLFOG_SHADOW_DARKENING_AMBIENT,
		PARAM_VOLFOG_SHADOW_RANGE,

		PARAM_SKYLIGHT_SUN_INTENSITY,
		PARAM_SKYLIGHT_SUN_INTENSITY_MULTIPLIER,

		PARAM_SKYLIGHT_KM,
		PARAM_SKYLIGHT_KR,
		PARAM_SKYLIGHT_G,

		PARAM_SKYLIGHT_WAVELENGTH_R,
		PARAM_SKYLIGHT_WAVELENGTH_G,
		PARAM_SKYLIGHT_WAVELENGTH_B,

		PARAM_NIGHSKY_HORIZON_COLOR,
		PARAM_NIGHSKY_HORIZON_COLOR_MULTIPLIER,
		PARAM_NIGHSKY_ZENITH_COLOR,
		PARAM_NIGHSKY_ZENITH_COLOR_MULTIPLIER,
		PARAM_NIGHSKY_ZENITH_SHIFT,

		PARAM_NIGHSKY_START_INTENSITY,

		PARAM_NIGHSKY_MOON_COLOR,
		PARAM_NIGHSKY_MOON_COLOR_MULTIPLIER,
		PARAM_NIGHSKY_MOON_INNERCORONA_COLOR,
		PARAM_NIGHSKY_MOON_INNERCORONA_COLOR_MULTIPLIER,
		PARAM_NIGHSKY_MOON_INNERCORONA_SCALE,
		PARAM_NIGHSKY_MOON_OUTERCORONA_COLOR,
		PARAM_NIGHSKY_MOON_OUTERCORONA_COLOR_MULTIPLIER,
		PARAM_NIGHSKY_MOON_OUTERCORONA_SCALE,

		PARAM_CLOUDSHADING_SUNLIGHT_MULTIPLIER,
		PARAM_CLOUDSHADING_SKYLIGHT_MULTIPLIER,
		PARAM_CLOUDSHADING_SUNLIGHT_CUSTOM_COLOR,
		PARAM_CLOUDSHADING_SUNLIGHT_CUSTOM_COLOR_MULTIPLIER,
		PARAM_CLOUDSHADING_SUNLIGHT_CUSTOM_COLOR_INFLUENCE,
		PARAM_CLOUDSHADING_SKYLIGHT_CUSTOM_COLOR,
		PARAM_CLOUDSHADING_SKYLIGHT_CUSTOM_COLOR_MULTIPLIER,
		PARAM_CLOUDSHADING_SKYLIGHT_CUSTOM_COLOR_INFLUENCE,

		PARAM_SUN_SHAFTS_VISIBILITY,
		PARAM_SUN_RAYS_VISIBILITY,
		PARAM_SUN_RAYS_ATTENUATION,
		PARAM_SUN_RAYS_SUNCOLORINFLUENCE,
		PARAM_SUN_RAYS_CUSTOMCOLOR,

		PARAM_OCEANFOG_COLOR,
		PARAM_OCEANFOG_COLOR_MULTIPLIER,
		PARAM_OCEANFOG_DENSITY,

		PARAM_SKYBOX_MULTIPLIER,

    PARAM_HDR_FILMCURVE_SHOULDER_SCALE,
    PARAM_HDR_FILMCURVE_LINEAR_SCALE,
    PARAM_HDR_FILMCURVE_TOE_SCALE,
    PARAM_HDR_FILMCURVE_WHITEPOINT,

    PARAM_HDR_COLORGRADING_COLOR_SATURATION,
    PARAM_HDR_COLORGRADING_COLOR_BALANCE,

		PARAM_HDR_EYEADAPTATION_SCENEKEY,
		PARAM_HDR_EYEADAPTATION_MIN_EXPOSURE,
		PARAM_HDR_EYEADAPTATION_MAX_EXPOSURE,

		PARAM_COLORGRADING_FILTERS_GRAIN,
		PARAM_COLORGRADING_FILTERS_PHOTOFILTER_COLOR,
		PARAM_COLORGRADING_FILTERS_PHOTOFILTER_DENSITY,

		PARAM_COLORGRADING_DOF_FOCUSRANGE,
		PARAM_COLORGRADING_DOF_BLURAMOUNT,

		PARAM_SHADOWSC0_BIAS,
    PARAM_SHADOWSC0_SLOPE_BIAS,
		PARAM_SHADOWSC1_BIAS,
    PARAM_SHADOWSC1_SLOPE_BIAS,
		PARAM_SHADOWSC2_BIAS,
    PARAM_SHADOWSC2_SLOPE_BIAS,
		PARAM_SHADOWSC3_BIAS,
		PARAM_SHADOWSC3_SLOPE_BIAS,
    PARAM_SHADOWSC4_BIAS,
    PARAM_SHADOWSC4_SLOPE_BIAS,
    PARAM_SHADOWSC5_BIAS,
    PARAM_SHADOWSC5_SLOPE_BIAS,
    PARAM_SHADOWSC6_BIAS,
    PARAM_SHADOWSC6_SLOPE_BIAS,
    PARAM_SHADOWSC7_BIAS,
    PARAM_SHADOWSC7_SLOPE_BIAS,

		PARAM_SHADOW_JITTERING,

		PARAM_TOTAL
	};

	enum EVariableType
	{
		TYPE_FLOAT,
		TYPE_COLOR
	};
	struct SVariableInfo
	{
		const char *name;  // Variable name.
		const char *displayName;  // Variable user readable name.
		const char *group; // Group name.
		int nParamId;
		EVariableType type;
		float fValue[3];    // Value of the variable (3 needed for color type)
		ISplineInterpolator* pInterpolator; // Splines that control variable value
		unsigned int bSelected : 1;
	};
	struct SAdvancedInfo
	{
		float fStartTime;
		float fEndTime;
		float fAnimSpeed;
	};
	struct SEnvironmentInfo
	{
		SEnvironmentInfo() : bSunLinkedToTOD(true), sunRotationLatitude(0), sunRotationLongitude(0){}
		bool bSunLinkedToTOD;
		float sunRotationLatitude;
		float sunRotationLongitude;
	};

	// <interfuscator:shuffle>
	virtual ~ITimeOfDay(){}
	//////////////////////////////////////////////////////////////////////////
	// Access to variables that control time of the day appearance.
	//////////////////////////////////////////////////////////////////////////
	virtual int GetVariableCount() = 0;
	virtual bool GetVariableInfo( int nIndex,SVariableInfo &varInfo ) = 0;
	virtual void SetVariableValue( int nIndex,float fValue[3] ) = 0;

	virtual void ResetVariables()=0;

	// Summary:
	//	 Sets the time of the day specified in hours.
	virtual void SetTime( float fHour,bool bForceUpdate=false ) = 0;
	virtual float GetTime() = 0;

	// Sun position
	virtual void SetSunPos( float longitude, float latitude  ) = 0;
	virtual float GetSunLatitude() = 0;
	virtual float GetSunLongitude() = 0;

	// Summary:
	//	 Updates the current tod.
	virtual void Tick() = 0;


	virtual void SetPaused(bool paused) = 0;

	virtual void SetAdvancedInfo( const SAdvancedInfo &advInfo ) = 0;
	virtual void GetAdvancedInfo( SAdvancedInfo &advInfo ) = 0;

	// Summary:
	//	 Updates engine parameters after variable values have been changed.
	virtual void Update( bool bInterpolate=true,bool bForceUpdate=false ) = 0;
	virtual void SetUpdateCallback(ITimeOfDayUpdateCallback* pCallback) = 0;

	virtual void BeginEditMode() = 0;
	virtual void EndEditMode() = 0;

	virtual void Serialize( XmlNodeRef &node,bool bLoading ) = 0;
	virtual void Serialize( TSerialize ser ) = 0;

	virtual void SetTimer( ITimer * pTimer ) = 0;
	virtual void SetEnvironmentSettings( const SEnvironmentInfo& envInfo ) = 0;

	// MP serialization
	static const int NETSER_FORCESET = BIT(0);
	static const int NETSER_COMPENSATELAG = BIT(1);
	static const int NETSER_STATICPROPS = BIT(2);
	virtual void NetSerialize( TSerialize ser, float lag, uint32 flags ) = 0;

	// LiveCreate
	virtual void SaveInternalState(struct IDataWriteStream& writer) = 0;
	virtual void LoadInternalState(struct IDataReadStream& reader) = 0;
	// </interfuscator:shuffle>
};

struct ITimeOfDayUpdateCallback
{
	// <interfuscator:shuffle>
	virtual ~ITimeOfDayUpdateCallback(){}
	virtual void BeginUpdate() = 0;
	virtual bool GetCustomValue(ITimeOfDay::ETimeOfDayParamID paramID, int dim, float* pValues, float& blendWeight) = 0;
	virtual void EndUpdate() = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IFoliage
{
	// <interfuscator:shuffle>
	virtual void AddRef() = 0;
  virtual void Release() = 0;
	virtual ~IFoliage(){}
	enum EFoliageFlags { FLAG_FROZEN=1 };
	virtual int Serialize(TSerialize ser) = 0;
	virtual void SetFlags(int flags) = 0;
	virtual int GetFlags() = 0;
	virtual IRenderNode* GetIRenderNode() = 0;
	virtual int GetBranchCount() = 0;
	virtual IPhysicalEntity *GetBranchPhysics(int iBranch) = 0;
	virtual SSkinningData* GetSkinningData(const Matrix34& RenderMat34, const SRenderingPassInfo &passInfo) =  0;
	// </interfuscator:shuffle>
};

struct SSkyLightRenderParams
{
	static const int skyDomeTextureWidth = 64;
	static const int skyDomeTextureHeight =32;
	static const int skyDomeTextureSize = 64 * 32;

	static const int skyDomeTextureWidthBy8 = 8;
	static const int skyDomeTextureWidthBy4Log = 4; // = log2(64/4)
	static const int skyDomeTextureHeightBy2Log = 4; // = log2(32/2)

	SSkyLightRenderParams()
	: m_pSkyDomeMesh( 0 )
	, m_pSkyDomeTextureDataMie( 0 )
	, m_pSkyDomeTextureDataRayleigh( 0 )
	, m_skyDomeTexturePitch( 0 )
	, m_skyDomeTextureTimeStamp( -1 )
	, m_partialMieInScatteringConst( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_partialRayleighInScatteringConst( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_sunDirection( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_phaseFunctionConsts( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_hazeColor( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_hazeColorMieNoPremul( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_hazeColorRayleighNoPremul( 0.0f, 0.0f, 0.0f, 0.0f )
	, m_skyColorTop( 0.0f, 0.0f, 0.0f )
	, m_skyColorNorth( 0.0f, 0.0f, 0.0f )
	, m_skyColorEast( 0.0f, 0.0f, 0.0f )
	, m_skyColorSouth( 0.0f, 0.0f, 0.0f )
	, m_skyColorWest( 0.0f, 0.0f, 0.0f )
	{
	}

	// Sky dome mesh
	_smart_ptr<IRenderMesh> m_pSkyDomeMesh;

	// temporarily add padding bytes to prevent fetching Vec4 constants below from wrong offset
	uint32 dummy0;
	uint32 dummy1;

	// Sky dome texture data
	const void* m_pSkyDomeTextureDataMie;
	const void* m_pSkyDomeTextureDataRayleigh;
	size_t m_skyDomeTexturePitch;
	int m_skyDomeTextureTimeStamp;

	int pad;//Enable 16 byte alignment for Vec4s

	// Sky dome shader constants
	Vec4 m_partialMieInScatteringConst;
	Vec4 m_partialRayleighInScatteringConst;
	Vec4 m_sunDirection;
	Vec4 m_phaseFunctionConsts;
	Vec4 m_hazeColor;
	Vec4 m_hazeColorMieNoPremul;
	Vec4 m_hazeColorRayleighNoPremul;

	// Sky hemisphere colors
	Vec3 m_skyColorTop;
	Vec3 m_skyColorNorth;
	Vec3 m_skyColorEast;
	Vec3 m_skyColorSouth;
	Vec3 m_skyColorWest;
};

struct sRAEColdData
{
	Vec4												m_RAEPortalInfos[96];											// It stores all data needed to solve the problem between the portals & indirect lighting.
//	byte												m_OcclLights[MAX_LIGHTS_NUM];
};

struct SVisAreaInfo
{
  float fHeight;
  Vec3 vAmbientColor;
  bool bAffectedByOutLights;
  bool bIgnoreSkyColor;
  bool bSkyOnly;
  float fViewDistRatio;
  bool bDoubleSide;
  bool bUseDeepness;
  bool bUseInIndoors;
	bool bOceanIsVisible;
	bool bIgnoreGI;
};

struct SDebugFPSInfo
{
	SDebugFPSInfo() : fAverageFPS(0.0f), fMaxFPS(0.0f), fMinFPS(0.0f)
	{
	}
	float fAverageFPS;
	float fMinFPS;
	float fMaxFPS;
};

// Summary:
//	 Common scene rain parameters shared across engine and editor
struct SRainParams
{
	SRainParams() 
    : fAmount(0.f), fRadius(0.f), nUpdateFrameID(-1), bIgnoreVisareas(false), bDisableOcclusion(false)
    , matOccTrans(IDENTITY), matOccTransRender(IDENTITY), qRainRotation(IDENTITY), areaAABB(AABB::RESET)
	{
	}

	Matrix44	matOccTrans;				// Transformation matrix for rendering into a new occ map	
	Matrix44	matOccTransRender;	// Transformation matrix for rendering occluded rain using current occ map
	Quat			qRainRotation;			// Quaternion for the scene's rain entity rotation
	AABB			areaAABB;

	Vec3			vWorldPos;
	Vec3			vColor;

	float			fAmount;
	float			fRadius;
	
	float			fFakeGlossiness;
	float			fFakeReflectionAmount;
	
	float			fRainDropsAmount;
	float			fRainDropsSpeed;
	float			fRainDropsLighting;

	float			fMistAmount;
	float			fMistHeight;

	float			fPuddlesAmount;
	float			fPuddlesMaskAmount; 
	float			fPuddlesRippleAmount;
	float			fSplashesAmount;
	
	int				nUpdateFrameID;
	bool			bApplyOcclusion;
	bool			bIgnoreVisareas;
	bool			bDisableOcclusion;
};

UNIQUE_IFACE struct IMaterialMergePipe
{
	enum EMergeMaterialsFlags
	{
		mfDefaut              = 0,
		mfDoNotMergeTextures  = BIT(0),
		mfOnlyUsedSpace       = BIT(1),// Use only used space from source texture
		mfUseDiffuseAtlas     = BIT(2),// Uses diffuse channel atlas to generate output textures (ie resizes all other channels to diffuse sizes)
		mfVerbose             = BIT(3),// Verbose
	};
	// <interfuscator:shuffle>
	virtual ~IMaterialMergePipe(){}
  virtual int GetMaterialsCount() const = 0;
  virtual IMaterial* GetMaterial(int index) const = 0;
  virtual const char* GetTexmapName(int mtlIndex,int mtlSubsetIndex,int texmapIndex) = 0;
  // Used space
  virtual size_t GetTextureCoordinatesSubsets(IMaterial* mtl) = 0;
  virtual const Vec2* GetTextureCoordinates(int subset,IMaterial* mtl,int& firstVertex,int& count) = 0;
  virtual const vtx_idx* GetIndicies(int subset,IMaterial* mtl,int& firstIndex,int& count) = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IMergePipeBackend
{
	// <interfuscator:shuffle>
	virtual ~IMergePipeBackend(){}
  virtual void GetFaceVerticies(int chrIndex,int face,bool bIncludeMorphs,Vec3* vert123) const = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IMergePipe
{
public:
  enum EMergeFlags
  {
    mfDefault         = 0,
    mfPhysics         = BIT(0),
    mfSkin            = BIT(1),
    mfMergeMaterials  = BIT(2),// or use incoming
    mfMergeMorphs     = BIT(3),
    mfVerbose_LOD     = BIT(4),
  };
  enum ELodFlags
  {
    elfNothing =        0,
    elfHandleSplits =   BIT(0),
    elfHandleSkinning = BIT(1),// handle skinning using less accurate method
  };
	struct SMergePhysicsBoneResult
	{
		SMergePhysicsBoneResult()
		{
			okToMerge = false;
			new_name = NULL;
			bOverrideB2W = false;
		}
		bool okToMerge;
		const char* new_name;
		bool bOverrideB2W;
		Matrix34 m_B2W;
	};
	// <interfuscator:shuffle>
	virtual ~IMergePipe(){}
  virtual int GetModelsCount() const = 0;
  virtual const char* GetModelFilename(int index) const = 0;
  virtual IMaterial* GetModelMaterial(int index) const = 0;
  virtual float GetModelLodMaxAngle(int chrindex) const = 0;
	virtual IMergePipe::ELodFlags GetModelLodFlags(int chrindex) const = 0;
  virtual const char* GetModelLogPrefix(int chrindex) const = 0;
  // Summary:
  //	Sets merged material.
  virtual void SetMergedMaterial(IMaterial* pMergedMaterial) = 0;
  virtual const char* GetMaterialMergeTexmapName(int mtlIndex,int mtlSubsetIndex,int texmapIndex) = 0;
  virtual IMaterialMergePipe::EMergeMaterialsFlags GetMaterialMergeFlags() const = 0;
  // Summary:
  //	Returns the name of newly created node.
  virtual const char* GetModelName() const = 0;
  //
	virtual IMergePipe::SMergePhysicsBoneResult GetPhysicsBoneMergeInfo(int chrindex,const char* modelName,const char* boneName,IMergePipeBackend* info) const = 0;
  //
  virtual float GetMorphTargetWeight(int chrIndex,const char* targetName) const = 0;
	// </interfuscator:shuffle>
};

struct IMergeMaterialsResult
{
	// <interfuscator:shuffle>
	virtual ~IMergeMaterialsResult(){}
  virtual void Release() = 0;
	// </interfuscator:shuffle>
};


struct IMergeCHRMorphsPipe
{
	// <interfuscator:shuffle>
	virtual ~IMergeCHRMorphsPipe(){}
  virtual size_t getNumMorphs() const = 0;
  virtual const char* getMorphName(size_t index) const = 0;
  virtual float getMorphWeight(size_t index) const = 0;
	// </interfuscator:shuffle>
};

struct IScreenshotCallback
{
	// <interfuscator:shuffle>
	virtual ~IScreenshotCallback(){}
	virtual void SendParameters(void* data, uint32 width, uint32 height, f32 minx, f32 miny, f32 maxx, f32 maxy) = 0;
	// </interfuscator:shuffle>
};

class IStreamedObjectListener
{
public:
	// <interfuscator:shuffle>
	virtual void OnCreatedStreamedObject(const char* filename, void* pHandle) = 0;
	virtual void OnRequestedStreamedObject(void* pHandle) = 0;
	virtual void OnReceivedStreamedObject(void* pHandle) = 0;
	virtual void OnUnloadedStreamedObject(void* pHandle) = 0;
	virtual void OnBegunUsingStreamedObjects(void** pHandles, size_t numHandles) = 0;
	virtual void OnEndedUsingStreamedObjects(void** pHandles, size_t numHandles) = 0;
	virtual void OnDestroyedStreamedObject(void* pHandle) = 0;
	// </interfuscator:shuffle>
protected:
	virtual ~IStreamedObjectListener() {}
};

// Summary:
//     Light volumes data

#define LIGHTVOLUME_MAXLIGHTS 16

struct SLightVolume
{
	struct SLightData
	{
		SLightData()
		{
		}
		SLightData(const Vec4 &vInPos, const Vec3 &vInColor, uint32 nInProjector):vPos(vInPos), vColor(vInColor), nProjector(nInProjector)
		{			
		}

		Vec4 vPos;
		Vec3 vColor;
		uint32 nProjector;
	};

	SLightVolume()
	{
		pData.reserve(LIGHTVOLUME_MAXLIGHTS);
	}

	typedef DynArray<SLightData> LightDataVector;

	DEFINE_ALIGNED_DATA(LightDataVector, pData, 16);
};

// Summary:
//     Interface to the 3d Engine.
UNIQUE_IFACE struct I3DEngine : public IProcess
{	
	
	DEVIRTUALIZATION_VTABLE_FIX
	
	struct SObjectsStreamingStatus
	{
		int nReady;
		int nInProgress;
		int nTotal;
		int nActive;
		int nAllocatedBytes;
		int nMemRequired;
		int nMeshPoolSize; // in MB
	};

	struct SStremaingBandwidthData
	{
		SStremaingBandwidthData()
		{
			memset(this, 0, sizeof(SStremaingBandwidthData));
		}
		float fBandwidthActual;
		float fBandwidthRequested;
	};

	enum eStreamingSubsystem
	{
		eStreamingSubsystem_Textures,
		eStreamingSubsystem_Objects,
		eStreamingSubsystem_Audio,
	};

	// <interfuscator:shuffle>
	// Summary:
	//     Initializes the 3D Engine.
	// See Also:
	//     ShutDown
	// Notes:
	//     Only call once, after creating the instance.
	virtual bool Init() = 0;

	// Summary:
	//     Sets the path used to load levels.
	// See Also:
	//     LoadLevel
	// Arguments:
	//     szFolderName - Should contains the folder to be used
	virtual void SetLevelPath( const char * szFolderName ) = 0;

	// Description:
	//     Will load a level from the folder specified with SetLevelPath. If a 
	//     level is already loaded, the resources will be deleted before.
	// See Also:
	//     SetLevelPath
	// Arguments:
	//     szFolderName - Name of the subfolder to load
	//     szMissionName - Name of the mission
	// Return Value:
	//     A boolean which indicate the result of the function; true is 
	//     succeed, or false if failed.
	// Summary:
	//     Load a level.
	virtual bool LoadLevel(const char * szFolderName, const char * szMissionName) = 0;
	virtual bool InitLevelForEditor(const char * szFolderName, const char * szMissionName) = 0;

	// Summary:
	//     Handles any work needed at start of new frame.
	// Notes:
	//     Should be called for every frame.
	virtual void OnFrameStart() = 0;

	// Description:
	//    Must be called after the game completely finishes loading the level.
	//    3D engine uses it to pre-cache some resources needed for rendering.
	// See Also:
	//    LoadLevel
	// Summary:
	//	  Pre-caches some resources need for rendering.
	virtual void PostLoadLevel() = 0;

	// Summary:
	//   Clears all rendering resources, all objects, characters and materials, voxels and terrain.
	// Notes:
	//   Should always be called before LoadLevel, and also before loading textures from a script.
	virtual void UnloadLevel()=0;


	// Summary:
	//     Updates the 3D Engine.
	// Notes:
	//     Should be called for every frame.
	virtual void Update() = 0;

	// Summary:
	//    Returns the Camera used for Rendering on 3DEngine Side, normaly equal to the view camera, except if frozen with e_camerafreeze
	// Notes:
	//		Only valid during RenderWorld(else the camera of the last frame is used)
	//		This is the camera which should be used for all Engine side culling (since e_camerafreeze allows easy debugging then)
	virtual const CCamera& GetRenderingCamera() const =0;
	virtual float GetZoomFactor() const = 0;
	virtual float IsZoomInProgress()	const = 0;

	// Summary:
	// clear all per frame temp data used in SRenderingPass
	virtual void Tick() = 0;

	// Summary:
	//	 Activate all shader items, used by editor (else called by C3DEngine::Update())
	virtual void ActivateShaderItems() =0;

	// Summary:
	//     Deletes the 3D Engine instance.
	virtual void Release() = 0;

	// Summary:
	//     Sync all changes to the octree to prevent reading data which is about to be changed
	virtual void SyncOctreeUpdate(IRenderNode* pEnt = NULL) = 0;

	// Summary:
	//     Draws the world.
	// See Also:
	//     SetCamera
	// Arguments:
	//   szDebugName - name that can be visualized for debugging purpose, must not be 0
	virtual void RenderWorld(const int nRenderFlags, const SRenderingPassInfo &passInfo, const char *szDebugName) = 0;

	// Summary:
	//     Shuts down the 3D Engine.
	virtual void ShutDown() = 0;

	// Summary:
	//     Loads a static object from a CGF file.
	// See Also:
	//     IStatObj
	// Arguments:
	//     szFileName - CGF Filename - should not be 0 or ""
	//     szGeomName - Optional name of geometry inside CGF.
	//     ppSubObject - [Out]Optional Out parameter,Pointer to the
	//		nLoadingFlags - Zero or a bitwise combination of the flags from ELoadingFlags,
	//										defined in IMaterial.h, under the interface IMaterialManager.
	// Return Value:
	//     A pointer to an object derived from IStatObj.
	virtual IStatObj* LoadStatObj( const char *szFileName,const char *szGeomName=NULL,/*[Out]*/IStatObj::SSubObject **ppSubObject=NULL, bool bUseStreaming=true,unsigned long nLoadingFlags=0) = 0;

	// Summary:
	//     Finds a static object created from the given filename
	// See Also:
	//     IStatObj
	// Arguments:
	//     szFileName - CGF Filename - should not be 0 or ""
	// Return Value:
	//     A pointer to an object derived from IStatObj.
	virtual IStatObj * FindStatObjectByFilename(const char * filename) = 0;

	virtual void SetCoverageBufferDepthReady() = 0;
	virtual void ResetCoverageBufferSignalVariables() = 0;
	virtual void DrawDeferredCoverageBufferDebug() = 0;

	// Summary:
	//     Gets the amount of loaded objects.
	// Return Value:
	//     An integer representing the amount of loaded objects.
	virtual int GetLoadedObjectCount() { return 0; }

	// Summary:
	//     Fills pObjectsArray with pointers to loaded static objects
	//     if pObjectsArray is NULL only fills nCount parameter with amount of loaded objects.
	virtual void GetLoadedStatObjArray( IStatObj** pObjectsArray,int &nCount ) = 0;

	// Summary:
	//     Gets stats on streamed objects
  virtual void GetObjectsStreamingStatus(SObjectsStreamingStatus &outStatus) = 0;

	// Summary:
	//		Gets stats on the streaming bandwidth requests from subsystems
	// Arguments:
	//		subsystem - the streaming subsystem we want bandwidth data for
	//		outData - structure containing the bandwidth data for the subsystem requested
	virtual void GetStreamingSubsystemData(int subsystem, SStremaingBandwidthData &outData) = 0;

	// Summary:
	//     Registers an entity to be rendered.
	// Arguments:
	//     pEntity - The entity to render
	virtual void RegisterEntity( IRenderNode * pEntity, int nSID=-1, int nSIDConsideredSafe=-1 )=0;

	// Summary:
	//     Selects an entity for debugging.
	// Arguments:
	//     pEntity - The entity to render
	virtual void SelectEntity( IRenderNode * pEntity )=0;

	// Summary:
	//     Notices the 3D Engine to stop rendering a specified entity.
	// Arguments:
	//     pEntity - The entity to stop render
	virtual void UnRegisterEntityDirect( IRenderNode * pEntity )=0;
	virtual void UnRegisterEntityAsJob( IRenderNode* pEnt ) =0;
	// Summary:
	//	   Returns whether a world pos is under water.
	virtual bool IsUnderWater( const Vec3& vPos) const = 0;
  
	// Summary:
	//	   Returns whether ocean volume is visible or not.
	virtual void SetOceanRenderFlags( uint8 nFlags ) = 0;
	virtual uint8 GetOceanRenderFlags() const = 0;
	virtual uint32 GetOceanVisiblePixelsCount() const = 0;

	// Summary:
	//     Gets the closest walkable bottom z straight beneath the given reference position.
	// Notes:
	//     This function will take into account both the global terrain elevation and local voxel (or other solid walkable object).
	// Arguments:
	//     referencePos - Position from where to start searching downwards.
	//     maxRelevantDepth - Max depth caller is interested in relative to referencePos (for ray casting performance reasons).
	//     objtypes - expects physics entity flags.  Use this to specify what object types make a valid bottom for you.
	// Return Value:
	//     A float value which indicate the global world z of the bottom level beneath the referencePos.
	//     If the referencePos is below terrain but not inside any voxel area BOTTOM_LEVEL_UNKNOWN is returned.
	virtual float GetBottomLevel(const Vec3 & referencePos, float maxRelevantDepth, int objtypes) = 0;
	// A set of overloads for enabling users to use different sets of input params.  Basically, only
	// referencePos is mandatory.  The overloads as such don't need to be virtual but this seems to be
	// a purely virtual interface.
	virtual float GetBottomLevel(const Vec3 & referencePos, float maxRelevantDepth = 10.0f) = 0;
	virtual float GetBottomLevel(const Vec3 & referencePos, int objflags) = 0;

	// Summary:
	//     Gets the ocean water level. Fastest option, always prefer is only ocean height required.
	// Notes:
	//     This function will take into account just the global water level.
	// Return Value:
	//     A float value which indicate the water level. In case no water was 
	//     found at the specified location, the value WATER_LEVEL_UNKNOWN will 
	//     be returned.
	virtual float GetWaterLevel() = 0;

	// Summary:
	//     Gets the closest walkable bottom z straight beneath the given reference position.
	//				- Use with caution the accurate query - SLOW
	// Notes:	
	//     This function will take into account both the global water level and any water volume present.
	//     Function is provided twice for performance with diff. arguments.
	// Arguments:
	//     pvPos - Desired position to inspect the water level
	//     pent - Pointer to return the physical entity to test against (optional)
	// Return Value:
	//     A float value which indicate the water level. In case no water was 
	//     found at the specified location, the value WATER_LEVEL_UNKNOWN will 
	//     be returned.
	virtual float GetWaterLevel(const Vec3 * pvPos, IPhysicalEntity * pent = NULL, bool bAccurate = false) = 0;

	// Summary:
	//     Gets the ocean water level for a specified position.
	//				- Use with caution the accurate query - SLOW
	// Notes:
	//     This function only takes into account ocean water.
	// Arguments:
	//     pCurrPos - Position to check water level
	// Return Value:
	//     A float value which indicate the water level.
	virtual float GetAccurateOceanHeight( const Vec3 &pCurrPos ) const = 0;

	// Summary:
	//     Gets caustics parameters.
	// Return Value:
	//     A Vec4 value which constains:
	//     x = unused, y = distance attenuation, z = caustics multiplier, w = caustics darkening multiplier  
	virtual Vec4 GetCausticsParams() const = 0;  

  // Summary:
  //     Gets ocean animation caustics parameters.
  // Return Value:
  //     A Vec4 value which constains:
  //     x = unused, y = height, z = depth, w = intensity
  virtual Vec4 GetOceanAnimationCausticsParams() const = 0;  


	// Summary:
	//     Gets ocean animation parameters.
	// Return Value:
	//     2 Vec4 which constain:
	//     0: x = ocean wind direction, y = wind speed, z = waves speed, w = waves amount  
	//     1: x = waves size, y = free, z = free, w = free  

	virtual void GetOceanAnimationParams(Vec4 &pParams0, Vec4 &pParams1 ) const = 0;  

	// Summary:
	//     Gets HDR setup parameters.
	// Return Value:
	virtual void GetHDRSetupParams	(Vec4 pParams[4]) const = 0; 

	// Summary:
	//     Removes all particles and decals from the world.
	virtual void ResetParticlesAndDecals( ) = 0;

	// Summary:
	//     Creates new decals on the walls, static objects, terrain and entities.
	// Arguments:
	//     Decal - Structure describing the decal effect to be applied
	virtual void CreateDecal( const CryEngineDecalInfo & Decal )=0;

	// Summary:
	//     Removes decals in a specified range.
	// Arguments:
	//     vAreaBox - Specify the area in which the decals will be removed
	//     pEntity  - if not NULL will only delete decals attached to this entity
	virtual void DeleteDecalsInRange( AABB * pAreaBox, IRenderNode * pEntity ) = 0;

	//DOC-IGNORE-BEGIN
	  // Summary:
	  //	Renders far trees/objects as sprites.
	  // Description:
	  //	It's a call back for renderer. It renders far trees/objects as sprites. 
	  // Notes:
	  //	Will be removed from here.
	  virtual void DrawFarTrees(const SRenderingPassInfo &passInfo) = 0; // used by renderer

	  virtual void GenerateFarTrees(const SRenderingPassInfo &passInfo) = 0; // used by renderer

	//DOC-IGNORE-END

	// Summary:
	//     Sets the current outdoor ambient color. 
	virtual void SetSkyColor(Vec3 vColor)=0;

	// Summary:
	//     Sets the current sun color.
	virtual void SetSunColor(Vec3 vColor)=0;

	// Summary:
	//     Sets the current sky brightening multiplier.
	virtual void SetSkyBrightness(float fMul) = 0;

	// Summary:
	//     Gets the current sun/sky color relation.
	virtual float GetSunRel() const = 0;

	// Summary:
	//     Sets current rain parameters.
	virtual void SetRainParams(const SRainParams & rainParams) = 0;

	// Summary:
	//     Gets the validity and fills current rain parameters.
	virtual bool GetRainParams(SRainParams & rainParams) = 0;

	// Summary:
	//     Sets the view distance scale.
	// Arguments:
	//     fScale - may be between 0 and 1, 1.f = Unmodified view distance set by level designer, value of 0.5 will reduce it twice
  // Notes:
  //     This value will be reset automatically to 1 on next level loading.
  virtual void SetMaxViewDistanceScale(float fScale) = 0;

	// Summary:
	//     Gets the view distance.
	// Return Value:
	//     A float value representing the maximum view distance.
	virtual float GetMaxViewDistance( bool bScaled = true ) = 0;

	// Summary:
	//     Sets the fog color.
	virtual void SetFogColor(const Vec3& vFogColor)=0;

	// Summary:
	//     Gets the fog color.
	virtual Vec3 GetFogColor( )=0;

	// Summary:
	//   Gets various sky light parameters.
	virtual void GetSkyLightParameters( Vec3& sunDir, Vec3& sunIntensity, float& Km, float& Kr, float& g, Vec3& rgbWaveLengths ) = 0;

	// Summary:
	//   Sets various sky light parameters.
	virtual void SetSkyLightParameters( const Vec3& sunDir, const Vec3& sunIntensity, float Km, float Kr, float g, const Vec3& rgbWaveLengths, bool forceImmediateUpdate = false ) = 0;

	// Return Value:
	//   In logarithmic scale -4.0 .. 4.0
	virtual float GetLightsHDRDynamicPowerFactor() const = 0;

	// Return true if tessellation is allowed for given render object
	virtual bool IsTessellationAllowed(const CRenderObject *pObj, const SRenderingPassInfo &passInfo, bool bIgnoreShadowPass = false) const = 0;

	// allows to modify material on render nodes at run-time (make sure it is properly restored back)
	virtual void SetRenderNodeMaterialAtPosition( EERType eNodeType, const Vec3 & vPos, IMaterial * pMat ) = 0;

	// override the camera precache point with the requested position for the current round
	virtual void OverrideCameraPrecachePoint( const Vec3& vPos ) = 0;

	// begin streaming of meshes and textures for specified position, pre-cache stops after fTimeOut seconds
	virtual int AddPrecachePoint( const Vec3& vPos, const Vec3& vDir, float fTimeOut = 3.f, float fImportanceFactor = 1.0f) = 0;
	virtual void ClearPrecachePoint(int id) = 0;
	virtual void ClearAllPrecachePoints() = 0;

	virtual void GetPrecacheRoundIds(int pRoundIds[MAX_STREAM_PREDICTION_ZONES]) = 0;

	virtual void TraceFogVolumes( const Vec3& worldPos, ColorF& fogVolumeContrib, const SRenderingPassInfo &passInfo ) = 0;
	
	// Summary:
	//     Gets the interpolated terrain elevation for a specified location.
	// Notes:
	//     All x,y values are valid.
	// Arguments:
	//     x						- X coordinate of the location
	//     y						- Y coordinate of the location
	// Return Value:
	//     A float which indicate the elevation level.
	virtual float GetTerrainElevation(float x, float y, int nSID = DEFAULT_SID) = 0;

	// Summary:
	//     Gets the terrain elevation for a specified location.
	// Notes:
	//     Only values between 0 and WORLD_SIZE.
	// Arguments:
	//     x - X coordinate of the location
	//     y - Y coordinate of the location
	// Return Value:
	//     A float which indicate the elevation level.
	virtual float GetTerrainZ(int x, int y) = 0;

  // Summary:
  //     Gets the terrain hole flag for a specified location.
  // Notes:
  //     Only values between 0 and WORLD_SIZE.
  // Arguments:
  //     x - X coordinate of the location
  //     y - Y coordinate of the location
  // Return Value:
  //     A bool which indicate is there hole or not.
  virtual bool GetTerrainHole(int x, int y) = 0;

  // Summary:
  //     Gets the terrain surface normal for a specified location.
  // Arguments:
  //     vPos.x - X coordinate of the location
  //     vPos.y - Y coordinate of the location
  //     vPos.z - ignored
  // Return Value:
  //     A terrain surface normal.
  virtual Vec3 GetTerrainSurfaceNormal(Vec3 vPos) = 0;

	// Summary:
	//     Gets the unit size of the terrain
	// Notes:
	//     The value should currently be 2.
	// Return Value:
	//     A int value representing the terrain unit size in meters.
	virtual int   GetHeightMapUnitSize() = 0;

	// Summary:
	//     Gets the size of the terrain
	// Notes:
	//     The value should be 2048 by default.
	// Return Value:
	//     An int representing the terrain size in meters.
	virtual int   GetTerrainSize()=0;

	// Summary:
	//     Gets the size of the terrain sectors
	// Notes:
	//     The value should be 64 by default.
	// Return Value:
	//     An int representing the size of a sector in meters.
	virtual int   GetTerrainSectorSize()=0;

//DOC-IGNORE-BEGIN

	// Internal functions, mostly used by the editor, which won't be documented for now

	// Summary:
	//		Places object at specified position (for editor)
//	virtual bool AddStaticObject(int nObjectID, const Vec3 & vPos, const float fScale, unsigned char ucBright=255) = 0;
	// Summary:
	//		Removes static object from specified position (for editor)
//	virtual bool RemoveStaticObject(int nObjectID, const Vec3 & vPos) = 0;
	// Summary:
	//		On-demand physicalization of a static object
//	virtual bool PhysicalizeStaticObject(void *pForeignData,int iForeignData,int iForeignFlags) = 0;
	// Summary:
	//		Removes all static objects on the map (for editor)
	virtual void RemoveAllStaticObjects(int nSID=DEFAULT_SID) = 0;
	// Summary:
	//		Allows to set terrain surface type id for specified point in the map (for editor)
	virtual void SetTerrainSurfaceType(int x, int y, int nType)=0; // from 0 to 6 - sur type ( 7 = hole )
  
	// Summary:
	//		Returns true if game modified terrain hight map since last update by editor
	virtual bool IsTerrainHightMapModifiedByGame() = 0;
	// Summary:
	//		Updates hight map max height (in meters)
	virtual void SetHeightMapMaxHeight(float fMaxHeight) = 0;
    
	// Summary:
	//		Sets terrain sector texture id, and disable streaming on this sector
	virtual void SetTerrainSectorTexture( const int nTexSectorX, const int nTexSectorY, unsigned int textureId ) = 0;

	// Summary:
	//		Returns size of smallest terrain texture node (last leaf) in meters
	virtual int GetTerrainTextureNodeSizeMeters() = 0;

	// Arguments:
	//   nLayer - 0=diffuse texture, 1=occlusionmap
	// Return value:
	//   an integer value representing the size of terrain texture node in pixels
	virtual int GetTerrainTextureNodeSizePixels(int nLayer) = 0;

	// Summary:
	//		Sets group parameters
	virtual bool SetStatInstGroup(int nGroupId, const IStatInstGroup & siGroup, int nSID=0) = 0;

	// Summary:
	//		Gets group parameters
	virtual bool GetStatInstGroup(int nGroupId, IStatInstGroup & siGroup, int nSID=0) = 0;

	// Summary:
	//		Sets burbed out flag
	virtual void SetTerrainBurnedOut(int x, int y, bool bBurnedOut) = 0;
	
	// Summary:
	//		Gets burbed out flag
	virtual bool IsTerrainBurnedOut(int x, int y) = 0;

//DOC-IGNORE-END

	// Summary:
	//   Notifies of an explosion, and maybe creates an hole in the terrain
	// Description:
	//   This function should usually make sure that no static objects are near before making the hole.
	// Arguments:
	//   vPos - Position of the explosion
	//   fRadius - Radius of the explosion
	//   bDeformTerrain - Allow to deform the terrain
	virtual void OnExplosion(Vec3 vPos, float fRadius, bool bDeformTerrain = true) = 0;

	// Summary:
	//   Sets the physics material enumerator
	// Arguments:
	//   pPhysMaterialEnumerator - The physics material enumarator to set
	virtual void SetPhysMaterialEnumerator(IPhysMaterialEnumerator * pPhysMaterialEnumerator) = 0;

	// Summary:
	//   Gets the physics material enumerator
	// Return Value:
	//   A pointer to an IPhysMaterialEnumerator derived object.
	virtual IPhysMaterialEnumerator * GetPhysMaterialEnumerator() = 0;

//DOC-IGNORE-BEGIN
//Internal functions
	
	// Summary:
	//	 Allows to enable fog in editor
	virtual void SetupDistanceFog() = 0;

	// Summary:
	//	 Loads environment settings for specified mission
	virtual void LoadMissionDataFromXMLNode(const char * szMissionName) = 0;

	virtual void LoadEnvironmentSettingsFromXML(XmlNodeRef pInputNode, int nSID=DEFAULT_SID) = 0;

	// Summary:
	//	 Loads detail texture and detail object settings from XML doc (load from current LevelData.xml if pDoc is 0)
	virtual void	LoadTerrainSurfacesFromXML(XmlNodeRef pDoc, bool bUpdateTerrain, int nSID=DEFAULT_SID) = 0;

//DOC-IGNORE-END

	// Description:
	//   Physics applied to the area will apply to vegetations and allow it to move/blend.
	// Arguments:
	//   vPos - Center position to apply physics
	//   fRadius - Radius which specify the size of the area to apply physics
	//   fAmountOfForce - The amount of force, should be at least of 1.0f
	// Summary:
	//   Applies physics in a specified area
	virtual void ApplyForceToEnvironment(Vec3 vPos, float fRadius, float fAmountOfForce) = 0;

//DOC-IGNORE-BEGIN
	// Set direction to the sun
//	virtual void SetSunDir( const Vec3& vNewSunDir ) = 0;

	// Summary:
	//		Return non-normalized direction to the sun
	virtual Vec3 GetSunDir()  const= 0;

	// Summary:
	//		Return normalized direction to the sun
	virtual Vec3 GetSunDirNormalized()  const= 0;

	// Summary:
	//		Return realtime (updated every frame with real sun position) normalized direction to the scene
	virtual Vec3 GetRealtimeSunDirNormalized()  const= 0;

	// Summary:
	//     Gets the particles lighting related parameters
	virtual void GetParticlesLightingParams(float &fAmbMul, float &fLightsMul, float &fSunMul) const = 0;

// Internal function used by the 3d engine
	// Summary:
	//		Returns lighting level for this point.
	virtual Vec3 GetAmbientColorFromPosition(const Vec3 & vPos, float fRadius=1.f) = 0;

//Internal function used by 3d engine and renderer
	// Summary:
	//		Gets distance to the sector containig ocean water
	virtual float GetDistanceToSectorWithWater() = 0;
//DOC-IGNORE-END

	// Summary:
	//   Gets the environment ambient color.
	// Notes:
	//   Should have been specified in the editor.
	// Return Value:
	//   An rgb value contained in a Vec3 object.
	virtual Vec3 GetSkyColor() const = 0;

	// Summary:
	//   Gets the sun color
	// Notes:
	//   Should have been specified in the editor.
	// Return Value:
	//   An rgb value contained in a Vec3 object.
	virtual Vec3 GetSunColor() const = 0;

	// Summary:
	//   Retrieves the current sky brightening multiplier
	// Notes:
	// Return Value:
	//   Scalar value
	virtual float GetSkyBrightness() const = 0;

  // Summary:
  //   Retrieves the current SSAO multiplier
  // Notes:
  // Return Value:
  //   scalar value
  virtual float GetSSAOAmount() const = 0;

	// Summary:
	//   Retrieves the current SSAO contrast multiplier
	// Notes:
	// Return Value:
	//   scalar value
	virtual float GetSSAOContrast() const = 0;

	// Summary:
	//   Retrieves the current GI multiplier
	// Notes:
	// Return Value:
	//   scalar value
	virtual float GetGIAmount() const = 0;

  // Summary:
  //   Returns terrain texture multiplier.
  // Return Value:
  //   Scalar value
	virtual float GetTerrainTextureMultiplier(int nSID=0) const = 0;

	//  check object visibility taking into account portals and terrain occlusion test
	//  virtual bool IsBoxVisibleOnTheScreen(const Vec3 & vBoxMin, const Vec3 & vBoxMax, OcclusionTestClient * pOcclusionTestClient = NULL)=0;
	//  check object visibility taking into account portals and terrain occlusion test
	//  virtual bool IsSphereVisibleOnTheScreen(const Vec3 & vPos, const float fRadius, OcclusionTestClient * pOcclusionTestClient = NULL)=0;

//mat: todo

	// Summary:
	//	 Frees entity render info.
	virtual void FreeRenderNodeState(IRenderNode * pEntity) = 0;

	// Summary:
	//   Adds the level's path to a specified filename.
	// Arguments:
	//   szFileName - The filename for which we need to add the path
	// Return Value:
	//   Full path for the filename; including the level path and the filename appended after.
	virtual const char * GetLevelFilePath(const char * szFileName) = 0;

	// Summary:
	//   Displays statistic on the 3d Engine.
	// Arguments:
	//   fTextPosX - X position for the text
	//   fTextPosY - Y position for the text
	//   fTextStepY - Amount of pixels to distance each line
	//   bEnhanced - false=normal, true=more interesting information
	virtual void DisplayInfo(float & fTextPosX, float & fTextPosY, float & fTextStepY, const bool bEnhanced ) = 0;

	// Summary:
	//	 Draws text right aligned at the y pixel precision.
	virtual void DrawTextRightAligned( const float x, const float y, const char * format, ...) PRINTF_PARAMS(4, 5) =0;
	virtual void DrawTextRightAligned( const float x, const float y, const float scale,const ColorF &color,const char * format, ...) PRINTF_PARAMS(6, 7) =0;

	// Summary:
	//   Enables or disables portal at a specified position.
	// Arguments:
	//   vPos - Position to place the portal
	//   bActivate - Set to true in order to enable the portal, or to false to disable
	//   szEntityName -
	virtual void ActivatePortal(const Vec3 &vPos, bool bActivate, const char * szEntityName) = 0;

//DOC-IGNORE-BEGIN
	// Summary:
	//	 Counts memory usage
	virtual void GetMemoryUsage(ICrySizer * pSizer) const =0;

	// Summary:
	//	 Counts resource memory usage
	// Arguments:
	//		cstAABB - Use the whole level AABB if you want to grab the resources
	//              from the whole level.  For height level, use something BIG 
	//							(ie: +-FLT_MAX)
	// See also:
	//		ITerrain::GetTerrainSize().
	virtual void GetResourceMemoryUsage(ICrySizer * pSizer,const AABB& cstAABB)=0;
//DOC-IGNORE-END

	// Summary:
	//   Creates a new VisArea.
	// Return Value:
	//   A pointer to a newly created VisArea object
	virtual IVisArea * CreateVisArea(uint64 visGUID) = 0;

	// Summary:
	//   Deletes a VisArea.
	// Arguments:
	//   pVisArea - A pointer to the VisArea to delete
	virtual void DeleteVisArea(IVisArea * pVisArea) = 0;

//mat: todo

	// Summary:
	//   Updates the VisArea
	// Arguments:
	//   pArea -
	//   pPoints - 
	//   nCount -
	//   szName -
	//	 info	-
	//	 bReregisterObjects -
	virtual void UpdateVisArea(IVisArea * pArea, const Vec3 * pPoints, int nCount, const char * szName, const SVisAreaInfo & info, bool bReregisterObjects) = 0;

	// Summary:
	//   Determines if two VisAreas are connected.
	// Description:
	//   Used to determine if a sound is potentially hearable between two VisAreas.
	// Arguments:
	//   pArea1 - A pointer to a VisArea
	//   pArea2 - A pointer to a VisArea
	//   nMaxRecursion - Maximum number of recursions to be done
	//   bSkipDisabledPortals - Indicate if disabled portals should be skipped
	// Return Value:
	//   A boolean value set to true if the two VisAreas are connected, else false will be returned.
	virtual bool IsVisAreasConnected(IVisArea * pArea1, IVisArea * pArea2, int nMaxRecursion = 1, bool bSkipDisabledPortals = true) = 0;

//mat: todo

	// Summary:
	//   Creates instance of IRenderNode object with specified type.
	virtual IRenderNode* CreateRenderNode( EERType type ) = 0;

	// Summary:
	//	 Delete RenderNode object.
	virtual void DeleteRenderNode(IRenderNode * pRenderNode) = 0;

	// Summary:
	//	 Set global wind vector.
	virtual void SetWind( const Vec3 & vWind ) = 0;

	// Summary:
	//	 Gets wind direction and force, averaged within a box.
	virtual Vec3 GetWind( const AABB & box, bool bIndoors ) const =0;

	// Summary:
	//	 Gets the global wind vector.
	virtual Vec3 GetGlobalWind( bool bIndoors ) const =0;

	// Summary:
	//	 Gets wind direction and forace at the sample points provided. 
	// Note: the positions defining the samples will be overwritten 
	// with the accumulated wind influences.
	virtual bool SampleWind( Vec3* pSamples, int nSamples, const AABB& volume, bool bIndoors ) const = 0;

	// Description:
	//   Gets the VisArea which is present at a specified point.
	// Arguments:
	//	 vPos: 
	// Return Value:
	//	 VisArea containing point, if any. 0 otherwise.
	virtual	IVisArea * GetVisAreaFromPos(const Vec3 &vPos) = 0;	

	// Description:
	//   Tests for intersection against Vis Areas.
	// Arguments:
	//   box: Volume to test for intersection.
	//   pNodeCache (out, optional): Set to a cached pointer, for quicker calls to ClipToVisAreas.
	// Return Value:
	//	 Whether box intersects any vis areas.
	virtual	bool IntersectsVisAreas(const AABB& box, void** pNodeCache = 0) = 0;	

	// Description:
	//   Clips geometry against the boundaries of VisAreas.
	// Arguments:
	//   pInside: Vis Area to clip inside of. If 0, clip outside all Vis Areas.
	//	 sphere -
	//	 vNormal -
	//	 pNodeChache -
	// Return Value:
	//	 Whether it was clipped
	virtual	bool ClipToVisAreas(IVisArea* pInside, Sphere& sphere, Vec3 const& vNormal, void* pNodeCache = 0) = 0;	

	// Summary:
	//   Enables or disables ocean rendering.
	// Arguments:
	//   bOcean - Will enable or disable the rendering of ocean
	virtual void EnableOceanRendering(bool bOcean) = 0;

	// Description:
	//    Retrieve pointer to the material manager interface.
	virtual IMaterialManager* GetMaterialManager() = 0;
	
	// Summary:
	//   Creates a new light source.
	// Return Value:
	//   Pointer to newly created light or -1 if it fails.
	virtual struct ILightSource * CreateLightSource() = 0;

	// Summary:
	//   Deletes a light.
	// Arguments:
	//   Pointer to the light
	virtual void DeleteLightSource(ILightSource * pLightSource) = 0;

	// Summary:
	//   Gives access to the list holding all static light sources
	// Return Value:
	//   An array holding all the CDLight pointers.
	virtual const PodArray<CDLight*> * GetStaticLightSources() = 0;
	virtual const PodArray<ILightSource*> * GetLightEntities() = 0;

	// Summary:
	//   Gives access to list holding all lighting volumes
	// Return Value:
	//   An array holding all the SLightVolume pointers.
	virtual void GetLightVolumes( threadID nThreadID, SLightVolume *&pLightVols, uint32 &nNumVols ) = 0;

	// Summary:
	//   Reload the heightmap.
	// Description:
	//   Reloading the heightmap will resets all decals and particles.
	// Notes:
	//	 In future will restore deleted vegetations
	// Returns:
	//   success
  virtual bool RestoreTerrainFromDisk(int nSID=0) = 0;

//DOC-IGNORE-BEGIN
  // tmp
  virtual const char * GetFilePath(const char * szFileName) { return GetLevelFilePath(szFileName); }
//DOC-IGNORE-END

	// Summary:
	//	 Post processing effects interfaces.
	virtual void SetPostEffectParam(const char *pParam, float fValue, bool bForceValue=false) const = 0;
	virtual void SetPostEffectParamVec4(const char *pParam, const Vec4 &pValue, bool bForceValue=false) const = 0;
	virtual void SetPostEffectParamString(const char *pParam, const char *pszArg) const = 0;

	virtual void GetPostEffectParam(const char *pParam, float &fValue) const = 0;  
	virtual void GetPostEffectParamVec4(const char *pParam, Vec4 &pValue) const = 0;  
	virtual void GetPostEffectParamString(const char *pParam, const char* &pszArg) const = 0;

	virtual int32 GetPostEffectID(const char* pPostEffectName) = 0;

	virtual void ResetPostEffects(bool bOnSpecChange = false) const = 0;

	virtual void SetShadowsGSMCache(bool bCache) = 0;

	// Summary:
	//	 Physicalizes area if not physicalized yet.
	virtual void CheckPhysicalized(const Vec3 & vBoxMin, const Vec3 & vBoxMax) = 0;

	// Summary:
	//	 In debug mode check memory heap and makes assert, do nothing in release
	virtual void CheckMemoryHeap() = 0;

	// Summary:
	//	 Sets material parameter.
	// Arguments:
	//	 szMatName - materal name
	//	 nTexSlot  - text slot id, see EFTT_DIFFUSE for example
	//	 nSubMatId - submaterial id, -1 means use root material
	//	 szParamName - can be one of:
		
	//		m_eTGType
	//		m_eRotType
	//		m_eUMoveType
	//		m_eVMoveType
	//		m_bTexGenProjected

	//		m_Tiling[0]
	//		m_Tiling[1]
    //		m_Tiling[2]
	//		m_Offs[0]
	//		m_Offs[1]
	//		m_Offs[2]	

	//		m_Rot[0]
	//		m_Rot[1]
	//		m_Rot[2]
	//		m_RotOscRate[0]
	//		m_RotOscRate[1]
	//		m_RotOscRate[2]
	//		m_RotOscAmplitude[0]
	//		m_RotOscAmplitude[1]
	//		m_RotOscAmplitude[2]
	//		m_RotOscPhase[0]
	//		m_RotOscPhase[1]
	//		m_RotOscPhase[2]

	//		m_UOscRate
	//		m_VOscRate
	//		m_UOscAmplitude
	//		m_VOscAmplitude
	//		m_UOscPhase
	//		m_VOscPhase

	//	 fValue - new value
	virtual bool SetMaterialFloat( char * szMatName, int nSubMatId, int nTexSlot, char * szParamName, float fValue ) = 0;

	// Summary:
	//	 Closes terrain texture file handle and allows to replace/update it.
	virtual void CloseTerrainTextureFile(int nSID=DEFAULT_SID) = 0;
	
	// Summary:
	//	 Removes all decals attached to specified entity.
	virtual void DeleteEntityDecals(IRenderNode * pEntity) = 0;

	// Summary:
	//	 Finishes objects geometery generation/loading.
	virtual void CompleteObjectsGeometry() = 0;

	// Summary:
	//	 Disables CGFs unloading.
	virtual void LockCGFResources() = 0;

	// Summary:
	//	 Enables CGFs unloading (this is default state), this function will also release all not used CGF's.
	virtual void UnlockCGFResources() = 0;

	// Summary:
	//	 Gives access to materials library.
	virtual class CMatMan * GetMatMan() = 0;

	// Summary:
	//	 Creates static object containing empty IndexedMesh.
	virtual IStatObj * CreateStatObj() = 0;
	virtual IStatObj * CreateStatObjOptionalIndexedMesh(bool createIndexedMesh ) = 0;

	// Summary:
	//	 Creates the instance of the indexed mesh.
	virtual IIndexedMesh* CreateIndexedMesh() = 0;

	// Summary:
	//	 Paints voxel shape.
	virtual void Voxel_Paint(Vec3 vPos, float fRadius, int nSurfaceTypeId, Vec3 vBaseColor, EVoxelEditOperation eOperation, EVoxelBrushShape eShape, EVoxelEditTarget eTarget, PodArray<IRenderNode*> * pBrushes, float fMinVoxelSize) = 0;

	// Description:
	//	 Gets list of voxel objects that will be affected by paint operation, IMemoryBlock will contain array of IVoxelObject pointers.
	virtual IMemoryBlock * Voxel_GetObjects(Vec3 vPos, float fRadius, int nSurfaceTypeId, EVoxelEditOperation eOperation, EVoxelBrushShape eShape, EVoxelEditTarget eTarget) = 0;

	// Summary:
	//	Setups voxel flags.
	virtual void Voxel_SetFlags(bool bPhysics, bool bSimplify, bool bShadows, bool bMaterials) = 0;

	// Summary:
	//	 Updates rendering mesh in the stat obj associated with pPhysGeom. 
	// Notes:
	//	Creates or clones the object if necessary.
	virtual IStatObj * UpdateDeformableStatObj(IGeometry *pPhysGeom, bop_meshupdate *pLastUpdate=0, IFoliage *pSrcFoliage=0) = 0;

	// Summary:
	//	 Saves/loads state of engine objects.
	virtual void SerializeState( TSerialize ser ) = 0;
	
	// Summary:
	//	 Cleanups after save/load.	
	virtual void PostSerialize( bool bReading ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// CGF Loader.
	//////////////////////////////////////////////////////////////////////////
  // Description:
	// Creates a chunkfile content instance
  // Returns 'NULL' if the memory for the instance could not be allocated
  virtual CContentCGF* CreateChunkfileContent( const char *filename ) = 0;

  // Description:
	// Deletes the chunkfile content instance
  virtual void ReleaseChunkfileContent( CContentCGF* ) = 0;

  // Description:
	// Loads the contents of a chunkfile into the given CContentCGF. 
  // Returns 'false' on error.
	virtual bool LoadChunkFileContent( CContentCGF* pCGF, const char *filename,bool bNoWarningMode=false, bool bCopyChunkFile = true ) = 0;

  // Description:
	// Loads the contents of a chunkfile into the given CContentCGF. 
  // Returns 'false' on error.
	virtual bool LoadChunkFileContentFromMem( CContentCGF* pCGF, const void* pData, size_t nDataLen, uint32 nLoadingFlags, bool bNoWarningMode=false, bool bCopyChunkFile = true ) = 0;

	// Description:
	//    Creates ChunkFile.
	virtual IChunkFile * CreateChunkFile( bool bReadOnly=false ) = 0;

	// Description:
	//	  Returns interface to terrain engine
	virtual ITerrain * GetITerrain() = 0;

	// Description:
	//	  Creates terrain engine
	virtual ITerrain * CreateTerrain(const STerrainInfo & TerrainInfo) = 0;

	// Description:
	//	  Deletes terrain
	virtual void DeleteTerrain() = 0;

	// Description:
	//	  Deprecated
	virtual IVoxTerrain * GetIVoxTerrain() = 0;

	// Description:
	//	  Returns interface to visarea manager.
	virtual IVisAreaManager * GetIVisAreaManager() = 0;

	// Description:
	//	  Returns interface to the mergedmeshes subsystem
	virtual IMergedMeshesManager * GetIMergedMeshesManager() = 0;

	// Description:
	//	  Returns amount of light affecting a point in space inside a specific range (0 means no light affecting,
	//	  1 is completely affected by light). Use accurate parameter for a more expensive but with higher accuracy computation.
	virtual float GetLightAmountInRange(const Vec3 &pPos, float fRange, bool bAccurate = 0) = 0;

	// Description:
	//	  Places camera into every visarea or every manually set pre-cache points and render the scenes.
	virtual void PrecacheLevel(bool bPrecacheAllVisAreas, Vec3 * pPrecachePoints, int nPrecachePointsNum) = 0;

	// Description:
	//	  Proposes 3dengine to load on next frame all shaders and textures synchronously.
	virtual void ProposeContentPrecache() = 0;

	// Description:
	//	  Returns TOD interface.
	virtual ITimeOfDay* GetTimeOfDay() = 0;

	// Description:
	//	  Returns SkyBox material.
	virtual IMaterial* GetSkyMaterial() = 0;

	// Description:
	//    Sets global 3d engine parameter.
	virtual void SetGlobalParameter( E3DEngineParameter param,const Vec3 &v ) = 0;
	void SetGlobalParameter( E3DEngineParameter param,float val ) { SetGlobalParameter( param,Vec3(val,0,0) ); };

	// Description:
	//    Retrieves global 3d engine parameter.
	virtual void GetGlobalParameter( E3DEngineParameter param,Vec3 &v ) = 0;
	float GetGlobalParameter( E3DEngineParameter param ) { Vec3 v(0,0,0); GetGlobalParameter(param,v); return v.x; };

	virtual void SetShadowMode( EShadowMode shadowMode ) = 0;
	virtual EShadowMode GetShadowMode() const = 0;

	// Description:
	//		Saves pStatObj to a stream. 
	// Notes:
	//		Full mesh for generated ones, path/geom otherwise
	virtual int SaveStatObj(IStatObj *pStatObj, TSerialize ser) = 0;
	// Description:
	//		Loads statobj from a stream
	virtual IStatObj *LoadStatObj(TSerialize ser) = 0;

	// Description:
	//		Returns true if input line segment intersect clouds sprites.
	virtual bool CheckIntersectClouds(const Vec3 & p1, const Vec3 & p2) = 0;

	// Description:
	//		Removes references to RenderMesh
	virtual void OnRenderMeshDeleted(IRenderMesh * pRenderMesh) = 0;

	// Description:
	//	 Used to visualize frustum updates
	// Arguments:
	//   szName		- must not be 0
	//	 rCam		-
	//	 col		-
	//	 fQuadDist	-
	virtual void DebugDraw_PushFrustrum( const char *szName, const CCamera &rCam, const ColorB col, const float fQuadDist=-1.0f ) = 0;

	// Description:
	//	 Used to visualize frustum updates
	// Arguments:
	//   szName - must not be 0
	virtual void DebugDraw_PushFrustrum( const char *szName, const CRenderCamera &rCam, const ColorB col, const float fQuadDist=-1.0f ) = 0;

	// Description:
	//	 Used to highlight an object under the reticule
	virtual void DebugDraw_UpdateDebugNode() = 0;

	// Description:
	//	 Used by editor during AO computations
	virtual bool RayObjectsIntersection2D( Vec3 vStart, Vec3 vEnd, Vec3 & vHitPoint, EERType eERType ) = 0;


	// Description:
	//    Used by editor during object alignment
	virtual bool RenderMeshRayIntersection( IRenderMesh *pRenderMesh, SRayHitInfo &hitInfo,IMaterial *pCustomMtl=0 ) = 0;


	// Description:
	//	 Frees lod transition state
	virtual void FreeRNTmpData(CRNTmpData ** ppInfo) = 0;

	// Return Value:
	//	 True if e_ambient_occlusion is ON and AO data is valid
	virtual bool IsAmbientOcclusionEnabled() = 0;

	// pointer to ISegmentsManager interface
	virtual ISegmentsManager *GetSegmentsManager() = 0;
	virtual void SetSegmentsManager(ISegmentsManager *pSegmentsManager) = 0;

	// true if segmented world is performing an operation (load/save/move/etc)
	virtual bool IsSegmentOperationInProgress() = 0;
	virtual void SetSegmentOperationInProgress(bool bActive) = 0;

	// Description:
	//	 Call function 2 times (first to get the size then to fill in the data)
	// Arguments:
	//   pObjects - 0 if only the count is required
	// Return Value:
	//   Count returned
	virtual uint32 GetObjectsByType( EERType objType, IRenderNode **pObjects=0 )=0;
	virtual uint32 GetObjectsByTypeInBox( EERType objType, const AABB &bbox, IRenderNode **pObjects=0 ) = 0;
	virtual uint32 GetObjectsInBox(const AABB &bbox, IRenderNode **pObjects=0 ) =0 ;

	virtual void FillDebugFPSInfo(SDebugFPSInfo&) = 0;

	virtual void SetTerrainLayerBaseTextureData(int nLayerId, byte*pImage, int nDim, const char * nImgFileName, IMaterial * pMat, float fBr, float fTiling, int nDetailSurfTypeId, float fTilingDetail, float fSpecularAmount, float fSortOrder, ColorF layerFilterColor, float fUseRemeshing, bool bShowSelection) = 0;

	virtual bool IsAreaActivationInUse() = 0;

	virtual void RenderRenderNode_ShadowPass(IShadowCaster * pRNode, const SRenderingPassInfo &passInfo, JobManager::SJobState *pJobState ) = 0;

	// Arguments:
	//	 srcfilename -
	//	 filename	 -
	//	 pipe		 -
	//	 bSaveMorps  - saves all other morphs that are not in morphs list
	virtual bool SaveCHR(const char* srcfilename,const char* filename,IMergeCHRMorphsPipe* pipe,bool bSaveMorps=false) = 0;

	// Description:
	//	 Saves mesh by FaceGen
	virtual bool SaveFaceCHR(const char* temlatefilename,const char* headFGTfilename,const char* facefilename,const char* attFGTfilename,const char* outfilename, IMaterial** material) = 0;

	virtual bool MergeCHRs(const char* basechr,IMergePipe* pipe,const char* result_filename,IMergeMaterialsResult**,IMergePipe::EMergeFlags flags) = 0;

	// Description:
	//	 Returns true if materials can be merged in to one
	virtual bool CanBeMerged(IMaterial* mt1,IMaterial* mt2,bool bVerbose,IMaterialMergePipe::EMergeMaterialsFlags flags) = 0;
	virtual IMergeMaterialsResult* MergeMaterials(IMaterialMergePipe* pipe,IMaterialMergePipe::EMergeMaterialsFlags flags) = 0;

	virtual const char * GetVoxelEditOperationName(EVoxelEditOperation eOperation) = 0;

	// Summary:
	//   Gives 3dengine access to original and most precise heighmap data in the editor
	virtual void SetGetLayerIdAtCallback(IGetLayerIdAtCallback * pCallBack) = 0;

	virtual PodArray<CDLight*>* GetDynamicLightSources() = 0;

	virtual IParticleManager * GetParticleManager() = 0;

	virtual IOpticsManager * GetOpticsManager() = 0;

	// Description:
	//		Syncs and performs outstanding operations for the Asyncrhon ProcessStreaming Update
	virtual void SyncProcessStreamingUpdate() = 0;

	// Set Callback for Editor to store additional information in Minimap tool
	virtual void SetScreenshotCallback(IScreenshotCallback* pCallback) = 0;

	// Show/Hide objects by layer (useful for streaming and performance)
	virtual void ActivateObjectsLayer(uint16 nLayerId, bool bActivate, bool bPhys, bool bObjects, bool bStaticLights, const char * pLayerName, IGeneralMemoryHeap* pHeap = NULL, bool bCheckLayerActivation=true) = 0;

	// Get object layer memory usage
	virtual void GetLayerMemoryUsage(uint16 nLayerId, ICrySizer* pSizer, int* pNumBrushes, int* pNumDecals) const = 0;

	// Collect layer ID's to skip loading objects from these layers, e.g. to skip console specific layers
	virtual void SkipLayerLoading(uint16 nLayerId, bool bClearList) = 0;

	// Activate streaming of character and all sub-components
	virtual void PrecacheCharacter(IRenderNode * pObj, const float fImportance,  ICharacterInstance * pCharacter, IMaterial* pSlotMat, const Matrix34& matParent, const float fEntDistance, const float fScale, int nMaxDepth, bool bForceStreamingSystemUpdate, const SRenderingPassInfo &passInfo ) = 0;

	// Activate streaming of render node and all sub-components
	virtual void PrecacheRenderNode(IRenderNode * pObj, float fEntDistanceReal) = 0;

	// Called when the segmented world moves
	virtual void OffsetPosition(Vec3 &delta) = 0;

	virtual IDeferredPhysicsEventManager* GetDeferredPhysicsEventManager() =0;
	
	// Return true if terrain texture streaming takes place
	virtual bool IsTerrainTextureStreamingInProgress() = 0;
	virtual void SetStreamableListener(IStreamedObjectListener* pListener) = 0;

	// following functions are used by SRenderingPassInfo
	virtual CCamera* GetRenderingPassCamera( const CCamera &rCamera ) =0;
	virtual SRenderObjectModifier* GetTempRenderObjectModifier( const SRenderObjectModifier &rRenderObjectModifier, const SRenderingPassInfo &passInfo ) = 0;

	virtual int GetZoomMode() const = 0;
	virtual float GetPrevZoomFactor() = 0;
	virtual void SetZoomMode( int nZoomMode ) = 0;
	virtual void SetPrevZoomFactor( float fZoomFactor ) = 0;
    // Enable/Disable octree updates in a seperate thread
    virtual bool ToggelAsyncOctreeUpdates(bool) = 0;

	// LiveCreate
	virtual void SaveInternalState(struct IDataWriteStream& writer, const AABB& filterArea, const bool bTerrain, const uint32 objectMask) = 0;
	virtual void LoadInternalState(struct IDataReadStream& reader, const uint8* pVisibleLayersMasks, const uint16* pLayerIdTranslation) = 0;

	// </interfuscator:shuffle>
};

//==============================================================================================

// Summary:
//	 Types of binary files used by 3dengine
enum EFileTypes
{
	eTerrainTextureFile=100,
};

#define FILEVERSION_TERRAIN_TEXTURE_FILE 9

// Summary:
//	 Common header for binary files used by 3dengine
struct SCommonFileHeader
{
	void Set(uint16 t, uint16 v) { strcpy(signature,"CRY"); file_type = (uint8)t; version = v; }
	bool Check(uint16 t, uint16 v) { return strcmp(signature,"CRY")==0 && t==file_type && v==version; }

	char				signature[4];						// File signature, should be "CRY "
  uint8			  file_type;							// File type
  uint8			  flags;							    // File common flags
	uint16			version;								// File version

	AUTO_STRUCT_INFO
};

// Summary:
//	 Sub header for terrain texture file
// Notes:
//	 "locally higher texture resolution" following structure can be removed (as well in autotype)
struct STerrainTextureFileHeader_old
{
	uint16			nSectorSizeMeters;			//
	uint16			nLodsNum;								//
	uint16			nLayerCount;						// STerrainTextureLayerFileHeader count following (also defines how may layers are interleaved) 1/2
	uint16			nReserved;

  AUTO_STRUCT_INFO
};

#define TTFHF_AO_DATA_IS_VALID 1
#define TTFHF_BIG_ENDIAN 2

// Summary:
// Sub header for terrain texture file
struct STerrainTextureFileHeader
{
	uint16			nLayerCount;						// STerrainTextureLayerFileHeader count following (also defines how may layers are interleaved) 1/2
	uint16			dwFlags;
  float				fBrMultiplier;

  AUTO_STRUCT_INFO
};

// Summary:
//	 Layer header for terrain texture file (for each layer)
struct STerrainTextureLayerFileHeader
{
	uint16			nSectorSizePixels;	//
	uint16			nReserved;					// ensure padding and for later usage
	ETEX_Format eTexFormat;					// typically eTF_DXT1, eTF_A4R4G4B4 or eTF_R5G6B5
	uint32			nSectorSizeBytes;		// redundant information for more convenient loading code

  AUTO_STRUCT_INFO
};

#pragma pack(pop)

#include <Cry_Camera.h>

// state of 3dengine during rendering
// used to prevent global state
struct SRenderingPassInfo
{
	enum EShadowMapType
	{
		SHADOW_MAP_NONE = 0,
		SHADOW_MAP_GSM0,
		SHADOW_MAP_GSM1,
		SHADOW_MAP_GSM2,
		SHADOW_MAP_GSM3,
		SHADOW_MAP_GSM4,
		SHADOW_MAP_GSM5,
		SHADOW_MAP_GSM6,
		SHADOW_MAP_GSM7,
		SHADOW_MAP_LOCAL,
		SHADOW_MAP_REFLECTIVE
	};

	// enum flags to identify which objects to skip for this pass
	enum ESkipRenderingFlags
	{
		SHADOWS = BIT(0),
		BRUSHES = BIT(1),
		VEGETATION = BIT(2),
		ENTITIES = BIT(3),
		TERRAIN = BIT(4),
		WATEROCEAN = BIT(5),
		PARTICLES = BIT(6),
		DECALS = BIT(7),
		TERRAIN_DETAIL_MATERIALS = BIT(8),
		FAR_SPRITES = BIT(9),
		WATER_WAVES = BIT(12),
		ROADS = BIT(13),
		WATER_VOLUMES = BIT(14),
		CLOUDS = BIT(15),
		CUBEMAP_GEN = BIT(16),
		// below are precombined flags
		STATIC_OBJECTS = BRUSHES | VEGETATION,
		DEFAULT_FLAGS = SHADOWS | BRUSHES | VEGETATION | ENTITIES | TERRAIN | WATEROCEAN | PARTICLES | DECALS | TERRAIN_DETAIL_MATERIALS | FAR_SPRITES | WATER_WAVES | ROADS | WATER_VOLUMES | CLOUDS, 
		DEFAULT_RECURSIVE_FLAGS = BRUSHES | VEGETATION | ENTITIES | TERRAIN | WATEROCEAN | PARTICLES | DECALS | TERRAIN_DETAIL_MATERIALS | FAR_SPRITES | WATER_WAVES | ROADS | WATER_VOLUMES | CLOUDS
	};
	
	// creating function for RenderingPassInfo, the create functions will fetch all other necessary
	// information like thread id/frame id, etc
	static SRenderingPassInfo CreateGeneralPassRenderingInfo( const CCamera &rCamera, uint32 nRenderingFlags = DEFAULT_FLAGS, bool bAuxWindow = false );
	static SRenderingPassInfo CreateRecursivePassRenderingInfo( const CCamera &rCamera, uint32 nRenderingFlags = DEFAULT_RECURSIVE_FLAGS  );
	static SRenderingPassInfo CreateShadowPassRenderingInfo( const CCamera &rCamera, SRenderingPassInfo::EShadowMapType eShadowMapType, uint32 nRenderingFlags = DEFAULT_FLAGS  );
	static SRenderingPassInfo CreateShadowPassRenderingInfo( const CCamera &rCamera, int nLightFlags, int nShadowMapLod, uint32 *pShadowGenMask, uint32 nSide, uint32 nShadowFrustumID, uint32 nRenderingFlags = DEFAULT_FLAGS  );	
	static SRenderingPassInfo CreateTempRenderingInfo( const CCamera &rCamera, const SRenderingPassInfo &rPassInfo );
	static SRenderingPassInfo CreateTempRenderingInfo( uint32 nRenderingFlags, const SRenderingPassInfo &rPassInfo );
	
	// state getter	
	bool IsGeneralPass() const;

	bool IsRecursivePass() const;
	uint32 GetRecursiveLevel() const;

	bool IsShadowPass() const;
	EShadowMapType GetShadowMapType() const;

	bool IsAuxWindow() const;

	threadID ThreadID() const;

	int GetFrameID() const;
	uint32 GetMainFrameID() const;

	const CCamera& GetCamera() const;
	bool IsCameraUnderWater() const;

	float GetZoomFactor() const;
	float GetInverseZoomFactor() const;
	bool IsZoomActive() const;
	bool IsZoomInProgress() const;

	bool RenderShadows() const;
	bool RenderBrushes() const;
	bool RenderVegetation() const;
	bool RenderEntities() const;
	bool RenderTerrain() const;
	bool RenderWaterOcean() const;
	bool RenderParticles() const;
	bool RenderDecals() const;
	bool RenderTerrainDetailMaterial() const;
	bool RenderFarSprites() const;
	bool RenderWaterWaves() const;
	bool RenderRoads() const;
	bool RenderWaterVolumes() const;
	bool RenderClouds() const;

	bool IsRenderingCubemap() const;

	uint32* ShadowGenMaskAddress() const;
	uint32 ShadowFrustumID() const;
	uint8 ShadowFrustumSide() const;

private:

	// private constructor, creation is only allowed with create functions
	SRenderingPassInfo()
		: pShadowGenMask(NULL)
		, nSide(0)
		, nShadowFrustumId(0)
		, m_bAuxWindow(0)
		, m_nRenderStackLevel(0)
		, m_eShadowMapRendering(static_cast<uint8>(SHADOW_MAP_NONE))
	{
		threadID nThreadID = 0;
		gEnv->pRenderer->EF_Query(EFQ_MainThreadList, nThreadID);
		m_nThreadID =  static_cast<uint8>(nThreadID);
		m_nRenderFrameID = gEnv->pRenderer->GetFrameID();
		m_nRenderMainFrameID = gEnv->pRenderer->GetFrameID(false);
	}

	void InitRenderingFlags(uint32 nRenderingFlags);
	void SetCamera(const CCamera& cam);

	uint8 m_nThreadID;
	uint8 m_nRenderStackLevel;	
	uint8 m_eShadowMapRendering;		// State flag denoting what type of shadow map is being currently rendered into
	uint8 m_bCameraUnderWater;

	uint32 m_nRenderingFlags;

	float m_fZoomFactor;

	int m_nRenderFrameID;
	uint32 m_nRenderMainFrameID;

	const CCamera *m_pCamera;

	// members used only in shadow pass
	uint32 *pShadowGenMask;	
	uint32 nShadowFrustumId;	
	uint8 nSide;
	uint8 m_nZoomInProgress;
	uint8 m_nZoomMode;
	uint8 m_bAuxWindow;
};
 
///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsGeneralPass() const 
{ 
	return m_nRenderStackLevel == 0 && m_bAuxWindow == 0 && static_cast<EShadowMapType>(m_eShadowMapRendering) == SHADOW_MAP_NONE; 
}

///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsRecursivePass() const
{ 
	return m_nRenderStackLevel > 0; 
}

///////////////////////////////////////////////////////////////////////////////
inline uint32 SRenderingPassInfo::GetRecursiveLevel() const 
{ 
	return m_nRenderStackLevel; 
}

///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsShadowPass() const 
{ 
	return static_cast<EShadowMapType>(m_eShadowMapRendering) != SHADOW_MAP_NONE; 
}

///////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo::EShadowMapType SRenderingPassInfo::GetShadowMapType() const 
{
	assert(IsShadowPass());
	return static_cast<EShadowMapType>(m_eShadowMapRendering); 
}

///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsAuxWindow() const 
{ 
	return m_bAuxWindow != 0;
}

///////////////////////////////////////////////////////////////////////////////
inline threadID SRenderingPassInfo::ThreadID() const 
{
	return m_nThreadID;
}

///////////////////////////////////////////////////////////////////////////////
inline int SRenderingPassInfo::GetFrameID() const
{
	return m_nRenderFrameID;
}

///////////////////////////////////////////////////////////////////////////////
inline uint32 SRenderingPassInfo::GetMainFrameID() const
{
	return m_nRenderMainFrameID;
}

///////////////////////////////////////////////////////////////////////////////
inline const CCamera& SRenderingPassInfo::GetCamera() const 
{ 
	assert(m_pCamera!= NULL); 
	return *m_pCamera; 
}
	
///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsCameraUnderWater() const
{
	return m_bCameraUnderWater != 0;
}

///////////////////////////////////////////////////////////////////////////////
inline float SRenderingPassInfo::GetZoomFactor() const
{
	return m_fZoomFactor;
}

///////////////////////////////////////////////////////////////////////////////
inline float SRenderingPassInfo::GetInverseZoomFactor() const
{
	return 1.0f/m_fZoomFactor;
}

///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsZoomActive() const
{
	return m_nZoomMode != 0;
}

///////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsZoomInProgress() const
{
	return m_nZoomInProgress != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderShadows() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::SHADOWS) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderBrushes() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::BRUSHES) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderVegetation() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::VEGETATION) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderEntities() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::ENTITIES) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderTerrain() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::TERRAIN) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderWaterOcean() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::WATEROCEAN) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderParticles() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::PARTICLES) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderDecals() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::DECALS) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderFarSprites() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::FAR_SPRITES) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderTerrainDetailMaterial() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::TERRAIN_DETAIL_MATERIALS) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderWaterWaves() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::WATER_WAVES) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderRoads() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::ROADS) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderWaterVolumes() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::WATER_VOLUMES) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::RenderClouds() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::CLOUDS) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline bool SRenderingPassInfo::IsRenderingCubemap() const
{
	return (m_nRenderingFlags & SRenderingPassInfo::CUBEMAP_GEN) != 0;
}

////////////////////////////////////////////////////////////////////////////////
inline uint32* SRenderingPassInfo::ShadowGenMaskAddress() const
{
	assert(pShadowGenMask);
	return pShadowGenMask;
}

////////////////////////////////////////////////////////////////////////////////
inline uint32 SRenderingPassInfo::ShadowFrustumID() const
{
	return nShadowFrustumId;
}

////////////////////////////////////////////////////////////////////////////////
inline uint8 SRenderingPassInfo::ShadowFrustumSide() const
{
	return nSide;
}

////////////////////////////////////////////////////////////////////////////////
inline void SRenderingPassInfo::SetCamera(const CCamera& cam)
{
	m_pCamera = gEnv->p3DEngine->GetRenderingPassCamera(cam);
	m_bCameraUnderWater = gEnv->p3DEngine->IsUnderWater(cam.GetPosition());
	m_fZoomFactor = 0.2f + 0.8f*(RAD2DEG(cam.GetFov())/60.f);
	m_nZoomInProgress = 0;
	m_nZoomMode = 0;
}

////////////////////////////////////////////////////////////////////////////////
inline void SRenderingPassInfo::InitRenderingFlags(uint32 nRenderingFlags)
{
	m_nRenderingFlags = nRenderingFlags;
#if !defined(_RELEASE)
	static ICVar *pDefaultMaterial = gEnv->pConsole->GetCVar("e_DefaultMaterial");
	static ICVar *pDetailMaterial = gEnv->pConsole->GetCVar("e_TerrainDetailMaterials");
	static ICVar *pShadows = gEnv->pConsole->GetCVar("e_Shadows");
	static ICVar *pBrushes = gEnv->pConsole->GetCVar("e_Brushes");
	static ICVar *pVegetation = gEnv->pConsole->GetCVar("e_Vegetation");
	static ICVar *pEntities = gEnv->pConsole->GetCVar("e_Entities");
	static ICVar *pTerrain = gEnv->pConsole->GetCVar("e_Terrain");
	static ICVar *pWaterOcean = gEnv->pConsole->GetCVar("e_WaterOcean");
	static ICVar *pParticles = gEnv->pConsole->GetCVar("e_Particles");
	static ICVar *pDecals = gEnv->pConsole->GetCVar("e_Decals");
	static ICVar *pWaterWaves = gEnv->pConsole->GetCVar("e_WaterWaves");
	static ICVar *pWaterVolumes = gEnv->pConsole->GetCVar("e_WaterVolumes");
	static ICVar *pRoads = gEnv->pConsole->GetCVar("e_Roads");
	static ICVar *pClouds = gEnv->pConsole->GetCVar("e_Clouds");

	if(pShadows->GetIVal() == 0)				m_nRenderingFlags &= ~SRenderingPassInfo::SHADOWS;
	if(pBrushes->GetIVal() == 0)				m_nRenderingFlags &= ~SRenderingPassInfo::BRUSHES;
	if(pVegetation->GetIVal() == 0)			m_nRenderingFlags &= ~SRenderingPassInfo::VEGETATION;
	if(pEntities->GetIVal() == 0)				m_nRenderingFlags &= ~SRenderingPassInfo::ENTITIES;
	if(pTerrain->GetIVal() == 0)				m_nRenderingFlags &= ~SRenderingPassInfo::TERRAIN;
	if(pWaterOcean->GetIVal() == 0)			m_nRenderingFlags &= ~SRenderingPassInfo::WATEROCEAN;
	if(pParticles->GetIVal() == 0)			m_nRenderingFlags &= ~SRenderingPassInfo::PARTICLES;
	if(pDecals->GetIVal() == 0)					m_nRenderingFlags &= ~SRenderingPassInfo::DECALS;
	if(pWaterWaves->GetIVal() == 0)			m_nRenderingFlags &= ~SRenderingPassInfo::WATER_WAVES;
	if(pRoads->GetIVal() == 0)					m_nRenderingFlags &= ~SRenderingPassInfo::ROADS;
	if(pWaterVolumes->GetIVal() == 0)		m_nRenderingFlags &= ~SRenderingPassInfo::WATER_VOLUMES;
	if(pClouds->GetIVal() == 0)					m_nRenderingFlags &= ~SRenderingPassInfo::CLOUDS;
	
	if(pDefaultMaterial->GetIVal() != 0 || pDetailMaterial->GetIVal() == 0)
		m_nRenderingFlags &= ~SRenderingPassInfo::TERRAIN_DETAIL_MATERIALS;

	// on dedicated server, never render any object at all
	if( gEnv->IsDedicated() )
		m_nRenderingFlags = 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo SRenderingPassInfo::CreateGeneralPassRenderingInfo( const CCamera &rCamera, uint32 nRenderingFlags, bool bAuxWindow )
{
	static ICVar *pCameraFreeze = gEnv->pConsole->GetCVar("e_CameraFreeze");

	// Update Camera only if e_camerafreeze is not set
	const CCamera &rCameraToSet = (pCameraFreeze && pCameraFreeze->GetIVal() != 0) ? gEnv->p3DEngine->GetRenderingCamera() : rCamera;

	SRenderingPassInfo passInfo;
	passInfo.SetCamera(rCameraToSet);
	passInfo.InitRenderingFlags(nRenderingFlags);
	passInfo.m_bAuxWindow = bAuxWindow;

	// update general pass zoom factor
	passInfo.m_nZoomMode = gEnv->p3DEngine->GetZoomMode();	
	float fPrevZoomFactor = gEnv->p3DEngine->GetPrevZoomFactor();
	
	passInfo.m_nZoomInProgress = passInfo.m_nZoomMode && fabs(fPrevZoomFactor - passInfo.m_fZoomFactor) > 0.02f;

	int nZoomMode = passInfo.m_nZoomMode;
	const float fZoomThreshold = 0.7f;
	if (passInfo.m_fZoomFactor < fZoomThreshold)
      ++nZoomMode;
  else  
      --nZoomMode;
  
	// clamp zoom mode into valid range
	passInfo.m_nZoomMode = clamp_tpl<int>(nZoomMode,0,4);

	// store information for next frame in 3DEngine
	gEnv->p3DEngine->SetPrevZoomFactor(passInfo.m_fZoomFactor);
	gEnv->p3DEngine->SetZoomMode(passInfo.m_nZoomMode);


	return passInfo;
}

///////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo SRenderingPassInfo::CreateRecursivePassRenderingInfo( const CCamera &rCamera, uint32 nRenderingFlags )
{
	static ICVar *pRecursionViewDistRatio = gEnv->pConsole->GetCVar("e_RecursionViewDistRatio");
	
	SRenderingPassInfo passInfo;
	passInfo.m_nRenderStackLevel = 1;
	passInfo.SetCamera(rCamera);

	// adjust view distance in recursive mode by adjusting the ZoomFactor
	passInfo.m_fZoomFactor /= pRecursionViewDistRatio->GetFVal();

	passInfo.InitRenderingFlags(nRenderingFlags);

	return passInfo;
}

///////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo SRenderingPassInfo::CreateShadowPassRenderingInfo( const CCamera &rCamera, SRenderingPassInfo::EShadowMapType eShadowMapType, uint32 nRenderingFlags )
{
	SRenderingPassInfo passInfo;
	passInfo.m_eShadowMapRendering = static_cast<uint8>(eShadowMapType);
	passInfo.SetCamera(rCamera);
	passInfo.InitRenderingFlags(nRenderingFlags);

	return passInfo;
}

///////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo SRenderingPassInfo::CreateShadowPassRenderingInfo( const CCamera &rCamera, int nLightFlags, int nShadowMapLod, uint32 *pShadowGenMask, uint32 nSide, uint32 nShadowFrustumID, uint32 nRenderingFlags  )
{
	SRenderingPassInfo passInfo;
	passInfo.SetCamera(rCamera);
	passInfo.InitRenderingFlags(nRenderingFlags);

	// set correct shadow map type
	if (nLightFlags & DLF_SUN)
	{
		assert(nShadowMapLod >= 0 && nShadowMapLod < 8);
		passInfo.m_eShadowMapRendering = static_cast<uint8>(EShadowMapType(SHADOW_MAP_GSM0 + nShadowMapLod));
	}
	else if (nLightFlags & DLF_REFLECTIVE_SHADOWMAP)
		passInfo.m_eShadowMapRendering = static_cast<uint8>(SHADOW_MAP_REFLECTIVE);
	else if (nLightFlags & (DLF_POINT | DLF_PROJECT | DLF_AREA_LIGHT))
		passInfo.m_eShadowMapRendering = static_cast<uint8>(SHADOW_MAP_LOCAL);
	else
		passInfo.m_eShadowMapRendering = static_cast<uint8>(SHADOW_MAP_NONE);
	
	passInfo.pShadowGenMask = pShadowGenMask;
	passInfo.nSide = nSide;
	passInfo.nShadowFrustumId = nShadowFrustumID;

	return passInfo;
}

///////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo SRenderingPassInfo::CreateTempRenderingInfo( const CCamera &rCamera, const SRenderingPassInfo &rPassInfo )
{
	SRenderingPassInfo passInfo = rPassInfo;
	passInfo.SetCamera(rCamera);

	passInfo.pShadowGenMask = NULL;
	passInfo.nSide = 0;
	passInfo.nShadowFrustumId = 0;

	return passInfo;
}

///////////////////////////////////////////////////////////////////////////////
inline SRenderingPassInfo SRenderingPassInfo::CreateTempRenderingInfo( uint32 nRenderingFlags, const SRenderingPassInfo &rPassInfo )
{
	SRenderingPassInfo passInfo = rPassInfo;
	passInfo.m_nRenderingFlags = nRenderingFlags;
	return passInfo;
}

///////////////////////////////////////////////////////////////////////////////
// class to wrap a special counter used to presort SRendItems
// this is used to fix random ordering introduced by parallelization
// of parts of the 3DEngine
struct SRendItemSorter
{
	// Deferred PreProcess needs a special ordering, use these to prefix the values
	// to ensure the deferred shading pass is after all LPV objects
	enum EDeferredPreprocess
	{		
		eLPVPass = 0,
		eDeferredShadingPass = BIT(30)
	};

	static SRendItemSorter CreateRendItemSorter( const SRenderingPassInfo &passInfo );
	static SRendItemSorter CreateShadowPassRendItemSorter( const SRenderingPassInfo &passInfo );
	static SRendItemSorter CreateParticleRendItemSorter( const SRenderingPassInfo &passInfo );
	static SRendItemSorter CreateDeferredPreProcessRendItemSorter( const SRenderingPassInfo &passInfo, EDeferredPreprocess deferredPrerocessType);
	static SRendItemSorter CreateDefaultRendItemSorter();

	void IncreaseOctreeCounter()	{ nValue += eOctreeNodeCounter; }
	void IncreaseObjectCounter()	{ nValue += eObjectCounter; }
	void IncreaseGroupCounter()		{ nValue += eGroupCounter; }
	
	void IncreaseParticleCounter()	{ nValue += eParticleCounter; }	
	uint32 ParticleCounter() const	{ return nValue & ~eRecursivePassMask; }

	uint32 ShadowFrustumID() const	{ return nValue  & ~eRecursivePassMask; }

	uint32 GetValue() const { return nValue; }

	bool operator<( const SRendItemSorter &rOther ) const
	{
		return nValue < rOther.nValue;
	}

	bool IsRecursivePass() const { return (nValue & eRecursivePassMask) != 0; }

	SRendItemSorter() : nValue(0) {}
	explicit SRendItemSorter(uint32 _nValue) : nValue(_nValue) {}

private:
	// encode various counter in a single value
	enum { eRecursivePassMask = BIT(31) }; // present in all combinations

	// flags used for regular SRendItems
	enum { eObjectCounter = BIT(0) };					// bits 0-14 used
	enum { eOctreeNodeCounter = BIT(14) };		// bits 15-27 used
	enum { eGroupCounter = BIT(27) };					// bits 28-31 used

	// flags used for Particles
	enum { eParticleCounter = BIT(0) };

	uint32 nValue;
};

///////////////////////////////////////////////////////////////////////////////
inline SRendItemSorter SRendItemSorter::CreateRendItemSorter( const SRenderingPassInfo &passInfo )
{
	SRendItemSorter rendItemSorter;
	rendItemSorter.nValue = 0;
	rendItemSorter.nValue |= passInfo.IsRecursivePass() ? eRecursivePassMask : 0;
	return rendItemSorter;
}

///////////////////////////////////////////////////////////////////////////////
inline SRendItemSorter SRendItemSorter::CreateShadowPassRendItemSorter( const SRenderingPassInfo &passInfo )
{
	SRendItemSorter rendItemSorter;	
	rendItemSorter.nValue = passInfo.ShadowFrustumID();
	rendItemSorter.nValue |= passInfo.IsRecursivePass() ? eRecursivePassMask : 0;
	return rendItemSorter;
}

///////////////////////////////////////////////////////////////////////////////
inline SRendItemSorter SRendItemSorter::CreateParticleRendItemSorter( const SRenderingPassInfo &passInfo )
{
	SRendItemSorter rendItemSorter;
	rendItemSorter.nValue = 0;
	rendItemSorter.nValue |= passInfo.IsRecursivePass() ? eRecursivePassMask : 0;	
	return rendItemSorter;
}

///////////////////////////////////////////////////////////////////////////////
inline SRendItemSorter SRendItemSorter::CreateDeferredPreProcessRendItemSorter( const SRenderingPassInfo &passInfo, EDeferredPreprocess deferredPrerocessType)
{
	SRendItemSorter rendItemSorter;
	rendItemSorter.nValue = 0;
	rendItemSorter.nValue |= passInfo.IsRecursivePass() ? eRecursivePassMask : 0;	
	rendItemSorter.nValue |= deferredPrerocessType;
	return rendItemSorter;
}

///////////////////////////////////////////////////////////////////////////////
inline SRendItemSorter SRendItemSorter::CreateDefaultRendItemSorter()
{
	SRendItemSorter rendItemSorter;
	rendItemSorter.nValue = 0;
	return rendItemSorter;
}

#endif //CRY3DENGINEINTERFACE_H

