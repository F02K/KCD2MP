//////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2007.
// -------------------------------------------------------------------------
//  File name:   CryCustomTypes.h
//  Created:     2009-10-23 by Scott.
//  Description: Derived CTypeInfos for structs, enums, etc.
//               Compressed numerical types, and associated TypeInfos.
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#ifndef _CRY_CUSTOM_TYPES_H
#define _CRY_CUSTOM_TYPES_H

#include "CryTypeInfo.h"
#include "CryFixedString.h"

#pragma warning(push)
#pragma warning(disable: 4800)

#if defined(__SPU__) // use a version without functon static variables for SPUs(these aren't supported there)
#define STATIC_CONST(T, name, val) \
	static inline T name()  { return val; }
#else
#define STATIC_CONST(T, name, val) \
	static inline T name()  { static T t = val; return t; }
#endif
	
//---------------------------------------------------------------------------
// String helper function.

template<class T>
inline bool HasString(const T& val, FToString flags, const void* def_data = 0)
{
	if (flags._SkipDefault)
	{
		if (val == (def_data ? *(const T*)def_data : T()))
			return false;
	}
	return true;
}

float NumToFromString(float val, int digits, bool floating, char buffer[], int buf_size);

template<class T>
string NumToString(T val, int min_digits, int max_digits, bool floating)
{
	char buffer[64];
	float f(val);
	for (int digits = min_digits; digits < max_digits; digits++)
	{
		if (T(NumToFromString(f, digits, floating, buffer, 64)) == val)
			break;
	}
	return buffer;
}

//---------------------------------------------------------------------------
// TypeInfo for structs

struct CStructInfo: CTypeInfo
{
	CStructInfo( cstr name, size_t size, size_t align, Array<CVarInfo> vars = Array<CVarInfo>(), Array<CTypeInfo const*> templates = Array<CTypeInfo const*>() );
	virtual bool IsType( CTypeInfo const& Info ) const;
	virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const;
	virtual bool FromString(void* data, cstr str, FFromString flags = 0) const;
	virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const;
	virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const;
	virtual bool ValueEqual(const void* data, const void* def_data) const;
	virtual void SwapEndian(void* pData, size_t nCount, bool bWriting) const;
	virtual void GetMemoryUsage(ICrySizer* pSizer, void const* data) const;

	virtual const CVarInfo* NextSubVar(const CVarInfo* pPrev, bool bRecurseBase = false) const;
	virtual const CVarInfo* FindSubVar(cstr name) const;

	virtual CTypeInfo const* const* NextTemplateType(CTypeInfo const* const* pPrev) const
	{
		pPrev = pPrev ? pPrev+1 : TemplateTypes.begin();
		return pPrev < TemplateTypes.end() ? pPrev : 0;
	}

protected:
	Array<CVarInfo>						Vars;
	CryStackStringT<char, 16>	EndianDesc;				// Encodes instructions for endian swapping.
	bool											HasBitfields;
	Array<CTypeInfo const*>		TemplateTypes;

	void MakeEndianDesc();
	size_t AddEndianDesc(cstr desc, size_t dim, size_t elem_size);
	bool IsCompatibleType( CTypeInfo const& Info ) const;
};

//---------------------------------------------------------------------------
// TypeInfo for enums

struct CEnumInfo: CTypeInfo
{
	struct CEnumElem
	{
		int			Value;
		cstr		Name;
	};

	CEnumInfo( cstr name, size_t size, size_t align, size_t num_elems = 0, CEnumElem* elems = 0 );

	virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const;
	virtual bool FromString(void* data, cstr str, FFromString flags = 0) const;

	virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const;
	virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const;

	virtual cstr EnumElem(uint nIndex) const
		{ return nIndex < nDisplayElems ? ShortName(nIndex) : 0; }

protected:

	Array<CEnumElem>	Elems;
	uint							nDisplayElems;
	bool							bRegular;
	int								MinValue, MaxValue;
	size_t						nPrefixLength;

