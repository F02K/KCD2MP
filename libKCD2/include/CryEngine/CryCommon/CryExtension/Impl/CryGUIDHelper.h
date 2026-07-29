//////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2009.
// -------------------------------------------------------------------------
//  File name:   CryGUIDHelper.h
//  Version:     v1.00
//  Created:     02/25/2009 by CarstenW
//  Description: Part of CryEngine's extension framework.
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#ifndef _CRYGUIDHELPER_H_
#define _CRYGUIDHELPER_H_

#pragma once


#include "../CryGUID.h"
#include "../../CryString.h"


namespace CryGUIDHelper
{
	string Print(const CryGUID& val)
	{
		char buf[39]; // sizeof("{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}")

		static const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		char* p = buf;
		*p++ = '{';
		for (int i=15; i>=8; --i)
			*p++ = hex[(unsigned char) ((val.hipart >> (i<<2)) & 0xF)];
		*p++ = '-';
		for (int i=7; i>=4; --i)
			*p++ = hex[(unsigned char) ((val.hipart >> (i<<2)) & 0xF)];
		*p++ = '-';
		for (int i=3; i>=0; --i)
			*p++ = hex[(unsigned char) ((val.hipart >> (i<<2)) & 0xF)];
		*p++ = '-';
		for (int i=15; i>=12; --i)
			*p++ = hex[(unsigned char) ((val.lopart >> (i<<2)) & 0xF)];
		*p++ = '-';
		for (int i=11; i>=0; --i)
			*p++ = hex[(unsigned char) ((val.lopart >> (i<<2)) & 0xF)];
		*p++ = '}';
		*p++ = '\0';

		return string(buf);
	}
}


#endif // #ifndef _CRYGUIDHELPER_H_
