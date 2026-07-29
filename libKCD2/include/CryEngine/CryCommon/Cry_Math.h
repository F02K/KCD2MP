//////////////////////////////////////////////////////////////////////
//
//	Crytek Common Source code
//
//	File:Cry_Math.h
//	Description: Common math class
//
//	History:
//	-Feb 27,2003: Created by Ivo Herzeg
//
//////////////////////////////////////////////////////////////////////

#ifndef CRYMATH_H
#define CRYMATH_H

#if _MSC_VER > 1000
# pragma once
#endif

//========================================================================================

#include <platform.h>
#include MATH_H
#include "Cry_ValidNumber.h"

#if defined(LINUX)
#include <values.h>
#endif

#include <MetaUtils.h>  // metautils::is_same etc.
#include <CryEndian.h>  // eLittleEndian

#include <CryHalf.inl>

//need mapping for indexed loading

#if defined(PS3)
	#include "console/Cry_PS3_Math.h"
#else
	#if defined(XENON_INTRINSICS)
		ILINE XMVECTOR XMLoadFloat4AIndexed(const XMFLOAT4A* const __restrict pSrc, unsigned int off)
		{
			return XMLoadVector4A(&pSrc[off]);
		}

		ILINE void XMStoreFloat4AIndexed(XMFLOAT4A* const __restrict pDst, XMVECTOR val, unsigned int off)
		{
			XMStoreFloat4A(&pDst[off], val);
		}
	#endif
#endif

#ifdef XENON_INTRINSICS
#include "console/Cry_Xenon_Math.h"
#else 

struct XMVec4
{
};

struct XMVec4A
{
#ifdef XENON
	static ILINE XMVECTOR LoadVec4(const void *pV)
	{
		return XMLoadVector4A(pV);
	}
#endif
	static void *New(size_t s)
	{
#ifndef PS3
		unsigned char *p = ::new unsigned char[s + 16];
		if (p)
		{
			unsigned char offset = (unsigned char)(16 - ((UINT_PTR)p & 15));
			p += offset;
			p[-1] = offset;
		}
#else
	unsigned char *p = ::new(16) unsigned char[s];
#endif
		return p;
	}
	static void Del(void *p)
	{
#ifndef PS3
		if(p)
		{
			unsigned char* pb = static_cast<unsigned char*>(p);
			pb -= pb[-1];
			::delete [] pb;
		}
#else
		::delete[] static_cast<unsigned char*>(p);
#endif
	}
};

#endif

///////////////////////////////////////////////////////////////////////////////
// Forward declarations                                                      //
///////////////////////////////////////////////////////////////////////////////
template <typename F> struct Vec2_tpl;
template <typename F> struct Vec3_tpl;
template <typename F> struct Vec4_tpl;

template <typename F> struct Ang3_tpl;
template <typename F> struct Plane_tpl;
template <typename F> struct AngleAxis_tpl;
template <typename F> struct Quat_tpl;
template <typename F> struct QuatT_tpl;
template <typename F> struct DualQuat_tpl;
template <typename F> struct QuatTS_tpl;

template <typename F> struct Diag33_tpl;
template <typename F> struct Matrix33_tpl;
template <typename F> struct Matrix34_tpl;
template <typename F> struct Matrix44_tpl;

//these types exist only on X360 and PS3
template<typename F> struct Vec4A_tpl;
template<typename F> struct Matrix34A_tpl;
template<typename F> struct Matrix44A_tpl;


///////////////////////////////////////////////////////////////////////////////
// Definitions                                                               //
///////////////////////////////////////////////////////////////////////////////
#define gf_PI  f32(3.14159265358979323846264338327950288419716939937510) // pi
#define gf_PI2 f32(3.14159265358979323846264338327950288419716939937510*2.0) // 2*pi

template<class F> ILINE F sqr(const F &op) { return op*op; }
template<class F> ILINE F cry_sqr(const F &op) { return sqr(op); }

#if defined(PS3) && defined(__SPU__) || defined(REAL_IS_FLOAT)
	#define g_PI gf_PI
	#define g_PI2 gf_PI2
#else
	const f64 g_PI  =  3.14159265358979323846264338327950288419716939937510; // pi
	const f64 g_PI2 =  3.14159265358979323846264338327950288419716939937510*2.0; // 2*pi
#endif
#if defined(__SPU__)
	const f32 sqrt2	= 1.4142135623730950488016887242097f;
	const f32 sqrt3	= 1.7320508075688772935274463415059f;
#else	
	const f64 sqrt2	= f64(1.4142135623730950488016887242097);
	const f64 sqrt3	= f64(1.7320508075688772935274463415059);
#endif

#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))

#define	VEC_EPSILON	( 0.05f )
#define	RAD_EPSILON	( 0.01f )
#define DEG2RAD( a ) ( (a) * (gf_PI/180.0f) )
#define RAD2DEG( a ) ( (a) * (180.0f/gf_PI) )
#define DEG2COS( a ) ( cos_tpl( (a)*(gf_PI/180.0f) ) )
#define COS2DEG( a ) ( acos_tpl(a)*(180.0f/gf_PI)  )
#define RAD2HCOS( a ) ( cos_tpl( (a*0.5f) ) )
#define HCOS2RAD( a ) ( acos_tpl(a)*2.0f  )
#define DEG2HCOS( a ) ( cos_tpl( (a*0.5f)*(gf_PI/180.0f) ) )
#define HCOS2DEG( a ) ( acos_tpl(a)*2.0f*(180.0f/gf_PI)  )

// 32/64 Bit versions.
#define SIGN_MASK(x) ((intptr_t)(x) >> ((sizeof(size_t)*8)-1))

#define TANGENT30 0.57735026918962576450914878050196f // tan(30)
#define TANGENT30_2 0.57735026918962576450914878050196f*2 // 2*tan(30)

#define LN2 0.69314718055994530941723212145818f // ln(2)

//////////////////////////////////////////////////////////////////////////
// Define min/max
//////////////////////////////////////////////////////////////////////////
#ifdef min
#undef min
#endif //min

#ifdef max
#undef max
#endif //max

#if !defined(XENON) && !defined(PS3)
ILINE f32 __fsel(const f32 _a, const f32 _b, const f32 _c) { return (_a < 0.0f) ? _c : _b;}
ILINE f64 __fsel(const f64 _a, const f64 _b, const f64 _c) { return (_a < 0.0f) ? _c : _b;}
ILINE f32 __fself(const f32 _a, const f32 _b, const f32 _c) { return (_a < 0.0f) ? _c : _b;}
ILINE f32 __fsels(const f32 _a, const f32 _b, const f32 _c) { return (_a < 0.0f) ? _c : _b;}
ILINE f32 __fres(const f32 _a) { return 1.f / _a; }

ILINE f64 sqrt_tpl(f64);
ILINE f32 sqrt_tpl(f32);
ILINE f64 isqrt_tpl(f64 );
ILINE f32 isqrt_tpl(f32);
ILINE f32 __fsqrts(const f32 x) { return sqrt_tpl(x); }
ILINE f64 __frsqrte(const f64 x) { return isqrt_tpl(x); }
ILINE f32 __frsqrte(const f32 x) { return isqrt_tpl(x); }
#endif


