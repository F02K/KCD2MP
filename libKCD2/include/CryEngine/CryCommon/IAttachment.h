#include DEVIRTUALIZE_HEADER_FIX(IAttachment.h)
#ifndef IAttachment_h
#define IAttachment_h

#include <ICryAnimation.h>

struct IStatObj;
struct IMaterial;
struct IPhysicalEntity;
struct ICharacterInstance;
struct IAttachmentSkin;
struct IAttachment;
struct IAttachmentObject;
struct IVertexAnimation;



//Flags used by game DO NOT REORDER
enum AttachmentTypes
{
	CA_BONE,
	CA_FACE,
	CA_SKIN,
};

enum ProxyTypes
{
	CA_NONE  = 0,

	CA_CONE  = 0x01, //pendulum type
	CA_HINGE = 0x02, //pendulum type
	CA_HCONE = 0x03, //pendulum type
	CA_ELLIPSOID = 0x04, //spring type
};

enum AttachmentFlags
{
	//Static Flags
	FLAGS_ATTACH_HIDE_ATTACHMENT					= 0x01,	//already stored in CDF, so don't change this
	FLAGS_ATTACH_PHYSICALIZED_RAYS				= 0x02, //already stored in CDF, so don't change this
	FLAGS_ATTACH_PHYSICALIZED_COLLISIONS	= 0x04, //already stored in CDF, so don't change this
	FLAGS_ATTACH_SW_SKINNING							= 0x08,	//already stored in CDF, so don't change this
	FLAGS_ATTACH_RENDER_ONLY_EXISTING_LOD	= 0x10,	//already stored in CDF, so don't change this

	//Dynamic Flags
	FLAGS_ATTACH_VISIBLE					  = 1<<13,	//we set this flag if we can render the object
	FLAGS_ATTACH_PROJECTED					= 1<<14,	//we set this flag if we can attacht the object to a triangle
	FLAGS_ATTACH_WAS_PHYSICALIZED		= 1<<15, // the attachment actually was physicalized
	FLAGS_ATTACH_HIDE_MAIN_PASS			= 1<<16,
	FLAGS_ATTACH_HIDE_SHADOW_PASS		= 1<<17,
	FLAGS_ATTACH_HIDE_RECURSION			= 1<<18,
	FLAGS_ATTACH_NEAREST_NOFOV 			= 1<<19,
	FLAGS_ATTACH_CLOTH					    = 1<<20,
	FLAGS_ATTACH_NO_BBOX_INFLUENCE  = 1<<21,
	FLAGS_ATTACH_COMBINEATTACHMENT 	= 1<<24,
	FLAGS_ATTACH_PHYSICALIZED = FLAGS_ATTACH_PHYSICALIZED_RAYS|FLAGS_ATTACH_PHYSICALIZED_COLLISIONS,
};



struct SAttachmentPhysParams
{
	uint8	m_nProxyType;
	uint8 m_usePhysics;
	uint8 m_useDebug;
	uint8 m_nRecommendedFPS;  
	f32   m_fMaxDeg;
	f32   m_fHRotation;
	f32   m_fScaleZP;
	f32   m_fScaleZN;   
	f32   m_fMass;
	f32   m_fGravity;
	f32   m_fDamping; 
	f32   m_fStiffness;   
	Vec3  m_vPivotOffset;
	Vec3  m_vSimulationAxis;
	Vec3  m_vStiffnessTarget;

	SAttachmentPhysParams()
	{
		m_nProxyType     =0;
		m_usePhysics     =1;
		m_useDebug       =0;
		m_nRecommendedFPS=10;  
		m_fMaxDeg        =45.0f;
		m_fHRotation     =0.0f;
		m_fScaleZP       =1.0f;
		m_fScaleZN       =1.0f;
		m_fMass          =1.0f;
		m_fGravity       =9.81f;
		m_fDamping       =1.0f; 
		m_fStiffness     =0.0f;   
		m_vPivotOffset.zero();
		m_vSimulationAxis=Vec3(0.0f,0.5f,0.0f);
		m_vStiffnessTarget.zero();
	};
};

