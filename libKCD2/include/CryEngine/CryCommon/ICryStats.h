#ifndef __ICRYSTATS_H__
#define __ICRYSTATS_H__

#pragma once

#include <ICryLobby.h> // <> required for Interfuscator
#include <ICryMatchMaking.h> // <> required for Interfuscator

typedef	uint32	CryStatsLeaderBoardID;

// The score column for a leader board entry. 
// In Live the score id is the property id from the xlast program given to the score column when defining a leaderboard.
struct SCryStatsLeaderBoardScore
{
	SCryLobbyUserData								score;
	CryLobbyUserDataID							id;
};

// The user defined columns for a leaderboard.
// In Live a leaderboard can have up to 64 user defined columns.
// The CryLobbyUserDataID inside data is the property id from the xlast program given to the column when defining a leaderboard.
// columnID is the id output by the xlast program when defining a leaderboard.
struct SCryStatsLeaderBoardUserColumn
{
	SCryLobbyUserData								data;
	CryLobbyUserDataID							columnID;
};

// A leaderboard row contains a score column and 0 or more custom columns
struct SCryStatsLeaderBoardData
{
	SCryStatsLeaderBoardScore				score;
	SCryStatsLeaderBoardUserColumn*	pColumns;
	uint32													numColumns;
};

// In Live the leaderboard id is output by the xlast program when defining a leaderboard.
struct SCryStatsLeaderBoardWrite
{
	SCryStatsLeaderBoardData				data;
	CryStatsLeaderBoardID						id;
};

struct SCryStatsLeaderBoardReadRow
{
	SCryStatsLeaderBoardData				data;
	uint32													rank;
	CryUserID												userID;
	char														name[CRYLOBBY_USER_NAME_LENGTH];
};

struct SCryStatsLeaderBoardReadResult
{
	CryStatsLeaderBoardID						id;
	SCryStatsLeaderBoardReadRow*		pRows;
	uint32													numRows;
	uint32													totalNumBoardRows;
};

// CryStatsCallback
// taskID		-	Task ID allocated when the function was executed
// error		- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// pArg			- Pointer to application-specified data
typedef void (*CryStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, void* pArg);

// CryStatsReadLeaderBoardCallback
// taskID		-	Task ID allocated when the function was executed
// error		- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// pResult	- If error is eCLE_Success a pointer to a SCryStatsLeaderBoardReadResult which contains the information read from the leaderboard.
// pArg			- Pointer to application-specified data
typedef void (*CryStatsReadLeaderBoardCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCryStatsLeaderBoardReadResult* pResult, void* pArg);

// CryStatsReadUserDataCallback
// taskID		-	Task ID allocated when the function was executed
// error		- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// pData		- Pointer to an array of SCryLobbyUserData that will match the data registered and contain the last data written.
// numData	- The number of SCryLobbyUserData returned.
// pArg			- Pointer to application-specified data
typedef void (*CryStatsReadUserDataCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCryLobbyUserData* pData, uint32 numData, void* pArg);