// equivalent to MIN(a, b); [branchless]
ILINE int min_branchless(int a, int b)
{
	int diff = a - b;
	int mask = diff >> 31; // sign extend
	return (b & (~mask)) | (a & mask); 
}

#if defined(XENON) || (defined(PS3) && !defined(__SPU__))
ILINE f64 min(f64 op1,f64 op2) { return __fsel(op2-op1,op1,op2); }
ILINE f64 max(f64 op1,f64 op2) { return __fsel(op1-op2,op1,op2); }
ILINE f32 min(f32 op1,f32 op2) { return (f32)__fsel(op2-op1,op1,op2); }
ILINE f32 max(f32 op1,f32 op2) { return (f32)__fsel(op1-op2,op1,op2); }
ILINE f32 clamp( f32 X, f32 Min, f32 Max ) { return min(max(X,Min),Max); }
ILINE f64 clamp( f64 X, f64 Min, f64 Max ) { return min(max(X,Min),Max); } 
ILINE void Limit( f32& X, const f32 Min, const f32 Max ) { X=clamp(X,Min,Max ); }
ILINE void Limit( f64& X, const f64 Min, const f64 Max ) { X=clamp(X,Min,Max ); }
#else
ILINE f32 clamp( f32 X, f32 Min, f32 Max ) 
{	
	return X<Min ? Min : X<Max ? X : Max;
}
ILINE f64 clamp( f64 X, f64 Min, f64 Max ) 
{	
	return X<Min ? Min : X<Max ? X : Max;
}
#endif

template<class T> ILINE void Limit(T& val, const T& min, const T& max)
{
	if (val < min) val = min;
	else if (val > max)	val = max;
}

template<class T> ILINE T clamp_tpl( T X, T Min, T Max ) 
{	
	return X<Min ? Min : X<Max ? X : Max; 
}

#if defined(XENON) || (defined(PS3) && !defined(__SPU__))
template<> ILINE void Limit<float>(float& val, const float& min, const float& max)
{
	assert(min <= max);
	float v = (float)__fsel(val-min, __fsel(max-val, val, max), min);
	val = v;
}

template<> ILINE void Limit<double>(double& val, const double& min, const double& max)
{
	assert(min <= max);
	double v = __fsel(val-min, __fsel(max-val, val, max), min);
	val = v;
}

template<> ILINE int clamp_tpl<int>( int X, int Min, int Max ) 
{	
	//implement branchfree using masks
	const uint32 minMask = (uint32)((Min-X) >> 31);
	const uint32 maxMask = (uint32)((X-Max) >> 31);
	int res = minMask & X | ~minMask & Min;
	res = maxMask & res | ~maxMask & Max;
	return res;
}

template<> ILINE uint32 clamp_tpl<uint32>( uint32 X, uint32 Min, uint32 Max ) 
{	
	//implement branchfree using masks
	const uint32 minMask = (uint32)(((int64)Min-(int64)X) >> 63);
	const uint32 maxMask = (uint32)(((int64)X-(int64)Max) >> 63);
	uint32 res = (uint32)(minMask & X | ~minMask & Min);
	res = maxMask & res | ~maxMask & Max;
	return res;
}

template<> ILINE float clamp_tpl<float>( float X, float Min, float Max ) 
{	
	return (float)__fsel(X-Min, __fsel(X-Max,Max,X), Min);
}

template<> ILINE double clamp_tpl<double>( double X, double Min, double Max ) 
{	
	return __fsel(X-Min, __fsel(X-Max,Max,X), Min);
}
#endif

#if defined(__CRYCG__)
// GCC generates very bad (slow) code for the template versions of min() and
// max() below, so we'll provided overloads for all primitive types.
#define _MINMAXFUNC(TYPE) \
	ILINE TYPE min(TYPE a, TYPE b) { return a < b ? a : b; } \
	ILINE TYPE max(TYPE a, TYPE b) { return b < a ? a : b; }

_MINMAXFUNC(int) _MINMAXFUNC(unsigned)
#if !defined(XENON)
#if !defined(PS3) || defined(__SPU__)	
_MINMAXFUNC(float)
_MINMAXFUNC(double)
#endif
#endif

ILINE int min(int a, unsigned b) { return min(a, static_cast<int>(b)); }
ILINE int min(unsigned a, int b) { return min(static_cast<int>(a), b); }
ILINE unsigned max(int a, unsigned b) { return max(static_cast<unsigned>(a), b); }
ILINE unsigned max(unsigned a, int b) { return max(a, static_cast<unsigned>(b)); }

#undef _MINMAXFUNC
#endif//__CRYCG__

#ifndef __CRYCG__
// Bring min and max from std namespace to global scope.
template <class T>
ILINE T min( const T& a, const T& b )
{
	return b < a ? b : a;
}

template <class T>
ILINE T max( const T& a, const T& b )
{
	return a < b ? b : a;
}

template <class T, class _Compare>
ILINE const T& min( const T& a, const T& b, _Compare comp)
{
	return comp(b,a) ? b : a;
}

template <class T, class _Compare>
ILINE const T& max( const T& a, const T& b, _Compare comp)
{
	return comp(a,b) ? b : a;
}
#endif

// Perform a safe division, with specified maximum quotient.
// Returns min( n/d, m )
#if !defined(__CRYCG__) && (defined(XENON) || (defined(PS3) && !defined(__SPU__)))
ILINE float div_min(float n, float d, float m)
{
	//potentially use __fres instead of fdiv
 	float safe_divisor = (float)__fsel(-fabsf(d), FLT_MIN, d );
 	return (float)__fsel(-((m*d*d) - (n*d)), m, n / safe_divisor );
	//return n*d < m*d*d ? n/d : m;
}
#else
ILINE float div_min(float n, float d, float m)
{
	return n*d < m*d*d ? n/d : m;
}
#endif


ILINE float CorrectInvSqrt(float fNum, float fInvSqrtEst)
{
	// Newton-Rhapson method for improving estimated inv sqrt.
	/*
		f(x) = x^(-1/2)
		f(n) = f(a) + (n-a)f'(a)
				 = a^(-1/2) + (n-a)(-1/2)a^(-3/2)
				 = a^(-1/2)*3/2 - na^(-3/2)/2
				 = e*3/2 - ne^3/2
	*/
	return fInvSqrtEst * (1.5f - fNum * fInvSqrtEst * fInvSqrtEst * 0.5f);
}

//////////////////////////////////////////////////////////////////////////