UNIQUE_IFACE struct IAttachmentManager
{
	// <interfuscator:shuffle>
	virtual ~IAttachmentManager(){}
	virtual uint32 LoadAttachmentList(const char* pathname ) = 0;
	virtual uint32 SaveAttachmentList(const char* pathname ) = 0;

	virtual IAttachment* CreateAttachment( const char* szName, uint32 type, const char* szJointName=0,bool bCallProject=true) =0;
	virtual int32 RemoveAttachmentByInterface( const IAttachment* ptr  ) =0;
	virtual int32 RemoveAttachmentByName( const char* szName ) =0;
	virtual int32 RemoveAttachmentByNameCRC( uint32 nameCRC ) =0;

	virtual IAttachment* GetInterfaceByName( const char* szName ) const =0;
	virtual IAttachment* GetInterfaceByIndex( uint32 c) const =0;
	virtual IAttachment* GetInterfaceByNameCRC( uint32 nameCRC ) const =0;

	virtual int32 GetAttachmentCount() const =0;
	virtual int32 GetIndexByName( const char* szName ) const =0;
	virtual int32 GetIndexByNameCRC( uint32 nameCRC ) const =0;

	virtual uint32 ProjectAllAttachment()=0;

	virtual void PhysicalizeAttachment( int idx, IPhysicalEntity *pent=0, int nLod=0 )=0;
	virtual void DephysicalizeAttachment( int idx, IPhysicalEntity *pent=0 )=0;

  virtual ICharacterInstance* GetSkelInstance() const = 0;
	// </interfuscator:shuffle>
};

struct IAttachment
{
	// <interfuscator:shuffle>
	virtual void AddRef()=0;
	virtual void Release()=0;
	virtual const char* GetName() const=0;   
	virtual uint32 GetNameCRC() const=0;
	virtual uint32 ReName( const char* szSocketName, uint32 crc ) =0;   

	virtual uint32 GetType() const =0; 
	virtual uint32 SetJointName(const char* szJointName=0)=0;

	virtual uint32 GetFlags()=0;
	virtual void SetFlags(uint32 flags)=0;

	//attachment location in default model-pose
	virtual const QuatT& GetAttAbsoluteDefault() const =0; 
	virtual void SetAttAbsoluteDefault(const QuatT& rot)=0; 

	//attachment location relative to the attachment point (bone,face). Similar to an additional rotation 
	//its is the location in the default-pose 
	virtual void SetAttRelativeDefault(const QuatT& mat)=0; 
	virtual const QuatT& GetAttRelativeDefault() const =0; 

	//its is the location of the attachment in the animated-pose is model-space 
	virtual const QuatT& GetAttModelRelative() const=0; 
	//its is the location of the attachment in the animated-pose in world-space 
	virtual const QuatTS GetAttWorldAbsolute() const=0; 

	virtual uint32 ProjectAttachment()=0;

	virtual void HideAttachment( uint32 x )=0;
	virtual uint32 IsAttachmentHidden()=0;
	virtual void HideInRecursion( uint32 x )=0;
	virtual uint32 IsAttachmentHiddenInRecursion()=0;
	virtual void HideInShadow( uint32 x )=0;
	virtual uint32 IsAttachmentHiddenInShadow()=0;

	virtual void AlignJointAttachment()=0;

	virtual uint32 GetJointID() const =0; 

	virtual uint32 AddBinding( IAttachmentObject* pModel,ISkin* pISkin=0,uint32 nLoadingFlags=0) =0;   
	virtual IAttachmentObject* GetIAttachmentObject() const=0; 
	virtual IAttachmentSkin* GetIAttachmentSkin() =0;  
	virtual void ClearBinding(uint32 nLoadingFlags=0) =0; 
 	virtual uint32 SwapBinding(IAttachment* pNewAttachment) = 0; 

	virtual SAttachmentPhysParams GetPendulumParams() = 0; 
	virtual void SetPendulumParams(const SAttachmentPhysParams& ap) = 0;	virtual size_t SizeOfThis()=0;
	virtual void Serialize(TSerialize ser)=0;
	virtual void GetMemoryUsage(ICrySizer *pSizer) const =0;
	virtual ~IAttachment(){}
	// </interfuscator:shuffle>
};

