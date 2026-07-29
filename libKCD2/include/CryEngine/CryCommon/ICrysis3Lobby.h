#ifndef __ICRYSIS3LOBBY_H__
#define __ICRYSIS3LOBBY_H__

#pragma once

#include <ICryFriendsManagement.h> // <> required for Interfuscator

struct ICrysis3AuthenticationHandler
{
	enum EAuthenticationError
	{
		eAE_Unknown,
		eAE_Success,										// Not an error, was successful.
		eAE_InvalidLoginParams,					// Login : Failed - incorrect email, password or account doesn't exist
		eAE_AccountNotActive,						// Login : Account either not activated yet or has been deactivated/disabled
		eAE_AccountBanned,							// Login : Account has been banned
		eAE_PersonaNotFoundOrInactive,	// Login : Persona not found or has been deactivated
		eAE_AlreadyExists,							// Email Account/Persona already exists
		eAE_InvalidCountry,							// Create : Invalid country given
		eAE_TooYoung,										// Create : User is too young
		eAE_TOSRequired,								// Both : User must accept TOS
		eAE_InvalidEmail,								// Create: Invalid email, i.e. bob@bob.com2
		eAE_NoEntitlement,							// No Entitlement/Product key on this account
		eAE_Timeout,										// RPC timed out
		eAE_Disconnected,								// Disconnected. Cable pull etc.

		eAE_Max
	};

	// For now these map directly to blaze equivalents 1-1
	enum EValidationField
	{
		eVF_Unknown,
		eVF_Password,
		eVF_Email,
		eVF_ParentalEmail,
		eVF_DisplayName,
		eVF_Status,
		eVF_DOB,
		eVF_Token,
		eVF_Expiration,
		eVF_OptInType,
		eVF_Application,
		eVF_Source,

		eVF_Max
	};

	// For now these map directly to blaze equivalents 1-1
	enum EValidationError
	{
		eVE_Unknown,
		eVE_InvalidValue,
		eVE_IllegalValue,
		eVE_MissingValue,
		eVE_DuplicateValue,
		eVE_InvalidEmailDomain,
		eVE_SpacesNotAllowed,
		eVE_TooShort,
		eVE_TooLong,
		eVE_TooYoung,
		eVE_TooOld,
		eVE_IllegalForCountry,
		eVE_BannedCountry,
		eVE_NotAllowed,
		eVE_TokenExpired,
		eVE_OptInMismatch,

		eVE_Max
	};

	// UI Feedback
	struct SValidationError
	{
		EValidationField m_field;
		EValidationError m_error;
	};
	
	const static int s_MaxDisplayNameLength = 32;
	struct SPersona
	{
		SPersona(); 
		SPersona(const char* pDisplayName, const int id):
			m_id(id)
		{
			strncpy_s(m_pdisplayName, s_MaxDisplayNameLength, pDisplayName, _TRUNCATE);
		}

		char m_pdisplayName[s_MaxDisplayNameLength];
		const int m_id;
	};

	// <interfuscator:shuffle>
	virtual ~ICrysis3AuthenticationHandler() {}

	virtual void OnDisplayCountrySelect() = 0;
	virtual void OnDisplayLegalDocs(const char *pTermsOfServiceText, const char *pPrivacyText) = 0;
	virtual void OnDisplayCreateAccount() = 0;
	virtual void OnDisplayLogin() = 0;
	virtual void OnDisplayEntitleGame() = 0;
	virtual void OnDisplayPersonas(const SPersona *pPersonas, const int numPersonas) = 0;
	virtual void OnPasswordRequired() = 0;

	virtual void OnCreateAccountError(EAuthenticationError errorCode, const SValidationError *pErrors, const int numErrors) = 0;
	virtual void OnCreatePersonaError(EAuthenticationError errorCode) = 0;
	virtual void OnForgotPasswordError(EAuthenticationError errorCode) = 0;
	virtual void OnAssociateAccountError(EAuthenticationError errorCode) = 0;
	virtual void OnGeneralError(EAuthenticationError error) = 0;
	virtual void OnLoginFailure(EAuthenticationError error) = 0;

	// Xbox only
	virtual void OnProfileSelected() = 0;
	virtual void OnProfileLoaded(EAuthenticationError errorCode) = 0;
	virtual void OnProfileUnloaded(EAuthenticationError errorCode) = 0;
	// </interfuscator:shuffle>
};

struct SCrysis3SettingsLoadResult
{
	const char *pKey;
	const char *pValue;
};

enum ECrysis3MatchMakingResult
{
	eMR_CreatedGame,
	eMR_JoinedGame,
	eMR_Failed,
};

struct SCrysis3MatchMakingResult
{
	CrySessionID sessionId;
	CrySessionHandle h;
	ECrysis3MatchMakingResult result;
};

typedef uint32 CryPlaygroupHandle;
const CryPlaygroupHandle CryPlaygroupInvalidHandle = 0xffffffff;

struct SCryLobbyPlaygroupEventData
{
	CryPlaygroupHandle			playgroup;
};

struct SCryLobbyPlaygroupUserData
{
	CryPlaygroupHandle			playgroup;
	SCryUserInfoResult			data;
};

struct SPersistentGameId
{
public:
	SPersistentGameId()
	{
		m_id[0] = '\0';
	}

	bool operator==(const SPersistentGameId &rhs) const
	{
		return (strcmp(m_id, rhs.m_id) == 0);
	}

	bool IsValid() const { return (m_id[0] != '\0'); }

	const char* ToString() const { return m_id; }

	void FromString(const char *pString)
	{
		strcpy_s(m_id, 64, pString);
		m_id[63] = '\0';
	}

private:
	char m_id[64];
};

struct SCrysis3PersistentGameIdSearchResult
{
	SCrySessionSearchResult m_result;
	SPersistentGameId m_persistentGameId;
};

#define IC3_JOINED_BY_PLAYGROUP_MAX_ATTRIBUTE_NAME_LENGTH		32
#define IC3_JOINED_BY_PLAYGROUP_MAX_ATTRIBUTE_LENGTH				32
#define IC3_JOINED_BY_PLAYGROUP_MAX_ATTRIBUTES							6

struct SCryLobbyJoinedGameByPlaygroupData
{
	struct SAttribute
	{
		char strName[IC3_JOINED_BY_PLAYGROUP_MAX_ATTRIBUTE_NAME_LENGTH];
		char strAttribute[IC3_JOINED_BY_PLAYGROUP_MAX_ATTRIBUTE_LENGTH];
	};

	SAttribute m_attributes[IC3_JOINED_BY_PLAYGROUP_MAX_ATTRIBUTES];
	int m_numAttributes;

	ECryLobbyError m_error;
	CrySessionHandle m_session;
	bool m_bIsHost;
};

//pre declaration of struct defined below
struct SMatchMakeParams;
struct SPasswordRules;
struct SReceivedFeedItem;
struct SCrysis3GeneralPlayerStats;
struct SCrysis3MiscPlayerStats;
struct SCrysis3ScopedPlayerStats;
struct SCrysis3WeaponStats;
struct SCrysis3PerkStats;
struct SCrysis3KillsByDamageTypeStats;
struct SCrysis3DogtagStats;
struct SCrysis3AssessmentStats;
struct SChallengeInfo;
struct SChallengeXPConfig;
struct SCryStatsLeaderBoardReadResult;
struct SChallengeInfo;
struct SPlaygroupAttributes;
struct SPlaygroupSearchDetails;
struct SDetailedServerInfo;

typedef void (*Crysis3LobbyBasicCallback)(CryLobbyTaskID taskID, ECryLobbyError error, void *arg);
typedef void (*Crysis3LobbySettingsLoadCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3SettingsLoadResult *pResults, uint32 numResults, void *arg);
typedef void (*Crysis3LobbyMatchMakingCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3MatchMakingResult *pResult, void *arg);
typedef void (*Crysis3LobbyPlaygroupCallback)(CryLobbyTaskID taskID, ECryLobbyError error,  CryPlaygroupHandle playgroupHandle, void *arg);
typedef void (*Crysis3LobbySessionCallback)(CryLobbyTaskID taskID, ECryLobbyError error, CrySessionID sessionID, SMatchMakeParams* params, void *arg);
typedef void (*Crysis3LobbyPasswordCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SPasswordRules *pRules, void *arg);
typedef void (*Crysis3LobbyGetFeedCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SReceivedFeedItem* pFeedItems, uint32 numFeedItems, void *arg);
typedef void (*Crysis3LobbyGetChallengesCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SChallengeInfo* pChallenges, uint32 numChallenges, void *arg);
typedef void (*Crysis3LobbyGetChallengesCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SChallengeInfo* pChallenges, uint32 numChallenges, void *arg);
typedef void (*Crysis3LobbyGetChallengeXPCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SChallengeXPConfig* pChallengeXPConfig, void *arg);
typedef void (*Crysis3LobbyFeedPushedCallback)(SReceivedFeedItem* pFeedItems, uint32 numFeedItems, void *arg);
typedef void (*Crysis3GeneralStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3GeneralPlayerStats *pResults, void *arg);
typedef void (*Crysis3MiscStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3MiscPlayerStats *pResults, void *arg);
typedef void (*Crysis3ScopedStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3ScopedPlayerStats *pResults, void *arg);
typedef void (*Crysis3WeaponStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3WeaponStats *pResults, void *arg);
typedef void (*Crysis3KillsByDamageTypeStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3KillsByDamageTypeStats *pResults, void *arg);
typedef void (*Crysis3DogtagStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3DogtagStats *pResults, void *arg);
typedef void (*Crysis3AssessmentStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3AssessmentStats *pResults, void *arg);


typedef void (*Crysis3GeneralStatsHistoryCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3GeneralPlayerStats *pResults, const uint32 numResults, void *arg);
typedef void (*Crysis3ScopedStatsHistoryCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3ScopedPlayerStats *pResults, const uint32 numResults, void *arg);
typedef void (*Crysis3WeaponStatsHistoryCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3WeaponStats *pResults, const uint32 numResults, void *arg);
typedef void (*Crysis3KillsByDamageTypeStatsHistoryCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3KillsByDamageTypeStats *pResults, const uint32 numResults, void *arg);

typedef void (*Crysis3PerkStatsCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3PerkStats *pResults, void *arg);
typedef void (*Crysis3ReadLeaderBoardCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCryStatsLeaderBoardReadResult* pResult, void* pArg);
typedef void (*Crysis3LobbyEntitlementCallback)(CryLobbyTaskID taskID, ECryLobbyError error, bool bHasEntitlement, void *arg);
typedef void (*Crysis3LobbyListEntitlementCallback)(CryLobbyTaskID taskID, ECryLobbyError error, const char* entitlementTag, void *arg);

