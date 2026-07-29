/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2011.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Telemetry format.

-------------------------------------------------------------------------
History:
- 12:05:2011: Created by Paul Slinger

*************************************************************************/

#ifndef __TELEMETRYFORMAT_H__
#define __TELEMETRYFORMAT_H__

namespace Telemetry
{
	struct EChunkId { enum
	{
		PacketHeader = 0,
		SessionInfo,
		EventDeclaration,
		Event
	}; };

	struct SChunkHeader
	{
		uint16	id, size;
	};
}

#endif //__TELEMETRYFORMAT_H__