//Interface to a skin-attachment
UNIQUE_IFACE struct IAttachmentSkin
{
	// <interfuscator:shuffle>
	virtual void AddRef()=0;
	virtual void Release()=0;
	virtual ISkin* GetISkin()=0;
	virtual IVertexAnimation* GetIVertexAnimation()=0;
	virtual float GetExtent(EGeomForm eForm)=0;
	virtual void GetRandomPos(PosNorm& ran, EGeomForm eForm) const=0;
	virtual void GetMemoryUsage(class ICrySizer* pSizer) const=0;
	virtual ~IAttachmentSkin(){}
	// </interfuscator:shuffle>

#ifdef EDITOR_PCDEBUGCODE
	virtual void TriggerMeshStreaming(uint32 nDesiredRenderLOD, const SRenderingPassInfo &passInfo)=0;
	virtual void DrawWireframeStatic( const Matrix34& m34, int nLOD, uint32 color)=0;
#endif
};




// Description:
//     This interface define a way to allow an object to be bound to a character.
struct IAttachmentObject
{
	enum EType
	{
		eAttachment_Unknown,
		eAttachment_StatObj,
		eAttachment_Skeleton,
		eAttachment_SkinMesh,
		eAttachment_Entity,
		eAttachment_Light,
		eAttachment_Effect,
	};

	// <interfuscator:shuffle>
	virtual ~IAttachmentObject(){}
	virtual EType GetAttachmentType() = 0;

	virtual void ProcessAttachment( IAttachment* pIAttachment ) =0;
	virtual void RenderAttachment( SRendParams& rParams, const SRenderingPassInfo &passInfo) {};

  // returns handled state
  virtual bool PhysicalizeAttachment( IAttachmentManager* pManager, int idx, int nLod, IPhysicalEntity *pent, const Vec3 &offset ) { return false; }
  virtual bool UpdatePhysicalizedAttachment( IAttachmentManager* pManager, int idx, IPhysicalEntity *pent, const QuatT &offset) { return false; }

  virtual AABB GetAABB()=0;
	virtual float GetRadiusSqr()=0;

	virtual IStatObj* GetIStatObj() const;
	virtual ICharacterInstance* GetICharacterInstance() const;
	virtual IAttachmentSkin* GetIAttachmentSkin() const { return 0; };
	virtual const char* GetObjectFilePath() const
	{
		ICharacterInstance* pICharInstance = GetICharacterInstance();
		if(pICharInstance)
			return  pICharInstance->GetFilePath();
		IStatObj* pIStaticObject = GetIStatObj();
		if (pIStaticObject)
			return pIStaticObject->GetFilePath();
		IAttachmentSkin* pIAttachmentSkin = GetIAttachmentSkin();
		if (pIAttachmentSkin)
		{
			ISkin* pISkin = pIAttachmentSkin->GetISkin();
			if(pISkin)
				return pISkin->GetModelFilePath();
		}
		return NULL;
	}
	virtual IMaterial* GetBaseMaterial(uint32 nLOD=0) const = 0;
	virtual void SetReplacementMaterial(IMaterial* pMaterial,uint32 nLOD=0) = 0;
	virtual IMaterial* GetReplacementMaterial(uint32 nLOD=0) const = 0;

  virtual void OnRemoveAttachment(IAttachmentManager* pManager, int idx){}
	virtual void Release() = 0;
	// </interfuscator:shuffle>
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
ICharacterInstance* IAttachmentObject::GetICharacterInstance() const
{ 
	return 0; 
}
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IStatObj* IAttachmentObject::GetIStatObj() const
{ 
	return 0; 
}
#endif
//

struct CCGFAttachment : public IAttachmentObject
{
	virtual EType GetAttachmentType() { return eAttachment_StatObj; };
	void ProcessAttachment(IAttachment *pIAttachment ) {}
	void RenderAttachment(SRendParams &rParams, const SRenderingPassInfo &passInfo )
  {
		rParams.pInstance = this;
		IMaterial *pPrev = rParams.pMaterial;
		rParams.pMaterial=pObj->GetMaterial();
		if (m_pReplacementMaterial)
			rParams.pMaterial = m_pReplacementMaterial;
		pObj->Render(rParams, passInfo);
		rParams.pMaterial = pPrev;
	};