	cstr FullName(int nIndex) const
		{ return Elems[nIndex].Name; }
	cstr ShortName(int nIndex) const
		{ return *Elems[nIndex].Name ? Elems[nIndex].Name+nPrefixLength : ""; }
};

//---------------------------------------------------------------------------
// Template TypeInfo for base types, using global To/FromString functions.

template<class T>
struct TTypeInfo: CTypeInfo
{
	TTypeInfo( cstr name )
		: CTypeInfo( name, sizeof(T), alignof(T) )
	{}

	virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const
	{
		if (&typeVal == this)
			return *(T*)value = *(const T*)data, true;
		return false;
	}
	virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const
	{
		if (&typeVal == this)
			return *(T*)data = *(const T*)value, true;
		return false;
	}

	virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const
	{
		if (!HasString(*(const T*)data, flags, def_data))
			return string();
		return ::ToString(*(const T*)data);
	}
	virtual bool FromString(void* data, cstr str, FFromString flags = 0) const
	{
		if (!*str)
		{
			if (!flags._SkipEmpty)
				*(T*)data = T();
			return true;
		}
		return ::FromString(*(T*)data, str);
	}
	virtual bool ValueEqual(const void* data, const void* def_data = 0) const
	{
		return *(const T*)data == (def_data ? *(const T*)def_data : T());
	}

	virtual void GetMemoryUsage(ICrySizer* pSizer, void const* data) const
	{}
};

//---------------------------------------------------------------------------
// Template TypeInfo for modified types (e.g. compressed, range-limited)

template<class T, class S>
struct TProxyTypeInfo: CTypeInfo
{
	TProxyTypeInfo( cstr name )
		: CTypeInfo( name, sizeof(S), alignof(S) )
	{}

	virtual bool IsType( CTypeInfo const& Info ) const		
		{ return &Info == this || ValTypeInfo().IsType(Info); }

	virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const
	{
		if (&typeVal == this)
		{
			*(S*)value = *(const S*)data;
			return true;
		}
		T val = T(*(const S*)data);
		return ValTypeInfo().ToValue(&val, value, typeVal);
	}
	virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const
	{
		if (&typeVal == this)
		{
			*(S*)data = *(const S*)value;
			return true;
		}
		T val;
		if (ValTypeInfo().FromValue(&val, value, typeVal))
		{
			*(S*)data = S(val);
			return true;
		}
		return false;
	}

	virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const
	{
		T val = T(*(const S*)data);
		T def_val = def_data ? T(*(const S*)def_data) : T();
		return ValTypeInfo().ToString(&val, flags, &def_val);
	}
	virtual bool FromString(void* data, cstr str, FFromString flags = 0) const
	{
		T val;
		if (!*str)
		{
			if (!flags._SkipEmpty)
				*(S*)data = S();
			return true;
		}
		if (!TypeInfo(&val).FromString(&val, str))
			return false;
		*(S*)data = S(val);
		return true;
	}

	// Forward additional TypeInfo functions.
	virtual bool GetLimit(ENumericLimit eLimit, float& fVal) const
		{ return ValTypeInfo().GetLimit(eLimit, fVal); }
	virtual cstr EnumElem(uint nIndex) const
		{ return ValTypeInfo().EnumElem(nIndex); }

protected:

	static const CTypeInfo& ValTypeInfo()
		{ return TypeInfo((T*)0); }
};

//---------------------------------------------------------------------------
// Customisation for string.

template<> 
inline string TTypeInfo<string>::ToString(const void* data, FToString flags, const void* def_data) const
{
	const string& val = *(const string*)data;
	if (def_data && flags._SkipDefault)
	{
		if (val == *(const string*)def_data)
			return string();
	}
	return val;
}

template<> 
inline bool TTypeInfo<string>::FromString(void* data, cstr str, FFromString flags) const
{
	if (!*str && flags._SkipEmpty)
		return true;
	*(string*)data = str;
	return true;
}

template<> 
void TTypeInfo<string>::GetMemoryUsage(ICrySizer* pSizer, void const* data) const;

