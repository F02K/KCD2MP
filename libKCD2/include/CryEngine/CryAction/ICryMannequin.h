////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2011.
//
////////////////////////////////////////////////////////////////////////////
#ifndef __I_CRY_MANNEQUIN_H__
#define __I_CRY_MANNEQUIN_H__

#include <ICryAnimation.h>
#include <CryExtension/ICryUnknown.h>
#include "ICryMannequinDefs.h"
#include "BitFiddling.h"

#ifdef CRYACTION_EXPORTS
#define CRYMANNEQUIN_API DLL_EXPORT
#else
#define CRYMANNEQUIN_API DLL_IMPORT
#endif

struct IMannequinEditorManager;
struct IMannequinGameListener;
class CAnimationDatabase;
class CTagDefinition;
struct AnimEventInstance;

template<typename T>
class TagStateBase
{
public:
	typedef T value_type;

	ILINE TagStateBase() {}
	ILINE explicit TagStateBase(const T t) : m_bits(t) {}

	ILINE const value_type& GetBits() const { return m_bits; }

	ILINE bool operator==(const TagStateBase<T> &t) const { return m_bits == t.m_bits; }
	ILINE bool operator!=(const TagStateBase<T> &t) const { return m_bits != t.m_bits; }

protected:
	T m_bits;

private:
	// The following bit operations are private and should only be accessed by
	// the friend class CTagDefinition

	ILINE const TagStateBase<T>& operator|=(const TagStateBase<T>& t) { m_bits |= t.m_bits; return *this; }
	ILINE TagStateBase<T> operator|(const TagStateBase<T>& t) const { return TagStateBase<T>(m_bits | t.m_bits); }

	ILINE const TagStateBase<T>&  operator&=(const TagStateBase<T>& t) { m_bits &= t.m_bits; return *this; }
	ILINE TagStateBase<T> operator&(const TagStateBase<T>& t) const { return TagStateBase<T>(m_bits & t.m_bits); }

	ILINE const TagStateBase<T>&  operator<<=(uint32 bitPos) { m_bits <<= bitPos; return *this; }
	ILINE TagStateBase<T> operator<<(uint32 bitPos) const { return TagStateBase<T>(m_bits << bitPos); }

	ILINE const TagStateBase<T>&  operator>>=(uint32 bitPos) { m_bits >>= bitPos; return *this; }
	ILINE TagStateBase<T> operator>>(uint32 bitPos) const { return TagStateBase<T>(m_bits >> bitPos); }

	ILINE const TagStateBase<T>& operator^=(const TagStateBase<T> t) { m_bits ^= t.m_bits; return *this; }
	ILINE TagStateBase<T> operator^(const TagStateBase<T>& t) const { return TagStateBase<T>(m_bits ^ t.m_bits); }

	ILINE TagStateBase<T> operator~() const { return TagStateBase<T>(~m_bits); }

	ILINE void SetBit(uint32 bitPos) { m_bits = T(1) << bitPos; }

	friend class CTagDefinition;
	friend class CAnimationDatabaseManager;
};


typedef TagStateBase<uint64> TagState;
static const TagState TAG_STATE_FULL(~TagState::value_type(0));
static const TagState TAG_STATE_EMPTY(0);

const uint32 MANN_ERROR_BUFFER_SIZE = 1024;


struct SAnimationEntry
{
	SAnimationEntry()
		:
		flags(0),
		playbackSpeed(1.0f),
		playbackWeight(1.0f),
		weightList(0)
	{
	}

	ILINE bool IsEmpty() const
	{
		return animRef.IsEmpty();
	}

	friend bool operator == (const SAnimationEntry& entryA, const SAnimationEntry& entryB)
	{
		return (entryA.animRef.crc == entryB.animRef.crc) 
							&& (entryA.flags == entryB.flags)
							&& (entryA.playbackSpeed == entryB.playbackSpeed)
							&& (entryA.playbackWeight == entryB.playbackWeight)
							&& (entryA.weightList == entryB.weightList);
	}

	SAnimRef animRef;
	uint32 flags;
	float  playbackSpeed;
	float  playbackWeight;
	uint8	 weightList;
};


struct SProceduralEntry
{
	enum EConfig{ MAX_PARAMS = 28};

	SProceduralEntry() : type(0)
	{
		memset(parameters, 0, sizeof(parameters[0]) * MAX_PARAMS);
	}

	friend bool operator == (const SProceduralEntry& entryA, const SProceduralEntry& entryB)
	{
		for(int p=0; p<MAX_PARAMS; p++)
		{
			if(entryA.parameters[p] != entryB.parameters[p])
			{
				return false;
			}
		}
		
		return (entryA.type == entryB.type)
							&& (entryA.animRef.crc == entryB.animRef.crc) 
							&& (entryA.dataString == entryB.dataString)
							&& (entryA.dataCRC.crc == entryB.dataCRC.crc);
	}

	uint32 type;
	SAnimRef animRef;
	SProcDataCRC dataCRC;
	TProcClipString dataString;
	float parameters[MAX_PARAMS];

	// FIXME: Currently this string is used during the game at run-time so we cannot
	//   conditionally remove it.
	//   I propose we store the CryClassID in the type variable instead of a CRC.
	//   Then the name can still be retrieved at run-time using
	//   ICryFactoryRegistry::GetFactory(classID)->GetClassName().
	//   We might still want to include the name for the editor though,
	//   because loading/saving a proc-clip that refers to an extension you don't
	//   have should not remove it (imho).
	CCryName typeString;
};

struct SMannParameter
{
	uint32 crc;
	QuatT  value;

	SMannParameter()
		:
		crc(0),
		value(IDENTITY)
	{
	}
};

typedef DynArray<SMannParameter> TMannParamList;


//---------------------------------------------------------------------------------------------------------
// SAnimBlend - Specific blend information for a clip on a layer
//---------------------------------------------------------------------------------------------------------
// ExitTime				- Time to start the following clip relative to the previous one
//                   -ve values mean to align to the end of the previous clip
// StartTime			- Time in secs for the following clip to start off advanced to
// Duration				- Duration of the blend in secs
// flags					- Specific playback flags for timeAlignment etc.
// terminal				- Has this clip been explicitly setup to blend into the next fragment?
//---------------------------------------------------------------------------------------------------------
struct SAnimBlend
{
	SAnimBlend(float _exitTime = 0.0f, float _startTime = 0.0f, float _duration = 0.2f, uint32 _flags = 0)
		:
		exitTime(_exitTime),
		startTime(_startTime),
		duration(_duration),
		flags(_flags),
		terminal(false)
	{
	}

	friend bool operator == (const SAnimBlend& entryA, const SAnimBlend& entryB)
	{
		return (entryA.exitTime == entryB.exitTime) 
			&& (entryA.startTime == entryB.startTime)
			&& (entryA.duration == entryB.duration)
			&& (entryA.flags == entryB.flags)
			&& (entryA.terminal == entryB.terminal);
	} 

	float exitTime;			
	float startTime;		
	float duration;			
	uint32 flags;				
	bool	 terminal;		
};

enum EClipType
{
	eCT_Normal,
	eCT_Transition,
	eCT_TransitionOutro,
};

//---------------------------------------------------------------------------------------------------------
// SAnimClip - Single animation clip used by the sequencer
//---------------------------------------------------------------------------------------------------------
// blend					- Blend data to use for this clip
// animation			- Pure animation data
// blendPart			- The fragment part which the blend came from
// part						- The fragment part which the animation data came from
//---------------------------------------------------------------------------------------------------------
struct SAnimClip
{
	SAnimClip()
		:
		part(0),
		blendPart(0),
		referenceLength(0.0f)
	{
	}

	friend bool operator == (const SAnimClip& entryA, const SAnimClip& entryB)
	{
		return (entryA.blend == entryB.blend) 
			&& (entryA.animation == entryB.animation)
			&& (entryA.blendPart == entryB.blendPart)
			&& (entryA.part == entryB.part);
	} 

	SAnimBlend			blend;				
	SAnimationEntry	animation;		
	float						referenceLength;
	uint8						blendPart;		
	uint8						part;			
};
typedef DynArray<SAnimClip> TAnimClipSequence;

struct SProcClip
{
	SProcClip()
		:
		part(0),
		blendPart(0)
	{
	}

	friend bool operator == (const SProcClip& entryA, const SProcClip& entryB)
	{
		return (entryA.blend == entryB.blend) 
			&& (entryA.procedural == entryB.procedural)
			&& (entryA.blendPart == entryB.blendPart)
			&& (entryA.part == entryB.part);
	}

	SAnimBlend				blend;
	SProceduralEntry	procedural;
	uint8							blendPart;
	uint8							part;
};
typedef DynArray<SProcClip> TProcClipSequence;

struct SFragmentData
{
	enum
	{
		PART_FRAGMENT,
		PART_TRANSITION1,
		PART_TRANSITION2,
		PART_TOTAL
	};

	SFragmentData()
		:
		isOneShot(false)
	{
		for (uint32 p=0; p<PART_TOTAL; p++)
		{
			duration[p] = 0.0f;
			transitionType[p] = eCT_Normal;
		}
	}

	DynArray<TAnimClipSequence> animLayers;
	DynArray<TProcClipSequence> procLayers;
	float			duration[PART_TOTAL];
	EClipType transitionType[PART_TOTAL];
	bool			isOneShot;
};

class CFragment
{
public:

	DynArray<TAnimClipSequence>	m_animLayers;
	DynArray<TProcClipSequence>	m_procLayers;
};

//---------------------------------------------------------------------------------------------------------
// SFragmentBlend - High level blend information
//---------------------------------------------------------------------------------------------------------
// SelectTime					- Time value used for selecting the best transition
// StartTime					- Time to begin the transition relative to the end of the previous fragment
// EnterTime					- Time delta to apply to the following fragment, allows snipping of the start of the entire fragment
// pFragment					- Transition fragment: includes additional tween clips and blends into the intial fragment clips.
//										  This is merged onto the front of the following fragment on installation.
//---------------------------------------------------------------------------------------------------------
struct SFragmentBlend
{
	enum EFlags
	{
		Cyclic					= BIT(0),
		ExitTransition	= BIT(1)
	};

	SFragmentBlend()
		:
		selectTime(0.0f),
		startTime(0.0f),
		enterTime(0.0f),
		pFragment(NULL),
		flags(0)
	{
	}

	bool operator <(const SFragmentBlend &blend) const
	{
		return selectTime < blend.selectTime;
	}

	ILINE const bool IsExitTransition() const
	{
		return (flags & ExitTransition) != 0;
	}

	float			 selectTime;
	float			 startTime;
	float			 enterTime;
	CFragment *pFragment;
	uint8			 flags;
};

class CTagDefinition
{
public:

	struct SPriorityCount
	{
		int priority;
		int count;
	};

	CTagDefinition()
		: m_numBits(0)
	{
	}

	struct STagGroup
	{
		STagGroup(const char *szGroupName)
		{
			m_name.SetByString(szGroupName);
		}

		TagState m_mask;
		STagRef m_name;
	};

	struct STag
	{
		STag(const char *szTag, uint32 priority, int groupID)
		{
			m_mask = TAG_STATE_EMPTY;
			m_name.SetByString(szTag);
			m_priority = priority;
			m_groupID = groupID;
			m_pTagDefinition = NULL;
		}

