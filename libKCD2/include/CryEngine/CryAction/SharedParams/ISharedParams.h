/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2010.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Shared parameters type information.

-------------------------------------------------------------------------
History:
- 15:07:2010: Created by Paul Slinger

*************************************************************************/

#ifndef __ISHAREDPARAMSTYPE_H__
#define __ISHAREDPARAMSTYPE_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "ISharedParamsManager.h"
#include "SharedParamsTypeInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared parameters interface class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ISharedParams
{
	public:

		virtual ~ISharedParams()
		{
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Clone.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ISharedParams *Clone() const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Get type information.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual const CSharedParamsTypeInfo &GetTypeInfo() const = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Begin shared parameters.
////////////////////////////////////////////////////////////////////////////////////////////////////
#define BEGIN_SHARED_PARAMS(name)														\
struct name;																								\
																														\
DECLARE_BOOST_POINTERS(name);																\
																														\
struct name : public ISharedParams													\
{																														\
	virtual ISharedParams *Clone() const											\
	{																													\
		return new name(*this);																	\
	}																													\
																														\
	virtual const CSharedParamsTypeInfo &GetTypeInfo() const	\
	{																													\
		return s_typeInfo;																			\
	}																													\
																														\
	static const CSharedParamsTypeInfo s_typeInfo;

////////////////////////////////////////////////////////////////////////////////////////////////////
// End shared parameters.
////////////////////////////////////////////////////////////////////////////////////////////////////
#define END_SHARED_PARAMS };

////////////////////////////////////////////////////////////////////////////////////////////////////
// Define shared parameters type information.
////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEFINE_SHARED_PARAMS_TYPE_INFO(name) const CSharedParamsTypeInfo name::s_typeInfo(sizeof(name), #name, __FILE__, __LINE__);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Cast shared parameters pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TO> inline boost::shared_ptr<TO> CastSharedParamsPtr(ISharedParamsPtr pSharedParams)
{
	if(pSharedParams && (pSharedParams->GetTypeInfo() == TO::s_typeInfo))
	{
		return boost::static_pointer_cast<TO>(pSharedParams);
	}
	else
	{
		return boost::shared_ptr<TO>();
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Cast shared parameters pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TO> inline boost::shared_ptr<const TO> CastSharedParamsPtr(ISharedParamsConstPtr pSharedParams)
{
	if(pSharedParams && (pSharedParams->GetTypeInfo() == TO::s_typeInfo))
	{
		return boost::static_pointer_cast<const TO>(pSharedParams);
	}
	else
	{
		return boost::shared_ptr<const TO>();
	}
};

#endif //__ISHAREDPARAMSTYPE_H__