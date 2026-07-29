#ifndef __IFACEGEN_H__
#define __IFACEGEN_H__

#pragma once

struct SBoneMod
{
	Vec3 trans;
	float scale;
	int bone;
	int knot;
	int vec;
	float w;

	struct SLink
	{
		float wts[3]; // max of 3 key points to link
		int   idx[3];
		SLink()
		{
			Reset();
		}
		void Reset()
		{
			wts[0]=wts[1]=wts[2]=0.0f;
			idx[0]=idx[1]=idx[2]=-1;
		}
	};

	SLink links[3]; // 3 coordinates
	SBoneMod():w(1.0f), scale(1.0f){}
};

struct IFaceGen{
	virtual ~IFaceGen(){}
	virtual void FindNearestUVVertexShift(const Vec3& pos, const Vec2& uv, Vec3& shift) const = 0;
	virtual void CalcVertexNormals(void) = 0;
	virtual void CalcVertexPositions(void) = 0;
	virtual void ScaleAttachments() = 0;
	virtual void Drop(void) = 0;
	virtual void CalcMinZ(void) = 0;
	virtual void AutoFaceBack(void) = 0;
	virtual void FaceBack(void) = 0;
	virtual int  Load(void const *,size_t) = 0;
	virtual void Destroy()=0;
	virtual SBoneMod* GetBoneMod(size_t index) = 0;
	virtual void SetBoneModTrans(size_t index, const Vec3& trans) = 0;
	virtual bool GetBoneModTrans(const char* name, size_t knot, size_t vec, Vec3& trans, float& scale)const = 0;
	virtual void FetchTransform(const IFaceGen* gf) = 0;
};

#endif // __IFACEGEN_H__