//-------------------------------------------
//-- the portability functions for AMD64
//-------------------------------------------
#if defined(WIN64) &&  defined(_CPU_AMD64) && !defined(LINUX) && !defined(APPLE)
/*
extern "C" void fastsincosf(f32 x, f32 * sincosfx);
extern "C" f32 fastsinf(f32 x);
extern "C" f32 fastcosf(f32 x);

ILINE void cry_sincosf (f32 angle, f32* pCosSin) {	fastsincosf(angle,pCosSin);	}
ILINE void cry_sincos  (f64 angle, f64* pCosSin) {	pCosSin[0] = cos(angle);	pCosSin[1] = sin(angle); }
ILINE f32 cry_sinf(f32 x) {return fastsinf(x); }
ILINE f32 cry_cosf(f32 x) {return fastcosf(x); }

ILINE f32 cry_fmod(f32 x, f32 y) {return (f32)fmod((f64)x,(f64)y);}

ILINE f32 cry_asinf(f32 x) {return f32(   asin( clamp((f64)x,-1.0,+1.0) )    );}
ILINE f32 cry_acosf(f32 x) {return f32(   acos( clamp((f64)x,-1.0,+1.0) )    );}
ILINE f32 cry_atanf(f32 x) {return (f32)atan((f64)x);}
ILINE f32 cry_atan2f(f32 y, f32 x) {return (f32)atan2((f64)y,(f64)x);}

ILINE f32 cry_tanhf(f32 x) {	f64 expz = exp(f64(x)), exp_z = exp(-f64(x));	return (f32)((expz-exp_z)/(expz+exp_z)); }
ILINE f32 cry_tanf(f32 x) {return (f32)tan((f64)x);}

ILINE f32 cry_sqrtf(f32 x) {return (f32)sqrt((f64)x);}
ILINE f32 cry_isqrtf(f32 x) {return 1.f/cry_sqrtf(x);}
ILINE f32 cry_isqrtf_fast(f32 x) {return 1.f/cry_sqrtf(x);}
ILINE f32 cry_isqrtf_corrected(f32 x) {return 1.f/cry_sqrtf(x);}
ILINE f32 cry_fabsf(f32 x) {return (f32)fabs((f64)x);}
ILINE f32 cry_expf(f32 x) {return (f32)exp((f64)x);}
ILINE f32 cry_logf(f32 x) {return (f32)log((f64)x);}
ILINE f32 cry_powf(f32 x, f32 y) {return (f32) pow((f64)x,(f64)y);}

ILINE f32 cry_ceilf(f32 x) {return (f32)ceil((f64)x);}
ILINE f32 cry_floorf(f32 x) {return (f32)floor((f64)x);}

ILINE f64 cry_sinh(f64 z) {return (exp (z) - exp (-z)) * 0.5;}
ILINE f64 cry_cosh(f64 z) {return (exp (z) + exp (-z)) * 0.5;}
*/
#endif



//-------------------------------------------
//-- the portability functions for CPU_X86
//-------------------------------------------
#if defined(_CPU_X86) && defined(_MSC_VER) && !defined(_DEBUG) && !defined(LINUX) && !defined(APPLE)


// calculates the cosine and sine of the given angle in radians 
ILINE void cry_sincosf (f32 angle, f32* pSin, f32* pCos) {
	__asm {
		FLD         DWORD PTR       angle
			FSINCOS
			MOV         eAX,pCos
			MOV         eDX,pSin
			FSTP        DWORD PTR [eAX]	//store cosine
			FSTP        DWORD PTR [eDX]	//store sine
	}
}

// calculates the cosine and sine of the given angle in radians 
ILINE void cry_sincos (f64 angle, f64* pSin, f64* pCos) {
	__asm {
		FLD         QWORD PTR       angle
			FSINCOS
			MOV         eAX,pCos
			MOV         eDX,pSin
			FSTP        QWORD PTR [eAX]	//store cosine
			FSTP        QWORD PTR [eDX]	//store sine
	}
}

#include <xmmintrin.h>

ILINE f32 cry_sqrtf(f32 x) 
{
	__m128 s = _mm_sqrt_ss(_mm_set_ss(x));
	float r; _mm_store_ss(&r, s); 
	return r;
}
ILINE f32 cry_sqrtf_fast(f32 x)
{
	__m128 s = _mm_rcp_ss(_mm_rsqrt_ss(_mm_set_ss(x)));
	float r; _mm_store_ss(&r, s); 
	return r;
}
ILINE f32 cry_isqrtf(f32 x) 
{
	__m128 s = _mm_rsqrt_ss(_mm_set_ss(x));
	float r; _mm_store_ss(&r, s);
	return CorrectInvSqrt(x, r);
}
ILINE f32 cry_isqrtf_fast(f32 x) 
{
	__m128 s = _mm_rsqrt_ss(_mm_set_ss(x));
	float r; _mm_store_ss(&r, s);
	return r;
}
ILINE f32 cry_isqrtf_corrected(f32 x) 
{
	return cry_isqrtf(x);
}

#else // #if defined(_CPU_X86) && defined(_MSC_VER) && !defined(LINUX)

//-------------------------------------------
//-- Portable version.
//-------------------------------------------
#if defined(PS3) && defined(PS3OPT) && !defined(__SPU__) && !defined(SUPP_FP_EXC)
#else
	ILINE void cry_sincosf (f32 angle, f32* pSin, f32* pCos) {	*pSin = f32(sin(angle));	*pCos = f32(cos(angle));	}
	ILINE void cry_sincos  (f64 angle, f64* pSin, f64* pCos) {	*pSin = f64(sin(angle));  *pCos = f64(cos(angle));	}
#endif

#	if defined(DURANGO)
	ILINE f32 cry_sqrtf(f32 x) 
	{
		__m128 s = _mm_sqrt_ss(_mm_set_ss(x));
		float r; _mm_store_ss(&r, s); 
		return r;
	}
	ILINE f32 cry_sqrtf_fast(f32 x) {return cry_sqrtf(x);}
	ILINE f32 cry_isqrtf(f32 x) {return 1.f/cry_sqrtf(x);}
#	else
	ILINE f32 cry_sqrtf(f32 x) {return sqrtf(x);}
	ILINE f32 cry_sqrtf_fast(f32 x) {return sqrtf(x);}
	ILINE f32 cry_isqrtf(f32 x) {return 1.f/sqrtf(x);}
#	endif

#	if !defined(XENON)
	ILINE f32 cry_isqrtf_fast(f32 x) {return 1.f/sqrtf(x);}
	ILINE f32 cry_isqrtf_corrected(f32 x) {return 1.f/sqrtf(x);}
#	else
	ILINE f32 cry_isqrtf_fast(f32 x) {return (f32)__frsqrte((f32)x);}
	ILINE f32 cry_isqrtf_corrected(f32 x) {f32 r = (f32)__frsqrte((f32)x); return CorrectInvSqrt(x, r); }
#	endif

#endif

#if !defined(PS3) || !defined(PS3OPT) || defined(__SPU__) || defined(SUPP_FP_EXC)
	ILINE f32 cry_sinf(f32 x) {return sinf(x);}
	ILINE f32 cry_cosf(f32 x) {return cosf(x);}
	ILINE f32 cry_asinf(f32 x) {return asinf( clamp(x,-1.0f,+1.0f) );}
	ILINE f32 cry_acosf(f32 x) {return acosf( clamp(x,-1.0f,+1.0f) );}
	ILINE f32 cry_atanf(f32 x) {return atanf(x);}
	ILINE f32 cry_atan2f(f32 y, f32 x) {return atan2f(y,x);}
	ILINE f32 cry_tanf(f32 x) {return tanf(x);}