typedef void (*Crysis3MatchMakingGetPersistentIdCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SPersistentGameId persistentId, void *pArg);
typedef void (*Crysis3MatchMakingSessionPersistentIdSearchCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SCrysis3PersistentGameIdSearchResult* session, void* arg);
typedef void (*Crysis3MatchMakingDetailedServerInfoCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SDetailedServerInfo *pInfo, void *pArg);

typedef void (*Crysis3PlaygroupAttributesCallback)(CryLobbyTaskID taskID, ECryLobbyError error, CryPlaygroupHandle h, SPlaygroupAttributes *pAttributes, void *arg);
typedef void (*Crysis3PlaygroupSearchCallback)(CryLobbyTaskID taskID, ECryLobbyError error, SPlaygroupSearchDetails *pResults, int numResults, void *arg);

//-------------------------------------------------------------------------
struct ICrysis3UserSettings
{
	typedef uint32 CryUserSettingsTaskID;

	// <interfuscator:shuffle>
	virtual ~ICrysis3UserSettings() {}

	virtual ECryLobbyError	SettingsSave(uint32 user, const CryUserID& userId, const char *pKey, const char *pValue, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError	SettingsLoad(uint32 user, const CryUserID& userId, const char *pKey, CryLobbyTaskID* pTaskID, Crysis3LobbySettingsLoadCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError	SettingsLoadAll(uint32 user, const CryUserID& userId, CryLobbyTaskID* pTaskID, Crysis3LobbySettingsLoadCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError	SettingsDelete(uint32 user, const CryUserID& userId, const char *pKey, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual void						CancelTask(CryLobbyTaskID taskID) = 0;
	virtual uint32					GetServerTime() = 0;
	// </interfuscator:shuffle>
};

#define IC3_MAX_COUNTRY_CODE_LENGTH 8
#define IC3_MAX_LANGUAGE_CODE_LENGTH 8
#define IC3_MAX_EMAIL_LENGTH 64
#define IC3_MAX_PASSWORD_LENGTH 64
#define IC3_MAX_PERSONA_NAME_LENGTH 64
#define IC3_MAX_ENTITLE_GAME_KEY_LENGTH 64

struct SDisplayCountrySelectResponse
{
	char pCountry[IC3_MAX_COUNTRY_CODE_LENGTH];

	uint8  birthDay;
	uint8  birthMonth;
	uint16 birthYear; 
};

struct SDisplayLoginResponse
{
	char pEmail[IC3_MAX_EMAIL_LENGTH];
	char pPassword[IC3_MAX_PASSWORD_LENGTH];
};

struct SDisplayEntitleGameResponse
{
	char pGameCDKey[ IC3_MAX_ENTITLE_GAME_KEY_LENGTH]; 
};

struct SDisplayPersonasResponse
{
	char pPersonaName[IC3_MAX_PERSONA_NAME_LENGTH];
};

struct SPasswordRequiredResponse
{
	char pPassword[IC3_MAX_PASSWORD_LENGTH];
};

struct SCreateAccountResponse
{
	char pEmail[IC3_MAX_EMAIL_LENGTH];
	char pPassword[IC3_MAX_PASSWORD_LENGTH];
	char pPersonaName[IC3_MAX_PERSONA_NAME_LENGTH];
	char pLanguage[IC3_MAX_COUNTRY_CODE_LENGTH];
	bool mEaEmailAllowed;
	bool mThirdPartyEmailAllowed;
};

struct SAssociateAccountResponse
{
	char pEmail[IC3_MAX_EMAIL_LENGTH];
	char pPassword[IC3_MAX_PASSWORD_LENGTH];
};

struct SForgotPasswordResponse
{
	char pEmail[IC3_MAX_EMAIL_LENGTH];
};

#define PASSWORD_MAX_NUM_VALID_CHARACTERS (257)

struct SPasswordRules
{
	uint16 minLength;
	uint16 maxLength;
	uint16 minLowerCaseCharacters;
	uint16 minUpperCaseCharacters;
	uint16 minDigits;
	char validCharacters[PASSWORD_MAX_NUM_VALID_CHARACTERS];
};

#define IC3_MATCHMAKING_RULE_MAX_ATTRIBUTE_LENGTH 32
#define IC3_MATCHMAKING_RULE_MAX_THRESHOLD_LENGTH 32

struct SMatchMakeParams
{
	enum EMatchMakingMode
	{
		eMMM_FindGame = BIT(0),
		eMMM_CreateGame = BIT(1),
		eMMM_FindOrCreateGame = eMMM_FindGame | eMMM_CreateGame,
	};

	enum EMatchMakingRule
	{
		eMMR_Playlist,
		eMMR_Variant,
		eMMR_Mode,
		eMMR_Ranked,
		eMMR_Playlist_Variant_Id,
		eMMR_Squad,
		eMMR_NumRules
	};

	enum EMatchMakingPredefinedRule
	{
		eMMPR_HostBalancing,
		eMMPR_HostViability,
		eMMPR_PingSite,
		eMMPR_GeoLocation,
		eMMPR_SkillRule,
		eMMPR_NumPredefinedRules
	};

	struct SMatchMakingRuleGeneric
	{
		char pAttribute[IC3_MATCHMAKING_RULE_MAX_ATTRIBUTE_LENGTH];
		char pThreshold[IC3_MATCHMAKING_RULE_MAX_THRESHOLD_LENGTH];
		bool bEnabled;
	};

	struct SMatchMakingRuleBasic
	{
		char pThreshold[IC3_MATCHMAKING_RULE_MAX_THRESHOLD_LENGTH];
		bool bEnabled;
	};

	struct SMatchMakingRuleGameSize
	{
		char pThreshold[IC3_MATCHMAKING_RULE_MAX_THRESHOLD_LENGTH];
		uint8 min;
		uint8 desired;
		bool bEnabled;
	};

	struct SMatchMakingRuleDNF
	{
		char pThreshold[IC3_MATCHMAKING_RULE_MAX_THRESHOLD_LENGTH];
		uint8 maxPercent;
		bool bEnabled;
	};

	SMatchMakeParams() : numSlots(12), mode(eMMM_FindGame)
	{
		for (int i = 0; i < (int)eMMR_NumRules; ++ i)
		{
			rules[i].bEnabled = false;
			rules[i].pAttribute[0] = '\0';
			rules[i].pThreshold[0] ='\0';
		}

		for( int j = 0; j < (int)eMMPR_NumPredefinedRules; ++j )
		{
			predefinedRules[ j ].bEnabled = false;
			predefinedRules[ j ].pThreshold[ 0 ] ='\0';
		}

		gameSizeRule.pThreshold[ 0 ] = '\0';
		gameSizeRule.min = 0;
		gameSizeRule.desired = 0;
		gameSizeRule.bEnabled = false;

		DNFRule.pThreshold[ 0 ] = '\0';
		DNFRule.maxPercent = 0;
		DNFRule.bEnabled = false;

		duration = 0;
		numSlots = 0;
	}

	SMatchMakingRuleGeneric rules[eMMR_NumRules];
	SMatchMakingRuleBasic		predefinedRules[ eMMPR_NumPredefinedRules ];

	SMatchMakingRuleGameSize	gameSizeRule;
	SMatchMakingRuleDNF				DNFRule;

	EMatchMakingMode	mode;
	uint32						duration;
	uint8 numSlots;
};

struct SGameReport_Game
{
	SGameReport_Game()
		: mapId(0)
		, modeId(0)
	{}

	int32							mapId;
	int32							modeId;
};

struct SGameReport_Weapon
{
	SGameReport_Weapon()
		: pNext(NULL)
		, weaponId(0)
		, headshotKills(0)
		, headshots(0)
		, kills(0)
		, deaths(0)
		, hits(0)
		, shots(0)
		, silencedShots(0)
		, timeUsed(0)
		, usageCount(0)
	{}

	SGameReport_Weapon *pNext;
	uint16 weaponId;
	uint16 headshotKills;
	uint16 headshots;
	uint16 kills;
	uint16 deaths;
	uint16 hits;
	uint16 shots;
	uint16 silencedShots;
	uint16 timeUsed;
	uint16 usageCount;
};

struct SGameReport_Perk
{
	SGameReport_Perk()
		: pNext(NULL)
		, perkId(0)
		,	kills(0)
		,	deaths(0)
		,	timeUsed(0)
		,	usageCount(0)
		,	xpgain(0)
	{}

	SGameReport_Perk *pNext;
	uint16 perkId;
	uint16 kills;
	uint16 deaths;
	uint16 timeUsed;
	uint16 usageCount;
	uint16 xpgain;
};

struct SGameReport_KillsByDamageType
{
	SGameReport_KillsByDamageType()
		: pNext(NULL)
		, kills(0)
		, damageTypeId(0)
	{}

	SGameReport_KillsByDamageType *pNext;
	uint16 damageTypeId;
	uint16 kills;
};

struct SGameReport_PlayerOffline
{
	SGameReport_PlayerOffline()
		: playerId(NULL)
		, xp(0)
		, timePlayed(0)
		, skillrank(0)
		, interactiveKills(0)
		, pWeaponReports(NULL)
		, pPerkReports(NULL)
		, pKillsByDamageTypeReports(NULL)
	{}

	CryUserID playerId;
	uint32		xp;
	uint32		skillrank;
	uint16		interactiveKills;
	uint16		timePlayed;
	uint16		mapId;
	uint16		modeId;
	SGameReport_Weapon*	pWeaponReports;
	SGameReport_Perk*		pPerkReports;	
	SGameReport_KillsByDamageType* pKillsByDamageTypeReports;
};

struct SGameReport_Player
{
	SGameReport_Player()
		: playerId(NULL)
		, kills(0)
		, deaths(0)
		, highscore(0)
		, gamesPlayed(0)
		, gamesWon(0)
		, gamesLost(0)
		, gamesDrawn(0)
		, mvp(0)
	{}

	CryUserID playerId;
	uint16		kills;
	uint16		deaths;
	uint16		highscore;
	uint8			gamesPlayed;
	uint8			gamesWon;
	uint8			gamesLost;
	uint8			gamesDrawn;
	uint8			mvp;
};

struct SDogtagReport
{
	SDogtagReport()
		: pNext( NULL )
		, dogtagID( 0 )
		, dateUnlocked( 0 )
		, timeEquipped( 0 )
		, timesAwarded( 0 )
		, timesCollected( 0 )
	{}

	SDogtagReport* pNext;
	uint32	dogtagID;				//linear ID/Index for dogtag
	uint64	dateUnlocked;		//some date format
	uint32	timeEquipped;		//in some time units
	uint8		timesAwarded;		//from completing awards/assessments. always zero or one?
	uint8		timesCollected;	//from killing people and collecting dogtags
};

struct SSkillAssessmentReport
{
	SSkillAssessmentReport()
		: pNext( NULL )
		, assessmentID( 0 )
		, level( 0 )
		, initialValue( 0 )
	{}

	SSkillAssessmentReport* pNext;
	uint32	assessmentID;		// ID/Index of the assessment
	uint32	level;					// current level
	uint32	initialValue;		// value when the assessment was unlocked
};


//Any Stats removed from this list also need to be removed from the blaze .cfgs and .tdfs
// Instead, add a comment saying they are deprecated, and we will remove them all at once during finalizing
#define CRYSIS3_STATS_UINT32_LIST(f) \
	f(uint32, Num3DogtagsCollected, EIPS_3DogtagsFrom1Player) \
	f(uint32, Num3FastGrenadeKills, EIPS_3FastGrenadeKills) \
	f(uint32, Num5HealthRestoresInOneLife, EIPS_5HealthRestoresInOneLife) \
	f(uint32, AcceleratorKills, EIPS_AcceleratorKills) \
	f(uint32, AirDeathKills, EIPS_AirDeathKills) \
	f(uint32, AirHeadshots, EIPS_AirHeadshots) \
	f(uint32, AirKillKills, EIPS_AirKillKills) \
	f(uint32, AlienGunshipCalls, EIPS_AlienGunship) \
	f(uint32, AlienGunshipKills, EIPS_AlienGunshipKills) \
	f(uint32, AlienTicksExtracted, EIPS_AlienTicksExtracted) \
	f(uint32, AllEnemyDogTagsCollected, EIPS_AllEnemyDogTagsGot) \
	f(uint32, ArmourActivations, EIPS_ArmorActivations) \
	f(uint32, ArmourHits, EIPS_ArmourHits) \
	f(uint32, AssaultDefendingKills, EIPS_AssaultDefendingKills) \
	f(uint32, AssaultKillLastAttack5pc, EIPS_AssaultKillLastAttack5pc) \
	f(uint32, AssaultScopeKills, EIPS_AssaultScopeKills) \
	f(uint32, BlindKills, EIPS_BlindKills) \
	f(uint32, BlindSelf, EIPS_BlindSelf) \
	f(uint32, BlindingKills, EIPS_BlindingKills) \
	f(uint32, BulletPenetrationKills, EIPS_BulletPenetrationKills) \
	f(uint32, CaptureObjectives, EIPS_CaptureObjectives) \
	f(uint32, CarryObjectives, EIPS_CarryObjectives) \
	f(uint32, CloakedReloads, EIPS_CloakedReloads) \
	f(uint32, CloakedWatchNearbyKill, EIPS_CloakedWatchNearbyKill) \
	f(uint32, CompleteOnlineMatches, EIPS_CompleteOnlineMatches) \
	f(uint32, RapidFireKills, EIPS_RapidFireKills) \
	f(uint32, CrashSiteAttackingKills, EIPS_CrashSiteAttackingKills) \
	f(uint32, CrashSiteDefendingKills, EIPS_CrashSiteDefendingKills) \
	f(uint32, CrouchedKills, EIPS_CrouchedKills) \
	f(uint32, CrouchedMeleeKills, EIPS_CrouchedMeleeKills) \
	f(uint32, CrouchingOverCorpses, EIPS_CrouchingOverCorpses) \
	f(uint32, DeathsNoSuit, EIPS_DeathsNoSuit) \
	f(uint32, DeathsSuitArmor, EIPS_DeathsSuitArmor) \
	f(uint32, DeathsSuitDefault, EIPS_DeathsSuitDefault) \
	f(uint32, DeathsSuitStealth, EIPS_DeathsSuitStealth) \
	f(uint32, DetonationDelays, EIPS_DetonationDelays) \
	f(uint32, DogtagsCollected, EIPS_DogtagsCollected) \
	f(uint32, DogtagsUnlocked, EIPS_DogtagsUnlocked) \
	f(uint32, DoubleKills, EIPS_DoubleKills) \
	f(uint32, DualWeaponKills, EIPS_DualWeaponKills) \
	f(uint32, ExtractionDefendingKills, EIPS_ExtractionDefendingKills) \
	f(uint32, FinalIntel5SecRemaining, EIPS_FinalIntel5SecRemaining) \
	f(uint32, FirstBlood, EIPS_FirstBlood) \
	f(uint32, FlagCaptures, EIPS_FlagCaptures) \
	f(uint32, FlagCarrierKills, EIPS_FlagCarrierKills) \
	f(uint32, FlushedKills, EIPS_FlushedKills) \
	f(uint32, FriendlyFires, EIPS_FriendlyFires) \
	f(uint32, GameComplete, EIPS_GameComplete) \
	f(uint32, GameOverAfter2Before4, EIPS_GameOverAfter2Before4) \
	f(uint32, GotYourBackKills, EIPS_GotYourBackKills) \
	f(uint32, GrenadeLauncherKills, EIPS_GrenadeLauncherKills) \
	f(uint32, GrenadeSurvivals, EIPS_GrenadeSurvivals) \
	f(uint32, Groinhits, EIPS_Groinhits) \
	f(uint32, GuardianKills, EIPS_GuardianKills) \
	f(uint32, PointFireEnhanceKills, EIPS_PointFireEnhanceKills) \
	f(uint32, HeadShotKills, EIPS_HeadShotKills) \
	f(uint32, HealthRestore, EIPS_HealthRestore) \
	f(uint32, HologramKills, EIPS_HologramKills) \
	f(uint32, InAirDeaths, EIPS_InAirDeaths) \
	f(uint32, InAirGrenadeKills, EIPS_InAirGrenadeKills) \
	f(uint32, InterventionKills, EIPS_InterventionKills) \
	f(uint32, KickedCarKills, EIPS_KickedCarKills) \
	f(uint32, KillAllAssaultAttackers, EIPS_KillAllAssaultAttackers) \
	f(uint32, KillAssists, EIPS_KillAssists) \
	f(uint32, KillJoyKills, EIPS_KillJoyKills) \
	f(uint32, KilledAllEnemies, EIPS_KilledAllEnemies) \
	f(uint32, KilledAllEnemiesNotDied, EIPS_KilledAllEnemiesNotDied) \
	f(uint32, KillsNoSuit, EIPS_KillsNoSuit) \
	f(uint32, KillsSuitArmor, EIPS_KillsSuitArmor) \
	f(uint32, KillsSuitDefault, EIPS_KillsSuitDefault) \
	f(uint32, KillsSuitStealth, EIPS_KillsSuitStealth) \
	f(uint32, KillsWithoutAssist, EIPS_KillsWithoutAssist) \
	f(uint32, LaserSightKills, EIPS_LaserSightKills) \
	f(uint32, LedgeGrabs, EIPS_LedgeGrabs) \
	f(uint32, LightShotgunKills, EIPS_LightShotgunKills) \
	f(uint32, LoneWolfKills, EIPS_LoneWolfKills) \
	f(uint32, AimEnhanceKills, EIPS_AimEnhanceKills) \
	f(uint32, MeleeDeaths, EIPS_MeleeDeaths) \
	f(uint32, MeleeTakeDownKills, EIPS_MeleeTakeDownKills) \
	f(uint32, MeleeTakeDownKillsNoAmmo, EIPS_MeleeTakeDownKillsNoAmmo) \
	f(uint32, MicrowaveBeam, EIPS_MicrowaveBeam) \
	f(uint32, MicrowaveBeamKills, EIPS_MicrowaveBeamKills) \
	f(uint32, MobilityKills, EIPS_MobilityKills) \
	f(uint32, MountedKills, EIPS_MountedKills) \
	f(uint32, NearDeathExperienceKills, EIPS_NearDeathExperienceKills) \
	f(uint32, NumCloakedVictimKills, EIPS_NumCloakedVictimKills) \
	f(uint32, OneHitArmorMeleeKills, EIPS_OneHitArmorMeleeKills) \
	f(uint32, PhantomKills, EIPS_PhantomKills) \
	f(uint32, PiercingKills, EIPS_PiercingKills) \
	f(uint32, ProximityKills, EIPS_ProximityKills) \
	f(uint32, QuadKills, EIPS_QuadKills) \
	f(uint32, QuinKills, EIPS_QuinKills) \
	f(uint32, RecoveryKills, EIPS_RecoveryKills) \
	f(uint32, ReflexSightKills, EIPS_ReflexSightKills) \
	f(uint32, RetaliationKills, EIPS_RetaliationKills) \
	f(uint32, RipOffMountedWeapon, EIPS_RipOffMountedWeapon) \
	f(uint32, RumbledKills, EIPS_RumbledKills) \
	f(uint32, RunOver, EIPS_RunOver) \
	f(uint32, SafetyInNumbersKills, EIPS_SafetyInNumbersKills) \
	f(uint32, SlidingKills, EIPS_SlidingKills) \
	f(uint32, ShotInBack, EIPS_ShotInBack) \
	f(uint32, ShotsInMyBack, EIPS_ShotsInMyBack) \
	f(uint32, LoadoutProKills, EIPS_LoadoutProKills) \
	f(uint32, SkillKills, EIPS_SkillKills) \
	f(uint32, SnipedFoot, EIPS_SnipedFoot) \
	f(uint32, SniperScopeKills, EIPS_SniperScopeKills) \
	f(uint32, StealthActivations, EIPS_StealthActivations) \
	f(uint32, StealthKills, EIPS_StealthKills) \
	f(uint32, Suicides, EIPS_Suicides) \
	f(uint32, SuicidesByFalling, EIPS_SuicidesByFalling) \
	f(uint32, SuicidesByFallingCollided, EIPS_SuicidesByFallingCollided) \
	f(uint32, SuicidesByFrag, EIPS_SuicidesByFrag) \
	f(uint32, SuitBoost, EIPS_SuitBoost) \
	f(uint32, SuitBoostKills, EIPS_SuitBoostKills) \
	f(uint32, SuppressorKills, EIPS_SuppressorKills) \
	f(uint32, TagAssist, EIPS_TagAssist) \
	f(uint32, TaggedAndBagged, EIPS_TaggedAndBagged) \
	f(uint32, TaggedEntities, EIPS_TaggedEntities) \
	f(uint32, TeamRadar, EIPS_TeamRadar) \
	f(uint32, ThrownObjectKill, EIPS_ThrownObjectKill) \
	f(uint32, TrackerKills, EIPS_TrackerKills) \
	f(uint32, TripleKills, EIPS_TripleKills) \
	f(uint32, UnderTheRadarKills, EIPS_UnderTheRadarKills) \
	f(uint32, UnmountedKills, EIPS_UnmountedKills) \
	f(uint32, VictimOnFinalKillcam, EIPS_VictimOnFinalKillcam) \
	f(uint32, WarBirdKills, EIPS_WarBirdKills) \
	f(uint32, TakeLateFlagCaptureLead, EIPS_TakeLateFlagCaptureLead) \
	f(uint32, WinningKill, EIPS_WinningKill) \
	f(uint32, WinningKillAndFirstBlood, EIPS_WinningKillAndFirstBlood) \
	f(uint32, WonCTFWithoutGivingUpAScore, EIPS_WonCTFWithoutGivingUpAScore) \
	f(uint32, WonExtractDefendingNoGiveUp, EIPS_WonExtractDefendingNoGiveUp) \
	f(uint32, SuitDefaultTime, EIPS_SuitDefaultTime) \
	f(uint32, SuitArmorTime, EIPS_SuitArmorTime) \
	f(uint32, SuitStealthTime, EIPS_SuitStealthTime) \
	f(uint32, NoSuitTime, EIPS_NoSuitTime) \
	f(uint32, VisorActiveTime, EIPS_VisorActiveTime) \
	f(uint32, TimePlayed, EIPS_TimePlayed) \
	f(uint32, TimeCrouched, EIPS_TimeCrouched) \
	f(uint32, CloakedNearEnemy, EIPS_CloakedNearEnemy) \
	f(uint32, RunOnceVersion, EIPS_RunOnceVersion) \
	f(uint32, RunOnceTrackingVersion, EIPS_RunOnceTrackingVersion) \
	\
	f(uint32, ArmourTransferEnergyRestored, EIPS_ArmorTransferEnergyRestored) \
	f(uint32, EnhancedVisorKills, EIPS_EnhancedVisorKillsOrTags) \
	f(uint32, EnemyC4DestroyedWithThreatAwareness, EIPS_C4DestroyedThreatAwareness) \
	f(uint32, HeavyWeaponKills, EIPS_HeavyWeaponKills) \
	f(uint32, LedgeGrabsWithMobilityEnhance, EIPS_LedgeGrabsWithMobEnhance) \
	f(uint32, ScoutKills, EIPS_ScoutKills) \
	f(uint32, ThreatAwarenessEvasions, EIPS_ThreatAwarenessEvasions) \
	f(uint32, WeaponProKillsAfterReload, EIPS_WeaponProKillsAfterReload) \
	f(uint32, XPBonusArmour, EIPS_XPBonusArmor) \
	f(uint32, XPBonusStealth, EIPS_XPBonusStealth) \
	f(uint32, XPBonusPower, EIPS_XPBonusPower) \
	f(uint32, TimeInVTOLs, EIPS_TimeInVTOLs ) \
	f(uint32, MountedVTOLKills, EIPS_MountedVTOLKills) \
	f(uint32, ThrownWeaponKills, EIPS_ThrownWeaponKills) \
	f(uint32, HunterHideAndSeek, EIPS_HunterHideAndSeek) \
	f(uint32, TheHunter, EIPS_TheHunter) \
	\
	f(uint32, TimeAliveStreak, ESIPS_TimeAlive) \
	f(uint32, AssaultAttackersKilledStreak, ESIPS_AssaultAttackersKilled) \
	f(uint32, DeathsStreak, ESIPS_Deaths) \
	f(uint32, GameOverLateAndLowScoreStreak, ESIPS_GameOverLateAndLowScore) \
	f(uint32, HeadshotsStreak, ESIPS_Headshots) \
	f(uint32, HeadshotsPerLifeStreak, ESIPS_HeadshotsPerLife) \
	f(uint32, HealthRestoresPerLifeStreak, ESIPS_HealthRestoresPerLife) \
	f(uint32, KillsStreak, ESIPS_Kills) \
	f(uint32, KillsNoReloadWeapChangeStreak, ESIPS_KillsNoReloadWeapChange) \
	f(uint32, LoseStreak, ESIPS_Lose) \
	f(uint32, MultiKillStreakStreak, ESIPS_MultiKillStreak) \
	f(uint32, OnlineRankedWinStreak, ESIPS_OnlineRankedWin) \
	f(uint32, WinStreak, ESIPS_Win) \
	f(uint32, MeleeKillsThisSessionStreak, ESIPS_MeleeKillsThisSession) \
	f(uint32, HeadshotKillsPerLifeStreak, ESIPS_HeadshotKillsPerLife) \
	f(uint32, HeadshotKillsPerMatchStreak, ESIPS_HeadshotKillsPerMatch) \
	\
	f(uint32, PingerKills, EIPS_PingerKills) \
	f(uint32, ChallengesCompleted, EIPS_ChallengesCompleted) \
	f(uint32, MarineKillsAsHunter, EIPS_MarineKillsAsHunter) \
	f(uint32, RetrieverPerkKills, EIPS_RetrieverPerkKills) \
	\
	f(uint32, SwarmerActivations, EIPS_SwarmerActivations) \
	f(uint32, EMPBlastActivations, EIPS_EMPBlastActivations) \
	f(uint32, AlienGunshipsDestroyed, EIPS_AlienGunshipsDestroyed) \
	f(uint32, HunterKillsAsMarine, EIPS_HunterKillsAsMarine) \
	f(uint32, UnderBarrelTyphoonKills, EIPS_UnderBarrelTyphoonKills) \
	f(uint32, TechScopeKills, EIPS_TechScopeKills) \
	f(uint32, SpearShieldKills, EIPS_SpearShieldKills) \
	f(uint32, AllSpearsCaptured, EIPS_AllSpearsCaptured) \
	f(uint32, EmergencyStat1, EIPS_EmergencyStat1) \
	f(uint32, EmergencyStat2, EIPS_EmergencyStat2) \
	\
	f(uint32, PlayersKilledWithTheirGun, EIPS_PlayersKilledWithTheirGun ) \
	f(uint32, KillsWithTheirThrownPole, EIPS_KillsWithTheirThrownPole ) \
	f(uint32, KillsWithTheirThrownShield, EIPS_KillsWithTheirThrownShield ) \
	f(uint32, KillsWithAllCombatPerks, EIPS_KillsWithAllCombatPerks ) \
	f(uint32, KillsWithAllSupportPerks, EIPS_KillsWithAllSupportPerks ) \
	f(uint32, KillsWithAllWeaponPerks, EIPS_KillsWithAllWeaponPerks ) \
	f(uint32, KillsWithOneOfEachPerks, EIPS_KillsWithOneOfEachPerks ) \
	f(uint32, DoubleKillsWithPole, EIPS_DoubleKillsWithPole ) \
	f(uint32, TripleKillswithPole, EIPS_TripleKillswithPole ) \
	f(uint32, DoubleKillsWithPanel, EIPS_DoubleKillsWithPanel ) \
	f(uint32, TripleKillswithPanel, EIPS_TripleKillswithPanel ) \
	f(uint32, PingersKilled, EIPS_PingersKilled ) \
	f(uint32, PingersKilledEnvWeap, EIPS_PingersKilledEnvWeap ) \
	f(uint32, PingersKilledFromVTOL, EIPS_PingersKilledFromVTOL ) \
	f(uint32, PingersKilledOrbiStrike, EIPS_PingersKilledOrbiStrike ) \
	f(uint32, PingersKilledSwarmer, EIPS_PingersKilledSwarmer ) \
	f(uint32, VTOLsDestroyed, EIPS_VTOLsDestroyed ) \
	f(uint32, VTOLsDestroyedOrbitalStrike, EIPS_VTOLsDestroyedOrbitalStrike ) \
	f(uint32, VTOLsDestroyedPinger, EIPS_VTOLsDestroyedPinger) \
	f(uint32, PlayersInVTOLKilled, EIPS_PlayersInVTOLKilled ) \
	f(uint32, PlayersRippedFromPinger, EIPS_PlayersRippedFromPinger ) \
	f(uint32, TaggedEntireEnemyTeam, EIPS_TaggedEntireEnemyTeam ) \
	f(uint32, PingerAllWeaponKills, EIPS_PingerAllWeaponKills ) \
	f(uint32, EnemyTeamKilledInXSeconds, EIPS_EnemyTeamKilledInXSeconds ) \
	f(uint32, C4AttachedToTeamMateKills, EIPS_C4AttachedToTeamMateKills ) \
	f(uint32, PingerPrimaryKills, EIPS_PingerPrimaryKills ) \
	f(uint32, PingerSecondaryKills, EIPS_PingerSecondaryKills ) \
	f(uint32, PingerPingKills, EIPS_PingerPingKills ) \
	f(uint32, ChallengesCompleteFriend, EIPS_ChallengesCompleteFriend ) \
	f(uint32, ChallengesCompleteLobby, EIPS_ChallengesCompleteLobby ) \
	f(uint32, ChallengesCompleteSquad, EIPS_ChallengesCompleteSquad ) \
	f(uint32, ChallengesCompleteCrynet, EIPS_ChallengesCompleteCrynet ) \
	f(uint32, ChallengesCompletePersonal, EIPS_ChallengesCompletePersonal ) \
	f(uint32, ThrownObjectDoubleKills, EIPS_ThrownObjectDoubleKills ) \
	f(uint32, ThrownObjectDistantKills, EIPS_ThrownObjectDistantKills ) \
	f(uint32, AirStompDoubleKills, EIPS_AirStompDoubleKills ) \
	f(uint32, AirStompTripleKills, EIPS_AirStompTripleKills ) \
	f(uint32, PreKillsStuckExplosiveArrow, EIPS_PreKillsStuckExplosiveArrow ) \
	f(uint32, KillWithShieldStuckWArrow, EIPS_KillWithShieldStuckWArrow ) \
	f(uint32, TimeInPingers, EIPS_TimeInPingers ) \
	f(uint32, EmergencyStat3, EIPS_EmergencyStat3) \
	f(uint32, EmergencyStat4, EIPS_EmergencyStat4) \

#define CRYSIS3_STATS_FLOAT_LIST(f) \
	f(float, CrashSiteHeldTime, EFPS_CrashSiteHeldTime) \
	f(float, DamageDelt, EFPS_DamageDelt) \
	f(float, DamageTaken, EFPS_DamageTaken) \
	f(float, DistanceAir, EFPS_DistanceAir) \
	f(float, DistanceRan, EFPS_DistanceRan) \
	f(float, DistanceSlid, EFPS_DistanceSlid) \
	f(float, DistanceSprint, EFPS_DistanceSprint) \
	f(float, EnergyUsed, EFPS_EnergyUsed) \
	f(float, FallDistance, EFPS_FallDistance) \
	f(float, FlagCarriedTime, EFPS_FlagCarriedTime) \
	f(float, IntelCollectedTime, EFPS_IntelCollectedTime) \
	f(float, KillCamTime, EFPS_KillCamTime) \
	\
	f(float, DistanceSprintWithMobilityEnhance, EFPS_DistanceSprintMobEnhance) \
	f(float, DistanceSwum, EFPS_DistanceSwum )\
	f(float, DamageToFlyers, EFPS_DamageToFlyers ) \
	f(float, WallOfSteel, EFPS_WallOfSteel ) \
	\
	f(float, DistanceAirStreak, ESFPS_DistanceAir) \
	f(float, HeightOnRooftops, ESFPS_HeightOnRooftops)

#define CRYSIS3_STATS_LIST(f) \
	CRYSIS3_STATS_UINT32_LIST(f) \
	CRYSIS3_STATS_FLOAT_LIST(f)



//------------------------------------------
struct SCrysis3GeneralPlayerStats
{
#define CRYSIS3_STATS_INITIALISE_MEMBER_VARIABLES(dataType, name, enumValue) \
	m_##name = (dataType) 0;

#define CRYSIS3_STATS_CREATE_MEMBER_VARIABLES(dataType, name, enumValue) \
	dataType m_##name;

#define CRYSIS3_STATS_CREATE_INC_DIFF(dataType, name, enumValue) \
	outResult.m_##name = (m_##name > originalStats.m_##name) ? (m_##name - originalStats.m_##name) : (dataType) 0;

#define CRYSIS3_STATS_ADD(dataType, name, enumValue) \
	m_##name += rhs.m_##name;

	SCrysis3GeneralPlayerStats()
	{
		CRYSIS3_STATS_LIST(CRYSIS3_STATS_INITIALISE_MEMBER_VARIABLES);
	}

	void CreateDiff(const SCrysis3GeneralPlayerStats &originalStats, SCrysis3GeneralPlayerStats &outResult)
	{
		CRYSIS3_STATS_LIST(CRYSIS3_STATS_CREATE_INC_DIFF);
	}

	SCrysis3GeneralPlayerStats &operator += (const SCrysis3GeneralPlayerStats &rhs)
	{
		CRYSIS3_STATS_LIST(CRYSIS3_STATS_ADD);
		return *this;
	}

	CRYSIS3_STATS_LIST(CRYSIS3_STATS_CREATE_MEMBER_VARIABLES);

#undef CRYSIS3_STATS_INITIALISE_MEMBER_VARIABLES
#undef CRYSIS3_STATS_CREATE_MEMBER_VARIABLES
#undef CRYSIS3_STATS_CREATE_INC_DIFF
#undef CRYSIS3_STATS_ADD
};

//------------------------------------------
struct SCrysis3MiscPlayerStats
{
	SCrysis3MiscPlayerStats()
		: m_dogtag(0)
		, m_overallTime(0)
		, m_lobbyTime(0)
		, m_entitlements(0)
		,	m_rank(0)
		,	m_dogtagUnlocks(0)
		,	m_weaponUnlocks(0)
		,	m_moduleUnlocks(0)
		,	m_skillAssessmentsCompleted(0)
		, m_zeus(0)
		, m_rebootCount(0)
		, m_suitBootXP(0)
	{}

	int64 m_dogtag;			//could be U32 if we move to the new dogtag format
	uint32 m_overallTime;
	uint32 m_lobbyTime;
	uint32 m_entitlements; //not written by SaveC3MiscStats
	uint32 m_rank;
	uint32 m_dogtagUnlocks;
	uint32 m_weaponUnlocks;
	uint32 m_moduleUnlocks;
	uint32 m_skillAssessmentsCompleted;
	uint32 m_zeus;
	uint32 m_rebootCount; // only needs to go to 5
	uint32 m_suitBootXP;
	uint32 m_skillRank; //not written by SaveC3MiscStats
};

#define CRYSIS3_NUM_MAPS 20
#define CRYSIS3_NUM_MODES 12
#define CRYSIS3_NUM_WEAPONS 50
#define CRYSIS3_NUM_PERKS 30
#define CRYSIS3_NUM_DAMAGETYPES 30
#define CRYSIS3_NUM_DOGTAGS 350
#define CRYSIS3_NUM_SKILL_ASSESSMENTS 300

//-------------------------------------------------------------------------
struct SCrysis3ScopedPlayerStats
{
	struct SStats
	{
		SStats()
			: m_xp(0)
			, m_kills(0)
			, m_deaths(0)
			, m_timePlayed(0)
			, m_gamesPlayed(0)
			, m_mvp(0)
			, m_highscore(0)
			, m_gamesWon(0)
			, m_gamesLost(0)
			, m_gamesDrawn(0)
			, m_interactiveKills(0)
		{}

		uint32 m_xp;
		uint32 m_kills;
		uint32 m_deaths;
		uint32 m_timePlayed;
		uint32 m_gamesPlayed;
		uint32 m_mvp;
		uint32 m_highscore;
		uint32 m_gamesWon;
		uint32 m_gamesLost;
		uint32 m_gamesDrawn;
		uint32 m_interactiveKills;
	};

	struct SModeStats
	{
		SStats m_mapStats[CRYSIS3_NUM_MAPS];
	};

	SStats &GetStats(uint32 modeId, uint32 mapId)
	{
#ifndef _RELEASE
		if ((modeId < 1) || (modeId > CRYSIS3_NUM_MODES) || (mapId < 1) || (mapId > CRYSIS3_NUM_MAPS))
		{
			CryFatalError("Out of bounds");
		}
#endif
		CRY_ASSERT((modeId > 0) && (modeId <= CRYSIS3_NUM_MODES) && (mapId > 0) && (mapId <= CRYSIS3_NUM_MAPS));
		// -1 as our ids start from 1 but indexes start from 0
		return m_modeStats[modeId - 1].m_mapStats[mapId - 1];
	}

	SModeStats m_modeStats[CRYSIS3_NUM_MODES];
	SStats m_totals;
};

struct SCrysis3WeaponStats
{
	struct SStats
	{
		SStats()
			: m_headShotKills(0)
			, m_headShots(0)
			, m_kills(0)
			, m_deaths(0)
			, m_hits(0)
			, m_shots(0)
			, m_silencedShots(0)
			, m_time(0)
			, m_usageCount(0)
		{}

		uint32 m_headShotKills;
		uint32 m_headShots;
		uint32 m_kills;
		uint32 m_deaths;
		uint32 m_hits;
		uint32 m_shots;
		uint32 m_silencedShots;
		uint32 m_time;
		uint32 m_usageCount;

		static uint32 GetExpectedSize(){return 9;};
	};

	SStats &GetStats(uint32 weaponId)
	{
#ifndef _RELEASE
		if ((weaponId < 1) || (weaponId > CRYSIS3_NUM_WEAPONS))
		{
			CryFatalError("Out of bounds");
		}
#endif
		CRY_ASSERT((weaponId > 0) && (weaponId <= CRYSIS3_NUM_WEAPONS));
		// -1 as our ids start from 1 but indexes start from 0
		return m_stats[weaponId - 1];
	}

	SStats m_stats[CRYSIS3_NUM_WEAPONS];
	SStats m_totals;
};


struct SCrysis3KillsByDamageTypeStats
{
	struct SStats
	{
		SStats()
			: m_kills(0)
		{}

		uint32 m_kills;

		static uint32 GetExpectedSize(){return 1;};
	};

	SStats &GetStats(uint32 damageTypeId)
	{
#ifndef _RELEASE
		if ((damageTypeId < 1) || (damageTypeId > CRYSIS3_NUM_DAMAGETYPES))
		{
			CryFatalError("Out of bounds");
		}
#endif
		CRY_ASSERT((damageTypeId > 0) && (damageTypeId <= CRYSIS3_NUM_DAMAGETYPES));
		// -1 as our ids start from 1 but indexes start from 0
		return m_stats[damageTypeId - 1];
	}

	SStats m_stats[CRYSIS3_NUM_DAMAGETYPES];
	SStats m_totals;
};

struct SCrysis3DogtagStats
{
	struct SStats
	{
		SStats()
			: m_dateUnlocked(0)
			, m_timeEquipped(0)		
			, m_timesAwarded(0)
			, m_timesCollected(0)
		{}

		uint64	m_dateUnlocked;		//some date format
		uint32	m_timeEquipped;		//in some time units
		uint32	m_timesAwarded;		//from completing awards/assessments. always zero or one?
		uint32	m_timesCollected;	//from killing people and collecting dogtags

		static uint32 GetExpectedSize(){return 4;};
	};

	SStats& GetStatsById( uint32 dogtagIndex )
	{
#ifndef _RELEASE
		if( dogtagIndex > CRYSIS3_NUM_DOGTAGS )
		{
			CryFatalError("Out of bounds");
		}
#endif
		CRY_ASSERT( dogtagIndex <= CRYSIS3_NUM_DOGTAGS && dogtagIndex > 0 );
		// -1 as our ids start from 1 but indexes start from 0
		return m_dogtags[ dogtagIndex-1 ];
	}

	SStats m_dogtags[ CRYSIS3_NUM_DOGTAGS ];
};

struct SCrysis3AssessmentStats
{
	struct SStats
	{
		SStats()
			: m_level(0)
			, m_initialValue(0)		
		{}

		uint32	m_level;					//current level. Should only be read for other player's profiles
		uint32	m_initialValue;		//value of stat when skill assessment was unlocked. subtracted from stat to get progress
	
		static uint32 GetExpectedSize(){return 2;};
	};

	SStats& GetStatsById( uint32 assessIndex )
	{
#ifndef _RELEASE
		if( assessIndex > CRYSIS3_NUM_SKILL_ASSESSMENTS )
		{
			CryFatalError("Out of bounds");
		}
#endif
		CRY_ASSERT( assessIndex <= CRYSIS3_NUM_SKILL_ASSESSMENTS && assessIndex > 0 );

		// -1 as our ids start from 1 but indexes start from 0
		return m_assessments[ assessIndex-1 ];
	}

	SStats m_assessments[ CRYSIS3_NUM_SKILL_ASSESSMENTS ];
};


struct SCrysis3PerkStats
{
	struct SStats
	{
		SStats()
			: m_kills(0)
			, m_deaths(0)
			, m_time(0)
			, m_usageCount(0)
			, m_xpGain(0)
		{}

		uint32 m_kills;
		uint32 m_deaths;
		uint32 m_time;
		uint32 m_usageCount;
		uint32 m_xpGain;

		static uint32 GetExpectedSize(){return 5;};
	};

	SStats &GetStats(uint32 perkId)
	{
#ifndef _RELEASE
		if ((perkId < 1) || (perkId > CRYSIS3_NUM_PERKS))
		{
			CryFatalError("Out of bounds");
		}
#endif
		CRY_ASSERT((perkId > 0) && (perkId <= CRYSIS3_NUM_PERKS));
		// -1 as our ids start from 1 but indexes start from 0
		return m_stats[perkId - 1];
	}

	SStats m_stats[CRYSIS3_NUM_PERKS];
	SStats m_totals;
};

//------------------------------------------------------------------------
struct SCensusData
{
	struct SElement
	{
		char	strValue[ IC3_MATCHMAKING_RULE_MAX_ATTRIBUTE_LENGTH ];
		int		intValue;
		int		nGames;
		int		nPlayers;
	};

	struct SAttribute
	{
		char	name[ IC3_MATCHMAKING_RULE_MAX_ATTRIBUTE_LENGTH ];
		DynArray< SElement > elements;
	};

	DynArray< SAttribute > attributes;

	int	connectedClients;
	int	activeGames;
	int	inGameClients;
	int	matchmakingSessions;

	CTimeValue lastUpdated;

};

//-------------------------------------------------------------------------
#define IC3_PLAYGROUP_MAX_ATTRIBUTE_NAME_LENGTH 8
#define IC3_PLAYGROUP_MAX_ATTRIBUTE_LENGTH 32
#define IC3_PLAYGROUP_MAX_ATTRIBUTES 8

struct SPlaygroupAttributes
{
	struct SAttribute
	{
		char strName[IC3_PLAYGROUP_MAX_ATTRIBUTE_NAME_LENGTH];
		char strAttribute[IC3_PLAYGROUP_MAX_ATTRIBUTE_LENGTH];
	};

	SAttribute attributes[IC3_PLAYGROUP_MAX_ATTRIBUTES];
	int numAttributes;
};

//-------------------------------------------------------------------------
struct SPlaygroupSearchDetails
{
	SPlaygroupAttributes attributes;
	CryUserID leaderId;			// User in charge of this playgroup
	CryUserID userId;				// User that we searched for to find this playgroup
};

#define DETAILED_SESSION_INFO_MOTD_SIZE (256)
#define DETAILED_SESSION_INFO_URL_SIZE	(256)
#define DETAILED_SESSION_MAX_CUSTOMS		(32)

// Can't use MAX_PLAYER_LIMIT define since it only exists inside game dll
#if defined(XENON) || defined(PS3)
	#define DETAILED_SESSION_MAX_PLAYERS		(12)
#else
	#define DETAILED_SESSION_MAX_PLAYERS		(16)
#endif

//-------------------------------------------------------------------------
struct SDetailedServerInfo
{
	char		m_motd[DETAILED_SESSION_INFO_MOTD_SIZE];
	char		m_url[DETAILED_SESSION_INFO_URL_SIZE];
	char		m_names[DETAILED_SESSION_MAX_PLAYERS][CRYLOBBY_USER_NAME_LENGTH];
	uint16	m_customs[DETAILED_SESSION_MAX_CUSTOMS];
	uint16	m_namesCount;
	uint16	m_customsCount;
};

//-------------------------------------------------------------------------
struct SGameModeDetails
{
	enum EActiveType
	{
		eAT_Disabled,
		eAT_Ranked,
		eAT_UnRanked,
	};

	const char *pName;
	EActiveType activeType;
};

//-------------------------------------------------------------------------
struct ICrysis3MatchMaking
{
	enum ENetworkHostType
	{
		eNHT_Peer,
		eNHT_DedicatedServer,
		eNHT_Unknown,
	};

	// <interfuscator:shuffle>
	virtual ~ICrysis3MatchMaking() {}
	
	virtual ECryLobbyError MatchMake(const SMatchMakeParams &params, SGameModeDetails *pDisallowedModes, int numDisallowedModes, CryLobbyTaskID* pTaskID, Crysis3LobbyMatchMakingCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError ReportGame(CrySessionHandle gh, SGameReport_Game *pGameReport, SGameReport_Player *pPlayerReports, uint32 numPlayerReports, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError ReplayGame(CrySessionHandle gh, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError SetupGameAttributes(CrySessionHandle gh, uint16 modeId, uint16 mapId, uint16 variantId, bool ranked, int maxNewSquadSize, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError FindSessionByUserID( CryUserID userId, CryLobbyTaskID* taskId, Crysis3LobbySessionCallback cb, void *cbArg ) = 0;

	virtual ENetworkHostType GetGameTopologyType(CrySessionHandle gh) = 0;
	virtual void SetGameProtocolVersion(uint32 protocolVersion) = 0;
	virtual void LeaveGameQueue() = 0;

	virtual ECryLobbyError SessionPersistentIdsSearch(SPersistentGameId* pIds, int numIds, CryLobbyTaskID* taskID, Crysis3MatchMakingSessionPersistentIdSearchCallback cb, void* cbArg) = 0;
	virtual ECryLobbyError SessionGetPersistentId(CrySessionID id, CryLobbyTaskID* taskID, Crysis3MatchMakingGetPersistentIdCallback pCb, void *pCbArg) = 0;

	virtual ECryLobbyError SessionSetDetailedInfo(CrySessionHandle gh, SDetailedServerInfo *pDetailedInfo, CryLobbyTaskID* taskID, Crysis3LobbyBasicCallback pCb, void *pCbArg) = 0;
	virtual ECryLobbyError SessionGetDetailedInfo(CrySessionID id, CryLobbyTaskID* taskID, Crysis3MatchMakingDetailedServerInfoCallback pCb, void *pCbArg) = 0;
	
	virtual ECryLobbyError SessionSetGameModes(CrySessionHandle gh, SGameModeDetails *pModes, int numModes, CryLobbyTaskID* taskId, Crysis3LobbyBasicCallback pCb, void *pCbArg) = 0;

	virtual ECryLobbyError ReportLoss(CrySessionHandle gh, int modeId, int mapId, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	// </interfuscator:shuffle>
};

//-------------------------------------------------------------------------
struct ICrysis3Playgroups
{
	typedef uint32 CryPlaygroupsTaskID;

	// <interfuscator:shuffle>
	virtual ~ICrysis3Playgroups() {}

	virtual ECryLobbyError CreatePlaygroup( const char* name, uint maxMembers, bool enableVoip, bool privateGroup, CryLobbyTaskID* pTaskID, Crysis3LobbyPlaygroupCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError JoinPlaygroup( uint32* userIndex, CryUserID inviteUserId, CryLobbyTaskID* pTaskId, Crysis3LobbyPlaygroupCallback cb, void *cbArg ) = 0; 
	virtual ECryLobbyError LeavePlaygroup( CryPlaygroupHandle playgroupHandle, CryLobbyTaskID* taskId, Crysis3LobbyBasicCallback cb, void *cbArg ) = 0;

	virtual ECryLobbyError JoinPlaygroupByUsername( uint32* userIndex, const char* pPlayerName, CryLobbyTaskID* taskId, Crysis3LobbyPlaygroupCallback cb, void *cbArg ) = 0;
	virtual ECryLobbyError KickMemberFromPlaygroup( CryPlaygroupHandle playgroupHandle, CryUserID member, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback pCb, void *pCbArg ) = 0;

	virtual bool IsUserInPlaygroup( CryPlaygroupHandle handle, CryUserID userId ) = 0;

	virtual SCryMatchMakingConnectionUID GetLeaderConnectionUID( CryPlaygroupHandle playgroupHandle ) = 0;

	virtual ECryLobbyError SetAttributes(CryPlaygroupHandle h, SPlaygroupAttributes *pAttributes, CryLobbyTaskID *pTaskId, Crysis3LobbyBasicCallback pCb, void *pCbArg) = 0;
	virtual ECryLobbyError GetAttributes(CryPlaygroupHandle h, CryLobbyTaskID *pTaskId, Crysis3PlaygroupAttributesCallback pCb, void *pCbArg) = 0;

	virtual ECryLobbyError GetPlaygroupDetailsByUsers(CryUserID *pUserIds, int numUsers, CryLobbyTaskID *pTaskId, Crysis3PlaygroupSearchCallback pCb, void *pCbArg) = 0;
	// </interfuscator:shuffle>
};

//-------------------------------------------------------------------------
#define IC3_MAX_FEEDSTR_LENGTH 512
#define IC3_MAX_USERDATAS 8

enum EFeedItemType
{
	eItemType_Unknown = -1,
	eItemType_Broadcast = 0,	// broadcast type *must* be kept as 0 
	eItemType_Promotion,					
	eItemType_Accomplishment,	
	eItemType_Unlock,
	eItemType_Max
};

#define IC3_MAX_PARAMSTRS 8
#define IC3_MAX_FEEDPARAMSTR_LENGTH 48

struct SReceivedFeedItem
{
	SReceivedFeedItem()
	{
		Clear(); 
	}

	void Clear()
	{
		creatorId = CryUserInvalidID;
		memset(pString,0,IC3_MAX_FEEDSTR_LENGTH);
		memset(mUserData,0,IC3_MAX_USERDATAS);	
		numUserData  = 0;
		creationTime = 0;
		type = eItemType_Unknown;
	}

	char pString[IC3_MAX_FEEDSTR_LENGTH];
	uint32 mUserData[IC3_MAX_USERDATAS];
	int numUserData; 
	int creationTime;
	CryUserID creatorId;  
	EFeedItemType type; 

#ifndef _RELEASE
	void DumpInfo() const
	{
		CryLogAlways("[pString]: %s", pString ? pString: "NULL");
		CryLogAlways("[NumUserData]: %d", numUserData);
		for(int i = 0; i < numUserData; ++i)		
		{
			CryLogAlways("[UserData %d]: %d", i, mUserData[i]);
		}
		CryLogAlways("[type]: %s", type == eItemType_Broadcast ? "BROADCAST" : type == eItemType_Accomplishment ? "ChallengeCompleted" : type == eItemType_Promotion ? "PROMOTION" :  type ==  eItemType_Unlock ? "Unlock" :"UNKNOWN");
	}
#endif //#ifndef _RELEASE

};

#define IC3_MAX_IMAGEURL_LENGTH 128
#define IC3_MAX_STRID_LENGTH 32

struct SRequestedFeedItem
{
	SRequestedFeedItem()
	{
		Clear(); 
	}

	// Handy wrappers for adding various string params
	bool AddStringParam(const char* pStringParam)
	{
		if(numStrParams < IC3_MAX_PARAMSTRS)
		{
			char* pDest = &stringParams[numStrParams*IC3_MAX_FEEDPARAMSTR_LENGTH]; 
			strncpy_s(pDest, IC3_MAX_FEEDPARAMSTR_LENGTH, pStringParam,_TRUNCATE);
			++numStrParams; 
			return true;
		}
		return false; 
	}

	bool AddStringParam(const int intStringParam)
	{
		if(numStrParams < IC3_MAX_PARAMSTRS)
		{
			char* pDest = &stringParams[numStrParams*IC3_MAX_FEEDPARAMSTR_LENGTH]; 
			sprintf_s(pDest, IC3_MAX_FEEDPARAMSTR_LENGTH, "%d",intStringParam);
			++numStrParams; 
			return true;
		}
		return false; 
	}

	const char* GetStringParam(int index) const
	{
		return &stringParams[ min(index,IC3_MAX_PARAMSTRS-1) * IC3_MAX_FEEDPARAMSTR_LENGTH];
	}

	void SetStringId(const char* pStringId)
	{
		char* pDest = stringId; 
		strncpy_s(pDest, IC3_MAX_STRID_LENGTH, pStringId, _TRUNCATE);
	}

	void SetType(EFeedItemType newType)
	{
		type = newType; 
	}

	bool AddUserData(uint32 iParam)
	{
		if(numUserDatas < IC3_MAX_USERDATAS)
		{
			pUserDatas[numUserDatas] = iParam; 
			++numUserDatas; 
			return true; 
		}
		return false; 
	}

	uint32 GetUserData(int index) const
	{
		return pUserDatas[ min(index,IC3_MAX_USERDATAS-1)];
	}

	void Clear()
	{
		numStrParams = 0; 
		numUserDatas = 0; 
		type		 = eItemType_Unknown;
	  memset(stringId,0,IC3_MAX_STRID_LENGTH);
		memset(stringParams,0,IC3_MAX_FEEDPARAMSTR_LENGTH * IC3_MAX_PARAMSTRS); 	
		memset(pUserDatas,0,IC3_MAX_USERDATAS); 
	}

	// Data
	char stringId[IC3_MAX_STRID_LENGTH];			// e.g. "@pl_promotion	  
	
	char stringParams [IC3_MAX_FEEDPARAMSTR_LENGTH * IC3_MAX_PARAMSTRS];  // e.g. <str>"$128471"</str> was promoted to level <str>"8"</str> 
	int	 numStrParams; 

	uint32 pUserDatas[IC3_MAX_USERDATAS];			// e.g. DogtagId = <int>11274</int>
	int			  numUserDatas;

	EFeedItemType type;							    // e.g. eItemType_Promotion

};

// Requests
struct ICrysis3FeedProvider
{
	typedef uint32 CryFeedProviderTaskID;

	// <interfuscator:shuffle>
	virtual ~ICrysis3FeedProvider() {}		

	// Config // 	
	virtual void SetMemLimit(uint32 limitInBytes) = 0; 

	// Requests // 
	virtual ECryLobbyError Subscribe(const bool bSubscribe, uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, Crysis3LobbyFeedPushedCallback onFeedPushCb, void *cbArg)		= 0;
	virtual ECryLobbyError GetFeed(uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyGetFeedCallback cb, void *cbArg)								= 0;
	virtual ECryLobbyError AddFeedItem(const SRequestedFeedItem& feedItem, uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg)	= 0;

	virtual void					 CancelTask(CryLobbyTaskID taskId) = 0;
	// </interfuscator:shuffle>
};


static const uint32 TELEM_MAX_ADDRESS_LEN = 65;
static const uint32 TELEM_MAX_KEY_LEN = 513;
static const uint32 TELEM_MAX_DISABLE_LEN = 1025;
static const uint32 TELEM_MAX_NOTOGGLEOK_LEN = 1025;
static const uint32 TELEM_MAX_FILTER_LEN = 1025;
static const uint32 TELEM_MAX_SESSIONID_LEN = 33;

static const uint32 TELEM_MAX_HEX_KEY_LEN = (TELEM_MAX_KEY_LEN+1)*2;

class CTelemetryToken
{
public:
	CTelemetryToken(): m_port( 0 ), m_sendDelay( 0 ), m_sendPercentage( 0 ), m_locale( 0 ), m_anonymous( false ), m_optin( false ), m_use( false ){}
	~CTelemetryToken() {}

	void Init( const char* address, uint32 port, const char* key, const char* disableRegs, const char* noToggleRegs,  const char* filter, const char* sessionID, uint32 sendDelay, uint32 sendPercentage, uint32 locale, bool anonymous, bool optIn )
	{
		m_address = address;
		m_port = port;
		m_key = key;
		m_disabledRegions = disableRegs;
		m_noToggleRegions = noToggleRegs;
		m_filter = filter;
		m_sessionID = sessionID;
		m_sendDelay = sendDelay;
		m_sendPercentage = sendPercentage;
		m_locale = locale;
		m_anonymous = anonymous;
		m_optin = optIn;
		m_use = true;
	}

	CryFixedStringT<TELEM_MAX_ADDRESS_LEN>	m_address;
	CryFixedStringT<TELEM_MAX_HEX_KEY_LEN> m_key;
	CryFixedStringT<TELEM_MAX_FILTER_LEN> m_filter;
	CryFixedStringT<TELEM_MAX_DISABLE_LEN> m_disabledRegions;
	CryFixedStringT<TELEM_MAX_NOTOGGLEOK_LEN> m_noToggleRegions;
	CryFixedStringT<TELEM_MAX_SESSIONID_LEN> m_sessionID;

	uint32 m_port;
	uint32 m_sendDelay;
	uint32 m_sendPercentage;
	uint32 m_locale;
	bool m_optin;
	bool m_anonymous;
	bool m_use;
};

// collect and send telemetry that our partner(s) require
struct ICrysis3PartnerTelemetry
{
	typedef int TelemetryElementID;	//should be constructed like 'ABCD'

	enum EGameDifficulty
	{
		eGD_None,
		eGD_Easy,
		eGD_Normal,
		eGD_Hard,
		eGD_VeryHard,
		eGD_Expert,
	};

	//career time is cumulative campaign time. 
	//game time is amount of time playing game since mode select (no including paused or idle)
	//session time is amount of time since mode select (including paused and idle)
	//give all times in seconds
	typedef void (*Crysis3TelemClocksCallback)( int& careerTime, int& gameTime, int& sessionTime, void* pArg );
	typedef void (*Crysis3TelemTokenCallback)( const CTelemetryToken& token, void* pArg );

	// <interfuscator:shuffle>
	virtual ~ICrysis3PartnerTelemetry() {}

	virtual void RegisterGetClocksCallback( Crysis3TelemClocksCallback cb, void* pArg ) = 0;
	virtual void RegisterTelemTokenRecievedCallback( Crysis3TelemTokenCallback cb, void* pArg ) = 0;

	virtual ECryLobbyError InitWithTelemToken( const CTelemetryToken& token ) = 0;
	virtual ECryLobbyError InitFromBlazeServer() = 0;

	virtual ECryLobbyError BootSessionEvent( const char* version = NULL, const char* edition = NULL ) = 0;
	virtual ECryLobbyError StartGameSessionEvent( EGameDifficulty diff, bool multiPlayer, const char* map, const char* modeName, CrySessionHandle h  ) = 0;
	virtual ECryLobbyError EndGameSessionEvent() = 0;
	virtual ECryLobbyError MilestoneEvent( EGameDifficulty diff, bool multiPlayer, int milestoneID ) = 0;
	virtual ECryLobbyError AchievementEvent( EGameDifficulty diff, bool multiPlayer, int achievementID ) = 0;

	//bespoke events for C3
	virtual ECryLobbyError VotingClosedEvent( const char* mapA, const char* modeA, int votesA, const char* mapB, const char* modeB, int votesB, int nInLobby, int amHost, int ranked ) = 0;
	virtual ECryLobbyError MPProgressionEvent( int rank, int reboots ) = 0;

	virtual ECryLobbyError GeneralMatchEvent( int dogtags, int kills, int deaths, int suicides, int vtolTime, int pingerTime, int xp ) = 0; 
	virtual ECryLobbyError ChallengeEvent( bool success, int id, const char* category, int slot, int target, bool friendChallenge, int matchesAllowed, int matchesTaken ) = 0;

	virtual ECryLobbyError CampaignCompleted() = 0;

	virtual ECryLobbyError OnShutdown() = 0;
	// </interfuscator:shuffle>
};

//-------------------------------------------------------------------------
struct ICrysis3Stats
{
	// <interfuscator:shuffle>
	virtual ~ICrysis3Stats() {}

	virtual ECryLobbyError LoadC3GeneralStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3GeneralStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError SaveC3GeneralStats(uint32 userIndex, SCrysis3GeneralPlayerStats *pStats, CryLobbyTaskID *pTaskId, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;

	virtual ECryLobbyError LoadC3MiscStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3MiscStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError SaveC3MiscStats(uint32 userIndex, SCrysis3MiscPlayerStats *pStats, CryLobbyTaskID *pTaskId, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	
	virtual ECryLobbyError SaveC3Entitlements(uint32 userIndex , uint32 entitlementBitfield, uint32 xpToAward, CryLobbyTaskID *pTaskId,  Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError SuitRebootC3(uint32 userIndex , uint32 currentXP, CryLobbyTaskID *pTaskId,  Crysis3LobbyBasicCallback cb, void *cbArg) = 0;

	virtual ECryLobbyError SavePlayerOfflineStats(uint32 userIndex, SGameReport_PlayerOffline* pReport, CryLobbyTaskID *pTaskId, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;

	virtual ECryLobbyError SaveUpdateDogtags(uint32 userIndex, SDogtagReport* pReport, CryLobbyTaskID* pTaskId, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;

	virtual ECryLobbyError SaveAssessments( uint32 userIndex, SSkillAssessmentReport* pReport, CryLobbyTaskID* pTaskId, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;

	virtual ECryLobbyError LoadC3ScopedStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3ScopedStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3WeaponStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3WeaponStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3PerkStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3PerkStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3KillsByDamageTypeStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3KillsByDamageTypeStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3DogtagStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3DogtagStatsCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3AssessmentStats(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3AssessmentStatsCallback cb, void *cbArg) = 0;


	// Game history (just necessary history stats for now)
	virtual ECryLobbyError LoadC3GeneralStatsHistory(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3GeneralStatsHistoryCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3ScopedStatsHistory(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3ScopedStatsHistoryCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3WeaponStatsHistory(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3WeaponStatsHistoryCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError LoadC3KillsByDamageTypeStatsHistory(CryUserID userId, CryLobbyTaskID *pTaskId, Crysis3KillsByDamageTypeStatsHistoryCallback cb, void *cbArg) = 0;

	virtual ECryLobbyError ReadLeaderBoardByRankForRange(const char *pBoardName, uint32 startRank, uint32 num, CryLobbyTaskID* pTaskID, Crysis3ReadLeaderBoardCallback cb, void* pCbArg) = 0;
	virtual ECryLobbyError ReadLeaderBoardByRankForUser(const char *pBoardName, uint32 user, uint32 num, CryLobbyTaskID* pTaskID, Crysis3ReadLeaderBoardCallback cb, void* pCbArg) = 0;
	virtual ECryLobbyError ReadLeaderBoardByUserID(const char *pBoardName, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, Crysis3ReadLeaderBoardCallback cb, void* pCbArg) = 0;

	virtual void					 CancelTask(CryLobbyTaskID taskId) = 0;
	// </interfuscator:shuffle>
};


//-------------------------------------------------------------------------
// Used to describe a challenge assigned externally (e.g. friend challenges..)
typedef uint64 TServerChallengeUID; 
struct SChallengeInfo
{

	SChallengeInfo()
	{
		// default to easily recognizable invalid data	
		m_serverUID			= 0;  
		m_senderUID			= CryUserInvalidID;		
		m_challenge_id		= 0;  		
		m_target			= 255;		
		m_numCompletions	= 255;
	}

	~SChallengeInfo(){};

	TServerChallengeUID  m_serverUID; // UID used to refer to this challenge in the server database
	CryUserID m_senderUID;	// The issuing friend UID 
	uint32  m_challenge_id;  // gameside challenge id (CRC based UID)
	uint8  m_target;		// The target score for this challenge (e.g. '5' for 5 headshots etc)
	uint8  m_numCompletions;// The 'bounce count' e.g. > 1 means this is a rechallenge
	uint8	 m_subCategory; // Integer subcategory e.g. 1 == 'skill' 2 == 'Weapon' etc
	// m_pubDate			// Could time-sorted if desired. 
};

enum EChallengeDifficulty
{
	eCD_Easy = 0,
	eCD_Normal,
	eCD_Hard,
	eCD_Max,
};

// Store xp for challenge type at different difficulty levels
struct SChallengeXPConfig
{
	SChallengeXPConfig()
	{

		const int defaultXP = 0;		
		for(int i = 0; i < static_cast<int>(eCD_Max); ++i)
		{
			baseSocialXP[i] = defaultXP;
			baseChanceXP[i] = defaultXP;
			baseLobbyXP[i]  = defaultXP;
			baseSquadXP[i]  = defaultXP;
		}
	}

	uint16 baseSocialXP[eCD_Max];
	uint16 baseChanceXP[eCD_Max];
	uint16 baseLobbyXP[eCD_Max];
	uint16 baseSquadXP[eCD_Max];
};

struct SErrorInfo
{
	SErrorInfo()
	{
		m_errorCode = 0; 
		m_strError = "default"; 
		m_timeStampSecs = gEnv->pTimer->GetFrameStartTime().GetSeconds(); 
	}
	SErrorInfo(int errorCode, const char* errorStr)
	{
		m_errorCode = errorCode; 
		m_strError = errorStr; 
		m_timeStampSecs = gEnv->pTimer->GetFrameStartTime().GetSeconds(); 
	}
	// Timestamp?
	int	m_errorCode; 
	CryFixedStringT<48> m_strError;
	float m_timeStampSecs; 
};
//-------------------------------------------------------------------------

struct ICrysis3ChallengeSystem
{
	typedef uint32 CryChallengeSystemTaskID;

	// <interfuscator:shuffle>
	virtual ~ICrysis3ChallengeSystem() {}		

	virtual ECryLobbyError GetChallenges(uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyGetChallengesCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError AddChallenge(const SChallengeInfo& challenge, uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError RemoveChallenge(const TServerChallengeUID challengeServerUID, uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError CompletedChallenge(const SChallengeInfo& challenge, int targetAchieved, uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void *cbArg) = 0;
	virtual ECryLobbyError RequestChallengeConfig(uint32 user, CryLobbyTaskID* pTaskID, Crysis3LobbyGetChallengeXPCallback cb, void *cbArg) = 0;

	virtual void					 CancelTask(CryLobbyTaskID taskId) = 0;
	// </interfuscator:shuffle>
};

#define FRIENDS_MESSAGE_GAME_INVITE			"game_invite"
#define FRIENDS_MESSAGE_SQUAD_INVITE		"squad_invite"
#define FRIENDS_MESSAGE_SQUAD_REQUEST		"squad_request"

//-------------------------------------------------------------------------
struct ICrysis3Friends
{
	// <interfuscator:shuffle>
	virtual ~ICrysis3Friends() {}

	virtual ECryLobbyError FriendsSendSquadInvite(uint32 user, CryUserID* pUserIDs, uint32 numUserIDs, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void* pCbArg) = 0;
	virtual ECryLobbyError FriendsSendRequestSquadInvite(uint32 user, CryUserID userID, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void* pCbArg) = 0;
	virtual ECryLobbyError FriendsAcceptSquadInvite(uint32 user, CryUserID inviterId, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback cb, void* pCbArg) = 0;
	virtual ECryLobbyError FriendsGetName(CryUserID userID, CryLobbyTaskID* pTaskID, CryFriendsManagementSearchCallback pCb, void* pCbArg) = 0;
	// </interfuscator:shuffle>
};

//-------------------------------------------------------------------------

typedef CryFixedStringT<1024> TTextBuffer; 

struct ICrysis3Lobby
{
	// <interfuscator:shuffle>
	virtual ~ICrysis3Lobby() {}

	virtual ICrysis3UserSettings *GetUserSettings() = 0;
	virtual ICrysis3MatchMaking *GetCrysis3MatchMaking() = 0;
	virtual ICrysis3Playgroups *GetPlaygroups() = 0;
	virtual ICrysis3FeedProvider *GetFeedProvider() = 0;
	virtual ICrysis3PartnerTelemetry *GetPartnerTelemetry() = 0;
	virtual ICrysis3Stats *GetCrysis3Stats() = 0;
	virtual ICrysis3ChallengeSystem *GetChallengeSystem() = 0;
	virtual ICrysis3Friends *GetCrysis3Friends() = 0;

	virtual const char *GetCurrentPersonaName() = 0;
	virtual CryUserID GetCurrentUser() = 0;
	virtual bool HasGameAccount(CryUserID userId) const = 0;

	virtual ECryLobbyError StartLoginProcess(ICrysis3AuthenticationHandler *pHandler, unsigned int userIndex, bool silentLogin, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback pCB, void* pCBArg) = 0;
	virtual void BeginCreateAccount(const unsigned int userIndex) = 0;
	virtual void BeginAssociateAccount(const unsigned int userIndex, const SAssociateAccountResponse& response) = 0;

	virtual ECryLobbyError Logout(unsigned int userIndex, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback pCB, void* pCBArg) = 0;
	virtual ECryLobbyError GetPasswordRules(unsigned int userIndex, CryLobbyTaskID* pTaskID, Crysis3LobbyPasswordCallback pCB, void* pCBArg) = 0;
	virtual ECryLobbyError UpdateAccount(unsigned int userIndex, const char *pOldPassword, const char *pEmail, const char *pNewPassword, CryLobbyTaskID* pTaskID, Crysis3LobbyBasicCallback pCB, void* pCBArg) = 0;

	virtual void OnDisplayCountrySelectResponse(const unsigned int userIndex, const SDisplayCountrySelectResponse& response) = 0;
	virtual void OnDisplayLegalDocsResponse(const unsigned int userIndex, const bool bAccepted) = 0;
	virtual void OnDisplayCreateAccountResponse(const unsigned int userIndex, const SCreateAccountResponse& response) = 0;
	virtual void OnDisplayLoginResponse(const unsigned int userIndex, const SDisplayLoginResponse& response) = 0;
	virtual void OnDisplayEntitleGameResponse(const unsigned int userIndex, const SDisplayEntitleGameResponse& response) = 0;
	virtual void OnDisplayPersonasResponse(const unsigned int userIndex, const SDisplayPersonasResponse& response) = 0;
	virtual void OnPasswordRequiredResponse(const unsigned int userIndex, const SPasswordRequiredResponse& response) = 0;
	virtual void CreatePersona(const unsigned int userIndex, const SDisplayPersonasResponse& response) = 0;
	virtual void DisablePersona(const unsigned int userIndex, const SDisplayPersonasResponse& response) = 0;
	virtual void ForgotPassword(const unsigned int userIndex, const SForgotPasswordResponse &response) = 0;
	virtual void LoginGoBack(const unsigned int userIndex) = 0;
	virtual void RepromptDisplay(const unsigned int userIndex) = 0;

	virtual const SCensusData& GetCensusData() const = 0;

	virtual void SetLocale(const char* pLanguage) = 0;

	virtual ECryLobbyError HasEntitlement(const unsigned int userIndex, const char *pProjectId, const char *pGroupName, const char *pEntitlementTag, CryLobbyTaskID *pTaskId, Crysis3LobbyEntitlementCallback pCb, void *pCbArg) = 0;
	virtual ECryLobbyError ListEntitlements(const unsigned int userIndex, const char *pGroupName, CryLobbyTaskID *pTaskId, Crysis3LobbyListEntitlementCallback pCb, void *pCbArg) = 0;
	virtual ECryLobbyError SetEntitlement(const unsigned int userIndex,  const char *pGroupName,  const char *pTagName,  CryLobbyTaskID *pTaskId, Crysis3LobbyBasicCallback pCB, void* pCBArg ) = 0;

	virtual bool GetLastErrorCode( int &error) = 0;
	virtual void GetLast5ErrorInfos ( SErrorInfo errors[5], int &numErrors ) = 0;

	// GetPartyMembers
	// Causes the current party members to be obtained and dispatched within an eCLSE_PartyMembers event
	//							Party members are a XENON feature only
	// return			-	eCLE_Success if successful or an error code if failed
	virtual ECryLobbyError		GetPartyMembers() = 0;

	virtual void GetStateInfo( TTextBuffer& textBuffer ) const = 0;
	// </interfuscator:shuffle>
};

#endif // __ICRYSIS3LOBBY_H__
