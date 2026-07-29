#ifndef __ICRYLOBBYUI_H__
#define __ICRYLOBBYUI_H__

#pragma once

#include <ICryLobby.h> // <> required for Interfuscator


// SCryLobbyUIOnlineRetailCounts
// For returning the number of available DLC items in the Online Retail system.
struct SCryLobbyUIOnlineRetailCounts
{
	uint32			newItems;
	uint32			totalItems;
};


// CryLobbyUICallback
// taskID		-	Task ID allocated when the function was executed
// error		- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// pArg			- Pointer to application-specified data
typedef void (*CryLobbyUICallback)(CryLobbyTaskID taskID, ECryLobbyError error, void* pArg);

// CryLobbyUIOnlineRetailStatusCallback
// taskID		-	Task ID allocated when the function was executed
// error		- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// pArg			- Pointer to application-specified data
typedef void (*CryLobbyUIOnlineRetailStatusCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCryLobbyUIOnlineRetailCounts* pCounts, void* pArg);


struct ICryLobbyUI
{
	// <interfuscator:shuffle>
	virtual ~ICryLobbyUI(){}
	// ShowGamerCard
	// Show the gamer card of the given user id.
	// user				-	The pad number of the user making the call
	// userID			- The the user id of the gamer card to show
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowGamerCard(uint32 user, CryUserID userID, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// ShowGameInvite
	// Show the game invite ui to invite users to a session.
	// user				-	The pad number of the user making the call
	// h					- A handle to the session the invite is for
	// pUserIDs		- Pointer to an array of user ids to send invites to
	// numUserIDs	- The number of users to invite
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowGameInvite(uint32 user, CrySessionHandle h, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// ShowFriends
	// Show the friends ui for the given user.
	// user				-	The pad number of the user making the call
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual	ECryLobbyError					ShowFriends(uint32 user, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// ShowFriendRequest
	// Show the friend request ui for inviting a user to be your friend.
	// user				-	The pad number of the user making the call
	// userID			- The the user id of the user to invite
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowFriendRequest(uint32 user, CryUserID userID, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// ShowParty
	// XBOX LIVE Only
	// Show the Live party ui.
	// TCR 138 can be satisfied by providing an option labeled "Invite Xbox LIVE Party" and calling ShowParty when a user is in a party with at least one other user
	// and providing an option labeled "Invite Friends" and calling ShowFriends if they are not.
	// user				-	The pad number of the user making the call
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowParty(uint32 user, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// ShowCommunitySessions
	// XBOX LIVE Only
	// Show Live Community Sessions ui
	// TCR 139 can be satisfied by calling this function.
	// user				-	The pad number of the user making the call
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowCommunitySessions(uint32 user, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// SetRichPresence
	// Set the Rich Presence string for a user that will be displayed in the platform UI.
	// For Live a Rich Presence string can be up to 2 lines each of up to 22 characters.
	// user				-	The pad number of the user making the call
	// pData			- An array of SCryLobbyUserData used to define the string.
	//						- The array should start with the main presence string that can contain embedded format tags followed by the items to fill those tags.
	// numData		- The number of user data that makes up the string
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					SetRichPresence(uint32 user, SCryLobbyUserData* pData, uint32 numData, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// ShowOnlineRetailUI
	// Starts the platform UI online retailing process by terminating the game.
	// user				-	The pad number of the user making the call
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowOnlineRetailBrowser(uint32 user, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	// CheckOnlineRetailStatus
	// Gathers information about new and existing DLC content available on the online retailing service.
	// Data is returned in the callback in a SCryLobbyUIOnlineRetailCounts structure.
	// Worth noting that on PS3 this allocates approximately 1200KB of temporary working buffers for the duration of the task.
	// user				-	The pad number of the user making the call
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					CheckOnlineRetailStatus(uint32 user, CryLobbyTaskID* pTaskID, CryLobbyUIOnlineRetailStatusCallback cb, void* pCbArg) = 0;

	// ShowMessageList
	// Shows a list of the available messages in the local user's message/mail list
	// user				-	The pad number of the user making the call
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					ShowMessageList(uint32 user, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;

	virtual void							PostLocalizationChecks() = 0;

	// HandleOldNotSignedInInviteGame
	// PS3 only
	// can be called when a previous HandleInvite() call has returned eCLE_UserNotSignedIn after the game has
	// signed the user in and switched to multiplayer. The cached original invite data will be used to try and recover the pending invite
	// and properly HandleInvite() again
	virtual void							HandleOldNotSignedInInviteGame() = 0;

	// AddRecentPlayers
	// PS3 and PS4 require recently players be registered with system software, even if they are not friends
	// Typically should be called at the end of each game round.
	// user				-	The pad number of the user making the call
	// pUserIDs		- list of recent players
	// numUserIDs - number of recent players
	// gameMostStr - PS4 only: must supply a string for the type of game played - eg: DEATHMATCH.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError					AddRecentPlayers(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, const char* gameModeStr, CryLobbyTaskID* pTaskID, CryLobbyUICallback cb, void* pCbArg) = 0;						

	// </interfuscator:shuffle>
};

#endif // __ICRYLOBBYUI_H__