		TagState m_mask;
		uint32	 m_priority;
		TagGroupID	 m_groupID;
		const CTagDefinition *m_pTagDefinition;
		STagRef m_name;
	};
	
	ILINE bool IsValidTagID(TagID tagID) const
	{
		return (tagID >= 0) && (tagID < (TagID)m_tags.size());
	}

	ILINE bool IsValidTagGroupID(TagGroupID groupID) const
	{
		return (groupID >= 0) && (groupID < (TagGroupID)m_tagGroups.size());
	}

	TagID GetNum() const
	{
		return (TagID)m_tags.size();
	}

	uint32 GetPriority(TagID tagID) const
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return 0;

		return m_tags[tagID].m_priority;
	}

	void SetPriority(TagID tagID, uint32 priority)
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return;

		m_tags[tagID].m_priority = priority;
	}

	void Clear()
	{
		m_tags.clear();
		m_tagGroups.clear();
	}

	const CTagDefinition *GetSubTagDefinition(TagID tagID) const
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return NULL;

		return m_tags[tagID].m_pTagDefinition;
	}

	void SetSubTagDefinition(TagID tagID, const CTagDefinition *pTagDef)
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (IsValidTagID(tagID))
		{
			m_tags[tagID].m_pTagDefinition = pTagDef;
		}
	}

	TagID AddTag(const char *szTag, const char *szGroup = NULL, uint32 priority = 0)
	{
		const bool tagExistsAlready = (Find(szTag) != TAG_ID_INVALID);
		if (tagExistsAlready)
			return TAG_ID_INVALID;

		TagGroupID groupID = GROUP_ID_NONE;
		if (szGroup)
			groupID = AddGroup(szGroup);

		m_tags.push_back(STag(szTag, priority, groupID));

		return (TagID)m_tags.size()-1;
	}

	void RemoveTag(TagID tagID)
	{
		m_tags.erase(tagID);
	}

	TagGroupID AddGroup(const char *szGroup)
	{
		TagGroupID groupID = FindGroup(szGroup);

		if (groupID == GROUP_ID_NONE)
		{
			groupID = m_tagGroups.size();
			m_tagGroups.push_back(STagGroup(szGroup));
		}

		return groupID;
	}

	void RemoveGroup(TagGroupID groupID)
	{
		m_tagGroups.erase(groupID);

		// Adjust GroupID reference in the tags
		DynArray<STag>::iterator itEnd = m_tags.end();
		for (DynArray<STag>::iterator it = m_tags.begin(); it != itEnd; ++it)
		{
			STag &tag = *it;
			if (tag.m_groupID == groupID)
				tag.m_groupID = GROUP_ID_NONE;
			else if (tag.m_groupID > groupID)
				tag.m_groupID--;
		}
	}

	void AssignBits()
	{
		const TagID numTags = (TagID)m_tags.size();
		const TagGroupID numGroups = (TagGroupID)m_tagGroups.size();

		uint32 curBit = 0;

		for (TagGroupID g=0; g<numGroups; g++)
		{
			uint32 numTagsInGroup = 0;
			STagGroup &group = m_tagGroups[g];
			for (TagID i=0; i<numTags; i++)
			{
				STag &tag = m_tags[i];
				if (tag.m_groupID == g)
				{
					numTagsInGroup++;
					tag.m_mask = TagState(numTagsInGroup) << curBit;
				}
			}

			uint32 numBits = 32 - countLeadingZeros32(numTagsInGroup);
			group.m_mask = TAG_STATE_EMPTY;
			for (uint32 i=0; i<numBits; i++)
			{
				TagState bitMask;
				bitMask.SetBit(i+curBit);
				group.m_mask |= bitMask;
			}
			curBit += numBits;
		}

		for (TagID i=0; i<numTags; i++)
		{
			STag &tag = m_tags[i];
			if (tag.m_groupID == GROUP_ID_NONE)
			{
				tag.m_mask.SetBit(curBit);
				curBit++;
			}
		}

		m_numBits = curBit;

		CalculatePriorityTallies();
	}

	TagGroupID GetGroupID(TagID tagID) const
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return GROUP_ID_NONE;

		return m_tags[tagID].m_groupID;
	}

	TagGroupID GetNumGroups() const
	{
		return (TagGroupID)m_tagGroups.size();
	}

#if STORE_TAG_STRINGS

	const char *GetGroupName(TagGroupID groupID) const
	{
		CRY_ASSERT(IsValidTagGroupID(groupID));
		if (!IsValidTagGroupID(groupID))
			return "<invalid>";

		return m_tagGroups[groupID].m_name.GetString();
	}

	void SetGroupName(TagGroupID groupID, const char *szGroup)
	{
		CRY_ASSERT(IsValidTagGroupID(groupID));
		if (!IsValidTagGroupID(groupID))
			return;

		m_tagGroups[groupID].m_name.SetByString(szGroup);
	}

#endif //STORE_TAG_STRINGS

	TagID Find(const char *szTag) const
	{
		if (szTag)
		{
			uint32 crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(szTag);

			return Find(crc);
		}
		else
		{
			return TAG_ID_INVALID;
		}
	}

	TagID Find(uint32 crc) const
	{
		// TODO: Use hash/map
		TagID numTags = (TagID)m_tags.size();
		for (TagID i=0; i<numTags; i++)
		{
			if (m_tags[i].m_name.crc == crc)
			{
				return i;
			}
		}

		return TAG_ID_INVALID;
	}

	TagGroupID FindGroup(const char *szTag) const
	{
		uint32 crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(szTag);

		return FindGroup(crc);
	}

	TagGroupID FindGroup(uint32 crc) const
	{
		// TODO: Use hash/map
		TagGroupID numGroups = (TagGroupID)m_tagGroups.size();
		for (TagGroupID i=0; i<numGroups; i++)
		{
			if (m_tagGroups[i].m_name.crc == crc)
			{
				return i;
			}
		}

		return GROUP_ID_NONE;
	}

	void SetFilename(const char *filename)
	{
		m_filename = filename;
	}
	const char *GetFilename() const
	{
		return m_filename.c_str();
	}

	uint32 GetTagCRC(TagID tagID) const 
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return 0;

		return m_tags[tagID].m_name.crc;
	}

#if STORE_TAG_STRINGS
	const char *GetTagName(TagID tagID) const
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return "<invalid>";

		return m_tags[tagID].m_name.GetString();
	}

	void SetTagName(TagID tagID, const char *szTag)
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return;

		m_tags[tagID].m_name.SetByString(szTag);
	}
