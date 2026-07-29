/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2004.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Item parameters structure.

-------------------------------------------------------------------------
History:
- 7:10:2005   14:20 : Created by Márcio Martins

*************************************************************************/
#ifndef __ITEMPARAMS_H__
#define __ITEMPARAMS_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "IItemSystem.h"
#include <ConfigurableVariant.h>
#include <StlUtils.h>


template <class F, class T>
struct SItemParamConversion
{
	static ILINE bool ConvertValue(const F &from, T &to)
	{
		to = (T)from;
		return true;
	};
};


// taken from IFlowSystem.h and adapted...
#define ITEMSYSTEM_STRING_CONVERSION(type, fmt) \
	template <> \
	struct SItemParamConversion<type, string> \
	{ \
		static ILINE bool ConvertValue( const type& from, string& to ) \
	{ \
		to.Format( fmt, from ); \
		return true; \
	} \
	}; \
	template <> \
	struct SItemParamConversion<string, type> \
	{ \
		static ILINE bool ConvertValue( const string& from, type& to ) \
	{ \
		return 1 == sscanf( from.c_str(), fmt, &to ); \
	} \
	};

ITEMSYSTEM_STRING_CONVERSION(int, "%d");
ITEMSYSTEM_STRING_CONVERSION(float, "%f");

#undef ITEMSYSTEM_STRING_CONVERSION


template<>
struct SItemParamConversion<Vec3, bool>
{
	static ILINE bool ConvertValue(const Vec3 &from, float &to)
	{
		to = from.GetLengthSquared()>0;
		return true;
	}
};

template<>
struct SItemParamConversion<bool, Vec3>
{
	static ILINE bool ConvertValue(const float &from, Vec3 &to)
	{
		to = Vec3(from?1.f:0.f, from?1.f:0.f, from?1.f:0.f);
		return true;
	}
};

template<>
struct SItemParamConversion<Vec3, float>
{
	static ILINE bool ConvertValue(const Vec3 &from, float &to)
	{
		to = from.x;
		return true;
	}
};

template<>
struct SItemParamConversion<float, Vec3>
{
	static ILINE bool ConvertValue(const float &from, Vec3 &to)
	{
		to = Vec3(from, from, from);
		return true;
	}
};

template<>
struct SItemParamConversion<Vec3, int>
{
	static ILINE bool ConvertValue(const Vec3 &from, int &to)
	{
		to = (int)from.x;
		return true;
	}
};

template<>
struct SItemParamConversion<int, Vec3>
{
	static ILINE bool ConvertValue(const int &from, Vec3 &to)
	{
		to = Vec3((float)from, (float)from, (float)from);
		return true;
	}
};

template<>
struct SItemParamConversion<Vec3, string>
{
	static ILINE bool ConvertValue(const Vec3 &from, string &to)
	{
		to.Format("%s,%s,%s", from.x, from.y, from.z);
		return true;
	}
};

template<>
struct SItemParamConversion<string, Vec3>
{
	static ILINE bool ConvertValue(const string &from, Vec3 &to)
	{
		return sscanf(from.c_str(), "%f,%f,%f", &to.x, &to.y, &to.z) == 3;
	}
};


typedef	CConfigurableVariant<TItemParamTypes, sizeof(void *), SItemParamConversion> TItemParamValue;


class CItemParamsNode: public IItemParamsNode
{
public:

	enum EXMLFilterType
	{
		eXMLFT_none = 0,
		eXMLFT_add = 1,
		eXMLFT_remove = 2
	};

	CItemParamsNode();
	virtual ~CItemParamsNode();

	virtual void AddRef() const { ++m_refs; };
	VIRTUAL uint32 GetRefCount() const { return m_refs; };
	virtual void Release() const { if (!--m_refs) delete this; };

	VIRTUAL void GetMemoryUsage(ICrySizer *pSizer) const;

