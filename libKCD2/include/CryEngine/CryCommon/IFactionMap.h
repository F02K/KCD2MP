#ifndef __IFactionMap_h__
#define __IFactionMap_h__

#pragma once


struct IFactionMap
{
	enum ReactionType
	{
		Hostile = 0, // intentionally from most-hostile to most-friendly
		Neutral,
		Friendly,
	};

	enum
	{
		InvalidFactionID = 0xff,
	};

	// <interfuscator:shuffle>
	virtual ~IFactionMap(){}
	virtual uint32 GetFactionCount() const = 0;
	virtual const char* GetFactionName(uint8 fraction) const = 0;
	virtual uint8 GetFactionID(const char* name) const = 0;

	virtual void SetReaction(uint8 factionOne, uint8 factionTwo, IFactionMap::ReactionType reaction) = 0;
	virtual IFactionMap::ReactionType GetReaction(uint8 factionOne, uint8 factionTwo) const = 0;
	// </interfuscator:shuffle>
};

#endif