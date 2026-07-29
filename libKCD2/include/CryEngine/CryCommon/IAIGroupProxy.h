#ifndef __IAIGroupProxy_h__
#define __IAIGroupProxy_h__

#pragma once


#include <IAIObject.h>


struct IAIGroupProxyFactory
{
	// <interfuscator:shuffle>
	virtual IAIGroupProxy* CreateGroupProxy(int groupID) = 0;
	virtual ~IAIGroupProxyFactory(){}
	// </interfuscator:shuffle>
};


UNIQUE_IFACE struct IAIGroupProxy :
public _reference_target_t
{
	// <interfuscator:shuffle>
	virtual void Reset(EObjectResetType type) = 0;
	virtual void Serialize(TSerialize ser) = 0;

	virtual const char* GetCurrentBehaviorName() const = 0;
	virtual const char* GetPreviousBehaviorName() const = 0;

	virtual void Notify(uint32 notificationID, tAIObjectID senderID, const char* notification) = 0;

	virtual void SetBehaviour(const char* behaviour, bool callCDtors = true) = 0;

	virtual void MemberAdded(tAIObjectID memberID) = 0;
	virtual void MemberRemoved(tAIObjectID memberID) = 0;

	virtual IScriptTable* GetScriptTable() = 0;
	// </interfuscator:shuffle>
};

#endif