//---------------------------------------------------------------------------
//
// TypeInfo for small integer types.


#pragma warning(disable: 4554) // BOGUS WARNING: check operator precedence for possible error; use parentheses to clarify precedence

template<class T> struct TIntTraits
{
	static const bool bSIGNED
		= T(-1) < T(0);

	static const T nMIN_FACTOR
		= bSIGNED ? T(-1) : T(0);

	static const T nMIN
		= bSIGNED ? T (T(1) << T(sizeof(T)*8-1)) : T(0);

	static const T nMAX
		= ~nMIN;
};

template<class D, class S>
inline bool ConvertInt( D& dest, S src, D nMin = TIntTraits<D>::nMIN, D nMax = TIntTraits<D>::nMAX)
{
	if (src < S(nMin))
	{
		dest = nMin;
		return false;
	}
	if (src > S(nMax))
	{
		dest = nMax;
		return false;
	}
	dest = D(src);
	assert(S(dest) == src);
	return true;
}

template<class D>
inline bool ConvertInt( D& dest, const void* src, const CTypeInfo& typeSrc, D nMin = TIntTraits<D>::nMIN, D nMax = TIntTraits<D>::nMAX)
{
	if (typeSrc.IsType<int>())
	{
		switch (typeSrc.Size)
		{
			case 1: return ConvertInt(dest, *(const int8*)src, nMin, nMax);
			case 2: return ConvertInt(dest, *(const int16*)src, nMin, nMax);
			case 4: return ConvertInt(dest, *(const int32*)src, nMin, nMax);
			case 8: return ConvertInt(dest, *(const int64*)src, nMin, nMax);
		}
	}
	else if (typeSrc.IsType<uint>())
	{
		switch (typeSrc.Size)
		{
			case 1: return ConvertInt(dest, *(const uint8*)src, nMin, nMax);
			case 2: return ConvertInt(dest, *(const uint16*)src, nMin, nMax);
			case 4: return ConvertInt(dest, *(const uint32*)src, nMin, nMax);
			case 8: return ConvertInt(dest, *(const uint64*)src, nMin, nMax);
		}
	}
	return false;
}

template<class T>
struct TIntTypeInfo: TTypeInfo<T>
{
	TIntTypeInfo( cstr name )
		: TTypeInfo<T>(name)
	{}

	virtual bool IsType( CTypeInfo const& Info ) const		
		{ return &Info == this || &Info == ( TIntTraits<T>::bSIGNED ? &TypeInfo((int*)0) : &TypeInfo((uint*)0) ); }

	virtual bool GetLimit(ENumericLimit eLimit, float& fVal) const
	{
		if (eLimit == eLimit_Min)
			return fVal = float(TIntTraits<T>::nMIN), true;
		if (eLimit == eLimit_Max)
			return fVal = float(TIntTraits<T>::nMAX), true;
		if (eLimit == eLimit_Step)
			return fVal = 1.f, true;
		return false;
	}

	// Override to allow int conversion
	virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const
		{ return ConvertInt(*(T*)data, value, typeVal); }
	virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const
		{ return typeVal.FromValue(value, *(T*)data); }
};

//---------------------------------------------------------------------------
// Store any type, such as an enum, in a small int.

template<class T, int nMIN = INT_MIN, int nMAX = INT_MAX, int nDEFAULT = 0>
struct TRangedType
{
	TRangedType(T init = T(nDEFAULT))
		: m_Val(init)
	{ 
		CheckRange(m_Val);
	}

	operator T() const
		{ return m_Val; }

	CUSTOM_STRUCT_INFO(CCustomInfo)

protected:
	T		m_Val;

	static bool HasMin()
		{ return nMIN > INT_MIN; }
	static bool HasMax()
		{ return nMAX < INT_MAX; }

	static bool CheckRange(T& val)
	{
		if (HasMin() && val < T(nMIN))
		{
			val = T(nMIN);
			return false;
		}
		else if (HasMax() && val > T(nMAX))
		{
			val = T(nMAX);
			return false;
		}
		return true;
	}