#endif //STORE_TAG_STRINGS

	void SetTagGroup(TagID tagID, TagGroupID groupID)
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return;

		m_tags[tagID].m_groupID = groupID;
	}

	bool TagListToFlags(const char *tagList, TagState &tagState) const
	{
		bool success = true;
		tagState = TAG_STATE_EMPTY;

		if (tagList && (tagList[0] != '\0'))
		{
			const char *cur = tagList;
			const char *nxt = strstr(cur, "+");
			char tagBuffer[128];
			while (nxt)
			{
				strncpy(tagBuffer, cur, nxt-cur);
				tagBuffer[nxt-cur] = '\0';
				int tag = Find(tagBuffer);
				if (tag < 0)
				{
					CryWarning(VALIDATOR_MODULE_ANIMATION, VALIDATOR_ERROR, "[TagListToFlags] Invalid tag '%s'", tagBuffer);
					success = false;
				}
				else
				{
					tagState |= m_tags[tag].m_mask;
				}

				cur = nxt+1;
				nxt = strstr(nxt+1, "+");
			}

			strcpy(tagBuffer, cur);
			TagID tag = Find(tagBuffer);
			if (tag == TAG_ID_INVALID)
			{
				CryWarning(VALIDATOR_MODULE_ANIMATION, VALIDATOR_ERROR, "[TagListToFlags] Invalid tag '%s'", tagBuffer);
				success = false;
			}
			else
			{
				tagState |= m_tags[tag].m_mask;
			}
		}

		return success;
	}
	
	bool IsGroupSet(const TagState &state, const TagGroupID &groupID) const
	{
		CRY_ASSERT(IsValidTagGroupID(groupID));
		if (!IsValidTagGroupID(groupID))
			return false;
		
		const STagGroup &group = m_tagGroups[groupID];

		return (state & group.m_mask) != TAG_STATE_EMPTY;
	}

	void SetGroup(TagState &state, const TagGroupID groupID, const TagID tagID) const
	{
		CRY_ASSERT(IsValidTagGroupID(groupID));
		if (!IsValidTagGroupID(groupID))
			return;

		const TagState groupMask = m_tagGroups[groupID].m_mask;
		state &= ~groupMask;

		if (IsValidTagID(tagID))
		{
			const STag &tag = m_tags[tagID];
			CRY_ASSERT(tag.m_groupID == groupID);
			if (tag.m_groupID != groupID)
				return;

			state |= tag.m_mask;
		}
	}

	void ClearGroup(TagState &state, const TagGroupID groupID) const
	{
		CRY_ASSERT(IsValidTagGroupID(groupID));
		if (!IsValidTagGroupID(groupID))
			return;

		const STagGroup &group = m_tagGroups[groupID];
		state &= ~group.m_mask;
	}

	bool IsSet(const TagState &state, const TagID tagID) const
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return false;

		return IsSetInternal(state, tagID);
	}

	void Set(TagState &state, const TagID tagID, bool set) const
	{
		CRY_ASSERT(IsValidTagID(tagID));
		if (!IsValidTagID(tagID))
			return;

		const STag &tag = m_tags[tagID];

		if (tag.m_groupID != GROUP_ID_NONE)
		{
			const STagGroup &group = m_tagGroups[tag.m_groupID];

			const bool isSet = ((group.m_mask & state) == tag.m_mask);

			if (isSet != set)
			{
				const TagState mask = group.m_mask;
				state &= ~mask;

				if (set)
				{
					state |= tag.m_mask;
				}
			}
		}
		else
		{
			if (set)
			{
				state |= tag.m_mask;
			}
			else
			{
				state &= ~tag.m_mask;
			}
		}
	}

	//-----------------------------------------------------------------
	// Returns a tagState which has set bits for all the tags 
	// that are also set in the passed in tagDef
	//-----------------------------------------------------------------
	TagState GetSharedTags(const CTagDefinition &tagDef) const
	{
		const TagGroupID numGroups = (TagGroupID)m_tagGroups.size();
		TagState ret = TAG_STATE_EMPTY;
		for (TagGroupID i=0; i<numGroups; i++)
		{
			if (tagDef.FindGroup(m_tagGroups[i].m_name.crc) != GROUP_ID_NONE)
			{
				ret |= m_tagGroups[i].m_mask;
			}
		}

		const uint32 numTags = GetNum();
		for (uint32 i=0; i<numTags; i++)
		{
			if ((m_tags[i].m_groupID == GROUP_ID_NONE) && (tagDef.Find(m_tags[i].m_name.crc) != TAG_ID_INVALID))
			{
				ret |= m_tags[i].m_mask;
			}
		}

		return ret;
	}

	// Returns the union of the tags in a and b.
  // When a and b contain different tags within the same tag group,
  // b gets precedence.
	TagState GetUnion(const TagState& a, const TagState& b) const
	{
		TagState ret = a | b;

		const TagGroupID numGroups = (TagGroupID)m_tagGroups.size();
		for (TagGroupID i=0; i<numGroups; i++)
		{
			const TagState &groupMask = m_tagGroups[i].m_mask;
			const TagState groupA = groupMask & a;
			const TagState groupB = groupMask & b;
			if ((groupA != TAG_STATE_EMPTY) && (groupB != TAG_STATE_EMPTY))
			{
				if (groupA != groupB)
				{
					// pick B (both in case of conflict and when a is simply 0)
					ret = (ret & ~groupMask) | groupB;
				}
			}
		}

		return ret;
	}
	
	TagState GetIntersection(const TagState& a, const TagState& b) const
	{
		TagState ret = a & b;

		const TagGroupID numGroups = (TagGroupID)m_tagGroups.size();
		for (TagGroupID i=0; i<numGroups; i++)
		{
			const TagState &groupMask = m_tagGroups[i].m_mask;
			if ((groupMask & a) != (groupMask & b))
				ret &= ~groupMask;
		}

		return ret;
	}

	// Returns a - b (all tags set in a which are not set in b)
	//
	// To clear a specific tag group in a, include a tag of that group in b.
	// (so if both a and b contain tags in the same group, the tag in a
	// is cleared whether or not the tag in b is the same as the one in a)
	TagState GetDifference(const TagState& a, const TagState& b) const
	{
		TagState ret = a & ~b;

		const TagGroupID numGroups = (TagGroupID)m_tagGroups.size();
		for (TagGroupID i=0; i<numGroups; i++)
		{
			const TagState &groupMask = m_tagGroups[i].m_mask;
			if ((groupMask & b) != TAG_STATE_EMPTY)
				ret &= ~groupMask;
		}

		return ret;
	}

	
	ILINE TagState GenerateMask(const TagState &tagState) const 
	{
		typedef TagState::value_type T;
		T ret = (T)tagState.GetBits();
		const TagGroupID numGroups = (TagGroupID)m_tagGroups.size();
		for (TagGroupID i=0; i<numGroups; i++)
		{
			const T groupMask = m_tagGroups[i].m_mask.GetBits();
			ret |= iselnz((T)groupMask & tagState.GetBits(), (T)groupMask, (T)0);
		}
		return TagState(ret);
	}

	ILINE bool Contains(const TagState &compParent, const TagState &compChild, const TagState &comparisonMask) const
	{
		return ((compParent & comparisonMask) == compChild);
	}

	ILINE bool Contains(const TagState &compParent, const TagState &compChild) const
	{
		if (((compParent & compChild) != compChild))
		{
			return false;
		}
		else
		{
			TagState comparisonMask = GenerateMask(compChild);

			return ((compParent & comparisonMask) == compChild);
		}
	}

	uint32 RateTagState(const TagState &tagState, const DynArray<SPriorityCount> *pPriorityTallies = NULL) const
	{
		if (pPriorityTallies == NULL)
		{
			pPriorityTallies = &m_priorityTallies;
		}
		const uint32 numPriorities = pPriorityTallies->size();

		uint32 score = 0;
		const uint32 numTags = GetNum();
		for (uint32 i=0; i<numTags; i++)
		{
			if (IsSetInternal(tagState, i))
			{
				int priority = m_tags[i].m_priority;
				uint32 priorityTally = 1;
				for (uint32 p=0; (p<numPriorities); p++)
				{
					const SPriorityCount &priorityCount = m_priorityTallies[p];

					if (priority > priorityCount.priority)
					{
						uint32 newTally = priorityTally * (priorityCount.count+1);
						CRY_ASSERT_MESSAGE(newTally >= priorityTally, "TagState rating overflow - too many distinct priority levels!");
						priorityTally = newTally;
					}
					else
					{
						break;
					}
				}

				score+=priorityTally;
			}
		}

		return score;
	}

	void FlagsToTagList(const TagState &tagState, char *tagList, uint32 length) const
	{
		const TagID numTags = GetNum();
		bool isFirst = true;
		CryStackStringT<char, 512> retBuffer;
		for (TagID i=0; i<numTags; i++)
		{
			if (IsSetInternal(tagState, i))
			{
				const char *tagName = GetTagName(i);
				if (!isFirst)
				{
					retBuffer += "+";
				}
				isFirst = false;
				retBuffer += tagName;
			}
		}
		retBuffer += "\0";

		strncpy(tagList, retBuffer, min(length, (uint32)retBuffer.length()+1));
	}

	uint32 GetNumBits() const
	{
		return m_numBits;
	}

	bool HasTooManyBits() const
	{
		return m_numBits > sizeof(TagState)*8;
	}

	void CombinePriorities(const CTagDefinition &tagDef, DynArray<SPriorityCount> &combinedPriorities) const
	{
		const uint32 numPriorities1 = m_priorityTallies.size();
		const uint32 numPriorities2 = tagDef.m_priorityTallies.size();

		std::set<int> combPrioritySet;

		for (uint32 i=0; i<numPriorities1; i++)
		{
			combPrioritySet.insert(m_priorityTallies[i].priority);
		}
		for (uint32 i=0; i<numPriorities2; i++)
		{
			combPrioritySet.insert(tagDef.m_priorityTallies[i].priority);
		}

		const uint32 numCombinedPriorities = combPrioritySet.size();
		combinedPriorities.resize(numCombinedPriorities);
		std::set<int>::iterator iter = combPrioritySet.begin();
		for (uint32 i=0; i<numCombinedPriorities; i++, iter++)
		{
			const int priority = *iter;
			SPriorityCount &priorityCount = combinedPriorities[i];

			priorityCount.priority = priority;
			priorityCount.count = 0;

			for (uint32 p=0; p<numPriorities1; p++)
			{
				if (m_priorityTallies[p].priority == priority)
				{
					priorityCount.count += m_priorityTallies[p].count;
					break;
				}
			}
			for (uint32 p=0; p<numPriorities2; p++)
			{
				if (tagDef.m_priorityTallies[p].priority == priority)
				{
					priorityCount.count += tagDef.m_priorityTallies[p].count;
					break;
				}
			}
		}
	}

private:

	bool IsSetInternal(const TagState state, const TagID tagID) const
	{
		const STag &tag = m_tags[tagID];
		TagState mask = tag.m_mask;
		if (tag.m_groupID != GROUP_ID_NONE)
		{
			mask = m_tagGroups[tag.m_groupID].m_mask;
		}

		return ((state & mask) == tag.m_mask);
	}

	void CalculatePriorityTallies()
	{
		std::set<int> priorities;
		const int numTags = m_tags.size();
		for (TagID tagID=0; tagID<numTags; tagID++)
		{
			const STag &tag = m_tags[tagID];
			priorities.insert(tag.m_priority);
		}

		const uint32 numPriorities = priorities.size();
		m_priorityTallies.resize(numPriorities);
		std::set<int>::iterator iter = priorities.begin();
		for (uint32 i=0; i<numPriorities; i++, iter++)
		{
			const int priority = *iter;
			SPriorityCount &priorityCount = m_priorityTallies[i];

			priorityCount.priority = priority;
			priorityCount.count = 0;

			for (TagID tagID=0; tagID<numTags; tagID++)
			{
				const STag &tag = m_tags[tagID];
				if (tag.m_priority == priority)
				{
					priorityCount.count++;
				}
			}
		}
	}

	string							m_filename;
	DynArray<STag>			m_tags;
	DynArray<STagGroup>	m_tagGroups;
	uint32							m_numBits;
	DynArray<SPriorityCount> m_priorityTallies;
};

class CTagState
{
public:
	CTagState(const CTagDefinition &defs, TagState state = TAG_STATE_EMPTY)
		:
		m_defs(defs),
		m_state(state)
	{
#ifndef _RELEASE
		const size_t maxSupportedBits = sizeof(TagState) * 8;
		const size_t definitionBits = defs.GetNumBits();
		if (maxSupportedBits < definitionBits)
		{
			CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "!Number of bits required for tag definition '%s' (%" PRISIZE_T " bits) is greater than %" PRISIZE_T " bits. To fix this, group mutually exclusive tags together, remove unnecessary tags or increase the size of TagState.", defs.GetFilename(), definitionBits, maxSupportedBits);
		}
#endif
	}

	void Clear()
	{
		m_state = TAG_STATE_EMPTY;
	}

	void SetByCRC(uint32 CRC, bool enable)
	{
		const TagID id = m_defs.Find(CRC);
		if (m_defs.IsValidTagID(id))
		{
			Set(id, enable);
		}
	}

	void Set(TagID id, bool enable)
	{
		m_defs.Set(m_state, id, enable);
	}

	void SetGroup(TagGroupID groupID, TagID tagID)
	{
		m_defs.SetGroup(m_state, groupID, tagID);
	}

	void ClearGroup(TagGroupID groupID)
	{
		m_defs.ClearGroup(m_state, groupID);
	}

	ILINE bool IsSet(TagID tagID) const
	{
		return m_defs.IsSet(m_state, tagID);
	}

	ILINE bool IsGroupSet(TagGroupID tagGroupID) const
	{
		return m_defs.IsGroupSet(m_state, tagGroupID);
	}

	ILINE bool Contains(const TagState &comp) const
	{
		return m_defs.Contains(m_state, comp);
	}
	ILINE bool Contains(const CTagState &comp) const
	{
		return m_defs.Contains(m_state, comp.m_state);
	}

	const TagState &GetMask() const
	{
		return m_state;
	}

	const CTagDefinition &GetDef() const
	{
		return m_defs;
	}

	CTagState &operator =(const TagState &tagState)
	{
		m_state = tagState;
		return *this;
	}

private:

	const CTagDefinition &m_defs;
	TagState m_state;

};

struct SFragTagState
{
	explicit SFragTagState(const TagState &_globalTags = TAG_STATE_EMPTY, const TagState &_fragmentTags = TAG_STATE_EMPTY)
		: 
		globalTags(_globalTags),
		fragmentTags(_fragmentTags)
	{
	}

	bool operator ==(const SFragTagState &fragTagState) const
	{
		return (globalTags == fragTagState.globalTags) && (fragmentTags == fragTagState.fragmentTags);
	}
	bool operator !=(const SFragTagState &fragTagState) const
	{
		return (globalTags != fragTagState.globalTags) || (fragmentTags != fragTagState.fragmentTags);
	}

	TagState globalTags;
	TagState fragmentTags;
};

template<typename T>
class TTagSortedList
{
public:

	friend class CAnimationDatabaseManager;
	friend class CAnimationDatabase;

	struct SSortStruct
	{
		uint32 priority;
		uint16 originalIdx;

		bool operator<(const SSortStruct &compare) const
		{
			return priority > compare.priority;
		}
	};

	TTagSortedList<T>()
	{
	}

	~TTagSortedList<T>()
	{
	}

	T &Insert(const SFragTagState &fragTags, const T &data)
	{
		const uint32 numEntries = m_keys.size();
		for (uint32 i=0; i<numEntries; i++)
		{
			if (m_keys[i] == fragTags)
			{
				m_values[i] = data;
				return m_values[i];
			}
		}

		Resize(numEntries+1);
		m_keys[numEntries]		= fragTags;
		m_values[numEntries] = data;
		return m_values[numEntries];
	}

	void Erase(uint32 idx)
	{
		const uint32 numEntries = m_keys.size();
		CRY_ASSERT(idx < numEntries);

		if (idx < numEntries)
		{
			m_keys.erase(m_keys.begin()+idx);
			m_values.erase(m_values.begin()+idx);
		}
	}

	void Resize(const uint32 newSize)
	{
		if (newSize != m_keys.size())
		{
			m_keys.resize(newSize);
			m_values.resize(newSize);
		}
	}