#endif
ILINE f32 cry_fmod(f32 x, f32 y) {return (f32)fmodf(x,y);}
ILINE f32 cry_ceilf(f32 x) {return ceilf(x);}
ILINE f32 cry_tanhf(f32 x) {return tanhf(x);}
ILINE f32 cry_fabsf(f32 x) {return fabsf(x);}
ILINE f32 cry_expf(f32 x) {return expf(x);}
ILINE f32 cry_logf(f32 x) {return logf(x);}
ILINE f32 cry_floorf(f32 x) {return floorf(x);}
ILINE f32 cry_powf(f32 x, f32 y) {return powf(x,y);}


//-----------------------------------------------------------------------

ILINE void sincos_tpl(f64 angle, f64* pSin,f64* pCos) { cry_sincos(angle,pSin,pCos); }
ILINE void sincos_tpl(f32 angle, f32* pSin,f32* pCos) { cry_sincosf(angle,pSin,pCos); }

#if !defined(PS3) || !defined(PS3OPT) || defined(__SPU__) || defined(SUPP_FP_EXC)
	ILINE f64 cos_tpl(f64 op) { return cos(op); }
	ILINE f64 sin_tpl(f64 op) { return sin(op); }
	ILINE f64 acos_tpl(f64 op) { return acos( clamp(op,-1.0,+1.0) ); }
	ILINE f64 asin_tpl(f64 op) { return asin( clamp(op,-1.0,+1.0) ); }
	ILINE f64 atan_tpl(f64 op) { return atan(op); }
	ILINE f64 atan2_tpl(f64 op1,f64 op2) { return atan2(op1,op2); }
	ILINE f64 tan_tpl(f64 op) {return tan(op);}
#endif

ILINE f32 cos_tpl(f32 op) { return cry_cosf(op); }
ILINE f32 sin_tpl(f32 op) { return cry_sinf(op); }
ILINE f32 acos_tpl(f32 op) { return cry_acosf(op); }
ILINE f32 asin_tpl(f32 op) { return cry_asinf(op); }
ILINE f32 atan_tpl(f32 op) { return cry_atanf(op); }
ILINE f32 atan2_tpl(f32 op1,f32 op2) { return cry_atan2f(op1,op2); }
ILINE f32 tan_tpl(f32 op) {return cry_tanf(op);}

ILINE f64 exp_tpl(f64 op) { return exp(op); }
ILINE f32 exp_tpl(f32 op) { return cry_expf(op); }

ILINE f64 log_tpl(f64 op) { return log(op); }
ILINE f32 log_tpl(f32 op) { return cry_logf(op); }

ILINE f64 sqrt_tpl(f64 op) { return sqrt(op); }
ILINE f32 sqrt_tpl(f32 op) { return cry_sqrtf(op); }

// Only f32 version implemented, as it's approximate.
ILINE f32 sqrt_fast_tpl(f32 op) { return cry_sqrtf_fast(op); }

ILINE f64 isqrt_tpl(f64 op) { return 1.0/sqrt(op); }
ILINE f32 isqrt_tpl(f32 op) { return cry_isqrtf(op); }

ILINE f32 isqrt_fast_tpl(f32 op) { return cry_isqrtf_fast(op); }
ILINE f32 isqrt_corrected_tpl(f32 op) { return cry_isqrtf_corrected(op); }

ILINE f64 isqrt_safe_tpl(f64 op) { return 1.0/sqrt(op + DBL_MIN); }
ILINE f32 isqrt_safe_tpl(f32 op) { return cry_isqrtf(op + FLT_MIN); }

ILINE f64 fabs_tpl(f64 op) { return fabs(op); }
ILINE f32 fabs_tpl(f32 op) { return cry_fabsf(op); }
ILINE int32 fabs_tpl(int32 op) { int32 mask=op>>31; return op+mask^mask; }

ILINE int32 floor_tpl(int32 op) {return op;}
ILINE f32 floor_tpl(f32 op) {return cry_floorf(op);}
ILINE f64 floor_tpl(f64 op) {return floor(op);}

ILINE int32 ceil_tpl(int32 op) {return op;}
ILINE f32 ceil_tpl(f32 op) {return cry_ceilf(op);}
ILINE f64 ceil_tpl(f64 op) {return ceil(op);}

// Fast float-int rounding functions.
#if defined(WIN32) && defined(_CPU_X86)
  ILINE int32 int_round(f32 f)
  {
    int32 i;
    __asm fld [f]
    __asm fistp [i]
    return i;
  }
	ILINE int32 pos_round(f32 f)  { return int_round(f); }
#elif defined(PS3) && defined(PS3OPT) && !defined(__SPU__) && !defined(SUPP_FP_EXC)
	ILINE int32 int_round(f32 f)  { return static_cast<int32>(__fcfid(__fctid(static_cast<double>(f))));}
	ILINE int32 pos_round(f32 f)  { return int32(f+0.5f); }
#elif defined(XENON)
	ILINE int32 int_round(f32 f)
	{
		int32 iNumber;
		int* p = &iNumber;
		__stfiwx( __fctiw( f ), 0, p );
		return iNumber;
	}
	ILINE int32 pos_round(f32 f)  { return int32(f+0.5f); }
#else
	ILINE int32 int_round(f32 f)  { return f < 0.f ? int32(f-0.5f) : int32(f+0.5f); }
	ILINE int32 pos_round(f32 f)  { return int32(f+0.5f); }
#endif

ILINE int64 int_round(f64 f)  { return f < 0.0 ? int64(f-0.5) : int64(f+0.5); }
ILINE int64 pos_round(f64 f)  { return int64(f+0.5); }

// Faster than using std ceil().
ILINE int32 int_ceil(f32 f)
{
	int32 i = int32(f);
	return (f > f32(i)) ? i+1 : i;
}
ILINE int64 int_ceil(f64 f)
{
	int64 i = int64(f);
	return (f > f64(i)) ? i+1 : i;
}

// Fixed-int to float conversion.
ILINE float ufrac8_to_float( float u )
{
	return u * (1.f/255.f);
}
ILINE float ifrac8_to_float( float i )
{
	return i * (1.f/127.f);
}
ILINE uint8 float_to_ufrac8( float f )
{
	int i = pos_round(f * 255.f);
	assert(i >= 0 && i < 256);
	return uint8(i);
}
ILINE int8 float_to_ifrac8( float f )
{
	int i = int_round(f * 127.f);
	assert(abs(i) <= 127);
	return int8(i);
}


#if defined(__SPU__)
struct imod
{
  ILINE int32 operator[] (const int32 i) const { return i+1 & (i-2)>>31; };
}; 
struct dmod
{
  ILINE int32 operator[] (const int32 i) const { return i-1 + ((i-1)>>31&3); };
}; 
#define inc_mod3 ({ imod(); })
#define dec_mod3 ({ dmod(); })
#else
static int32 inc_mod3[]={1,2,0}, dec_mod3[]={2,0,1};
#endif
#ifdef PHYSICS_EXPORTS
#define incm3(i) inc_mod3[i]
#define decm3(i) dec_mod3[i]
#else
ILINE int32 incm3(int32 i) { return i+1 & (i-2)>>31; }
ILINE int32 decm3(int32 i) { return i-1 + ((i-1)>>31&3); }
#endif