	// Adaptor TypeInfo for converting between sizes.
	struct CCustomInfo: TTypeInfo<T>
	{
		CCustomInfo()
			: TTypeInfo<T>( ::TypeInfo((T*)0).Name )
		{}

		virtual bool IsType( const CTypeInfo& Info ) const
			{ return &Info == this || ::TypeInfo((T*)0).IsType(Info); }

		virtual bool GetLimit(ENumericLimit eLimit, float& fVal) const
		{
			if (eLimit == eLimit_Min && HasMin())
				return fVal = T(nMIN), true;
			if (eLimit == eLimit_Max && HasMax())
				return fVal = T(nMAX), true;
			return ::TypeInfo((T*)0).GetLimit(eLimit, fVal);
		}

		// Override to check range
		virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const
		{
			return ::TypeInfo((T*)0).ToValue(data, value, typeVal);
		}
		virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const
		{
			return ::TypeInfo((T*)0).FromValue(data, value, typeVal)
				&& CheckRange(*(T*)data);
		}
		virtual bool FromString(void* data, cstr str, FFromString flags = 0) const
		{
			return ::TypeInfo((T*)0).FromString(data, str, flags)
				&& CheckRange(*(T*)data);
		}
	};
};

//---------------------------------------------------------------------------
// Store any type, such as an enum, in a small int.

template<class T, class S = uint8>
struct TSmall
{
	typedef T				TValue;

	inline TSmall(T val = T(0))
	{
		set(val);
	}
	void set(T val = T(0))
	{
		m_Val = S(val);
		assert(T(m_Val) == val);
	}

	inline operator T() const
		{ return T(m_Val); }
	inline T operator +() const
		{ return T(m_Val); }

	const CTypeInfo& TypeInfo() const
	{
		static TProxyTypeInfo<T,S> Info( ::TypeInfo((T*)0).Name );
		return Info;
	}

protected:
	S		m_Val;
};

//---------------------------------------------------------------------------
// Quantise a float linearly in an int.
template<class S, int nLIMIT, S nQUANT = TIntTraits<S>::nMAX, bool bTRUNC = false>
struct TFixed
{
	typedef float TValue;

	inline TFixed()
		: m_Store(0)
	{}

	inline TFixed(float fIn)
	{
		float fStore = ToStore(fIn);
		int32 iStore = bTRUNC ? int32(fStore) : int_round(fStore);
		ConvertInt(m_Store, iStore, S(TIntTraits<S>::nMIN_FACTOR * nQUANT), nQUANT);
	}

	// Conversion.
	inline operator float() const
		{ return FromStore(m_Store); }
	inline float operator +() const
		{ return FromStore(m_Store); }
	inline bool operator !() const
		{ return !m_Store; }

	inline bool operator ==(const TFixed& x) const
		{ return m_Store == x.m_Store; }
	inline bool operator ==(float x) const
		{ return m_Store == TFixed(x); }
	inline S GetStore() const
	  { return m_Store; }

	inline float InflateValue(float f) const
		{ return FromStore(f);	}
	inline S GetCompressedValue() const
		{ return m_Store;	}

	CUSTOM_STRUCT_INFO(CCustomInfo)

protected:
	S		m_Store;

	typedef TFixed<S, nLIMIT, nQUANT, bTRUNC> TThis;

	static const int nMAX = nLIMIT;
	static const int nMIN = TIntTraits<S>::nMIN_FACTOR * nLIMIT;

	static inline float ToStore(float f)
		{ return f * float(nQUANT) / float(nLIMIT); }
	static inline float FromStore(float f)
		{ return f * float(nLIMIT) / float(nQUANT); }

	// TypeInfo implementation.
	struct CCustomInfo: TProxyTypeInfo<float, TThis>
	{
		CCustomInfo()
			: TProxyTypeInfo<float, TThis>("TFixed<>")
		{}

		virtual bool GetLimit(ENumericLimit eLimit, float& fVal) const
		{
			if (eLimit == eLimit_Min)
				return fVal = float(nMIN), true;
			if (eLimit == eLimit_Max)
				return fVal = float(nMAX), true;
			if (eLimit == eLimit_Step)
				return fVal = FromStore(1.f), true;
			return false;
		}