	VIRTUAL int GetAttributeCount() const;
	VIRTUAL const char *GetAttributeName(int i) const;
	VIRTUAL const char *GetAttribute(int i) const;
	VIRTUAL bool GetAttribute(int i, Vec3 &attr) const;
	VIRTUAL bool GetAttribute(int i, Ang3 &attr) const;
	VIRTUAL bool GetAttribute(int i, float &attr) const;
	VIRTUAL bool GetAttribute(int i, int &attr) const;
	VIRTUAL int GetAttributeType(int i) const;

	VIRTUAL const char *GetAttribute(const char *name) const;
	VIRTUAL bool GetAttribute(const char *name, Vec3 &attr) const;
	VIRTUAL bool GetAttribute(const char *name, Ang3 &attr) const;
	VIRTUAL bool GetAttribute(const char *name, float &attr) const;
	VIRTUAL bool GetAttribute(const char *name, int &attr) const;
	VIRTUAL int GetAttributeType(const char *name) const;

	VIRTUAL const char *GetAttributeSafe(const char *name) const;

	VIRTUAL const char *GetNameAttribute() const;
	
	VIRTUAL int GetChildCount() const;
	VIRTUAL const char *GetChildName(int i) const;
	VIRTUAL const IItemParamsNode *GetChild(int i) const;
	VIRTUAL const IItemParamsNode *GetChild(const char *name) const;
	
	EXMLFilterType ShouldConvertNodeFromXML(const XmlNodeRef &xmlNode, const char * keepWithThisAttrValue) const;

	VIRTUAL void SetAttribute(const char *name, const char *attr);
	VIRTUAL void SetAttribute(const char *name, const Vec3 &attr);
	VIRTUAL void SetAttribute(const char *name, float attr);
	VIRTUAL void SetAttribute(const char *name, int attr);

	VIRTUAL void SetName(const char *name) { m_name = name; };
	VIRTUAL const char *GetName() const { return m_name.c_str(); };

	VIRTUAL IItemParamsNode *InsertChild(const char *name);
	VIRTUAL void ConvertFromXML(const XmlNodeRef &root);
	VIRTUAL bool ConvertFromXMLWithFiltering(const XmlNodeRef &root, const char * keepWithThisAttrValue);

private:
	struct SAttribute
	{
		CCryName first; // Using CryName to save memory on duplicate strings
		TItemParamValue second;

		SAttribute() {}
		SAttribute( const char *key,const TItemParamValue &val) { first = key; second = val; }

		void GetMemoryUsage(ICrySizer *pSizer) const
		{
			pSizer->AddObject(second);
		}
	};

	typedef DynArray<SAttribute>																						TAttributeMap;
	typedef DynArray<CItemParamsNode *>																			TChildVector;

	template<typename MT>
		typename MT::const_iterator GetConstIterator(const MT &m, int i) const
	{
		typename MT::const_iterator it = m.begin();
		//std::advance(it, i);
		it += i;
		return it;
	};
	TAttributeMap::const_iterator FindAttrIterator( const TAttributeMap &m,const char *name) const
	{
		TAttributeMap::const_iterator it;
		for (it = m.begin(); it != m.end(); ++it)
		{
			if (stricmp(it->first.c_str(),name) == 0)
				return it;
		}
		return m.end();
	};
	TAttributeMap::iterator FindAttrIterator( TAttributeMap &m,const char *name) const
	{
		TAttributeMap::iterator it;
		for (it = m.begin(); it != m.end(); ++it)
		{
			if (stricmp(it->first.c_str(),name) == 0)
				return it;
		}
		return m.end();
	};
	void AddAttribute( const char *name,const TItemParamValue &val )
	{
		TAttributeMap::iterator it = FindAttrIterator(m_attributes,name);
		if (it == m_attributes.end())
		{
			m_attributes.push_back( SAttribute(name,val) );
		}
		else
			it->second = val;
	}

	CCryName      m_name;
	string				m_nameAttribute;
	TAttributeMap	m_attributes;
	TChildVector	m_children;
	
	mutable uint32	m_refs;
};


#endif