//for int->float conversions we have to use a float vector
//simple *(float*)&<float vec> does the job of converting to single float
//for all platforms but PPU it simply evaluates to float
#if defined(PS3) && !defined(__SPU__)
	typedef	vec_float4 floatv;
#else
	typedef	float floatv;
#endif

//converts float to int using vector registers
//	it should not be used for most recently created integer vars,
//	rather for integers needed to be loaded anyway and then used as float
//any implementation for 360 is welcome
ILINE floatv itof(int i)
{
#if defined(PS3) && !defined(__SPU__)
	__vecreg int _i = i;
	vec_int4		i4	= (vec_int4)_i;
	floatv			f4	= vec_ctf(i4,0);
	return f4;
#else
	return (floatv)i;
#endif
}

//provides fast way of checking against 0 (saves fcmp)
ILINE bool fzero(const float& val)
{
#if !defined(__SPU__)
	union {const float * f; const DWORD * dw;} u; u.f = &val;
	return *u.dw == 0;
#else
	return val == 0.0f;
#endif
}

ILINE bool fzero(float* pVal)
{
#if !defined(__SPU__)
	union {float *pF; DWORD *pDW;} u; u.pF = pVal;
	return *u.pDW == 0;
#else
	return *pVal == 0.0f;
#endif
}

//---------------------Integer mask assignments-----------------------
//	implements branch-free c?a:b
template<class T> ILINE T iselnz(int c, T a, T b)
{
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
	assert(sizeof(T) <= sizeof(int));	uint32 m=(c|-c)>>31;return (T)(m&(uint32)a|~m&(uint32)b);
#else
	return c?a:b;
#endif
}
template<class T> ILINE T iselnz(uint32 c, T a, T b)
{
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
	assert(sizeof(T) <= sizeof(int));	uint32 m=((int)c|-(int)c)>>31;return (T)(m&(uint32)a|~m&(uint32)b);
#else
	return c?a:b;
#endif
}
template<class T> ILINE T iselnz(int64 c, T a, T b)
{
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
	assert(sizeof(T) <= sizeof(int64));	uint64 m=(c|-c)>>63;return (T)(m&(uint64)a|~m&(uint64)b);
#else
	return c?a:b;
#endif
}
template<class T> ILINE T iselnz(uint64 c, T a, T b)
{
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
	assert(sizeof(T) <= sizeof(uint64));	uint64 m=((int64)c|-(int64)c)>>63;return (T)(m&(uint64)a|~m&(uint64)b);
#else
	return c?a:b;
#endif
}
//	implements branch-free c < 0?b:a (same as fsel for fp)
template<class T> ILINE T isel(int c, T a, T b)
{
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
	assert(sizeof(T) <= sizeof(int));	uint32 m=(uint32)((((int)c))>>31);return (T)(m&(uint32)b|~m&(uint32)a);
#else
	return (c<0)?b:a;
#endif
}
template<class T> ILINE T isel(int64 c, T a, T b)
{
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
	assert(sizeof(T) <= sizeof(int64));	uint64 m=(uint64)((((int64)c))>>63);return (T)(m&(uint64)b|~m&(uint64)a);
#else
	return (c<0)?b:a;
#endif
}

//add mapping similar to __fsel
#define __isel isel
#define __iselns iselns

//implement signed min/max operations branch-free
//only for consoles as pc seems to cope better with it
#if defined(XENON) || (defined(PS3) && !defined(__CRYCG__))
ILINE uint64 min(uint64 op1,uint64 op2) { return op1 < op2 ? op1 : op2; }//out of scope for arithmetic shifts
ILINE uint64 max(uint64 op1,uint64 op2) { return op1 < op2 ? op2 : op1; }//out of scope for arithmetic shifts
ILINE uint32 min(uint32 op1,uint32 op2) { return isel((int64)op1-(int64)op2,op2,op1); }
ILINE uint32 max(uint32 op1,uint32 op2) { return isel((int64)op1-(int64)op2,op1,op2); }
ILINE uint16 min(uint16 op1,uint16 op2) { return isel((int)op1-(int)op2,op2,op1); }
ILINE uint16 max(uint16 op1,uint16 op2) { return isel((int)op1-(int)op2,op1,op2); }
ILINE uint8 min(uint8 op1,uint8 op2) { return isel((int)op1-(int)op2,op2,op1); }
ILINE uint8 max(uint8 op1,uint8 op2) { return isel((int)op1-(int)op2,op1,op2); }
ILINE int64 min(int64 op1,int64 op2) { return isel(op1-op2,op2,op1); }
ILINE int64 max(int64 op1,int64 op2) { return isel(op1-op2,op1,op2); }
ILINE int min(int op1,int op2) { return isel(op1-op2,op2,op1); }
ILINE int max(int op1,int op2) { return isel(op1-op2,op1,op2); }
ILINE int16 min(int16 op1,int16 op2) { return isel(op1-op2,op2,op1); }
ILINE int16 max(int16 op1,int16 op2) { return isel(op1-op2,op1,op2); }
ILINE int8 min(int8 op1,int8 op2) { return isel(op1-op2,op2,op1); }
ILINE int8 max(int8 op1,int8 op2) { return isel(op1-op2,op1,op2); }
ILINE void Limit( int8& X, const int8 Min, const int8 Max ) { X=clamp_tpl(X,Min,Max ); }
ILINE void Limit( int16& X, const int16 Min, const int16 Max ) { X=clamp_tpl(X,Min,Max ); }
ILINE void Limit( int& X, const int Min, const int Max ) { X=clamp_tpl(X,Min,Max ); }
ILINE void Limit( int64& X, const int64 Min, const int64 Max ) { X=clamp_tpl(X,Min,Max ); }
#endif

//--------------------------------------------------------------------

template<class F> ILINE F square(F fOp) { return(fOp*fOp); }

template<class F> ILINE F cube(const F &op) { return op*op*op; }
template<class F> ILINE F sqr_signed(const F &op) { return op*fabs_tpl(op); }

#if defined(PS3) && !defined(__SPU__)
	#define __fsel __fsels
#endif

#if defined(XENON) || (defined(PS3) && !defined(__SPU__))
	template<class F> struct sgnnz_result {
		F x;
		explicit sgnnz_result(F _x) { x=_x; }
		//template<class T> sgnnz_result<F> operator*(T op) const { return sgnnz_result<F>(op*x); }
		sgnnz_result<F> operator*(const sgnnz_result<F> &op) const { return sgnnz_result<F>(x*op.x); }
		sgnnz_result<F> operator-() const { return sgnnz_result<F>(-x); }
		float fval() const { return __fsel(x,1,-1); }
		operator int() const { return (int)__fsel(x,1,-1); }
	};
	template<class F> ILINE sgnnz_result<F> sgnnz(F x) { return sgnnz_result<F>(x); }
	#define sgnnz_result_type(F) sgnnz_result<F>

	template<class F> struct isneg_result {
		F x;
		explicit isneg_result(F _x) { x=_x; }
		isneg_result<F> operator&(const isneg_result<F> &op) const { return isneg_result<F>(__fsel(x-op.x,x,op.x)); }
		isneg_result<F> operator|(const isneg_result<F> &op) const { return isneg_result<F>(__fsel(x-op.x,op.x,x)); }
		template<class T> T operator*(const T &op) { return (T)(op*fval()); }
		float fval() const { return __fsel(x,0,1); }
		operator int() const { return x>=(F)0 ? 0 : 1; }
	};
	template<class F> ILINE isneg_result<F> isneg(F x) { return isneg_result<F>(x); }
	#define isneg_result_type(F) isneg_result<F>
