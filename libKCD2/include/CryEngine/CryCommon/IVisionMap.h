#pragma once

#ifndef __IVisionMap_h__
#define __IVisionMap_h__

// This should be somewhere in the game code
// Can only be from [0..31]
enum EObservableType
{
	eTypeStimulus = 0,
	eTypeActor,
};


enum EChangeHint
{
	eChangedPosition    = 1<<0,
	eChangedOrientation = 1<<1,
	eChangedSight       = 1<<2,
	eChangedPriority    = 1<<3,
	eChangedFaction     = 1<<4,
	eChangedCallback    = 1<<5,
	eChangedUserData    = 1<<6,
	eChangedSkipList    = 1<<7,
	eChangedEntityId    = 1<<8,
	eChangedCamouflage  = 1<<9,
	eChangedTypeMask    = 1<<10,
	eChangedAll         = 0xffffffff,
};


enum EVisionPriority
{
	eLowPriority			= 0,
	eMediumPriority		= 1,
	eHighPriority			= 2,
	eVeryHighPriority = 3,
};


struct VisionID
{
	VisionID(): m_id(0) {};

	operator uint32() const {	return m_id;	};

	friend class CVisionMap;

private:
	uint32 m_id;

#ifdef _RELEASE
	VisionID(uint32 id, const char* name): m_id(id) {};
#else
	VisionID(uint32 id, const char* name): m_id(id), m_debugName(name) {};
	string m_debugName;
#endif
};

struct ObservableParams;

struct ObserverParams
{
	ObserverParams()
		: callback(0)
		, userData(0)
		, priority(eLowPriority)
		, factionMask(0xffffffff)
		, typeMask(0xffffffff)
		, sightRange(50.0f)
		, primaryFoVCos(cry_cosf(gf_PI/4.0f))
		, peripheralFoVCos(cry_cosf(gf_PI/2.0f))
		, eyePos(ZERO)
		, eyeDir(ZERO)
		, skipListSize(0)
		, entityID(0)
		, orientationUpdateTreshold(0.05f)
	{
	}

	enum
	{
		MaxSkipListSize = 32,
	};

	typedef Functor5<const VisionID&, const ObserverParams&, const VisionID&, const ObservableParams&, bool> Callback;
	Callback callback;
	void* userData;

	uint32 factionMask;
	uint32 typeMask;

	float sightRange;
	float primaryFoVCos;
	float peripheralFoVCos;
	float orientationUpdateTreshold;

	Vec3 eyeDir;
	Vec3 eyePos;

	IPhysicalEntity *skipList[MaxSkipListSize];
	EntityId entityID;

	EVisionPriority priority : 3;
	uint8 skipListSize : 5;
};


struct ObservableParams
{
	enum
	{
		MaxPositionCount = 6,
	};

	enum
	{
		MaxSkipListSize = 32,
	};

	ObservableParams()
		: callback(0)
		, userData(0)
		, entityID(0)
		, typeMask(0)
		, faction(0)
		, camouflage(0.0f)
		, posCount(0)
		, skipListSize(0)
	{
		memset(&pos[0], 0, sizeof(pos));
	}

	typedef Functor5<const VisionID&, const ObserverParams&, const VisionID&, const ObservableParams&, bool> Callback;
	Callback callback;
	void* userData;

	EntityId entityID;
	uint32 typeMask;
	uint8 faction;

	Vec3 pos[MaxPositionCount];
	uint8 posCount;

	uint8 skipListSize;
	IPhysicalEntity *skipList[MaxSkipListSize];

	float camouflage;
};

typedef VisionID ObserverID;
typedef VisionID ObservableID;

class IVisionMap
{
public:
	// <interfuscator:shuffle>
	virtual ~IVisionMap() {}

	virtual void Reset() = 0;
	virtual VisionID CreateVisionID(const char* name) = 0;

	virtual void RegisterObserver(const ObserverID& observerID, const ObserverParams& params) = 0;
	virtual void UnregisterObserver(const ObserverID& observerID) = 0;

	virtual void RegisterObservable(const ObservableID& observableID, const ObservableParams& params) = 0;
	virtual void UnregisterObservable(const ObservableID& observableID) = 0;

	virtual void ObserverChanged(const ObserverID& observerID, const ObserverParams& params, uint32 hint) = 0;
	virtual	void ObservableChanged(const ObservableID& observableID, const ObservableParams& params, uint32 hint) = 0;

	virtual bool IsVisible(const ObserverID& observerID, const ObservableID& observableID) const = 0;
	virtual const ObserverParams* GetObserverParams(const ObserverID& observerID) const = 0;
	virtual const ObservableParams* GetObservableParams(const ObservableID& observableID) const = 0;

	virtual void Update(float frameTime) = 0;
	// </interfuscator:shuffle>
};

#endif // __IVisionMap_h__
