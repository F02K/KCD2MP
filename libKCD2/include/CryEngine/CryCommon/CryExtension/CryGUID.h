//////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2009.
// -------------------------------------------------------------------------
//  File name:   CryGUID.h
//  Version:     v1.00
//  Created:     02/25/2009 by CarstenW
//  Description: Part of CryEngine's extension framework.
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#ifndef _CRYGUID_H_
#define _CRYGUID_H_

#pragma once


struct CryGUID
{
	uint64 hipart;
	uint64 lopart;

	// !!! Do NOT turn CryGUID into a non-aggregate !!!
	// It will prevent inlining and type list unrolling opportunities within 
	// cryinterface_cast<T>() and cryiidof<T>(). As such prevent constructors,
	// non-public members, base classes and virtual functions!

	//CryGUID() : hipart(0), lopart(0) {}
	//CryGUID(uint64 h, uint64 l) : hipart(h), lopart(l) {}

	static CryGUID Construct(const uint64& hipart, const uint64& lopart)
	{
		CryGUID guid = {hipart, lopart};
		return guid;
	}

	bool operator ==(const CryGUID& rhs) const {return hipart == rhs.hipart && lopart == rhs.lopart;}
	bool operator !=(const CryGUID& rhs) const {return hipart != rhs.hipart || lopart != rhs.lopart;}
	bool operator <(const CryGUID& rhs) const {return hipart == rhs.hipart ? lopart < rhs.lopart : hipart < rhs.hipart;}
};


#define MAKE_CRYGUID(high, low) CryGUID::Construct((uint64) high##LL, (uint64) low##LL)


#endif // #ifndef _CRYGUID_H_