		// Override ToString: Limit to significant digits.
		virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const
		{
			if (!HasString(*(const S*)data, flags, def_data))
				return string();
			static int digits = int_ceil( log10f(float(nQUANT)) );
			return NumToString(*(const TFixed*)data, 1, digits+3, true);
		}
	};
};


// Define the canonical float-to-byte quantisation.
typedef TFixed<uint8,1>	UnitFloat8;

//---------------------------------------------------------------------------
// A floating point number, with templated storage size (and sign), and number of exponent bits
template<class S, int nEXP_BITS>
struct TFloat
{
	typedef float TValue;

	ILINE TFloat()
		: m_Store(0)
	{}

	ILINE TFloat(float fIn)
		: m_Store(FromFloat(fIn))
	{}

	ILINE operator float() const
		{ return ToFloat(m_Store); }
	ILINE float operator +() const
		{ return ToFloat(m_Store); }

	ILINE bool operator !() const
		{ return !m_Store; }

	ILINE bool operator ==(TFloat x) const
		{ return m_Store == x.m_Store; }
	ILINE bool operator ==(float x) const
		{ return float(*this) == x; }

	inline TFloat& operator *=(float x)
		{ return *this = *this * x; }

	ILINE uint32 partial_float_conversion() const
		{ return (0 == m_Store)?0:ToFloatCore( m_Store ); }

	STATIC_CONST(float, fMAX, ToFloat(TIntTraits<S>::nMAX));
	STATIC_CONST(float, fPOS_MIN, ToFloat(1 << nMANT_BITS));
	STATIC_CONST(float, fMIN, -fMAX() * (float)TIntTraits<S>::bSIGNED);

	CUSTOM_STRUCT_INFO(CCustomInfo)

protected:
	S		m_Store;

	typedef TFloat<S,nEXP_BITS> TThis;

	static const S nBITS = sizeof(S)*8;
	static const S nSIGN = TIntTraits<S>::bSIGNED;
	static const S nMANT_BITS = nBITS - nEXP_BITS - nSIGN;
	static const S nSIGN_MASK = S(nSIGN << (nBITS-1));
	static const S nMANT_MASK = (S(1) << nMANT_BITS) - 1;
	static const S nEXP_MASK = ~S(nMANT_MASK | nSIGN_MASK);
	static const int nEXP_MAX = 1 << (nEXP_BITS-1),
	                 nEXP_MIN = 1 - nEXP_MAX;

	STATIC_CONST(float, fROUNDER, 1.f + fPOS_MIN()*0.5f);

	static inline S FromFloat(float fIn)
	{
		COMPILE_TIME_ASSERT(sizeof(S) <= 4);
		COMPILE_TIME_ASSERT(nEXP_BITS > 0 && nEXP_BITS <= 8 && nEXP_BITS < sizeof(S)*8-4);

		// Clamp to allowed range.
		float fClamped = clamp_tpl(fIn * fROUNDER(), fMIN(), fMAX());

		// Bit shift to convert from IEEE float32.
		uint32 uBits = *(const uint32*)&fClamped;
		
		// Convert exp.
    int32 iExp = (uBits >> 23) & 0xFF;
		iExp -= 127 + nEXP_MIN;
		IF (iExp < 0,0)
			// Underflow.
			return 0;

		// Reduce mantissa.
		uint32 uMant = uBits >> (23 - nMANT_BITS);

		S bits = (uMant & nMANT_MASK)
					 | (iExp << nMANT_BITS)
					 | ((uBits >> (32-nBITS)) & nSIGN_MASK);

		#ifdef _DEBUG
			fIn = clamp_tpl(fIn, fMIN(), fMAX());
			float fErr = fabs(ToFloat(bits) - fIn);
			float fMaxErr = fabs(fIn) / float(1<<nMANT_BITS);
			assert(fErr <= fMaxErr);
		#endif

		return bits;
	}

