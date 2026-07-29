
//////////////////////////////////////////////////////////////////////
//
//	Crytek Common Source code
//
//	File:Cry_XMath.h
//	Description: Vector SSE mathematical functions
//
//	History:
//	-Dec 01,2007: SSE math optimisations by Andrey Honich
//
//////////////////////////////////////////////////////////////////////

//
#ifndef CRY_XMATH_H
#define CRY_XMATH_H

#if _MSC_VER > 1000
# pragma once
#endif

#ifdef _WIN32
#ifdef XENON_INTRINSICS
#ifdef _CPU_SSE

#include <math.h>
#include <xmmintrin.h>

// 3D Vector; 32 bit floating point components
struct XMFLOAT3
{
  f32 x;
  f32 y;
  f32 z;

  XMFLOAT3() {};
  XMFLOAT3 operator= (const XMFLOAT3& Float3) { x=Float3.x; y=Float3.y; z=Float3.z; return *this; }
};

// 4D Vector; 32 bit floating point components
struct XMFLOAT4
{
  f32 x;
  f32 y;
  f32 z;
  f32 w;

  XMFLOAT4() {};

  XMFLOAT4 operator= (const XMFLOAT4& Float4) { x=Float4.x; y=Float4.y; z=Float4.z; w=Float4.w; return *this; }
};

struct XMVECTOR
{
  union
  {
    __m128 m128;
    struct
    {
      float v[4];
    };
    struct
    {
      float x, y, z, w;
    };
  };
  /* Constructors: __m128, 4 floats, 1 float */
  XMVECTOR() {}

  /* initialize 4 SP FP with __m128 data type */
  XMVECTOR(const XMVECTOR& m)  { m128 = m.m128;}
  XMVECTOR(__m128 m)  { m128 = m;}

  /* initialize 4 SP FPs with 4 floats */
  XMVECTOR(float f3, float f2, float f1, float f0) { m128 = _mm_set_ps(f0,f1,f2,f3); }

  /* Explicitly initialize each of 4 SP FPs with same float */
  explicit XMVECTOR(float f)       { m128 = _mm_set_ps1(f); }

  /* Assignment operations */
  XMVECTOR& operator = (float f) { m128 = _mm_set_ps1(f); return *this; }

  /* Conversion functions */
  operator  __m128() const        { return m128; }         /* Convert to __m128 */

  /* Logical Operators */
  friend XMVECTOR operator & (const XMVECTOR &a, const XMVECTOR &b) { return _mm_and_ps(a,b); }
  friend XMVECTOR operator | (const XMVECTOR &a, const XMVECTOR &b) { return _mm_or_ps(a,b); }
  friend XMVECTOR operator ^ (const XMVECTOR &a, const XMVECTOR &b) { return _mm_xor_ps(a,b); }

  /* Arithmetic Operators */
  friend XMVECTOR operator +(const XMVECTOR &a, const XMVECTOR &b) { return _mm_add_ps(a,b); }
  friend XMVECTOR operator -(const XMVECTOR &a, const XMVECTOR &b) { return _mm_sub_ps(a,b); }
  friend XMVECTOR operator *(const XMVECTOR &a, const XMVECTOR &b) { return _mm_mul_ps(a,b); }
  friend XMVECTOR operator *(const XMVECTOR &a, const float b) { return _mm_mul_ps(a,_mm_set_ps1(b)); }
  friend XMVECTOR operator /(const XMVECTOR &a, const XMVECTOR &b) { return _mm_div_ps(a,b); }
  friend XMVECTOR operator /(const XMVECTOR &a, const float b) { return _mm_div_ps(a,_mm_set_ps1(b)); }

  XMVECTOR& operator =(const XMVECTOR &a) { m128 = a.m128; return *this; }
  XMVECTOR& operator =(const __m128 &avec) { m128 = avec; return *this; }
  XMVECTOR& operator +=(XMVECTOR &a) { return *this = _mm_add_ps(m128,a); }
  XMVECTOR& operator -=(XMVECTOR &a) { return *this = _mm_sub_ps(m128,a); }
  XMVECTOR& operator *=(const XMVECTOR &a) { return *this = _mm_mul_ps(m128,a); }
  XMVECTOR& operator *=(const float f) { return *this = _mm_mul_ps(m128, _mm_set_ps1(f)); }
  XMVECTOR& operator /=(const XMVECTOR &a) { return *this = _mm_div_ps(m128, a); }
  XMVECTOR& operator /=(const float f) { return *this = _mm_div_ps(m128, _mm_set_ps1(f)); }
  XMVECTOR& operator &=(XMVECTOR &a) { return *this = _mm_and_ps(m128,a); }
  XMVECTOR& operator |=(XMVECTOR &a) { return *this = _mm_or_ps(m128,a); }
  XMVECTOR& operator ^=(XMVECTOR &a) { return *this = _mm_xor_ps(m128,a); }