#else
	#define sgnnz_result_type(F) int
	#define isneg_result_type(F) int
	ILINE int32 sgnnz(f64 x) {
		union { f32 f; int32 i; } u;
		u.f=(f32)x; return ((u.i>>31)<<1)+1;
	}
	ILINE int32 sgnnz(f32 x) {
		union { f32 f; int32 i; } u;
		u.f=x; return ((u.i>>31)<<1)+1;
	}	

	ILINE int32 isneg(f64 x) {
		union { f32 f; uint32 i; } u;
		u.f=(f32)x; return (int32)(u.i>>31);
	}
	ILINE int32 isneg(f32 x) {
		union { f32 f; uint32 i; } u;
		u.f=x; return (int32)(u.i>>31);
	}
#endif
	ILINE int32 isneg(int32 x) {
		return (int32)((uint32)x>>31);
	}

//integer branch removal: (a>=0) ? 1 : -1;
ILINE int32 sgnnz(int32 x) {
	return ((x>>31)<<1)+1;
}

//float branch removal: (a>=0) ? 1 : -1;
#if (defined(PS3) && !defined(__SPU__)) || defined(XENON)
static ILINE float fsgnnz(f32 x)
{
	return (float)__fsel(x,1.0f,-1.0f);
}
#else
static ILINE float fsgnnz(f32 x)
{
	union { f32 f; int32 i; } u;
	u.f=x;
	u.i=(u.i&0x80000000)|0x3f800000;
	return u.f;
}
#endif




ILINE int32 sgn(f64 x) {
	union { f32 f; int32 i; } u;
	u.f=(f32)x; return (u.i>>31)+((u.i-1)>>31)+1;
}
ILINE int32 sgn(f32 x) {
	union { f32 f; int32 i; } u;
	u.f=x; return (u.i>>31)+((u.i-1)>>31)+1;
}


#if (defined(PS3) && !defined(__SPU__)) || defined(XENON)
static ILINE float fsgnf(f32 x)
{
	return (float)__fsel(x, (float)__fsel(-x, 0.f, 1.f), -1.f);
}
#else
static ILINE float fsgnf(f32 x)
{
	return (float)sgn(x);
}
#endif

ILINE int32 isnonneg(f64 x) {
	union { f32 f; uint32 i; } u;
	u.f=(f32)x; return (int32)(u.i>>31^1);
}
ILINE int32 isnonneg(f32 x) {
	union { f32 f; uint32 i; } u;
	u.f=x; return (int32)(u.i>>31^1);
}
ILINE int32 iszero(f64 x) {
	union { f32 f; int32 i; } u;
	u.f=(f32)x;
	u.i&=0x7FFFFFFF;
	return -((u.i>>31)^(u.i-1)>>31);
}
ILINE int32 iszero(f32 x) {
	union { f32 f; int32 i; } u;
	u.f=x; u.i&=0x7FFFFFFF; return -(u.i>>31^(u.i-1)>>31);
}
ILINE int32 sgn(int32 x) {
	return (x>>31)+((x-1)>>31)+1;
}
ILINE int32 isnonneg(int32 x) {
	return (int32)((uint32)x>>31^1);
}
ILINE int32 iszero(int32 x) {
	return -(x>>31^(x-1)>>31);
}
//long isn't always 32bits
#if defined(LINUX) && !defined(LINUX64)
ILINE int32 iszero(long x) {
	return -(x>>31^(x-1)>>31);
}
#endif
#if defined(PLATFORM_64BIT) && !defined(__clang__)
// AMD64 port: TODO: optimize
ILINE int64 iszero(__int64 x) 
{
	return -(x>>63^(x-1)>>63);
}
#elif defined(PLATFORM_64BIT) && defined(__clang__) && !defined(LINUX)
ILINE int64 iszero(int64_t x)
{
	return -(x>>63^(x-1)>>63);
}
#endif

#if defined(LINUX) || defined(APPLE)
ILINE unsigned long int iszero(long int x) {
    return -(x>>63^(x-1)>>63);
}
#endif

ILINE float if_neg_else(float test, float val_neg, float val_nonneg)
{
	return (float)__fsel(test, val_nonneg, val_neg);
}
ILINE float if_pos_else(float test, float val_pos, float val_nonpos)
{
	return (float)__fsel(-test, val_nonpos, val_pos);
}


template<class F> 
ILINE int32 inrange(F x, F end1, F end2) {
	return isneg(fabs_tpl(end1+end2-x*(F)2) - fabs_tpl(end1-end2));
}

template<class F> 
ILINE F cond_select(int32 bFirst, F op1,F op2) {
	F arg[2] = { op1,op2 };
	return arg[bFirst^1];
}

template<class F> 
ILINE int32 idxmax3(const F *pdata) {
	int32 imax = isneg(pdata[0]-pdata[1]);
	imax |= isneg(pdata[imax]-pdata[2])<<1;
	return imax & (2|(imax>>1^1));
}
template<class F> 
ILINE int32 idxmax3(const Vec3_tpl<F> &vec) {
	int32 imax = isneg(vec.x-vec.y);
	imax |= isneg(vec[imax]-vec.z)<<1;
	return imax & (2|(imax>>1^1));
}
template<class F> 
ILINE int32 idxmin3(const F *pdata) {
	int32 imin = isneg(pdata[1]-pdata[0]);
	imin |= isneg(pdata[2]-pdata[imin])<<1;
	return imin & (2|(imin>>1^1));
}
template<class F> 
ILINE int32 idxmin3(const Vec3_tpl<F> &vec) {
	int32 imin = isneg(vec.y-vec.x);
	imin |= isneg(vec.z-vec[imin])<<1;
	return imin & (2|(imin>>1^1));
}

ILINE int32 getexp(f32 x) 
{ 
#if defined(PS3)	
	//do not break strict aliasing rules, treated diff.since union code slows potentially down 
	union f32_u
	{
		uint32 uintVal;
		f32 floatVal;
	};
	f32_u u;	u.floatVal = x;
	u.uintVal = (u.uintVal>>23&0x0FF)-127;
	return (int32)u.uintVal;
#else
	return (int32)(*(uint32*)&x>>23&0x0FF)-127; 
#endif
}
ILINE int32 getexp(f64 x) 
{ 
#if defined(PS3)	
	//do not break strict aliasing rules, treated diff.since union code slows potentially down 
	union f64_u
	{
		uint32 uintVal[2];
		f64 doubleVal;
	};
	f64_u u;	u.doubleVal = x;
	u.uintVal[1] = (u.uintVal[1]>>20&0x7FF)-1023;
	return (int32)u.uintVal[1];
#else
	return (int32)(*((uint32*)&x+1)>>20&0x7FF)-1023;
#endif
}

