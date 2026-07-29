#ifndef __ICRYFRIENDS_H__
#define __ICRYFRIENDS_H__

#pragma once

#include <ICryLobby.h> // <> required for Interfuscator

#define CRYLOBBY_PRESENCE_MAX_SIZE				256

struct SFriendInfo 
{
	CryUserID								userID;
	char										name[CRYLOBBY_USER_NAME_LENGTH];
	char										presence[CRYLOBBY_PRESENCE_MAX_SIZE];
	ELobbyFriendStatus			status;
};

// CryFriendsCallback
// taskID				-	Task ID allocated when the function was executed.
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function.
// pArg					- Pointer to application-specified data.
typedef void (*CryFriendsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, void* pArg);

// CryFriendsGetFriendsListCallback
// taskID				-	Task ID allocated when the function was executed
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// pFriendInfo	- Pointer to an array of SFriendInfo containing info about the friends retrieved.
// numFriends		- Number of items in the pFriendInfo array.
// pArg					- Pointer to application-specified data.
typedef void (*CryFriendsGetFriendsListCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SFriendInfo* pFriendInfo, uint32 numFriends, void* pArg);

struct ICryFriends
{
	// <interfuscator:shuffle>
	virtual ~ICryFriends(){}
	// FriendsGetFriendsList
	// Retrieves the Friends list of the specified user.
	// user				- The pad number of the user to retrieve the friends list for.
	// start			- The start index to retrieve from. First friend is 0.
	// num				- Maximum number of friends to retrieve.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual	ECryLobbyError	FriendsGetFriendsList(uint32 user, uint32 start, uint32 num, CryLobbyTaskID* pTaskID, CryFriendsGetFriendsListCallback cb, void* pCbArg) = 0;

	// FriendsSendGameInvite
	// Send game invites to the given list of users for the given session.
	// user				- The pad number of the user sending the game invites.
	// h					- The handle to the session the invites are for.
	// pUserIDs		- Pointer to an array of user ids to send invites to
	// numUserIDs	- The number of users to invite
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError	FriendsSendGameInvite(uint32 user, CrySessionHandle h, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsCallback cb, void* pCbArg) = 0;
	// </interfuscator:shuffle>
};

#endif // __ICRYFRIENDS_H__