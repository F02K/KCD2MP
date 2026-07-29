//////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2009.
// -------------------------------------------------------------------------
//  File name:   CryCreateClassInstance.h
//  Version:     v1.00
//  Created:     02/25/2009 by CarstenW
//  Description: Part of CryEngine's extension framework.
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#ifndef _CRYCREATECLASSINSTANCE_H_
#define _CRYCREATECLASSINSTANCE_H_

#pragma once


#include "ICryUnknown.h"
#include "ICryFactory.h"
#include "ICryFactoryRegistry.h"
#include <ISystem.h> // <> required for Interfuscator


template <class T>
bool CryCreateClassInstance(const CryClassID& cid, boost::shared_ptr<T>& p)
{
	p = boost::shared_ptr<T>();
	ICryFactoryRegistry* pFactoryReg = gEnv->pSystem->GetCryFactoryRegistry();
	if (pFactoryReg)
	{
		ICryFactory* pFactory = pFactoryReg->GetFactory(cid);
		if (pFactory && pFactory->ClassSupports(cryiidof<T>()))
		{
			ICryUnknownPtr pUnk = pFactory->CreateClassInstance();
			boost::shared_ptr<T> pT = cryinterface_cast<T>(pUnk);
			if (pT)
				p = pT;
		}
	}
    return (p) ? true : false;
}


template <class T>
bool CryCreateClassInstance(const char* cname, boost::shared_ptr<T>& p)
{
	p = boost::shared_ptr<T>();
	ICryFactoryRegistry* pFactoryReg = gEnv->pSystem->GetCryFactoryRegistry();
	if (pFactoryReg)
	{
		ICryFactory* pFactory = pFactoryReg->GetFactory(cname);
		if (pFactory != NULL && pFactory->ClassSupports(cryiidof<T>()))
		{
			ICryUnknownPtr pUnk = pFactory->CreateClassInstance();
			boost::shared_ptr<T> pT = cryinterface_cast<T>(pUnk);
			if (pT)
				p = pT;
		}
	}
    return (p) ? true : false;
}


template <class T>
bool CryCreateClassInstanceForInterface(const CryInterfaceID& iid, boost::shared_ptr<T>& p)
{
	p = boost::shared_ptr<T>();
	ICryFactoryRegistry* pFactoryReg = gEnv->pSystem->GetCryFactoryRegistry();
	if (pFactoryReg)
	{
		size_t numFactories = 1;
		ICryFactory* pFactory = 0;
		pFactoryReg->IterateFactories(iid, &pFactory, numFactories);
		if (numFactories == 1 && pFactory)
		{
			ICryUnknownPtr pUnk = pFactory->CreateClassInstance();
			boost::shared_ptr<T> pT = cryinterface_cast<T>(pUnk);
			if (pT)
				p = pT;
		}
	}
    return (p) ? true : false;
}


#endif // #ifndef _CRYCREATECLASSINSTANCE_H_