	AABB GetAABB() { return pObj->GetAABB(); };
	float GetRadiusSqr() { return sqr(pObj->GetRadius()); };
	IStatObj* GetIStatObj() const;
	void Release() { delete this;	}

	IMaterial *GetBaseMaterial(uint32 nLOD) const;
	void SetReplacementMaterial(IMaterial *pMaterial,uint32 nLOD=0) { m_pReplacementMaterial = pMaterial; };
	IMaterial* GetReplacementMaterial(uint32 nLOD) const { return m_pReplacementMaterial; }

	_smart_ptr<IStatObj> pObj;
	_smart_ptr<IMaterial> m_pReplacementMaterial;
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IStatObj* CCGFAttachment::GetIStatObj() const
{	
	return pObj;	
}
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IMaterial *CCGFAttachment::GetBaseMaterial(uint32 nLOD) const 
{ 
	return m_pReplacementMaterial ? m_pReplacementMaterial.get() : pObj->GetMaterial();
}
#endif
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct CSKELAttachment : public IAttachmentObject
{
  CSKELAttachment()
  {
  }

  virtual EType GetAttachmentType() { return eAttachment_Skeleton; };
  void ProcessAttachment( IAttachment *pIAttachment )	
  {
		m_pCharInstance->ProcessAttachment( pIAttachment );
  };
	void RenderAttachment( SRendParams &rParams, const SRenderingPassInfo &passInfo )	
	{
		rParams.pInstance = this;
		IMaterial *pPrev = rParams.pMaterial;
		rParams.pMaterial = (IMaterial*)(m_pCharInstance ? m_pCharInstance->GetIMaterial() : 0);
		if (m_pReplacementMaterial)
			rParams.pMaterial = m_pReplacementMaterial;
		m_pCharInstance->Render(rParams,QuatTS(IDENTITY), passInfo);
		rParams.pMaterial = pPrev;
	};
  AABB GetAABB() {	return m_pCharInstance ? m_pCharInstance->GetAABB() : AABB(AABB::RESET);	};
	float GetRadiusSqr() { return m_pCharInstance ? m_pCharInstance->GetRadiusSqr() : 0.0f;		}
  ICharacterInstance* GetICharacterInstance() const;

  void Release() 
  { 
    if (m_pCharInstance) 
      m_pCharInstance->OnDetach(); 
    delete this;	
  }

	IMaterial *GetBaseMaterial(uint32 nLOD) const;
	void SetReplacementMaterial(IMaterial *pMaterial,uint32 nLOD) { m_pReplacementMaterial = pMaterial; }
	IMaterial* GetReplacementMaterial(uint32 nLOD) const { return m_pReplacementMaterial; }

  _smart_ptr<ICharacterInstance> m_pCharInstance;
	_smart_ptr<IMaterial> m_pReplacementMaterial;
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
ICharacterInstance* CSKELAttachment::GetICharacterInstance() const
{ 
	return m_pCharInstance; 
}
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IMaterial *CSKELAttachment::GetBaseMaterial(uint32 nLOD) const 
{ 
	return m_pReplacementMaterial ? m_pReplacementMaterial.get() : (m_pCharInstance?m_pCharInstance->GetIMaterial():NULL); 
}
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct CSKINAttachment : public IAttachmentObject
{
  CSKINAttachment()
  {
  }

  virtual EType GetAttachmentType() { return eAttachment_SkinMesh; };
  AABB GetAABB() { return AABB(AABB::RESET); };
	float GetRadiusSqr() { return 0.0f; }
	IAttachmentSkin* GetIAttachmentSkin() const { return m_pIAttachmentSkin; }
  void Release()  {  delete this; }
	void ProcessAttachment( IAttachment *pIAttachment )	{ };

	IMaterial* GetBaseMaterial(uint32 nLOD) const;
	void SetReplacementMaterial(IMaterial *pMaterial,uint32 nLOD) {	m_pReplacementMaterial[nLOD] = pMaterial;	}
	IMaterial* GetReplacementMaterial(uint32 nLOD) const { return m_pReplacementMaterial[nLOD]; }

  _smart_ptr<IAttachmentSkin> m_pIAttachmentSkin;
	_smart_ptr<IMaterial> m_pReplacementMaterial[6];
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IMaterial* CSKINAttachment::GetBaseMaterial(uint32 nLOD) const  
{ 
	return m_pReplacementMaterial[nLOD] ? m_pReplacementMaterial[nLOD].get() : (m_pIAttachmentSkin ? m_pIAttachmentSkin->GetISkin()->GetIMaterial(nLOD):NULL); 
}
#endif


struct CEntityAttachment :	public IAttachmentObject
{
public:
	CEntityAttachment():m_scale(1.0f,1.0f,1.0f),m_id(0){}
	
	virtual EType GetAttachmentType() { return eAttachment_Entity; };
	void SetEntityId(EntityId id) { m_id = id; };
	EntityId GetEntityId() { return m_id; }

	void ProcessAttachment(IAttachment *pIAttachment )
	{
		const QuatT quatT = QuatT(pIAttachment->GetAttWorldAbsolute());
		IEntity *pEntity = gEnv->pEntitySystem->GetEntity(m_id);
		if (pEntity)
		{
			//	float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
			//	gEnv->pRenderer->Draw2dLabel(500.0f, 560.0f, 1.6f, white, false, "UpdateAttach transform: %.12f, %.12f, %.12f\n  rot: %.9f, %.9f, %.9f, %.9f", quatT.t.x, quatT.t.y, quatT.t.z, quatT.q.v.x, quatT.q.v.y, quatT.q.v.z, quatT.q.w);
			pEntity->SetPosRotScale( quatT.t, quatT.q, m_scale, ENTITY_XFORM_NO_PROPOGATE);
		}
	}

	AABB GetAABB()
	{
		IEntity *pEntity = gEnv->pEntitySystem->GetEntity(m_id);
		AABB aabb(Vec3(0,0,0), Vec3(0,0,0));

		if (pEntity) pEntity->GetLocalBounds(aabb);
		return aabb;
	};

	float GetRadiusSqr() 
	{
		IEntity *pEntity = gEnv->pEntitySystem->GetEntity(m_id);
		if(pEntity)
		{
			AABB aabb;
			pEntity->GetLocalBounds(aabb);
			return aabb.GetRadiusSqr();
		}
		else
		{
			return 0.0f;
		}
	};

	void Release() { delete this;	};

	IMaterial *GetBaseMaterial(uint32 nLOD) const;
	void SetReplacementMaterial(IMaterial *pMaterial,uint32 nLOD) {};
	IMaterial* GetReplacementMaterial(uint32 nLOD) const { return 0; }
  void SetScale(const Vec3& scale)
  {
    m_scale = scale;
  }

private:
	EntityId	m_id;
  Vec3 m_scale;
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IMaterial *CEntityAttachment::GetBaseMaterial(uint32 nLOD) const 
{ 
	return 0; 
}
#endif

struct CLightAttachment : public IAttachmentObject
{
public:
	CLightAttachment(): m_pLightSource(0)	{};
	virtual ~CLightAttachment()
	{
		if (m_pLightSource)
		{
			gEnv->p3DEngine->UnRegisterEntityDirect(m_pLightSource);
			m_pLightSource->ReleaseNode();
		}
	};

	virtual EType GetAttachmentType() { return eAttachment_Light; };

	void LoadLight(const CDLight &light)
	{
		m_pLightSource = gEnv->p3DEngine->CreateLightSource();
    if (m_pLightSource)
			m_pLightSource->SetLightProperties(light);
	}

  ILightSource* GetLightSource() { return m_pLightSource; }

	void ProcessAttachment(IAttachment* pIAttachment )	
	{
		if (m_pLightSource)
		{
			CDLight &light = m_pLightSource->GetLightProperties();
			Matrix34 worldMatrix = Matrix34(pIAttachment->GetAttWorldAbsolute());
			Vec3 origin = worldMatrix.GetTranslation();
			light.SetPosition( origin );
			light.SetMatrix(worldMatrix);
			light.m_sName = pIAttachment->GetName();
			m_pLightSource->SetMatrix(worldMatrix);
			f32 r = light.m_fRadius;
			m_pLightSource->SetBBox(AABB(Vec3(origin.x-r, origin.y-r, origin.z-r), Vec3(origin.x+r,origin.y+r,origin.z+r)));
			gEnv->p3DEngine->RegisterEntity(m_pLightSource);
		}
	}

	AABB GetAABB()
	{
		f32 r = m_pLightSource->GetLightProperties().m_fRadius;
		return AABB(Vec3(-r, -r, -r), Vec3(+r, +r, +r));
	};

	float GetRadiusSqr() { return sqr(m_pLightSource->GetLightProperties().m_fRadius); }

	void Release() { delete this; };

	IMaterial* GetBaseMaterial(uint32 nLOD) const;
	void SetReplacementMaterial(IMaterial *pMaterial,uint32 nLOD) {};
	IMaterial* GetReplacementMaterial(uint32 nLOD) const { return 0; }

private:
	ILightSource *m_pLightSource;
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IMaterial* CLightAttachment::GetBaseMaterial(uint32 nLOD) const 
{ 
	return 0; 
}
#endif

struct CEffectAttachment : public IAttachmentObject
{
public:

	virtual EType GetAttachmentType() { return eAttachment_Effect; };

	CEffectAttachment(const char *effectName, const Vec3 &offset, const Vec3 &dir, f32 scale)
		: m_loc( IParticleEffect::ParticleLoc(offset, dir, scale) )
	{
		m_pEffect = gEnv->pParticleManager->FindEffect(effectName,"Character Attachment",true);
	}

	CEffectAttachment(IParticleEffect* pParticleEffect, const Vec3 &offset, const Vec3 &dir, f32 scale)
		: m_loc( IParticleEffect::ParticleLoc(offset, dir, scale) )
	{
		m_pEffect = pParticleEffect;
	}

	virtual ~CEffectAttachment()
	{
		if (m_pEmitter)
			m_pEmitter->Activate(false);
	}

	IParticleEmitter *GetEmitter()
	{
		return m_pEmitter;
	}

	void ProcessAttachment(IAttachment *pIAttachment )
	{
		if (!pIAttachment->IsAttachmentHidden())
		{
			QuatTS loc = pIAttachment->GetAttWorldAbsolute() * m_loc;
			if (!m_pEmitter && m_pEffect != 0)
				m_pEmitter = m_pEffect->Spawn(loc);
			else if (m_pEmitter)
				m_pEmitter->SetLocation(loc);
		}
		else
		{
			m_pEmitter = 0;
		}
	}

	AABB GetAABB()
	{
		if (m_pEmitter)
		{
			AABB bb;
			m_pEmitter->GetLocalBounds(bb);
			return bb;
		}
		else
		{
			return AABB(Vec3(-0.1f), Vec3(0.1f));
		}
	};

	float GetRadiusSqr()
	{
		if (m_pEmitter)
		{
			AABB bb;
			m_pEmitter->GetLocalBounds(bb);
			return bb.GetRadiusSqr();
		}
		else
		{
			return 0.1f;
		}
	}

	void Release() { delete this; };

	IMaterial* GetBaseMaterial(uint32 nLOD) const;
	void SetReplacementMaterial(IMaterial *pMaterial,uint32 nLOD) {};
	IMaterial* GetReplacementMaterial(uint32 nLOD) const { return 0; }

	void SetSpawnParams( const SpawnParams& params )
	{  
		if (m_pEmitter)
			m_pEmitter->SetSpawnParams(params);
	}

private:
	_smart_ptr<IParticleEmitter>	m_pEmitter;
	_smart_ptr<IParticleEffect>		m_pEffect;
	QuatTS												m_loc;
};

// ugly ugly CryCG workaround, i only can feel sorry for it
// but the way IAttachmentObjects are implemented, they need their default implementation
// in CryCommon. But this leads to multipe defines on CryCG side, leading to not found
// SPU Indirect function (these are looked up by UUID, which is different per translation unit)
#if !defined(__SPU__) || (defined(__SPU__) && defined(SPU_INCLUDE_ASYNC_OCTREEUPDATE_FUNCTIONS))
SPU_INDIRECT(AsyncOctreeUpdate(M))
#if !defined(__SPU__)
inline
#endif
IMaterial* CEffectAttachment::GetBaseMaterial(uint32 nLOD) const 
{ 
	return 0; 
}
#endif

#endif // IAttachment_h