struct ICryStats
{
	// <interfuscator:shuffle>
	virtual ~ICryStats(){}
	// StatsRegisterLeaderBoards
	// This function must be called before any other leaderboard functions.
	// It defines the applications custom data used for it's leaderboards.
	// pBoards		- Pointer to an array of SCryStatsLeaderBoardWrite that defines the applications leaderboards
	// numBoards	- Number of leaderboards to register
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback
	// cb					- Callback function that is called when function completes
	// pCbArg			- Pointer to application-specified data that is passed to the callback
	// return			- eCLE_Success if function successfully started or an error code if function failed to start
	virtual	ECryLobbyError	StatsRegisterLeaderBoards(SCryStatsLeaderBoardWrite* pBoards, uint32 numBoards, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;

	// StatsWriteLeaderBoards
	// Write one or more leaderboard entries for the given user.
	// In Live this call must be made between SessionStart and SessionEnd.
	// session		- The session the user is in and the stats are for.
	// user				- The pad number of the local user the stats are being written for.
	// pBoards		- Pointer to an array of leaderboard entires to be written.
	// numBoards	- Number of leaderboard entries to be written.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual	ECryLobbyError	StatsWriteLeaderBoards(CrySessionHandle session, uint32 user, SCryStatsLeaderBoardWrite* pBoards, uint32 numBoards, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;

	// StatsWriteLeaderBoards
	// Write one or more leaderboard entries for one or more users.
	// In Live this call must be made between SessionStart and SessionEnd.
	// session		- The session the users are in and the stats are for.
	// pUserIDs		- Pointer to an array of CryUserID of the users the stats are for.
	// ppBoards		- Pointer to an array of arrays of leaderboard entries to be written. One array to be written for each user.
	// pNumBoards	- The number of leaderboard entries to be written for each user.
	// numUserIDs	- The number of users being written.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual	ECryLobbyError	StatsWriteLeaderBoards(CrySessionHandle session, CryUserID* pUserIDs, SCryStatsLeaderBoardWrite** ppBoards, uint32* pNumBoards, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;

	// StatsReadLeaderBoardByRankForRange
	// Retrieves a list of entires in the order of their ranking within a leaderboard, starting with a specified rank value.
	// board			- The leaderboard to read from.
	// startRank	- The rank to start retrieving from.
	// num				- The number of entires to retrieve.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsReadLeaderBoardByRankForRange(CryStatsLeaderBoardID board, uint32 startRank, uint32 num, CryLobbyTaskID* pTaskID, CryStatsReadLeaderBoardCallback cb, void* pCbArg) = 0;

	// StatsReadLeaderBoardByRankForUser
	// Retrieves a list of entires in the order of their ranking within a leaderboard, with the given local user appearing in the middle.
	// board			- The leaderboard to read from.
	// user				- The pad number of the local user who will appear in the middle of the list.
	// num				- The number of entires to retrieve.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsReadLeaderBoardByRankForUser(CryStatsLeaderBoardID board, uint32 user, uint32 num, CryLobbyTaskID* pTaskID, CryStatsReadLeaderBoardCallback cb, void* pCbArg) = 0;

	// StatsReadLeaderBoardByUserID
	// Retrieves a list of entires for a given list of users in the order of their ranking within a leaderboard.
	// board			- The leaderboard to read from.
	// pUserIDs		- Pointer to an array of CryUserID for the users to read.
	// numUserIDs	- Number of users to read.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsReadLeaderBoardByUserID(CryStatsLeaderBoardID board, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryStatsReadLeaderBoardCallback cb, void* pCbArg) = 0;

	// StatsRegisterUserData
	// This function must be called before any other user data functions.
	// It defines the applications custom data used for it's users.
	// In Live a maximum of 3000 bytes of custom user data can be stored.
	// pData			- Pointer to an array of SCryLobbyUserData that defines the user data the application wants to store for each user.
	// numData		- Number of items to store.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsRegisterUserData(SCryLobbyUserData* pData, uint32 numData, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;

	// StatsWriteUserData
	// Write the user data for a local user.
	// user				- The pad number of the local user to have their data written.
	// pData			- Pointer to an array of SCryLobbyUserData that defines the user data to write.
	// numData		- Number of data items to write.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsWriteUserData(uint32 user, SCryLobbyUserData* pData, uint32 numData, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;

	// StatsWriteUserData
	// Write the user data for an array of users.
	// pUserIDs		- The user IDs to have their data written.
	// ppData			- Ragged 2D array of SCryLobbyUserData that defines the user data to write.
	// pNumData		- Array of lengths of rows in ppData.
	// numUserIDs	- The number of user IDs.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsWriteUserData(CryUserID* pUserIDs, SCryLobbyUserData** ppData, uint32* pNumData, uint32 numUserIDs, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;

	// StatsReadUserData
	// Read the user data for a local user.
	// user				- The pad number of the local user to read data for.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsReadUserData(uint32 user, CryLobbyTaskID* pTaskID, CryStatsReadUserDataCallback cb, void* pCbArg) = 0;

	// StatsReadUserData
	// Read the user data for a given CryUserID.
	// user				- The pad number of the local user who is doing the read.
	// userID			- The CryUserID of the user to read data for.
	// pTaskID		- Pointer to buffer to store the task ID to identify this call in the callback.
	// cb					- Callback function that is called when function completes.
	// pCbArg			- Pointer to application-specified data that is passed to the callback.
	// return			- eCLE_Success if function successfully started or an error code if function failed to start.
	virtual ECryLobbyError	StatsReadUserData(uint32 user, CryUserID userID, CryLobbyTaskID* pTaskID, CryStatsReadUserDataCallback cb, void* pCbArg) = 0;

	// CancelTask
	// Cancel the given task. The task will still be running in the background but the callback will not be called when it finishes.
	// taskID			- The task to cancel
	virtual void CancelTask(CryLobbyTaskID taskID) = 0;

	// SetLeaderboardType
	// Set the leaderboard type.
	// leaderboardType	- the leaderboard type
	virtual void SetLeaderboardType( ECryLobbyLeaderboardType leaderboardType ) = 0;

	// GetLeaderboardType
	// Get the leaderboard type.
	virtual ECryLobbyLeaderboardType GetLeaderboardType() = 0;

	// Access the profanity filter (only on PS3 at the moment - for Xbox profanity filter, see PlatformOS_Xenon).
	virtual ECryLobbyError StatsVerifyString(uint32 user, uint8* pUTF8Buffer, uint32 nSizeofBuffer, CryLobbyTaskID* pTaskID, CryStatsCallback cb, void* pCbArg) = 0;
	// </interfuscator:shuffle>
};

#endif // __ICRYSTATS_H__
