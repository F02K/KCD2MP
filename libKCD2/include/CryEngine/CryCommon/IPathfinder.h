/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2009.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description:	Pathfinder Interface.

-------------------------------------------------------------------------
History:
- 15:1:2009   18:01 : Created by Márcio Martins
- 4 May 2009        : Evgeny Adamenkov: Removed IRenderer

*************************************************************************/

#ifndef __IPATHFINDER_H__
#define __IPATHFINDER_H__

#if _MSC_VER > 1000
#	pragma once
#endif

class CAIActor;

struct IAIPathFinderListerner;
struct IAIPathAgent;

#include <IMemory.h> // <> required for Interfuscator
#include <INavigationSystem.h>
#include "functor.h"

/* WARNING: These interfaces and structures are soon to be deprecated.
						Use at your own risk of having to change your code later!
*/


// Passing through navigational SO methods.
enum ENavSOMethod
{
	nSOmNone,							// not passing or not passable
	nSOmAction,						// execute an AI action
	nSOmPriorityAction,		// execute a higher priority AI action
	nSOmStraight,					// just pass straight
	nSOmSignalAnimation,	// play signal animation
	nSOmActionAnimation,	// play action animation
	nSOmLast
};

//====================================================================
// NavigationBlocker
// Represents an object that might be blocking a link. Each blocker is
// assumed to be spherical, with the position centred around the floor 
// so that links can intersect it.
//====================================================================
struct NavigationBlocker
{
	/// pos and radius define the sphere. 
	/// costAddMod is a fixed cost (in m) associated with the blocker obscuring a link - a value of 0 has
	/// no effect - a value of 10 would make the link effectively 10m longer than it is.
	/// costMultMod is the cost modification factor - a value of 0 has no effect - a value of 10 would make the link 
	/// 10x more costly. -ve disables the link
	/// radialDecay indicates if the cost modifiers should decay linearly to 0 over the radius of the sphere
	/// directional indicates if the cost should be unaffected for motion in a radial direction
	NavigationBlocker(const Vec3& pos, float radius, float costAddMod, float costMultMod, bool radialDecay, bool directional) 
		: sphere(pos, radius), costAddMod(costAddMod), costMultMod(costMultMod), restrictedLocation(false), 
		radialDecay(radialDecay), directional(directional) {}

	/// Just to allow std::vector::resize(0)
	NavigationBlocker() : sphere(Vec3(0, 0, 0), 0.0f), costAddMod(0), costMultMod(0), radialDecay(false) {assert("Should never get called");}

	Sphere sphere;
	bool radialDecay;
	bool directional;

	/// absolute cost added to any link going through this blocker (useful for small blockers)
	float costAddMod;
	/// multiplier for link costs going through this blocker (0 means no extra cost, 1 means to double etc)
	float costMultMod;

	// info to speed up the intersection checks
	/// If this is true then the blocker is small enough that it only affects the
	/// nav type it resides in. If false then it affects everything.
	bool restrictedLocation;
	IAISystem::ENavigationType navType;
	union Location
	{
		struct // similar for other nav types when there's info to go in them
		{
			// no node because the node "areas" can overlap so it's not useful
			int nBuildingID;
		} waypoint;
	};
	/// Only gets used if restrictedLocation = true
	Location location;
};

typedef DynArray<NavigationBlocker> NavigationBlockers;


//====================================================================
// PathPointDescriptor
//====================================================================
struct PathPointDescriptor
{
	struct SmartObjectNavData: public _i_reference_target_t
	{
		unsigned fromIndex;
		unsigned toIndex;

		// Callable only inside the AISystem module. It's implemented there.
		void Serialize(TSerialize ser);
	};
	typedef _smart_ptr< SmartObjectNavData > SmartObjectNavDataPtr;

	struct SmartObjectMNMData
	{
		SmartObjectMNMData()
			: meshID(0)
			, offMeshLinkID(0)
		{

		}

		uint32	meshID;
		uint32	offMeshLinkID;
	};

	PathPointDescriptor(IAISystem::ENavigationType navType = IAISystem::NAV_UNSET, const Vec3& vPos = ZERO) 
		: vPos(vPos), pSONavData(0), navType(navType), navSOMethod(nSOmNone), navTypeCustomId(0) {}