	uint32 Size() const
	{
		return m_keys.size();
	}

	const T *Get(uint32 idx) const
	{
		const uint32 numEntries = m_keys.size();
		CRY_ASSERT(idx < numEntries);

		if (idx < numEntries)
		{
			return &m_values[idx];
		}

		return NULL;
	}

	int FindIdx(const SFragTagState &fragTags) const
	{
		const uint32 numEntries = m_keys.size();
		for (uint32 i=0; i<numEntries; i++)
		{
			if (m_keys[i] == fragTags)
			{
				return i;
			}
		}

		return -1;
	}

	const T *Find(const SFragTagState &fragTags) const
	{
		int idx = FindIdx(fragTags);

		if (idx >= 0)
		{
			return &m_values[idx];
		}

		return NULL;
	}

	T *Find(const SFragTagState &fragTags)
	{
		int idx = FindIdx(fragTags);

		if (idx >= 0)
		{
			return &m_values[idx];
		}

		return NULL;
	}

	void Sort(const CTagDefinition &tagDefs, const CTagDefinition *pFragTagDefs)
	{
		DynArray<CTagDefinition::SPriorityCount> *pCombinedPriorities = NULL;

		if (pFragTagDefs)
		{
			pCombinedPriorities = new DynArray<CTagDefinition::SPriorityCount>();
			tagDefs.CombinePriorities(*pFragTagDefs, *pCombinedPriorities);
		}

		const uint32 size = m_keys.size();
		SSortStruct *pSortKeys = new SSortStruct[size];
		for (uint32 i=0; i<size; i++)
		{
			pSortKeys[i].priority			= tagDefs.RateTagState(m_keys[i].globalTags, pCombinedPriorities);
			pSortKeys[i].originalIdx  = i;
		}
		if (pFragTagDefs)
		{
			for (uint32 i=0; i<size; i++)
			{
				pSortKeys[i].priority += pFragTagDefs->RateTagState(m_keys[i].fragmentTags, pCombinedPriorities);
			}
		}

		delete(pCombinedPriorities);

		std::stable_sort( pSortKeys, pSortKeys+size);

		for (uint32 i=0; i<size; i++)
		{
			const uint32 originalIdx = pSortKeys[i].originalIdx;
			if (originalIdx != i)
			{
				SFragTagState bufferedTS = m_keys[i];
				T bufferedValue = m_values[i];
				m_keys[i]   = m_keys[originalIdx];
				m_values[i] = m_values[originalIdx];
				m_keys[originalIdx]   = bufferedTS;
				m_values[originalIdx] = bufferedValue;

				for (uint32 k=i+1; k<size; k++)
				{
					if (pSortKeys[k].originalIdx == i)
					{
						pSortKeys[k].originalIdx = originalIdx;
						break;
					}
				}
			}
		}

		delete [] pSortKeys;
	}


	const T &GetDefault() const
	{
		const uint32 size = m_keys.size();
		if (size == 0)
		{
			CryFatalError("[TTagSortedList] No default in list, this should not happen");
		}

		return m_values[size-1];
	}

	const T *GetBestMatch(const SFragTagState &fragTags, const CTagDefinition *pGlobalTagDef, const CTagDefinition *pFragTagDef, SFragTagState *pMatchedFragTags = NULL, uint32 *pTagSetIdx = NULL) const
	{
		const uint32 numEntries = m_keys.size();
		for (uint32 i=0; i<numEntries; i++)
		{
			const SFragTagState &fragTagState = m_keys[i];

			if (pGlobalTagDef->Contains(fragTags.globalTags, fragTagState.globalTags) 
				&& (!pFragTagDef || pFragTagDef->Contains(fragTags.fragmentTags, fragTagState.fragmentTags)))
			{
				if (pMatchedFragTags)
				{
					*pMatchedFragTags = fragTagState;
				}
				if (pTagSetIdx)
				{
					*pTagSetIdx = i;
				}
				return &m_values[i];
			}
		}

		if (pTagSetIdx)
		{
			*pTagSetIdx = TAG_SET_IDX_INVALID;
		}
		return NULL;
	}

	const T *GetBestMatch(const SFragTagState &fragTags, const TagState &requiredTagState, const CTagDefinition *pGlobalTagDef, const CTagDefinition *pFragTagDef, SFragTagState *pMatchedFragTags = NULL) const
	{
		TagState requiredComparisonMask = pGlobalTagDef->GenerateMask(requiredTagState);

		const uint32 numEntries = m_keys.size();
		for (uint32 i=0; i<numEntries; i++)
		{
			const SFragTagState &fragTagState = m_keys[i];

			if (pGlobalTagDef->Contains(fragTagState.globalTags, requiredTagState, requiredComparisonMask)
				&& pGlobalTagDef->Contains(fragTags.globalTags, fragTagState.globalTags) 
				&& (!pFragTagDef || pFragTagDef->Contains(fragTags.fragmentTags, fragTagState.fragmentTags)))
			{
				if (pMatchedFragTags)
				{
					*pMatchedFragTags = fragTagState;
				}
				return &m_values[i];
			}
		}

		return NULL;
	}

private:

	DynArray<SFragTagState> m_keys;
	DynArray<T>						  m_values;
};

typedef TTagSortedList<ActionScopes> TTagListActionScope;

struct SFragmentDef
{
	enum EFlags
	{
		PERSISTENT = BIT(0),
		AUTO_REINSTALL_BEST_MATCH = BIT(1)
	};

	SFragmentDef()
		:
		flags(0)
	{
		scopeMaskList.Insert(SFragTagState(), ACTION_SCOPES_NONE);
	}

	TTagListActionScope		scopeMaskList;
	uint8									flags;
};

struct SScopeDef
{
	SScopeDef()
		:
		context(SCOPE_CONTEXT_ID_INVALID),
		layer(0),
		numLayers(0),
		additionalTags(0)
	{
	}

	uint32 context;
	uint32 layer;
	uint32 numLayers;
	TagState additionalTags;
};

struct SSubContext
{
	ActionScopes scopeMask;
	TagState additionalTags;
};

struct SControllerDef
{
	SControllerDef(const CTagDefinition &tags, const CTagDefinition &fragmentIDs, const CTagDefinition &scopeIDs)
		:
		m_tags(tags),
		m_fragmentIDs(fragmentIDs),
		m_scopeIDs(scopeIDs)
	{
	}

	TDefPathString        m_filename;
	const CTagDefinition &m_tags;
	const CTagDefinition &m_fragmentIDs;
	const CTagDefinition  m_scopeIDs;
	CTagDefinition				m_subContextIDs;
	CTagDefinition        m_scopeContexts;

	const SFragmentDef &GetFragmentDef(FragmentID fragment) const
	{
		IF_LIKELY (m_fragmentIDs.IsValidTagID(fragment))
		{
			return m_fragmentDef[fragment];
		}
		else
		{
			CRY_ASSERT(false);
			return m_fragmentDef[0];
		}
	}

	const CTagDefinition *GetFragmentTagDef(FragmentID fragment) const
	{
		return m_fragmentIDs.GetSubTagDefinition(fragment);
	}

	const ActionScopes GetScopeMask(FragmentID fragID, const SFragTagState &fragTagState, TagID subContext = TAG_ID_INVALID) const
	{
		ActionScopes scopeMask = ACTION_SCOPES_NONE;
		IF_LIKELY (m_fragmentIDs.IsValidTagID(fragID))
		{
			const CTagDefinition *pFragTagDef = m_fragmentIDs.GetSubTagDefinition(fragID);
			scopeMask = *m_fragmentDef[fragID].scopeMaskList.GetBestMatch(fragTagState, &m_tags, pFragTagDef);

			if (subContext != TAG_ID_INVALID)
			{
				scopeMask |= m_subContext[subContext].scopeMask;
			}
		}

		return scopeMask;
	}

	DynArray<SFragmentDef> m_fragmentDef;
	DynArray<SScopeDef> m_scopeDef;
	DynArray<SSubContext> m_subContext;
};

struct SAnimationContext
{
	SAnimationContext(const SControllerDef &_controllerDef)
		:
		controllerDef(_controllerDef),
		state(_controllerDef.m_tags)
	{
		subStates.resize(_controllerDef.m_subContextIDs.GetNum(), state);
	}

	const SControllerDef &controllerDef;
	CTagState             state;
	DynArray<CTagState>   subStates;
	CMTRand_int32         randGenerator;
};

struct SMannHistoryItem
{
	SMannHistoryItem()
		:
		time(-1.0f),
		type(None),
		tagState(0),
		scopeMask(0),
		fragment(FRAGMENT_ID_INVALID),
		optionIdx(0),
		trumpsPrevious(false)
	{
	}

	SMannHistoryItem(ActionScopes _scopeMask, FragmentID _fragment, const TagState &_tagState, uint32 _optionIdx, bool _trumpsPrevious = false)
		:
		time(-1.0f),
		type(Fragment),
		tagState(_tagState),
		scopeMask(_scopeMask),
		fragment(_fragment),
		optionIdx(_optionIdx),
		trumpsPrevious(_trumpsPrevious)
	{
	}
	SMannHistoryItem(const TagState &_tagState)
		:
		time(-1.0f),
		type(Tag),
		tagState(_tagState),
		scopeMask(0),
		fragment(FRAGMENT_ID_INVALID),
		optionIdx(0),
		trumpsPrevious(false)
	{
	}

	enum EType
	{
		Fragment,
		Tag,
		None
	};
	float					time;
	TagState			tagState;
	ActionScopes	scopeMask;
	FragmentID		fragment;
	uint32				optionIdx;
	bool					trumpsPrevious;
	uint8					type;
};

struct IMannequinListener
{
	virtual ~IMannequinListener(){}

	virtual void OnEvent(const SMannHistoryItem &historyItem, const class IActionController& actionController) = 0;
};

struct SMannequinErrorReport
{
	SMannequinErrorReport()
		:
		errorType(None),
		fragID(FRAGMENT_ID_INVALID),
		fragIDTo(FRAGMENT_ID_INVALID),
		fragOptionID(0)
	{
		error[0] = '\0';
	}
	enum ErrorType
	{
		None,
		Fragment,
		Blend
	};

	char					error[MANN_ERROR_BUFFER_SIZE];
	ErrorType			errorType;
	FragmentID		fragID;
	SFragTagState	tags;
	FragmentID		fragIDTo;
	SFragTagState	tagsTo;
	uint32				fragOptionID;
};

struct SAnimAssetReport
{
	SAnimAssetReport()
		:
		pAnimName(NULL),
		pAnimPath(NULL),
		animID(-1),
		fragID(FRAGMENT_ID_INVALID),
		fragIDTo(FRAGMENT_ID_INVALID),
		fragOptionID(0)
	{
	}

	const char	  *pAnimName;
	const char		*pAnimPath;
	int						 animID;
	FragmentID		 fragID;
	SFragTagState	 tags;
	FragmentID		 fragIDTo;
	SFragTagState	 tagsTo;
	uint32				fragOptionID;
};

struct SBlendQueryResult
{
	SBlendQueryResult()
		:
		fragmentFrom(FRAGMENT_ID_INVALID),
		fragmentTo(FRAGMENT_ID_INVALID),
		pFragmentBlend(NULL),
		blendIdx(0),
		selectTime(0.0f),
		duration(0.0f)
	{
	}