ILINE f32 &setexp(f32 &x,int32 iexp) 
{ 
#if defined(PS3)	
	//do not break strict aliasing rules, treated diff.since union code slows potentially down 
	union f32_u
	{
		uint32 uintVal;
		f32 floatVal;
	};
	f32_u u;	u.floatVal = x;
	u.uintVal &= ~(0x0FF<<23);
	u.uintVal |= (iexp+127)<<23;
	x = u.floatVal;
	return x;
#else
	(*(uint32*)&x &= ~(0x0FF<<23)) |= (iexp+127)<<23; 
	return x; 
#endif
}
ILINE f64 &setexp(f64 &x,int32 iexp) 
{ 
#if defined(PS3)
	//do not break strict aliasing rules, treated diff.since union code slows potentially down 
	union f64_u
	{
		uint32 uintVal[2];
		f64 doubleVal;
	};
	f64_u u;	u.doubleVal = x;
	u.uintVal[1] &= ~(0x7FF<<20);
	u.uintVal[1] |= (iexp+1023)<<20;
	x = u.doubleVal; 
	return x;
#else
	(*((uint32*)&x+1) &= ~(0x7FF<<20)) |= (iexp+1023)<<20; 
	return x; 
#endif
}

template<class dtype> class strided_pointer
{
public:
	strided_pointer()
		: data(0)
		, iStride(sizeof(dtype))
	{ 
	}

	strided_pointer(dtype *pdata,int32 stride=sizeof(dtype))
		: data(pdata)
		, iStride(stride)
	{ 
	}

	template<typename dtype1>
	strided_pointer(dtype1 *pdata)
	{ 
		set(pdata, sizeof(dtype1));
	}

	template<typename dtype1>
	strided_pointer(const strided_pointer<dtype1> &src)
	{
		set(src.data, src.iStride);
	}

	template<typename dtype1>
	ILINE strided_pointer& operator=(const strided_pointer<dtype1> &src)
	{ 
		set(src.data, src.iStride);
		return *this;
	}

	ILINE dtype& operator[](int32 idx) { return *(dtype*)((char*)data+idx*iStride); }
	ILINE const dtype& operator[](int32 idx) const { return *(const dtype*)((const char*)data+idx*iStride); }
	ILINE strided_pointer<dtype> operator+(int32 idx) const { return strided_pointer<dtype>((dtype*)((char*)data+idx*iStride),iStride); }
	ILINE strided_pointer<dtype> operator-(int32 idx) const { return strided_pointer<dtype>((dtype*)((char*)data-idx*iStride),iStride); }

	ILINE operator bool() const
	{
		return data != 0;
	}

private:
	template<typename dtype1>
	ILINE void set(dtype1 *pdata, int32 stride)
	{
#		if !defined(eLittleEndian)
#			error eLittleEndian is not defined, please include CryEndian.h.
#		endif
		COMPILE_TIME_ASSERT(metautils::is_const<dtype>::value || !metautils::is_const<dtype1>::value);
		// note: we allow xint32 -> xint16 converting
		COMPILE_TIME_ASSERT(
			(metautils::is_same<typename metautils::remove_const<dtype1>::type, typename metautils::remove_const<dtype>::type>::value ||			
			((metautils::is_same<typename metautils::remove_const<dtype1>::type, sint32>::value ||
			metautils::is_same<typename metautils::remove_const<dtype1>::type, uint32>::value ||
			metautils::is_same<typename metautils::remove_const<dtype1>::type, sint16>::value ||
			metautils::is_same<typename metautils::remove_const<dtype1>::type, uint16>::value) &&
			(metautils::is_same<typename metautils::remove_const<dtype>::type, sint16>::value ||
			metautils::is_same<typename metautils::remove_const<dtype>::type, uint16>::value))));
		data = (dtype*)pdata + (sizeof(dtype1)/sizeof(dtype)-1)*(int)eLittleEndian; 
		iStride = stride;
	}

private:
	// Prevents assignment of a structure member's address by mistake:
	//   stridedPtrObject = &myStruct.member;
	// Use explicit constructor instead:
	//   stridedPtrObject = strided_pointer<baseType>(&myStruct.member, sizeof(myStruct));
	//
	// Keep it private and non-implemented.
	strided_pointer& operator=(dtype *pdata);

	// Prevents using the address directly by mistake:
	//   memcpy(destination, stridedPtrObject, sizeof(baseType) * n);
	// Use address of the first element instead:
	//   memcpy(destination, &stridedPtrObject[0], stridedPtrObject.iStride * n);
	// 
	// Keep it private and non-implemented.
	operator void*() const;

	// Prevents direct dereferencing to avoid confusing it with "normal" pointers:
	//   val = *stridedPtrObject;
	// Use operator [] instead:
	//   val = stridedPtrObject[0];
	//
	// Keep it private and non-implemented.
	dtype& operator*() const;

public:
	dtype *data;
	int32 iStride;
};


typedef struct VALUE16 {
	union {
		struct { unsigned char a,b; } c;
		unsigned short ab;
	};
} VALUE16;

ILINE unsigned short SWAP16(unsigned short l) {
	VALUE16 l16;
	unsigned char a,b;
	l16.ab=l;
 	a=l16.c.a;  b=l16.c.b;
 	l16.c.a=b; 	l16.c.b=a;
	return l16.ab;
}

//--------------------------------------------

typedef struct VALUE32 {
	union {
		struct { unsigned char a,b,c,d; } c;
		f32 FLOAT;
		unsigned long abcd;
		const void* ptr;
	};
} VALUE32;

ILINE uint32 SWAP32(uint32 l) {
    VALUE32 l32;
	unsigned char a,b,c,d;
	l32.abcd=l;
 	a=l32.c.a;  b=l32.c.b;  c=l32.c.c;  d=l32.c.d;
 	l32.c.a=d;	l32.c.b=c; 	l32.c.c=b; 	l32.c.d=a;
	return l32.abcd;
}

ILINE const void* SWAP32(const void* l) {
	VALUE32 l32;
	unsigned char a,b,c,d;
	l32.ptr=l;
 	a=l32.c.a;  b=l32.c.b;  c=l32.c.c;  d=l32.c.d;
 	l32.c.a=d;	l32.c.b=c; 	l32.c.c=b; 	l32.c.d=a;
	return l32.ptr;
}

ILINE f32 FSWAP32(f32 f) {
	VALUE32 l32;
	unsigned char a,b,c,d;
	l32.FLOAT=f;
	a=l32.c.a;  b=l32.c.b;  c=l32.c.c;  d=l32.c.d;
	l32.c.a=d;	l32.c.b=c; 	l32.c.c=b; 	l32.c.d=a;
	return l32.FLOAT;
}

