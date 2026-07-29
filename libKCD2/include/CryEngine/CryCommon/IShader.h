/*=============================================================================
  IShader.h : Shaders common interface.
  Copyright (c) 2001-2002 Crytek Studios. All Rights Reserved.

  Revision history:
    * Created by Honich Andrey

=============================================================================*/
#include DEVIRTUALIZE_HEADER_FIX(IShader.h)

#ifndef _ISHADER_H_
#define _ISHADER_H_


#if defined(LINUX) || defined(APPLE) || defined(PS3)
  #include <platform.h>
#endif

#include "smartptr.h"
#include <IMaterial.h> // <> required for Interfuscator
#include <IFlares.h> // <> required for Interfuscator
#include "VertexFormats.h"

#include "Cry_XOptimise.h"
#include <CrySizer.h>

#include "crc32.h"
#include <CryThreadSafeRendererContainer.h>
#include <SpuUtils.h>

struct IRenderMesh;
struct IShader;
struct IVisArea;
class CShader;
struct STexAnim;
struct SShaderPass;
struct SShaderItem;
class ITexture;
struct SParam;
class CMaterial;
struct SShaderSerializeContext;
enum EVertexFormat;
struct IRenderMesh;
struct IAnimNode;
struct SSkinningData;
struct SSTexSamplerFX;
namespace JobManager { struct SJobState; }

//================================================================

// Summary:
//	 Geometry Culling type.
enum ECull
{
  eCULL_Back = 0,	// Back culling flag.
  eCULL_Front,		// Front culling flag.
  eCULL_None		// No culling flag.
};

enum ERenderResource
{
  eRR_Unknown,
  eRR_Mesh,
  eRR_Texture,
  eRR_Shader,
  eRR_ShaderResource,
};

//=========================================================================

// Summary:
//	 Array Pointers for Shaders.

enum ESrcPointer
{
  eSrcPointer_Unknown,
  eSrcPointer_Vert,
  eSrcPointer_Color,
  eSrcPointer_Tex,
  eSrcPointer_TexLM,
  eSrcPointer_Normal,
  eSrcPointer_Binormal,
  eSrcPointer_Tangent,
  eSrcPointer_Max,
};

struct SWaveForm;
struct SWaveForm2;

#define FRF_REFRACTIVE 1
#define FRF_GLOW       2
#define FRF_HEAT       4
#define MAX_HEATSCALE 4

#if !defined(MAX_JOINT_AMOUNT)
#error MAX_JOINT_AMOUNT is not defined
#endif

#if (MAX_JOINT_AMOUNT<=256)
typedef uint8 JointIdType;
#else
typedef uint16 JointIdType;				
#endif

//=========================================================================

enum EParamType
{
  eType_UNKNOWN,
  eType_BYTE,
  eType_BOOL,
  eType_SHORT,
  eType_INT,
  eType_HALF,
  eType_FLOAT,
  eType_STRING,
  eType_FCOLOR,
  eType_VECTOR,
  eType_TEXTURE_HANDLE,
  eType_CAMERA,
};

union UParamVal
{
  byte m_Byte;
  bool m_Bool;
  short m_Short;
  int m_Int;
  float m_Float;
  char *m_String;
  float m_Color[4];
  float m_Vector[3];
  CCamera *m_pCamera;
};

// Note:
//	 In order to facilitate the memory allocation tracking, we're using here this class;
//	 if you don't like it, please write a substitute for all string within the project and use them everywhere.
struct SShaderParam
{
  char m_Name[32];
  EParamType m_Type;
  UParamVal m_Value;
  string m_Script;
  uint8 m_eSemantic;

  inline void Construct()
  {
    memset(&m_Value, 0, sizeof(m_Value));
    m_Type = eType_UNKNOWN;
    m_eSemantic = 0;
    m_Name[0] = 0;
  }
  inline SShaderParam()
  {
    Construct();
  }
  size_t Size()
  {
    size_t nSize = sizeof(*this);
    if (m_Type == eType_STRING)
      nSize += strlen (m_Value.m_String) + 1;

    return nSize;
  }
	void GetMemoryUsage(ICrySizer* pSizer) const
	{
		pSizer->AddObject(m_Script  );
		if (m_Type == eType_STRING)
			pSizer->AddObject( m_Value.m_String, strlen (m_Value.m_String) + 1 );
	}

  inline void Destroy()
  {
    if (m_Type == eType_STRING)
      delete [] m_Value.m_String;
  }
  inline ~SShaderParam()
  {
    Destroy();
  }
  inline SShaderParam (const SShaderParam& src)
  {
    memcpy(m_Name, src.m_Name, sizeof(m_Name));
    m_Script = src.m_Script;
    m_Type = src.m_Type;
    m_eSemantic = src.m_eSemantic;
    if (m_Type == eType_STRING)
		{
      m_Value.m_String = new char[ strlen(src.m_Value.m_String)+1 ];
      strcpy(m_Value.m_String, src.m_Value.m_String);
		}
		else
			m_Value = src.m_Value;
  }
  inline SShaderParam& operator = (const SShaderParam& src)
  {
    this->~SShaderParam();
    new(this) SShaderParam(src);
    return *this;
  }

  static bool SetParam(const char* name, DynArrayRef<SShaderParam> *Params, UParamVal& pr)
  {
    uint32 i;
    for (i=0; i<(uint32)Params->size(); i++)
    { 
      SShaderParam *sp = &(*Params)[i]; 
      if (!sp)
        continue;
      if (!stricmp(sp->m_Name, name))
      {
        switch (sp->m_Type)
        {          
          case eType_FLOAT:
            sp->m_Value.m_Float = pr.m_Float;
            break;
          case eType_SHORT:
            sp->m_Value.m_Short = pr.m_Short;
            break;
          case eType_INT:
          case eType_TEXTURE_HANDLE:
            sp->m_Value.m_Int = pr.m_Int;
            break;

          case eType_VECTOR:
            sp->m_Value.m_Vector[0] = pr.m_Vector[0];
            sp->m_Value.m_Vector[1] = pr.m_Vector[1];
            sp->m_Value.m_Vector[2] = pr.m_Vector[2];
            break;

          case eType_FCOLOR:
            sp->m_Value.m_Color[0] = pr.m_Color[0];
            sp->m_Value.m_Color[1] = pr.m_Color[1];
            sp->m_Value.m_Color[2] = pr.m_Color[2];
            sp->m_Value.m_Color[3] = pr.m_Color[3];
            break;

          case eType_STRING:
            {
              char *str = pr.m_String;
              size_t len = strlen(str)+1;
              sp->m_Value.m_String = new char [len];
              strcpy(sp->m_Value.m_String, str);
            }
            break;
        }
        break;
      }
    }
    if (i == Params->size())
      return false;
    return true;
  }
	static bool GetValue(const char* szName, DynArrayRef<SShaderParam> *Params, float *v, int nID);

	static bool GetValue(uint8 eSemantic, DynArrayRef<SShaderParam> *Params, float *v, int nID);
};


// Description:
//    IShaderPublicParams can be used to hold a collection of the shader public params.
//    Manipulate this collection, and use them during rendering by submit to the SRendParams.
UNIQUE_IFACE struct IShaderPublicParams
{
	// <interfuscator:shuffle>
	virtual ~IShaderPublicParams(){}
  virtual void AddRef() = 0;
  virtual void Release() = 0;

  // Description:
  //    Changes number of parameters in collection.
  virtual void SetParamCount( int nParam ) = 0;
  
  // Description:
  //    Retrieves number of parameters in collection.
  virtual int  GetParamCount() const = 0;

  // Description:
  //    Retrieves shader public parameter at specified index of the collection.
  virtual SShaderParam& GetParam( int nIndex ) = 0;
  virtual const SShaderParam& GetParam( int nIndex ) const = 0;

  // Description:
  //    Retrieves shader public parameter at specified index of the collection.
  virtual SShaderParam* GetParamByName( const char *pszName ) = 0;
  virtual const SShaderParam* GetParamByName( const char *pszName ) const = 0;

  virtual SShaderParam* GetParamBySemantic( uint8 eParamSemantic ) = 0;
  virtual const SShaderParam* GetParamBySemantic( uint8 eParamSemantic ) const = 0;

  // Description:
  //    Sets a shader parameter (and if doesn't exists, add it to the parameters list).
  virtual void SetParam(const char *pszName, UParamVal &pParam, EParamType nType = eType_FLOAT, uint8 eSemantic = 0) = 0;

  // Description:
  //    Assigns shader public parameter at specified index of the collection.
  virtual void SetParam( int nIndex,const SShaderParam &param ) = 0;

  // Description:
  //    Assigns existing shader parameters list.
  virtual void SetShaderParams( const DynArray<SShaderParam> &pParams) = 0;

  // Description:
  //    Adds a new shader public parameter at the end of the collection.
  virtual void AddParam( const SShaderParam &param ) = 0;

  // Description:
  //    Removes a shader public parameter
  virtual void RemoveParamByName( const char *pszName ) = 0;
  virtual void RemoveParamBySemantic( uint8 eParamSemantic ) = 0;

  // Description:
  //    Assigns collection of shader public parameters to the specified render params structure.
  virtual void AssignToRenderParams( struct SRendParams &rParams ) = 0;

  virtual uint8 GetSemanticByName( const char* pszName ) = 0;

  // Description:
  //    Gets shader parameters.
  virtual DynArray<SShaderParam> *GetShaderParams() = 0;
  virtual const DynArray<SShaderParam> *GetShaderParams() const = 0;
	// </interfuscator:shuffle>
};

//=================================================================================

class CInputLightMaterial
{
public:
  CInputLightMaterial() :
    m_Diffuse(0,0,0,0),
    m_Specular(0,0,0,0),
    m_Emission(0,0,0,0),
    m_SpecShininess(0)
  {
  }

  ColorF m_Diffuse;
  ColorF m_Specular;
  ColorF m_Emission;
  float m_SpecShininess;

  inline friend bool operator == (const CInputLightMaterial &m1, const CInputLightMaterial &m2)
  {
    if (m1.m_Diffuse == m2.m_Diffuse && m1.m_Specular == m2.m_Specular && m1.m_Emission == m2.m_Emission && m1.m_SpecShininess == m2.m_SpecShininess)
      return true;
    return false;
  }

  int Size()
  {
    int nSize = sizeof(CInputLightMaterial);
    return nSize;
  }
};

class CTexture;
#include <ITexture.h>

// Summary:
//	 Vertex modificators definitions (must be 16 bit flag).

#define MDV_BENDING            0x100
#define MDV_DET_BENDING        0x200
#define MDV_DET_BENDING_GRASS  0x400
#define MDV_WIND               0x800
#define MDV_DEPTH_OFFSET       0x2000

// Summary:
//	 Deformations/Morphing types.
enum EDeformType
{
	eDT_Unknown = 0,
	eDT_SinWave = 1,
	eDT_SinWaveUsingVtxColor = 2,
	eDT_Bulge = 3,
	eDT_Squeeze = 4,
	eDT_Perlin2D = 5,
	eDT_Perlin3D = 6,
	eDT_FromCenter = 7,
	eDT_Bending = 8,  
	eDT_ProcFlare = 9,
	eDT_AutoSprite = 10,
	eDT_Beam = 11,
	eDT_FixedOffset = 12,
};

// Summary:
//	 Wave form evaluator flags.
enum EWaveForm
{
  eWF_None,
  eWF_Sin,
  eWF_HalfSin,
  eWF_InvHalfSin,
  eWF_Square,
  eWF_Triangle,
  eWF_SawTooth,
  eWF_InvSawTooth,
  eWF_Hill,
  eWF_InvHill,
};

#define WFF_CLAMP 1
#define WFF_LERP  2

// Summary:
//	 Wave form definition.
struct SWaveForm
{
  EWaveForm m_eWFType;
  byte m_Flags;

  float m_Level;
  float m_Level1;
  float m_Amp;
  float m_Amp1;
  float m_Phase;
  float m_Phase1;
  float m_Freq;
  float m_Freq1;

  SWaveForm(EWaveForm eWFType, float fLevel, float fAmp, float fPhase, float fFreq)
  {
    m_eWFType = eWFType;
    m_Level = m_Level1 = fLevel;
    m_Amp = m_Amp1 = fAmp;
    m_Phase = m_Phase1 = fPhase;
    m_Freq = m_Freq1 = fFreq;
  }

  int Size()
  {
    int nSize = sizeof(SWaveForm);
    return nSize;
  }
  SWaveForm()
  {
    memset(this, 0, sizeof(SWaveForm));
  }
  bool operator == (const SWaveForm& wf) const
  {
    if (m_eWFType == wf.m_eWFType && m_Level == wf.m_Level && m_Amp == wf.m_Amp && m_Phase == wf.m_Phase && m_Freq == wf.m_Freq && m_Level1 == wf.m_Level1 && m_Amp1 == wf.m_Amp1 && m_Phase1 == wf.m_Phase1 && m_Freq1 == wf.m_Freq1 && m_Flags == wf.m_Flags)
      return true;
    return false;
  }

  SWaveForm& operator += (const SWaveForm& wf )
  {
    m_Level  += wf.m_Level;
    m_Level1 += wf.m_Level1;
    m_Amp  += wf.m_Amp;
    m_Amp1 += wf.m_Amp1;
    m_Phase  += wf.m_Phase;
    m_Phase1 += wf.m_Phase1;
    m_Freq  += wf.m_Freq;
    m_Freq1 += wf.m_Freq1;
    return *this;
  }
};

struct SWaveForm2
{
  EWaveForm m_eWFType;

  float m_Level;
  float m_Amp;
  float m_Phase;
  float m_Freq;

  SWaveForm2()
  {
    memset(this, 0, sizeof(SWaveForm2));
  }
  bool operator == (const SWaveForm2& wf) const
  {
    if (m_eWFType == wf.m_eWFType && m_Level == wf.m_Level && m_Amp == wf.m_Amp && m_Phase == wf.m_Phase && m_Freq == wf.m_Freq)
      return true;
    return false;
  }

  SWaveForm2& operator += (const SWaveForm2& wf )
  {
    m_Level  += wf.m_Level;
    m_Amp  += wf.m_Amp;
    m_Phase  += wf.m_Phase;
    m_Freq  += wf.m_Freq;
    return *this;
  }
};

struct SDeformInfo
{
	EDeformType m_eType;
	SWaveForm2 m_WaveX;
	SWaveForm2 m_WaveY;
	SWaveForm2 m_WaveZ;
	SWaveForm2 m_WaveW;
	float m_fDividerX;
	float m_fDividerY;
	float m_fDividerZ;
	float m_fDividerW;
	Vec3 m_vNoiseScale;

	SDeformInfo()
	{
		m_eType = eDT_Unknown;
		m_fDividerX = 0.01f;
		m_fDividerY = 0.01f;
		m_fDividerZ = 0.01f;
		m_fDividerW = 0.01f;
		m_vNoiseScale = Vec3(1,1,1);
	}

