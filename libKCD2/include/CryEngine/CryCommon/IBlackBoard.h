#include DEVIRTUALIZE_HEADER_FIX(IBackboard.h)

#ifndef _BLACKBOARD_H_
#define _BLACKBOARD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

UNIQUE_IFACE struct IBlackBoard
{
	// <interfuscator:shuffle>
	virtual SmartScriptTable& GetForScript() = 0;
	virtual void							SetFromScript( SmartScriptTable& ) = 0;
	virtual void							Clear() = 0;	
	virtual ~IBlackBoard(){}
	// </interfuscator:shuffle>
};

#endif // _BLACKBOARD_H_