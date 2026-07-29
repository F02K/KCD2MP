//////////////////////////////////////////////////////////////////////
//
//	Crytek Common Source code
//	
//	File: Cry_Xenon_HWVector3.h
//	Description: hardware vector code
//
//	History:
//	-Mar 4, 2010: Created by Richard Semmens
//
//////////////////////////////////////////////////////////////////////

#ifndef XENON_HWVECTOR_H
#define XENON_HWVECTOR_H

#if _MSC_VER > 1000
# pragma once
#endif

#ifdef XENON_INTRINSICS
#pragma warning(disable: 4800)  // forcing value to bool 'true' or 'false'
#endif

//we need a conversion type to access the members in a XMVECTOR, union is defined inside Cry_PS3_Math.h
#if !defined(PS3)
	#define XMVECTOR_CONV XMVECTOR
#else
	#ifndef __SPU__
		#define __fsel __fsels
	#endif
#endif

#if defined(PS3) && defined(PS3OPT)
	#define USE_PS3_SIMD
#endif

typedef XMVECTOR hwvec3;
typedef XMVECTOR hwvec4;
typedef XMVECTOR simdf;
typedef XMVECTORF32 hwvec4fconst;
typedef XMVECTORI hwvec4i;

#define HWV_PERMUTE_0X XM_PERMUTE_0X 
#define HWV_PERMUTE_0Y XM_PERMUTE_0Y 
#define HWV_PERMUTE_0Z XM_PERMUTE_0Z
#define HWV_PERMUTE_0W XM_PERMUTE_0W
#define HWV_PERMUTE_1X XM_PERMUTE_1X
#define HWV_PERMUTE_1Y XM_PERMUTE_1Y 
#define HWV_PERMUTE_1Z XM_PERMUTE_1Z
#define HWV_PERMUTE_1W XM_PERMUTE_1W

#define HWV3Constant(name, f0, f1, f2) static const hwvec4fconst name = {f0, f1, f2, 0.0f}
#define HWV4Constant(name, f0, f1, f2, f3) static const hwvec4fconst name = {f0, f1, f2, f3}
#define HWV4PermuteControl(name, i0, i1, i2, i3) static const hwvec4i name = {i0, i1, i2, i3};
#define SIMDFConstant(name, f0) static const simdf name = {f0, f0, f0, f0}
#define SIMDFAsVec3(x) (hwvec3)x
#define HWV3AsSIMDF(x) (simdf)x

ILINE hwvec3 HWVLoadVecUnaligned(const Vec3 * pLoadFrom)
{
	return XMLoadVector3(pLoadFrom);
}

ILINE hwvec3 HWVLoadVecAligned(const Vec3 * pLoadFrom)
{
	return XMLoadVector3A(pLoadFrom);
}

ILINE void HWVSaveVecUnaligned(Vec3 * pSaveTo, const hwvec3& pSaveFrom)
{
	XMStoreVector3(pSaveTo, pSaveFrom);
}

ILINE void HWVSaveVecAligned(Vec4 * pSaveTo, const hwvec4& pSaveFrom)
{
	XMStoreVector4A(pSaveTo, pSaveFrom);
}

ILINE hwvec3 HWVAdd(const hwvec3& a, const hwvec3& b )
{
	return XMVectorAdd(a, b);
}

ILINE hwvec3 HWVMultiply(const hwvec3& a, const hwvec3& b )
{
	return XMVectorMultiply(a, b);
}

ILINE hwvec3 HWVMultiplySIMDF(const hwvec3& a, const simdf& b )
{
	return XMVectorMultiply(a, b);
}

ILINE hwvec3 HWVMultiplyAdd(const hwvec3& a, const hwvec3& b, const hwvec3& c )
{
	return XMVectorMultiplyAdd(a, b, c);
}

ILINE hwvec3 HWVMultiplySIMDFAdd(const hwvec3& a, const simdf& b, const hwvec3& c )
{
	return XMVectorMultiplyAdd(a, b, c);
}

ILINE hwvec3 HWVSub(const hwvec3& a, const hwvec3& b )
{
	return XMVectorSubtract(a, b);
}

ILINE hwvec3 HWVCross(const hwvec3& a, const hwvec3& b )
{
	return XMVector3Cross(a, b);
}

ILINE simdf HWV3Dot(const hwvec3& a, const hwvec3& b)
{
	return XMVector3Dot(a, b);
}

ILINE hwvec3 HWVMax(const hwvec3& a, const hwvec3& b)
{
	return XMVectorMax(a, b);
}