	inline bool operator == (const SDeformInfo &m)
	{
		if (m_eType == m.m_eType && 
				m_WaveX == m.m_WaveX && m_WaveY == m.m_WaveY && 
				m_WaveZ == m.m_WaveZ && m_WaveW == m.m_WaveW &&
				m_vNoiseScale == m.m_vNoiseScale &&
				m_fDividerX != m.m_fDividerX && m_fDividerY != m.m_fDividerY &&
				m_fDividerZ != m.m_fDividerZ && m_fDividerW != m.m_fDividerW )
			return true;

		return false;
	}

  int Size()
  {
    return sizeof(SDeformInfo);
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->Add( *this );
	}
};

struct SDetailDecalInfo
{  
  Vec4   vTileOffs[2];  // xy = tilling, zw = offsets
  uint16 nRotation[2];
  uint8  nThreshold[2];
  uint8  nDeformation[2];
  uint8  nBlending;
  uint8  nSSAOAmount;

  SDetailDecalInfo()
  {
    vTileOffs[0] = Vec4(1.0f, 1.0f, 0.0f, 0.0f);
    vTileOffs[1] = Vec4(0.5f, 0.5f, 0.5f, 0.5f);
    nThreshold[0] = nThreshold[1] = 0;
    nDeformation[0] = nDeformation[1] = 0;
    nRotation[0] = nRotation[1] = 0;
    nBlending = 128;
    nSSAOAmount = 0;
  }
  inline bool operator == (const SDetailDecalInfo &m)
  {
    if (vTileOffs[0].x == m.vTileOffs[0].x && vTileOffs[0].y == m.vTileOffs[0].y &&
        vTileOffs[0].z == m.vTileOffs[0].z && vTileOffs[0].w == m.vTileOffs[0].w &&
        vTileOffs[1].x == m.vTileOffs[1].x && vTileOffs[1].y == m.vTileOffs[1].y &&
        vTileOffs[1].z == m.vTileOffs[1].z && vTileOffs[1].w == m.vTileOffs[1].w &&
        nDeformation[0] == m.nDeformation[0] && nDeformation[1] != m.nDeformation[1] &&
        nRotation[0] == m.nRotation[0] && nRotation[1] != m.nRotation[1] &&
        nThreshold[0] != m.nThreshold[0] && nThreshold[1] != m.nThreshold[1] &&
        nBlending != m.nBlending && nSSAOAmount != m.nSSAOAmount)
      return true;
    return false;
  }

  void Reset()
  {
    vTileOffs[0] = Vec4(1.0f, 1.0f, 0.0f, 0.0f);
    vTileOffs[1] = Vec4(0.5f, 0.5f, 0.5f, 0.5f);
    nThreshold[0] = nThreshold[1] = 0;
    nDeformation[0] = nDeformation[1] = 0;
    nRotation[0] = nRotation[1] = 0;
    nBlending = 128;
    nSSAOAmount = 0;
  }

  int Size()
  {
    return sizeof(SDetailDecalInfo);
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->Add( *this );
	}
};

//==============================================================================
// CRenderObject

//////////////////////////////////////////////////////////////////////
// CRenderObject::m_ObjFlags: Flags used by shader pipeline

enum ERenderObjectFlags
{
	FOB_TRANS_ROTATE                = BIT(0),
	FOB_TRANS_SCALE                 = BIT(1),
	FOB_TRANS_TRANSLATE             = BIT(2),
	FOB_RENDER_AFTER_POSTPROCESSING = BIT(3),
	FOB_OWNER_GEOMETRY              = BIT(4),
	FOB_MESH_SUBSET_INDICES         = BIT(5),
	FOB_SELECTED                    = BIT(6),
	FOB_RENDERER_IDENDITY_OBJECT    = BIT(7),
	FOB_GLOBAL_ILLUMINATION         = BIT(8),
	FOB_NO_FOG                      = BIT(9),
	FOB_DECAL                       = BIT(10),
	FOB_OCTAGONAL                   = BIT(11),
	FOB_BLEND_WITH_TERRAIN_COLOR    = BIT(12),
	FOB_POINT_SPRITE                = BIT(13),
	FOB_SOFT_PARTICLE               = BIT(14),
	FOB_REQUIRES_RESOLVE            = BIT(15),
	FOB_UPDATED_RTMASK              = BIT(16),
	FOB_AFTER_WATER                 = BIT(17),
	FOB_BENDED                      = BIT(18),
	FOB_ZPREPASS                    = BIT(19),
	FOB_INSHADOW                    = BIT(20),
	FOB_DISSOLVE                    = BIT(21),
	FOB_MOTION_BLUR                 = BIT(22),
	FOB_NEAREST                     = BIT(23), // [Rendered in Camera Space]
	FOB_SKINNED                     = BIT(24),
	FOB_DISSOLVE_OUT                = BIT(25),
	FOB_DYNAMIC_OBJECT              = BIT(26),
	FOB_ALLOW_TESSELLATION          = BIT(27),
	FOB_DECAL_TEXGEN_2D             = BIT(28),
	FOB_IN_DOORS                    = BIT(29),
	FOB_HAS_PREVMATRIX              = BIT(30),
	FOB_LIGHTVOLUME                 = BIT(31),

	FOB_TRANS_MASK = (FOB_TRANS_ROTATE | FOB_TRANS_SCALE | FOB_TRANS_TRANSLATE),
	FOB_DECAL_MASK =  (FOB_DECAL | FOB_DECAL_TEXGEN_2D),
	FOB_PARTICLE_MASK = (FOB_SOFT_PARTICLE | FOB_NO_FOG | FOB_GLOBAL_ILLUMINATION | FOB_INSHADOW | FOB_NEAREST | FOB_MOTION_BLUR | FOB_LIGHTVOLUME | FOB_ALLOW_TESSELLATION | FOB_IN_DOORS | FOB_AFTER_WATER),

	// WARNING: FOB_MASK_AFFECTS_MERGING must start from 0x10000/bit 16 (important for instancing).
	FOB_MASK_AFFECTS_MERGING_GEOM  = (FOB_ZPREPASS | FOB_SKINNED | FOB_BENDED | FOB_DYNAMIC_OBJECT | FOB_ALLOW_TESSELLATION | FOB_NEAREST),
	FOB_MASK_AFFECTS_MERGING = (FOB_ZPREPASS | FOB_MOTION_BLUR | FOB_HAS_PREVMATRIX | FOB_SKINNED | FOB_BENDED | FOB_INSHADOW | FOB_AFTER_WATER | FOB_DISSOLVE | FOB_DISSOLVE_OUT | FOB_NEAREST | FOB_DYNAMIC_OBJECT | FOB_ALLOW_TESSELLATION)
};


struct SInstanceInfo
{
  Matrix34A m_Matrix;
  ColorF m_AmbColor;
};

struct SSkyInfo
{
	ITexture *m_SkyBox[3];
	float m_fSkyLayerHeight;

	int Size()
	{
		int nSize = sizeof(SSkyInfo);
		return nSize;
	}
	SSkyInfo()
	{
		memset(this, 0, sizeof(SSkyInfo));
	}
};

struct SBending
{
  Vec2 m_vBending;
  float m_fMainBendingScale;
  SWaveForm2 m_Waves[2];

  SBending()
  {
    m_vBending.zero();
    m_fMainBendingScale = 1.f;
  }
};

// Description:
//	 Interface for the skinnable objects (renderer calls its functions to get the skinning data).
// should only created by EF_CreateSkinningData
_MS_ALIGN(16) struct SSkinningData
{
	uint32 									nNumBones;				
	uint32 									nHWSkinningFlags;
	DualQuat* 							pBoneQuatsS;
	JointIdType* 						pRemapTable;
	JobManager::SJobState*	pAsyncJobs;
	SSkinningData*					pPreviousSkinningRenderData; // used for motion blur	
	// members below are for Software Skinning
	void*										pCustomData; // client specific data, used for example for sw-skinning on animation side	
	SSkinningData**					pMasterSkinningDataList;	// used by the SkinningData for a Character Instance, contains a list of all Skin Instances which need SW-Skinning
	SSkinningData*					pNextSkinningData;				// List to the next element which needs SW-Skinning
} _ALIGN(16);

struct SRenderObjData
{
	union
	{
		SLockFreeSingleLinkedListEntry m_LinkedListEntry;						// freelist object, reuse memory of m_fAlpha, needs to be the first element		
		void							*m_pInstance;					
	};

	CRendElementBase	*m_pRE;
	SSkinningData	*m_pSkinningData;
	TArray<Vec4>	m_Constants;

	float	m_fTempVars[10];									// Different useful vars (ObjVal component in shaders)

	// using a pointer, the client code has to ensure that the data stays valid
	const DynArray<SShaderParam> * m_pShaderParams; 

	uint16 m_nLightID[RT_COMMAND_BUF_COUNT];

	uint32	m_nVisionParams;
	uint32	m_nHUDSilhouetteParams;

	uint32	m_pLayerEffectParams; // only used for layer effects

	uint64 m_nSubObjHideMask;

	uint64 m_ShadowCasters;          // Mask of shadow casters.

	union
	{
		SBending* m_pBending;
		const struct ParticleParams* m_pParticleParams;		// Obtain shader consts directly from here, no need to store in TempVars
	};

	uint16	m_FogVolumeContribIdx[2];

	uint16	m_scissorX;
	uint16	m_scissorY;

	uint16	m_scissorWidth;
	uint16	m_scissorHeight;

	uint16	m_LightVolumeId[2];

	uint8 m_screenBounds[4];

	uint16	m_nCustomFlags;
	uint8		m_nCustomData;
	
	uint8	m_nObjID;
	uint8	m_nVisionScale;

  SRenderObjData()
  {
    Init();
  }

	void Init()
	{
		m_nSubObjHideMask = 0;		
		m_Constants.Free();
		//m_pInstancingInfo = NULL;
		m_pInstance = NULL;
		m_nVisionScale = 1;
    m_nVisionParams = 0;
		m_pRE = NULL;	
		m_pLayerEffectParams = 0;
		m_nLightID[0] = m_nLightID[1] = 0;
		m_LightVolumeId[0] = m_LightVolumeId[1] = 0;
		m_pSkinningData = NULL;
		m_scissorX = m_scissorY = m_scissorWidth = m_scissorHeight = 0;
		m_screenBounds[0] = m_screenBounds[1] = m_screenBounds[2] = m_screenBounds[3] = 0;
		m_nObjID = 0;
		m_nCustomData = 0;
		m_nCustomFlags = 0;
		m_pLayerEffectParams = m_nHUDSilhouetteParams = m_nVisionParams = 0;
		m_ShadowCasters = 0;
		m_pBending = NULL;
		m_pShaderParams = NULL;
	}

	void SetShaderParams(const DynArray<SShaderParam> *pShaderParams)
	{
		m_pShaderParams = pShaderParams;
	}

	void GetMemoryUsage(ICrySizer *pSizer) const
	{
		pSizer->AddObject(m_Constants);
	}
}_ALIGN(16);

//////////////////////////////////////////////////////////////////////
// Objects using in shader pipeline


// Summary:
//	 Same as in the 3dEngine.
#define MAX_LIGHTS_NUM 32

struct ShadowMapFrustum;

// Description:
//	 Size of CRenderObject currently is 128 bytes. 
// Note:
//	 Don't change it's size please!
class CRenderObject
{
public:
  CRenderObject()
  {
    //m_ShaderParams = NULL;
#ifdef XENON
    XMemSet(this, 0, sizeof(CRenderObject));
    m_nRenderQuality = 65535;
    m_nObjDataId = -1;

    m_nCBID = -1;
#else
    m_nRenderQuality = 65535;
    m_nRTMask = 0;
    m_nObjDataId = -1;
    m_fSort = 0;

    m_nVisAreaStencilRef = 0;
    m_nMaterialLayers = 0;
    m_pRenderNode = NULL;

		m_nMDV = 0;
    m_ObjFlags = 0;
    m_pCurrMaterial = NULL;
    m_DissolveRef = 0;
    m_nCBID = -1;
		m_bHasShadowCasters = false;
#endif
  }

  ~CRenderObject();

	// trying to avoid L2 cache misses, keep most accessed data in first cache line
	union
	{
		SLockFreeSingleLinkedListEntry m_LinkedListEntry;						// freelist object, reuse memory of m_fAlpha, needs to be the first element
		float                       m_fAlpha;						// Object alpha.	
	};
	uint32                      m_ObjFlags;
	uint32                      m_Id;

	float											  m_fDistance;					// Distance to the object.			

	//Moving matrix down by 16 bytes to allow m_fSort and m_fDistance to have spatial locality to avoid cache misses
	//	in mfSortByDist
	union
	{
		float					  m_fSort;						// Custom sort value.
		uint16          m_nSort;
	};

	uint16                      m_nMDV; 	            		// Vertex modificator flags.  	
	uint16										  m_nRenderQuality;				// 65535 - full quality, 0 - lowest quality, used by CStatObj
	int16												m_nTextureID;						// Custom texture id.
	int16                       m_nCBID;      				// Constant buffer id.

	uint8												m_nLod;	           
  uint8                       m_nVisAreaStencilRef;     // Per instance vis area stencil reference ID
  uint8												m_DissolveRef;								//
	uint8		  									m_RState;									//	

  SInstanceInfo               m_II;  // Must be aligned by 16 from begining of CRenderObject!
  
	uint32											m_DynLMMask[RT_COMMAND_BUF_COUNT];
  uint32                      m_nMaterialLayers;          // Which mtl layers active and how much to blend them  
  uint32                      m_nRTMask;
	bool                        m_bHasShadowCasters;  // Has non-empty list of lights casting shadows in render object data

	void        *               m_pRenderNode;				// Will define instance id.
	IMaterial   *               m_pCurrMaterial;				// Current material (is this available somewhere ??).
private:
  int32                       m_nObjDataId;
public:

  //=========================================================================================================

  inline Vec3 GetTranslation() const
  {
    return m_II.m_Matrix.GetTranslation();
  }
  inline float GetScaleX() const
  {
    return cry_sqrtf(m_II.m_Matrix(0,0)*m_II.m_Matrix(0,0) + m_II.m_Matrix(0,1)*m_II.m_Matrix(0,1) + m_II.m_Matrix(0,2)*m_II.m_Matrix(0,2));
  }
  inline float GetScaleZ() const
  {
    return cry_sqrtf(m_II.m_Matrix(2,0)*m_II.m_Matrix(2,0) + m_II.m_Matrix(2,1)*m_II.m_Matrix(2,1) + m_II.m_Matrix(2,2)*m_II.m_Matrix(2,2));
  }

  static CThreadSafeRendererContainer<SRenderObjData> m_sObjData[RT_COMMAND_BUF_COUNT];
	static SRenderObjData *m_pIdendityRenderObjData;