	PathPointDescriptor(const Vec3& vPos)
		: vPos(vPos), pSONavData(0), navType(IAISystem::NAV_UNSET), navSOMethod(nSOmNone), navTypeCustomId(0) {}

	// Callable only inside the AISystem module. It's implemented there.
	void Serialize(TSerialize ser);

	Vec3 vPos;
	IAISystem::ENavigationType navType;
	uint16 navTypeCustomId;
	
	SmartObjectNavDataPtr pSONavData;
	SmartObjectMNMData	soMNMData;

	ENavSOMethod	navSOMethod;
	bool IsEquivalent(const PathPointDescriptor& other) const
	{
		return (navType == other.navType) && vPos.IsEquivalent(other.vPos, 0.01f);
	}

	static bool ArePointsEquivalent(const PathPointDescriptor& point1, const PathPointDescriptor& point2)
	{
		return point1.IsEquivalent(point2);
	}
};

struct PathfindingExtraConstraint
{
	enum EExtraConstraintType
	{
		ECT_MAXCOST,
		ECT_MINDISTFROMPOINT,
		ECT_AVOIDSPHERE,
		ECT_AVOIDCAPSULE
	};

	EExtraConstraintType type;

	union UConstraint
	{
		struct SConstraintMaxCost
		{
			float maxCost;
		};
		struct SConstraintMinDistFromPoint
		{
			float px, py, pz; // Can't use Vec3 as it has a constructor.
			float minDistSq;
		};
		struct SConstraintAvoidSphere
		{
			float px, py, pz; // Can't use Vec3 as it has a constructor.
			float minDistSq;
		};
		struct SConstraintAvoidCapsule
		{
			float px, py, pz;
			float qx, qy, qz;
			float minDistSq;
		};
		SConstraintMaxCost maxCost;
		SConstraintMinDistFromPoint minDistFromPoint;
		SConstraintAvoidSphere avoidSphere;
		SConstraintAvoidCapsule avoidCapsule;
	};
	UConstraint constraint;
};

typedef DynArray<PathfindingExtraConstraint> PathfindingExtraConstraints;


struct PathfindRequest
{
	enum ERequestType
	{
		TYPE_ACTOR,
		TYPE_RAW,
	};
	ERequestType type;

	unsigned startIndex;
	unsigned endIndex;
	Vec3 startPos;
	Vec3 startDir;
	Vec3 endPos;
	/// endDir magnitude indicates the tendency to line up at the end of the path - 
	/// magnitude should be between 0 and 1
	Vec3 endDir;
	bool bSuccess;
	IAIPathAgent *pRequester;
	int nForceTargetBuildingId;
	bool allowDangerousDestination;
	float endTol;
	float endDistance;
	// as a result of RequestPathInDirection or RequestPathTo
	bool isDirectional;

	/// This gets set to false if the path end position doesn't match the requested end position
	/// (e.g. in the event of a partial path, or if the destination is in forbidden)
	bool bPathEndIsAsRequested;

	int id;
	IAISystem::tNavCapMask navCapMask;
	float passRadius;

	PathfindingExtraConstraints extraConstraints;

	PathfindRequest(ERequestType type)
		:	type(type),
		startIndex(0),
		endIndex(0),
		pRequester(0),
		bPathEndIsAsRequested(false),
		allowDangerousDestination(false),
		endTol(std::numeric_limits<float>::max()),
		endDistance(0),
		nForceTargetBuildingId(-1),
		isDirectional(false),
		id(-1),
		navCapMask(IAISystem::NAV_UNSET),
		passRadius(0.0f)
	{
	}

	// Callable only inside the AISystem module. It's implemented there.
	void Serialize(TSerialize ser);

	void GetMemoryUsage(ICrySizer *pSizer) const {/*LATER*/}
};


struct PathfindingHeuristicProperties
{
	PathfindingHeuristicProperties(const AgentPathfindingProperties& properties, const IAIPathAgent* pAgent = 0)
		: agentproperties(properties), pAgent(pAgent) {}

	PathfindingHeuristicProperties()
		: pAgent(0) {}

	AgentPathfindingProperties agentproperties;
	const IAIPathAgent *pAgent;
};