	FragmentID		fragmentFrom;
	FragmentID		fragmentTo;
	SFragTagState	tagStateFrom;
	SFragTagState	tagStateTo;
	const SFragmentBlend *pFragmentBlend;
	uint32				blendIdx;
	float					selectTime;
	float					duration;
};

struct SBlendQuery
{
	enum EFlags
	{
		fromInstalled	 = BIT(0),
		toInstalled	 = BIT(1),
		higherPriority = BIT(2)
	};

	SBlendQuery()
		:
		fragmentFrom(FRAGMENT_ID_INVALID),
		fragmentTo(FRAGMENT_ID_INVALID),
		additionalTags(0),
		fragmentTime(0.0f),
		normalisedTime(0.0f),
		flags(0)
	{
	}

	bool IsFlagSet(uint32 flag) const
	{
		return (flags & flag) == flag;
	}
	void SetFlag(uint32 flag, bool set)
	{
		if (set)
			flags |= flag;
		else
			flags &= ~flag;
	}

	FragmentID		fragmentFrom;
	FragmentID		fragmentTo;
	SFragTagState	tagStateFrom;
	SFragTagState tagStateTo;
	TagState			additionalTags;
	float					fragmentTime;
	float					normalisedTime;
	uint32				flags;
};

struct SFragmentQuery
{
	SFragmentQuery(const CTagDefinition& fragDef, FragmentID _fragID = FRAGMENT_ID_INVALID, SFragTagState _tagState = SFragTagState(), TagState _requiredTags = TAG_STATE_EMPTY, uint32	_optionIdx = OPTION_IDX_RANDOM)
		:
	fragID(_fragID),
		tagState(_tagState),
		requiredTags(_requiredTags),
		optionIdx(_optionIdx)
	{
		tagState.globalTags = fragDef.GetUnion(tagState.globalTags, _requiredTags);
	}

	SFragmentQuery(FragmentID _fragID = FRAGMENT_ID_INVALID, SFragTagState _tagState = SFragTagState(), uint32	_optionIdx = OPTION_IDX_RANDOM)
	:
		fragID(_fragID),
		tagState(_tagState),
		requiredTags(0),
		optionIdx(_optionIdx)
	{
	}

	FragmentID		fragID;
	SFragTagState tagState;
	TagState			requiredTags;
	uint32				optionIdx;
};

struct SFragmentSelection
{
	SFragmentSelection(SFragTagState _tagState = SFragTagState(), uint32	_optionIdx = OPTION_IDX_RANDOM)
		:
		tagState(_tagState),
		optionIdx(_optionIdx)
	{
	}

	SFragTagState tagState;
	uint32				optionIdx;
};

struct SMiniSubADB
{
	TagState tags;
	string filename;
	const CTagDefinition *pFragDef;

	typedef DynArray<FragmentID> TFragIDArray;
	TFragIDArray vFragIDs;

	typedef DynArray<SMiniSubADB> TSubADBArray;
	TSubADBArray vSubADBs;
};

struct SEditorFragmentBlendID
{
	FragmentID fragFrom;
	FragmentID fragTo;
	typedef DynArray<SEditorFragmentBlendID> TEditorFragmentBlendIDArray;
};

struct SEditorFragmentBlendVariant
{
	SFragTagState tagsFrom;
	SFragTagState tagsTo;
	typedef DynArray<SEditorFragmentBlendVariant> TEditorFragmentBlendVariantArray;
};

typedef void (*MannErrorCallback)(const SMannequinErrorReport &errorReport, void *_context);
typedef void (*MannAssetCallback)(const SAnimAssetReport &assetReport, void *_context);
class IAnimationDatabase
{
public:
	virtual ~IAnimationDatabase() {}

	virtual bool Validate(const struct IAnimationSet *animSet, MannErrorCallback errorCallback = NULL, MannErrorCallback warningCallback = NULL, void *errorCallbackContext = NULL) const = 0;
	virtual void EnumerateAnimAssets(const IAnimationSet *animSet, MannAssetCallback assetCallback, void *callbackContext) const = 0;

	virtual const char *GetFilename() const = 0;
	virtual bool Query(SFragmentData &outFragmentData, const SFragmentQuery &inFragQuery, SFragmentSelection *outFragSelection = NULL) const = 0;

	//-----------------------------------------------------------------
	// Main Query function, expands queried fragment and transition data out into the target fragData buffer
	// Returns the following flags to say which sources are contributing to the data
	// From eSequenceFlags: eSF_Fragment, eSF_TransitionOutro, eSF_Transition
	//-----------------------------------------------------------------
	virtual uint32 Query(SFragmentData &outFragmentData, const SBlendQuery &inBlendQuery, uint32 inOptionIdx, const IAnimationSet *inAnimSet, SFragmentSelection *outFragSelection = NULL) const = 0;

	virtual uint32 FindBestMatchingTag(FragmentID fragmentId, const SFragTagState &state, SFragTagState *matchingTagState = NULL, uint32* tagSetIdx = NULL) const = 0;
	virtual const CTagDefinition &GetTagDefs() const = 0;
	virtual FragmentID GetFragmentID(const char *szActionName) const = 0;
	virtual const CTagDefinition &GetFragmentDefs() const = 0;

	virtual uint32 GetTotalTagSets(FragmentID fragmentID) const = 0;
	virtual uint32 GetTagSetInfo(FragmentID fragmentID, uint32 tagSetID, SFragTagState &fragTagState) const = 0;

	virtual const CFragment *GetBestEntry(const SFragmentQuery &fragQuery, SFragmentSelection *fragSelection = NULL) const = 0;
	virtual const CFragment *GetEntry(FragmentID fragmentID, const SFragTagState &tags, uint32 optionIdx) const = 0;
	
	virtual void FindBestBlends(const SBlendQuery &blendQuery, SBlendQueryResult &result1, SBlendQueryResult &result2) const = 0;
	virtual uint32 GetNumBlends(FragmentID fragmentIDFrom, FragmentID fragmentIDTo, const SFragTagState &tagFrom, const SFragTagState &tagTo) const = 0;
	virtual const SFragmentBlend *GetBlend(FragmentID fragmentIDFrom, FragmentID fragmentIDTo, const SFragTagState &tagFrom, const SFragTagState &tagTo, uint32 blendNum) const = 0;
	virtual void SetBlend(FragmentID fragmentIDFrom, FragmentID fragmentIDTo, const SFragTagState &tagFrom, const SFragTagState &tagTo, uint32 blendNum, const SFragmentBlend &fragBlend) = 0;
	virtual uint32 AddBlend(FragmentID fragmentIDFrom, FragmentID fragmentIDTo, const SFragTagState &tagFrom, const SFragTagState &tagTo, const SFragmentBlend &fragBlend) = 0;
	virtual void DeleteBlend(FragmentID fragmentIDFrom, FragmentID fragmentIDTo, const SFragTagState &tagFrom, const SFragTagState &tagTo, uint32 blendNum) = 0;

	virtual const char* FindSubADBFilenameForID(FragmentID fragmentID) const = 0;
	virtual bool RemoveSubADBFragmentFilter ( FragmentID fragmentID ) = 0;
	virtual bool AddSubADBFragmentFilter ( const string &sADBFileName, FragmentID fragmentID ) = 0;
	virtual void GetSubADBFragmentFilters( SMiniSubADB::TSubADBArray &outList ) = 0;

	virtual bool AddSubADBTagFilter( const string &sParentFilename, const string &sADBFileName, const TagState tag ) = 0;
	virtual bool MoveSubADBFilter( const string &sADBFileName, const bool bMoveUp) = 0;
	virtual bool DeleteSubADBFilter( const string &sADBFileName ) = 0;
	virtual bool ClearSubADBFilter( const string &sADBFileName ) = 0;

	virtual void GetFragmentBlends(SEditorFragmentBlendID::TEditorFragmentBlendIDArray &outBlendIDs) const = 0;
	virtual void GetFragmentBlendVariants(const FragmentID fragmentIDFrom, const FragmentID fragmentIDTo, SEditorFragmentBlendVariant::TEditorFragmentBlendVariantArray &outVariants) const = 0;
};


class IAnimationDatabaseManager
{
public:
	virtual ~IAnimationDatabaseManager() {}

	virtual int GetTotalDatabases() const = 0;
	virtual const IAnimationDatabase *GetDatabase(int idx) const = 0;

	virtual const IAnimationDatabase *FindDatabase(const uint32 crcFilename) const = 0;
	virtual const IAnimationDatabase *Load(const char *filename) = 0;
	virtual const SControllerDef *LoadControllerDef(const char *filename) = 0;
	virtual const CTagDefinition *LoadTagDefs(const char *filename) = 0;

	virtual const SControllerDef *FindControllerDef(const uint32 crcFilename) const = 0;
	virtual const SControllerDef *FindControllerDef(const char *filename) const = 0;

	virtual const CTagDefinition *FindTagDef(const uint32 crcFilename) const = 0;
	virtual const CTagDefinition *FindTagDef(const char *filename) const = 0;

	// Registers a tag definition with the animation database manager, and *gives ownership of tagDefinition in case of success*.
	// The filename in the tag definition will be used as key for registering. If a tagDefinition with that filename is already in the database manager, registration will fail.
	// When registered, a tag definition can be searched for by filename or by filename crc with the FindTagDef functions.
	virtual bool RegisterTagDef(CTagDefinition *tagDefinition) = 0;

	virtual IAnimationDatabase *Create(const char *filename, const char *defFilename) = 0;
	virtual CTagDefinition *CreateTagDefinition(const char *filename) = 0;

	virtual void ReloadAll() = 0;
	virtual void UnloadAll() = 0;
};


enum EPriorityComparison
{
	Lower,
	Equal,
	Higher
};

class IActionController;
class IAction;
typedef _smart_ptr< IAction > IActionPtr;
class CAnimation;
struct SGameObjectEvent;

class IScope
{
public:
	virtual ~IScope() {}

	virtual const char *GetName() = 0;
	virtual uint32 GetID() = 0;
	virtual ICharacterInstance *GetCharInst() = 0;
	virtual IActionController &GetActionController() const = 0;
	virtual SAnimationContext &GetContext() const = 0;
	virtual uint32 GetContextID() const = 0;
	virtual const IAnimationDatabase &GetDatabase() const = 0;
	virtual IEntity &GetEntity() const = 0;
	virtual EntityId GetEntityId() const = 0;
	virtual IActionPtr GetAction() const = 0;
	virtual bool HasDatabase() const = 0;

	virtual uint32 GetTotalLayers() const = 0;
	virtual uint32 GetBaseLayer() const = 0;

	virtual void IncrementTime(float timeDelta) = 0;

	virtual const CAnimation *GetTopAnim(int layer) const = 0;
	virtual CAnimation *GetTopAnim(int layer) = 0;

	virtual void ApplyAnimWeight(uint32 layer, float weight) = 0;

	virtual bool IsDifferent(FragmentID aaID, TagState fragmentTags, const TagID subContext = TAG_ID_INVALID) const = 0;
	virtual FragmentID GetLastFragmentID() const = 0;
	virtual const SFragTagState &GetLastTagState() const = 0;
	virtual float CalculateFragmentTimeRemaining() const = 0;
	virtual float CalculateFragmentDuration(const CFragment &fragment) const = 0;

	virtual float GetFragmentDuration() const = 0;
	virtual float GetFragmentTime() const = 0;

