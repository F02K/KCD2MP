/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2011.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Telemetry parameters.

-------------------------------------------------------------------------
History:
- 12:05:2011: Created by Paul Slinger

*************************************************************************/

#ifndef __TELEMETRYPARAMS_H__
#define __TELEMETRYPARAMS_H__

#include "GenericParam.h"
#include "VariadicParams.h"
#include "TelemetryConfig.h"
#include "TelemetryFormat.h"

namespace Telemetry
{
	typedef CGenericParam<GenericParamUtils::CPtrStorage> TParam;

	typedef CVariadicParams<TParam> TVariadicParams;

	inline TParam FilterParam(const char *value, const char *prev)
	{
		return TParam(value, (value != prev) && (strcmp(value, prev) != 0));
	}

	template <typename TYPE> inline TParam FilterParam(const TYPE &value, const TYPE &prev)
	{
		return TParam(value, value != prev);
	}

	template <typename TYPE> inline TParam FilterParamDelta(const TYPE &value, const TYPE &prev, const TYPE &minDelta)
	{
		return TParam(value, Abs(value - prev) >= minDelta);
	}
}

#endif //__TELEMETRYPARAMS_H__