struct PathFollowerParams
{
	PathFollowerParams()
		: normalSpeed(0.0f),
		  pathRadius(0.0f),
			pathLookAheadDist(1.0f), 
			maxAccel(0.0f),
			maxDecel(1.0f),
			minSpeed(0.0f),
			maxSpeed(10.0f),
			endAccuracy(0.2f),
		  endDistance(0.0f),
			stopAtEnd(true),
			use2D(true),
			isVehicle(false),
			isAllowedToShortcut(true),
			navCapMask(IAISystem::NAV_UNSET),
			passRadius(0.5f)
	{}

	// OLD: Remove this when possible, Animation to take over majority of logic
	float normalSpeed; ///< normal entity speed
	float pathRadius; ///< max deviation allowed from the path
	float pathLookAheadDist; ///< how far we look ahead along the path - normally the same as pathRadius
	float maxAccel; ///< maximum acceleration of the entity
	float maxDecel; ///< maximum deceleration of the entity
	float minSpeed; ///< minimum output speed (unless it's zero on path end etc)
	float maxSpeed; ///< maximum output speed

	// KEEP: Additions and useful state for new impl.
	float endAccuracy; ///< How close to the end point the agent must be to finish pathing
	float endDistance; ///< stop this much before the end
	bool stopAtEnd; ///< aim to finish the path by reaching the end position (stationary), or simply overshoot
	bool use2D; ///< follow in 2 or 3D
	bool isVehicle;
	bool isAllowedToShortcut;

	// TODO: Add to serialize...
	/// The navigation capabilities of the agent
	IAISystem::tNavCapMask navCapMask;
	/// The minimum radius of the agent for navigating
	float passRadius;

	// Callable only inside the AISystem module. It's implemented there.
	void Serialize(TSerialize ser);
};


struct PathFollowResult
{
	struct SPredictedState {
		SPredictedState() {}
		SPredictedState(const Vec3 &p, const Vec3 &v) : pos(p), vel(v) {}
		Vec3 pos; Vec3 vel;
	};
	typedef DynArray<SPredictedState> TPredictedStates;
	
	// OLD: Obsolete & to be replaced by new impl.

	/// maximum time to predict out to - actual prediction may not go this far
	float desiredPredictionTime;
	/// the first element in predictedStates will be now + predictionDeltaTime, etc
	float predictionDeltaTime;
	/// if this is non-zero then on output the prediction will be placed into it
	TPredictedStates *predictedStates;

	PathFollowResult() 
	: predictionDeltaTime(0.1f), predictedStates(0), desiredPredictionTime(0), followTargetPos(0), inflectionPoint(0) {}

	
	bool reachedEnd;
	Vec3 velocityOut;

	// NEW: Replaces data above

	// NOTE: If the turningPoint and inflectionPoint are equal, they represent the end of the path.
	/// The furthest point on the path we can move directly towards without colliding with anything
	Vec3 followTargetPos;
	/// The next point on the path beyond the follow target that deviates substantially from a straight-line path
	Vec3 inflectionPoint;

	/// The maximum distance the agent can safely move in a straight line beyond the turning point
//	float maxOverrunDistance;
};



// Intermediary and minimal interface to use the pathfinder without requiring an AI object
// TODO: Fix the long function names. At the moment they collide with IAIObject base.
//			 The alternative would be to make IAIObject derive from IAIPathAgent.
struct IAIPathAgent
{
	// <interfuscator:shuffle>
	virtual ~IAIPathAgent(){}
	virtual IEntity *GetPathAgentEntity() const = 0;
	virtual const char *GetPathAgentName() const = 0;
	virtual unsigned short GetPathAgentType() const = 0;

	virtual float GetPathAgentPassRadius() const = 0;
	virtual Vec3 GetPathAgentPos() const = 0;
	virtual Vec3 GetPathAgentVelocity() const = 0;

	virtual const AgentMovementAbility &GetPathAgentMovementAbility() const = 0;
	virtual void PathEvent(SAIEVENT *pEvent) = 0;

	// This cannot easily be const, but has no side-effects
	virtual void GetPathAgentNavigationBlockers(NavigationBlockers &blockers, const PathfindRequest *pRequest) = 0;

	// TODO(marcio): Remove this from the interface
	// Most of it could be stored in the path request, except that it gets set at the start of the path request
	// and it's used everywhere in the AISystem.
	virtual unsigned int GetPathAgentLastNavNode() const = 0;
	virtual void SetPathAgentLastNavNode(unsigned int lastNavNode) = 0;

