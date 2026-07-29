//////////////////////////////////////////////////////////////////////
//
//	Crytek Common Source code
//	
//	File:Cry_Xenon_HWMatrix3.h
//	Description: Hardware vec implementation of matrices
//
//	History:
//	-March 5, 2010: Created by Richard Semmens
//                
//
//////////////////////////////////////////////////////////////////////


#ifndef HWMATRIX_XENON_H
#define HWMATRIX_XENON_H 

#if _MSC_VER > 1000
# pragma once
#endif

// SF: There doesn't seem to be a unified interface for using vector intrinsics across platforms
// and performing matrix vector transforms, so I'll put these here for now!

//======================================================
// xm Decalarations
//======================================================
static inline void xmTranspose33(XMVECTOR out[3], const XMVECTOR in[3]);
static inline void xmLoadMatrix33(XMVECTOR m[3], const Matrix33& rot);
static inline XMVECTOR xmRotateVector(const XMVECTOR rot[3], const XMVECTOR v);
static inline XMVECTOR xmRotateVectorTransposed(const XMVECTOR rot[3], const XMVECTOR v);


//======================================================
// xm Definitions
//======================================================

static inline void xmTranspose33(XMVECTOR out[3], const XMVECTOR in[3])
{
	// Note: result in fourth element is undefined
	static const XMVECTORI PERMA = { XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_0W, XM_PERMUTE_0X};
	static const XMVECTORI PERMB = { XM_PERMUTE_0X, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X};

	XMVECTOR tmp0, tmp1, res0, res1, res2;
	tmp0 = __vmrghw( in[0], in[2] );
	tmp1 = __vmrglw( in[0], in[2] );
	res0 = __vmrghw( tmp0, in[1] );
	res1 = XMVectorPermute( tmp0, in[1], (XMVECTOR)PERMA );
	res2 = XMVectorPermute( tmp1, in[1], (XMVECTOR)PERMB );

	out[0] = res0;
	out[1] = res1;
	out[2] = res2;
}

static inline void xmLoadMatrix33(XMVECTOR m[3], const Matrix33& rot)
{
	// Note: result in fourth element is undefined
	m[0] = XMLoadVector4(&rot.m00);
	m[1] = XMLoadVector4(&rot.m10);
	m[2] = XMLoadVector4(&rot.m20);
}

static inline void xmStoreMatrix33(Matrix33& rot, const XMVECTOR m[3])
{
	XMStoreVector3(&rot.m00, m[0]);
	XMStoreVector3(&rot.m10, m[1]);
	XMStoreVector3(&rot.m20, m[2]);
}

// v = rot33 * v
static inline XMVECTOR xmRotateVector33(const XMVECTOR rot[3], const XMVECTOR v)
{
	// Its really unfortunate that Matrix33 are stored like this
	// because they need transposing to get the basis in a useful form
	XMVECTOR transposed[3];
	xmTranspose33(transposed, rot);

	// Note: result in fourth element is undefined
	XMVECTOR xxxx = __vspltw(v, 0);
	XMVECTOR yyyy = __vspltw(v, 1);
	XMVECTOR zzzz = __vspltw(v, 2);
	return __vmaddfp(transposed[2], zzzz, __vmaddfp(transposed[1], yyyy, __vmulfp(transposed[0], xxxx)));
}

// v = transposed(rot33) * v
static inline XMVECTOR xmRotateVectorTransposed33(const XMVECTOR rot[3], const XMVECTOR v)
{
	// Note: result in fourth element is undefined
	XMVECTOR xxxx = __vspltw(v, 0);
	XMVECTOR yyyy = __vspltw(v, 1);
	XMVECTOR zzzz = __vspltw(v, 2);
	return __vmaddfp(rot[2], zzzz, __vmaddfp(rot[1], yyyy, __vmulfp(rot[0], xxxx)));
}

typedef XMVECTOR hwvec3;

struct hwmtx33
{
	union
	{
		struct
		{
			hwvec3 m0, m1, m2;
		};
		struct
		{
			hwvec3 v[3];
		};
	};
};


ILINE void HWMtx33LoadAligned(hwmtx33& out, const Matrix34A& inMtx )
{
	out.m0 = inMtx.m0;
	out.m1 = inMtx.m1;
	out.m2 = inMtx.m2;
}

