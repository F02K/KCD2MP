/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2010.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: Interface to use serialization helpers
  
 -------------------------------------------------------------------------
  History:
  - 02:06:2010: Created by Kevin Kirst

*************************************************************************/

#ifndef __ISERIALIZEHELPER_H__
#define __ISERIALIZEHELPER_H__

struct ISerializedObject
{
	// <interfuscator:shuffle>
	virtual ~ISerializedObject() {}
	virtual uint32 GetGUID() const = 0;
	virtual void GetMemoryUsage(ICrySizer *pSizer) const = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;

	// IsEmpty
	//	Returns True if object contains no serialized data
	virtual bool IsEmpty() const = 0;

	// Reset
	//	Resets the serialized object to an initial (empty) state
	virtual void Reset() = 0;

	// Serialize
	//	Call to inject the serialized data into another TSerialize object
	// Arguments:
	//	TSerialize &serialize - The TSerialize object to use
	virtual void Serialize(TSerialize &serialize) = 0;
	// </interfuscator:shuffle>
};

struct ISerializeHelper
{
	typedef bool (*TSerializeFunc)(TSerialize serialize, void *pArgument);

	// <interfuscator:shuffle>
	virtual ~ISerializeHelper() {}
	virtual void GetMemoryUsage(ICrySizer *pSizer) const = 0;

	virtual void AddRef() = 0;
	virtual void Release() = 0;

	// CreateSerializedObject
	//	Returns an ISerializedObject to be used with this helper
	// Arguments:
	//	const char* szSection - Name of the serialized object's section
	virtual _smart_ptr<ISerializedObject> CreateSerializedObject(const char* szSection) = 0;

	// Write
	//	Begins the writing process using the supplied functor
	// Arguments:
	//	ISerializedObject *pObject - Serialization object to write with
	//	TSerializeFunc serializeFunc - Functor called to supply the serialization logic
	//	void *pArgument - Optional argument passed in to functor.
	// Returns:
	//	True if writing occurred with given serialization object
	virtual bool Write(ISerializedObject *pObject, TSerializeFunc serializeFunc, void *pArgument = NULL) = 0;

	// Read
	//	Begins the reading process using the supplied functor
	// Arguments:
	//	ISerializedObject *pObject - Serialization object to read from
	//	TSerializeFunc serializeFunc - Functor called to supply the serialization logic
	//	void *pArgument - Optional argument passed in to functor.
	// Returns:
	//	True if writing occurred with given serialization object
	virtual bool Read(ISerializedObject *pObject, TSerializeFunc serializeFunc, void *pArgument = NULL) = 0;
	// </interfuscator:shuffle>
};

#endif //__ISERIALIZEHELPER_H__
