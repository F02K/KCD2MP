////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2011.
//
////////////////////////////////////////////////////////////////////////////
#ifndef __I_CRY_MANNEQUINDEFS_H__
#define __I_CRY_MANNEQUINDEFS_H__

#define STORE_TAG_STRINGS 1 //TODO: !_RELEASE

// Enable this if the sh*t hits the fan ('only' saves about 15% per ADB currently but makes debugging very hard)
//#if defined(_RELEASE) || defined(XENON) || defined(PS3)
//#define STORE_ANIMNAME_STRINGS 0
//#else
//#define STORE_ANIMNAME_STRINGS 1
//#endif
#define STORE_ANIMNAME_STRINGS 1

#if defined(_RELEASE) || defined(XENON) || defined(PS3)
#define STORE_PROCCLIP_STRINGS 0
#else
#define STORE_PROCCLIP_STRINGS 1
#endif

typedef int32 TagID; // should be uint32, but it's too scary because we regularly check (tagID >= 0) instead of (tagID != TAG_ID_INVALID)
static const TagID TAG_ID_INVALID = ~TagID(0);

typedef int32 TagGroupID; // should be uint32, but it's too scary because we regularly check (tagGroupID >= 0) instead of (tagGroupID != TAG_ID_INVALID)
static const TagGroupID GROUP_ID_NONE = ~TagGroupID(0);

typedef TagID FragmentID; // should be uint32, but it's too scary because we regularly check (fragmentID >= 0) instead of (fragmentID != TAG_ID_INVALID)
static const FragmentID FRAGMENT_ID_INVALID = TAG_ID_INVALID;

static const TagID SCOPE_ID_INVALID = TAG_ID_INVALID;

typedef uint32 AnimCRC; // CRC of the name of the animation 
static const AnimCRC ANIM_CRC_INVALID = AnimCRC(0);

static const uint32 OPTION_IDX_RANDOM = 0xfffffffe;
static const uint32 OPTION_IDX_INVALID = 0xffffffff;

static const uint32 TAG_SET_IDX_INVALID = ~uint32(0);

typedef uint32 ActionScopes;
static const ActionScopes ACTION_SCOPES_ALL = ~ActionScopes(0);
static const ActionScopes ACTION_SCOPES_NONE = ActionScopes(0);

//suggestion: typedef uint32 ScopeContextId;
static const TagID SCOPE_CONTEXT_ID_INVALID = TAG_ID_INVALID;

const uint32 TAG_DEBUG_STRING_LENGTH = 64;
const uint32 ANIMNAME_DEBUG_STRING_LENGTH = 64;


template <uint32 t_stringLength, uint32 t_invalid, uint32 t_storeStrings> struct SCRCRef
{
};


template <uint32 t_stringLength, uint32 t_invalid> struct SCRCRef<t_stringLength, t_invalid, 0>
{
	SCRCRef()
		: crc(t_invalid)
	{
	}

	SCRCRef(const char* nameString)
	{
		SetByString(nameString);
	}

	void SetByString(const char* nameString)
	{
		if (nameString && (nameString[0] != '\0'))
		{
			crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(nameString);
			if (crc == t_invalid)
			{
				CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Congratulations, you hit the jackpot! The string '%s' has a CRC32 equal to %d. Unfortunately this number is reserved for errors, so please rename and try again...", nameString, t_invalid);
			}
		}
		else
		{
			crc = t_invalid;
		}
	}

	ILINE const char *GetString() const
	{
		return "STRIPPED_CRC_NAMES";
	}

	ILINE bool IsEmpty() const
	{
		return (crc == t_invalid);
	}

	uint32 crc;
};


template <uint32 t_stringLength, uint32 t_invalid> struct SCRCRef<t_stringLength, t_invalid, 1>
{
	SCRCRef()
		: crc(t_invalid)
		, stringValue(NULL)
	{
	}

	SCRCRef(const char* nameString)
		: stringValue(NULL)
	{
		SetByString(nameString);
	}

	SCRCRef(const SCRCRef<t_stringLength, t_invalid, 1>& other)
		: stringValue(NULL)
	{
		SetByString(other.stringValue);
	}

	const SCRCRef<t_stringLength, t_invalid, 1>& operator=(const SCRCRef<t_stringLength, t_invalid, 1>& other)
	{
		if (&other != this)
			SetByString(other.stringValue);
		return *this;
	}

	~SCRCRef()
	{
		delete[] stringValue;
	}

	void SetByString(const char* nameString)
	{
		if (nameString && (nameString[0] != '\0'))
		{
			size_t lengthPlusOne = strlen(nameString) + 1;
			delete[] stringValue;
			stringValue = new char[lengthPlusOne];
			memcpy(stringValue, nameString, lengthPlusOne);

			crc = gEnv->pSystem->GetCrc32Gen()->GetCRC32Lowercase(nameString);
			if (crc == t_invalid)
			{
				CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Congratulations, you hit the jackpot! The string '%s' has a CRC32 equal to %d. Unfortunately this number is reserved for errors, so please rename and try again...", nameString, t_invalid);
			}
		}
		else
		{
			delete[] stringValue;
			stringValue = NULL;
			crc = t_invalid;
		}
	}

	ILINE const char *GetString() const
	{
		if (stringValue == NULL)
			return "";
		else
			return stringValue;
	}

	ILINE bool IsEmpty() const
	{
		return (crc == t_invalid);
	}

	uint32 crc;

private:
	char* stringValue;
};

typedef SCRCRef<TAG_DEBUG_STRING_LENGTH, ANIM_CRC_INVALID, STORE_TAG_STRINGS> STagRef;
typedef SCRCRef<ANIMNAME_DEBUG_STRING_LENGTH, ANIM_CRC_INVALID, STORE_ANIMNAME_STRINGS> SAnimRef;

const uint32 DEF_PATH_LENGTH = 512;
typedef SCRCRef<DEF_PATH_LENGTH, 0, 1> TDefPathString;

const uint32 PROC_STRING_LENGTH = 128;
typedef SCRCRef<PROC_STRING_LENGTH, 0, STORE_PROCCLIP_STRINGS> SProcDataCRC; 

typedef CCryName TProcClipString;

enum eSequenceFlags
{
	eSF_Fragment					= BIT(0),
	eSF_TransitionOutro		= BIT(1),
	eSF_Transition				= BIT(2)
};

struct SWeightData
{
	enum EConfig {MAX_WEIGHTS = 4};

	SWeightData() { memset(&weights, 0, MAX_WEIGHTS * sizeof(weights[0])); }

	float weights[MAX_WEIGHTS];
};

#endif //__I_CRY_MANNEQUINDEFS_H__