	virtual void SetPathToFollow( const char* pathName ) = 0;
	virtual void SetPathAttributeToFollow( bool bSpline ) = 0;

	//Path finding avoids blocker type by radius. 
	virtual void SetPFBlockerRadius(int blockerType, float radius) = 0;


	//Can path be modified to use request.targetPoint?  Results are cacheded in request.
	virtual ETriState CanTargetPointBeReached(CTargetPointRequest& request) = 0;

	//Is request still valid/use able
	virtual bool UseTargetPointRequest(const CTargetPointRequest& request) = 0;//??

	virtual bool GetValidPositionNearby(const Vec3& proposedPosition, Vec3& adjustedPosition) const = 0;
	virtual bool GetTeleportPosition(Vec3& teleportPos) const = 0;

	virtual class IPathFollower* GetPathFollower() const = 0;
	// </interfuscator:shuffle>
};


UNIQUE_IFACE class INavPath
{
public:
	// <interfuscator:shuffle>
	virtual ~INavPath(){}
	virtual void Release() = 0;
	virtual void CopyTo(INavPath *pRecipient) const = 0;
	virtual INavPath *Clone() const = 0;

	virtual NavigationMeshID GetMeshID() const = 0;

	virtual int GetVersion() const = 0;
	virtual void SetVersion(int version) = 0;

	virtual float	GetPathLength(bool twoD) const = 0;
	virtual void PushFront(const PathPointDescriptor &newPathPoint, bool force=false) = 0;
	virtual void PushBack(const PathPointDescriptor &newPathPoint, bool force=false) = 0;
	virtual void Clear(const char *contextName) = 0;
	virtual bool Advance(PathPointDescriptor &nextPathPoint) = 0;

	virtual bool GetPathEndIsAsRequested() const = 0;
	virtual void SetPathEndIsAsRequested(bool value) = 0;

	virtual bool Empty() const = 0;

	virtual const PathPointDescriptor	*GetLastPathPoint() const = 0;
	virtual const PathPointDescriptor *GetPrevPathPoint() const = 0;
	virtual const PathPointDescriptor *GetNextPathPoint() const = 0;
	virtual const PathPointDescriptor *GetNextNextPathPoint() const = 0;

	virtual const Vec3 &GetNextPathPos(const Vec3 &defaultPos = Vec3Constants<float>::fVec3_Zero) const = 0;
	virtual const Vec3 &GetLastPathPos(const Vec3 &defaultPos = Vec3Constants<float>::fVec3_Zero) const = 0;

	virtual bool GetPosAlongPath(Vec3 &posOut, float dist, bool twoD, bool extrapolateBeyondEnd, IAISystem::ENavigationType *nextPointType=NULL) const = 0;
	virtual float GetDistToPath(Vec3 &pathPosOut, float &distAlongPathOut, const Vec3 &pos, float dist, bool twoD) const = 0;
	virtual float GetDistToSmartObject(bool twoD) const = 0;
	virtual PathPointDescriptor::SmartObjectNavDataPtr GetLastPathPointAnimNavSOData() const = 0;
	virtual void SetPreviousPoint(const PathPointDescriptor &previousPoint) = 0;

	virtual AABB GetAABB(float dist) const = 0;

	virtual bool GetPathPropertiesAhead(float distAhead, bool twoD, Vec3 &posOut, Vec3 &dirOut, float *invROut, float &lowestPathDotOut, bool scaleOutputWithDist) const = 0;

	virtual void SetEndDir(const Vec3 &endDir) = 0;
	virtual const Vec3 &GetEndDir() const=0;
	
	virtual bool UpdateAndSteerAlongPath(Vec3& dirOut, float& distToEndOut, float &distToPathOut, bool& isResolvingSticking, Vec3 &pathDirOut, Vec3 &pathAheadDirOut, Vec3 &pathAheadPosOut, Vec3 currentPos, const Vec3& currentVel, float lookAhead, float pathRadius, float dt, bool resolveSticking, bool twoD) = 0;

	virtual bool AdjustPathAroundObstacles(const Vec3& currentpos, const AgentMovementAbility& movementAbility) = 0;

	virtual void TrimPath(float length, bool twoD) = 0;;
	virtual float GetDiscardedPathLength() const = 0;
	//virtual bool AdjustPathAroundObstacles(const CPathObstacles &obstacles, IAISystem::tNavCapMask navCapMask) = 0;
	//virtual void AddObjectAdjustedFor(const class CAIObject *pObject) = 0;
	//virtual const std::vector<const class CAIObject*> &GetObjectsAdjustedFor() const = 0;
	//virtual void ClearObjectsAdjustedFor() = 0;
	virtual float UpdatePathPosition(Vec3 agentPos, float pathLookahead, bool twoD, bool allowPathToFinish) = 0;
	virtual Vec3 CalculateTargetPos(Vec3 agentPos, float lookAhead, float minLookAheadAlongPath, float pathRadius, bool twoD) const = 0;
	virtual ETriState CanTargetPointBeReached(CTargetPointRequest &request, const CAIActor* pAIActor, bool twoD) const = 0;
	virtual bool UseTargetPointRequest(const CTargetPointRequest &request, CAIActor* pAIActor, bool twoD) = 0;

	virtual void Draw(const Vec3 &drawOffset = ZERO) const = 0;
	virtual void Dump(const char *name) const = 0;
	// </interfuscator:shuffle>
};


