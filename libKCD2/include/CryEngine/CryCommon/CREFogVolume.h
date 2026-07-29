#ifndef _CREFOGVOLUME_
#define _CREFOGVOLUME_

#pragma once

#include "VertexFormats.h"


struct IFogVolumeRenderNode;


class CREFogVolume : public CRendElementBase
{
public:
	CREFogVolume();

	virtual ~CREFogVolume();
	virtual void mfPrepare(bool bCheckOverflow);
	virtual bool mfDraw( CShader* ef, SShaderPass* sfm );

	virtual void GetMemoryUsage(ICrySizer *pSizer) const 
	{
		pSizer->AddObject(this, sizeof(*this));
	}

	Vec3 m_center;
	bool m_viewerInsideVolume;
	AABB m_localAABB;
	Matrix34 m_matWSInv;
	float m_globalDensity;
	float m_densityOffset;
	float m_nearCutoff;
	Vec2 m_softEdgesLerp;
	ColorF m_fogColor;								// color already combined with fHDRDynamic
	Vec3 m_heightFallOffDirScaled;
	Vec3 m_heightFallOffBasePoint;
	Vec3 m_eyePosInWS;
	Vec3 m_eyePosInOS;
};


#endif // #ifndef _CREFOGVOLUME_