  ILINE XMVECTOR operator - ( void ) const
  {
    return _mm_sub_ps(_mm_setzero_ps(), m128);
  }
};

struct XMMATRIX
{
  union
  {
    struct
    {
      __m128 m128[4];
    };
    struct
    {
      XMVECTOR r[4];
    };
    struct
    {
      float m[4][4];
    };
  };
  /* Constructors: __m128, 4 floats, 1 float */
  XMMATRIX() {}

};

ILINE XMVECTOR XMVectorReplicate(float f)
{
  return _mm_set_ps1(f);
}
ILINE XMVECTOR XMVectorZero()
{
  return _mm_setzero_ps();
}

ILINE bool XMVector3Equal(XMVECTOR& X, XMVECTOR& Y)
{
  __m128 mask = _mm_cmpeq_ps(X, Y);
  int maskBits = _mm_movemask_ps(mask);
  return ((maskBits&7) == 7);
}
ILINE bool XMVector3LessOrEqual(XMVECTOR& X, XMVECTOR& Y)
{
  __m128 mask = _mm_cmple_ps(X, Y);
  int maskBits = _mm_movemask_ps(mask);
  return ((maskBits&7) == 7);
}
ILINE bool XMVector4Less(XMVECTOR& X, XMVECTOR& Y)
{
  __m128 mask = _mm_cmplt_ps(X, Y);
  int maskBits = _mm_movemask_ps(mask);
  return ((maskBits&15) == 15);
}
ILINE XMVECTOR XMVector4Dot(XMVECTOR& X, const XMVECTOR& Y)
{
  XMVECTOR res = X * Y;
  res.x = res.x + res.y + res.z + res.w;
  return res;
}
ILINE XMVECTOR XMVector4Length(XMVECTOR& X)
{
  XMVECTOR res;
  res.x = sqrtf(X.x*X.x + X.y*X.y + X.z*X.z + X.w*X.w);
  return res;
}


ILINE XMVECTOR XMVectorAbs(XMVECTOR& X)
{
  XMVECTOR x;
  x.v[0] = fabsf(X.v[0]);
  x.v[1] = fabsf(X.v[1]);
  x.v[2] = fabsf(X.v[2]);
  x.v[3] = fabsf(X.v[3]);

  return x;
}
ILINE bool XMVector3NearEqual(XMVECTOR& X, XMVECTOR& Y, XMVECTOR& Epsilon)
{
  XMVECTOR dif = XMVectorAbs(X - Y);
  return XMVector3LessOrEqual (dif, Epsilon);
}
ILINE XMVECTOR XMVector3Length(XMVECTOR& X)
{
  XMVECTOR res;
  res.v[0] = sqrtf(X.v[0]*X.v[0] + X.v[1]*X.v[1] + X.v[2]*X.v[2]);
  return res;
}
ILINE XMVECTOR XMVector3LengthEst(XMVECTOR& X)
{
  XMVECTOR res = XMVectorReplicate(sqrtf(X.v[0]*X.v[0] + X.v[1]*X.v[1] + X.v[2]*X.v[2]));
  return res;
}
ILINE XMVECTOR XMVector3LengthSq(XMVECTOR& X)
{
  XMVECTOR res = XMVectorReplicate(X.v[0]*X.v[0] + X.v[1]*X.v[1] + X.v[2]*X.v[2]);
  return res;
}
ILINE XMVECTOR XMVector3Normalize(XMVECTOR& X)
{
  XMVECTOR res;
  float fDist2 = X.v[0]*X.v[0] + X.v[1]*X.v[1] + X.v[2]*X.v[2];
  if (!fDist2)
    return XMVectorZero();
  XMVECTOR Dist = XMVectorReplicate(fDist2);
  Dist.m128 = _mm_sqrt_ps(Dist.m128);
  return X / Dist;
}
ILINE XMVECTOR XMVector3NormalizeEst(XMVECTOR& X)
{
  XMVECTOR res;
  float fDist2 = X.v[0]*X.v[0] + X.v[1]*X.v[1] + X.v[2]*X.v[2];
  if (!fDist2)
    return XMVectorZero();
  XMVECTOR Dist = XMVectorReplicate(fDist2);
  Dist.m128 = _mm_rsqrt_ps(Dist.m128);
  return X * Dist;
}
ILINE XMVECTOR XMVector3Dot(XMVECTOR& X, XMVECTOR& Y)
{
  XMVECTOR res = X * Y;
  res = XMVectorReplicate(res.x + res.y + res.z);
  return res;
}
ILINE XMVECTOR XMVector3Cross(XMVECTOR& v0, XMVECTOR& v1)
{
	return XMVECTOR(v0.y*v1.z-v0.z*v1.y, v0.z*v1.x-v0.x*v1.z, v0.x*v1.y-v0.y*v1.x, 0); 
}