UNIQUE_IFACE class IPathFollower
{
public:
	// <interfuscator:shuffle>
	virtual ~IPathFollower(){}

	virtual void Release() = 0;

	virtual void Reset() = 0;

	/// This attaches us to a particular path (pass 0 to detach)
	virtual void AttachToPath(INavPath *navPath) = 0;

	virtual void SetParams(const PathFollowerParams &params) = 0;

	/// Just view the params
	virtual const PathFollowerParams &GetParams() const = 0;

	// Advances the follow target along the path as far as possible while ensuring the follow 
	// target remains reachable. Returns true if the follow target is reachable, false otherwise.
	virtual bool Update(PathFollowResult &result, const Vec3 &curPos, const Vec3 &curVel, float dt) = 0;

	/// Advances the current state in terms of position - effectively pretending that the follower
	/// has gone further than it has.
	virtual void Advance(float distance) = 0;

	/// Returns the distance from the lookahead to the end, plus the distance from the position passed in
	/// to the LA if pCurPos != 0
	virtual float GetDistToEnd(const Vec3 *pCurPos) const = 0;

	/// Returns the distance along the path from the current look-ahead position to the 
	/// first smart object path segment. If there's no path, or no smart objects on the 
	/// path, then std::numeric_limits<float>::max() will be returned
	virtual float GetDistToSmartObject() const = 0;
	virtual float GetDistToNavType(IAISystem::ENavigationType navType) const = 0;

	/// Returns a point on the path some distance ahead. actualDist is set according to 
	/// how far we looked - may be less than dist if we'd reach the end
	virtual Vec3 GetPathPointAhead(float dist, float &actualDist) const = 0;

	virtual void Draw(const Vec3& drawOffset = ZERO) const = 0;

	virtual void Serialize(TSerialize ser) = 0;

	// Checks ability to walk along a piecewise linear path starting from the current position 
	// (useful for example when animation would like to deviate from the path)
	virtual bool CheckWalkability( const Vec2* path, const size_t length ) const = 0;

	// Can the pathfollower cut corners if there is space to do so? (default: true)
	virtual bool GetAllowCuttingCorners() const = 0;

	// Sets whether or not the pathfollower is allowed to cut corners if there is space to do so. (default: true)
	virtual void SetAllowCuttingCorners(const bool allowCuttingCorners) = 0;
	// </interfuscator:shuffle>
};

typedef unsigned int MNMQueuedPathID;

enum EMNMDangers
{
	eMNMDangers_None            = 0,
	eMNMDangers_AttentionTarget = BIT(0),
	eMNMDangers_Explosive       = BIT(1),
	eMNMDangers_GroupMates      = BIT(2),
};

typedef uint32 MNMDangersFlags;

struct MNMPathRequest
{
	typedef Functor2<const MNMQueuedPathID&, SAIEVENT&> Callback;

