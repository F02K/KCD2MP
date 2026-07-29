/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2011.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Telemetry configuration.

-------------------------------------------------------------------------
History:
- 12:05:2011: Created by Paul Slinger

*************************************************************************/

#ifndef __TELEMETRYCONFIG_H__
#define __TELEMETRYCONFIG_H__

#ifndef TELEMETRY_DEBUG
#ifdef _DEBUG
#define TELEMETRY_DEBUG 1
#else
#define TELEMETRY_DEBUG 0
#endif //_DEBUG
#endif //TELEMETRY_DEBUG

#define TELEMETRY_VERBOSITY_OFF			0
#define TELEMETRY_VERBOSITY_LOWEST	1
#define TELEMETRY_VERBOSITY_LOW			2
#define TELEMETRY_VERBOSITY_DEFAULT	3
#define TELEMETRY_VERBOSITY_HIGH		4
#define TELEMETRY_VERBOSITY_HIGHEST	5
#define TELEMETRY_VERBOSITY_NOT_SET	6

#ifndef TELEMETRY_MIN_VERBOSITY 
#define TELEMETRY_MIN_VERBOSITY TELEMETRY_VERBOSITY_HIGHEST
#endif //TELEMETRY_MIN_VERBOSITY

#endif //__TELEMETRYCONFIG_H__