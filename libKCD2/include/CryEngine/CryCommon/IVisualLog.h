////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   IVisualLog.h
//  Description: Interface of the VisualLog
//    The VisualLog system captures frames at runtime and associates
//    it with logging data for subsequent offline playback
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(IVisualLog.h)

#ifndef __IVISUAL_LOG_H_
#define __IVISUAL_LOG_H_
#pragma once

struct SVisualLogParams
{
	ColorF color;
	float size;
	int column;
	bool alignColumnsToThis;

	SVisualLogParams()	{ Init(); }
	SVisualLogParams( ColorF color )
	{
		Init();
		this->color = color;
	}
	SVisualLogParams( ColorF color, float size )
	{
		Init();
		this->color = color;
		this->size = size;
	}
	SVisualLogParams( ColorF color, float size, int column, bool align )
	{
		Init();
		this->color = color;
		this->size = size;
		this->column = column;
		this->alignColumnsToThis = align;
	}

private:
	void Init()
	{
		color = ColorF(1.f,1.f,1.f,1.f);
		size = 2.f;
		column = 1;
		alignColumnsToThis = false;
	}
};

UNIQUE_IFACE struct IVisualLog
{
	// <interfuscator:shuffle>
	virtual ~IVisualLog(){}
	virtual void Log ( const char *format, ... )  PRINTF_PARAMS(2, 3) = 0;
	virtual void Log ( const SVisualLogParams& params, const char *format, ... )  PRINTF_PARAMS(3, 4) = 0;

	virtual void Reset() = 0;
	// </interfuscator:shuffle>
};

#endif // __IVISUAL_LOG_H_