ILINE XMVECTOR XMVector2LengthSq(XMVECTOR& X)
{
  XMVECTOR res = XMVectorReplicate(X.x*X.x + X.y*X.y);
  return res;
}
ILINE XMVECTOR XMVector2Length(XMVECTOR& X)
{
  XMVECTOR res = XMVectorReplicate(sqrtf(X.x*X.x + X.y*X.y));
  return res;
}

//===========================================================================

ILINE float XMFloatReciprocalSqrt(XMVECTOR& v)
{
  return 1.0f / sqrtf(v.x);
}
ILINE float XMFloatReciprocalSqrtEst(XMVECTOR& v)
{
  XMVECTOR res;
  res.m128 = _mm_rsqrt_ss(v);
  return res.v[0];
}

ILINE XMVECTOR XMVectorReciprocalSqrt(XMVECTOR& v)
{
  return _mm_rsqrt_ps(v);
}

ILINE XMVECTOR XMVectorMin(XMVECTOR& X, XMVECTOR& Y)
{
  return _mm_min_ps(X, Y);
}
ILINE XMVECTOR XMVectorMax(XMVECTOR& X, XMVECTOR& Y)
{
  return _mm_max_ps(X, Y);
}
ILINE XMVECTOR XMVectorLerpV(XMVECTOR& X, XMVECTOR& Y, XMVECTOR& t)
{
  return X + t * (Y - X);
}
ILINE XMVECTOR XMVectorLerp(XMVECTOR& X, XMVECTOR& Y, float t)
{
  return XMVectorLerpV(X, Y, XMVectorReplicate(t));
}
ILINE XMVECTOR XMVectorMergeXY(const XMVECTOR& X, const XMVECTOR& Y)
{
  return _mm_unpacklo_ps(X, Y);
}
ILINE XMVECTOR XMVectorMergeZW(const XMVECTOR& X, const XMVECTOR& Y)
{
  return _mm_unpackhi_ps(X, Y);
}

ILINE XMVECTOR XMVectorAdd(XMVECTOR& X, XMVECTOR& Y)
{
  return _mm_add_ps(X, Y);
}
ILINE XMVECTOR XMVectorSubtract(XMVECTOR& X, XMVECTOR& Y)
{
  return _mm_sub_ps(X, Y);
}
ILINE XMVECTOR XMVectorMultiply(XMVECTOR& X, XMVECTOR& Y)
{
  return _mm_mul_ps(X, Y);
}
ILINE XMVECTOR XMVectorMultiplyAdd(XMVECTOR& X, XMVECTOR& Y, XMVECTOR& A)
{
  __m128 r = _mm_mul_ps(X, Y);
  return _mm_add_ps(r, A);
}

ILINE XMVECTOR XMVectorSplatX(XMVECTOR& X)
{
  return _mm_shuffle_ps(X, X, _MM_SHUFFLE(0,0,0,0));
}
ILINE XMVECTOR XMVectorSplatY(XMVECTOR& X)
{
  return _mm_shuffle_ps(X, X, _MM_SHUFFLE(1,1,1,1));
}
ILINE XMVECTOR XMVectorSplatZ(XMVECTOR& X)
{
  return _mm_shuffle_ps(X, X, _MM_SHUFFLE(2,2,2,2));
}
ILINE XMVECTOR XMVectorSplatW(XMVECTOR& X)
{
  return _mm_shuffle_ps(X, X, _MM_SHUFFLE(3,3,3,3));
}

//============================================================================

ILINE XMVECTOR XMLoadVector4A(const void *pV)
{
  return _mm_load_ps((float *)pV);
}
ILINE XMVECTOR XMLoadVector4(const void *pV)
{
  return _mm_loadu_ps((float *)pV);
}

ILINE XMVECTOR XMLoadVector3A(const void *pV)
{
  float *p = (float *)pV;
  XMVECTOR res;
  float v[2] = {p[2], 1};
  res.m128 = _mm_loadl_pi(res.m128, (__m64 *)p);
  res.m128 = _mm_loadh_pi(res.m128, (__m64 *)&v);
  return res;
}
ILINE XMVECTOR XMLoadVector3(const void *pV)
{
  float *p = (float *)pV;
  XMVECTOR res;
  float v[2] = {p[2], 1};
  res.m128 = _mm_loadl_pi(res.m128, (__m64 *)p);
  res.m128 = _mm_loadh_pi(res.m128, (__m64 *)&v);
  return res;
}
ILINE XMVECTOR XMLoadVector2(const void *pV)
{
  float *p = (float *)pV;
  XMVECTOR res = XMVectorZero();
  res.m128 = _mm_loadl_pi(res.m128, (__m64 *)p);
  return res;
}

