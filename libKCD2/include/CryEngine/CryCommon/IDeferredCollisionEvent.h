////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2010.
// -------------------------------------------------------------------------
//  File name:   IDeferredCollisionEvent.h
//  Version:     v1.00
//  Created:     12/08/2010 by Christopher Bolte
//  Compilers:   Visual Studio.NET
// -------------------------------------------------------------------------
//  History:
////////////////////////////////////////////////////////////////////////////
#ifndef IDEFERREDCOLLISIONINTERFACE_H
#define IDEFERREDCOLLISIONINTERFACE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IThreadTask.h>

struct EventPhys;

// Base class for all deferred physics events
// Basically this class works like a future,
// Start() start the computation(some in the main thread, major part in a task/spu)
// Result() will sync the task operation and return the result
struct IDeferredPhysicsEvent : 
	public IThreadTask
{
	// enum list of all types of deferred events
	enum DeferredEventType {
		PhysCallBack_OnCollision
	};

	IDeferredPhysicsEvent(){}
	// <interfuscator:shuffle>
	virtual ~IDeferredPhysicsEvent(){}

	// == "future" like interface == //

	// start the execution of the event
	virtual void Start() =0;

	// sync the event and do all necessary post-processing, then return the result
	virtual int Result(EventPhys* pOrigEvent = 0) =0;	

	// just wait for the event to finish
	virtual void Sync() = 0;

	// check if the async computation part has finished
	virtual bool HasFinished() = 0;

	// Get the concrete Type of this deferred event
	virtual DeferredEventType GetType() const =0;		

	// returns a ptr to the original physics event
	virtual  EventPhys* PhysicsEvent() =0;
	// </interfuscator:shuffle>
};


// Manager class  for deferred physics events
struct IDeferredPhysicsEventManager
{
	// type of create function used to create needed deferred events in the HandleEvent function
	typedef IDeferredPhysicsEvent*(*CreateEventFunc)(const EventPhys *pEvent);

	IDeferredPhysicsEventManager(){}
	// <interfuscator:shuffle>
	virtual ~IDeferredPhysicsEventManager(){}

	// dispatch an deferred event to the task thread
	virtual void DispatchDeferredEvent( IDeferredPhysicsEvent *pEvent ) =0;

	// Encapsulates common logic for deferred events, should be called from the physics callbacks
	// handles the cvar management as well as deferred event creating
	virtual int HandleEvent( const EventPhys *pEvent, IDeferredPhysicsEventManager::CreateEventFunc, IDeferredPhysicsEvent::DeferredEventType) =0;

	// Register and Unregister Deferred events in the manager to allow 
	virtual void RegisterDeferredEvent(IDeferredPhysicsEvent *pDeferredEvent ) =0;
	virtual void UnRegisterDeferredEvent(IDeferredPhysicsEvent *pDeferredEvent ) =0;

	// Delete all Deferred Events in flight, use only when also clearing the physics event queue
	// or else this call results in dangling points, mostly used for save/load
	virtual void ClearDeferredEvents() =0;

	virtual void Update() = 0;

	virtual IDeferredPhysicsEvent* GetLastCollisionEventForEntity(IPhysicalEntity *pPhysEnt) = 0;
	// </interfuscator:shuffle>
};


#endif // IDEFERREDCOLLISIONINTERFACE_H