ILINE void HWMtx33Transpose(hwmtx33& out, const hwmtx33& inMtx)
{
	// Note: result in fourth element is undefined
	static const XMVECTORI PERMA = { XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_0W, XM_PERMUTE_0X};
	static const XMVECTORI PERMB = { XM_PERMUTE_0X, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X};

	XMVECTOR tmp0, tmp1, res0, res1, res2;
	tmp0 = __vmrghw( inMtx.v[0], inMtx.v[2] );
	tmp1 = __vmrglw( inMtx.v[0], inMtx.v[2] );
	res0 = __vmrghw( tmp0, inMtx.v[1] );
	res1 = XMVectorPermute( tmp0, inMtx.v[1], (XMVECTOR)PERMA );
	res2 = XMVectorPermute( tmp1, inMtx.v[1], (XMVECTOR)PERMB );

	out.v[0] = res0;
	out.v[1] = res1;
	out.v[2] = res2;
}


ILINE hwvec3 HWMtx33RotateVec(const hwmtx33& m, const hwvec3& v)
{
	hwmtx33 transposed;
	HWMtx33Transpose(transposed, m);

	// Note: result in fourth element is undefined
	hwvec3 xxxx = __vspltw(v, 0);
	hwvec3 yyyy = __vspltw(v, 1);
	hwvec3 zzzz = __vspltw(v, 2);
	return HWVMultiplyAdd(transposed.v[2], zzzz, HWVMultiplyAdd(transposed.v[1], yyyy, HWVMultiply(transposed.v[0], xxxx)));
}

ILINE hwvec3 HWMtx33RotateVecOpt(const hwmtx33& m, const hwvec3& v)
{
	// Note: result in fourth element is undefined
	hwvec3 xxxx = __vspltw(v, 0);
	hwvec3 yyyy = __vspltw(v, 1);
	hwvec3 zzzz = __vspltw(v, 2);
	return HWVMultiplyAdd(m.v[2], zzzz, HWVMultiplyAdd(m.v[1], yyyy, HWVMultiply(m.v[0], xxxx)));
}


//Returns a matrix optimized for this platform's matrix ops, in this case, transposed;
ILINE hwmtx33 HWMtx33GetOptimized(const hwmtx33& m)
{
	hwmtx33 opt;
	HWMtx33Transpose(opt, m);
	return opt;
}

