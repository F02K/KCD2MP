#ifndef _CRELIGHT_PROPAGATION_VOLUME_
#define _CRELIGHT_PROPAGATION_VOLUME_

#pragma once

#if !defined(RENDERNODES_LEAN_AND_MEAN)

#include "VertexFormats.h"

struct SReflectiveShadowMap
{
	Matrix44A mxLightViewProj;
	ITexture*	pDepthRT;
	ITexture*	pNormalsRT;
	ITexture*	pFluxRT;
	SReflectiveShadowMap()
	{
		mxLightViewProj.SetIdentity();
		pDepthRT = NULL;
		pNormalsRT = NULL;
		pFluxRT = NULL;
	}
	void Release();
};

class CRELightPropagationVolume : public CRendElementBase
{
protected:
	friend struct ILPVRenderNode;
	friend class CLPVRenderNode;
	friend class CLPVCascade;
	friend class CLightPropagationVolumesManager;
private:
	// Internal methods

	// injects single texture or colored shadow map with vertex texture fetching
	void _injectWithVTF(SReflectiveShadowMap& rCSM);

	// injects single texture or colored shadow map with render to vertex buffer technique
	void _injectWithR2VB(SReflectiveShadowMap& rCSM);

	// injects occlusion from camera depth buffer
	void _injectOcclusionFromCamera();

protected:
	// injects occlusion information from depth buffer
	void InjectOcclusionFromRSM(SReflectiveShadowMap& rCSM, bool bCamera);

	// propagates radiance
	void Propagate();

	// post-injection phase (injection after propagation)
	bool Postinject();

	// post-injects single ready light
	void PostnjectLight(const CDLight& rLight);

	// optimizations for faster deferred rendering
	void ResolveToVolumeTexture();
public:

	// returns LPV id (might be the same as light id for GI volumes)
	int GetId() const { return m_nId; }

	ILightSource* GetAttachedLightSource() { return m_pAttachedLightSource; }
	void AttachLightSource(ILightSource* pLightSource) { m_pAttachedLightSource = pLightSource; }

	CRELightPropagationVolume();
	virtual ~CRELightPropagationVolume();

	// flags for LPV render element
	enum EFlags
	{
		efGIVolume				= 1ul<<0,
		efHasOcclusion		= 1ul<<1,
	};

	// define the global maximum possible grid size for LPVs (should be GPU-friendly and <= 64 - h/w limitation on old platforms)
	enum { nMaxGridSize = 32u };

	enum EStaticProperties
	{
#if defined(XENON) || defined(PS3)
		espMaxInjectRSMSize = 128ul,
#else
		espMaxInjectRSMSize = 384ul,
#endif
	};

	// LPV render settings
	struct Settings
	{
		Vec3 			m_pos;
		AABB			m_bbox;											// BBox
		Matrix44A	m_mat;
		Matrix44A	m_matInv;
		int				m_nGridWidth;								// grid dimensions in texels
		int				m_nGridHeight;							// grid dimensions in texels
		int				m_nGridDepth;								// grid dimensions in texels
		int				m_nNumIterations;						// number of iterations to propagate
		Vec4			m_gridDimensions;						// grid size
		Vec4			m_invGridDimensions;				// 1.f / (grid size)
		Settings();
	};

	// render reflective shadow map to render
	void InjectReflectiveShadowMap(SReflectiveShadowMap& rCSM);

	// query point light source to render
	virtual void InsertLight( const CDLight &light );

	// propagates radiance
	void Evaluate();

	// apply radiance to accumulation RT
	void DeferredApply();

	// pass only CRELightPropagationVolume::EFlags enum flags combination here
	void SetNewFlags(uint32 nFlags) { m_nGridFlags = nFlags; }

	// CRELightPropagationVolume::EFlags enum flags returned
	uint32 GetFlags() const { return m_nGridFlags; }

	virtual void mfPrepare(bool bCheckOverflow);
	virtual bool mfPreDraw(SShaderPass *sl);
	virtual bool mfDraw(CShader* ef, SShaderPass* sfm);

	void Invalidate() { m_bIsUpToDate = false; }

	ITexture* GetLPVTexture(int iTex) { assert(iTex < 3); return m_pVolumeTextures[iTex]; }
	float GetVisibleDistance() const { return m_fDistance; }
	float GetIntensity() const { return m_fAmount; }

	// is it applicable to render in the deferred pass?
	inline bool IsRenderable() const { return m_bIsRenderable; }

	virtual Settings* GetFillSettings();

	//this function operates on both fill/render settings, do NOT call it unless right after creating the
	//CRELightPropagationVolume instance, at which point no other threads can access it
	virtual void DuplicateFillSettingToOtherSettings();

	virtual void GetMemoryUsage(ICrySizer *pSizer) const 
	{
		pSizer->AddObject(this, sizeof(*this));
		pSizer->AddContainer(m_lightsToPostinject[0][0]);
		pSizer->AddContainer(m_lightsToPostinject[0][1]);
		pSizer->AddContainer(m_lightsToPostinject[1][0]);
		pSizer->AddContainer(m_lightsToPostinject[1][1]);	
	}
	const Settings& GetRenderSettings() const;
protected:
	virtual void UpdateRenderParameters();
	virtual void EnableSpecular(const bool bEnabled);
	void Cleanup();

protected:
	Settings	m_Settings[RT_COMMAND_BUF_COUNT];

	int				m_nId;											// unique ID of the volume(need for RTs)
	uint32		m_nGridFlags;								// grid flags

	float			m_fDistance;								// max affected distance
	float			m_fAmount;									// affection scaler

	union 
	{	
		ITexture*	m_pRT[3];	
		CTexture*	m_pRTex[3];
	};																		// grid itself

	union																	// volume textures
	{
		ITexture*	m_pVolumeTextures[3];
		CTexture*	m_pVolumeTexs[3];
	};
#ifdef XENON
	D3DVolumeTexture*	m_pVolumeTexturesAux[3][32];	// work-around for X360
#elif PS3
	union																	// render to volume texture for PS3		
	{
		ITexture*	m_p2DVolumeUnwraps[3];
		CTexture*	m_p2DVolumeUnwrapsTex[3];
	};
#endif

	union																	// volume texture for fuzzy occlusion from depth buffers
	{
		ITexture*	m_pOcclusionTexture;
		CTexture*	m_pOcclusionTex;
	};

	struct ILPVRenderNode* m_pParent;

	ILightSource*	m_pAttachedLightSource;

	bool			m_bIsRenderable;						// is the grid not dark?
	bool			m_bNeedPropagate;						// is the grid needs to be propagated
	bool			m_bNeedClear;								// is the grid needs to be cleared after past frame
	bool			m_bIsUpToDate;							// invalidates dimensions
	bool			m_bHasSpecular;							// enables specular
	int				m_nUpdateFrameID;						// last frame updated

	typedef DynArray<CDLight> Lights;
	Lights		m_lightsToPostinject[RT_COMMAND_BUF_COUNT][2];	// lights in the grid
};

#endif // #if !defined(RENDERNODES_LEAN_AND_MEAN)

#endif // #ifndef _CRELIGHT_PROPAGATION_VOLUME_
