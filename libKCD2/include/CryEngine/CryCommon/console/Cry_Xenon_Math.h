//////////////////////////////////////////////////////////////////////
//
//	Crytek Common Source code
//
//	File:Cry_Math.h
//	Description: Common math class for Xenon
//
//	History:
//	-Dec 12,2007: Created by Alexey
//
//////////////////////////////////////////////////////////////////////


#ifndef CRY_XENON_MATH
#define CRY_XENON_MATH
#pragma once 


#ifdef XENON 
ILINE int vec_any_lt(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgefpR(lhs, rhs, &compareResult);
	return !(compareResult & 0x80); // Bit 7 for all
}

ILINE int vec_all_lt(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgefpR(lhs, rhs, &compareResult);
	return (0 != (compareResult & 0x20)); // Bit 5 for none
}

ILINE int vec_any_gt(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgtfpR(lhs, rhs, &compareResult);
	return !(compareResult & 0x20); // Bit 5 for none
}

ILINE int vec_all_gt(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgtfpR(lhs, rhs, &compareResult);
	return (0 != (compareResult & 0x80)); // Bit 7 for all
}

ILINE int vec_any_ge(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgefpR(lhs, rhs, &compareResult);
	return !(compareResult & 0x20); // Bit 5 for none
}

ILINE int vec_all_ge(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgefpR(lhs, rhs, &compareResult);
	return (0 != (compareResult & 0x80)); // Bit 7 for all
}

ILINE int vec_all_le(XMVECTOR lhs, XMVECTOR rhs)
{
	unsigned int compareResult;
	__vcmpgefpR(lhs, rhs, &compareResult);
	return (0 != (compareResult & 0x20)); // Bit 5 for none
}

ILINE int vec_any_eq_uint(XMVECTOR lhs, XMVECTOR rhs)
{
	// Note: for vectors of 4 x uint
	unsigned int compareResult;
	__vcmpequwR(lhs, rhs, &compareResult);
	return !(compareResult & 0x20); // Bit 5 for none
}

#elif defined(PS3)
#define vec_any_eq_uint(lhs, rhs)		vec_any_eq((vec_uint4)(lhs), (vec_uint4)(rhs))
#endif

//=============================================================================

struct XMVec4A
{
	static ILINE XMVECTOR LoadVec4 (const void *pV)
	{
		return XMLoadVector4A(pV);
	}
	static ILINE XMVECTOR LoadVec3 (const void *pV)
	{
		return XMLoadVector3A(pV);
	}
	static ILINE void StoreVec4 (void *pDest, const XMVECTOR& xm)
	{
		XMStoreVector4A(pDest, xm);
	}
	static ILINE void StoreVec3 (void *pDest, const XMVECTOR& xm)
	{
		XMStoreVector3A(pDest, xm);
	}

	static void *New(size_t s)
	{
		unsigned char *p = ::new unsigned char[s + 16];
		if (p)
		{
			unsigned char offset = (unsigned char)(16 - ((UINT_PTR)p & 15));
			p += offset;
			p[-1] = offset;
		}
		return p;
	}
	static void Del(void *p)
	{
		if(p)
		{
			unsigned char* pb = static_cast<unsigned char*>(p);
			pb -= pb[-1];
			::delete [] pb;
		}
	}
};

struct XMVec4
{

	static ILINE XMVECTOR LoadVec4 (const void *pV)
	{
		return XMLoadVector4(pV);
	}
	static ILINE XMVECTOR LoadVec3 (const void *pV)
	{
		return XMLoadVector3(pV);
	}
  static ILINE XMVECTOR LoadVec2 (const void *pV)
  {
    return XMLoadVector2(pV);
  }
	static ILINE void StoreVec4 (void *pDest, const XMVECTOR& xm)
	{
		XMStoreVector4(pDest, xm);
	}
	static ILINE void StoreVec3 (void *pDest, const XMVECTOR& xm)
	{
		XMStoreVector3(pDest, xm);
	}
	static void *New(size_t s)
	{
		unsigned char *p = ::new unsigned char[s];
		return p;
	}
	static void Del(void *p)
	{
		if(p)
			::delete [] (unsigned char*)p;
	}
};

#if defined(XENON) && defined(XENON_INTRINSICS)
// On PS3 these casts are necessary, on Xenon they cause errors
// See Cry_PS3_Math.h for the corresponding implementations
#define XMCAST_VEC_FLOAT(a) (a)
#define XMCAST_VEC_UINT(a) (a)
#define XMCAST_VEC_UCHAR(a) (a)

#define __loadunalignedvector_volatile(ptr)		(__vor(__lvlx_volatile(ptr, 0), __lvrx_volatile(ptr,16)))

#endif

#endif
