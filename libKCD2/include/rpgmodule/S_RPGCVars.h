#pragma once
#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// wh::rpgmodule::S_RPGCVars -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x158 (SYNTHETIC name; flat POD).
// -----------------------------------------------
// C_RPGModule's cvar-bound value block (module+0xA8; raw-allocated with NO ctor, plain-freed). Every
// field NAME below is the exact registered console-variable string (int binds via sub_1823D8168,
// float via sub_1823D8B0C/ABC, string via ICVar* pointer). Interleaved int/float scalars + string-
// cvar ICVar* pointers; no sub-objects. A handful of 4-byte gaps are genuine alignment padding.
// Handy debug/cheat toggles live here: OneShotKill, UnlockAll, LockOnAllTargets, SkipAll,
// EnableReputationOverride/ReputationOverride, ItemFullHealthByDefault.

struct ICVar;   // CryEngine console-variable interface (string cvars stored as ICVar*)

namespace wh::rpgmodule {

struct S_RPGCVars {
    int32_t  DebugSouls;                          // +0x000
    float    DebugSoulsDistance;                  // +0x004
    ICVar*   DebugSoulsFilter;                    // +0x008
    ICVar*   DebugSoulsStringFilter;              // +0x010
    ICVar*   storm_DebugOperationTypeFilter;      // +0x018
    ICVar*   storm_DebugSubstringFilter;          // +0x020
    ICVar*   storm_ProgramLocation;               // +0x028
    int32_t  DebugFactions;                       // +0x030
    uint32_t _pad034;                             // +0x034
    int32_t  DebugFactionId;                       // +0x038
    uint32_t _pad03C;                             // +0x03C
    int32_t  SkipAll;                             // +0x040
    uint32_t _pad044;                             // +0x044
    ICVar*   SkipEffectsOn;                       // +0x048
    ICVar*   SkipTimersOn;                        // +0x050
    int32_t  SkipFoodTimers;                      // +0x058
    int32_t  OneShotKill;                         // +0x05C
    int32_t  UnlockAll;                           // +0x060
    int32_t  LockOnAllTargets;                    // +0x064
    int32_t  SchedulerUpdate;                     // +0x068
    int32_t  ParallelUpdate;                      // +0x06C
    int32_t  SchedulerDebug;                      // +0x070
    int32_t  SchedulerSortIterations;             // +0x074
    int32_t  SchedulerSoulsInLayer0;              // +0x078
    int32_t  SchedulerBudgetPerLayer;             // +0x07C
    float    MaxSoulUpdateDelta;                  // +0x080
    int32_t  StatisticsDebug;                     // +0x084
    int32_t  AchievementsDebug;                   // +0x088
    int32_t  GameTimeRatioOn;                     // +0x08C
    float    CombatHistoryExpirationTime;         // +0x090
    int32_t  LogTarget;                           // +0x094
    int32_t  LogLevel;                            // +0x098
    int32_t  LogType;                             // +0x09C
    int32_t  LogReasons;                          // +0x0A0
    int32_t  LogSize;                             // +0x0A4
    int32_t  LogCompression;                      // +0x0A8  (default -1)
    int32_t  EventLogDebug;                       // +0x0AC
    float    LogInterval;                         // +0x0B0
    uint32_t _pad0B4;                             // +0x0B4
    ICVar*   LogFilepath;                         // +0x0B8
    ICVar*   TelemetryServer;                     // +0x0C0
    int32_t  TelemetryMaxRetryCount;              // +0x0C8
    int32_t  TelemetryRetryDelay;                 // +0x0CC
    int32_t  TelemetryEnforced;                   // +0x0D0
    int32_t  TelemetryLimitedToPlayline;          // +0x0D4
    int32_t  TelemetryLimitedToPlayer;            // +0x0D8
    int32_t  TelemetryMarketingConsent;           // +0x0DC
    int32_t  TelemetrySpamFiltering;              // +0x0E0
    int32_t  TelemetryPlaionEnable;               // +0x0E4
    int32_t  ProfilingLog;                        // +0x0E8
    float    ProfilingLogSmoothingFactor;         // +0x0EC
    int32_t  ProfilingLogInterval;                // +0x0F0
    int32_t  DebugAlcohol;                        // +0x0F4
    int32_t  EnableReputationOverride;            // +0x0F8
    float    ReputationOverride;                  // +0x0FC
    int32_t  ReputationLogMaxSize;                // +0x100
    int32_t  PerceptionPriorRelationshipFuncType; // +0x104
    ICVar*   ActorIlluminanceIntervalEndpoints;   // +0x108
    int32_t  ContextOverride_Health;              // +0x110  (default -1)
    float    ContextOverride_SkillLevelRelative;  // +0x114
    int32_t  ContextOverride_BodyPart;            // +0x118  (default -1)
    int32_t  AutoReloadRoles;                     // +0x11C
    int32_t  CorpseDisappearAddedDelay;           // +0x120  (default 60000)
    int32_t  ItemFullHealthByDefault;             // +0x124
    float    IndulgencePriceCacheExpirationTime;  // +0x128
    float    LocationReputationCacheExpirationTime; // +0x12C
    int32_t  FollowDrainHorseStamina;             // +0x130
    int32_t  FollowDrainRiderStamina;             // +0x134
    float    DrunkennessLimitedCameraSway;        // +0x138
    uint32_t _pad13C;                             // +0x13C
    ICVar*   PlagueDecalsStage1;                  // +0x140
    ICVar*   PlagueDecalsStage2;                  // +0x148
    ICVar*   PlagueDecalsStage3;                  // +0x150
};
static_assert(sizeof(S_RPGCVars) == 0x158, "S_RPGCVars must be 0x158");
static_assert(offsetof(S_RPGCVars, OneShotKill) == 0x5C, "OneShotKill at 0x5C");
static_assert(offsetof(S_RPGCVars, PlagueDecalsStage3) == 0x150, "last cvar at 0x150");

}  // namespace wh::rpgmodule