	static inline uint32 ToFloatCore(S bits)
	{
		// Extract FP components.
		uint32 uBits = bits & nMANT_MASK,
			uExp = (bits & ~nSIGN_MASK) >> nMANT_BITS,
			uSign = bits & nSIGN_MASK;

		// Shift to 32-bit.
		uBits <<= 23 - nMANT_BITS;
		uBits |= (uExp + 127 + nEXP_MIN) << 23;
		uBits |= uSign << (32-nBITS);

		return uBits;
	}

	static ILINE float ToFloat(S bits)
	{
		IF (bits == 0, 0)
			return 0.f;

		uint32 uBits = ToFloatCore( bits );
		return *(float*)&uBits;
	}

	// TypeInfo implementation.
	struct CCustomInfo: TProxyTypeInfo<float, TThis>
	{
		CCustomInfo()
			: TProxyTypeInfo<float, TThis>("TFloat<>") 
		{}

		virtual bool GetLimit(ENumericLimit eLimit, float& fVal) const
		{
			if (eLimit == eLimit_Min)
				return fVal = fMIN(), true;
			if (eLimit == eLimit_Max)
				return fVal = fMAX(), true;
			if (eLimit == eLimit_Step)
				return fVal = fPOS_MIN(), true;
			return false;
		}

		// Override ToString: Limit to significant digits.
		virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const
		{
			if (!HasString(*(const S*)data, flags, def_data))
				return string();
			static int digits = int_ceil( log10f(1<<nMANT_BITS) );
			return NumToString(*(const TFloat*)data, 1, digits+3, true);
		}
	};
};

// Canonical float16 types, with range ~= 64K.
typedef TFloat<int16,5>		SFloat16;
typedef TFloat<uint16,5>	UFloat16;

template<typename T>
ILINE T partial_float_cast( const SFloat16& s ) { return static_cast<T>( s.partial_float_conversion() ); }

template<typename T>
ILINE T partial_float_cast( const UFloat16& u ) { return static_cast<T>( u.partial_float_conversion() ); }

#ifdef _DEBUG

// Classes for unit tests.
template<class T2, class T>
void TestValues(T val)
{
	T2 val2 = val;
	T2 val2c;

	bool b = TypeInfo(&val2c).FromValue(&val2c, val);
	assert(b);
	assert(val2 == val2c);
	b = TypeInfo(&val2c).ToValue(&val2c, val);
	assert(b);
	assert(val2 == T2(val));
}

template<class T2, class T>
void TestTypes(T val)
{
	T2 val2 = val;
	string s = TypeInfo(&val2).ToString(&val2);
	bool b = TypeInfo(&val).FromString(&val, s);
	assert(b);
	assert(val2 == T2(val));

	TestValues<T2>(val);
}

template<class T>
void TestType(T val)
{
	TestTypes<T>(val);
}

#endif // _DEBUG

//---------------------------------------------------------------------------
// deep_ptr: acts as a value type, copying the pointee value, and allocating on write.
// To manually force allocation (when bINIT = false): *ptr
// To manually free it: ptr = 0

template<class T, bool bINIT = false>
struct deep_ptr
{
	typedef deep_ptr<T,bINIT> this_t;

	~deep_ptr()
		{ delete m_ptr; }

	deep_ptr()
		: m_ptr( bINIT ? new T() : 0 ) {}

	deep_ptr( T* p )
		: m_ptr(p) {}

	deep_ptr( const this_t& init )
		: m_ptr( init.m_ptr ? new T(*init.m_ptr) : 0) {}

	this_t& operator=( const this_t& init )
	{
		if (init.m_ptr != m_ptr)
		{
			delete m_ptr;
			m_ptr = init.m_ptr ? new T(*init.m_ptr) : 0;
		}
		return *this;
	}

	this_t& operator=( T* p )
	{
		if (p != m_ptr)
		{
			delete m_ptr;
			m_ptr = p;
		}
		return *this;
	}

	// Read access
	operator bool() const
		{ return !!m_ptr; }
	bool operator !() const
		{ return !m_ptr; }