ILINE hwvec3 HWVMin(const hwvec3& a, const hwvec3& b)
{
	return XMVectorMin(a, b);
}

ILINE hwvec3 HWVClamp(const hwvec3& a, const hwvec3& min, const hwvec3& max)
{
	return HWVMax(min, HWVMin(a, max));
}

ILINE hwvec3 HWV3Normalize(const hwvec3& a)
{
	return XMVector3Normalize(a);
}

ILINE hwvec3 HWVGetOrthogonal(const hwvec3& a)
{
	return XMVector3Orthogonal(a);
}

ILINE hwvec3 HWV3SplatX(const hwvec3& a)
{
	return XMVectorSplatX(a);
}

ILINE hwvec3 HWV3SplatY(const hwvec3& a)
{
	return XMVectorSplatY(a);
}

ILINE simdf HWV3SplatXToSIMDF(const hwvec3& a)
{
	return XMVectorSplatX(a);
}

ILINE simdf HWV3SplatYToSIMDF(const hwvec3& a)
{
	return XMVectorSplatY(a);
}

ILINE hwvec3 HWV3PermuteWord(const hwvec3& a, const hwvec3& b, const hwvec4i& p)
{
	return XMVectorPermute(a, b, (XMVECTOR)p);
}

ILINE hwvec3 HWV3Zero()
{
	return XMVectorZero();
}

ILINE hwvec4 HWV4Zero()
{
	return XMVectorZero();
}

ILINE hwvec3 HWV3Negate(const hwvec3& a)
{
	return XMVectorNegate(a);
}

ILINE hwvec3 HWVSelect(const hwvec3& a, const hwvec3& b, const hwvec3& control)
{
	return XMVectorSelect(a, b, control);
}

ILINE hwvec3 HWVSelectSIMDF(const hwvec3& a, const hwvec3& b, const simdf& control)
{
	return XMVectorSelect(a, b, control);
}

ILINE simdf HWV3LengthSq(const hwvec3& a)
{
	return XMVector3LengthSq(a);
}


//////////////////////////////////////////////////
//SIMDF functions for float stored in HWVEC4 data
//////////////////////////////////////////////////

ILINE simdf SIMDFLoadFloat(const float& f)
{
	return XMVectorSplatX(XMLoadVector4(&f));
}

ILINE void SIMDFSaveFloat(float * f, const simdf& a)
{
	XMStoreFloat(f, a);
}

ILINE bool SIMDFGreaterThan(const simdf& a, const simdf& b)
{
	return XMVector4Greater(a, b);
}

ILINE bool SIMDFLessThanEqualB(const simdf& a, const simdf& b)
{
	return !XMVector4Less(b, a);
}

ILINE simdf SIMDFLessThanEqual(const simdf& a, const simdf& b)
{
	return XMVectorLessOrEqual(a, b);
}

ILINE bool SIMDFLessThanB(const simdf& a, const simdf& b)
{
	return XMVector4Less(a, b);
}

ILINE simdf SIMDFLessThan(const simdf& a, const simdf& b)
{
	return XMVectorLess(a, b);
}

ILINE simdf SIMDFAdd(const simdf& a, const simdf& b)
{
	return XMVectorAdd(a, b);
}

ILINE simdf SIMDFMult(const simdf& a, const simdf& b)
{
	return XMVectorMultiply(a, b);
}

ILINE simdf SIMDFReciprocal(const simdf& a)
{
	return XMVectorReciprocal(a);
}

ILINE simdf SIMDFSqrt(const simdf& a )
{
	return XMVectorSqrt(a);
}

ILINE simdf SIMDFSqrtEst(const simdf& a )
{
	return XMVectorSqrtEst(a);
}

ILINE simdf SIMDFSqrtEstFast(const simdf& a )
{
	XMVECTOR vRSqrtE = __vrsqrtefp(a);
	return __vmulfp(a, vRSqrtE);
}

ILINE simdf SIMDFMax(const simdf& a, const simdf& b)
{
	return XMVectorMax(a, b);
}

ILINE simdf SIMDFMin(const simdf& a, const simdf& b)
{
	return XMVectorMin(a, b);
}

ILINE simdf SIMDFClamp(const simdf& a, const simdf& min, const simdf& max)
{
	return HWVMax(min, HWVMin(a, max));
}

ILINE simdf SIMDFAbs(const simdf& a)
{
	return XMVectorAbs(a);
}

#endif //vector
