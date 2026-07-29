//////////////////////////////////////////////////////////////////////
//
//  CryCommon Source Code
//
//  File: IStreamPersist.h
//  Description: IStreamPersist interface.
//
//  History:
//  - August 6, 2001: Created by Alberto Demichelis
//
//////////////////////////////////////////////////////////////////////

#ifndef GAME_ISTREAMPERSIST_H
#define GAME_ISTREAMPERSIST_H
#if _MSC_VER > 1000
# pragma once
#endif

struct IScriptObject;
class CStream;

enum DirtyFlags 
{
		DIRTY_NAME		= 0x1,
		DIRTY_POS			= 0x2,
		DIRTY_ANGLES	= 0x4,
};

//!store the stream status per connection(per serverslot)
/*struct StreamStatus
{
	StreamStatus()
	{
		nUserFlags=0;
		nLastUpdate=0;
		nUpdateNumber=0;
	}
	unsigned int nUserFlags;
	unsigned int nLastUpdate;
	unsigned int nUpdateNumber;
};*/


// Description:
//		This interface must be implemented by all objects that must be serialized
//		through the network or file.
//	Remarks: 
//		The main purpose of the serialization is reproduce the game remotely
//		or saving and restoring.This mean that the object must not save everything
//		but only what really need to be restored correctly.
struct IStreamPersist
{
	// <interfuscator:shuffle>
	// Description:
	//		Serializes the object to a bitstream(network)
	// Arguments:	
	//	stm - the stream class that will store the bitstream
	// Return Value:
	//		True if succeded,false failed
	// See also:
	//		CStream
	virtual bool Write(CStream&) = 0;
	// Description:
	//		Reads the object from a stream(network)
	// Arguments:
	//		stm - The stream class that store the bitstream.
	// Return Value:	
	//		True if succeeded,false failed.
	// See also:
	//		CStream
	virtual bool Read(CStream&) = 0;
	// Description:
	//		Checks if the object must be synchronized since the last serialization.
	// Return Value:
	//		True must be serialized, false the object didn't change.
	virtual bool IsDirty() = 0;
	// Description:
	//		Serializes the object to a bitstream(file persistence)
	// Arguments:
	//		stm - The stream class that will store the bitstream.
	// Return Value:
	//		True if succeeded,false failed
	// See also:
	//		CStream
	virtual bool Save(CStream &stm) = 0;
	// Description:
	//		Reads the object from a stream(file persistence).
	// Arguments:
	//	stm - The stream class that store the bitstream.
	//	pStream - script wrapper for the stream(optional).
	// Return Value:
	//		True if succeeded,false failed.
	// See also:
	//		CStream
	virtual bool Load(CStream &stm,IScriptObject *pStream=NULL) = 0;
	// </interfuscator:shuffle>
};

#endif // GAME_ISTREAMPERSIST_H