	const T* operator ->() const
	{
		assert(m_ptr);
		return m_ptr;
	}
	T* operator ->()
	{
		// When writing empty value, allocate new.
		if (bINIT)
		{
			assert(m_ptr);
		}
		else if (!m_ptr) 
			m_ptr = new T;
		return m_ptr;
	}

	const T& operator *() const
		{ return *operator->(); }
	T& operator *()
		{ return *operator->(); }

	void GetMemoryUsage(ICrySizer *pSizer) const
	{
		if (m_ptr)
			m_ptr->GetMemoryUsage(pSizer, true);
	}

	CUSTOM_STRUCT_INFO(CCustomInfo)

private:
	T*	m_ptr;

	// Provide interface to pointee data.
	struct CCustomInfo: CTypeInfo
	{
		static const CTypeInfo& ValTypeInfo()
			{ return ::TypeInfo((T*)0); }

		CCustomInfo()
			: CTypeInfo( ValTypeInfo().Name, sizeof(T*), alignof(T*)) {}

		static void FreeDefault(this_t& ptr)
		{
			T objDefault;
			if (ValTypeInfo().ValueEqual(&*ptr, &objDefault))
				ptr = 0;
		}

		virtual bool IsType( CTypeInfo const& Info ) const		
			{ return &Info == this || ValTypeInfo().IsType(Info); }

		// Read functions: read from a default object if not allocated.
		virtual string ToString(const void* data, FToString flags = 0, const void* def_data = 0) const
		{
			const T* ptr = *(const T**)data;
			const T* def_ptr = def_data ? *(const T**)def_data : 0;
			if (ptr)
			{
				return ValTypeInfo().ToString(ptr, flags, def_ptr); 
			}
			else
			{
				T objDefault;
				return ValTypeInfo().ToString(&objDefault, flags, def_ptr);
			}
		}
		virtual bool ToValue(const void* data, void* value, const CTypeInfo& typeVal) const
		{
			const T* ptr = *(const T**)data;
			if (ptr)
			{
				return ValTypeInfo().ToValue(ptr, value, typeVal); 
			}
			else
			{
				T objDefault;
				return ValTypeInfo().ToValue(&objDefault, value, typeVal); 
			}
		}
		virtual bool ValueEqual(const void* data, const void* def_data = 0) const
		{
			const T* ptr = *(const T**)data;
			const T* def_ptr = def_data ? *(const T**)def_data : 0;
			if (!ptr && !def_ptr)
				return true;
			if (ptr && def_ptr)
				return ValTypeInfo().ValueEqual(ptr, def_ptr); 
			T objDefault;
			return ValTypeInfo().ValueEqual(ptr ? ptr : &objDefault, def_ptr ? def_ptr : &objDefault); 
		}

		// Write functions: allocate and copy data only if non-default.
		virtual bool FromString(void* data, cstr str, FFromString flags = 0) const
		{
			this_t& ptr = *(this_t*)data;
			if (!*str)
			{
				if (!flags._SkipEmpty)
					ptr = 0;
				return true;
			}
			bool bSuccess = ValTypeInfo().FromString( &*ptr, str, flags ); 
			FreeDefault(ptr);
			return bSuccess;
		}
		virtual bool FromValue(void* data, const void* value, const CTypeInfo& typeVal) const
		{ 
			this_t& ptr = *(this_t*)data;
			bool bSuccess = ValTypeInfo().FromValue( &*ptr, value, typeVal );
			FreeDefault(ptr);
			return bSuccess;
		}

		virtual void SwapEndian(void* data, size_t nCount, bool bWriting) const
		{
			T* ptr = *(T**)data;
			if (ptr)
				ValTypeInfo().SwapEndian(ptr, nCount, bWriting);
		}
		virtual void GetMemoryUsage(ICrySizer* pSizer, const void* data) const
		{
			const T* ptr = *(const T**)data;
			if (ptr)
				ValTypeInfo().GetMemoryUsage(pSizer, ptr);
		}
	};
};


#pragma warning(pop)
#endif