	void Init(int nThreadID)
	{
		m_ObjFlags = 0;
		m_nObjDataId = -1;
		m_nRenderQuality = 65535;
		m_DynLMMask[nThreadID] = 0;
		m_RState = 0;
		m_fDistance = 0.0f;

		m_nVisAreaStencilRef = 0;
		m_nMaterialLayers = 0;
		m_DissolveRef = 0;

		m_nMDV = 0;
		m_fSort = 0;

		m_II.m_AmbColor = Col_White;
		m_fAlpha = 1.0f;
		m_nTextureID = -1;
		m_nCBID = -1;
		m_pCurrMaterial = NULL;

		m_nRTMask = 0;
		m_bHasShadowCasters = false;

		m_pRenderNode = NULL;
	}

	void AssignId(int id)
	{
		m_Id = id;
	}

  void CloneObject(CRenderObject *srcObj)
  {
    int Id = m_Id;
    memcpy(this, srcObj, sizeof(*srcObj));
    m_Id = Id;
  }

  ILINE Matrix34A &GetMatrix()
  {
    return m_II.m_Matrix;
  }  

  ILINE SRenderObjData *GetObjData(int nProcessID)
  {
		IF( (m_ObjFlags & FOB_RENDERER_IDENDITY_OBJECT) != 0, 0 )
			return CRenderObject::m_pIdendityRenderObjData;

    IF( m_nObjDataId < 0, 0 )
			return NULL;

    return &m_sObjData[nProcessID][m_nObjDataId];
  }

  ILINE int16 GetObjDataId() 
  {
    return m_nObjDataId; 
  }

  ILINE void SetObjDataId(int16 newObjDataId) 
  {
    m_nObjDataId = newObjDataId; 
  }

  void CopyTo(CRenderObject *pObjNew, int nThreadID);

  ILINE CRendElementBase *GetRE(int nProcessID)
  {
    return m_nObjDataId>=0 ? GetObjData(nProcessID)->m_pRE : NULL;
  }

	void GetMemoryUsage(ICrySizer *pSizer) const{}

  /*ILINE DynArray16<SInstanceInfo> *GetInstanceInfo(int nProcessID)
  {
    return m_nObjDataId>=0 ? GetObjData(nProcessID)->m_pInstancingInfo : NULL;
  }*/

#ifndef PS3
  void* operator new( size_t Size )
  {
    return CryModuleMemalign(Size, 16);
  }
  void* operator new(size_t Size, const std::nothrow_t &nothrow)
  {
    return CryModuleMemalign(Size, 16);
  }
	void* operator new(size_t Size, CRenderObject *pPtr)
  {		
		 return pPtr;
  }
  void* operator new[](size_t Size)
  {		
		 return CryModuleMemalign(Size, 16);
  }
  void* operator new[](size_t Size, const std::nothrow_t &nothrow)
  {
     return CryModuleMemalign(Size, 16);
  }

  void operator delete( void *Ptr )
  {
    CryModuleMemalignFree(Ptr);
  }

  void operator delete[]( void *Ptr )
  {
		CryModuleMemalignFree(Ptr);
  }

	void operator delete( void *pPtr1, CRenderObject *pPtr2 ) {}
#endif //PS3

private:
	// Disallow copy (potential bugs with PERMANENT objects)
	// alwasy use IRendeer::EF_DuplicateRO if you want a copy
	// of a CRenderObject
	CRenderObject(const CRenderObject& other);
  CRenderObject& operator= (const CRenderObject& other);
  
} _ALIGN(16);

struct SRenderObjectModifier
{
  SRenderObjectModifier() { memset(this, 0, sizeof(*this)); }

	SRenderObjectModifier(const SRenderObjectModifier* pROM, const Matrix34& newMat, 
												const Matrix34& newPrevMat, IRenderMesh* const pNewWeights)
	: mat(newMat)
	, prev_mat(newPrevMat)
	, pWeights(pNewWeights)
	,	nSubObjId(0)
	, nMatricesInUse(1)
	, nStatesInUse(pROM?pROM->nStatesInUse:0)
	, fDistance(pROM?pROM->fDistance:0.0f)
	{}

	SRenderObjectModifier(const SRenderObjectModifier* pROM, const Matrix34& newMat, 
		const Matrix34& newPrevMat, IRenderMesh* const pNewWeights, uint8 nNewSubObjId)
		: mat(newMat)
		, prev_mat(newPrevMat)
		, pWeights(pNewWeights)
		,	nSubObjId(nNewSubObjId)
		, nMatricesInUse(1)
		, nStatesInUse(pROM?pROM->nStatesInUse:0)
		, fDistance(pROM?pROM->fDistance:0.0f)
	{}

  const bool InUse() const { return nStatesInUse || nMatricesInUse; }

  Matrix34 mat;
  Matrix34 prev_mat;
  IRenderMesh * pWeights;
	uint8 nSubObjId;
  byte nMatricesInUse;
  byte nStatesInUse;
  float fDistance;
} _ALIGN(16);

// namespace to contain function which allow caching CRenderObject and SRenderObjData on SPU local storage
// to improve simulation time. This cache needs to be flushed when the job exists
// on non SPU platforms, the original pointer is returned
namespace SPURenderObjectsCache
{
	enum { nRenderObjectCacheSize = 5 };
	enum { nRenderObjDataCacheSize = 5 };
	enum { nRenderObjectModifierCacheSize = 5 };
}

#if defined(__SPU__)
extern SPU_LOCAL char sRenderObjectCacheStorage[SPURenderObjectsCache::nRenderObjectCacheSize][sizeof(CRenderObject)] _ALIGN(128);
extern SPU_LOCAL char sRenderObjDataCacheStorage[SPURenderObjectsCache::nRenderObjDataCacheSize][sizeof(SRenderObjData)] _ALIGN(128);
extern SPU_LOCAL char sRenderObjModifierCacheStorage[SPURenderObjectsCache::nRenderObjectModifierCacheSize][sizeof(SRenderObjectModifier)] _ALIGN(128);

extern SPU_LOCAL CRenderObject*		sRenderObjectCacheUsed[SPURenderObjectsCache::nRenderObjectCacheSize];
extern SPU_LOCAL SRenderObjData*	sRenderObjDataCacheUsed[SPURenderObjectsCache::nRenderObjDataCacheSize];
extern SPU_LOCAL SRenderObjectModifier*	sRenderObjModifierCacheUsed[SPURenderObjectsCache::nRenderObjectModifierCacheSize];
#endif

namespace SPURenderObjectsCache
{
	/////////////////////////////////////////////////////////////////////////////
	// forward declaration of RenderObjectsCache function
	inline SPU_DOMAIN_LOCAL CRenderObject* CacheObject( CRenderObject *pObj );
	inline SPU_DOMAIN_LOCAL SRenderObjData* CacheObject( SRenderObjData *pObj );
	inline SPU_DOMAIN_LOCAL SRenderObjectModifier* CacheObject( SRenderObjectModifier *pObj );
	inline void FlushRenderObject(int nSlot);
	inline void FlushRenderObjData(int nSlot);
	inline void FlushRenderObjModifier(int nSlot);
	inline void FlushRenderObject(CRenderObject *pObj);
	inline void FlushRenderObjData(SRenderObjData *pObj);
	inline void FlushRenderObjModifier(SRenderObjectModifier *pObj);
	inline void FlushCache();
	inline void ClearRenderObjectCache();
	inline void ClearRenderObjectDataCache();
	inline void ClearRenderObjectModifierCache();
	inline void ClearCache();

	/////////////////////////////////////////////////////////////////////////////
	// implementation of RenderObjectsCache function
	inline SPU_DOMAIN_LOCAL CRenderObject* CacheObject( CRenderObject *pObj )
	{
#if defined(__SPU__)		
		if( pObj == NULL )
			return SPU_CONST_LOCAL_PTR(NULL);

		// first check if we have this object already
		for( int i = 0 ; i < nRenderObjectCacheSize ; ++i )
		{
			if( sRenderObjectCacheUsed[i] == pObj )
				return reinterpret_cast<CRenderObject*>(sRenderObjectCacheStorage[i]);
			if( reinterpret_cast<CRenderObject*>(sRenderObjectCacheStorage[i]) == pObj )
				return SPU_LOCAL_PTR(pObj);
		}

		for( int i = 0 ; i < nRenderObjectCacheSize ; ++i )
		{
			if( sRenderObjectCacheUsed[i] == NULL )
			{
				if((int)pObj < 256*1024) snPause();
				memtransfer_from_main( SPU_LOCAL_PTR(sRenderObjectCacheStorage[i]), SPU_MAIN_PTR(pObj), sizeof(CRenderObject), 0 );
				memtransfer_sync(0);
				sRenderObjectCacheUsed[i] = pObj;
				return reinterpret_cast<CRenderObject*>(sRenderObjectCacheStorage[i]);
			}
		}

		snPause();
		return NULL;			
#else
		return pObj;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline SPU_DOMAIN_LOCAL SRenderObjData* CacheObject( SRenderObjData *pObj )
	{
#if defined(__SPU__)
		if( pObj == NULL )
			return SPU_CONST_LOCAL_PTR(NULL);

		// first check if we have this object already
		for( int i = 0 ; i < nRenderObjDataCacheSize ; ++i )
		{
			if( sRenderObjDataCacheUsed[i] == pObj )			
				return reinterpret_cast<SRenderObjData*>(sRenderObjDataCacheStorage[i]);
			if( reinterpret_cast<SRenderObjData*>(sRenderObjDataCacheStorage[i]) == pObj )			
				return SPU_LOCAL_PTR(pObj);
		}

		for( int i = 0 ; i < nRenderObjDataCacheSize ; ++i )
		{
			if( sRenderObjDataCacheUsed[i] == NULL )
			{
				if((int)pObj < 256*1024) snPause();
				memtransfer_from_main( SPU_LOCAL_PTR(sRenderObjDataCacheStorage[i]), SPU_MAIN_PTR(pObj), sizeof(SRenderObjData), 0 );
				memtransfer_sync(0);
				sRenderObjDataCacheUsed[i] = pObj;
				return reinterpret_cast<SRenderObjData*>(sRenderObjDataCacheStorage[i]);
			}
		}

		snPause();
		return NULL;		
#else
		return pObj;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline SPU_DOMAIN_LOCAL SRenderObjectModifier* CacheObject( SRenderObjectModifier *pObj )
	{
#if defined(__SPU__)
		if( pObj == NULL )
			return SPU_CONST_LOCAL_PTR(NULL);

		// first check if we have this object already
		for( int i = 0 ; i < nRenderObjectModifierCacheSize ; ++i )
		{
			if( sRenderObjModifierCacheUsed[i] == pObj )			
				return reinterpret_cast<SRenderObjectModifier*>(sRenderObjModifierCacheStorage[i]);
			if( reinterpret_cast<SRenderObjectModifier*>(sRenderObjModifierCacheStorage[i]) == pObj )			
				return SPU_LOCAL_PTR(pObj);
		}

		for( int i = 0 ; i < nRenderObjectModifierCacheSize ; ++i )
		{
			if( sRenderObjModifierCacheUsed[i] == NULL )
			{
				if((int)pObj < 256*1024) snPause();
				memtransfer_from_main( SPU_LOCAL_PTR(sRenderObjModifierCacheStorage[i]), SPU_MAIN_PTR(pObj), sizeof(SRenderObjectModifier), 0 );
				memtransfer_sync(0);
				sRenderObjModifierCacheUsed[i] = pObj;
				return reinterpret_cast<SRenderObjectModifier*>(sRenderObjModifierCacheStorage[i]);
			}
		}
		
		snPause();
		return NULL;		
#else
		return pObj;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline CRenderObject* ResolveCacheObject( CRenderObject *pObj )
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectCacheSize ; ++i )
		{
			if(pObj == reinterpret_cast<CRenderObject*>(&sRenderObjectCacheStorage[i][0]))
				return sRenderObjectCacheUsed[i];
		}
		if(pObj!=NULL)
			snPause();
		return NULL;
#else
		return pObj;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline SRenderObjData* ResolveCacheObject( SRenderObjData *pObj )
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjDataCacheSize ; ++i )
		{
			if(pObj == reinterpret_cast<SRenderObjData*>(&sRenderObjDataCacheStorage[i][0]))
				return sRenderObjDataCacheUsed[i];
		}		
		if(pObj!=NULL)
			snPause();
		return NULL;
#else
		return pObj;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline SRenderObjectModifier* ResolveCacheObject( SRenderObjectModifier *pObj )
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectModifierCacheSize ; ++i )
		{
			if(pObj == reinterpret_cast<SRenderObjectModifier*>(&sRenderObjModifierCacheStorage[i][0]))
				return sRenderObjModifierCacheUsed[i];
		}		
		if(pObj!=NULL)
			snPause();
		return NULL;
#else
		return pObj;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushRenderObject(int nSlot)
	{
#if defined(__SPU__)
		if(sRenderObjectCacheUsed[nSlot] != NULL)
		{
			memtransfer_to_main( SPU_MAIN_PTR(sRenderObjectCacheUsed[nSlot]), sRenderObjectCacheStorage[nSlot], sizeof(CRenderObject), 0 );
			memtransfer_sync(0);			
		}
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushRenderObjData(int nSlot)
	{
#if defined(__SPU__)
		if(sRenderObjDataCacheUsed[nSlot] != NULL)
		{
			memtransfer_to_main( SPU_MAIN_PTR(sRenderObjDataCacheUsed[nSlot]), sRenderObjDataCacheStorage[nSlot], sizeof(SRenderObjData), 0 );
			memtransfer_sync(0);			
		}
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushRenderObjModifier(int nSlot)
	{
#if defined(__SPU__)
		if(sRenderObjModifierCacheUsed[nSlot] != NULL)
		{
			memtransfer_to_main( SPU_MAIN_PTR(sRenderObjModifierCacheUsed[nSlot]), sRenderObjModifierCacheStorage[nSlot], sizeof(SRenderObjectModifier), 0 );
			memtransfer_sync(0);			
		}
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushRenderObject(CRenderObject *pObj)
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectCacheSize ; ++i )
		{
			// flush if we got the main memory address or the SPU local address
			if(sRenderObjectCacheUsed[i] == pObj ||  reinterpret_cast<CRenderObject*>(&sRenderObjectCacheStorage[i]) == pObj )
			{
				FlushRenderObject(i);
				return;
			}
		}		
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushRenderObjData(SRenderObjData *pObj)
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjDataCacheSize ; ++i )
		{
			// flush if we got the main memory address or the SPU local address
			if(sRenderObjDataCacheUsed[i] == pObj || reinterpret_cast<SRenderObjData*>(&sRenderObjDataCacheStorage[i]) == pObj )
			{
				FlushRenderObjData(i);
				return;
			}
		}		
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushRenderObjModifier(SRenderObjectModifier *pObj)
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectModifierCacheSize ; ++i )
		{
			// flush if we got the main memory address or the SPU local address
			if(sRenderObjModifierCacheUsed[i] == pObj || reinterpret_cast<SRenderObjectModifier*>(&sRenderObjModifierCacheStorage[i]) == pObj )
			{
				FlushRenderObjModifier(i);
				return;
			}
		}		
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void FlushCache()
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectCacheSize ; ++i )
			FlushRenderObject(i);
		for( int i = 0 ; i < nRenderObjDataCacheSize ; ++i )
			FlushRenderObjData(i);
		for( int i = 0 ; i < nRenderObjectModifierCacheSize ; ++i )
			FlushRenderObjModifier(i);
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void ClearRenderObjectCache()
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectCacheSize ; ++i )
			sRenderObjectCacheUsed[i] = NULL;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void ClearRenderObjectDataCache()
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjDataCacheSize ; ++i )
			sRenderObjDataCacheUsed[i] = NULL;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void ClearRenderObjectModifierCache()
	{
#if defined(__SPU__)
		for( int i = 0 ; i < nRenderObjectModifierCacheSize ; ++i )
			sRenderObjModifierCacheUsed[i] = NULL;
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	inline void ClearCache()
	{
#if defined(__SPU__)
		FlushCache();
		ClearRenderObjectCache();
		ClearRenderObjectDataCache();
		ClearRenderObjectModifierCache();
#endif
	}

	/////////////////////////////////////////////////////////////////////////////
	// util class to flush the cache at function exit
	class ScopedFlushCache
	{
	public:
		ScopedFlushCache(){}
		~ScopedFlushCache()
		{
			FlushCache();
		}
	};
} //namespace SPURenderObjectsCache