ILINE void XMStoreVector4A(void *pV, const XMVECTOR& xm)
{
  _mm_store_ps((float *)pV, xm.m128);
}
ILINE void XMStoreVector4(void *pV, const XMVECTOR& xm)
{
  _mm_storeu_ps((float *)pV, xm.m128);
}

ILINE void XMStoreVector3A(void *pV, const XMVECTOR& xm)
{
  float *pF = (float *)pV;
  _mm_storel_pi((__m64 *)pF, xm.m128);
  pF[2] = xm.z;
}
ILINE void XMStoreVector3(void *pV, const XMVECTOR& xm)
{
  float *pF = (float *)pV;
  _mm_storel_pi((__m64 *)pF, xm.m128);
  pF[2] = xm.z;
}

_inline XMMATRIX XMMatrixTranspose(const XMMATRIX&  M)
{
  XMMATRIX R, P;
  P.r[0] = XMVectorMergeXY(M.r[0], M.r[2]); // m00m20m01m21
  P.r[1] = XMVectorMergeXY(M.r[1], M.r[3]); // m10m30m11m31
  P.r[2] = XMVectorMergeZW(M.r[0], M.r[2]); // m02m22m03m23
  P.r[3] = XMVectorMergeZW(M.r[1], M.r[3]); // m12m32m13m33

  R.r[0] = XMVectorMergeXY(P.r[0], P.r[1]); // m00m10m20m30
  R.r[1] = XMVectorMergeZW(P.r[0], P.r[1]); // m01m11m21m31
  R.r[2] = XMVectorMergeXY(P.r[2], P.r[3]); // m02m12m22m32
  R.r[3] = XMVectorMergeZW(P.r[2], P.r[3]); // m03m13m23m33

  return R;
}

#pragma warning(push)
#pragma warning(disable:4700) //  uninitialized local variable used

_inline XMMATRIX XMMatrixInverse(XMVECTOR* pDeterminant, const XMMATRIX&  M)
{
  __m128 minor0, minor1, minor2, minor3;
  __m128 det, tmp1;
  __m128 row0, row1, row2, row3;

  XMMATRIX R;
#ifdef _DEBUG
  tmp1 = _mm_setzero_ps();
  row1 = _mm_setzero_ps();
  row3 = _mm_setzero_ps();
#endif

  tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(&M.m[0][0])), (__m64*)(&M.m[1][0]));
  row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(&M.m[2][0])), (__m64*)(&M.m[3][0]));
  row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
  row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
  tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(&M.m[0][2])), (__m64*)(&M.m[1][2]));
  row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(&M.m[2][2])), (__m64*)(&M.m[3][2]));
  row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
  row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
  // -----------------------------------------------
  tmp1 = _mm_mul_ps(row2, row3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  minor0 = _mm_mul_ps(row1, tmp1);
  minor1 = _mm_mul_ps(row0, tmp1);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
  minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
  minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
  minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
  // -----------------------------------------------
  tmp1 = _mm_mul_ps(row1, row2);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
  minor3 = _mm_mul_ps(row0, tmp1);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
  minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
  minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
  // -----------------------------------------------
  tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  row2 = _mm_shuffle_ps(row2, row2, 0x4E);
  minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
  minor2 = _mm_mul_ps(row0, tmp1);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
  minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
  minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
  minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
  // -----------------------------------------------
  tmp1 = _mm_mul_ps(row0, row1);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
  minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
  minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
  minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
  // -----------------------------------------------
  tmp1 = _mm_mul_ps(row0, row3);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
  minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
  minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
  minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
  // -----------------------------------------------
  tmp1 = _mm_mul_ps(row0, row2);
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
  minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
  minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
  tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
  minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
  minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
  // -----------------------------------------------
  det = _mm_mul_ps(row0, minor0);
  det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
  det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
  __m128 z = _mm_setzero_ps();
  __m128 mask = _mm_cmpeq_ps(det, z);
  int maskBits = _mm_movemask_ps(mask);
  if (maskBits&1)
  {
    if (pDeterminant)
      *pDeterminant = det;
    R.r[0] = z;
    R.r[1] = z;
    R.r[2] = z;
    R.r[3] = z;
    return R;
  }
  tmp1 = _mm_rcp_ss(det);
  det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
  det = _mm_shuffle_ps(det, det, 0x00);
  R.r[0] = _mm_mul_ps(det, minor0);
  R.r[1] = _mm_mul_ps(det, minor1);
  R.r[2] = _mm_mul_ps(det, minor2);
  R.r[3] = _mm_mul_ps(det, minor3);
  if (pDeterminant)
    *pDeterminant = det;

  return R;
}

#pragma warning(pop)

#endif
#endif
#endif

#endif //math

