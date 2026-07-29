#ifndef CRY_HALF_INL
#define CRY_HALF_INL

#pragma once


typedef uint16 CryHalf;
class ICrySizer;

typedef union floatint_union {float f; uint32 i;} floatint_union;

#	if defined(WIN32) || defined(WIN64) || defined(LINUX) || defined(MAC) || defined(DURANGO) || defined(ORBIS)

__forceinline CryHalf CryConvertFloatToHalf(const float Value)
{
#if defined(LINUX) || defined(MAC)
	asm volatile("" ::: "memory");
#endif
	unsigned int Result;

	unsigned int IValue = ((unsigned int *)(&Value))[0];
	unsigned int Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			unsigned int Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}
	return (CryHalf)(Result|Sign);
}

__forceinline float CryConvertHalfToFloat(const CryHalf Value)
{
#if defined(LINUX) || defined(MAC)
	asm volatile("" ::: "memory");
#endif
	unsigned int Mantissa;
	unsigned int Exponent;
	unsigned int Result;

	Mantissa = (unsigned int)(Value & 0x03FF);

	if ((Value & 0x7C00) != 0)  // The value is normalized
	{
		Exponent = (unsigned int)((Value >> 10) & 0x1F);
	}
	else if (Mantissa != 0)     // The value is denormalized
	{
		// Normalize the value in the resulting float
		Exponent = 1;

		do
		{
			Exponent--;
			Mantissa <<= 1;
		} while ((Mantissa & 0x0400) == 0);

		Mantissa &= 0x03FF;
	}
	else                        // The value is zero
	{
		Exponent = (unsigned int)-112;
	}

	Result = ((Value & 0x8000) << 16) | // Sign
		((Exponent + 112) << 23) | // Exponent
		(Mantissa << 13);          // Mantissa

	return *(float*)&Result;
}

#	elif defined(XENON)

__forceinline CryHalf CryConvertFloatToHalf(const float Value)
{
	XMVECTOR ValueV;
	CryHalf Result;

	XMASSERT(((UINT_PTR)&Value & 3) == 0);

	ValueV = __lvlx(&Value, 0);
	ValueV = __vpkd3d(ValueV, ValueV, VPACK_FLOAT16_4, VPACK_64LO, 2);
	ValueV = __vsplth(ValueV, 0);

	__stvehx(ValueV, &Result, 0);

	return Result;
}

__forceinline float CryConvertHalfToFloat(const CryHalf Value)
{
	XMVECTOR ValueV;
	float Result;

	XMASSERT(((UINT_PTR)&Value & 1) == 0);

	ValueV = __lvlx(&Value, 0);
	ValueV = __vsplth(ValueV, 0);
	ValueV = __vupkd3d(ValueV, VPACK_FLOAT16_4);

	__stvewx(ValueV, &Result, 0);

	return Result;
}

#	elif defined(PS3)

#include <CryHalf_branchfree.inl>

#define HALF_FLOAT_USE_BRANCHFREE_CONVERSION

ILINE CryHalf CryConvertFloatToHalf(const float val)
{
#if defined(HALF_FLOAT_USE_BRANCHFREE_CONVERSION)
	floatint_union u;
	u.f = val;
  return half_from_float(u.i);
#else 
	uint8 *tmp = (uint8*)&val;
	uint32 bits = ((uint32)tmp[0] << 24) | ((uint32)tmp[1] << 16) | ((uint32)tmp[2] << 8) |(uint32)tmp[3];
	if(bits == 0)
		return 0;
	int e = ((bits & 0x7f800000) >> 23) - 127 + 15;
	if(e < 0)
		return 0;
	else 
		if (e > 31)
			e = 31;
	uint32 s = bits & 0x80000000;
	uint32 m = bits & 0x007fffff;
	return ((s >> 16) & 0x8000) | ((e << 10) & 0x7c00) | ((m >> 13) & 0x03ff);
#endif 
}

ILINE float CryConvertHalfToFloat(const CryHalf val)
{
#if defined(HALF_FLOAT_USE_BRANCHFREE_CONVERSION)
  const uint32_t converted = half_to_float(val);
	floatint_union u;	u.i = converted;
  return u.f;
#else 
	if(val == 0)
		return 0.0f;
	uint32 s = val & 0x8000;
	int e =((val & 0x7c00) >> 10) - 15 + 127;
	uint32 m =  val & 0x03ff;
	uint32 floatVal = (s << 16) | ((e << 23) & 0x7f800000) | (m << 13);
	float result;
	uint8 *tmp = (uint8*)&result;
	tmp[0] = (floatVal >> 24) & 0xff;
	tmp[1] = (floatVal >> 16) & 0xff;
	tmp[2] = (floatVal >> 8) & 0xff;
	tmp[3] = floatVal & 0xff;
	return result;
#endif
}

#if defined(HALF_FLOAT_USE_BRANCHFREE_CONVERSION)
# undef HALF_FLOAT_USE_BRANCHFREE_CONVERSION
#endif

#	endif

struct CryHalf2
{
	CryHalf x;
	CryHalf y;

	CryHalf2()
	{
	}
	CryHalf2(CryHalf _x, CryHalf _y)
	: x(_x)
	, y(_y)
	{
	}
	CryHalf2(const CryHalf* const __restrict pArray)
	{
		x = pArray[0];
		y = pArray[1];
	}
	CryHalf2(float _x, float _y)
	{
		x = CryConvertFloatToHalf(_x);
		y = CryConvertFloatToHalf(_y);
	}
	CryHalf2(const float* const __restrict pArray)
	{
		x = CryConvertFloatToHalf(pArray[0]);
		y = CryConvertFloatToHalf(pArray[1]);
	}
	CryHalf2& operator= (const CryHalf2& Half2)
	{
		x = Half2.x;
		y = Half2.y;
		return *this;
	}

	bool operator !=(const CryHalf2& rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}

	void GetMemoryUsage(ICrySizer* pSizer) const {}

	AUTO_STRUCT_INFO
};

struct CryHalf4
{
	CryHalf x;
	CryHalf y;
	CryHalf z;
	CryHalf w;

	CryHalf4()
	{
	}
	CryHalf4(CryHalf _x, CryHalf _y, CryHalf _z, CryHalf _w)
	: x(_x)
	, y(_y)
	, z(_z)
	, w(_w)
	{
	}
	CryHalf4(const CryHalf* const __restrict pArray)
	{
		x = pArray[0];
		y = pArray[1];
		z = pArray[2];
		w = pArray[3];
	}
	CryHalf4(float _x, float _y, float _z, float _w)
	{
		x = CryConvertFloatToHalf(_x);
		y = CryConvertFloatToHalf(_y);
		z = CryConvertFloatToHalf(_z);
		w = CryConvertFloatToHalf(_w);
	}
	CryHalf4(const float* const __restrict pArray)
	{
		x = CryConvertFloatToHalf(pArray[0]);
		y = CryConvertFloatToHalf(pArray[1]);
		z = CryConvertFloatToHalf(pArray[2]);
		w = CryConvertFloatToHalf(pArray[3]);
	}
	CryHalf4& operator= (const CryHalf4& Half4)
	{
		x = Half4.x;
		y = Half4.y;
		z = Half4.z;
		w = Half4.w;
		return *this;
	}
	bool operator !=(const CryHalf4& rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
	}

	void GetMemoryUsage(ICrySizer* pSizer) const {}

	AUTO_STRUCT_INFO
};

#endif // #ifndef CRY_HALF_INL
