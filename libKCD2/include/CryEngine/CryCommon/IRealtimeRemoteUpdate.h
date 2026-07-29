/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2004.
-------------------------------------------------------------------------
$Id: IRealtimeRemoveUpdate.h,v 1.1 2009/01/03 11:36:28 PauloZaffari Exp wwwrun $
$DateTime$
Description:  This is the interface for the realtime remote update system
							the main purpose for it is to allow users to enable or 
							disable it as needed.
-------------------------------------------------------------------------
History:
- 03:01:2009   11:36: Created by Paulo Zaffari
*************************************************************************/
#ifndef IRealtimeRemoteUpdate_h__
#define IRealtimeRemoteUpdate_h__

#pragma once

UNIQUE_IFACE struct IRealtimeUpdateGameHandler
{
	// <interfuscator:shuffle>
	virtual ~IRealtimeUpdateGameHandler(){}
	virtual bool UpdateGameData(XmlNodeRef oXmlNode, unsigned char * auchBinaryData) = 0;
	// </interfuscator:shuffle>
};

UNIQUE_IFACE struct IRealtimeRemoteUpdate
{ 
	// <interfuscator:shuffle>
	virtual ~IRealtimeRemoteUpdate(){}
	// Description:
	//   Enables or disables the realtime remote update system.
	// See Also: 
	// Arguments:
	//   boEnable: if true enable the realtime remote update system
	//						 if false, disables the realtime realtime remote 
	//							update system
	// Return:
	//   bool - true if the operation succeeded, false otherwise.

	virtual bool	Enable(bool boEnable=true)=0;

	// Description:
	//   Checks if the realtime remote update system is enabled.
	// See Also: 
	// Arguments:
	//	Nothing
	// Return:
	//   bool - true if the system is running, otherwise false.
	virtual bool	IsEnabled()=0;

	// Description:
	//    Method allowing us to use a pass-through mechanism for testing.
	// Arguments:
	//   pBuffer: contains the received message.
	//   lenght:  contains the lenght of the received message.
	// Return:
	//  Nothing.
	virtual void	OnNotificationNetworkReceive(const void *pBuffer, size_t length)=0;

	// Description:
	//		Method used to add a game specific handler to the live preview 	
	// Arguments:
	//		Interface to the game side handler that will be used for live preview
	// Return:
	//		Nothing.
	virtual void AddGameHandler(IRealtimeUpdateGameHandler * handler) = 0;

	// Description:
	//		Method used to remove the game specific handler from the live preview 	
	// Arguments:
	//		Interface to the game side handler that will be used for live preview
	// Return:
	//		Nothing.
	virtual void RemoveGameHandler(IRealtimeUpdateGameHandler * handler) = 0;

	// Description:
	//   Method used to check if the editor is currently syncing data to the 
	// the current engine instance withing the timeout of 30 seconds.
	// Return Value:
	//   True if the editor is syncing. Otherwise, false.
	virtual bool IsSyncingWithEditor()=0;

	// Description:
	//   Method used to do everything which was scheduled to be done in the
	// main thread.
	// Return Value:
	//   none
	virtual void Update()=0;
	// </interfuscator:shuffle>
};




#endif // IRealtimeRemoteUpdate_h__