enum EResClassName
{
  eRCN_Texture,
  eRCN_Shader,
};

// className: CTexture, CHWShader_VS, CHWShader_PS, CShader
struct SResourceAsync
{
  int nReady;            // 0: Not ready; 1: Ready; -1: Error
  byte *pData;
  EResClassName eClassName;       // Resource class name
  char *Name;            // Resource name
  union
  {
    // CTexture parameters
		struct { int nWidth, nHeight, nMips, nTexFlags, nFormat, nTexId; };
    // CShader parameters
    struct { int nShaderFlags; };
  };
  void *pResource; // Pointer to created resource

  SResourceAsync()
  {
    memset(this, 0, sizeof(SResourceAsync));
  }

  ~SResourceAsync()
  {
    delete Name;
  }  
};

//==============================================================================

// Summary:
//	 Color operations flags.
enum EColorOp
{
  eCO_NOSET = 0,
  eCO_DISABLE = 1,
  eCO_REPLACE = 2,
  eCO_DECAL = 3,
  eCO_ARG2 = 4,
  eCO_MODULATE = 5,
  eCO_MODULATE2X = 6,
  eCO_MODULATE4X = 7,
  eCO_BLENDDIFFUSEALPHA = 8,
  eCO_BLENDTEXTUREALPHA = 9,
  eCO_DETAIL = 10,
  eCO_ADD = 11,
  eCO_ADDSIGNED = 12,
  eCO_ADDSIGNED2X = 13,
  eCO_MULTIPLYADD = 14,
  eCO_BUMPENVMAP = 15,
  eCO_BLEND = 16,
  eCO_MODULATEALPHA_ADDCOLOR = 17,
  eCO_MODULATECOLOR_ADDALPHA = 18,
  eCO_MODULATEINVALPHA_ADDCOLOR = 19,
  eCO_MODULATEINVCOLOR_ADDALPHA = 20,
  eCO_DOTPRODUCT3 = 21,
  eCO_LERP = 22,
  eCO_SUBTRACT = 23,
};

enum EColorArg
{
  eCA_Unknown,
  eCA_Specular,
  eCA_Texture,
  eCA_Texture1,
  eCA_Normal,
  eCA_Diffuse,
  eCA_Previous,
  eCA_Constant,
};

#define DEF_TEXARG0 (eCA_Texture|(eCA_Diffuse<<3))
#define DEF_TEXARG1 (eCA_Texture|(eCA_Previous<<3))

enum ETexModRotateType
{
  ETMR_NoChange,
  ETMR_Fixed,
  ETMR_Constant,
  ETMR_Oscillated,
  ETMR_Max
};

enum ETexModMoveType
{
  ETMM_NoChange,
  ETMM_Fixed,
  ETMM_Constant,
  ETMM_Jitter,
  ETMM_Pan,
  ETMM_Stretch,
  ETMM_StretchRepeat,
  ETMM_Max
};

enum ETexGenType
{
  ETG_Stream,
  ETG_World,
  ETG_Camera,
  ETG_WorldEnvMap,
  ETG_CameraEnvMap,
  ETG_NormalMap,
  ETG_SphereMap,
  ETG_Max
};