	virtual TagState GetAdditionalTags() const = 0;

	virtual void _FlushFromEditor() = 0; // needs to be moved into an editor-only interface

	template <typename PODTYPE>
	bool GetParam(const char *paramName, PODTYPE &value) const;
};


enum EActionControllerFlags
{
	AC_PausedUpdate = (1<<0),
	AC_DebugDraw    = (1<<1),
	AC_DumpState    = (1<<2),
	AC_IsInUpdate   = (1<<3),
};

enum EActionFailure
{
	AF_QueueFull,
	AF_InvalidContext,
};

class CMannequinParams
{
public:

	void Reset()
	{
		m_paramList.clear();
	}

	template <typename PODTYPE>
	bool GetParam(const char *paramName, PODTYPE &value) const
	{
		const uint32 crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(paramName);
		return GetParam(crc, value);
	}

	template <typename PODTYPE>
	bool GetParam(uint32 paramNameCRC, PODTYPE &value) const
	{
		COMPILE_TIME_ASSERT(sizeof(PODTYPE) <= sizeof(QuatT));
		const uint32 numParams = m_paramList.size();
		for (uint32 i=0; i<numParams; i++)
		{
			const SMannParameter &param = m_paramList[i];
			if (param.crc == paramNameCRC)
			{
				value = *alias_cast<const PODTYPE*>(&param.value);
				return true;
			}
		}

		return false;
	}

	bool RemoveParam(const char *paramName)
	{
		const uint32 crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(paramName);
		return RemoveParam(crc);
	}

	bool RemoveParam(uint32 paramNameCRC)
	{
		const TMannParamList::iterator iEnd = m_paramList.end();
		for(TMannParamList::iterator i = m_paramList.begin(); i != iEnd; ++i)
		{
			const SMannParameter &param = *i;
			if (param.crc == paramNameCRC)
			{
				m_paramList.erase(i);
				return true;
			}
		}
		return false;
	}

	template <typename PODTYPE>
	void SetParam(const char *paramName, const PODTYPE &value)
	{
		const uint32 crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(paramName);
		SetParam(crc, value);
	}

	template <typename PODTYPE>
	void SetParam(uint32 paramNameCRC, const PODTYPE &value)
	{
		COMPILE_TIME_ASSERT(sizeof(PODTYPE) <= sizeof(QuatT));
		const uint32 numParams = m_paramList.size();
		for (uint32 i=0; i<numParams; i++)
		{
			SMannParameter &param = m_paramList[i];
			if (param.crc == paramNameCRC)
			{
				*alias_cast<PODTYPE*>(&param.value) = value;
				return;
			}
		}

		m_paramList.resize(numParams+1);
		SMannParameter &param = m_paramList[numParams];
		param.crc = paramNameCRC;
		*alias_cast<PODTYPE*>(&param.value) = value;
	}

private:
	TMannParamList m_paramList;
};

struct SScopeContextQuery
{
	SScopeContextQuery()
		: pDatabase(NULL)
		, pCharInst(NULL)
		, entityId(0)
	{
	}

	const IAnimationDatabase			*pDatabase;
	ICharacterInstance						*pCharInst;
	EntityId											 entityId;
};

class IActionController
{
public:
	virtual void OnEvent(const SGameObjectEvent& event) = 0;
	virtual void OnAnimationEvent(ICharacterInstance *pCharacter, const AnimEventInstance &event) = 0;

	virtual void FlushProceduralContexts() = 0;

	virtual void Reset() = 0;

	virtual uint32 GetTotalScopes() const = 0;
	virtual void SetScopeContext(uint32 scopeContextID, IEntity &entity, ICharacterInstance *pCharacter, const IAnimationDatabase *animDatabase) = 0;
	virtual void ClearScopeContext(uint32 scopeContextID, bool flushAnimations = true) = 0;
	virtual void GetScopeContext(uint32 scopeContextID, SScopeContextQuery &query) const = 0;

	virtual bool IsScopeActive(uint32 scopeID) const = 0;
	virtual ActionScopes GetActiveScopeMask() const = 0;

	virtual IEntity &GetEntity() const = 0;
	virtual EntityId GetEntityId() const = 0;

	virtual IScope *GetScope(uint32 scopeID) = 0;
	virtual const IScope *GetScope(uint32 scopeID) const = 0;

	virtual uint32 GetScopeID(const char *name) const = 0;

	virtual FragmentID GetFragID( uint32 crc ) const = 0;
	virtual TagID GetGlobalTagID( uint32 crc ) const = 0;
	virtual TagID GetFragTagID( FragmentID fragID, uint32 crc ) const = 0;
	virtual const CTagDefinition* GetTagDefinition( FragmentID fragID) const = 0;

	virtual bool CanInstallNow(int priority, ActionScopes scopeMask) const = 0;
	virtual void Queue(const IActionPtr &pAction, float time = -1.0f) = 0;
	virtual void Requeue(const IActionPtr &pAction) = 0;

	virtual void Update(float timePassed) = 0;

	virtual SAnimationContext &GetContext() = 0;
	virtual const SAnimationContext &GetContext() const = 0;

	virtual void Pause() = 0;
	enum EResumeFlags
	{
		ERF_RestartAnimations              = BIT(0),
		ERF_RestoreLoopingAnimationTime    = BIT(1),
		ERF_RestoreNonLoopingAnimationTime = BIT(2),

		ERF_Default = ERF_RestartAnimations | ERF_RestoreLoopingAnimationTime | ERF_RestoreNonLoopingAnimationTime
	};
	virtual void Resume(uint32 resumeFlags = ERF_Default) = 0;

	virtual void SetFlag(EActionControllerFlags flag, bool enable) = 0;

	// Only needed for animationgraph?
	virtual bool IsActionPending(uint32 userToken) const = 0;

	virtual bool CanInstall(const IAction &action, const ActionScopes &scopeMask, float timeStep, float &timeTillInstall) const = 0;

	virtual bool QueryDuration(IAction &action, float &fragmentDuration, float &transitionDuration) const = 0;

	virtual void SetSlaveController(IActionController *target, uint32 targetContext, bool enslave, const IAnimationDatabase* piOptionTargetDatabase) = 0;

	virtual void RegisterListener(IMannequinListener *listener) = 0;
	virtual void UnregisterListener(IMannequinListener *listener) = 0;

	virtual class IProceduralContext *FindOrCreateProceduralContext(const char *contextName) = 0;
	virtual const class IProceduralContext *FindProceduralContext(const char *contextName) const = 0;
	virtual class IProceduralContext *FindProceduralContext(const char *contextName) = 0;
	virtual class IProceduralContext *CreateProceduralContext(const char *contextName) = 0;

	virtual void Release() = 0;

	template <typename PODTYPE>
	bool GetParam(const char *paramName, PODTYPE &value) const
	{
		return m_mannequinParams.GetParam(paramName, value);
	}

	template <typename PODTYPE>
	bool GetParam(uint32 paramNameCRC, PODTYPE &value) const
	{
		return m_mannequinParams.GetParam(paramNameCRC, value);
	}

	bool RemoveParam(const char *paramName)
	{
		return m_mannequinParams.RemoveParam(paramName);
	}
	
	bool RemoveParam(uint32 paramNameCRC)
	{
		return m_mannequinParams.RemoveParam(paramNameCRC);
	}

	template <typename PODTYPE>
	void SetParam(const char *paramName, const PODTYPE &value)
	{
		return m_mannequinParams.SetParam(paramName, value);
	}

	template <typename PODTYPE>
	void SetParam(uint32 paramNameCRC, const PODTYPE &value)
	{
		return m_mannequinParams.SetParam(paramNameCRC, value);
	}

	void ResetParams()
	{
		m_mannequinParams.Reset();
	}

protected:
	virtual ~IActionController() {}

	CMannequinParams m_mannequinParams;
};

#define DEFINE_ACTION(name)	\
	virtual const char *GetName() const {return name; }	\
	virtual void  DoDelete() { delete this; }


class IAction
{
public:
	friend class CActionController;
	friend class CActionScope;

	//---------------------------------------------------------------------------------------------------------
	// Action Status
	//---------------------------------------------------------------------------------------------------------
	// None					- Not installed
	// Pending			- In the pending action queue
	// Installed		- Installed on a scope
	// Exiting			- Action is finishing up, via an exit transition
	// Finished			- Finished. Will be removed from its scope on the the next update
	//---------------------------------------------------------------------------------------------------------
	enum EStatus
	{
		None,
		Pending,
		Installed,
		Exiting,
		Finished
	};

	//---------------------------------------------------------------------------------------------------------
	// Action Flags
	//---------------------------------------------------------------------------------------------------------
	// BlendOut								- Can the action now blend out into a new action?
	// NoAutoBlendOut					- The action should not allow itself to be blended out on completion
	// Interruptable					- The action can be interrupted and so is pushed onto the pending action queue 
	//													rather than deleted
	// OnLastAnim							- Is this action currently on the last anim clip in the fragment?
	// Installing							- This action is in the process of being installed (Enter is not called yet & action is still in the queue)
	// Requeued								- This action is installed for a requeue on the pending queue
	// TrumpSelf							- This action should be treated as a higher priority when compared to itself
	// Transitioning					- This action is transitioning in
	// PlayingFragment				- This action is playing its core fragment
	// TransitioningOut				- This action is transitioning out
	// TransitionPending			- This action is going to transition off soon
	// FragmentIsOneShot			- This action is a one-shot & so will end itself at the end of the sequence
	// Stopping								- This action is marked for stopping
	//---------------------------------------------------------------------------------------------------------
	enum EFlags
	{
		BlendOut							= BIT(0),
		NoAutoBlendOut				= BIT(1),
		Interruptable					= BIT(2),
		OnLastAnim						= BIT(3),
		Installing						= BIT(4),
		Started								= BIT(5),
		Requeued							= BIT(6),
		TrumpSelf							= BIT(7),
		Transitioning					= BIT(8),
		PlayingFragment				= BIT(9),
		TransitioningOut			= BIT(10),
		TransitionPending			= BIT(11),
		FragmentIsOneShot			= BIT(12),
		Stopping							= BIT(13),
		PlaybackStateMask			= (Transitioning|PlayingFragment|TransitioningOut)
	};

	virtual ~IAction() 
	{
#ifndef _RELEASE
		if((m_flags & Started))
		{
			DEBUG_BREAK;
		}
#endif //_RELEASE
	}