//////////////////////////////////////////////////////////////////////////
//
// Random functions.
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Random number generator.
#include "Random.h"
// Random function to be used instead of rand()
extern unsigned int cry_rand();
// Generates random integer in full 32-bit range.
extern unsigned int cry_rand32();
// Generates random floating number in the closed interval [0,1].
extern float cry_frand();
//////////////////////////////////////////////////////////////////////////

// Random float between 0 and 1 inclusive.
ILINE float Random()
{
	return cry_frand();
}

// Random floats.
ILINE float Random(float fRange)
{
	return cry_frand() * fRange;
}
ILINE float Random(float fStart, float fEnd)
{
	return cry_frand() * (fEnd-fStart) + fStart;
}

// Random int from 0..nRange-1.
ILINE uint32 Random(uint32 nRange)
{
	return uint32((uint64(cry_rand32()) * nRange) >> 32);
}

// Proper overload resolution for ints.
ILINE uint32 Random(int32 nRange)
{
	assert(nRange >= 0);
	return Random(uint32(nRange));
}

ILINE float BiRandom(float fRange)
{
	return Random(-fRange, fRange);
}

#ifdef XENON_INTRINSICS
#include "console/Cry_Xenon_Math.h"
#endif

//////////////////////////////////////////////////////////////////////////
enum type_zero { ZERO };
enum type_min { VMIN };
enum type_max { VMAX };
enum type_identity { IDENTITY };

#include "Cry_Vector2.h"
#include "Cry_Vector3.h"
#include "Cry_Vector4.h"
#include "Cry_MatrixDiag.h"
#include "Cry_Matrix33.h"
#include "Cry_Matrix34.h"
#include "Cry_Matrix44.h"
#include "Cry_Quat.h"


#if (defined(PS3) || defined(XENON)) && defined(XENON_INTRINSICS)
#include <console/Cry_PS3_Vector4A.h>
#include <console/Cry_PS3_Matrix34A.h>
#include <console/Cry_PS3_Matrix44A.h>
#endif

#include "Cry_HWVector3.h"
#include "Cry_HWMatrix.h"

#if (defined(WIN32) || defined(DURANGO))
#include "Cry_XOptimise.h"
#endif

ILINE f32 sqr(Vec3 op) { return op|op; }
#ifndef REAL_IS_FLOAT
  ILINE real sqr(Vec3r op) { return op|op; }
#endif


// Approximation of exp(-x)
ILINE float approxExp(float x)
{
	return __fres(1.f+x);
}
	
// Approximation of 1.f - exp(-x)
ILINE float approxOneExp(float x)
{
	return x*__fres(1.f+x);
}

//////////////////////////////////////////////////////////////////////////

/// This function relaxes a value (val) towards a desired value (to) whilst maintaining continuity
/// of val and its rate of change (valRate). timeDelta is the time between this call and the previous one.
/// The caller would normally keep val and valRate as working variables, and smoothTime is normally 
/// a fixed parameter. The to/timeDelta values can change.
///
/// Implementation details:
///
/// This is a critically damped spring system. A linear spring is attached between "val" and "to" that
/// drags "val" to "to". At the same time a damper between the two removes oscillations; it's tweaked
/// so it doesn't dampen more than necessary. In combination this gives smooth ease-in and ease-out behavior.
///
/// smoothTime can be interpreted in a couple of ways:
/// - it's the "expected time to reach the target when at maximum velocity" (the target will, however, not be reached
///   in that time because the speed will decrease the closer it gets to the target)
/// - it's the 'lag time', how many seconds "val" lags behind "to". If your
///   target has a certain speed, the lag distance is simply the smoothTime times that speed.
/// - it's 2/omega, where omega is the spring's natural frequency (or less formally a measure of the spring stiffness)
///
/// The implementation is stable for varying timeDelta, but for performance reasons it uses a polynomial approximation
/// to the exponential function. The approximation works well (within 0.1% of accuracy) when smoothTime > 2*deltaTime,
/// which is usually the case. (but it might be troublesome when you want a stiff spring or have frame hikes!)
/// The implementation handles cases where smoothTime==0 separately and reliably. In that case the target will be
/// reached immediately, and valRate is updated appropriately.
///
/// Based on "Critically Damped Ease-In/Ease-Out Smoothing", Thomas Lowe, Game Programming Gems IV
///
template <typename T> ILINE void SmoothCD(
	T &val,                 ///< in/out: value to be smoothed
	T &valRate,             ///< in/out: rate of change of the value
	const float timeDelta,  ///< in: time interval
	const T &to,            ///< in: the target value
	const float smoothTime) ///< in: timescale for smoothing
{
	if (smoothTime > 0.0f)
	{
		const float omega = 2.0f / smoothTime;
		const float x = omega * timeDelta;
		const float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		const T change = (val - to);
		const T temp = (T)((valRate + change * omega) * timeDelta);
		valRate = (T)((valRate - temp * omega) * exp);
		val = (T)(to + (change + temp) * exp);
	}
	else if (timeDelta > 0.0f)
	{
		valRate = (T)((to - val) / timeDelta);
		val = to;
	}
	else
	{
		val = to;
		valRate -= valRate; // zero it...
	}
}


template <typename T> ILINE void SmoothCDWithMaxRate(
	T &val,                 ///< in/out: value to be smoothed
	T &valRate,             ///< in/out: rate of change of the value
	const float timeDelta,  ///< in: time interval
	const T &to,            ///< in: the target value
	const float smoothTime, ///< in: timescale for smoothing
	const T& maxValRate)    ///< in: maximum allowed rate of change
{
	if (smoothTime > 0.0f)
	{
		const float omega = 2.0f / smoothTime;
		const float x = omega * timeDelta;
		const float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		const T unclampedChange = val - to;
		const T maxChange = maxValRate * smoothTime;
		const T clampedChange = clamp_tpl<T>(unclampedChange, -maxChange, maxChange);
		const T clampedTo = val - clampedChange;
		const T temp = (T)((valRate + clampedChange * omega) * timeDelta);
		valRate = (T)((valRate - temp * omega) * exp);
		val = (T)(clampedTo + (clampedChange + temp) * exp);
	}
	else if (timeDelta > 0.0f)
	{
		const T unclampedRate = (T)((to - val) / timeDelta);
		valRate = clamp_tpl<T>(unclampedRate, -maxValRate, maxValRate);
		val += valRate * timeDelta;
	}
	else
	{
		val = to;
		valRate -= valRate; // zero it...
	}
}


// Smoothes linear blending into cubic (b-spline) with 0-derivatives
// near 0 and 1
inline f32 SmoothBlendValue (const f32 fBlend)
{
	const f32 fBlendAdj = fBlend-0.5f;
	return (f32)__fsel(-fBlend,0.f,__fsel(fBlend-1.f,1.f,0.5f - 2.f * (fBlendAdj*fBlendAdj*fBlendAdj) + 1.5f * fBlendAdj));
}

// function for safe comparsion of floating point  values
ILINE bool fcmp( f32 fA, f32 fB, f32 fEpsilon = FLT_EPSILON )
{
	return fabs(fA - fB) <= fEpsilon;
}
#endif //math
