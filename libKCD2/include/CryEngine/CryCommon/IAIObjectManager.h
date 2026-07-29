#ifndef IAIOBJECTMANAGER
#define IAIOBJECTMANAGER

#if _MSC_VER > 1000
#pragma once
#endif


#include <IAIObject.h> // <> required for Interfuscator


/// Filter flags for IAISystem::GetFirstAIObject
enum EGetFirstFilter
{
	OBJFILTER_TYPE,			// Include only objects of specified type (type 0 means all objects).
	OBJFILTER_GROUP,		// Include only objects of specified group id.
	OBJFILTER_FACTION,	// Include only objects of specified faction.
	OBJFILTER_DUMMYOBJECTS,	// Include only dummy objects
};

struct IAIObjectManager
{
	// <interfuscator:shuffle>
	virtual ~IAIObjectManager(){}
	virtual IAIObject *CreateAIObject(const AIObjectParams& params) = 0;
	virtual void RemoveObject(tAIObjectID objectID) = 0;//same as destroy??

	virtual IAIObject *GetAIObject(tAIObjectID aiObjectID) = 0;
	virtual IAIObject *GetAIObjectByName(unsigned short type, const char *pName) const = 0;


	// Returns AIObject iterator for first match, see EGetFirstFilter for the filter options.
	// The parameter 'n' specifies the type, group id or species based on the selected filter.
	// It is possible to iterate over all objects by setting the filter to OBJFILTER_TYPE
	// passing zero to 'n'.
	virtual IAIObjectIter* GetFirstAIObject(EGetFirstFilter filter, short n) = 0;
	// Iterates over AI objects within specified range.
	// Parameter 'pos' and 'rad' specify the enclosing sphere, for other parameters see GetFirstAIObject.
	virtual IAIObjectIter* GetFirstAIObjectInRange(EGetFirstFilter filter, short n, const Vec3& pos, float rad, bool check2D) = 0;
	// </interfuscator:shuffle>
};


#endif