	IAction(int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
		: 
		m_context(NULL),
		m_activeTime(0.0f),
		m_queueTime(-1.0f),
		m_forcedScopeMask(scopeMask),
		m_installedScopeMask(0),
		m_subContext(TAG_ID_INVALID),
		m_priority(priority),
		m_eStatus(None), 
		m_flags(flags),
		m_rootScope(NULL),
		m_fragmentID(fragmentID),
		m_userToken(userToken),
		m_refCount(0),
		m_speedBias(1.0f),
		m_animWeight(1.0f),
		m_fragTags(fragTags),
		m_optionIdx(OPTION_IDX_RANDOM)
	{
	}

	void AddRef()
	{
		m_refCount++;
	}
	void Release()
	{
		m_refCount--;
		if (m_refCount <= 0)
		{
			CRY_ASSERT((m_flags & Started) == 0);

			DoDelete();
		}
	}

	FragmentID GetFragmentID() const
	{
		return m_fragmentID;
	}
	TagState GetFragTagState() const
	{
		return m_fragTags;
	}
	uint32 GetOptionIdx() const
	{
		return m_optionIdx;
	}
	uint32 GetUserToken() const
	{
		return m_userToken;
	}
	int GetPriority() const
	{
		return m_priority;
	}
	void SetOptionIdx(uint32 optionIdx)
	{
		m_optionIdx = optionIdx;
	}

	bool CanBlendOut(EPriorityComparison priorityComparison) const
	{
		switch(priorityComparison)
		{
		case Higher:
			return true;
			break;
		case Lower:
		case Equal:
		default:
			return ((m_flags & (FragmentIsOneShot|NoAutoBlendOut)) == FragmentIsOneShot) 
							|| ((m_flags & BlendOut) != 0)
							|| (m_eStatus == Finished) 
							|| (m_eStatus == Exiting);
			break;
		}
	}
	ActionScopes GetInstalledScopeMask() const
	{
		return m_installedScopeMask;
	}
	ActionScopes GetForcedScopeMask() const
	{
		return m_forcedScopeMask;
	}
	EStatus GetStatus() const
	{
		return m_eStatus;
	}
	const IScope &GetRootScope() const
	{
		CRY_ASSERT_MESSAGE(m_rootScope, "Action not installed or queued into actionStack!");
		return *m_rootScope;
	}
	IScope &GetRootScope()
	{
		CRY_ASSERT_MESSAGE(m_rootScope, "Action not installed or queued into actionStack!");
		return *m_rootScope;
	}
	uint32 GetFlags() const
	{
		return m_flags;
	}
	float GetActiveTime() const
	{
		return m_activeTime;
	}
	virtual void Install()
	{
		if (m_eStatus != Finished)
		{
			m_eStatus = Installed;
		}
		m_flags	&= ~PlaybackStateMask;
	}
	virtual void Enter()
	{
		m_flags |= Started;
	}
	virtual void Fail(EActionFailure actionFailure)
	{
		m_eStatus = None;
		m_flags &= ~Started;
	}
	virtual void Exit()
	{
		m_eStatus = None;
		m_flags &= ~Started;
	}
	virtual EStatus UpdatePending(float timePassed)
	{
		const float oldActiveTime = m_activeTime;
		m_activeTime += timePassed;

		//--- If we have a passed a limited queue time and have ticked at least once, then exit
		if ((m_queueTime >= 0.0f) && (oldActiveTime > 0.0f) && (m_activeTime > m_queueTime))
		{
			m_eStatus = Finished;
		}

		return m_eStatus;
	}
	virtual EStatus Update(float timePassed)
	{
		m_activeTime += timePassed;

		if (m_fragmentID != FRAGMENT_ID_INVALID)
		{
			const SFragmentDef &fragmentDef = m_context->controllerDef.GetFragmentDef(m_fragmentID);
			if ((fragmentDef.flags & SFragmentDef::AUTO_REINSTALL_BEST_MATCH) != 0)
			{
				if (IsDifferent(m_fragmentID, m_fragTags))
				{
					SetFragment(m_fragmentID, m_fragTags, m_optionIdx, m_userToken, false);
				}
			}
		}

		return m_eStatus;
	}
	bool Interrupt()
	{
		if (IsInstalling() || ((m_flags & Started) == 0))
			m_eStatus = None;
		else
			Exit();

		return (m_flags & Interruptable) != 0;
	}

	virtual EPriorityComparison ComparePriority(const IAction &actionCurrent) const
	{
		return Equal;
	}

	virtual void OnRequestBlendOut(EPriorityComparison priorityComp)
	{
	}

	bool IsDifferent(FragmentID fragID, TagState fragmentTags) const;

	void SetSpeedBias(float speedBias)
	{
		m_speedBias = speedBias;
	}
	float GetSpeedBias() const
	{
		return m_speedBias;
	}

	TagID GetSubContext() const
	{
		return m_subContext;
	}
	void SetSubContext(const TagID subContext)
	{
		m_subContext = subContext;
	}

	void SetAnimWeight(float animWeight)
	{
		m_animWeight = animWeight;
	}
	float GetAnimWeight() const
	{
		return m_animWeight;
	}

	virtual void OnAnimInstalled(int layer, bool isLooping, bool lastInQueue)			
	{
		if ((layer == 0) && lastInQueue)
		{
			m_flags |= OnLastAnim;
		}
	}
	virtual void OnSequenceFinished(int layer, uint32 scopeId)	
	{
	}

	void Stop()
	{
		m_flags |= (BlendOut|Stopping);
	}

	void ForceFinish()
	{
		m_eStatus = Finished;
		m_flags &= ~Interruptable;
	}

	void BeginInstalling()
	{
		CRY_ASSERT(!IsInstalling());
		m_flags |= Installing;
		m_flags	&= ~(PlayingFragment|Transitioning);
	}
	void EndInstalling()
	{
		CRY_ASSERT(IsInstalling());
		m_flags &= ~Installing;
	}
	bool IsInstalling() const
	{
		return ((m_flags & Installing) != 0);
	}

	template <typename PODTYPE>
	bool GetParam(const char *paramName, PODTYPE &value) const
	{
		if (!m_mannequinParams.GetParam(paramName, value))
		{
			if (m_rootScope)
			{
				const IActionController& actionController = m_rootScope->GetActionController();
				return actionController.GetParam(paramName, value);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	template <typename PODTYPE>
	bool GetParam(uint32 paramNameCRC, PODTYPE &value) const
	{
		if (!m_mannequinParams.GetParam(paramNameCRC, value))
		{
			if (m_rootScope)
			{
				const IActionController& actionController = m_rootScope->GetActionController();
				return actionController.GetParam(paramNameCRC, value);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	template <typename PODTYPE>
	void SetParam(const char *paramName, const PODTYPE &value)
	{
		return m_mannequinParams.SetParam(paramName, value);
	}

	template <typename PODTYPE>
	void SetParam(uint32 paramNameCRC, const PODTYPE &value)
	{
		return m_mannequinParams.SetParam(paramNameCRC, value);
	}

	void ResetParams()
	{
		m_mannequinParams.Reset();
	}

	bool IsTransitioning() const
	{
		return (m_flags & Transitioning) != 0;
	}

	bool IsPlayingFragment() const
	{
		return (m_flags & PlayingFragment) != 0;
	}

	bool IsTransitioningOut() const
	{
		return (m_flags & TransitioningOut) != 0;
	}

	virtual void OnTransitionStarted()		{}
	virtual void OnFragmentStarted()			{}
	virtual void OnTransitionOutStarted() {}
	virtual void OnInitialise()						{}

	virtual void OnEvent(const SGameObjectEvent& event) {}
	virtual void OnAnimationEvent(ICharacterInstance *pCharacter, const AnimEventInstance &event) {}
	virtual void OnActionEvent(const uint32 eventCRC) {}

	virtual const char *GetName() const = 0;
	virtual void DoDelete() = 0;

protected:

	void SetFragment(const FragmentID fragmentID, const TagState tagState = TAG_STATE_EMPTY, uint32 optionIdx = OPTION_IDX_RANDOM, const uint32 userToken = 0, bool trumpSelf = true)
	{
		m_fragmentID = fragmentID;
		m_fragTags	 = tagState;
		m_optionIdx	 = optionIdx;
		m_userToken  = userToken;
		if (trumpSelf)
			m_flags	|= TrumpSelf;
		else
			m_flags	&= ~TrumpSelf;

		if (m_eStatus == Installed)
		{
			if ((m_flags & Requeued) == 0)
			{
				m_flags	|= Requeued;

				IActionController &actionController = m_rootScope->GetActionController();

				actionController.Requeue(this);
			}
		}
	}

	EPriorityComparison DoComparePriority(const IAction &actionCurrent) const
	{
		if ((&actionCurrent == this) && ((m_flags & TrumpSelf) != 0))
		{
			return Higher;
		}
		else if ((m_priority > actionCurrent.m_priority) 
						 || (actionCurrent.m_eStatus == Finished) 
						 || (actionCurrent.m_eStatus == Exiting))
		{
			return Higher;
		}
		else if (m_priority == actionCurrent.m_priority)
		{
			return ComparePriority(actionCurrent);
		}		
		return Lower;
	}

	SAnimationContext *m_context;
	float m_activeTime;
	float m_queueTime;
	ActionScopes m_forcedScopeMask;
	ActionScopes m_installedScopeMask;
	TagID  m_subContext;
	int m_priority;
	EStatus m_eStatus;
	uint32 m_flags;
	IScope *m_rootScope;
	FragmentID m_fragmentID;
	TagState   m_fragTags;
	uint32		 m_optionIdx;
	uint32 m_userToken;
	int m_refCount;
	float m_speedBias;
	float m_animWeight;

	CMannequinParams m_mannequinParams;

private:
	void TransitionStarted()
	{
		m_flags &= ~PlaybackStateMask;
		m_flags |= Transitioning;

		OnTransitionStarted();
	}

	void FragmentStarted()
	{
		m_flags &= ~PlaybackStateMask;
		m_flags |= PlayingFragment;

		OnFragmentStarted();
	}

	void TransitionOutStarted()
	{
		m_flags &= ~PlaybackStateMask;
		m_flags |= TransitioningOut;

		OnTransitionOutStarted();
	}

	void Initialise(SAnimationContext &context)
	{
		m_context		= &context;
		m_eStatus		= Pending;
		m_rootScope	= NULL;
		m_flags &= ~Started;

		m_activeTime = 0.0f;

		OnInitialise();
	}

};

template <class CONTEXT>
class TAction : public IAction
{
public:

	DEFINE_ACTION("BaseAction");

	TAction(int priority, FragmentID fragmentID = FRAGMENT_ID_INVALID, const TagState fragTags = TAG_STATE_EMPTY, uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0)
		: 
		IAction(priority, fragmentID, fragTags, flags, scopeMask, userToken)
	{
	}

	CONTEXT &GetContext()
	{
		return *((CONTEXT*)m_context);
	}
	const CONTEXT &GetContext() const
	{
		return *((const CONTEXT*)m_context);
	}
};

template <typename PODTYPE>
bool IScope::GetParam(const char *paramName, PODTYPE &value) const
{
	IActionPtr pAction = GetAction();
	return pAction ? pAction->GetParam(paramName, value) : false;
}

class CMannequinUserParamsManager;

struct IMannequin
{
	virtual ~IMannequin() {}

	virtual void UnloadAll() = 0;
	virtual void ReloadAll() = 0;

	virtual IAnimationDatabaseManager &GetAnimationDatabaseManager() = 0;
	virtual IActionController *CreateActionController(IEntity* pEntity, SAnimationContext &context) = 0;
	virtual IActionController *FindActionController(const IEntity &entity) = 0;
	virtual IMannequinEditorManager *GetMannequinEditorManager() = 0;
	virtual CMannequinUserParamsManager& GetMannequinUserParamsManager() = 0;

	virtual void AddMannequinGameListener(IMannequinGameListener *pListener) = 0;
	virtual void RemoveMannequinGameListener(IMannequinGameListener *pListener) = 0;
	virtual uint32 GetNumMannequinGameListeners() = 0;
	virtual IMannequinGameListener *GetMannequinGameListener(uint32 idx) = 0;
	// Indicates if the mouse is doing something (select / drag / move / etc...)
	virtual void SetSilentPlaybackMode (bool bSilentPlaybackMode) = 0;
	virtual bool IsSilentPlaybackMode() const = 0;
};

bool ILINE IAction::IsDifferent(FragmentID fragID, TagState fragmentTags) const
{
	IActionController &actionController = GetRootScope().GetActionController();
	const uint32 numScopes = actionController.GetTotalScopes();
	const ActionScopes mask = actionController.GetActiveScopeMask() & m_installedScopeMask;

	for (uint32 i=0; i<numScopes; i++)
	{
		if ((1<<i) & mask)
		{
			IScope *scope = actionController.GetScope(i);
			if (scope && scope->IsDifferent(fragID, fragmentTags))
				return true;
		}
	}

	return false;
}

struct SProceduralParams
{
	SAnimRef animRef;
	SProcDataCRC dataCRC;
	TProcClipString dataString;
};

class IProceduralClip :	public ICryUnknown
{
public:
	IProceduralClip()
		: 
		m_entity(NULL),
		m_charInstance(NULL),
		m_scope(NULL),
		m_action(NULL)
	{
	}

	CRYINTERFACE_DECLARE(IProceduralClip, 0x79741495637C491e, 0xB8ACEB8A7F2AC463)

	virtual void Initialise(IEntity	&entity, ICharacterInstance	&charInstance, IScope &scope, IAction &action)
	{
		m_entity				= &entity;
		m_charInstance	= &charInstance;
		m_scope					= &scope;
		m_action				= &action;
	}
	virtual void INTERNAL_OnEnter(float blendTime,  float duration, const SProceduralParams &proceduralParams) = 0;
	virtual void OnFail() {}
	virtual void OnExit(float blendTime) = 0;
	virtual void Update(float timePassed) = 0;
	virtual const char *GetContextName() const {return NULL;}
	virtual void SetContext(class IProceduralContext *procContext) {CRY_ASSERT(0);}

protected:
	template <typename PODTYPE>
	bool GetParam(const char *paramName, PODTYPE &value) const
	{
		CRY_ASSERT(m_action != 0);
		return m_action->GetParam(paramName, value);
	}

	template <typename PODTYPE>
	bool GetParam(uint32 paramNameCRC, PODTYPE &value) const
	{
		CRY_ASSERT(m_action != 0);
		return m_action->GetParam(paramNameCRC, value);
	}

	bool IsRootEntity() const
	{
		CRY_ASSERT(m_action != 0);
		return (m_scope == &m_action->GetRootScope()) || (m_scope->GetEntityId() != m_action->GetRootScope().GetEntityId());
	}

	void SendActionEvent(const uint32 eventCRC) const
	{
		CRY_ASSERT(m_action != 0);
		if(eventCRC != 0 && IsRootEntity())
		{
			m_action->OnActionEvent(eventCRC);
		}
	}

protected:
	IEntity            *m_entity;
	ICharacterInstance *m_charInstance;
	IScope             *m_scope;

private:
	IActionPtr m_action;
};

#define PROCEDURAL_CONTEXT(className, name, uid1, uid2)		\
		CRYINTERFACE_BEGIN()																	\
			CRYINTERFACE_ADD(IProceduralContext)								\
		CRYINTERFACE_END()																		\
		CRYGENERATE_CLASS(className, name, uid1, uid2)				\
	public:																									\
		static const char *GetContextName() { return name; }


class IProceduralContext :	public ICryUnknown
{
public:
	IProceduralContext()
		: 
		m_entity(NULL),
		m_actionController(NULL)
	{
	}

	CRYINTERFACE_DECLARE(IProceduralContext, 0xCC61BC284B5243E0, 0xAAE3950B2A7F7DCB)

	virtual void Initialise(IEntity	&entity, IActionController &actionController)
	{
		m_entity = &entity;
		m_actionController = &actionController;
	}

	virtual void Update(float timePassed) = 0;

protected:
	IEntity	*m_entity;
	IActionController *m_actionController;
};

template <class PARAMS>
class TProceduralClip : public IProceduralClip
{
public:

	virtual void INTERNAL_OnEnter(float blendTime, float duration, const SProceduralParams &proceduralParams)
	{
		m_params = *((const PARAMS *)&proceduralParams);
		OnEnter(blendTime, duration, m_params);
	}

	ILINE const PARAMS &GetParams() const
	{
		return m_params;
	}

	virtual void OnEnter(float blendTime, float duration, const PARAMS &proceduralParams) = 0;

private:
	PARAMS m_params;
};

template <class PARAMS, class CONTEXT>
class TProceduralContextualClip : public TProceduralClip<PARAMS>
{
public:

	virtual const char *GetContextName() const 
	{
		return CONTEXT::GetContextName();
	}
	virtual void SetContext(class IProceduralContext *procContext) 
	{
		m_context = (CONTEXT*)procContext;
	}

protected:
	CONTEXT *m_context;
};

//////////////////////////////////////////////////////////////////////////
// Specify a SFragmentQuery to get all animations cached that are contained in
// fragments that match the query.
class CFragmentCache
{
public:
	// This will not precache anything, you must call AddAllAnimsFromAllScopes or AddAnimCRCs.
	explicit CFragmentCache( const SFragmentQuery& fragmentQuery )
		: m_fragmentQuery(fragmentQuery)
		, m_numOptions(OPTION_IDX_INVALID)
	{
	}
	// Automatically precaches everything in the actioncontroller that matches the query.
	CFragmentCache( const SFragmentQuery& fragmentQuery, const IActionController* piActionController )
		: m_fragmentQuery(fragmentQuery)
		, m_numOptions(OPTION_IDX_INVALID)
	{
		PrecacheAnimsFromAllDatabases( piActionController );
	}
	// Copies an existing fragment cache but allows you to update the option - useful for round robins.
	CFragmentCache( const CFragmentCache& fragmentCache, const IActionController* piActionController, const uint32 optionIdx )
		: m_fragmentQuery(fragmentCache.m_fragmentQuery)
		, m_numOptions(OPTION_IDX_INVALID)
	{
		m_fragmentQuery.optionIdx = optionIdx;

		PrecacheAnimsFromAllDatabases( piActionController );
	}
	~CFragmentCache()
	{
		Release();
	}
	void PrecacheAnimsFromAllDatabases( const IActionController* piActionController )
	{
		const uint32 numScopes = piActionController->GetTotalScopes();
		for( uint32 i=0; i<numScopes; ++i )
		{
			// TODO: We should defend adding the same ADB more than once.
			const IScope* piScope = piActionController->GetScope(i);
			if( piScope->HasDatabase() )
			{
				const IAnimationDatabase& animationDB = piScope->GetDatabase();
				PrecacheAnimsFromDatabase( &animationDB, piActionController );
			}
		}
	}
	void PrecacheAnimsFromDatabase( const IAnimationDatabase* pAnimDB, const IActionController* piActionController )
	{
		SFragTagState fragTagStateMatch;

		// TODO: this should use Union, see for example CActionScope::FillBlendQuery.
		SFragTagState fragTagStateQuery( m_fragmentQuery.tagState.globalTags, m_fragmentQuery.tagState.fragmentTags );
		const uint32 numOptions = pAnimDB->FindBestMatchingTag(m_fragmentQuery.fragID, fragTagStateQuery, &fragTagStateMatch);

		if( (numOptions > 0) )
		{
			if( m_numOptions == OPTION_IDX_INVALID )
			{
				m_numOptions = numOptions;
			}
			else
			{
				m_numOptions = max( m_numOptions, numOptions );
			}
		}

		if( m_numOptions != OPTION_IDX_INVALID )
		{
			const CTagDefinition* pTagDef = pAnimDB->GetFragmentDefs().GetSubTagDefinition( m_fragmentQuery.fragID );
			if( pTagDef && pTagDef->Contains( m_fragmentQuery.tagState.fragmentTags, fragTagStateMatch.fragmentTags ) )
			{
				// TODO: get the charinstance from the scope.
				ICharacterInstance* piCharacterInstance = piActionController->GetEntity().GetCharacter(0);
				const IAnimationSet* piAnimationSet = piCharacterInstance->GetIAnimationSet();

				if( m_fragmentQuery.optionIdx == OPTION_IDX_RANDOM )
				{
					for( uint i=0; i<m_numOptions; ++i )
					{
						const CFragment *pFragment = pAnimDB->GetEntry(m_fragmentQuery.fragID, fragTagStateMatch, i);
						if( pFragment )
						{
							AddFragment( *pFragment, piAnimationSet );
						}
					}
				}
				else if( m_fragmentQuery.optionIdx < m_numOptions )
				{
					const CFragment *pFragment = pAnimDB->GetEntry(m_fragmentQuery.fragID, fragTagStateMatch, m_fragmentQuery.optionIdx);
					if( pFragment )
					{
						AddFragment( *pFragment, piAnimationSet );
					}
				}
			}
		}
	}
	void Release()
	{
		m_animsCached.clear();
		stl::free_container( m_animsCached );
	}

	bool IsLoaded() const
	{
		return( m_animsCached.end() == std::find_if( m_animsCached.begin(), m_animsCached.end(), FPredNotLoaded() ) );
	}

	uint32 GetNumOptions() const { return m_numOptions; }
	uint32 GetCurrentOption() const { return m_fragmentQuery.optionIdx; }

private:

	struct SCacheAnims
	{
		explicit SCacheAnims( const uint32 crc )
			: m_crc(crc)
		{
			gEnv->pCharacterManager->CAF_AddRef( m_crc ) ;
		}
		SCacheAnims( const SCacheAnims& rhs )
			: m_crc(rhs.m_crc)
		{
			gEnv->pCharacterManager->CAF_AddRef( m_crc ) ;
		}
		~SCacheAnims()
		{
			gEnv->pCharacterManager->CAF_Release( m_crc );
		}

		SCacheAnims& operator=( const SCacheAnims& anim )
		{
			if( m_crc != anim.m_crc )
			{
				gEnv->pCharacterManager->CAF_Release( m_crc ) ;
				m_crc = anim.m_crc;
				gEnv->pCharacterManager->CAF_AddRef( m_crc ) ;
			}
			return *this;
		}

		bool IsLoaded() const { return gEnv->pCharacterManager->CAF_IsLoaded( m_crc ); }

		uint32 m_crc;
	};

	struct FPredNotLoaded
	{
		bool operator()( const SCacheAnims& anim ) const { return !anim.IsLoaded(); }
	};

	void AddFragment( const CFragment& fragment, const IAnimationSet* piAnimationSet )
	{
		const DynArray<TAnimClipSequence>& animLayers = fragment.m_animLayers;
		for( int i=0; i<animLayers.size(); ++i )
		{
			const TAnimClipSequence& sequence = animLayers[i];
			for( int j=0; j<sequence.size(); ++j )
			{
				const SAnimClip& animClip = sequence[j];
				const int animID = piAnimationSet->GetAnimIDByCRC( animClip.animation.animRef.crc );
				const uint32 filePathCRC = piAnimationSet->GetFilePathCRCByAnimID( animID );

				const char* pAnimPath = piAnimationSet->GetFilePathByID( animID );

				// CharacterManager needs the filePathCRC
				m_animsCached.push_back( SCacheAnims(filePathCRC) );
			}
		}
	}

	SFragmentQuery m_fragmentQuery;
	uint32 m_numOptions;

	typedef std::vector<SCacheAnims> TAnimsCached;
	TAnimsCached m_animsCached;
};

#include "ICryMannequinUserParams.h"


#endif //__I_CRY_MANNEQUIN_H__