#define CASE_TEXMOD(var_name)\
  if(!stricmp(#var_name,szParamName))\
  {\
    var_name = fValue;\
    return true;\
  }\

#define CASE_TEXMODANGLE(var_name)\
  if(!stricmp(#var_name,szParamName))\
{\
  var_name = Degr2Word(fValue);\
  return true;\
}\

#define CASE_TEXMODBYTE(var_name)\
  if(!stricmp(#var_name,szParamName))\
{\
  var_name = (byte)fValue;\
  return true;\
}\

#define CASE_TEXMODBOOL(var_name)\
  if(!stricmp(#var_name,szParamName))\
{\
  var_name = (fValue==1.f);\
  return true;\
}\

struct SEfTexModificator
{
  bool SetMember(const char * szParamName, float fValue)
  {
    CASE_TEXMODBYTE(m_eTGType);
    CASE_TEXMODBYTE(m_eRotType);
    CASE_TEXMODBYTE(m_eUMoveType);
    CASE_TEXMODBYTE(m_eVMoveType);
    CASE_TEXMODBOOL(m_bTexGenProjected);

    CASE_TEXMOD(m_Tiling[0]);
    CASE_TEXMOD(m_Tiling[1]);
    CASE_TEXMOD(m_Tiling[2]);
    CASE_TEXMOD(m_Offs[0]);
    CASE_TEXMOD(m_Offs[1]);
    CASE_TEXMOD(m_Offs[2]);

    CASE_TEXMODANGLE(m_Rot[0]);
    CASE_TEXMODANGLE(m_Rot[1]);
    CASE_TEXMODANGLE(m_Rot[2]);
    CASE_TEXMODANGLE(m_RotOscRate[0]);
    CASE_TEXMODANGLE(m_RotOscRate[1]);
    CASE_TEXMODANGLE(m_RotOscRate[2]);
    CASE_TEXMODANGLE(m_RotOscAmplitude[0]);
    CASE_TEXMODANGLE(m_RotOscAmplitude[1]);
    CASE_TEXMODANGLE(m_RotOscAmplitude[2]);
    CASE_TEXMODANGLE(m_RotOscPhase[0]);
    CASE_TEXMODANGLE(m_RotOscPhase[1]);
    CASE_TEXMODANGLE(m_RotOscPhase[2]);
    CASE_TEXMOD(m_RotOscCenter[0]);
    CASE_TEXMOD(m_RotOscCenter[1]);
    CASE_TEXMOD(m_RotOscCenter[2]);

    CASE_TEXMOD(m_UOscRate);
    CASE_TEXMOD(m_VOscRate);
    CASE_TEXMOD(m_UOscAmplitude);
    CASE_TEXMOD(m_VOscAmplitude);
    CASE_TEXMOD(m_UOscPhase);
    CASE_TEXMOD(m_VOscPhase);

    return false;
  }

  bool m_bTexGenProjected;
  uint8 m_eTGType;
  uint8 m_eRotType;
  uint8 m_eUMoveType;
  uint8 m_eVMoveType;
  uint16 m_Rot[3];
  Matrix44 m_TexGenMatrix;
  Matrix44 m_TexMatrix;

  float m_Tiling[3];
  float m_Offs[3];

  uint16 m_RotOscRate[3];
  uint16 m_RotOscAmplitude[3];
  float m_RotOscCenter[3];

  float m_UOscRate;
  float m_VOscRate;
  float m_UOscAmplitude;
  float m_VOscAmplitude;
  float m_UOscPhase;
  float m_VOscPhase;

  // This members are used only during updating of the matrices
  float m_LastUTime;
  float m_LastVTime;
  float m_CurrentUJitter;
  float m_CurrentVJitter;

  uint16 m_RotOscPhase[3];
	
	inline SEfTexModificator()
  {
    memset(this, 0, sizeof(*this));
    m_Tiling[0] = m_Tiling[1] = 1.0f;
  }
	inline SEfTexModificator(const SEfTexModificator &m)
	{
		if (&m != this)
		{
			memcpy(this, &m, sizeof(*this));
		}
	}
  SEfTexModificator& operator = (const SEfTexModificator& src)
  {
    if (&src != this)
    {
      this->~SEfTexModificator();
      new(this) SEfTexModificator(src);
    }
    return *this;
  }
  int Size()
  {
    return sizeof(*this);
  }

  inline bool operator != (const SEfTexModificator &m)
  {
		return memcmp(this, &m, sizeof(*this)) != 0;
  }
#ifdef PS3
} ALIGN16;
#else
};
#endif


struct STexState
{
	struct
	{
		signed char m_nMinFilter:8;
		signed char m_nMagFilter:8;
		signed char m_nMipFilter:8;
		signed char m_nAddressU:8;
		signed char m_nAddressV:8;
		signed char m_nAddressW:8;
		signed char m_nAnisotropy:8;
		signed char padding:8;
	};
	DWORD  m_dwBorderColor;
	void *m_pDeviceState;
  bool m_bActive;
  bool m_bComparison;
  bool m_bSRGBLookup;
	byte m_bPAD;

  STexState ()
  {
    m_nMinFilter = 0;
    m_nMagFilter = 0;
    m_nMipFilter = 0;
    m_nAnisotropy = 0;
    m_nAddressU = 0;
    m_nAddressV = 0;
    m_nAddressW = 0;
    m_dwBorderColor = 0;
		padding = 0;
    m_bSRGBLookup = false;
    m_bActive = false;
    m_bComparison = false;
    m_pDeviceState = NULL;
		m_bPAD = 0;
  }
  STexState(int nFilter, bool bClamp)
  {
    m_pDeviceState = NULL;
    int nAddress = bClamp ? TADDR_CLAMP : TADDR_WRAP;
    SetFilterMode(nFilter);
    SetClampMode(nAddress, nAddress, nAddress);
    SetBorderColor(0);
		m_bSRGBLookup = false;
    m_bActive = false;
    m_bComparison = false;
		padding = 0;
		m_bPAD = 0;
  }
  STexState(int nFilter, int nAddressU, int nAddressV, int nAddressW, unsigned int borderColor)
  {
    m_pDeviceState = NULL;
    SetFilterMode(nFilter);
    SetClampMode(nAddressU, nAddressV, nAddressW);
    SetBorderColor(borderColor);
		m_bSRGBLookup = false;
    m_bActive = false;
    m_bComparison = false;
		padding = 0;
		m_bPAD = 0;
  }
#ifdef _RENDERER
  ~STexState();
  STexState (const STexState& src);
#else
  ~STexState(){}
  STexState (const STexState& src)
  {
    memcpy(this, &src, sizeof(STexState));
  }
#endif
  STexState& operator = (const STexState& src)
  {
    this->~STexState();
    new(this) STexState(src);
    return *this;
  }
  _inline friend bool operator == (const STexState &m1, const STexState &m2)
  {
    if (*(uint64 *)&m1 == *(uint64 *)&m2 && m1.m_dwBorderColor == m2.m_dwBorderColor &&
        m1.m_bActive == m2.m_bActive && m1.m_bComparison == m2.m_bComparison && m1.m_bSRGBLookup == m2.m_bSRGBLookup)
      return true;
    return false;
  }
  void Release()
  {
    delete this;
  }

  bool SetFilterMode(int nFilter);
  bool SetClampMode(int nAddressU, int nAddressV, int nAddressW);
  void SetBorderColor(DWORD dwColor);
  void SetComparisonFilter(bool bEnable);
  void PostCreate();
};


#define FSAMP_NOMIPS 1
#define FSAMP_BUMPPLANTS 2
#define FSAMP_GLOBAL FT_FROMIMAGE


UNIQUE_IFACE struct IRenderTarget
{
	virtual ~IRenderTarget(){}
  virtual void Release()=0;
  virtual void AddRef()=0;
};

//==================================================================================================================

enum EEfResTextures
{
	EFTT_DIFFUSE = 0,
	EFTT_BUMP, 
	EFTT_GLOSS,
	EFTT_ENV,
	EFTT_DETAIL_OVERLAY,
	EFTT_BUMP_DIFFUSE,
	EFTT_BUMP_HEIGHT,
	EFTT_DECAL_OVERLAY,
	EFTT_SUBSURFACE,
	EFTT_CUSTOM,
	EFTT_CUSTOM_SECONDARY,
	EFTT_OPACITY,
	EFTT_GLOSS_NORMAL_A,

	EFTT_MAX
};

// FX shader texture sampler (description)
struct STexSamplerFX
{
#if SHADER_REFLECT_TEXTURE_SLOTS
  string m_szUIName;
  string m_szUIDescription;
#endif

  string m_szName;
  string m_szTexture;

  union
  {
    struct SHRenderTarget *m_pTarget;
    IRenderTarget *m_pITarget;
  };

  int16 m_nTexState;
  byte m_eTexType;						// ETEX_Type e.g. eTT_2D or eTT_Cube
	byte m_nSlotId;             // EFTT_ index if it references one of the material texture slots, EFTT_MAX otherwise
  uint32 m_nTexFlags;
  STexSamplerFX()
  {
    m_nTexState = -1;
    m_eTexType = eTT_2D;
		m_nSlotId = EFTT_MAX;
    m_nTexFlags = 0;
    m_pTarget = NULL;
  }
  ~STexSamplerFX()
  {
    SAFE_RELEASE(m_pITarget);
  }

  int Size()
  {
    int nSize = sizeof(*this);
    nSize += m_szName.capacity();
    nSize += m_szTexture.capacity();
#if SHADER_REFLECT_TEXTURE_SLOTS
    nSize += m_szUIName.capacity();
    nSize += m_szUIDescription.capacity();
#endif
    return nSize;
  }

  void GetMemoryUsage( ICrySizer *pSizer ) const
  {
  }

  uint32 GetTexFlags() { return m_nTexFlags; }
  void Update();
  void PostLoad();
  NO_INLINE STexSamplerFX (const STexSamplerFX& src)
  {
    m_pITarget = src.m_pITarget;
    if (m_pITarget)
      m_pITarget->AddRef();
    m_szName = src.m_szName;
    m_szTexture = src.m_szTexture;
    m_nSlotId = src.m_nSlotId;
    m_eTexType = src.m_eTexType;
    m_nTexFlags = src.m_nTexFlags;
    m_nTexState = src.m_nTexState;
    
#if SHADER_REFLECT_TEXTURE_SLOTS
    m_szUIName = src.m_szUIName;
    m_szUIDescription = src.m_szUIDescription;
#endif
  }
  NO_INLINE STexSamplerFX& operator = (const STexSamplerFX& src)
  {
    this->~STexSamplerFX();
    new(this) STexSamplerFX(src);
    return *this;
  }
  _inline friend bool operator != (const STexSamplerFX &m1, const STexSamplerFX &m2)
  {
    if (m1.m_szTexture != m2.m_szTexture || m1.m_eTexType != m2.m_eTexType || m1.m_nTexFlags != m2.m_nTexFlags)
      return true;
    return false;
  }
  _inline bool operator == (const STexSamplerFX &m1)
  {
    return !(*this != m1);
  }

	bool Export(SShaderSerializeContext& SC);
	bool Import(SShaderSerializeContext& SC, SSTexSamplerFX *pTS);
};


// Resource texture sampler (runtime)
struct STexSamplerRT
{
  union
  {
    CTexture *m_pTex;
    ITexture *m_pITex;
  };

  union
  {
    struct SHRenderTarget *m_pTarget;
    IRenderTarget *m_pITarget;
  };

	STexAnim *m_pAnimInfo;
	IDynTextureSource* m_pDynTexSource;
	
  uint32 m_nTexFlags;
  int16 m_nTexState;

	uint8 m_eTexType;						// ETEX_Type e.g. eTT_2D or eTT_Cube
  int8 m_nSamplerSlot;
  int8 m_nTextureSlot;
  bool m_bGlobal;

  STexSamplerRT()
  {
    m_nTexState = -1;
    m_pTex = NULL;
    m_eTexType = eTT_2D;
    m_nTexFlags = 0;
    m_pTarget = NULL;
		m_pAnimInfo = NULL;
    m_pDynTexSource = NULL;
    m_nSamplerSlot = -1;
    m_nTextureSlot = -1;
    m_bGlobal = false;
  }
  ~STexSamplerRT()
  {
    Cleanup();
  }

  void Cleanup()
  {
    SAFE_RELEASE(m_pITex);
    // TODO: ref counted deleting of m_pAnimInfo & m_pTarget! - CW
    SAFE_RELEASE(m_pDynTexSource);
    SAFE_RELEASE(m_pITarget);
    SAFE_RELEASE(m_pAnimInfo);
  }
  int Size() const
  {
    int nSize = sizeof(*this);
    return nSize;
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
	}

  uint32 GetTexFlags() const { return m_nTexFlags; }
  void Update();
  void PostLoad();
  NO_INLINE STexSamplerRT (const STexSamplerRT& src)
  {
    m_pITex = src.m_pITex;
    if (m_pITex)
      m_pITex->AddRef();
    m_pDynTexSource = src.m_pDynTexSource;
    if (m_pDynTexSource)
      m_pDynTexSource->AddRef();
    m_pITarget = src.m_pITarget;
    if (m_pITarget)
      m_pITarget->AddRef();
    m_pAnimInfo = src.m_pAnimInfo;
    if (m_pAnimInfo)
      m_pAnimInfo->AddRef();
    m_eTexType = src.m_eTexType;
    m_nTexFlags = src.m_nTexFlags;
    m_nTexState = src.m_nTexState;
    m_nSamplerSlot = src.m_nSamplerSlot;
    m_nTextureSlot = src.m_nTextureSlot;
    m_bGlobal = src.m_bGlobal;
  }
  NO_INLINE STexSamplerRT& operator = (const STexSamplerRT& src)
  {
    this->~STexSamplerRT();
    new(this) STexSamplerRT(src);
    return *this;
  }
  STexSamplerRT (const STexSamplerFX& src)
  {
    m_pITex = NULL;
    m_pDynTexSource = NULL;
    m_pAnimInfo = NULL;
    m_pITarget = src.m_pITarget;
    if (m_pITarget)
      m_pITarget->AddRef();
    m_eTexType = src.m_eTexType;
    m_nTexFlags = src.m_nTexFlags;
    m_nTexState = src.m_nTexState;
    m_nSamplerSlot = -1;
    m_nTextureSlot = -1;
    m_bGlobal = (src.m_nTexFlags & FSAMP_GLOBAL) != 0;
  }
  inline bool operator != (const STexSamplerRT &m) const
  {
    if (m_pTex != m.m_pTex || m_eTexType != m.m_eTexType || m_nTexFlags != m.m_nTexFlags || m_nTexState != m.m_nTexState)
      return true;
    return false;
  }
};


//===============================================================================================================================

struct SEfResTextureExt
{
  int32 m_nFrameUpdated;
  int32 m_nUpdateFlags;
  int32 m_nLastRecursionLevel;
  SEfTexModificator* m_pTexModifier;

  SEfResTextureExt ()
  {
    m_nFrameUpdated = -1;
    m_nUpdateFlags = 0;
    m_nLastRecursionLevel = 0;
    m_pTexModifier = NULL;
  }
  ~SEfResTextureExt ()
  {
    Cleanup();
  }
  void Cleanup()
  {
    SAFE_DELETE(m_pTexModifier);
  }
  inline bool operator != (const SEfResTextureExt &m) const
  {
    if (m_pTexModifier && m.m_pTexModifier)
      return *m_pTexModifier != *m.m_pTexModifier;
    if (!m_pTexModifier && !m.m_pTexModifier)
      return false;
    return true;
  }
  SEfResTextureExt(const SEfResTextureExt& src)
  {
    if (&src != this)
    {
      Cleanup();
      if (src.m_pTexModifier)
      {
        m_pTexModifier = new SEfTexModificator;
        *m_pTexModifier = *src.m_pTexModifier;
      }
      m_nFrameUpdated = -1;
      m_nUpdateFlags = src.m_nUpdateFlags;
      m_nLastRecursionLevel = -1;
    }
  }
  SEfResTextureExt& operator = (const SEfResTextureExt& src)
  {
    if (&src != this)
    {
      Cleanup();
      new(this) SEfResTextureExt(src);
    }
    return *this;
  }

  void CopyTo(SEfResTextureExt *pTo) const
  {
    if (pTo != this)
    {
      pTo->Cleanup();
      pTo->m_nFrameUpdated = -1;
      pTo->m_nUpdateFlags = m_nUpdateFlags;
      pTo->m_nLastRecursionLevel = -1;
      pTo->m_pTexModifier = NULL;
    }
  }
  inline int Size() const
  {
    int nSize = sizeof(SEfResTextureExt);
    if (m_pTexModifier)
      nSize += m_pTexModifier->Size();
    return nSize;
  }
};
// Description:
//	 In order to facilitate the memory allocation tracking, we're using here this class;
//	 if you don't like it, please write a substitute for all string within the project and use them everywhere.
struct SEfResTexture
{
  string m_Name;
  byte m_TexFlags;
  bool m_bUTile;
  bool m_bVTile;
  signed char m_Filter;

  STexSamplerRT m_Sampler;
  SEfResTextureExt m_Ext;

	void UpdateForCreate();
  void Update(int nTSlot);
	void UpdateWithModifier(int nTSlot);

  inline bool operator != (const SEfResTexture &m) const
  {
    if (stricmp(m_Name.c_str(), m_Name.c_str()) != 0 ||
        m_TexFlags != m.m_TexFlags || 
        m_bUTile != m.m_bUTile ||
        m_bVTile != m.m_bVTile ||
        m_Filter != m.m_Filter ||
        m_Ext != m.m_Ext ||
        m_Sampler != m.m_Sampler)
      return true;
    return false;
  }
  inline bool IsHasModificators() const
  {
    return (m_Ext.m_pTexModifier != NULL);
  }

  bool IsNeedTexTransform() const
  {
    if (!m_Ext.m_pTexModifier)
      return false;
    if (m_Ext.m_pTexModifier->m_eRotType != ETMR_NoChange || m_Ext.m_pTexModifier->m_eUMoveType != ETMM_NoChange || m_Ext.m_pTexModifier->m_eVMoveType != ETMM_NoChange)
      return true;
    return false;
  }
  bool IsNeedTexGen() const
  {
    if (!m_Ext.m_pTexModifier)
      return false;
    if (m_Ext.m_pTexModifier->m_eTGType != ETG_Stream)
      return true;
    return false;
  }
  inline float GetTiling(int n) const
  {
    if (!m_Ext.m_pTexModifier)
      return 1.0f;
    return m_Ext.m_pTexModifier->m_Tiling[n];
  }
  inline float GetOffset(int n) const
  {
    if (!m_Ext.m_pTexModifier)
      return 0;
    return m_Ext.m_pTexModifier->m_Offs[n];
  }
  inline SEfTexModificator *AddModificator()
  {
    if (!m_Ext.m_pTexModifier)
      m_Ext.m_pTexModifier = new SEfTexModificator;
    return m_Ext.m_pTexModifier;
  }
  int Size() const
  {
    int nSize = sizeof(SEfResTexture) - sizeof(STexSamplerRT) - sizeof(SEfResTextureExt);
    nSize += m_Name.size();
    nSize += m_Sampler.Size();
    nSize += m_Ext.Size();

    return nSize;
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->Add( *this );
		pSizer->AddObject( m_Name );
		pSizer->AddObject( m_Sampler );
	}
  void Cleanup()
  {
    m_Sampler.Cleanup();
    m_Ext.Cleanup();
  }

  ~SEfResTexture()
  {
    Cleanup();
  }

  void Reset()
  {
    m_bUTile = true;
    m_bVTile = true;
    m_TexFlags = 0;
    m_Filter = FILTER_NONE;
    SAFE_DELETE(m_Ext.m_pTexModifier);
    m_Ext.m_nFrameUpdated = -1;
  }
  SEfResTexture (const SEfResTexture& src)
  {
		if (&src != this)
		{
      Cleanup();
	    m_Sampler = src.m_Sampler;
      m_Ext = src.m_Ext;
	    m_TexFlags = src.m_TexFlags;
	    m_Name = src.m_Name;
	    m_bUTile = src.m_bUTile;
	    m_bVTile = src.m_bVTile;
	    m_Filter = src.m_Filter;
		}
  }
  SEfResTexture& operator = (const SEfResTexture& src)
  {
    if (&src != this)
    {
      Cleanup();
      new(this) SEfResTexture(src);
    }
    return *this;
  }
  void CopyTo(SEfResTexture *pTo) const
  {
    if (pTo != this)
    {
      pTo->Cleanup();
      pTo->m_Sampler = m_Sampler;
      m_Ext.CopyTo(&pTo->m_Ext);
      pTo->m_TexFlags = m_TexFlags;
      pTo->m_Name = m_Name;
      pTo->m_bUTile = m_bUTile;
      pTo->m_bVTile = m_bVTile;
      pTo->m_Filter = m_Filter;
    }
  }

  SEfResTexture()
  {
    Reset();
  }
};

struct SBaseShaderResources
{
	DynArray<SShaderParam> m_ShaderParams;
	string m_TexturePath;
	const char *m_szMaterialName;

	float m_AlphaRef;
  uint32 m_ResFlags;

	uint16 m_SortPrio;

	uint8 m_FurAmount;
  uint8 m_HeatAmount;
	uint8 m_CloakAmount;

  int Size() const
  {
    int nSize = sizeof(SBaseShaderResources) + m_ShaderParams.size()*sizeof(SShaderParam);
    return nSize;
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->AddObject(m_ShaderParams);
	}
  SBaseShaderResources& operator=(const SBaseShaderResources& src)
  {
    if (&src != this)
    {
			ReleaseParams();
      m_szMaterialName = src.m_szMaterialName;
      m_ResFlags = src.m_ResFlags;
      m_AlphaRef = src.m_AlphaRef;
      m_HeatAmount = src.m_HeatAmount;
      m_FurAmount = src.m_FurAmount;
			m_CloakAmount = src.m_CloakAmount;
      m_SortPrio = src.m_SortPrio;
      m_ShaderParams = src.m_ShaderParams;
    }
    return *this;
  }

  SBaseShaderResources()
  {
    m_ResFlags = 0;
    m_AlphaRef = 0;
    m_HeatAmount = 0;
    m_FurAmount = 0;
		m_CloakAmount = 255;
    m_SortPrio =0;
    m_szMaterialName = NULL;
  }

  void ReleaseParams()
  {
    m_ShaderParams.clear();
  }
  virtual ~SBaseShaderResources()
  {
    ReleaseParams();
  }
};

UNIQUE_IFACE struct IRenderShaderResources
{
	// <interfuscator:shuffle>
  virtual void AddRef() = 0;
  virtual void UpdateConstants(IShader *pSH) = 0;
  virtual void CloneConstants(const IRenderShaderResources* pSrc) = 0;
  virtual void ExportModificators(IRenderShaderResources* pTrg, CRenderObject *pObj) = 0;
  virtual void SetInputLM(const CInputLightMaterial& lm) = 0;
  virtual void ToInputLM(CInputLightMaterial& lm) = 0;
  virtual ColorF& GetDiffuseColor() = 0;
	virtual float GetDiffuseColorAlpha() = 0;
  virtual ColorF& GetSpecularColor() = 0;
  virtual ColorF& GetEmissiveColor() = 0;
  virtual float& GetSpecularShininess() = 0;
  virtual int GetResFlags() = 0;
  virtual void SetMtlLayerNoDrawFlags( uint8 nFlags ) = 0;
  virtual uint8 GetMtlLayerNoDrawFlags() const = 0;
	virtual SSkyInfo *GetSkyInfo() = 0;
	virtual CCamera *GetCamera() = 0;
  virtual void SetCamera(CCamera *pCam) = 0;
  virtual void SetMaterialName(const char *szName) = 0;
  virtual float& GetGlow() = 0;
  virtual float& GetAlphaRef() = 0;
  virtual float& GetOpacity() = 0;
  virtual SEfResTexture *GetTexture(int nSlot) const = 0;
  virtual DynArrayRef<SShaderParam>& GetParameters() = 0;

	virtual ~IRenderShaderResources() {}
  virtual void Release() = 0;
  virtual void ConvertToInputResource(struct SInputShaderResources *pDst) = 0;
  virtual IRenderShaderResources *Clone() = 0;
  virtual void SetShaderParams(struct SInputShaderResources *pDst, IShader *pSH) = 0;

	virtual size_t GetResourceMemoryUsage(ICrySizer*	pSizer)=0;
  virtual SDetailDecalInfo * GetDetailDecalInfo() = 0;

	virtual void GetMemoryUsage(ICrySizer *pSizer) const = 0;
	// </interfuscator:shuffle>
};


struct SInputShaderResources : public SBaseShaderResources
{
  CInputLightMaterial m_LMaterial;
  SEfResTexture m_Textures[EFTT_MAX];
  SDeformInfo m_DeformInfo;
  SDetailDecalInfo m_DetailDecalInfo;
  float m_Opacity;
  float m_GlowAmount;

  int Size() const
  {
    int nSize = SBaseShaderResources::Size() - sizeof(SEfResTexture) * EFTT_MAX;
    nSize += m_TexturePath.size();
    for (int i=0; i<EFTT_MAX; i++)
    {
      nSize += m_Textures[i].Size();
    }
    return nSize;
  }
  SInputShaderResources& operator=(const SInputShaderResources& src)
  {
    if (&src != this)
    {
      Cleanup();
      SBaseShaderResources::operator = (src);
      m_TexturePath = src.m_TexturePath;
      m_DeformInfo = src.m_DeformInfo;
      m_DetailDecalInfo = src.m_DetailDecalInfo;
      m_Opacity = src.m_Opacity;
      m_GlowAmount = src.m_GlowAmount;
      int i;
      for (i=0; i<EFTT_MAX; i++)
      {
        m_Textures[i] = src.m_Textures[i];
      }
      m_LMaterial = src.m_LMaterial;
    }
    return *this;
  }

  SInputShaderResources()
  {
    for (int i=0; i<EFTT_MAX; i++)
    {
      m_Textures[i].Reset();
    }
    m_Opacity = 0;
    m_GlowAmount = 0.0f;
  }

  SInputShaderResources(struct IRenderShaderResources *pSrc)
  {
    pSrc->ConvertToInputResource(this);
    m_ShaderParams = pSrc->GetParameters();
  }

  void Cleanup()
  {
    for (int i=0; i<EFTT_MAX; i++)
    {
      m_Textures[i].Cleanup();
    }
  }
  virtual ~SInputShaderResources()
  {
    Cleanup();
  }
  bool IsEmpty(int nTSlot) const
  {
    return m_Textures[nTSlot].m_Name.empty();
  }
};

//===================================================================================
// Shader gen structure (used for automatic shader script generating).

//
#define SHGF_HIDDEN   1
#define SHGF_PRECACHE 2
#define SHGF_AUTO_PRECACHE 4
#define SHGF_LOWSPEC_AUTO_PRECACHE 8
#define SHGF_RUNTIME 0x10

#define SHGD_LM_DIFFUSE     1
#define SHGD_TEX_DETAIL     2
#define SHGD_TEX_BUMP       4
#define SHGD_TEX_ENVCM      8
#define SHGD_TEX_GLOSS   0x10
#define SHGD_TEX_BUMPDIF 0x20
#define SHGD_TEX_HEIGHTMAP 0x40
#define SHGD_TEX_SUBSURFACE 0x80
#define SHGD_HW_BILINEARFP16   0x100
#define SHGD_HW_SEPARATEFP16   0x200
#define SHGD_HW_DURANGO  0x400
#define SHGD_HW_ORBIS  0x800
#define SHGD_TEX_CUSTOM 0x1000
#define SHGD_TEX_CUSTOM_SECONDARY 0x2000
#define SHGD_TEX_DECAL 0x4000
#define SHGD_USER_ENABLED 0x40000
#define SHGD_HW_PS3  0x80000
#define SHGD_HW_X360 0x100000
#define SHGD_HW_DX10 0x200000
#define SHGD_HW_DX11 0x400000
#define SHGD_HW_DX9  0x800000
#define SHGD_HW_WATER_TESSELLATION 0x1000000
#define SHGD_HW_SILHOUETTE_POM 0x2000000
#define SHGD_TEX_MASK (SHGD_TEX_DETAIL | SHGD_TEX_BUMP | SHGD_TEX_ENVCM | SHGD_TEX_GLOSS | SHGD_TEX_BUMPDIF | SHGD_TEX_HEIGHTMAP | SHGD_TEX_SUBSURFACE | SHGD_TEX_CUSTOM | SHGD_TEX_CUSTOM_SECONDARY | SHGD_TEX_DECAL)

struct SShaderTextureSlot
{
  SShaderTextureSlot()
  {
    m_TexType = eTT_MaxTexType;
  }

  string m_Name;
  string m_Description;
  byte m_TexType;

  void GetMemoryUsage( ICrySizer *pSizer ) const
  {
    pSizer->AddObject(m_Name);
    pSizer->AddObject(m_Description);
    pSizer->AddObject(m_TexType);
  }
};

struct SShaderTexSlots
{
  uint32 m_nRefCount;
  SShaderTextureSlot *m_UsedSlots[EFTT_MAX];
  SShaderTexSlots()
  {
    m_nRefCount = 1;
		memset(m_UsedSlots, 0, sizeof(m_UsedSlots));
  }
  ~SShaderTexSlots()
  {
    uint32 i;
    for (i=0; i<EFTT_MAX; i++)
    {
      SShaderTextureSlot *pSlot = m_UsedSlots[i];
      SAFE_DELETE(pSlot);
    }
  }
  void Release()
  {
    m_nRefCount--;
    if (!m_nRefCount)
      delete this;
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->AddObject(m_UsedSlots);		
	}
};

struct SShaderGenBit
{
  SShaderGenBit()
  {
    m_Mask = 0;
    m_Flags = 0;
    m_nDependencySet = 0;
    m_nDependencyReset = 0;
		m_NameLength = 0;
		m_dwToken = 0;
  }
  string m_ParamName;
  string m_ParamProp;
  string m_ParamDesc;
  int m_NameLength;
  uint64 m_Mask;
  uint32 m_Flags;
  uint32 m_dwToken;
  std::vector<uint32> m_PrecacheNames;
  std::vector<string> m_DependSets;
  std::vector<string> m_DependResets;
  uint32 m_nDependencySet;
  uint32 m_nDependencyReset;

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->AddObject(m_ParamName);
		pSizer->AddObject(m_ParamProp);
		pSizer->AddObject(m_ParamDesc);
		pSizer->AddObject(m_PrecacheNames);
		pSizer->AddObject(m_DependSets);
		pSizer->AddObject(m_DependResets);
	}
};

struct SShaderGen
{
  uint32 m_nRefCount;
  TArray<SShaderGenBit *> m_BitMask;
  SShaderGen()
  {
    m_nRefCount = 1;
  }
  ~SShaderGen()
  {
    uint32 i;
    for (i=0; i<m_BitMask.Num(); i++)
    {
      SShaderGenBit *pBit = m_BitMask[i];
      SAFE_DELETE(pBit);
    }
    m_BitMask.Free();
  }
  void Release()
  {
    m_nRefCount--;
    if (!m_nRefCount)
      delete this;
  }

	void GetMemoryUsage( ICrySizer *pSizer ) const
	{
		pSizer->AddObject(m_BitMask);		
	}
};

//===================================================================================

enum EShaderType
{
  eST_All=-1,				// To set all with one call.

  eST_General=0,
  eST_Metal,
  eST_Glass,
  eST_Vegetation,
  eST_Ice,
  eST_Terrain,
  eST_Shadow,
  eST_Water,
  eST_FX,
  eST_PostProcess,
  eST_HDR,
  eST_Sky,
  eST_Particle,
  eST_Compute,
  eST_Max						// To define array size.
};

enum EShaderQuality
{
  eSQ_Low=0,
  eSQ_Medium=1,
  eSQ_High=2,
  eSQ_VeryHigh=3,
  eSQ_Max=4
};

enum ERenderQuality
{
  eRQ_Low=0,
  eRQ_Medium=1,
  eRQ_High=2,
  eRQ_VeryHigh=3,
  eRQ_Max=4
};

// Summary:
//	 Shader profile flags .
#define SPF_LOADNORMALALPHA		0x1

struct SShaderProfile
{
  SShaderProfile() :m_iShaderProfileQuality(eSQ_High), m_nShaderProfileFlags(SPF_LOADNORMALALPHA)
  {
  }

  EShaderQuality GetShaderQuality() const 
  { 
    return (EShaderQuality)CLAMP(m_iShaderProfileQuality,0,eSQ_VeryHigh);
  }

  void SetShaderQuality( const EShaderQuality &rValue ) 
  { 
    m_iShaderProfileQuality = (int)rValue;
  }

  // ----------------------------------------------------------------

  int									m_iShaderProfileQuality;		// EShaderQuality e.g. eSQ_Medium, use Get/Set functions if possible
  uint32							m_nShaderProfileFlags;			// SPF_...
};

//====================================================================================
// Phys. material flags

#define MATF_NOCLIP 1

//====================================================================================
// Registered shader techniques ID's

enum EShaderTechniqueID
{
	TTYPE_Z = 0,
	TTYPE_CAUSTICS,
	TTYPE_SHADOWPASS,
	TTYPE_SHADOWGEN,
	TTYPE_GLOWPASS,
	TTYPE_MOTIONBLURPASS,
	TTYPE_CUSTOMRENDERPASS,
	TTYPE_EFFECTLAYER,
	TTYPE_SOFTALPHATESTPASS,
	TTYPE_WATERREFLPASS,
	TTYPE_WATERCAUSTICPASS,
	TTYPE_ZPREPASS,
	TTYPE_PARTICLESTHICKNESSPASS,

	// PC specific techniques must go after this point, to support shader serialising
	// TTYPE_CONSOLE_MAX must equal TTYPE_MAX for console
#if !defined(XENON) && !defined(PS3)
	TTYPE_CONSOLE_MAX,
	TTYPE_SHADOWGENGS = TTYPE_CONSOLE_MAX,
	TTYPE_DEBUG,
#endif

	TTYPE_MAX
};

//====================================================================================

// EFSLIST_ lists
// Note - declaration order/index value has no explicit meaning.

enum ERenderListID
{
	EFSLIST_INVALID = 0,						 // Don't use, internally used.
	EFSLIST_PREPROCESS,						   // Pre-process items.
	EFSLIST_GENERAL,								 // Opaque ambient_light+shadow passes.
	EFSLIST_TERRAINLAYER,					   // Unsorted terrain layers.
	EFSLIST_SHADOW_GEN,						   // Shadow map generation.
	EFSLIST_DECAL,									 // Opaque or transparent decals.
	EFSLIST_WATER_VOLUMES,					 // After decals.
	EFSLIST_TRANSP,								   // Sorted by distance under-water render items.
	EFSLIST_WATER,									 // Water-ocean render items.
	EFSLIST_HDRPOSTPROCESS,				   // Hdr post-processing screen effects.
	EFSLIST_AFTER_HDRPOSTPROCESS,	   // After hdr post-processing screen effects.
	EFSLIST_POSTPROCESS,						 // Post-processing screen effects.
	EFSLIST_AFTER_POSTPROCESS,			 // After post-processing screen effects.
	EFSLIST_SHADOW_PASS,						 // Shadow mask generation (usually from from shadow maps).
	EFSLIST_DEFERRED_PREPROCESS,		 // Pre-process before deferred passes.
	EFSLIST_SKIN,										 // Skin rendering pre-process 
	EFSLIST_HALFRES_PARTICLES,			 // Half resolution particles
	EFSLIST_PARTICLES_THICKNESS,		 // Particles thickness passes
	EFSLIST_LENSOPTICS            ,// Lens-optics processing

	EFSLIST_NUM                 
};

//================================================================
// Different preprocess flags for shaders that require preprocessing (like recursive render to texture, screen effects, visibility check, ...)
// SShader->m_nPreprocess flags in priority order

#define  SPRID_FIRST          25 
#define  SPRID_SCANCM         25
#define  FSPR_SCANCM          (1<<SPRID_SCANCM)
#define  SPRID_SCANTEXWATER   26
#define  FSPR_SCANTEXWATER    (1<<SPRID_SCANTEXWATER)
#define  SPRID_SCANTEX        27
#define  FSPR_SCANTEX         (1<<SPRID_SCANTEX)
#define  SPRID_SCANLCM        28
#define  FSPR_SCANLCM         (1<<SPRID_SCANLCM)
#define  SPRID_GENSPRITES     29
#define  FSPR_GENSPRITES      (1<<SPRID_GENSPRITES)
#define  SPRID_CUSTOMTEXTURE  30
#define  FSPR_CUSTOMTEXTURE   (1<<SPRID_CUSTOMTEXTURE)
#define  SPRID_GENCLOUDS      31
#define  FSPR_GENCLOUDS       (1<<SPRID_GENCLOUDS)

#define  FSPR_MASK            0xfff00000
#define  FSPR_MAX             (1 << 31)

#define FEF_DONTSETTEXTURES   1					// Set: explicit setting of samplers (e.g. tex->Apply(1,nTexStatePoint)), not set: set sampler by sematics (e.g. $ZTarget).
#define FEF_DONTSETSTATES     2

// SShader::m_Flags
// Different useful flags
#define EF_RELOAD        1						// Shader needs tangent vectors array.
#define EF_FORCE_RELOAD  2
#define EF_RELOADED      4
#define EF_NODRAW        8
#define EF_HASCULL       0x10
#define EF_SUPPORTSDEFERREDSHADING_MIXED 0x20
#define EF_SUPPORTSDEFERREDSHADING_FULL 0x40
#define EF_SUPPORTSDEFERREDSHADING ( EF_SUPPORTSDEFERREDSHADING_MIXED | EF_SUPPORTSDEFERREDSHADING_FULL )
#define EF_DECAL         0x80
#define EF_LOADED        0x100
#define EF_LOCALCONSTANTS 0x200
#define EF_BUILD_TREE     0x400
#define EF_LIGHTSTYLE    0x800
#define EF_NOCHUNKMERGING 0x1000
#define EF_SUNFLARES     0x2000
#define EF_NEEDNORMALS   0x4000					// Need normals operations.
#define EF_OFFSETBUMP    0x8000
#define EF_NOTFOUND      0x10000
#define EF_DEFAULT       0x20000
#define EF_SKY           0x40000
#define EF_USELIGHTS     0x80000
#define EF_ALLOW3DC      0x100000
#define EF_FOGSHADER     0x200000
#define EF_FAILED_IMPORT 0x400000				// Currently just for debug, can be removed if necessary
#define EF_PRECACHESHADER 0x800000
#define EF_FORCEREFRACTIONUPDATE    0x1000000
#define EF_SUPPORTSINSTANCING_CONST 0x2000000
#define EF_SUPPORTSINSTANCING_ATTR  0x4000000
#define EF_SUPPORTSINSTANCING (EF_SUPPORTSINSTANCING_CONST | EF_SUPPORTSINSTANCING_ATTR)
#define EF_WATERPARTICLE  0x8000000
#define EF_CLIENTEFFECT  0x10000000
#define EF_SYSTEM        0x20000000
#define EF_REFRACTIVE    0x40000000
#define EF_NOPREVIEW     0x80000000

#define EF_PARSE_MASK    (EF_SUPPORTSINSTANCING | EF_SKY | EF_HASCULL | EF_USELIGHTS | EF_REFRACTIVE)


// SShader::Flags2
// Additional Different useful flags

#define EF2_PREPR_GENSPRITES 0x1
#define EF2_PREPR_GENCLOUDS 0x2
#define EF2_PREPR_SCANWATER 0x4
#define EF2_NOCASTSHADOWS  0x8
#define EF2_NODRAW         0x10
#define EF2_HASOPAQUE      0x40
#define EF2_AFTERHDRPOSTPROCESS  0x80
#define EF2_DONTSORTBYDIST 0x100
#define EF2_FORCE_WATERPASS    0x200
#define EF2_FORCE_GENERALPASS   0x400
#define EF2_AFTERPOSTPROCESS  0x800
#define EF2_IGNORERESOURCESTATES  0x1000
#define EF2_USELIGHTMATERIAL  0x2000
#define EF2_FORCE_TRANSPASS       0x4000
#define EF2_DEFAULTVERTEXFORMAT 0x8000
#define EF2_FORCE_ZPASS 0x10000
#define EF2_FORCE_DRAWLAST 0x20000
#define EF2_FORCE_DRAWAFTERWATER 0x40000
// free 0x80000
// free 0x100000
#define EF2_SINGLELIGHTPASS 0x200000
#define EF2_FORCE_DRAWFIRST 0x400000
#define EF2_HAIR            0x800000
#define EF2_DETAILBUMPMAPPING 0x1000000
#define EF2_HASALPHATEST      0x2000000
#define EF2_HASALPHABLEND     0x4000000
#define EF2_ZPREPASS 0x8000000
#define EF2_VERTEXCOLORS 0x10000000
#define EF2_SKINPASS 0x20000000
#define EF2_HW_TESSELLATION 0x40000000
#define EF2_ALPHABLENDSHADOWS 0x80000000

UNIQUE_IFACE struct IShader
{
public:
	// <interfuscator:shuffle>
	virtual ~IShader(){}
  virtual int GetID() = 0;
  virtual int AddRef()=0;
  virtual int Release()=0;
  virtual int ReleaseForce()=0;

  virtual const char *GetName()=0;
  virtual const char *GetName() const =0;
  virtual int GetFlags() const = 0;
  virtual int GetFlags2() const = 0;
  virtual void SetFlags2(int Flags) = 0;
  virtual void ClearFlags2(int Flags) = 0;
  virtual bool Reload(int nFlags, const char *szShaderName) = 0;
  virtual TArray<CRendElementBase *> *GetREs (int nTech) = 0;
  virtual DynArrayRef<SShaderParam>& GetPublicParams() = 0;
  virtual int GetTexId () = 0;
  virtual ITexture *GetBaseTexture(int *nPass, int *nTU) = 0;
  virtual unsigned int GetUsedTextureTypes(void) = 0;
	virtual SShaderTexSlots* GetUsedTextureSlots(int nTechnique) = 0;
  virtual ECull GetCull(void) = 0;
  virtual int Size(int Flags) = 0;
  virtual uint64 GetGenerationMask() = 0;
  virtual SShaderGen* GetGenerationParams() = 0;
  virtual int GetTechniqueID(int nTechnique, int nRegisteredTechnique) = 0;
  virtual EVertexFormat GetVertexFormat(void) = 0;

  virtual EShaderType GetShaderType() = 0;
  virtual uint32      GetVertexModificator() = 0;

	virtual void GetMemoryUsage(ICrySizer *pSizer) const=0;
	// </interfuscator:shuffle>
};

struct SShaderItem
{
  IShader *m_pShader;
  IRenderShaderResources *m_pShaderResources;
  int32 m_nTechnique;
  uint32 m_nPreprocessFlags;

  SShaderItem()
  {
    m_pShader = NULL;
    m_pShaderResources = NULL;
    m_nTechnique = -1;
    m_nPreprocessFlags = 1;
  }
  SShaderItem(IShader *pSH)
  {
    m_pShader = pSH;
    m_pShaderResources = NULL;
    m_nTechnique = -1;
    m_nPreprocessFlags = 1;
    if (pSH && (pSH->GetFlags2() & EF2_PREPR_GENSPRITES))
      m_nPreprocessFlags |= FSPR_GENSPRITES;
  }
  SShaderItem(IShader *pSH, IRenderShaderResources *pRS)
  {
    m_pShader = pSH;
    m_pShaderResources = pRS;
    m_nTechnique = -1;
    m_nPreprocessFlags = 1;
    if (pSH)
    {
      if (pSH->GetFlags2() & EF2_PREPR_GENSPRITES)
        m_nPreprocessFlags |= FSPR_GENSPRITES;
    }
  }
  SShaderItem(IShader *pSH, IRenderShaderResources *pRS, int nTechnique)
  {
    m_pShader = pSH;
    m_pShaderResources = pRS;
    m_nTechnique = nTechnique;
    m_nPreprocessFlags = 1;
    if (pSH)
    {
      if (pSH->GetFlags2() & EF2_PREPR_GENSPRITES)
        m_nPreprocessFlags |= FSPR_GENSPRITES;
    }
  }
	
  void PostLoad();
  bool Update();
  // Note: 
  //	 If you change this function please check bTransparent variable in CRenderMesh::Render().
  // See also:
  //	 CRenderMesh::Render()
  inline bool IsZWrite() const
  { 
    IShader *pSH = m_pShader;
    if (pSH->GetFlags() & (EF_NODRAW | EF_DECAL))
      return false;
    if (pSH->GetFlags2() & EF2_FORCE_ZPASS)
      return true;
    if (m_pShaderResources && m_pShaderResources->GetDiffuseColorAlpha() != 1.0f)
       return false;
    return true;
  }
  inline struct SShaderTechnique *GetTechnique() const;
  bool IsMergable(SShaderItem& PrevSI);

	void GetMemoryUsage(ICrySizer *pSizer) const
	{
		pSizer->AddObject(m_pShader);
		pSizer->AddObject(m_pShaderResources);
	}
};


//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// DLights

enum eDynamicLightFlags
{
	DLF_AREA_SPEC_TEX						=	BIT(0),
	DLF_DIRECTIONAL							=	BIT(1),
	DLF_BOX_PROJECTED_CM				=	BIT(2),
	DLF_LIGHTBOX_FALLOFF				=	BIT(3),
	DLF_POST_3D_RENDERER				=	BIT(4),
	DLF_CASTSHADOW_MAPS					=	BIT(5),
	DLF_POINT										=	BIT(6),
	DLF_PROJECT									=	BIT(7),
	DLF_LIGHT_BEAM							=	BIT(8),
	DLF_REFLECTIVE_SHADOWMAP		=	BIT(9),
	DLF_IGNORES_VISAREAS				=	BIT(10),
	DLF_DEFERRED_CUBEMAPS				=	BIT(11),
	DLF_DISABLE_X360_MSAA				=	BIT(12),
	DLF_DISABLED								=	BIT(13),
	DLF_AREA_LIGHT							=	BIT(14),
	DLF_HASCLIPBOUND						=	BIT(15),
	DLF_HASCLIPGEOM							=	BIT(16),	// Use stat geom for clip geom
	DLF_FAKE										=	BIT(17),	// No lighting, used for Flares, beams and such.
	DLF_SUN											=	BIT(18),
	DLF_LM											=	BIT(19),
	DLF_THIS_AREA_ONLY					=	BIT(20),	// Affects only current area/sector.
	DLF_AMBIENT									=	BIT(21),	// Ambient light (has name indicates, used for replacing ambient)
	DLF_INDOOR_ONLY							=	BIT(22),	// Do not affect height map.                            
	DLF_ALLOW_LPV 							=	BIT(24),	// Add only to  Light Propagation Volume if it's possible.
	DLF_ATTACH_TO_SUN 					=	BIT(25),	// Add only to  Light Propagation Volume if it's possible.
	DLF_TRACKVIEW_TIMESCRUBBING	=	BIT(26),	// Add only to  Light Propagation Volume if it's possible.

	// Deprecated. Remove once deferred shading by default
	DLF_DEFERRED_LIGHT				= BIT(29),

	// Deprecated. Remove all dependencies editor side, etc
	DLF_SPECULAROCCLUSION			= BIT(30),
	DLF_DIFFUSEOCCLUSION			= BIT(31),
	
	DLF_LIGHTTYPE_MASK				= (DLF_DIRECTIONAL | DLF_POINT | DLF_PROJECT | DLF_AREA_LIGHT)
};


//Area light types
#define DLAT_SPHERE				0x1
#define DLAT_RECTANGLE		0x2
#define DLAT_POINT				0x4

#define DL_SHADOW_UPDATE_SHIFT 8

struct IEntity;

#include <IRenderMesh.h>  // <> required for Interfuscator

struct IAnimNode;

UNIQUE_IFACE struct ILightAnimWrapper : public _i_reference_target_t
{
public:
	virtual bool Resolve() = 0;
	IAnimNode* GetNode() const { return m_pNode; }

protected:
	ILightAnimWrapper(const char* name) : m_name(name), m_pNode(0) {}
	virtual ~ILightAnimWrapper() {}

protected:
	string m_name;
	IAnimNode* m_pNode;
};

struct SRenderLight
{
  SRenderLight()
  {
		memset(this, 0, sizeof(SRenderLight));
    m_fLightFrustumAngle = 45.0f;
    m_fRadius = 4.0f;
		m_fBaseRadius = 4.0f;
    m_SpecMult = m_BaseSpecMult = 1.0f;
    m_ProjMatrix.SetIdentity();
    m_ObjMatrix.SetIdentity();
    m_BaseObjMatrix.SetIdentity();
		m_sName = "";
		m_pSoftOccQuery = NULL;
    m_pDeferredRenderMesh = NULL;
		m_pLightAnim = NULL;
		m_fAreaWidth = 1;
		m_fAreaHeight = 1;
		m_fAreaFov = 90.0f;
		m_fAreaDiffuseSoftness = 1.0f;
		m_fBoxWidth = 1.0f;
		m_fBoxHeight = 1.0f;
		m_fBoxLength = 1.0f;
		m_fTimeScrubbed = 0.0f;
    m_fShadowBias = 1.0f;
    m_fShadowSlopeBias = 1.0f;
		m_fShadowResolutionScale = 1.0f;
		m_nShadowMinResolution = 0;
		m_fShadowUpdateMinRadius = m_fRadius;
		m_nShadowUpdateRatio = 1<<DL_SHADOW_UPDATE_SHIFT;
		m_vFadeAABB = AABB(Vec3(0.0f,0.0f,0.0f), 0.0f);
		m_nEntityId = (uint32)-1;
		m_LensOpticsFrustumAngle = 255;
		m_nAttenFalloffMax = 255;
  }

  const Vec3 &GetPosition() const
  {
    return m_Origin;
  }
  void SetPosition(const Vec3& vPos)
  {
    m_BaseOrigin = vPos;
    m_Origin = vPos;
  }

  // Summary:
  //	 Use this instead of m_Color.
  void SetLightColor(const ColorF &cColor)
  {
    m_Color = cColor;
    m_BaseColor = cColor;
  }

  ITexture* GetDiffuseCubemap() const
  {
    assert(m_pDiffuseCubemap != (void*) 0xfeeefeee);
    return m_pDiffuseCubemap;
  }

  ITexture* GetSpecularCubemap() const
  {
    assert(m_pSpecularCubemap != (void*) 0xfeeefeee);
    return m_pSpecularCubemap;
  }

  IRenderMesh * GetDeferredRenderMesh() const
  {
    return m_pDeferredRenderMesh;
  }

	ITexture* GetLightTexture() const
	{
		return m_pLightImage ? m_pLightImage : m_pLightDynTexSource ? m_pLightDynTexSource->GetTexture() : NULL;
	}

	IOpticsElementBase* GetLensOpticsElement() const
	{
		return m_pLensOpticsElement;
	}

	void SetLensOpticsElement( IOpticsElementBase* pOptics )
	{
		if( m_pLensOpticsElement == pOptics )
			return;
		if( pOptics && pOptics->GetType() != eFT_Root )
			return;
		SAFE_RELEASE(m_pLensOpticsElement);
		m_pLensOpticsElement = pOptics;
		if( m_pLensOpticsElement )
			m_pLensOpticsElement->AddRef();
	}

  void GetMemoryUsage(ICrySizer *pSizer ) const {/*LATER*/}

	void AcquireResources()
	{
		if (m_Shader.m_pShader)
			m_Shader.m_pShader->AddRef();
		if (m_pLightImage)
			m_pLightImage->AddRef();
		if (m_pLightDynTexSource)
			m_pLightDynTexSource->AddRef();
		if(m_pReflectionImage)
			m_pReflectionImage->AddRef();
		if (m_pDiffuseCubemap)
			m_pDiffuseCubemap->AddRef();
		if (m_pSpecularCubemap)
			m_pSpecularCubemap->AddRef();
		if (m_pDeferredRenderMesh)
			m_pDeferredRenderMesh->AddRef();
		if (m_pLensOpticsElement)
			m_pLensOpticsElement->AddRef();
		if (m_pSoftOccQuery)
			m_pSoftOccQuery->AddRef();
		if (m_pLightAnim)
			m_pLightAnim->AddRef();
		if(m_pLightAttenMap)
			m_pLightAttenMap->AddRef();
	}

	void DropResources()
	{
		SAFE_RELEASE(m_Shader.m_pShader);
		SAFE_RELEASE(m_pLightImage);
		SAFE_RELEASE(m_pReflectionImage);
		SAFE_RELEASE(m_pDiffuseCubemap);
		SAFE_RELEASE(m_pSpecularCubemap);
		SAFE_RELEASE(m_pDeferredRenderMesh);
		SAFE_RELEASE(m_pLensOpticsElement);
		SAFE_RELEASE(m_pSoftOccQuery);
		SAFE_RELEASE(m_pLightAnim);
		SAFE_RELEASE(m_pLightAttenMap);
		SAFE_RELEASE(m_pLightDynTexSource);
	}

	void SetAnimSpeed( float fAnimSpeed )
	{
		m_nAnimSpeed = int_round( min(fAnimSpeed * 255.0f / 4.0f, 255.0f) ); // Assuming speed multiplier in range [0, 4]
	}

	float GetAnimSpeed() const
	{
		return ((float) m_nAnimSpeed) * (4.0f / 255.0f);
	}

	void SetFalloffMax(float fMax) 
	{
		m_nAttenFalloffMax = int_round( fMax* 255.0f );
	}

	float GetFalloffMax()  const
	{
		return ((float) m_nAttenFalloffMax) / 255.0f;
	}

//=========================================================================================================================

	// Commonly used on most code paths (64 bytes)
	int32							m_Id; 
	uint32						m_n3DEngineUpdateFrameID;
	uint32						m_nEntityId;
	uint32						m_Flags;		// light flags (DLF_etc).
	Vec3							m_Origin;		// World space position
	float							m_fRadius;		// xyz= Origin, w=Radius. (Do not change order)
	ColorF						m_Color;		// w component unused - todo pack spec mul into alpha (post c3 - touches quite some code)
	float							m_SpecMult;
	float							m_fHDRDynamic;	// <DEPRECATED> 0 to get the same results in HDR, <0 to get darker, >0 to get brighter.	
	int16							m_sX;			// Scissor parameters (2d extent).
	int16							m_sY;
	int16							m_sWidth;
	int16							m_sHeight; 

	// Env. probes
	ITexture*					m_pDiffuseCubemap;		// Very small cubemap texture to make a lookup for diffuse.
	ITexture*					m_pSpecularCubemap;		// Cubemap texture to make a lookup for local specular.
	float							m_fBoxWidth;
	float							m_fBoxHeight;
	float							m_fBoxLength;
	uint8							m_nAttenFalloffMax;
	uint8							m_nSortPriority;

	// Shadow map fields
	struct ILightSource* m_pOwner;
	ShadowMapFrustum**  m_pShadowMapFrustums;	
	float								m_fShadowBias;
	float								m_fShadowSlopeBias;
	float								m_fShadowResolutionScale;
	float								m_fShadowUpdateMinRadius;
	uint16							m_nShadowMinResolution;
	uint16							m_nShadowUpdateRatio;
	uint8								m_ShadowChanMask : 4;

	// Projector
	ITexture*						m_pLightAttenMap;										// User can specify custom light attenuation gradient
 	IDynTextureSource*	m_pLightDynTexSource;								// can be used to project dynamic textures
	ITexture*						m_pLightImage;
	Matrix44						m_ProjMatrix;
	Matrix34						m_ObjMatrix;
	float								m_fLightFrustumAngle;
	float								m_fProjectorNearPlane;

	// Misc fields. todo: put in cold data struct (post c3 - touches quite some code)
	const char*						m_sName;			// Optional name of the light source.
	SShaderItem           m_Shader;
	CRenderObject *       m_pObject[MAX_RECURSION_LEVELS];	// Object for light coronas and light flares.
	IOpticsElementBase*		m_pLensOpticsElement;
	ISoftOcclusionQuery*	m_pSoftOccQuery;	
	ITexture*							m_pReflectionImage;
	IRenderMesh*					m_pDeferredRenderMesh;	// <DEPRECATED>	Arbitrary render mesh. 
	const char*						m_sDeferredGeom;	// <DEPRECATED>	Optional deferred geom file
	ILightAnimWrapper*		m_pLightAnim;

	Matrix34					m_ClipBox;
	AABB							m_vFadeAABB; // TODO: Compress if possible with final implementation
	Matrix34					m_BaseObjMatrix;
	float							m_fTimeScrubbed;
	Vec3							m_BaseOrigin;	// World space position.
	float							m_fBaseRadius;
	ColorF						m_BaseColor;	// w component unused..
	float							m_BaseSpecMult;

	float							m_fAreaDiffuseSoftness;
	float							m_fAreaWidth;
	float							m_fAreaHeight;
	float							m_fAreaFov;

	uint8							m_nAnimSpeed;
	uint8							m_nLightStyle;
	uint8							m_nLightPhase;
	uint8							m_LensOpticsFrustumAngle;		// from 0 to 255, The range will be adjusted from 0 to 360 when used.
};

//////////////////////////////////////////////////////////////////////
class CDLight : public SRenderLight
{
public:
  CDLight() : SRenderLight()
  {
  }

  ~CDLight()
  {
    DropResources();
  }

  // Summary:
  //	 Good for debugging.
  bool IsOk() const
  {
    for(int i=0;i<3;++i)
    {			
      if(m_Color[i]<0 || m_Color[i]>100.0f || _isnan(m_Color[i]))
        return false;
      if(m_BaseColor[i]<0 || m_BaseColor[i]>100.0f || _isnan(m_BaseColor[i]))
        return false;
    }
    return true;
  }

  CDLight(const CDLight& other)
  {
    operator=(other);
  }

  CDLight& operator=(const CDLight& dl)
  {
		if (this == &dl) return *this;

		DropResources();

		m_pOwner=dl.m_pOwner;
		memcpy(m_pObject, dl.m_pObject, sizeof(m_pObject));
		m_pDeferredRenderMesh=dl.m_pDeferredRenderMesh;
		m_Shader=dl.m_Shader;
		m_pShadowMapFrustums=dl.m_pShadowMapFrustums;
		m_pDiffuseCubemap=dl.m_pDiffuseCubemap;
		m_pSpecularCubemap=dl.m_pSpecularCubemap;
		m_pLightImage=dl.m_pLightImage;
		m_pLightDynTexSource=dl.m_pLightDynTexSource;
		m_pLightAttenMap=dl.m_pLightAttenMap;
		m_pReflectionImage=dl.m_pReflectionImage;
		m_sName=dl.m_sName;
		m_sDeferredGeom=dl.m_sDeferredGeom;
		m_ProjMatrix=dl.m_ProjMatrix;
		m_ObjMatrix=dl.m_ObjMatrix;
		m_BaseObjMatrix=dl.m_BaseObjMatrix;
		m_ClipBox=dl.m_ClipBox;
		m_Color=dl.m_Color;
		m_BaseColor=dl.m_BaseColor;
		m_Origin=dl.m_Origin;
		m_BaseOrigin=dl.m_BaseOrigin;
		m_fRadius=dl.m_fRadius;
		m_fBaseRadius=dl.m_fBaseRadius;
		m_SpecMult=dl.m_SpecMult;
		m_BaseSpecMult=dl.m_BaseSpecMult;
		m_fShadowBias=dl.m_fShadowBias;
		m_fShadowSlopeBias=dl.m_fShadowSlopeBias;
		m_fShadowResolutionScale=dl.m_fShadowResolutionScale;
		m_fHDRDynamic=dl.m_fHDRDynamic;
		m_pLensOpticsElement = dl.m_pLensOpticsElement;
		m_LensOpticsFrustumAngle = dl.m_LensOpticsFrustumAngle;
		m_pSoftOccQuery = dl.m_pSoftOccQuery;
		m_fLightFrustumAngle=dl.m_fLightFrustumAngle;
		m_fProjectorNearPlane=dl.m_fProjectorNearPlane;
		m_Flags=dl.m_Flags;
		m_Id=dl.m_Id; 
		m_n3DEngineUpdateFrameID=dl.m_n3DEngineUpdateFrameID;
		m_sX=dl.m_sX;
		m_sY=dl.m_sY;
		m_sWidth=dl.m_sWidth;
		m_sHeight=dl.m_sHeight;
		m_nLightStyle=dl.m_nLightStyle;
		m_nLightPhase=dl.m_nLightPhase;
		m_ShadowChanMask=dl.m_ShadowChanMask;
		m_pLightAnim=dl.m_pLightAnim;
		m_fAreaWidth = dl.m_fAreaWidth;
		m_fAreaHeight = dl.m_fAreaHeight;
		m_fAreaFov = dl.m_fAreaFov;
		m_fAreaDiffuseSoftness = dl.m_fAreaDiffuseSoftness;
		m_fBoxWidth = dl.m_fBoxWidth;
		m_fBoxHeight = dl.m_fBoxHeight;
		m_fBoxLength = dl.m_fBoxLength;
		m_fTimeScrubbed=dl.m_fTimeScrubbed;
		m_nShadowMinResolution =dl.m_nShadowMinResolution;
		m_fShadowUpdateMinRadius =dl.m_fShadowUpdateMinRadius;
		m_nShadowUpdateRatio =dl.m_nShadowUpdateRatio;
		m_vFadeAABB = dl.m_vFadeAABB;
		m_nAnimSpeed = dl.m_nAnimSpeed;  
		m_nSortPriority = dl.m_nSortPriority;  
		m_nAttenFalloffMax = dl.m_nAttenFalloffMax;
		m_nEntityId = dl.m_nEntityId;
		AcquireResources();

		return *this;
  }

  // Summary:
  //	 Use this instead of m_Color.
  const ColorF &GetFinalColor( const ColorF &cColor ) const
  {
    return m_Color;
  }

	// Summary:
	//	 Use this instead of m_Color.
	void SetSpecularMult( float fSpecMult )
	{
		m_SpecMult = fSpecMult;
		m_BaseSpecMult = fSpecMult;
	}

  void SetShadowBiasParams( float fShadowBias, float fShadowSlopeBias )
  {
    m_fShadowBias = fShadowBias;
    m_fShadowSlopeBias = fShadowSlopeBias;
  }

	// Summary:
	//	 Use this instead of m_Color.
	const float &GetSpecularMult() const
	{
		return m_SpecMult;
	}
  void SetMatrix(const Matrix34& Matrix, bool reset=true)
  {
		// Scale the cubemap to adjust the default 45 degree 1/2 angle fustrum to 
		// the desired angle (0 to 90 degrees).
		float scaleFactor = cry_tanf((90.0f-m_fLightFrustumAngle)*gf_PI/180.0f);
		m_ProjMatrix = Matrix33(Matrix) * Matrix33::CreateScale(Vec3(1,scaleFactor,scaleFactor));
		Matrix44 transMat;
		transMat.SetIdentity();
		transMat(3,0) = -Matrix(0,3); transMat(3,1) = -Matrix(1,3); transMat(3,2) = -Matrix(2,3);
		m_ProjMatrix = transMat * m_ProjMatrix;
		m_ObjMatrix = Matrix;
		if (reset)
		{
			m_BaseObjMatrix = m_ObjMatrix;
		}
	}

	void SetSpecularCubemap(ITexture* texture)
	{
		m_pSpecularCubemap = texture;
	}

	void SetDiffuseCubemap(ITexture* texture)
	{
		m_pDiffuseCubemap = texture;
	}

	void ReleaseCubemaps()
	{
		SAFE_RELEASE(m_pSpecularCubemap);
		SAFE_RELEASE(m_pDiffuseCubemap);
	}

	void ReleaseDeferredRenderMesh()
	{
		SAFE_RELEASE(m_pDeferredRenderMesh);
	}

	void SetDeferredRenderMesh(IRenderMesh * pRenderMesh)
	{
    SAFE_RELEASE(m_pDeferredRenderMesh); 
    m_pDeferredRenderMesh = pRenderMesh;
		if(m_pDeferredRenderMesh)
			m_pDeferredRenderMesh->AddRef();
	}
};

#define DECAL_HAS_NORMAL_MAP	(1<<0)
#define DECAL_STATIC					(1<<1)
#define DECAL_HAS_SPECULAR_MAP (1<<2)

struct SDeferredDecal
{
	SDeferredDecal()
	{ 
		ZeroStruct(*this);
		rectTexture.w = rectTexture.h = 1.f;
	}

  Matrix34 projMatrix; // defines where projection should be applied in the world
  IMaterial * pMaterial; // decal material
  float fAlpha; // transparency of decal, used mostly for distance fading
  float fGrowAlphaRef;
	RectF rectTexture; // subset of texture to render
  uint32 nFlags;
  uint8 nSortOrder; // user defined sort order
};

// Summary:
//	 Runtime shader flags for HW skinning.
enum EHWSkinningRuntimeFlags
{
  eHWS_MotionBlured = 0x04
};


// Summary:
//	 Shader graph support.
enum EGrBlockType
{
  eGrBlock_Unknown,
  eGrBlock_VertexInput,
  eGrBlock_VertexOutput,
  eGrBlock_PixelInput,
  eGrBlock_PixelOutput,
  eGrBlock_Texture,
  eGrBlock_Sampler,
  eGrBlock_Function,
  eGrBlock_Constant,
};

enum EGrBlockSamplerType
{
  eGrBlockSampler_Unknown,
  eGrBlockSampler_2D,
  eGrBlockSampler_3D,
  eGrBlockSampler_Cube,
  eGrBlockSampler_Bias2D,
  eGrBlockSampler_BiasCube,
};

enum EGrNodeType
{
  eGrNode_Unknown,
  eGrNode_Input,
  eGrNode_Output,
};

enum EGrNodeFormat
{
  eGrNodeFormat_Unknown,
  eGrNodeFormat_Float,
  eGrNodeFormat_Vector,
  eGrNodeFormat_Matrix,
  eGrNodeFormat_Int,
  eGrNodeFormat_Bool,
  eGrNodeFormat_Texture2D,
  eGrNodeFormat_Texture3D,
  eGrNodeFormat_TextureCUBE,
};

enum EGrNodeIOSemantic
{
  eGrNodeIOSemantic_Unknown,
  eGrNodeIOSemantic_Custom,
  eGrNodeIOSemantic_VPos,
  eGrNodeIOSemantic_Color0,
  eGrNodeIOSemantic_Color1,
  eGrNodeIOSemantic_Color2,
  eGrNodeIOSemantic_Color3,
  eGrNodeIOSemantic_Normal,
  eGrNodeIOSemantic_TexCoord0,
  eGrNodeIOSemantic_TexCoord1,
  eGrNodeIOSemantic_TexCoord2,
  eGrNodeIOSemantic_TexCoord3,
  eGrNodeIOSemantic_TexCoord4,
  eGrNodeIOSemantic_TexCoord5,
  eGrNodeIOSemantic_TexCoord6,
  eGrNodeIOSemantic_TexCoord7,
  eGrNodeIOSemantic_Tangent,
  eGrNodeIOSemantic_Binormal,
};

struct SShaderGraphFunction
{
  string m_Data;
  string m_Name;
  std::vector<string> inParams;
  std::vector<string> outParams;
  std::vector<string> szInTypes;
  std::vector<string> szOutTypes;
};

struct SShaderGraphNode
{
  EGrNodeType m_eType;
  EGrNodeFormat m_eFormat;
  EGrNodeIOSemantic m_eSemantic;
  string m_CustomSemantics;
  string m_Name;
  bool m_bEditable;
  bool m_bWasAdded;
  SShaderGraphFunction *m_pFunction;
  DynArray<SShaderParam> m_Properties;

  SShaderGraphNode()
  {
    m_eType = eGrNode_Unknown;
    m_eFormat = eGrNodeFormat_Unknown;
    m_eSemantic = eGrNodeIOSemantic_Unknown;
    m_bEditable = false;
    m_bWasAdded = false;
    m_pFunction = NULL;
  }
  ~SShaderGraphNode();
};


typedef std::vector<SShaderGraphNode *> FXShaderGraphNodes;
typedef FXShaderGraphNodes::iterator FXShaderGraphNodeItor;

struct SShaderGraphBlock
{
  EGrBlockType m_eType;
  EGrBlockSamplerType m_eSamplerType;
  string m_ClassName;
  FXShaderGraphNodes  m_Nodes;

  ~SShaderGraphBlock();
};

typedef std::vector<SShaderGraphBlock *> FXShaderGraphBlocks;
typedef FXShaderGraphBlocks::iterator FXShaderGraphBlocksItor;

struct CRenderChunk
{
	PodArray<uint16> m_arrChunkBoneIDs;
	CREMesh *pRE;				// Pointer to the mesh.

	float m_texelAreaDensity;
	
	uint32 nFirstIndexId;
	uint32 nNumIndices;
  
	uint32 nFirstVertId;
	uint32 nNumVerts;
  
  uint16 m_nMatFlags;		// Material flags from originally assigned material @see EMaterialFlags.
  uint16 m_nMatID;				// Material Sub-object id.

	// Index of sub-object that this chunk originates from, used by sub-object hide mask.
	// @see IStatObj::GetSubObject
  uint32 nSubObjectIndex;
    
  //////////////////////////////////////////////////////////////////////////
	CRenderChunk():
	m_texelAreaDensity(1.0f),
	nFirstIndexId(0), 
	nNumIndices(0), 
	nFirstVertId(0),
	nNumVerts(0),
	pRE(0),
	m_nMatFlags(0),
	m_nMatID(0),
	nSubObjectIndex(0)
	{
	}

	int Size() const;

	void GetMemoryUsage( ICrySizer *pSizer ) const 
	{
		pSizer->AddObject(m_arrChunkBoneIDs);
	}
};

typedef DynArray<CRenderChunk> TRenderChunkArray; 

#include "RendElement.h"

#endif // _ISHADER
