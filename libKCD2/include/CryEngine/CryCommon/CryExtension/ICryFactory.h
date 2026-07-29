//////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2009.
// -------------------------------------------------------------------------
//  File name:   ICryFactory.h
//  Version:     v1.00
//  Created:     02/25/2009 by CarstenW
//  Description: Part of CryEngine's extension framework.
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#ifndef _ICRYFACTORY_H_
#define _ICRYFACTORY_H_

#pragma once


#include "CryTypeID.h"
#include <BoostHelpers.h>

struct ICryUnknown;
DECLARE_BOOST_POINTERS(ICryUnknown);

struct ICryFactory
{
	virtual const char* GetClassName() const = 0;
	virtual const CryClassID& GetClassID() const = 0;
	virtual bool ClassSupports(const CryInterfaceID& iid) const = 0;
	virtual void ClassSupports(const CryInterfaceID*& pIIDs, size_t& numIIDs) const = 0;
	virtual ICryUnknownPtr CreateClassInstance() const = 0;

protected:
	// prevent explicit destruction from client side (delete, shared_ptr, etc)
	virtual ~ICryFactory() {}
};

#endif // #ifndef _ICRYFACTORYREGISTRY_H_
