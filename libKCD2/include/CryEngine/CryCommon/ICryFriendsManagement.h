#ifndef __ICRYFRIENDSMANAGEMENT_H__
#define __ICRYFRIENDSMANAGEMENT_H__

#pragma once

#include <ICryFriends.h> // <> required for Interfuscator

// Used to return IsFriend/IsUserBlocked results
struct SFriendManagementInfo 
{
	CryUserID	userID;
	bool			result;
};

// Used to pass in user to search for
struct SFriendManagementSearchInfo
{
	char			name[CRYLOBBY_USER_NAME_LENGTH];
};

#define CRYLOBBY_FRIEND_STATUS_STRING_SIZE		256
#define CRYLOBBY_FRIEND_LOCATION_STRING_SIZE	256

enum EFriendManagementStatus
{
	eFMS_Unknown,
	eFMS_Offline,
	eFMS_Online,
	eFMS_Playing,
	eFMS_Staging,
	eFMS_Chatting,
	eFMS_Away
};

struct SFriendStatusInfo
{
	CryUserID								userID;
	EFriendManagementStatus	status;
	char										statusString[ CRYLOBBY_FRIEND_STATUS_STRING_SIZE ];
	char										locationString[ CRYLOBBY_FRIEND_LOCATION_STRING_SIZE ];
};

// CryFriendsManagementCallback
// taskID				-	Task ID allocated when the function was executed.
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function.
// pArg					- Pointer to application-specified data.
typedef void (*CryFriendsManagementCallback)(CryLobbyTaskID taskID, ECryLobbyError error, void* pCbArg);

// CryFriendsManagementInfoCallback
// taskID				-	Task ID allocated when the function was executed.
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function.
// pArg					- Pointer to application-specified data.
typedef void (*CryFriendsManagementInfoCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SFriendManagementInfo* pInfo, uint32 numUserIDs, void* pCbArg);

// CryFriendsManagementSearchCallback
// taskID				-	Task ID allocated when the function was executed.
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function.
// pArg					- Pointer to application-specified data.
typedef void (*CryFriendsManagementSearchCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SFriendInfo* pInfo, uint32 numUserIDs, void* pCbArg);

// CryFriendsManagementStatusCallback
// taskID				-	Task ID allocated when the function was executed.
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function.
// pInfo				- Array of returned friend status info
// numUserIDs		- Number of elements in array
// pArg					- Pointer to application-specified data.
typedef void (*CryFriendsManagementStatusCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SFriendStatusInfo* pInfo, uint32 numUserIDs, void* pCbArg);

struct ICryFriendsManagement
{
	// <interfuscator:shuffle>
	virtual	~ICryFriendsManagement() {}

	// FriendsManagementAddFriend
	// Sends a friend request to the specified list of users
	// user				- The pad number of the user sending the friend requests
	// pUserIDs		- Pointer to an array of user ids to send friend requests to
	// numUserIDs	- The number of users to send friend requests to
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementSendFriendRequest(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementAcceptFriendRequest
	// Accepts friend requests from the specified list of users
	// user				- The pad number of the user accepting the friend requests
	// pUserIDs		- Pointer to an array of user ids to accept friend requests from
	// numUserIDs	- The number of users to accept friend requests from
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementAcceptFriendRequest(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementRejectFriendRequest
	// Rejects friend requests from the specified list of users
	// user				- The pad number of the user rejecting the friend requests
	// pUserIDs		- Pointer to an array of user ids to reject friend requests from
	// numUserIDs	- The number of users to reject friend requests from
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementRejectFriendRequest(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementRevokeFriendStatus
	// Revokes friendship status from the specified list of users
	// user				- The pad number of the user revoking friend status
	// pUserIDs		- Pointer to an array of user ids to revoke friend status from
	// numUserIDs	- The number of users to revoke friend status from
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementRevokeFriendStatus(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementIsUserFriend
	// Determines friendship status of the specified list of users
	// user				- The pad number of the user determining friend status
	// pUserIDs		- Pointer to an array of user ids to determine friend status for
	// numUserIDs	- The number of users to determine friend status for
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementIsUserFriend(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementInfoCallback pCb, void* pCbArg) = 0;

	// FriendsManagementFindUser
	// Attempt to find the users specified
	// user				- The pad number of the user attempting to find other users
	// pUserIDs		- Pointer to an array of user ids to attempt to find
	// numUserIDs	- The number of users to attempt to find
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementFindUser(uint32 user, SFriendManagementSearchInfo* pUserName, uint32 maxResults, CryLobbyTaskID* pTaskID, CryFriendsManagementSearchCallback pCb, void* pCbArg) = 0;

	// FriendsManagementBlockUser
	// Add the specified users to the blocked list
	// user				- The pad number of the user adding users to their block list
	// pUserIDs		- Pointer to an array of user ids to block
	// numUserIDs	- The number of users to block
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementBlockUser(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementUnblockUser
	// Remove the specified users from the blocked list
	// user				- The pad number of the user removing users from their block list
	// pUserIDs		- Pointer to an array of user ids to unblock
	// numUserIDs	- The number of users to unblock
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementUnblockUser(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementIsUserBlocked
	// Determines if the specified users are in the blocked list
	// user				- The pad number of the user determining blocked status
	// pUserIDs		- Pointer to an array of user ids to determine blocked status for
	// numUserIDs	- The number of users to determine blocked status for
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementIsUserBlocked(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementInfoCallback pCb, void* pCbArg) = 0;

	// FriendsManagementAcceptInvite
	// Accepts the invite from the specified user
	// user				- The pad number of the user accepting the invite
	// pUserID		- Pointer to the user id to accept the invite from
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementAcceptInvite(uint32 user, CryUserID* pUserID, CryLobbyTaskID* pTaskID, CryFriendsManagementCallback pCb, void* pCbArg) = 0;

	// FriendsManagementGetName
	// Gets the names of the specified list of user IDs
	// user				- The pad number of the user requesting names
	// pUserIDs		- Pointer to an array of user ids to get names for
	// numUserIDs	- The number of users to get names for
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementGetName(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementSearchCallback pCb, void* pCbArg) = 0;

	// FriendsManagementGetStatus
	// Gets the status of the specified list of user IDs
	// user				- The pad number of the user requesting names
	// pUserIDs		- Pointer to an array of user ids to get names for
	// numUserIDs	- The number of users to get names for
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual ECryLobbyError FriendsManagementGetStatus(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryFriendsManagementStatusCallback pCb, void* pCbArg) = 0;

	// CancelTask
	// Cancels a task
	virtual void CancelTask(CryLobbyTaskID) = 0;
	// </interfuscator:shuffle>
};

#endif //__ICRYFRIENDSMANAGEMENT_H__ 