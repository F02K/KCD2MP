////////////////////////////////////////////////////////////////////////////
//
//  CryEngine Source File.
//  Copyright (C), Crytek, 2011.
// -------------------------------------------------------------------------
//  File name:   ICrySignIn.h
//  Version:     v1.00
//  Created:     09/11/2011 by Paul Mikell.
//  Description: ICrySignIn interface definition
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __ICRYSIGNIN_H__

#define __ICRYSIGNIN_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "CryArray.h"
#include "CryString.h"
#include <ICryLobby.h> // <> required for Interfuscator

// User credential identifiers used by GetUserCredentials task
enum ECryLobbyUserCredential
{
	eCLUC_Email,
	eCLUC_Password,
	eCLUC_UserName,
	eCLUC_Invalid
};

struct SUserCredential
{
	SUserCredential()
	: type( eCLUC_Invalid ),
	  value()
	{
	}

	SUserCredential( const SUserCredential& src )
	: type( src.type ),
		value( src.value )
	{
	}

	SUserCredential( ECryLobbyUserCredential srcType, const string& srcValue )
	: type( srcType ),
		value( srcValue )
	{
	}

	~SUserCredential()
	{
	}

	const SUserCredential& operator=( const SUserCredential& src )
	{
		type = src.type;
		value = src.value;

		return *this;
	}

	ECryLobbyUserCredential	type;
	string									value;
};

typedef DynArray< SUserCredential > TUserCredentials;

// CrySignInCallback
// taskID				-	Task ID allocated when the function was executed.
// error				- Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function.
// user         - local user index
// pArg					- Pointer to application-specified data.
typedef void ( *CrySignInCallback )( CryLobbyTaskID taskID, ECryLobbyError error, uint32 user, void* pCbArg );

// CrySignInCreateUserCallback
// taskID			  -	Task ID allocated when the function was executed
// error			  - Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// user         - local user index
// pName        - If email and password are correct for existing account, the user name for that account
// pArg				  - Pointer to application-specified data
typedef void ( *CrySignInCreateUserCallback )( CryLobbyTaskID taskID, ECryLobbyError error, uint32 user, const char* pName, void* pArg );

// CrySignInGetUserCredentialsCallback
// taskID			  -	Task ID allocated when the function was executed
// error			  - Error code - eCLE_Success if the function succeeded or an error that occurred while processing the function
// user         - local user index
// pCredentials - Pointer to array of user credentials
// pArg				  - Pointer to application-specified data
typedef void ( *CrySignInGetUserCredentialsCallback )( CryLobbyTaskID taskID, ECryLobbyError error, uint32 user, DynArray< SUserCredential >* pCredentials, void* pArg );

struct ICrySignIn
{
	// <interfuscator:shuffle>
	virtual ~ICrySignIn() {}

	// UserSignIn
	// Sign a user in.
	// user				  - local user index
	// credentials  - credentials of user
	// pTaskID		  - lobby task ID
	// pCB				  - callback
	// pCBArg			  - callback argument
	// return			  - eCLE_Success if task started successfully, otherwise a failure code
	virtual ECryLobbyError SignInUser( uint32 user, const TUserCredentials& credentials, CryLobbyTaskID* pTaskID, CrySignInCallback pCb, void* pCbArg ) = 0;

	// UserSignOut
	// Sign a user out.
	// user				  - local user index
	// pTaskID		  - lobby task ID
	// pCB				  - callback
	// pCBArg			  - callback argument
	// return			  - eCLE_Success if task started successfully, otherwise a failure code
	virtual ECryLobbyError SignOutUser( uint32 user, CryLobbyTaskID* pTaskID, CrySignInCallback pCb, void* pCbArg ) = 0;

	// CreateUser
	// Create a new user.
	// user				  - local user index
	// credentials  - credentials of new user
	// pTaskID		  - lobby task ID
	// pCB				  - callback
	// pCBArg			  - callback argument
	// return			  - eCLE_Success if task started successfully, otherwise a failure code
	virtual ECryLobbyError CreateUser( uint32 user, const TUserCredentials& credentials, CryLobbyTaskID* pTaskID, CrySignInCreateUserCallback pCb, void* pCbArg ) = 0;

	// CreateAndSignInUser
	// Create and sign in a new user.
	// user				  - local user index
	// credentials  - credentials of new user
	// pTaskID		  - lobby task ID
	// pCB				  - callback
	// pCBArg			  - callback argument
	// return			  - eCLE_Success if task started successfully, otherwise a failure code
	virtual ECryLobbyError CreateAndSignInUser( uint32 user, const TUserCredentials& credentials, CryLobbyTaskID* pTaskID, CrySignInCreateUserCallback pCb, void* pCbArg ) = 0;

	// GetUserCredentials
	// Get the given user's credentials.
	// user				  - local user index
	// pTaskID		  - lobby task ID
	// pCB				  - callback
	// pCBArg			  - callback argument
	// return			  - eCLE_Success if task started successfully, otherwise a failure code
	virtual	ECryLobbyError GetUserCredentials( uint32 user, CryLobbyTaskID* pTaskID, CrySignInGetUserCredentialsCallback pCB, void* pCBArg ) = 0;

	// CancelTask
	// Cancel a task
	// lTaskID		   - lobby task ID
	virtual void CancelTask( CryLobbyTaskID lTaskID ) = 0;
	// </interfuscator:shuffle>
};

#endif //__ICRYSIGNIN_H__