	MNMPathRequest()
		: resultCallback(0)
		, startLocation(ZERO)
		, endLocation(ZERO)
		, endDirection(FORWARD_DIRECTION)
		, agentTypeID(NavigationAgentTypeID())
		, forceTargetBuildingId(0)
		, endTolerance(0.0f)
		, endDistance(0.0f)
		, allowDangerousDestination(false)
		, dangersToAvoidFlags(eMNMDangers_None)
	{

	}

	MNMPathRequest(const Vec3& start, const Vec3& end, const Vec3& _endDirection, int _forceTargetBuildingId, 
		float _endTolerance, float _endDistance, bool _allowDangerousDestination, const Callback& callback, 
		const NavigationAgentTypeID& _agentTypeID, const MNMDangersFlags dangersFlags = eMNMDangers_None)
		: resultCallback(callback)
		, startLocation(start)
		, endLocation(end)
		, endDirection(_endDirection)
		, agentTypeID(_agentTypeID)
		, forceTargetBuildingId(_forceTargetBuildingId)
		, endTolerance(_endTolerance)
		, endDistance(_endDistance)
		, allowDangerousDestination(false)
		, dangersToAvoidFlags(dangersFlags)
	{

	}

	Callback resultCallback;

	Vec3	startLocation;
	Vec3	endLocation;
	Vec3	endDirection;

	NavigationAgentTypeID agentTypeID;

	int		forceTargetBuildingId;
	float	endTolerance;
	float	endDistance;
	bool	allowDangerousDestination;
	MNMDangersFlags  dangersToAvoidFlags;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UNIQUE_IFACE struct IMNMPathfinder 
{
	// <interfuscator:shuffle>
	virtual ~IMNMPathfinder() {};

	virtual MNMQueuedPathID RequestPathTo(const IAIPathAgent* pRequester, const MNMPathRequest& request) = 0;
	virtual void CancelPathRequest(MNMQueuedPathID requestId) = 0;

	virtual const INavPath &GetNavPath() const = 0;

	// </interfuscator:shuffle>
};


UNIQUE_IFACE struct IAIPathFinder
{
public:
	// <interfuscator:shuffle>
	virtual ~IAIPathFinder(){}
	virtual void RequestPathTo(const Vec3 &start, const Vec3 &end, const Vec3 &endDir, IAIPathAgent *pRequester, bool allowDangerousDestination, int forceTargetBuildingId, float endTol, float endDistance) = 0;
	virtual void RequestPathTo(uint32 startIndex, uint32 endIndex, const Vec3 &endDir, IAIPathAgent *pRequester, bool allowDangerousDestination, int forceTargetBuildingId, float endTol, float endDistance) = 0;
	virtual int RequestRawPathTo(const Vec3 &start, const Vec3 &end, float passRadius, IAISystem::tNavCapMask navCapMask, unsigned& lastNavNode, bool allowDangerousDestination, float endTol, const PathfindingExtraConstraints &constraints, IAIPathAgent *pReference = 0) = 0;
	virtual void RequestPathInDirection(const Vec3 &start, const Vec3 &pos, float maxDist, IAIPathAgent *pRequester, float endDistance) = 0;

	virtual void CancelAnyPathsFor(IAIPathAgent* pRequester, bool actorRemoved = false) = 0;
	virtual void CancelCurrentRequest() = 0;

	virtual void RescheduleCurrentPathfindRequest() = 0;
	virtual bool IsFindingPathFor(const IAIPathAgent *pRequester) const = 0;

	virtual Vec3 GetBestPosition(const PathfindingHeuristicProperties &heuristic, float maxCost, const Vec3 &startPos, const Vec3 &endPos, unsigned startHintIndex, IAISystem::tNavCapMask navCapMask) = 0;

	virtual INavPath *CreateEmptyPath() const = 0;
	virtual IPathFollower *CreatePathFollower(const PathFollowerParams &params) const = 0;

	virtual const INavPath *GetCurrentPath() const = 0;
	virtual const PathfindRequest *GetPathfindCurrentRequest() const = 0;

	virtual void FlushPathQueue () = 0;
	virtual bool IsPathQueueEmpty () const = 0;

	virtual void Reset(IAISystem::EResetReason reason) = 0;

	virtual void RegisterPathFinderListener(IAIPathFinderListerner* pListener) = 0;
	virtual void UnregisterPathFinderListener(IAIPathFinderListerner* pListener) = 0;
	// </interfuscator:shuffle>
};


#endif //__IPATHFINDER_H__