ILINE hwmtx33 HWMtx33CreateRotationV0V1(const hwvec3& v0, const hwvec3& v1)
{
	hwmtx33 m;	

	const XMVECTOR vZero = XMVectorZero();
	static const XMVECTORI selectx = {XM_PERMUTE_0X, XM_PERMUTE_1Y, XM_PERMUTE_1Z, XM_PERMUTE_1W};
	static const XMVECTORI selecty = {XM_PERMUTE_1X, XM_PERMUTE_0Y, XM_PERMUTE_1Z, XM_PERMUTE_1W};
	static const XMVECTORI selectz = {XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z, XM_PERMUTE_1W};

	simdf fDot = HWV3Dot(v0, v1);
	SIMDFConstant(fOpposed, -0.9999f);

	if(SIMDFLessThanB(fDot, fOpposed) )
	{
		const static XMVECTORF32 vTwo = {2.0f, 2.0f, 2.0f, 2.0f};
		const static XMVECTORF32 vMinusOne = {-1.0f, -1.0f, -1.0f, -1.0f};
		
		XMVECTOR axisTmp	= XMVector3Orthogonal(v0);
		XMVECTOR axis			= XMVector3Normalize(axisTmp);
		XMVECTOR axisX		= XMVectorSplatX(axis);
		XMVECTOR axisY		= XMVectorSplatY(axis);
		XMVECTOR axisZ		= XMVectorSplatZ(axis);
		
		const XMVECTOR axis2 = XMVectorMultiply(vTwo, axis);

		const XMVECTOR row0Add = XMVectorPermute( vMinusOne, vZero, selectx);
		const XMVECTOR row0Mult = XMVectorAdd(row0Add, axisX);
		
		m.m0 = XMVectorMultiply(axis2, row0Mult);

		const XMVECTOR row1Add = XMVectorPermute(vMinusOne, vZero, selecty);
		const XMVECTOR row1Mult = XMVectorAdd(row1Add, axisY);

		m.m1 = XMVectorMultiply(axis2, row1Mult);

		const XMVECTOR row2Add = XMVectorPermute(vMinusOne, vZero, selectz);
		const XMVECTOR row2Mult = XMVectorAdd(row2Add, axisY);

		m.m2 = XMVectorMultiply(axis2, row2Mult);

// 		m00=F(2*axis.x*axis.x-1);	m01=F(2*axis.x*axis.y);		m02=F(2*axis.x*axis.z);	
// 		m10=F(2*axis.y*axis.x);		m11=F(2*axis.y*axis.y-1);	m12=F(2*axis.y*axis.z);	
// 		m20=F(2*axis.z*axis.x);		m21=F(2*axis.z*axis.y);		m22=F(2*axis.z*axis.z-1);					
	}
	else
	{
		static const XMVECTORF32 vOne = {1.0f, 1.0f, 1.0f, 1.0f};

		simdf invh	= SIMDFAdd(vOne, fDot);
		hwvec3 v		= HWVCross(v0, v1);		
		simdf h			= SIMDFReciprocal(invh);

		const XMVECTOR addDotRow0 = XMVectorPermute(fDot, vZero, selectx);
		const XMVECTOR addDotRow1 = XMVectorPermute(fDot, vZero, selecty);
		const XMVECTOR addDotRow2 = XMVectorPermute(fDot, vZero, selectz);

		const XMVECTOR negV = XMVectorNegate(v);

		XMVECTOR vx = XMVectorSplatX(v);
		XMVECTOR vy = XMVectorSplatY(v);
		XMVECTOR vz = XMVectorSplatZ(v);
		
		const XMVECTOR row0h = XMVectorAdd(h, addDotRow0);
		const XMVECTOR row1h = XMVectorAdd(h, addDotRow1);
		const XMVECTOR row2h = XMVectorAdd(h, addDotRow2);

		static const XMVECTORI row0NegSelect = {XM_PERMUTE_0X, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_1W};

		const XMVECTOR row0Perm0 = XMVectorPermute(v, negV, row0NegSelect);
		const XMVECTOR row0Perm1 = XMVectorPermute(vZero, row0Perm0, selectx);

		const XMVECTOR row0Mult = XMVectorAdd(v, row0Perm1);
		
		m.m0 = XMVectorMultiply(XMVectorMultiply(row0h, vx), row0Mult);
		
		
		static const XMVECTORI row1NegSelect = {XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1W};
		static const XMVECTORI row1ReorderSelect = {XM_PERMUTE_0Y, XM_PERMUTE_0Y, XM_PERMUTE_0Z, XM_PERMUTE_0W};

		const XMVECTOR row1Perm0 = XMVectorPermute(v, negV, row1NegSelect);
		const XMVECTOR row1Perm1 = XMVectorPermute(vZero, row1Perm0, selecty);

		const XMVECTOR vRow1 = XMVectorPermute(v,v, row1ReorderSelect);

		const XMVECTOR row1Mult = XMVectorAdd(vRow1, row1Perm1);

		m.m1 = XMVectorMultiply(XMVectorMultiply(row1h, vy), row1Mult);

		static const XMVECTORI row2NegSelect = {XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_1Z, XM_PERMUTE_1W};

		const XMVECTOR row2Perm0 = XMVectorPermute(v, negV, row2NegSelect);
		const XMVECTOR row2Perm1 = XMVectorPermute(vZero, row2Perm0, selectz);

		const XMVECTOR row2Mult = XMVectorAdd(vz, row2Perm1);

		m.m2 = XMVectorMultiply(XMVectorMultiply(row2h, v), row2Mult);

// 		m00=F(dot+h*v.x*v.x);		m01=F(h*v.x*(v.y-v.z));		m02=F(h*v.x*(v.z+v.y));
// 		m10=F(h*v.x*(v.y+v.z));		m11=F(dot+h*v.y*v.y);		m12=F(h*v.y*(v.z-v.x));
// 		m20=F(h*v.x*(v.z-v.y));		m21=F(h*v.y*(v.z+v.x));		m22=F(dot+h*v.z*v.z);
 	}

	return m;
}

#endif //HWMATRIX_XENON_H
