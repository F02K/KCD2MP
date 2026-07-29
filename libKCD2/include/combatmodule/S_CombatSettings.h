#pragma once

// ==========================================================================
// S_CombatSettings.h  (Kingdom Come: Deliverance 2 -- WHGame.dll 1.5.6, x64)
//
// Combat tuning / CVar mirror. One field per wh_cs_* console variable.
// Global singleton:  g_combatSettings @ RVA 0x55D4D70  (unk_1855D4D70).
//   accessor sub_1809D5994 (function-static, TLS-guarded) returns &g_combatSettings.
// CVar registration:  sub_1810A8DA0 (0x6933 bytes), called from
//   C_CombatModule::Init sub_180D243C0.  Each registration writes a default to
//   base+offset, which fixes the field layout below (fields are ordered by offset,
//   NOT by registration order -- the two differ).
// CVar de-registration:  sub_182778CA0 (called from PostInit sub_182778C10) -- it
//   only UnregisterVariable()s the same names; it is NOT a second storage batch.
//
// Registration helpers -> ICVarManager vtable slot (mirrors KCD1):
//   int32 CVar (default present):  sub_1823D8168 / sub_18276BCC0 / sub_1823DB0B8 /
//                                  sub_1823D8A64   -> vtable+0x40
//   float CVar:                    sub_1823D8B0C / sub_1823D8ABC / sub_1823DB2B8
//                                  -> vtable+0x48  (default passed in xmm2)
//   string CVar (ICVar* stored):   sub_180B95258   -> vtable+0x10
//   command (no storage, skipped): sub_180B925B8   -> vtable+0x108
//
// Field names keep the wh_cs_ prefix to match the KCD1 reference header. String
// CVar fields hold the returned ICVar* (named pCVar_wh_cs_*). Float defaults were
// recovered from the xmm2 constant at each call site (verified: no xmm arithmetic).
//
// static_assert size 0x880: proven by the next .data symbol (unk_1855D55F0) starting
// exactly base+0x880; last field ends at 0x87C, +4 trailing pad for 8-byte align.
// ==========================================================================

#include <cstdint>

namespace wh::combatmodule {

#pragma pack(push, 1)
struct S_CombatSettings {
    int32_t  wh_cs_DebugDrawAttackActions;              // 0x000  "wh_cs_DebugDrawAttackActions" default=0 flags=4  Enable visual debug of attack actions.
    int32_t  wh_cs_DebugAttackStart;                    // 0x004  "wh_cs_DebugAttackStart" default=0 flags=2  AttackStart activation for debug wioth cvar. 1 for activation, 2 for deactivation
    int32_t  wh_cs_DebugShowCombatDebugInTests;         // 0x008  "wh_cs_DebugShowCombatDebugInTests" default=1 flags=2  Indicates when debug draw should be visible during tests.
    uint8_t  _pad_00C[0x4];                    // 0x00C
    int64_t  pCVar_wh_cs_DebugDrawQuery;                // 0x010  ICVar* "wh_cs_DebugDrawQuery"  Show query debug for specified actor.
    int32_t  wh_cs_UseDudeInTests;                      // 0x018  "wh_cs_UseDudeInTests" default=0 flags=4  Replace one soul in tests with dude. 1 - for first soul, 2 - for second soul
    int32_t  wh_cs_DebugCombatActions;                  // 0x01C  "wh_cs_DebugCombatActions" default=0 flags=4  Detailed information about combat actions in the log.
    int32_t  wh_cs_DisableWeakBlock;                    // 0x020  "wh_cs_DisableWeakBlock" default=0 flags=4  Disables weak block feature.
    int32_t  wh_cs_DisablePlayerOpponentSelection;      // 0x024  "wh_cs_DisablePlayerOpponentSelection" default=0 flags=4  Disable standard selection of opponent for player for tests.
    int32_t  wh_cs_DebugHitStrength;                    // 0x028  "wh_cs_DebugHitStrength" default=0 flags=4  Debug hit strength overriding rpg for debuggin weak and strong hits.
    int32_t  wh_cs_DonotUseDatabaseIndices;             // 0x02C  "wh_cs_DonotUseDatabaseIndices" default=0 flags=4  Bypass database optimization indices in combat databases.
    int64_t  pCVar_wh_cs_DebugCameraOverride;           // 0x030  ICVar* "wh_cs_DebugCameraOverride"  Override for combat camera. Allows incarnate camera to the npc in the combat.
    float    wh_cs_TimeWarpFadeIn;                      // 0x038  "wh_cs_TimeWarpFadeIn" default=0.01f flags=2  Time of fade in during PB time warp defined relative to hit point.
    float    wh_cs_TimeWarpFadeOut;                     // 0x03C  "wh_cs_TimeWarpFadeOut" default=0.1f flags=2  Time of fade out during PB time warp defined relative to hit point.
    float    wh_cs_TimeWarpDurationPlayer;              // 0x040  "wh_cs_TimeWarpDurationPlayer" default=0.3f flags=2  Time of fade out during PB time warp defined relative to hit point.
    float    wh_cs_TimeWarpDurationOpp;                 // 0x044  "wh_cs_TimeWarpDurationOpp" default=0.05f flags=2  Time of fade out during PB time warp defined relative to hit point.
    float    wh_cs_TimeWarpSyncPBFadeIn;                // 0x048  "wh_cs_TimeWarpSyncPBFadeIn" default=0.3f flags=4  Timewarp fade in for sync guards.
    float    wh_cs_RiposteSlotDuration;                 // 0x04C  "wh_cs_RiposteSlotDuration" default=0.5f flags=2  Basic riposte slot duration.
    float    wh_cs_StandardSlotDuration;                // 0x050  "wh_cs_StandardSlotDuration" default=1.0f flags=2  Standard slot duration.
    float    wh_cs_DeathSyncHitHPTolerance;             // 0x054  "wh_cs_DeathSyncHitHPTolerance" default=2.6f flags=2  Threshold when resting HP is considered as fatal and opponent dies.
    float    wh_cs_PerfectBlock_RetargetAngleFOVBias;   // 0x058  "wh_cs_PerfectBlock_RetargetAngleFOVBias" default=0.5f flags=2  Bias to camera FOV for PB which reacts to attacks from different actor to retarget to t...
    float    wh_cs_PerfectBlock_SpatialVolumeRadius;    // 0x05C  "wh_cs_PerfectBlock_SpatialVolumeRadius" default=0.1f flags=2  Radius for spatial test limiting perfect block possibility.
    float    wh_cs_Dodge_WindowMod;                     // 0x060  "wh_cs_Dodge_WindowMod" default=0.65f flags=2  Mod of Dodge slot window - determined as (t_hit - t_dodgeslot) from attack x this value.
    float    wh_cs_Dodge_SpatialTimeLimit;              // 0x064  "wh_cs_Dodge_SpatialTimeLimit" default=0.9f flags=2  Time limit in percent from dodge animation to get spatial data.
    int32_t  wh_cs_Dodge_IgnoreCollision;               // 0x068  "wh_cs_Dodge_IgnoreCollision" default=1 flags=2  When nonzero dodging actor will ignore weapon collision with his attacker.
    float    wh_cs_Dodge_MaxDistance;                   // 0x06C  "wh_cs_Dodge_MaxDistance" default=4.0f flags=2  Max distance (m) for dodge movement (player value).
    float    wh_cs_Dodge_Duration;                      // 0x070  "wh_cs_Dodge_Duration" default=1.0f flags=2  Duration of entity dodge movement (player value).
    float    wh_cs_Dodge_DistanceFactorOverride;        // 0x074  "wh_cs_Dodge_DistanceFactorOverride" default=-1.0f flags=2  Override dodge distance factor for debugging.
    float    wh_cs_AISlotDurationMod;                   // 0x078  "wh_cs_AISlotDurationMod" default=0.5f flags=2  Custom RPG Mod of riposte slot - 0 = lama, 0.5 = standard, 1 = profa
    float    wh_cs_PLSlotDurationMod;                   // 0x07C  "wh_cs_PLSlotDurationMod" default=0.5f flags=2  Custom RPG Mod of riposte slot - 0 = lama, 0.5 = standard, 1 = profa
    float    wh_cs_BlockCooldown;                       // 0x080  "wh_cs_BlockCooldown" default=0.2f flags=2  Cooldown for player block trigger.
    float    wh_cs_StaticAttackCooldown;                // 0x084  "wh_cs_StaticAttackCooldown" default=0.4f flags=2  Cooldown for player static attack.
    float    wh_cs_MoveCooldown;                        // 0x088  "wh_cs_MoveCooldown" default=0.35f flags=2  Cooldown for player move (dodge).
    float    wh_cs_RipostePerfectBlockZoneDistance;     // 0x08C  "wh_cs_RipostePerfectBlockZoneDistance" default=0.4f flags=2  Allowed distance from blocking zone when searching for attack part of riposte perfect b...
    float    wh_cs_OpenSlotFOVBiasPlayer;               // 0x090  "wh_cs_OpenSlotFOVBiasPlayer" default=35.0f flags=2  FOV bias for opening slot when blocker is player.
    float    wh_cs_OpenSlotFOVBiasNPC;                  // 0x094  "wh_cs_OpenSlotFOVBiasNPC" default=45.0f flags=2  FOV bias for opening slot when blocker is NPC.
    float    wh_cs_MasterStrikeOnRiposteTimeOffset;     // 0x098  "wh_cs_MasterStrikeOnRiposteTimeOffset" default=0.2f flags=2  Time offset of the seek when MS is made against riposte which is shorter.
    float    wh_cs_IgnoreHitDuringDeathTimer;           // 0x09C  "wh_cs_IgnoreHitDuringDeathTimer" default=0.4f flags=2  Cooldown for preventing hits during death reactions.
    float    wh_cs_PlayerInputMouseZoneChangeSensitivity; // 0x0A0  "wh_cs_PlayerInputMouseZoneChangeSensitivity" default=0.07f flags=4  Sensitivity for change zone with mouse. 20 - super sensitive, 0 - minimal sensitivity
    int32_t  wh_cs_PlayerInputMouseUnlocking;           // 0x0A4  "wh_cs_PlayerInputMouseUnlocking" default=1 flags=4  Enable mouse movement unlocking for player
    int32_t  wh_cs_PlayerInputBypass;                   // 0x0A8  "wh_cs_PlayerInputBypass" default=0 flags=4  Disables combat input
    float    wh_cs_PlayerFreeCameraModeMouseDeadZone;   // 0x0AC  "wh_cs_PlayerFreeCameraModeMouseDeadZone" default=5.0f flags=4  Dead zone in free camera mode for mouse
    float    wh_cs_PlayerFreeCameraModeGamepadDeadZone; // 0x0B0  "wh_cs_PlayerFreeCameraModeGamepadDeadZone" default=1.0f flags=4  Dead zone in free camera mode for gamepad
    float    wh_cs_PlayerInputLockingTolerance;         // 0x0B4  "wh_cs_PlayerInputLockingTolerance" default=60.0f flags=4  Player input - Screen distance vertical tolerance from target defining locking area. (i...
    float    wh_cs_PlayerMaxOpponentDistanceToLock;     // 0x0B8  "wh_cs_PlayerMaxOpponentDistanceToLock" default=4.0f flags=4  Maximum distance to player lock.
    float    wh_cs_PlayerLockOpponentAngleBias;         // 0x0BC  "wh_cs_PlayerLockOpponentAngleBias" default=45.0f flags=4  Bias of FOV angle to lock to opponent.
    float    wh_cs_PlayerMaxOpponentDistanceToUnlock;   // 0x0C0  "wh_cs_PlayerMaxOpponentDistanceToUnlock" default=7.0f flags=4  Maximum distance to player unlock.
    float    wh_cs_PlayerMaxOpponentAngleToUnlock;      // 0x0C4  "wh_cs_PlayerMaxOpponentAngleToUnlock" default=0.87266463f flags=4  Maximum possible angle to opponent before player is unlocked from combat.
    float    wh_cs_PlayerMaxOpponentAngleToLock;        // 0x0C8  "wh_cs_PlayerMaxOpponentAngleToLock" default=0.7853982f flags=4  Maximum possible angle to opponent to lock to combat.
    float    wh_cs_PlayerUnlockTimeout;                 // 0x0CC  "wh_cs_PlayerUnlockTimeout" default=0.7853982f flags=4  Timeout to unlock player from opp when his visibility is lost.
    int32_t  wh_cs_PlayerLockDisabled;                  // 0x0D0  "wh_cs_PlayerLockDisabled" default=0 flags=4  Disables player locking functionality.
    float    wh_cs_PlayerInputMouseZoneChangeMinTime;   // 0x0D4  "wh_cs_PlayerInputMouseZoneChangeMinTime" default=0.1f flags=4  Minimal time to allow zone change with mouse.
    float    wh_cs_PlayerInputMouseZoneChangeMinDistance; // 0x0D8  "wh_cs_PlayerInputMouseZoneChangeMinDistance" default=0.1f flags=4  Minimal distance to allow zone change with mouse.
    float    wh_cs_PlayerInputMouseZoneChangePreventionAfterAction; // 0x0DC  "wh_cs_PlayerInputMouseZoneChangePreventionAfterAction" default=0.05f flags=4  Safety cooldown after action where attack zone change is forbidden.
    float    wh_cs_PlayerInputMouseUnlockMinTime;       // 0x0E0  "wh_cs_PlayerInputMouseUnlockMinTime" default=0.15f flags=4  Minimal time to allow unlocking from selected opponent with mouse.
    float    wh_cs_PlayerInputMouseUnlockReturnTime;    // 0x0E4  "wh_cs_PlayerInputMouseUnlockReturnTime" default=0.2f flags=4  Time spent in unlock mode when mouse movement is still before return to lock mode.
    float    wh_cs_PlayerInputGamepadUnlockReturnTime;  // 0x0E8  "wh_cs_PlayerInputGamepadUnlockReturnTime" default=0.15f flags=4  Time spent in unlock mode when gamepad movement is still before return to lock mode.
    float    wh_cs_PlayerInputMouseUnlockMinDistanceToUnlock; // 0x0EC  "wh_cs_PlayerInputMouseUnlockMinDistanceToUnlock" default=0.2f flags=4  Minimal distance to process unlock with mouse.
    float    wh_cs_PlayerInputMouseUnlockMinOppAngle;   // 0x0F0  "wh_cs_PlayerInputMouseUnlockMinOppAngle" default=0.2f flags=4  Angle to opponent which will reset total distance to unlock.
    float    wh_cs_PlayerInputMouseUnlockZThreshold;    // 0x0F4  "wh_cs_PlayerInputMouseUnlockZThreshold" default=0.2f flags=4  Angle Z-axis that limits view in camera unlocked mode.
    float    wh_cs_PlayerInputMouseUnlockXThreshold;    // 0x0F8  "wh_cs_PlayerInputMouseUnlockXThreshold" default=0.2f flags=4  Angle X-axis that limits view in camera unlocked mode.
    float    wh_cs_PlayerInputGamepadUnlockMinTime;     // 0x0FC  "wh_cs_PlayerInputGamepadUnlockMinTime" default=0.5f flags=4  Minimal time to allow unlocking from selected opponent with gamepad.
    float    wh_cs_PlayerInputGamepadUnlockMinTimeSameZone; // 0x100  "wh_cs_PlayerInputGamepadUnlockMinTimeSameZone" default=0.3f flags=4  Minimal time to allow unlocking from selected opponent with gamepad.
    float    wh_cs_PlayerInputGamepadUnlockMinDistanceToUnlock; // 0x104  "wh_cs_PlayerInputGamepadUnlockMinDistanceToUnlock" default=0.3f flags=4  Minimal distance to process unlock with gamepad.
    float    wh_cs_PlayerInputLockAreaWidth;            // 0x108  "wh_cs_PlayerInputLockAreaWidth" default=0.4f flags=4  Distance from opponent in metres to start locking (X axis).
    float    wh_cs_PlayerInputLockAreaWidthNoCombat;    // 0x10C  "wh_cs_PlayerInputLockAreaWidthNoCombat" default=1.9f flags=4  Distance from opponent in metres to start locking (X axis) when player is not in combat.
    float    wh_cs_PlayerInputUnlockAreaWidthHyst;      // 0x110  "wh_cs_PlayerInputUnlockAreaWidthHyst" default=0.2f flags=4  Unlock hysterezis distance. PlayerInputLockAreaWidth + PlayerInputUnlockAreaWidthHyst g...
    float    wh_cs_PlayerInputGamepadSensitivity;       // 0x114  "wh_cs_PlayerInputGamepadSensitivity" default=0.05f flags=4  Camera movement minimal sensitivity when lock on opponent for gamepad.
    float    wh_cs_PlayerInputMouseSensitivity;         // 0x118  "wh_cs_PlayerInputMouseSensitivity" default=0.1f flags=4  Camera movement minimal sensitivity when lock on opponent for mouse.
    float    wh_cs_PlayerInputCursorUnlockDistance;     // 0x11C  "wh_cs_PlayerInputCursorUnlockDistance" default=0.2f flags=4  Minimal angle to unlock cursor.
    float    wh_cs_PlayerInputCombatUnlockDelay;        // 0x120  "wh_cs_PlayerInputCombatUnlockDelay" default=0.1f flags=4  Time in sec to unlock from combat when player changes target with mouse.
    float    wh_cs_PlayerInputCombatDelayAfterKill;     // 0x124  "wh_cs_PlayerInputCombatDelayAfterKill" default=0.6f flags=4  Time in sec to end combat for player when he destroys the opponent.
    float    wh_cs_PlayerInputCombatDelayAfterKillWithOtherOpponent; // 0x128  "wh_cs_PlayerInputCombatDelayAfterKillWithOtherOpponent" default=0.2f flags=4  Time in sec to end combat for player when he destroys the opponent and different oppone...
    float    wh_cs_PlayerMaxThreatDistance;             // 0x12C  "wh_cs_PlayerMaxThreatDistance" default=15.0f flags=4  Range for IsThreatNearby detection of threat.
    float    wh_cs_PlayerInputCombatAngleAfterKill;     // 0x130  "wh_cs_PlayerInputCombatAngleAfterKill" default=-0.6981317f flags=4  Limit X-axis angle when player is unlocked from opponent after kill.
    float    wh_cs_PlayerFreeAttackPrepareTime;         // 0x134  "wh_cs_PlayerFreeAttackPrepareTime" default=0.3f flags=4  Time in sec for prepare attack during free attack. The attack action is started after t...
    float    wh_cs_PlayerFreeAttackMinDistance;         // 0x138  "wh_cs_PlayerFreeAttackMinDistance" default=0.7f flags=4  Min distance for uninterrupted free attack in front of an obstacle.
    float    wh_cs_PlayerFreeAttackStabMinDistance;     // 0x13C  "wh_cs_PlayerFreeAttackStabMinDistance" default=1.3f flags=4  Min distance for uninterrupted free attack in front of an obstacle for stabbing.
    float    wh_cs_PlayerVIPTargetAttackDot;            // 0x140  "wh_cs_PlayerVIPTargetAttackDot" default=0.8f flags=4  Maximum dot product that allows attack even when VIP target is closer than opponent.
    float    wh_cs_PlayerInputIndicator;                // 0x144  "wh_cs_PlayerInputIndicator" default=0.8f flags=4  Indicator of unlocking is visible in combat.
    float    wh_cs_PlayerNormalAttackOffset;            // 0x148  "wh_cs_PlayerNormalAttackOffset" default=0.5f flags=4  Relative time offset for player's normal attacks.
    int32_t  wh_cs_DebugDrawPlayerInput;                // 0x14C  "wh_cs_DebugDrawPlayerInput" default=0 flags=4  Enable visual debug of player input.
    int32_t  wh_cs_PlayerTutorialStep;                  // 0x150  "wh_cs_PlayerTutorialStep" default=0 flags=4  Debugger trigger of combat tutorial step. [1-?, 0 disabled]
    float    wh_cs_PlayerHorizontalUnlockDelay;         // 0x154  "wh_cs_PlayerHorizontalUnlockDelay" default=0.5f flags=4  Delay before player unlocks from opponent when moving in horizontal direction.
    float    wh_cs_PlayerVerticalUnlockDelay;           // 0x158  "wh_cs_PlayerVerticalUnlockDelay" default=1.0f flags=4  Delay before player unlocks from opponent when moving in vertical direction.
    float    wh_cs_PlayerAttackInactivityDuration;      // 0x15C  "wh_cs_PlayerAttackInactivityDuration" default=2.0f flags=4  Duration to detect player inactivity (healing enabled in battles)
    float    wh_cs_PlayerAttackInactivityDurationVsDog; // 0x160  "wh_cs_PlayerAttackInactivityDurationVsDog" default=6.0f flags=4  Duration to detect player inactivity when last opponent is a dog
    float    wh_cs_FreeBlockPreparingDuration;          // 0x164  "wh_cs_FreeBlockPreparingDuration" default=0.3f flags=4  Delay before actor set PARRY_IN_PLACE in free blocks.
    float    wh_cs_BrokenBlockInterruptWhenDestroyedWeapon; // 0x168  "wh_cs_BrokenBlockInterruptWhenDestroyedWeapon" default=0.2f flags=4  Broken block is interrupted when weapon is destroyed after this time.
    int32_t  wh_cs_TargetUseSpineBone;                  // 0x16C  "wh_cs_TargetUseSpineBone" default=0 flags=4  Uses spine3 for targeting opponent.
    float    wh_cs_CombatCursorSmoothing;               // 0x170  "wh_cs_CombatCursorSmoothing" default=0.1f flags=4  Smoothing for opponent UI in combat.
    int32_t  wh_cs_CombatCursorMode;                    // 0x174  "wh_cs_CombatCursorMode" default=1 flags=2  Cursor mode in combat - 0 - completely hidden, 1 - normal mode, 2 - HC mode (only dot)
    int32_t  wh_cs_FixedCombatCursor;                   // 0x178  "wh_cs_FixedCombatCursor" default=0 flags=2  Combat cursor is fixed in the middle of the screen.
    float    wh_cs_TutorialSlotActionFadeIn;            // 0x17C  "wh_cs_TutorialSlotActionFadeIn" default=0.0f flags=4  Tutorial slot action - fade in of time warp effect.
    float    wh_cs_TutorialSlotActionFadeOut;           // 0x180  "wh_cs_TutorialSlotActionFadeOut" default=0.0f flags=4  Tutorial slot action - fade out of time warp effect.
    float    wh_cs_ComboSlotDuration;                   // 0x184  "wh_cs_ComboSlotDuration" default=0.5f flags=2  Duration of combo slot
    int32_t  wh_cs_CombosEnabled;                       // 0x188  "wh_cs_CombosEnabled" default=1 flags=2  Combo mechanic is enabled.
    float    wh_cs_WindowMinWeight;                     // 0x18C  "wh_cs_WindowMinWeight" default=0.5f flags=2  Weight <0, 1> to minimal window computation. For PB and Riposte.
    float    wh_cs_SlotEndHitOffset;                    // 0x190  "wh_cs_SlotEndHitOffset" default=0.15f flags=2  Delay of slot to automatic start from action.
    float    wh_cs_SyncSlotEndHitOffset;                // 0x194  "wh_cs_SyncSlotEndHitOffset" default=0.1f flags=2  Delay of slot to automatic start from action.
    int32_t  wh_cs_PLSkillProfile;                      // 0x198  "wh_cs_PLSkillProfile" default=1 flags=2  Skill profile for player.
    int32_t  wh_cs_AISkillProfile;                      // 0x19C  "wh_cs_AISkillProfile" default=1 flags=2  Skill profile for AI.
    float    wh_cs_StealthActionDistance;               // 0x1A0  "wh_cs_StealthActionDistance" default=2.0f flags=2  Defines max distance from victim alowing a stealth action.
    float    wh_cs_StealthActionAngle;                  // 0x1A4  "wh_cs_StealthActionAngle" default=135.0f flags=2  Defines max victim view angle (in degrees) enabling a stealth action.
    float    wh_cs_StealthActionLyingAngle;             // 0x1A8  "wh_cs_StealthActionLyingAngle" default=150.0f flags=2  Defines frustrum angle next to laying victim limiting stealth actions.
    float    wh_cs_StealthActionMaxAngle;               // 0x1AC  "wh_cs_StealthActionMaxAngle" default=0.5235988f flags=2  Maximal angle between combatants for stealth kills.
    int32_t  wh_cs_StealthAlwaysSuccess;                // 0x1B0  "wh_cs_StealthAlwaysSuccess" default=0 flags=2  Stealth kill action always succeeds.
    float    wh_cs_StealthTimewarpDurationOverride;     // 0x1B4  "wh_cs_StealthTimewarpDurationOverride" default=-1.0f flags=2  Debug stealth slot duration.
    int32_t  wh_cs_StealthDisableAIPerfectBlock;        // 0x1B8  "wh_cs_StealthDisableAIPerfectBlock" default=0 flags=2  Disabling PB for AI during stealth kill.
    int32_t  wh_cs_StealthDisableAIBackoff;             // 0x1BC  "wh_cs_StealthDisableAIBackoff" default=0 flags=2  Disabling backoff for AI during stealth kill.
    float    wh_cs_HorsePullDownAngle;                  // 0x1C0  "wh_cs_HorsePullDownAngle" default=55.0f flags=2  Defines max XY angle to do pulling down action. It is half angle from the center of horse.
    float    wh_cs_HorsePullDownZeroAngle;              // 0x1C4  "wh_cs_HorsePullDownZeroAngle" default=20.0f flags=2  Zero angle for computation during pull down from horse. The result angle limit is Horse...
    float    wh_cs_HorsePullDownZAngle;                 // 0x1C8  "wh_cs_HorsePullDownZAngle" default=15.0f flags=2  Defines max Z angle to do pulling down action.
    int32_t  wh_cs_HorsePullDownForce;                  // 0x1CC  "wh_cs_HorsePullDownForce" default=0 flags=2  Forse horse pulldown action to always succeeded (=1) of fail (=2).
    int32_t  wh_cs_HuntAttackAttackerMinLogical;        // 0x1D0  "wh_cs_HuntAttackAttackerMinLogical" default=0 flags=2  Minimal pseudospeed of attacker to enable hung attack.
    int32_t  wh_cs_HuntAttackVictimMinLogical;          // 0x1D4  "wh_cs_HuntAttackVictimMinLogical" default=1 flags=2  Minimal pseudospeed of victim to enable hung attack.
    int32_t  wh_cs_HuntAttackDisableVelocity;           // 0x1D8  "wh_cs_HuntAttackDisableVelocity" default=0 flags=2  Disable velocity checks for debugging.
    float    wh_cs_HuntAttackMoveWeight;                // 0x1DC  "wh_cs_HuntAttackMoveWeight" default=2.0f flags=2  Weight for movement compensation of actors.
    float    wh_cs_HuntAttackFwdAngleLimit;             // 0x1E0  "wh_cs_HuntAttackFwdAngleLimit" default=30.0f flags=2  Angle limit in deg for forward hunt attack selection
    float    wh_cs_HuntAttackOppFwdMovementAngleLimit;  // 0x1E4  "wh_cs_HuntAttackOppFwdMovementAngleLimit" default=90.0f flags=2  Angle limit in deg for forward movement of victim to prevent hunt attack during backwar...
    int32_t  wh_cs_HuntAttackDebugMode;                 // 0x1E8  "wh_cs_HuntAttackDebugMode" default=0 flags=2  Debug mode that all input conditions are skipped.
    int32_t  wh_cs_BattleDebugDraw;                     // 0x1EC  "wh_cs_BattleDebugDraw" default=0 flags=2  Debug draw fot battle manager.
    int64_t  pCVar_wh_cs_BattleDebugDrawFilter;         // 0x1F0  ICVar* "wh_cs_BattleDebugDrawFilter"  Filtering for battle debug draw by control entity.
    int64_t  pCVar_wh_cs_DebugAnimDebug;                // 0x1F8  ICVar* "wh_cs_DebugAnimDebug"  Enables animation debug draw for defined battle soul.
    int32_t  wh_cs_BattleMaximumNPC;                    // 0x200  "wh_cs_BattleMaximumNPC" default=30 flags=2  Maximum full and alive NPC on the battle field excluding player.
    int32_t  wh_cs_BattleMaximumDeadNPC;                // 0x204  "wh_cs_BattleMaximumDeadNPC" default=15 flags=2  Maximum count of dead NPCs on the scene. When there is more the oldest will disappear.
    float    wh_cs_BattleMaximumShooterDistance;        // 0x208  "wh_cs_BattleMaximumShooterDistance" default=100.0f flags=2  Maximum distance for shooter targeting.
    float    wh_cs_BattleShooterDistanceWeight;         // 0x20C  "wh_cs_BattleShooterDistanceWeight" default=1.0f flags=2  Weight for distance priority component.
    float    wh_cs_BattleMaximumShooterAngle;           // 0x210  "wh_cs_BattleMaximumShooterAngle" default=1.0f flags=2  Maximum angle deviation of shooter directed to his target.
    float    wh_cs_BattleShooterAngleWeight;            // 0x214  "wh_cs_BattleShooterAngleWeight" default=0.1f flags=2  Weight for angle priority component.
    float    wh_cs_BattleMaxIdleAnimationTimeOffset;    // 0x218  "wh_cs_BattleMaxIdleAnimationTimeOffset" default=6.0f flags=2  Maximum time offset of idle animation when creating a idle sequence.
    float    wh_cs_BattleMaxIdleAnimationTimeScale;     // 0x21C  "wh_cs_BattleMaxIdleAnimationTimeScale" default=0.2f flags=2  Maximum time scale of idle animation when creating a idle sequence.
    float    wh_cs_BattleShootingAimingMultiplier;      // 0x220  "wh_cs_BattleShootingAimingMultiplier" default=0.75f flags=2  Multiplier of aiming period for battle shooters.
    float    wh_cs_BattleShootingAimingVariation;       // 0x224  "wh_cs_BattleShootingAimingVariation" default=0.5f flags=2  Variation for aiming period for battle shooters.
    int32_t  wh_cs_DeadBodyDebugDraw;                   // 0x228  "wh_cs_DeadBodyDebugDraw" default=0 flags=2  Debug draw for dead body manager.
    uint8_t  _pad_22C[0x4];                    // 0x22C
    int64_t  pCVar_wh_cs_DeadBodyDebugDrawFilter;       // 0x230  ICVar* "wh_cs_DeadBodyDebugDrawFilter"  Filtering for deadbody debug draw by control entity.
    int64_t  pCVar_wh_cs_DebugInfo;                     // 0x238  ICVar* "wh_cs_DebugInfo"  Enable combat system debug info. You can provide entity name or 1.
    int32_t  wh_cs_DebugPhys;                           // 0x240  "wh_cs_DebugPhys" default=0 flags=2  Enable combat system debug info for physics
    float    wh_cs_DebugInfoPosY;                       // 0x244  "wh_cs_DebugInfoPosY" default=50.0f flags=2  Define combat system debug info position in Y
    int32_t  wh_cs_HitReactions;                        // 0x248  "wh_cs_HitReactions" default=0 flags=2  Enables combat hit reactions
    int32_t  wh_cs_DisableScripts;                      // 0x24C  "wh_cs_DisableScripts" default=0 flags=2  Disables Lua scripts - movement.
    float    wh_cs_PosAlignmentInterruptionZAngleThreshold; // 0x250  "wh_cs_PosAlignmentInterruptionZAngleThreshold" default=50.0f flags=2  When z angle between two combatants reach this level position alignment is interrupted.
    int32_t  wh_cs_PhysSim;                             // 0x254  "wh_cs_PhysSim" default=0 flags=2  Physical simulation.
    float    wh_cs_PhysWeaponColSim_ImpactCoef;         // 0x258  "wh_cs_PhysWeaponColSim_ImpactCoef" default=1.0f flags=2  Physical weapon collision simulation - impact coefficient.
    float    wh_cs_PhysHitReaction_ImpactCoef;          // 0x25C  "wh_cs_PhysHitReaction_ImpactCoef" default=15.0f flags=2  Physical hit reaction simulation - impact coefficient.
    float    wh_cs_PhysWeaponHitReaction_ImpactCoef;    // 0x260  "wh_cs_PhysWeaponHitReaction_ImpactCoef" default=3.0f flags=2  Physical weapon hit reaction simulation - impact coefficient.
    float    wh_cs_BlockWithdrawalBlendWeightLoss;      // 0x264  "wh_cs_BlockWithdrawalBlendWeightLoss" default=0.3f flags=2  Blendweight loss for blocking move back when obstacle found.
    int32_t  wh_cs_OpponentManagerBruteForce;           // 0x268  "wh_cs_OpponentManagerBruteForce" default=0 flags=2  Iterate through all actors! (AI grid wont be used)
    int32_t  wh_cs_ShowCombatUI;                        // 0x26C  "wh_cs_ShowCombatUI" default=1 flags=2  Enabled combat UI for testing purposes.
    int32_t  wh_cs_ShowComboZonesUI;                    // 0x270  "wh_cs_ShowComboZonesUI" default=1 flags=2  Show combo zones on combat cross.
    int32_t  wh_cs_ShowRiposteUI;                       // 0x274  "wh_cs_ShowRiposteUI" default=1 flags=2  Enable riposte slot UI.
    int32_t  wh_cs_Profiling;                           // 0x278  "wh_cs_Profiling" default=0 flags=2  Enabled profiling in combat -> disables all code which is not in master.
    float    wh_cs_CombatCapsuleWidth;                  // 0x27C  "wh_cs_CombatCapsuleWidth" default=0.4f flags=2  Width of additional combat capsule.
    float    wh_cs_CombatCapsulePosZ;                   // 0x280  "wh_cs_CombatCapsulePosZ" default=1.2f flags=2  Position above the ground of additional combat capsule.
    float    wh_cs_PreBlockRestartCooldown;             // 0x284  "wh_cs_PreBlockRestartCooldown" default=0.5f flags=2  Preblocks delay after a block action.
    int32_t  wh_cs_EnableCollisionResizing;             // 0x288  "wh_cs_EnableCollisionResizing" default=0 flags=2  Enables to enlarge capsule collision in combat mode.
    int32_t  wh_cs_ChargedAttackOverride;               // 0x28C  "wh_cs_ChargedAttackOverride" default=0 flags=2  Override for charged RPG state used for testing purposes.
    int32_t  wh_cs_BrokenBlockOverride;                 // 0x290  "wh_cs_BrokenBlockOverride" default=0 flags=2  Override for broken blocks for testing purposes 0 - by rpg, 1 - broken, 2 - normal blocks.
    float    wh_cs_MaxFakeCollisionDistance;            // 0x294  "wh_cs_MaxFakeCollisionDistance" default=1.8f flags=2  Maximum distance to player when fake collision can appear.
    int32_t  wh_cs_EnableCombatGroups;                  // 0x298  "wh_cs_EnableCombatGroups" default=1 flags=2  Enable combat groups for combat.
    int32_t  wh_cs_DebugCombatGroups;                   // 0x29C  "wh_cs_DebugCombatGroups" default=1 flags=4  Debug draw for combat groups. 1 - dumping history when some safe check fails, 2 - osd d...
    float    wh_cs_GuardVariationUpdatePeriod;          // 0x2A0  "wh_cs_GuardVariationUpdatePeriod" default=5.0f flags=4  Period [s] for updating variation system.
    float    wh_cs_GuardVariationOneShotPeriod;         // 0x2A4  "wh_cs_GuardVariationOneShotPeriod" default=3.0f flags=4  Average time for planning one shot animation in Guard variation system.
    float    wh_cs_GuardVariationOneShotVar;            // 0x2A8  "wh_cs_GuardVariationOneShotVar" default=2.0f flags=4  Variation time for planning one shot animation in Guard variation system.
    float    wh_cs_GuardVariationActionTimer;           // 0x2AC  "wh_cs_GuardVariationActionTimer" default=2.5f flags=4  Delay for idle var after combat action.
    float    wh_cs_GuardVariationStartCombatTimer;      // 0x2B0  "wh_cs_GuardVariationStartCombatTimer" default=1.5f flags=4  Delay for idle var after combat start.
    float    wh_cs_GuardVariationOpponentActionTimer;   // 0x2B4  "wh_cs_GuardVariationOpponentActionTimer" default=3.0f flags=4  Delay when threat is about to do some action.
    float    wh_cs_GuardVariationPrimaryTargetMinDistance; // 0x2B8  "wh_cs_GuardVariationPrimaryTargetMinDistance" default=3.5f flags=4  Minimal distance to target (which is targetting me) when idle var is allowed to start.
    float    wh_cs_GuardVariationNonPrimaryTargetMinDistance; // 0x2BC  "wh_cs_GuardVariationNonPrimaryTargetMinDistance" default=2.8f flags=4  Minimal distance to target (which is not targetting me) when idle var is allowed to start.
    float    wh_cs_EventIdleCooldown;                   // 0x2C0  "wh_cs_EventIdleCooldown" default=0.6f flags=4  Cooldown for idle event when both combatants are in guard.
    float    wh_cs_EventIdleCooldownVar;                // 0x2C4  "wh_cs_EventIdleCooldownVar" default=0.3f flags=4  Variation for idle event.
    float    wh_cs_EventIdleFollowingCooldown;          // 0x2C8  "wh_cs_EventIdleFollowingCooldown" default=3.0f flags=4  Cooldown for idle event when actor stays in the guard longer.
    float    wh_cs_EventIdleFollowingCooldownVar;       // 0x2CC  "wh_cs_EventIdleFollowingCooldownVar" default=1.5f flags=4  Variation for EventIdleFollowingCooldown.
    int32_t  wh_cs_DebugAI;                             // 0x2D0  "wh_cs_DebugAI" default=0 flags=2  Enables debug AI
    int32_t  wh_cs_DebugPlayer;                         // 0x2D4  "wh_cs_DebugPlayer" default=0 flags=2  Force debug AI to select player as opponent
    int32_t  wh_cs_ForceBlock;                          // 0x2D8  "wh_cs_ForceBlock" default=0 flags=2  Force AI to block by: 0 - disabled, 1 - normal block, 2 - perfect block, 3 - no block, ...
    int32_t  wh_cs_ForceDodge;                          // 0x2DC  "wh_cs_ForceDodge" default=0 flags=2  Force AI to dodge by: 0 - disabled, 1 - back, 2 - right, 3 - left
    int32_t  wh_cs_ForceAttack;                         // 0x2E0  "wh_cs_ForceAttack" default=0 flags=2  Force AI to Attack by: 0 - disabled, 1 - riposte, 2 - attack every 8s and, riposte,  3 ...
    int32_t  wh_cs_AIForceBlockMode;                    // 0x2E4  "wh_cs_AIForceBlockMode" default=0 flags=2  Force AI block mode trigger
    int32_t  wh_cs_AIReblockMode;                       // 0x2E8  "wh_cs_AIReblockMode" default=0 flags=2  AI reblock mode: 0 - never, 1 - always, 2 - random 50% chance
    int32_t  wh_cs_AIForceProfile;                      // 0x2EC  "wh_cs_AIForceProfile" default=1 flags=2  Force AI brain profile from table.
    int32_t  wh_cs_AIMoveMode;                          // 0x2F0  "wh_cs_AIMoveMode" default=1 flags=2  AI Move mode: 1-None, 2-Around, 3-Pursuit
    float    wh_cs_AIMoveSpeed;                         // 0x2F4  "wh_cs_AIMoveSpeed" default=0.6f flags=2  AI Move speed
    float    wh_cs_AIMoveTimer;                         // 0x2F8  "wh_cs_AIMoveTimer" default=0.6f flags=2  AI Move time for 'Around' mode - duration of one half-cycle.
    float    wh_cs_AIMoveDistance;                      // 0x2FC  "wh_cs_AIMoveDistance" default=2.6f flags=2  AI Move distance for 'Pusuit' mode - opp will stop at this distance.
    float    wh_cs_DelayedBlock;                        // 0x300  "wh_cs_DelayedBlock" default=2.6f flags=2  Delay for AI blocks in s.
    float    wh_cs_DelayedPerfectBlockInSlot;           // 0x304  "wh_cs_DelayedPerfectBlockInSlot" default=2.6f flags=2  Delay for AI blocks in slot <0, 1>. Enabled with PB blocks.
    float    wh_cs_DelayedMasterStrikeInSlot;           // 0x308  "wh_cs_DelayedMasterStrikeInSlot" default=2.6f flags=2  Delay for auto master strike in slot <0, 1>. Enabled with with in master strike slot
    float    wh_cs_DelayedRiposteInSlot;                // 0x30C  "wh_cs_DelayedRiposteInSlot" default=2.6f flags=2  Delay for auto riposte in slot <0, 1>. -1 for disable
    float    wh_cs_AIAttackInterval;                    // 0x310  "wh_cs_AIAttackInterval" default=8.0f flags=2  Time interval between opponent's attacks
    float    wh_cs_AIAttackFiglProb;                    // 0x314  "wh_cs_AIAttackFiglProb" default=0.5f flags=2  Probability that AI will be making figl when AIForceAttack=5.
    int32_t  wh_cs_AIForceSymetric;                     // 0x318  "wh_cs_AIForceSymetric" default=0 flags=2  Force AI to get symetric force zone.
    int32_t  wh_cs_ForceZone;                           // 0x31C  "wh_cs_ForceZone" default=-1 flags=2  Force combat zone selection by: -1 - disabled, 0-4 zones z0-z4
    int64_t  pCVar_wh_cs_DebugOpponent;                 // 0x320  ICVar* "wh_cs_DebugOpponent"  Set debug opponent which can then controlled with a keyboard.
    float    wh_cs_AIDistanceTolerance;                 // 0x328  "wh_cs_AIDistanceTolerance" default=0.0f flags=2  Attack distance tolerance for AI.
    float    wh_cs_PlayerDistanceTolerance;             // 0x32C  "wh_cs_PlayerDistanceTolerance" default=0.35f flags=2  Attack distance tolerance for Player.
    float    wh_cs_MaxAttackDistanceMoveOffset;         // 0x330  "wh_cs_MaxAttackDistanceMoveOffset" default=2.0f flags=2  Max offset for dynamic move part of attack distance computation.
    float    wh_cs_MaxAttackDistanceMoveOffsetStiffness; // 0x334  "wh_cs_MaxAttackDistanceMoveOffsetStiffness" default=0.2f flags=2  Spring stiffness for smotting of max attack distance.
    int32_t  wh_cs_MaxAttackDistanceDebug;              // 0x338  "wh_cs_MaxAttackDistanceDebug" default=0 flags=2  Debug draw for max attack distance.
    float    wh_cs_MaxAttackRotationDeviation;          // 0x33C  "wh_cs_MaxAttackRotationDeviation" default=0.2f flags=2  Max angle deviation from direct dir to opponent [degrees].
    float    wh_cs_AttackTimeToHitEst;                  // 0x340  "wh_cs_AttackTimeToHitEst" default=0.4f flags=2  Average duration of attack time to hit.
    float    wh_cs_SyncGuardAutoTriggerMinVelocity;     // 0x344  "wh_cs_SyncGuardAutoTriggerMinVelocity" default=0.0f flags=2  Min velocity for autotriggering sync guards.
    float    wh_cs_SyncGuardAutoTriggerMaxVelocity;     // 0x348  "wh_cs_SyncGuardAutoTriggerMaxVelocity" default=4.0f flags=2  Max velocity for autotriggering sync guards.
    float    wh_cs_SyncGuardAutoTriggerMoveWeight;      // 0x34C  "wh_cs_SyncGuardAutoTriggerMoveWeight" default=1.8f flags=2  Coef for movement part pushing limit of auto sync guards.
    float    wh_cs_SyncGuardMaxXYAngle;                 // 0x350  "wh_cs_SyncGuardMaxXYAngle" default=0.6981317f flags=2  Maximal XY angle between combatants for sync guards.
    float    wh_cs_SyncGuardMaxZAngle;                  // 0x354  "wh_cs_SyncGuardMaxZAngle" default=0.6981317f flags=2  Maximal Z angle between combatants for sync guards.
    float    wh_cs_SyncGuardkDistanceTolerance;         // 0x358  "wh_cs_SyncGuardkDistanceTolerance" default=0.2f flags=2  Max distance from optimal sync distance to approximate angle in that distance.
    float    wh_cs_SyncGuardBackoffTimer;               // 0x35C  "wh_cs_SyncGuardBackoffTimer" default=2.0f flags=2  After this time from start of sync guard will be backoff action available.
    float    wh_cs_LockedAttackMaxAngle;                // 0x360  "wh_cs_LockedAttackMaxAngle" default=1.0471976f flags=2  Maximal angle between combatants for locked attacks.
    float    wh_cs_LockedAttackMaxAngleDogVsNPC;        // 0x364  "wh_cs_LockedAttackMaxAngleDogVsNPC" default=0.34906584f flags=2  Maximal angle between combatants for locked attacks when attacker is dog and opponent NPC.
    float    wh_cs_LockedAttackMaxAngleDogVsPlayer;     // 0x368  "wh_cs_LockedAttackMaxAngleDogVsPlayer" default=0.6981317f flags=2  Maximal angle between combatants for locked attacks  when attacker is dog and opponent ...
    float    wh_cs_LockedAttackZAngle;                  // 0x36C  "wh_cs_LockedAttackZAngle" default=1.0471976f flags=2  Maximal Z angle between combatants for locked attacks.
    float    wh_cs_LogicalBlockMaxAngle;                // 0x370  "wh_cs_LogicalBlockMaxAngle" default=1.0471976f flags=2  Maximal angle between combatants logical block.
    float    wh_cs_LockedAttackZTolerance;              // 0x374  "wh_cs_LockedAttackZTolerance" default=0.3f flags=2  Max Z difference between combatants to ignore angle test.
    float    wh_cs_LockedAttackDistanceTolerance;       // 0x378  "wh_cs_LockedAttackDistanceTolerance" default=1.0f flags=2  Max distance from optimal hit distance to approximate angle in that distance.
    float    wh_cs_IntentionalTargetAngleDefault;       // 0x37C  "wh_cs_IntentionalTargetAngleDefault" default=0.4f flags=2  Angle to detect intentional hit for non-player-actors.
    float    wh_cs_IntentionalTargetAngleMin;           // 0x380  "wh_cs_IntentionalTargetAngleMin" default=0.4f flags=2  Minimal angle to detect intentional hit when in radius around actor is IntentionalTarge...
    float    wh_cs_IntentionalTargetAngleMax;           // 0x384  "wh_cs_IntentionalTargetAngleMax" default=0.4f flags=2  Max angle to detect intentional hit when in radius around actor is 0 actors.
    float    wh_cs_IntentionalTargetAngleLimitOpponentCount; // 0x388  "wh_cs_IntentionalTargetAngleLimitOpponentCount" default=0.4f flags=2  Number of actors near target actos as threshold for lerp <min,max> angle.
    float    wh_cs_IntentionalTargetAngleRadius;        // 0x38C  "wh_cs_IntentionalTargetAngleRadius" default=1.4f flags=2  Radius around actor to detect other actors.
    float    wh_cs_RepeatHitTimeout;                    // 0x390  "wh_cs_RepeatHitTimeout" default=1.5f flags=2  Timeout for repeated hit to the same entity.
    float    wh_cs_CanAlmostHitOffset;                  // 0x394  "wh_cs_CanAlmostHitOffset" default=2.0f flags=2  Offset from max attack distance limiting area for potential threat.
    float    wh_cs_NoCombatUnarmedAlignmentOffset;      // 0x398  "wh_cs_NoCombatUnarmedAlignmentOffset" default=-0.3f flags=2  Alignment offset in unarmed combat when one combatant is not in combat.
    float    wh_cs_NoCombatArmedAlignmentOffset;        // 0x39C  "wh_cs_NoCombatArmedAlignmentOffset" default=-0.2f flags=2  Alignment offset in unarmed combat when one combatant is not in combat.
    float    wh_cs_UnarmedVsArmedAlignmentOffsetOppositeStance; // 0x3A0  "wh_cs_UnarmedVsArmedAlignmentOffsetOppositeStance" default=-0.14f flags=2  Alignment offset in unarmed combat when opponent is armed.
    float    wh_cs_UnarmedVsArmedAlignmentOffsetSameStance; // 0x3A4  "wh_cs_UnarmedVsArmedAlignmentOffsetSameStance" default=-0.12f flags=2  Alignment offset in unarmed combat when opponent is armed.
    float    wh_cs_DogClinchOpponentAlignmentOffset;    // 0x3A8  "wh_cs_DogClinchOpponentAlignmentOffset" default=-0.5f flags=2  Alignment offset when my opponent is in clinch with a dog.
    int32_t  wh_cs_UnarmedTargetToHeadBone;             // 0x3AC  "wh_cs_UnarmedTargetToHeadBone" default=1 flags=2  Rotation alignment target headbone instead of reference.
    float    wh_cs_ClenchedHandOverlapTime;             // 0x3B0  "wh_cs_ClenchedHandOverlapTime" default=0.2f flags=2  Overlap in seconds for clenched hand action after request ends.
    int32_t  wh_cs_OnlyBigTurns;                        // 0x3B4  "wh_cs_OnlyBigTurns" default=0 flags=2  Use only big turns animations during combat.
    float    wh_cs_BigTurnThreshold;                    // 0x3B8  "wh_cs_BigTurnThreshold" default=40.0f flags=2  Big turn animation is used to turn a character above this angle.
    float    wh_cs_MinimalTurnThreshold;                // 0x3BC  "wh_cs_MinimalTurnThreshold" default=30.0f flags=2  Minimal angle required to initiate turning.
    float    wh_cs_MinimalTurnThresholdByTime;          // 0x3C0  "wh_cs_MinimalTurnThresholdByTime" default=14.0f flags=2  Minimal angle required to initiate turning after time.
    float    wh_cs_MinimalTurnThresholdWhenTurning;     // 0x3C4  "wh_cs_MinimalTurnThresholdWhenTurning" default=7.0f flags=2  Minimal angle required finish tutning when already turning.
    float    wh_cs_MinIdleTurnTimeout;                  // 0x3C8  "wh_cs_MinIdleTurnTimeout" default=1.0f flags=2  Minimal timeout before automatic idle turns.
    float    wh_cs_MaxIdleTurnTimeout;                  // 0x3CC  "wh_cs_MaxIdleTurnTimeout" default=2.0f flags=2  Maximal timeout before automatic idle turns.
    float    wh_cs_IdleTurnPredictionTimeout;           // 0x3D0  "wh_cs_IdleTurnPredictionTimeout" default=0.4f flags=2  Small amount of time to postpone turn angle prediction.
    float    wh_cs_IdleTurnInterruptionThreshold;       // 0x3D4  "wh_cs_IdleTurnInterruptionThreshold" default=60.0f flags=2  Threshold angle to requeue idle turn animations with trumping.
    float    wh_cs_IdleTurnDuration;                    // 0x3D8  "wh_cs_IdleTurnDuration" default=1.5f flags=2  Threshold angle to requeue idle turn animations with trumping.
    float    wh_cs_SignificantDeviationAngle;           // 0x3DC  "wh_cs_SignificantDeviationAngle" default=30.0f flags=2  Allow idle turn when deviation is greater than this value even when movement restrictio...
    int32_t  wh_cs_DebugIdleTurns;                      // 0x3E0  "wh_cs_DebugIdleTurns" default=0 flags=2  Debug idle turns.
    int32_t  wh_cs_WeaponProcSolver;                    // 0x3E4  "wh_cs_WeaponProcSolver" default=1 flags=2  Procedural weapon impact solver.
    float    wh_cs_WeaponProcSolverRecoveryTime;        // 0x3E8  "wh_cs_WeaponProcSolverRecoveryTime" default=0.3f flags=2  Procedural weapon impact solver parameter - recovery time.
    float    wh_cs_WeaponProcSolverRotRecoveryTimeMin;  // 0x3EC  "wh_cs_WeaponProcSolverRotRecoveryTimeMin" default=0.1f flags=2  Procedural weapon impact solver parameter - rotation recovery time max.
    float    wh_cs_WeaponProcSolverRotRecoveryTimeMax;  // 0x3F0  "wh_cs_WeaponProcSolverRotRecoveryTimeMax" default=0.3f flags=2  Procedural weapon impact solver parameter - rotation recovery time min.
    float    wh_cs_WeaponProcSolverRecoveryIKBlendThreshold; // 0x3F4  "wh_cs_WeaponProcSolverRecoveryIKBlendThreshold" default=0.5f flags=2  Procedural weapon impact solver parameter - rotation recovery blend threshold for IK bl...
    float    wh_cs_WeaponProcSolverRotSmoothTime;       // 0x3F8  "wh_cs_WeaponProcSolverRotSmoothTime" default=20.0f flags=2  Procedural weapon impact solver parameter - start rotation recovery time of smoother [l...
    float    wh_cs_WeaponProcSolverRotInterpolatorSel;  // 0x3FC  "wh_cs_WeaponProcSolverRotInterpolatorSel" default=0.1f flags=2  Procedural weapon impact solver parameter - weight betwen two interpolators - following...
    float    wh_cs_WeaponProcSolverRotRecoverySharpness; // 0x400  "wh_cs_WeaponProcSolverRotRecoverySharpness" default=0.8f flags=2  Procedural weapon impact solver parameter - sharpness of rot recovery - coef K in x^K..
    float    wh_cs_WeaponProcSolverRotAnimDelaySharpness; // 0x404  "wh_cs_WeaponProcSolverRotAnimDelaySharpness" default=0.3f flags=2  Procedural weapon impact solver parameter - sharpness of delay of anim controlling rot ...
    float    wh_cs_WeaponProcSolverRotMaxAnimDeviationAng; // 0x408  "wh_cs_WeaponProcSolverRotMaxAnimDeviationAng" default=120.0f flags=2  Procedural weapon impact solver parameter - max delay of targeting animation to real an...
    float    wh_cs_WeaponProcSolverPosOffhandFade;      // 0x40C  "wh_cs_WeaponProcSolverPosOffhandFade" default=0.1f flags=2  Procedural weapon impact solver parameter - fade in/out time of offhand position change
    float    wh_cs_WeaponProcSolverImpactArea;          // 0x410  "wh_cs_WeaponProcSolverImpactArea" default=0.1f flags=2  Procedural weapon impact solver parameter - impact area that is used for computation of...
    float    wh_cs_WeaponProcSolverColExtraOffset;      // 0x414  "wh_cs_WeaponProcSolverColExtraOffset" default=0.0f flags=2  Procedural weapon impact solver parameter - extra offset to avoid physical touch betwee...
    float    wh_cs_WeaponProcSolverColBlendLimiter;     // 0x418  "wh_cs_WeaponProcSolverColBlendLimiter" default=0.5f flags=2  Procedural weapon impact solver parameter - blend limit for accepting collisions
    float    wh_cs_WeaponProcSolverRecoilEffectDist;    // 0x41C  "wh_cs_WeaponProcSolverRecoilEffectDist" default=0.0f flags=2  Procedural weapon impact solver parameter - distance for first recoil effect
    float    wh_cs_WeaponProcSolverRecoilEffectTime;    // 0x420  "wh_cs_WeaponProcSolverRecoilEffectTime" default=0.5f flags=2  Procedural weapon impact solver parameter - time for first recoil effect
    int32_t  wh_cs_WeaponProcSolverDebug;               // 0x424  "wh_cs_WeaponProcSolverDebug" default=0 flags=2  Enables debug draw for procedural weapon impact solver
    int32_t  wh_cs_WeaponProcSolverMethod;              // 0x428  "wh_cs_WeaponProcSolverMethod" default=1 flags=2  Switches solver mode = 0 - 1. first collision, 2. continous collisions
    float    wh_cs_WeaponProcSolverNearColDist;         // 0x42C  "wh_cs_WeaponProcSolverNearColDist" default=0.12f flags=2  Procedural weapon impact solver parameter - distance from weapon bone where all collisi...
    float    wh_cs_WeaponProcSolverNearColDistVisualFilter; // 0x430  "wh_cs_WeaponProcSolverNearColDistVisualFilter" default=0.25f flags=2  Procedural weapon impact solver parameter - distance from weapon bone where all collisi...
    float    wh_cs_WeaponProcSolverColFilterFOVOBias;   // 0x434  "wh_cs_WeaponProcSolverColFilterFOVOBias" default=0.0f flags=2  Procedural weapon impact solver parameter - bias for fov angle filtering collisions
    int32_t  wh_cs_WeaponProcSolverDebugCollisions;     // 0x438  "wh_cs_WeaponProcSolverDebugCollisions" default=0 flags=2  Procedural weapon impact solver- enables collision debug draw
    float    wh_cs_ArmWeaponRadius;                     // 0x43C  "wh_cs_ArmWeaponRadius" default=0.13f flags=2  Radius of unarmed weapon (sphere).
    float    wh_cs_LegWeaponRadius;                     // 0x440  "wh_cs_LegWeaponRadius" default=0.14f flags=2  Radius of unarmed leg (sphere).
    float    wh_cs_RiderWeaponCollisionLen;             // 0x444  "wh_cs_RiderWeaponCollisionLen" default=0.5f flags=2  Lenght of extra collision for weapon held by rider to have better range.
    float    wh_cs_WeaponCollisionCCDPosBias;           // 0x448  "wh_cs_WeaponCollisionCCDPosBias" default=0.2f flags=2  Collidion weapon position bias value used to eliminate CCD error.
    int32_t  wh_cs_WeaponProcSolverProfiling;           // 0x44C  "wh_cs_WeaponProcSolverProfiling" default=0 flags=2  Enables profiling of weapon solver.
    int32_t  wh_cs_DBQueryProfiling;                    // 0x450  "wh_cs_DBQueryProfiling" default=0 flags=2  Enables profiling of database queries.
    int32_t  wh_cs_DBQueryCache;                        // 0x454  "wh_cs_DBQueryCache" default=0 flags=2  Enables cache for database queries.
    float    wh_cs_WeaponProcSolverLod1Duration;        // 0x458  "wh_cs_WeaponProcSolverLod1Duration" default=0.3f flags=2  Limits solver simulation for given duration before expected hit
    int32_t  wh_cs_WeaponProcSolverForceLOD;            // 0x45C  "wh_cs_WeaponProcSolverForceLOD" default=0 flags=2  Forces LOD - 0 disabled, 1 limited colliding time, 2 no collision
    int32_t  wh_cs_WeaponProcDisableOffhand;            // 0x460  "wh_cs_WeaponProcDisableOffhand" default=1 flags=2  Forces disabled offhand for weapon solver
    float    wh_cs_BehindCollisionIgnoreThreshold;      // 0x464  "wh_cs_BehindCollisionIgnoreThreshold" default=0.1f flags=2  Ignores all collision that y-coord is lesser than the value.
    int32_t  wh_cs_WeaponSolver;                        // 0x468  "wh_cs_WeaponSolver" default=1 flags=2  Procedural weapon impact solver.
    float    wh_cs_WeaponSolverRecoveryTime;            // 0x46C  "wh_cs_WeaponSolverRecoveryTime" default=0.5f flags=2  Procedural weapon impact solver parameter - recovery time.
    float    wh_cs_WeaponSolverRotRecoveryTimeMin;      // 0x470  "wh_cs_WeaponSolverRotRecoveryTimeMin" default=0.1f flags=2  Procedural weapon impact solver parameter - rotation recovery time max.
    float    wh_cs_WeaponSolverRotRecoveryTimeMax;      // 0x474  "wh_cs_WeaponSolverRotRecoveryTimeMax" default=0.3f flags=2  Procedural weapon impact solver parameter - rotation recovery time min.
    float    wh_cs_WeaponSolverRecoveryIKBlendThreshold; // 0x478  "wh_cs_WeaponSolverRecoveryIKBlendThreshold" default=0.5f flags=2  Procedural weapon impact solver parameter - rotation recovery blend threshold for IK bl...
    float    wh_cs_WeaponSolverRotSmoothTime;           // 0x47C  "wh_cs_WeaponSolverRotSmoothTime" default=20.0f flags=2  Procedural weapon impact solver parameter - start rotation recovery time of smoother [l...
    float    wh_cs_WeaponSolverRotInterpolatorSel;      // 0x480  "wh_cs_WeaponSolverRotInterpolatorSel" default=0.1f flags=2  Procedural weapon impact solver parameter - weight betwen two interpolators - following...
    float    wh_cs_WeaponSolverRotRecoverySharpness;    // 0x484  "wh_cs_WeaponSolverRotRecoverySharpness" default=0.8f flags=2  Procedural weapon impact solver parameter - sharpness of rot recovery - coef K in x^K..
    float    wh_cs_WeaponSolverRotAnimDelaySharpness;   // 0x488  "wh_cs_WeaponSolverRotAnimDelaySharpness" default=0.3f flags=2  Procedural weapon impact solver parameter - sharpness of delay of anim controlling rot ...
    float    wh_cs_WeaponSolverRotMaxAnimDeviationAng;  // 0x48C  "wh_cs_WeaponSolverRotMaxAnimDeviationAng" default=120.0f flags=2  Procedural weapon impact solver parameter - max delay of targeting animation to real an...
    float    wh_cs_WeaponSolverPosOffhandFade;          // 0x490  "wh_cs_WeaponSolverPosOffhandFade" default=0.1f flags=2  Procedural weapon impact solver parameter - fade in/out time of offhand position change
    float    wh_cs_WeaponSolverImpactArea;              // 0x494  "wh_cs_WeaponSolverImpactArea" default=0.1f flags=2  Procedural weapon impact solver parameter - impact area that is used for computation of...
    float    wh_cs_WeaponSolverColExtraOffset;          // 0x498  "wh_cs_WeaponSolverColExtraOffset" default=0.0f flags=2  Procedural weapon impact solver parameter - extra offset to avoid physical touch betwee...
    float    wh_cs_WeaponSolverColBlendLimiter;         // 0x49C  "wh_cs_WeaponSolverColBlendLimiter" default=0.5f flags=2  Procedural weapon impact solver parameter - blend limit for accepting collisions
    float    wh_cs_WeaponSolverRecoilEffectDist;        // 0x4A0  "wh_cs_WeaponSolverRecoilEffectDist" default=0.0f flags=2  Procedural weapon impact solver parameter - distance for first recoil effect
    int32_t  wh_cs_WeaponSolverDebug;                   // 0x4A4  "wh_cs_WeaponSolverDebug" default=0 flags=2  Enables debug draw for procedural weapon impact solver
    int32_t  wh_cs_WeaponSolverMethod;                  // 0x4A8  "wh_cs_WeaponSolverMethod" default=1 flags=2  Switches solver mode = 0 - 1. first collision, 2. continous collisions
    float    wh_cs_WeaponSolverNearColDist;             // 0x4AC  "wh_cs_WeaponSolverNearColDist" default=0.12f flags=2  Procedural weapon impact solver parameter - distance from weapon bone where all collisi...
    float    wh_cs_WeaponSolverNearColDistVisualFilter; // 0x4B0  "wh_cs_WeaponSolverNearColDistVisualFilter" default=0.25f flags=2  Procedural weapon impact solver parameter - distance from weapon bone where all collisi...
    float    wh_cs_WeaponSolverColFilterFOVOBias;       // 0x4B4  "wh_cs_WeaponSolverColFilterFOVOBias" default=0.0f flags=2  Procedural weapon impact solver parameter - bias for fov angle filtering collisions
    int32_t  wh_cs_WeaponSolverDebugCollisions;         // 0x4B8  "wh_cs_WeaponSolverDebugCollisions" default=0 flags=2  Procedural weapon impact solver- enables collision debug draw
    uint8_t  _pad_4BC[0x4];                    // 0x4BC
    int32_t  wh_cs_WeaponSolverProfiling;               // 0x4C0  "wh_cs_WeaponSolverProfiling" default=0 flags=2  Enables profiling of weapon solver.
    float    wh_cs_WeaponSolverLod1Duration;            // 0x4C4  "wh_cs_WeaponSolverLod1Duration" default=0.3f flags=2  Limits solver simulation for given duration before expected hit
    int32_t  wh_cs_WeaponSolverForceMode;               // 0x4C8  "wh_cs_WeaponSolverForceMode" default=-1 flags=2  Forced procedural weapon mode. -1 means no force.
    int32_t  wh_cs_WeaponSolverForceLOD;                // 0x4CC  "wh_cs_WeaponSolverForceLOD" default=0 flags=2  Forces LOD - 0 disabled, 1 limited colliding time, 2 no collision
    float    wh_cs_WeaponSolverForcePenetration;        // 0x4D0  "wh_cs_WeaponSolverForcePenetration" default=-1.0f flags=2  Forces penetration of weapon into body - given number: -1 = disabled force, 0 = force n...
    float    wh_cs_WeaponSolverForceStuckDelaySharpness; // 0x4D4  "wh_cs_WeaponSolverForceStuckDelaySharpness" default=2.0f flags=2  Procedural weapon impact solver forced parameter - sharpness of delay of anim controlli...
    float    wh_cs_WeaponSolverForceRecoilDuration;     // 0x4D8  "wh_cs_WeaponSolverForceRecoilDuration" default=-1.0f flags=2  Procedural weapon impact solver forced parameter - recoil effect duration
    float    wh_cs_WeaponSolverForceRecoilIntensityPos; // 0x4DC  "wh_cs_WeaponSolverForceRecoilIntensityPos" default=-1.0f flags=2  Procedural weapon impact solver forced parameter - pos intensity of recoil effect (how ...
    float    wh_cs_WeaponSolverForceRecoilIntensityRot; // 0x4E0  "wh_cs_WeaponSolverForceRecoilIntensityRot" default=-1.0f flags=2  Procedural weapon impact solver forced parameter - rot intensity of recoil effect (how ...
    int32_t  wh_cs_WeaponDisableOffhand;                // 0x4E4  "wh_cs_WeaponDisableOffhand" default=1 flags=2  Forces disabled offhand for weapon solver
    float    wh_cs_WeaponSolverSlidingDuration;         // 0x4E8  "wh_cs_WeaponSolverSlidingDuration" default=0.3f flags=2  Duration of weapon sliding phase.
    float    wh_cs_WeaponSolverRecoveryDuration;        // 0x4EC  "wh_cs_WeaponSolverRecoveryDuration" default=0.3f flags=2  Duration of weapon recovery phase.
    int32_t  wh_cs_WeaponSolverEnableMultipleCollisions; // 0x4F0  "wh_cs_WeaponSolverEnableMultipleCollisions" default=1 flags=2  Enables multicollision detection.
    int32_t  wh_cs_WeaponSolverForceEnableMultipleCollisionsOnWeapon; // 0x4F4  "wh_cs_WeaponSolverForceEnableMultipleCollisionsOnWeapon" default=0 flags=2  Force enables multicollision detection into weapon.
    float    wh_cs_TimeAlignment_MaxBlockSpeedBias;     // 0x4F8  "wh_cs_TimeAlignment_MaxBlockSpeedBias" default=10.0f flags=2  Maximal value of speed bias for time dilatation in blocks
    float    wh_cs_TimeAlignment_MinBlockSpeedBias;     // 0x4FC  "wh_cs_TimeAlignment_MinBlockSpeedBias" default=0.5f flags=2  Minimal value of speed bias for time dilatation in blocks
    float    wh_cs_TimeAlignment_BlockSpeedMod;         // 0x500  "wh_cs_TimeAlignment_BlockSpeedMod" default=0.8f flags=2  Modifier for blocking time alignment to right state switch
    float    wh_cs_TimeAlignment_MaxAttackSpeedBias;    // 0x504  "wh_cs_TimeAlignment_MaxAttackSpeedBias" default=2.0f flags=2  Maximal value of speed bias for time dilatation in attacks
    float    wh_cs_TimeAlignment_MinAttackSpeedBias;    // 0x508  "wh_cs_TimeAlignment_MinAttackSpeedBias" default=0.5f flags=2  Minimal value of speed bias for time dilatation in attacks
    float    wh_cs_TimeAlignment_MaxAnimAddition;       // 0x50C  "wh_cs_TimeAlignment_MaxAnimAddition" default=0.5f flags=2  Maximal time which can be added to running animation to move sync point.
    float    wh_cs_TimeAlignment_MinConstSpeedBias;     // 0x510  "wh_cs_TimeAlignment_MinConstSpeedBias" default=0.1f flags=2  Minimal const speed bias for time addition in timing.
    float    wh_cs_TimeAlignment_ImpulseMinSpeed;       // 0x514  "wh_cs_TimeAlignment_ImpulseMinSpeed" default=0.8f flags=2  Minimal const speed bias for time addition by impulse solver.
    float    wh_cs_TimeAlignment_SlotDelayDuration;     // 0x518  "wh_cs_TimeAlignment_SlotDelayDuration" default=0.07f flags=2  This slowdown animation around place defined by 'SlotDelay' combat event proc clip.
    int32_t  wh_cs_EnableStealthActions;                // 0x51C  "wh_cs_EnableStealthActions" default=1 flags=2  Enables stealth actions
    int32_t  wh_cs_CombatCameraStyle;                   // 0x520  "wh_cs_CombatCameraStyle" default=1 flags=2  Combat camera style [0] - Fixed at opponent, [1] - Dynamic
    int32_t  wh_cs_DisableCombatCamera;                 // 0x524  "wh_cs_DisableCombatCamera" default=0 flags=2  Disables combat camera.
    float    wh_cs_CombatAnimCameraSmooth;              // 0x528  "wh_cs_CombatAnimCameraSmooth" default=0.1f flags=2  Combat anim camera smoothing.
    float    wh_cs_CombatAnimCameraBlendOut;            // 0x52C  "wh_cs_CombatAnimCameraBlendOut" default=0.4f flags=2  Combat anim camera blend out [s].
    float    wh_cs_Combat3PCameraFOV;                   // 0x530  "wh_cs_Combat3PCameraFOV" default=0.4f flags=2  Combat 3P camera default FOV.
    int32_t  wh_cs_ShowLiveSpatialGrid;                 // 0x534  "wh_cs_ShowLiveSpatialGrid" default=0 flags=2  Show runtime spatial grid for debugging.
    int32_t  wh_cs_ShowPreprocessedSpatialGrid;         // 0x538  "wh_cs_ShowPreprocessedSpatialGrid" default=0 flags=2  Show preprocessed spatial grid for debugging.
    float    wh_cs_SpatialGridBlendSpace;               // 0x53C  "wh_cs_SpatialGridBlendSpace" default=0.2f flags=2  Determines blendspace of debug spatial grid to show.
    float    wh_cs_SpatialGridRadius;                   // 0x540  "wh_cs_SpatialGridRadius" default=1.0f flags=2  Spatial grid radius.
    float    wh_cs_SpatialGridWeaponLength;             // 0x544  "wh_cs_SpatialGridWeaponLength" default=0.5f flags=2  Length of weapon to consider in spatial grid.
    float    wh_cs_SpatialGridBlendSpaceDistance;       // 0x548  "wh_cs_SpatialGridBlendSpaceDistance" default=0.25f flags=2  Distance between blendspace samples for grid array (in metres).
    uint8_t  _pad_54C[0x4];                    // 0x54C
    int64_t  pCVar_wh_cs_SpatialGridDisection;          // 0x550  ICVar* "wh_cs_SpatialGridDisection"  32Spatial grid disections (333 or 332 etc)
    int64_t  pCVar_wh_cs_SpatialGridDisectionPlayerAddition; // 0x558  ICVar* "wh_cs_SpatialGridDisectionPlayerAddition"  2Spatial grid suffix used for player (333 or 332 etc)
    int32_t  wh_cs_ShowEnvSpatialGrid;                  // 0x560  "wh_cs_ShowEnvSpatialGrid" default=0 flags=4  Enable visual debug of environment.
    float    wh_cs_EnvSpatialGridRefreshRate;           // 0x564  "wh_cs_EnvSpatialGridRefreshRate" default=0.1f flags=2  Determines how often is updated environmental spatial grid (in sec)
    int64_t  pCVar_wh_cs_DebugObstacleTestTarget;       // 0x568  ICVar* "wh_cs_DebugObstacleTestTarget"  0Enables debug draw for obstacles test on given entity.
    int32_t  wh_cs_DebugObstacleMask;                   // 0x570  "wh_cs_DebugObstacleMask" default=0 flags=2  Allow filter obstacle test to debug draw.
    int32_t  wh_cs_SpatialGridPreprocess;               // 0x574  "wh_cs_SpatialGridPreprocess" default=1 flags=2  Enables spatial analyzation during preprocessing.
    int32_t  wh_cs_EnableNavmeshTest;                   // 0x578  "wh_cs_EnableNavmeshTest" default=1 flags=2  Enables navmesh tests for some action types.
    float    wh_cs_DisabledAttackTimeout;               // 0x57C  "wh_cs_DisabledAttackTimeout" default=0.0f flags=2  Timeout to interrupt attack after R2S.
    int32_t  wh_cs_DisabledAttackDebug;                 // 0x580  "wh_cs_DisabledAttackDebug" default=0 flags=2  Forces interrupted attacks for debugging.
    float    wh_cs_ThreatenedTargetsSelectionDistanceBias; // 0x584  "wh_cs_ThreatenedTargetsSelectionDistanceBias" default=-0.1f flags=2  Distance bias which is added to max attack distance to limit free attack threatening.
    float    wh_cs_ThreatenedTargetsSelectionDistanceOverride; // 0x588  "wh_cs_ThreatenedTargetsSelectionDistanceOverride" default=-0.1f flags=2  Override distance when free attacks are considered as threatening. 0 = off, max attack ...
    float    wh_cs_ThreatenedTargetsSelectionFOV;       // 0x58C  "wh_cs_ThreatenedTargetsSelectionFOV" default=80.0f flags=2  FOV angle along player dir that limits selection of threatened actors.
    int64_t  pCVar_wh_cs_DebugFilter;                   // 0x590  ICVar* "wh_cs_DebugFilter"  BASIC|MODEL|TIME_COP|ACTOR_ACTION|ACTOR_ACTION_HELPER|PROCEDURAL_CLIP|ALIGNMENT|MODIFIE...
    int32_t  wh_cs_TraceMNDebug;                        // 0x598  "wh_cs_TraceMNDebug" default=0 flags=2  Trace mn_debugs of all CA in combat to trace.
    int32_t  wh_cs_ProcessSyncHits;                     // 0x59C  "wh_cs_ProcessSyncHits" default=0 flags=4  Enabled update sync hits. It is disabled by default since this operation should do only...
    int64_t  pCVar_wh_cs_DebugIsAimedOn;                // 0x5A0  ICVar* "wh_cs_DebugIsAimedOn"  Enables state IsAimed on given actor.
    int64_t  pCVar_wh_cs_DebugIsAimedOff;               // 0x5A8  ICVar* "wh_cs_DebugIsAimedOff"  Disables state IsAimed on given actor.
    int32_t  wh_cs_DebugDrawLods;                       // 0x5B0  "wh_cs_DebugDrawLods" default=0 flags=4  Enables lod manager debug draw
    float    wh_cs_ForceLod;                            // 0x5B4  "wh_cs_ForceLod" default=0.3f flags=4  Forces combat LOD: 0..high, 1..med, 2..low
    float    wh_cs_LodNearDistance;                     // 0x5B8  "wh_cs_LodNearDistance" default=8.0f flags=4  Distance limit for near lod.
    float    wh_cs_LodFarDistance;                      // 0x5BC  "wh_cs_LodFarDistance" default=14.0f flags=4  Distance limit for far lod.
    float    wh_cs_LodAmbientDistance;                  // 0x5C0  "wh_cs_LodAmbientDistance" default=20.0f flags=4  Distance limit for ambient lod.
    float    wh_cs_LodBattleDistanceModifier;           // 0x5C4  "wh_cs_LodBattleDistanceModifier" default=0.7f flags=4  Distance modifier for lod distances in battle.
    float    wh_cs_CursorOffsetNPC;                     // 0x5C8  "wh_cs_CursorOffsetNPC" default=1.2f flags=4  Z offset of combat cursor for NPC.
    float    wh_cs_CursorOffsetDog;                     // 0x5CC  "wh_cs_CursorOffsetDog" default=0.3f flags=4  Z offset of combat cursor for dog.
    float    wh_cs_AutomationAction_DelayedEnd;         // 0x5D0  "wh_cs_AutomationAction_DelayedEnd" default=2.0f flags=2  Combat automation - when an opponent is not threat any more than defense actions (prebl...
    int32_t  wh_cs_AutomationAction_StaticPreblockEnabled; // 0x5D4  "wh_cs_AutomationAction_StaticPreblockEnabled" default=0 flags=2  Combat automation - enables static preblock.
    float    wh_cs_AutomationAction_StaticPreblockUpdateInterval; // 0x5D8  "wh_cs_AutomationAction_StaticPreblockUpdateInterval" default=0.3f flags=2  Combat automation - defines time interval between static preblock decision.
    float    wh_cs_AutomationAction_StaticPreblockRateHistoryDuration; // 0x5DC  "wh_cs_AutomationAction_StaticPreblockRateHistoryDuration" default=5.0f flags=2  Combat automation - defines history duration for computing rate between preblocks and b...
    float    wh_cs_AutomationAction_AttackUpdateInterval; // 0x5E0  "wh_cs_AutomationAction_AttackUpdateInterval" default=0.2f flags=2  Combat automation - defines frequency for updating attacking params.
    float    wh_cs_AutomationAction_AttackAfterComboTime; // 0x5E4  "wh_cs_AutomationAction_AttackAfterComboTime" default=1.4f flags=2  Combat automation - defines time after combo end before new normal attack.
    float    wh_cs_AutomationAction_AttackImmediateDistance; // 0x5E8  "wh_cs_AutomationAction_AttackImmediateDistance" default=0.8f flags=2  Combat automation - defines distance when NPC does immediate attack to keep distance fr...
    float    wh_cs_AutomationAction_DogAttackAngleLimit; // 0x5EC  "wh_cs_AutomationAction_DogAttackAngleLimit" default=0.9f flags=2  Combat automation - dog angle limitation for attack.
    float    wh_cs_AutomationAction_HigherComboStepBias; // 0x5F0  "wh_cs_AutomationAction_HigherComboStepBias" default=0.2f flags=2  Combo automation - max prob for first combo step. Prob is increased with combo step count.
    float    wh_cs_AutomationAction_ZoneChangeToAttackMinDelay; // 0x5F4  "wh_cs_AutomationAction_ZoneChangeToAttackMinDelay" default=0.5f flags=2  Combat automation - min time between zone change and attack (for visual quality).
    float    wh_cs_AutomationAction_ZoneChangeUpdateInterval; // 0x5F8  "wh_cs_AutomationAction_ZoneChangeUpdateInterval" default=0.2f flags=2  Combat automation - defines frequency for updating zone change params.
    int32_t  wh_cs_AutomationAction_ZoneChangeSequenceMinCount; // 0x5FC  "wh_cs_AutomationAction_ZoneChangeSequenceMinCount" default=2 flags=2  Combat automation - minimal sequence changes.
    int32_t  wh_cs_AutomationAction_ZoneChangeSequenceMaxCount; // 0x600  "wh_cs_AutomationAction_ZoneChangeSequenceMaxCount" default=5 flags=2  Combat automation - maximum sequence changes.
    int32_t  wh_cs_AutomationAction_ZoneChangeAttackSequenceMinCount; // 0x604  "wh_cs_AutomationAction_ZoneChangeAttackSequenceMinCount" default=2 flags=2  Combat automation - minimal preattack sequence changes.
    int32_t  wh_cs_AutomationAction_ZoneChangeAttackSequenceMaxCount; // 0x608  "wh_cs_AutomationAction_ZoneChangeAttackSequenceMaxCount" default=6 flags=2  Combat automation - maximum preattack sequence changes.
    float    wh_cs_AutomationAction_ZoneChangeSequenceMinTimeMin; // 0x60C  "wh_cs_AutomationAction_ZoneChangeSequenceMinTimeMin" default=15.0f flags=2  Combat automation - min time between natural sequences for min fencing.
    float    wh_cs_AutomationAction_ZoneChangeSequenceMaxTimeMin; // 0x610  "wh_cs_AutomationAction_ZoneChangeSequenceMaxTimeMin" default=20.0f flags=2  Combat automation - max time between natural sequences for min fencing.
    float    wh_cs_AutomationAction_ZoneChangeSequenceMinTimeMax; // 0x614  "wh_cs_AutomationAction_ZoneChangeSequenceMinTimeMax" default=4.0f flags=2  Combat automation - min time between natural sequences for max fencing.
    float    wh_cs_AutomationAction_ZoneChangeSequenceMaxTimeMax; // 0x618  "wh_cs_AutomationAction_ZoneChangeSequenceMaxTimeMax" default=8.0f flags=2  Combat automation - max time between natural sequences for max fencing.
    float    wh_cs_AutomationAction_ZoneChangeSequenceTime; // 0x61C  "wh_cs_AutomationAction_ZoneChangeSequenceTime" default=0.6f flags=2  Combat automation - safe time between sequence changes.
    float    wh_cs_AutomationAction_ZoneChangeSequenceChangeGap; // 0x620  "wh_cs_AutomationAction_ZoneChangeSequenceChangeGap" default=0.2f flags=2  Combat automation - time gap between changes.
    float    wh_cs_AutomationAction_ZoneChangeSequenceSkillBonus; // 0x624  "wh_cs_AutomationAction_ZoneChangeSequenceSkillBonus" default=0.6f flags=2  Combat automation - skill bonus for change zone timing (lower skill longer time).
    int32_t  wh_cs_AutomationAction_ZoneChangeDebugDraw; // 0x628  "wh_cs_AutomationAction_ZoneChangeDebugDraw" default=0 flags=2  Debug AI automation: Enables zone selection debug draw.
    float    wh_cs_AutomationAction_ZoneChangeQualityTimingCoef; // 0x62C  "wh_cs_AutomationAction_ZoneChangeQualityTimingCoef" default=0.5f flags=2  Debug AI automation: Coef <0, 1> for maximal speedup of zone timing for whorse combinat...
    float    wh_cs_AutomationAction_ZoneChangeComboSameZoneWeight; // 0x630  "wh_cs_AutomationAction_ZoneChangeComboSameZoneWeight" default=0.5f flags=2  Combo zone weight when zones are same.
    float    wh_cs_AutomationAction_ZoneChangeComboSameStanceWeight; // 0x634  "wh_cs_AutomationAction_ZoneChangeComboSameStanceWeight" default=1.0f flags=2  Combo zone weight when stances are same.
    float    wh_cs_AutomationAction_ZoneChangeComboOppositeStanceWeight; // 0x638  "wh_cs_AutomationAction_ZoneChangeComboOppositeStanceWeight" default=2.0f flags=2  Combo zone weight when stances are opposite.
    float    wh_cs_AutomationAction_ZoneChangePhysicalZoneTimer; // 0x63C  "wh_cs_AutomationAction_ZoneChangePhysicalZoneTimer" default=0.4f flags=2  Delay for physical zone behind guard zone.
    float    wh_cs_AutomationAction_ZoneChangeRestrictionWeight; // 0x640  "wh_cs_AutomationAction_ZoneChangeRestrictionWeight" default=0.01f flags=2  Weight multiplier for restricted zone by enviro.
    float    wh_cs_AutomationAction_ZoneChangeRestrictionTimer; // 0x644  "wh_cs_AutomationAction_ZoneChangeRestrictionTimer" default=1.0f flags=2  Checking period of zone validity by restriction.
    float    wh_cs_AutomationAction_ZoneChangeAdaptationHysteresis; // 0x648  "wh_cs_AutomationAction_ZoneChangeAdaptationHysteresis" default=1.1f flags=2  Hysteresis period when adaption (from zone change) is done before next adaption.
    int32_t  wh_cs_AutomationAction_GuardDebugDraw;     // 0x64C  "wh_cs_AutomationAction_GuardDebugDraw" default=0 flags=2  Debug AI automation: Guard automation debug draw.
    float    wh_cs_AutomationAction_GuardUpdateInterval; // 0x650  "wh_cs_AutomationAction_GuardUpdateInterval" default=0.1f flags=2  Debug AI automation: Update interval for guard autmation decision.
    float    wh_cs_AutomationAction_GuardMaxSpeed;      // 0x654  "wh_cs_AutomationAction_GuardMaxSpeed" default=2.9f flags=2  Debug AI automation: Max speed in combat guard.
    float    wh_cs_AutomationAction_GuardMinAtkDist;    // 0x658  "wh_cs_AutomationAction_GuardMinAtkDist" default=2.0f flags=2  Debug AI automation: Min atk dist for guard automation.
    float    wh_cs_AutomationAction_GuardFOV;           // 0x65C  "wh_cs_AutomationAction_GuardFOV" default=360.0f flags=2  FOV angle in degrees along actor dir that limits guard automation
    float    wh_cs_AutomationAction_GuardFOVVertical;   // 0x660  "wh_cs_AutomationAction_GuardFOVVertical" default=50.0f flags=2  Maximum possible angle to opponent before npc is unlocked from combat.
    float    wh_cs_AutomationAction_GuardFOVHyst;       // 0x664  "wh_cs_AutomationAction_GuardFOVHyst" default=5.0f flags=2  Hysteresis diameter for FOV angle in degrees along actor dir that limits guard automation
    float    wh_cs_AutomationAction_GuardEscapeMinDuration; // 0x668  "wh_cs_AutomationAction_GuardEscapeMinDuration" default=1.0f flags=2  Debug AI automation: Min actor speed for escape mode selection.
    float    wh_cs_AutomationAction_GuardRotDisableOnDodgeCooldown; // 0x66C  "wh_cs_AutomationAction_GuardRotDisableOnDodgeCooldown" default=3.0f flags=2  Timeout after opponent makes dodge to disable disabling guard by rot.
    float    wh_cs_AutomationAction_GuardApproachHystTimer; // 0x670  "wh_cs_AutomationAction_GuardApproachHystTimer" default=0.5f flags=2  Hysteresis timer to hold approach mode after real aiming ended to survive firing
    float    wh_cs_AutomationAction_GuardInjuredDistanceMod; // 0x674  "wh_cs_AutomationAction_GuardInjuredDistanceMod" default=1.4f flags=2  Guard distance is modified with mod in the injured state.
    float    wh_cs_AutomationAction_ShoutIdleUpdateInterval; // 0x678  "wh_cs_AutomationAction_ShoutIdleUpdateInterval" default=1.0f flags=2  Debug AI automation: Update interval for idle shout autmation decision.
    float    wh_cs_AutomationAction_ShoutIdleGuardCooldown; // 0x67C  "wh_cs_AutomationAction_ShoutIdleGuardCooldown" default=2.0f flags=2  Debug AI automation: Idle shouts are not palyed during this cooldown starting on enter ...
    float    wh_cs_AutomationAction_ShoutIdleGuardCooldownVariation; // 0x680  "wh_cs_AutomationAction_ShoutIdleGuardCooldownVariation" default=2.0f flags=2  Debug AI automation: Idle shouts are not palyed during this cooldown starting on enter ...
    float    wh_cs_AutomationAction_ShoutActionGuardCooldown; // 0x684  "wh_cs_AutomationAction_ShoutActionGuardCooldown" default=0.5f flags=2  Debug AI automation: Action shouts are not palyed during this cooldown starting on ente...
    float    wh_cs_AutomationAction_ShoutActionGuardCooldownVariation; // 0x688  "wh_cs_AutomationAction_ShoutActionGuardCooldownVariation" default=0.5f flags=2  Debug AI automation: Action shouts are not palyed during this cooldown starting on ente...
    float    wh_cs_AutomationAction_ShoutClinchGuardCooldown; // 0x68C  "wh_cs_AutomationAction_ShoutClinchGuardCooldown" default=0.25f flags=2  Debug AI automation: Clinch shouts are not palyed during this cooldown starting on ente...
    float    wh_cs_AutomationAction_ShoutDistanceLimitMax; // 0x690  "wh_cs_AutomationAction_ShoutDistanceLimitMax" default=15.0f flags=2  Debug AI automation: Distance limiting combat idle shouting.
    float    wh_cs_AutomationAction_WeaponsUpdateInterval; // 0x694  "wh_cs_AutomationAction_WeaponsUpdateInterval" default=0.5f flags=2  Debug AI automation: Weapon automation - update interval.
    float    wh_cs_AutomationAction_WeaponOversizedAttackProblemDuration; // 0x698  "wh_cs_AutomationAction_WeaponOversizedAttackProblemDuration" default=12.0f flags=2  Debug AI automation: Weapon automation - drop oversized weapon after given duration fro...
    float    wh_cs_AutomationAction_WeaponAttackDistTimeTolerance; // 0x69C  "wh_cs_AutomationAction_WeaponAttackDistTimeTolerance" default=3.0f flags=2  Debug AI automation: Weapon automation - non precise attack distance hysteresis over time.
    float    wh_cs_AutomationAction_WeaponMissileInitRandCoef; // 0x6A0  "wh_cs_AutomationAction_WeaponMissileInitRandCoef" default=0.3f flags=2  Debug AI automation: Weapon automation - rand coef for missile weapon init decision.
    float    wh_cs_AutomationAction_WeaponMissileLoadingMinDistance; // 0x6A4  "wh_cs_AutomationAction_WeaponMissileLoadingMinDistance" default=10.0f flags=2  Minimal range from attacker to allow loading of ranged werapon (without autoload).
    int32_t  wh_cs_AutomationAction_WeaponUnreachedTest; // 0x6A8  "wh_cs_AutomationAction_WeaponUnreachedTest" default=0 flags=2  Debug AI automation: Weapon automation - testing of movement enevent unreached.
    int32_t  wh_cs_AutomationAction_DebugDraw;          // 0x6AC  "wh_cs_AutomationAction_DebugDraw" default=0 flags=2  Debug AI automation: Enables debug draw.
    int64_t  pCVar_wh_cs_AutomationAction_DebugDrawTarget; // 0x6B0  ICVar* "wh_cs_AutomationAction_DebugDrawTarget"  Enables debug automation on given entity.
    float    wh_cs_AutomationAction_MoveProblem_UnreachableFilterDuration; // 0x6B8  "wh_cs_AutomationAction_MoveProblem_UnreachableFilterDuration" default=2.0f flags=2  AI automation: Defines init duration for filtering of unreachable event.
    float    wh_cs_AutomationAction_MoveProblem_EventFilteringTreshold; // 0x6BC  "wh_cs_AutomationAction_MoveProblem_EventFilteringTreshold" default=1.0f flags=2  AI automation: Defines init filtering time for filtering of unreachable event.
    float    wh_cs_AutomationAction_MoveProblem_AfterHuntAttackDuration; // 0x6C0  "wh_cs_AutomationAction_MoveProblem_AfterHuntAttackDuration" default=0.0f flags=2  AI automation: Defines duration after hunt attack to give a change to solve move problem.
    float    wh_cs_AutomationAction_MoveProblem_WeaponChangeDuration; // 0x6C4  "wh_cs_AutomationAction_MoveProblem_WeaponChangeDuration" default=6.0f flags=2  AI automation: Defines duration for weapon change attempt at solve move event.
    float    wh_cs_AutomationAction_MoveProblem_SlowPursuitDuration; // 0x6C8  "wh_cs_AutomationAction_MoveProblem_SlowPursuitDuration" default=40.0f flags=2  AI automation: Defines duration for slow pursuit state.
    float    wh_cs_AutomationAction_MoveProblem_UncertainDuration; // 0x6CC  "wh_cs_AutomationAction_MoveProblem_UncertainDuration" default=10.0f flags=2  AI automation: Defines duration for uncertain state of movement problem.
    float    wh_cs_AutomationAction_MoveProblem_DistanceThreat; // 0x6D0  "wh_cs_AutomationAction_MoveProblem_DistanceThreat" default=0.0f flags=2  AI automation: Defines threat distance for move problem decision.
    float    wh_cs_AutomationAction_MoveProblem_DistanceThreatForcedUncertain; // 0x6D4  "wh_cs_AutomationAction_MoveProblem_DistanceThreatForcedUncertain" default=7.0f flags=2  AI automation: Defines threat distance for move problem decision (in forced uncertain s...
    float    wh_cs_AutomationAction_MoveProblem_DistanceFlee; // 0x6D8  "wh_cs_AutomationAction_MoveProblem_DistanceFlee" default=60.0f flags=2  AI automation: Defines flee distance for move problem decision.
    float    wh_cs_AutomationAction_MoveProblem_UnreachedFilterDuration; // 0x6DC  "wh_cs_AutomationAction_MoveProblem_UnreachedFilterDuration" default=10.0f flags=2  AI automation: Duration of filtering for unreached event after 'Uncertain' state.
    float    wh_cs_AutomationAction_MoveProblem_TotalDuration; // 0x6E0  "wh_cs_AutomationAction_MoveProblem_TotalDuration" default=60.0f flags=2  AI automation: Total duration to resolve move problem. Then report to script.
    float    wh_cs_AutomationAction_MoveProblem_NoPathMissileAttemptCooldown; // 0x6E4  "wh_cs_AutomationAction_MoveProblem_NoPathMissileAttemptCooldown" default=4.0f flags=2  When pathfind files the npc try to change to missile after this cooldown.
    float    wh_cs_AutomationAction_MoveProblem_DebugForceUncertain; // 0x6E8  "wh_cs_AutomationAction_MoveProblem_DebugForceUncertain" default=4.0f flags=2  AI automation: Total duration to resolve move problem. Then report to script.
    float    wh_cs_AutomationAction_MoveProblem_EstimatingMoveCooldown; // 0x6EC  "wh_cs_AutomationAction_MoveProblem_EstimatingMoveCooldown" default=4.0f flags=2  AI automation: Cooldown for estimating move before automation sent problem to the skirm...
    float    wh_cs_AutomationAction_MoveProblem_MissileWeaponMinDistance; // 0x6F0  "wh_cs_AutomationAction_MoveProblem_MissileWeaponMinDistance" default=4.0f flags=2  AI automation: Minimal distance for missile weapon change when hunt attack timeouts.
    float    wh_cs_AutomationAction_MaxDistanceForTarget; // 0x6F4  "wh_cs_AutomationAction_MaxDistanceForTarget" default=60.0f flags=2  AI automation: Maximum distance to target - preventing targeting of too far targets.
    float    wh_cs_AutomationAction_MissileHuntAttackCooldown; // 0x6F8  "wh_cs_AutomationAction_MissileHuntAttackCooldown" default=5.0f flags=2  AI automation: Cooldown for hunt attacks against ranged opponent.
    float    wh_cs_AutomationAction_MoveProblem_BypassUncertainOnIsAimedDuration; // 0x6FC  "wh_cs_AutomationAction_MoveProblem_BypassUncertainOnIsAimedDuration" default=10.0f flags=2  AI automation: Cooldown duration for uncertain state triggered by is aimed state.
    float    wh_cs_AutomationAction_HuntAttackDist;     // 0x700  "wh_cs_AutomationAction_HuntAttackDist" default=10.0f flags=2  AI automation: Distance for hunt attack decision based on unreached move event.
    float    wh_cs_AutomationAction_HuntAttackDistRand; // 0x704  "wh_cs_AutomationAction_HuntAttackDistRand" default=2.0f flags=2  AI automation: Random offset for hunt attack distance.
    float    wh_cs_AutomationAction_HuntAttackTimeLimit; // 0x708  "wh_cs_AutomationAction_HuntAttackTimeLimit" default=5.0f flags=2  AI automation: Max time limit for hunt attack attempt.
    float    wh_cs_AutomationAction_MissileUpdateInterval; // 0x70C  "wh_cs_AutomationAction_MissileUpdateInterval" default=0.5f flags=2  AI automation: Update interval for missile automation.
    float    wh_cs_AutomationAction_MissileCylRaycastRadius; // 0x710  "wh_cs_AutomationAction_MissileCylRaycastRadius" default=0.5f flags=2  AI automation: Radius of cylindrical raycast for missile obstacle detection.
    float    wh_cs_AutomationAction_MissileCylRaycastOffset; // 0x714  "wh_cs_AutomationAction_MissileCylRaycastOffset" default=0.3f flags=2  AI automation: Offset of shape cast from the attacker weapon.
    float    wh_cs_AutomationAction_MissileAimingFailureCooldown; // 0x718  "wh_cs_AutomationAction_MissileAimingFailureCooldown" default=6.0f flags=2  AI automation: Shooting will fail if firing is not executed until this limit.
    int32_t  wh_cs_AutomationAction_MissileDebugDraw;   // 0x71C  "wh_cs_AutomationAction_MissileDebugDraw" default=0 flags=2  AI automation: debug draw for missile automation.
    float    wh_cs_AutomationAction_DefenseMoveReactionVelocityLim; // 0x720  "wh_cs_AutomationAction_DefenseMoveReactionVelocityLim" default=0.5f flags=2  AI defense automation: forward velocity limit for move reaction
    float    wh_cs_AutomationAction_ForceUnarmedHealthThreshold; // 0x724  "wh_cs_AutomationAction_ForceUnarmedHealthThreshold" default=0.5f flags=2  Health threshold when forced unarmed combat vs. player expired.
    int32_t  wh_cs_AutomationAction_WeaponChangeViaSignal; // 0x728  "wh_cs_AutomationAction_WeaponChangeViaSignal" default=0 flags=2  Changing weapon from automation is done via NPC state combat helper or via signal. 0 - ...
    float    wh_cs_AutomationAction_TemporaryGuardStarterPeriod; // 0x72C  "wh_cs_AutomationAction_TemporaryGuardStarterPeriod" default=0.5f flags=2  Time for temporary start of guard after StartCombat procclip.
    float    wh_cs_AutomationAction_MovementRestrictionOverlapTime; // 0x730  "wh_cs_AutomationAction_MovementRestrictionOverlapTime" default=0.8f flags=2  Period for movement restriction overlap after action or change of zone.
    float    wh_cs_AutomationAction_BattlementUpdateInterval; // 0x734  "wh_cs_AutomationAction_BattlementUpdateInterval" default=1.5f flags=2  Default update interval for battlement automation.
    float    wh_cs_AutomationAction_BattlementQueryRadius; // 0x738  "wh_cs_AutomationAction_BattlementQueryRadius" default=15.0f flags=2  Radius for battlement query.
    float    wh_cs_AutomationAction_BattlementShootingMinRange; // 0x73C  "wh_cs_AutomationAction_BattlementShootingMinRange" default=5.0f flags=2  Minimal distance for battlement range.
    float    wh_cs_AutomationAction_BattlementShootingMaxRange; // 0x740  "wh_cs_AutomationAction_BattlementShootingMaxRange" default=50.0f flags=2  Maximal distance for battlement range.
    float    wh_cs_AutomationAction_BattlementShootingElevationTolerance; // 0x744  "wh_cs_AutomationAction_BattlementShootingElevationTolerance" default=10.0f flags=2  Tolerance in z-axis for position of battlement and combatant that wants to use it.
    float    wh_cs_AutomationAction_BattlementShootingAngleVertical; // 0x748  "wh_cs_AutomationAction_BattlementShootingAngleVertical" default=150.0f flags=2  Vertical angle range for battlement shooting in degrees.
    float    wh_cs_AutomationAction_BattlementShootingAngleHorizontal; // 0x74C  "wh_cs_AutomationAction_BattlementShootingAngleHorizontal" default=80.0f flags=2  Horizontal angle range for battlement shooting in degrees.
    int64_t  pCVar_wh_cs_Automation1_Target;            // 0x750  ICVar* "wh_cs_Automation1_Target"  Enables debug AI on given entity.
    int32_t  wh_cs_Automation1_Mode;                    // 0x758  "wh_cs_Automation1_Mode" default=4 flags=2  Allows to specify whitch automation will be started after selecting target: 0..none, 1....
    float    wh_cs_Automation1_NoAction;                // 0x75C  "wh_cs_Automation1_NoAction" default=-1.0f flags=2  Debug AI probability for no defense action
    float    wh_cs_Automation1_NB;                      // 0x760  "wh_cs_Automation1_NB" default=-1.0f flags=2  Debug AI probability for normal block
    float    wh_cs_Automation1_PB;                      // 0x764  "wh_cs_Automation1_PB" default=-1.0f flags=2  Debug AI probability for perfect block
    float    wh_cs_Automation1_SPB;                     // 0x768  "wh_cs_Automation1_SPB" default=-1.0f flags=2  Debug AI probability for special perfect block
    float    wh_cs_Automation1_Dodge;                   // 0x76C  "wh_cs_Automation1_Dodge" default=-1.0f flags=2  Debug AI probability for dodge
    int32_t  wh_cs_Automation1_StaticPreblock;          // 0x770  "wh_cs_Automation1_StaticPreblock" default=-1 flags=2  Debug AI: enables static preblock
    float    wh_cs_Automation1_TrickReactionDelayMin;   // 0x774  "wh_cs_Automation1_TrickReactionDelayMin" default=-1.0f flags=2  Debug AI automation: Min time between opponent's trick and reblock.
    float    wh_cs_Automation1_TrickReactionDelayMax;   // 0x778  "wh_cs_Automation1_TrickReactionDelayMax" default=-1.0f flags=2  Debug AI automation: Max time between opponent's trick and reblock.
    float    wh_cs_Automation1_PeriodicalAttackTime;    // 0x77C  "wh_cs_Automation1_PeriodicalAttackTime" default=-1.0f flags=2  Debug AI automation: time for periodical attacking.
    float    wh_cs_Automation1_RiposteProb;             // 0x780  "wh_cs_Automation1_RiposteProb" default=-1.0f flags=2  Debug AI automation: riposte probability <0,1>.
    int32_t  wh_cs_Automation1_ComboSteps;              // 0x784  "wh_cs_Automation1_ComboSteps" default=-1 flags=2  Debug AI automation: max number of steps for natural combo.
    float    wh_cs_Automation1_ComboProb;               // 0x788  "wh_cs_Automation1_ComboProb" default=-1.0f flags=2  Debug AI automation: prob of normal combo.
    float    wh_cs_Automation1_ComboHitCancelProb;      // 0x78C  "wh_cs_Automation1_ComboHitCancelProb" default=-1.0f flags=2  Debug AI automation: prob that normal combo is cancelled when hit opponent.
    float    wh_cs_Automation1_TrickEndAttackProb;      // 0x790  "wh_cs_Automation1_TrickEndAttackProb" default=-1.0f flags=2  Debug AI automation: <0,1> Probability for terminating trick step without attack.
    float    wh_cs_Automation1_TrickAttackProb;         // 0x794  "wh_cs_Automation1_TrickAttackProb" default=-1.0f flags=2  Debug AI automation: <0,1> Probability for attack from trick.
    float    wh_cs_Automation1_TrickChangeZoneProb;     // 0x798  "wh_cs_Automation1_TrickChangeZoneProb" default=-1.0f flags=2  Debug AI automation: <0,1> Probability for changing zone during trick.
    float    wh_cs_Automation1_TrickActionDelayMin;     // 0x79C  "wh_cs_Automation1_TrickActionDelayMin" default=-1.0f flags=2  Debug AI automation: Min delay for selected trick action.
    float    wh_cs_Automation1_TrickActionDelayMax;     // 0x7A0  "wh_cs_Automation1_TrickActionDelayMax" default=-1.0f flags=2  Debug AI automation: Max delay for selected trick action.
    float    wh_cs_Automation1_TrickNewQueryTimeoutMin; // 0x7A4  "wh_cs_Automation1_TrickNewQueryTimeoutMin" default=-1.0f flags=2  Debug AI automation: Min delay for next trick step decision.
    float    wh_cs_Automation1_TrickNewQueryTimeoutMax; // 0x7A8  "wh_cs_Automation1_TrickNewQueryTimeoutMax" default=-1.0f flags=2  Debug AI automation: Max delay for next trick step decision.
    float    wh_cs_Automation1_ClinchActionDelayMin;    // 0x7AC  "wh_cs_Automation1_ClinchActionDelayMin" default=-1.0f flags=2  Debug AI automation: Min time for delayed clinch action
    float    wh_cs_Automation1_ClinchActionDelayMax;    // 0x7B0  "wh_cs_Automation1_ClinchActionDelayMax" default=-1.0f flags=2  Debug AI automation: Min time for delayed clinch action
    float    wh_cs_Automation1_ClinchAlternativeProb;   // 0x7B4  "wh_cs_Automation1_ClinchAlternativeProb" default=-1.0f flags=2  Debug AI automation: Probability of use of alternative guard in clinch.
    float    wh_cs_Automation1_GuardBorderLevel0;       // 0x7B8  "wh_cs_Automation1_GuardBorderLevel0" default=-1.0f flags=2  Debug AI automation: Guard border distance for begginer.
    float    wh_cs_Automation1_GuardBorderLevel1;       // 0x7BC  "wh_cs_Automation1_GuardBorderLevel1" default=-1.0f flags=2  Debug AI automation: Guard border distance for master.
    float    wh_cs_Automation1_GuardAbilityLevel;       // 0x7C0  "wh_cs_Automation1_GuardAbilityLevel" default=-1.0f flags=2  Debug AI automation: Guard ability level.
    float    wh_cs_Automation1_GuardBorderHystDiameter; // 0x7C4  "wh_cs_Automation1_GuardBorderHystDiameter" default=-1.0f flags=2  Debug AI automation: Guard border hysteresis diameter.
    float    wh_cs_Automation1_ZoneAdaptationDelayMin;  // 0x7C8  "wh_cs_Automation1_ZoneAdaptationDelayMin" default=-1.0f flags=2  Debug AI automation: When 'opponentZone' mode is selected then zone is adapted after th...
    float    wh_cs_Automation1_ZoneAdaptationDelayMax;  // 0x7CC  "wh_cs_Automation1_ZoneAdaptationDelayMax" default=-1.0f flags=2  Debug AI automation: When 'opponentZone' mode is selected then zone is adapted after th...
    float    wh_cs_Automation1_ZoneChangeTimeoutMin;    // 0x7D0  "wh_cs_Automation1_ZoneChangeTimeoutMin" default=-1.0f flags=2  Debug AI automation: Min time between changes of zones.
    float    wh_cs_Automation1_ZoneChangeTimeoutMax;    // 0x7D4  "wh_cs_Automation1_ZoneChangeTimeoutMax" default=-1.0f flags=2  Debug AI automation: Max time between changes of zones.
    float    wh_cs_Automation1_ZoneChangeOffenseModeWeight; // 0x7D8  "wh_cs_Automation1_ZoneChangeOffenseModeWeight" default=-1.0f flags=2  Debug AI automation: <0,1> Weight for blending between random selection and best offens...
    float    wh_cs_Automation1_ZoneChangeDefenseModeWeight; // 0x7DC  "wh_cs_Automation1_ZoneChangeDefenseModeWeight" default=-1.0f flags=2  Debug AI automation: <0,1> Weight for blending between random selection and best defens...
    float    wh_cs_Automation1_WeaponMissileDistRangeMin; // 0x7E0  "wh_cs_Automation1_WeaponMissileDistRangeMin" default=-1.0f flags=2  Debug AI automation: Overrides bow limits
    uint8_t  _pad_7E4[0x4];                    // 0x7E4
    int64_t  pCVar_wh_cs_Automation2_Target;            // 0x7E8  ICVar* "wh_cs_Automation2_Target"  Enables debug AI on given entity.
    int32_t  wh_cs_Automation2_Mode;                    // 0x7F0  "wh_cs_Automation2_Mode" default=4 flags=2  Allows to specify whitch automation will be started after selecting target: 0..none, 1....
    float    wh_cs_Automation2_NoAction;                // 0x7F4  "wh_cs_Automation2_NoAction" default=-1.0f flags=2  Debug AI probability for no defense action
    float    wh_cs_Automation2_NB;                      // 0x7F8  "wh_cs_Automation2_NB" default=-1.0f flags=2  Debug AI probability for normal block
    float    wh_cs_Automation2_PB;                      // 0x7FC  "wh_cs_Automation2_PB" default=-1.0f flags=2  Debug AI probability for perfect block
    float    wh_cs_Automation2_SPB;                     // 0x800  "wh_cs_Automation2_SPB" default=-1.0f flags=2  Debug AI probability for special perfect block
    float    wh_cs_Automation2_Dodge;                   // 0x804  "wh_cs_Automation2_Dodge" default=-1.0f flags=2  Debug AI probability for dodge
    int32_t  wh_cs_Automation2_StaticPreblock;          // 0x808  "wh_cs_Automation2_StaticPreblock" default=-1 flags=2  Debug AI: enables static preblock
    float    wh_cs_Automation2_TrickReactionDelayMin;   // 0x80C  "wh_cs_Automation2_TrickReactionDelayMin" default=-1.0f flags=2  Debug AI automation: Min time between opponent's trick and reblock.
    float    wh_cs_Automation2_TrickReactionDelayMax;   // 0x810  "wh_cs_Automation2_TrickReactionDelayMax" default=-1.0f flags=2  Debug AI automation: Max time between opponent's trick and reblock.
    float    wh_cs_Automation2_PeriodicalAttackTime;    // 0x814  "wh_cs_Automation2_PeriodicalAttackTime" default=-1.0f flags=2  Debug AI automation: time for periodical attacking.
    float    wh_cs_Automation2_RiposteProb;             // 0x818  "wh_cs_Automation2_RiposteProb" default=-1.0f flags=2  Debug AI automation: riposte probability <0,1>.
    int32_t  wh_cs_Automation2_ComboSteps;              // 0x81C  "wh_cs_Automation2_ComboSteps" default=-1 flags=2  Debug AI automation: max number of steps for natural combo.
    float    wh_cs_Automation2_ComboProb;               // 0x820  "wh_cs_Automation2_ComboProb" default=-1.0f flags=2  Debug AI automation: prob of normal combo.
    float    wh_cs_Automation2_ComboHitCancelProb;      // 0x824  "wh_cs_Automation2_ComboHitCancelProb" default=-1.0f flags=2  Debug AI automation: prob that normal combo is cancelled when hit opponent.
    float    wh_cs_Automation2_TrickEndAttackProb;      // 0x828  "wh_cs_Automation2_TrickEndAttackProb" default=-1.0f flags=2  Debug AI automation: <0,1> Probability for terminating trick step without attack.
    float    wh_cs_Automation2_TrickAttackProb;         // 0x82C  "wh_cs_Automation2_TrickAttackProb" default=-1.0f flags=2  Debug AI automation: <0,1> Probability for attack from trick.
    float    wh_cs_Automation2_TrickChangeZoneProb;     // 0x830  "wh_cs_Automation2_TrickChangeZoneProb" default=-1.0f flags=2  Debug AI automation: <0,1> Probability for changing zone during trick.
    float    wh_cs_Automation2_TrickActionDelayMin;     // 0x834  "wh_cs_Automation2_TrickActionDelayMin" default=-1.0f flags=2  Debug AI automation: Min delay for selected trick action.
    float    wh_cs_Automation2_TrickActionDelayMax;     // 0x838  "wh_cs_Automation2_TrickActionDelayMax" default=-1.0f flags=2  Debug AI automation: Max delay for selected trick action.
    float    wh_cs_Automation2_TrickNewQueryTimeoutMin; // 0x83C  "wh_cs_Automation2_TrickNewQueryTimeoutMin" default=-1.0f flags=2  Debug AI automation: Min delay for next trick step decision.
    float    wh_cs_Automation2_TrickNewQueryTimeoutMax; // 0x840  "wh_cs_Automation2_TrickNewQueryTimeoutMax" default=-1.0f flags=2  Debug AI automation: Max delay for next trick step decision.
    float    wh_cs_Automation2_ClinchActionDelayMin;    // 0x844  "wh_cs_Automation2_ClinchActionDelayMin" default=-1.0f flags=2  Debug AI automation: Min time for delayed clinch action
    float    wh_cs_Automation2_ClinchActionDelayMax;    // 0x848  "wh_cs_Automation2_ClinchActionDelayMax" default=-1.0f flags=2  Debug AI automation: Min time for delayed clinch action
    float    wh_cs_Automation2_ClinchAlternativeProb;   // 0x84C  "wh_cs_Automation2_ClinchAlternativeProb" default=-1.0f flags=2  Debug AI automation: Probability of use of alternative guard in clinch.
    float    wh_cs_Automation2_GuardBorderLevel0;       // 0x850  "wh_cs_Automation2_GuardBorderLevel0" default=-1.0f flags=2  Debug AI automation: Guard border distance for begginer.
    float    wh_cs_Automation2_GuardBorderLevel1;       // 0x854  "wh_cs_Automation2_GuardBorderLevel1" default=-1.0f flags=2  Debug AI automation: Guard border distance for master.
    float    wh_cs_Automation2_GuardAbilityLevel;       // 0x858  "wh_cs_Automation2_GuardAbilityLevel" default=-1.0f flags=2  Debug AI automation: Guard ability level.
    float    wh_cs_Automation2_GuardBorderHystDiameter; // 0x85C  "wh_cs_Automation2_GuardBorderHystDiameter" default=-1.0f flags=2  Debug AI automation: Guard border hysteresis diameter.
    float    wh_cs_Automation2_ZoneAdaptationDelayMin;  // 0x860  "wh_cs_Automation2_ZoneAdaptationDelayMin" default=-1.0f flags=2  Debug AI automation: When 'opponentZone' mode is selected then zone is adapted after th...
    float    wh_cs_Automation2_ZoneAdaptationDelayMax;  // 0x864  "wh_cs_Automation2_ZoneAdaptationDelayMax" default=-1.0f flags=2  Debug AI automation: When 'opponentZone' mode is selected then zone is adapted after th...
    float    wh_cs_Automation2_ZoneChangeTimeoutMin;    // 0x868  "wh_cs_Automation2_ZoneChangeTimeoutMin" default=-1.0f flags=2  Debug AI automation: Min time between changes of zones.
    float    wh_cs_Automation2_ZoneChangeTimeoutMax;    // 0x86C  "wh_cs_Automation2_ZoneChangeTimeoutMax" default=-1.0f flags=2  Debug AI automation: Max time between changes of zones.
    float    wh_cs_Automation2_ZoneChangeOffenseModeWeight; // 0x870  "wh_cs_Automation2_ZoneChangeOffenseModeWeight" default=-1.0f flags=2  Debug AI automation: <0,1> Weight for blending between random selection and best offens...
    float    wh_cs_Automation2_ZoneChangeDefenseModeWeight; // 0x874  "wh_cs_Automation2_ZoneChangeDefenseModeWeight" default=-1.0f flags=2  Debug AI automation: <0,1> Weight for blending between random selection and best defens...
    float    wh_cs_Automation2_WeaponMissileDistRangeMin; // 0x878  "wh_cs_Automation2_WeaponMissileDistRangeMin" default=-1.0f flags=2  Debug AI automation: Overrides bow limits
    uint8_t  _pad_87C[0x4];   // 0x87C  (trailing align pad to sizeof)

    static S_CombatSettings* GetInstance();   // S_CombatSettings.cpp -> g_combatSettings @ RVA 0x55D4D70
};
#pragma pack(pop)

static_assert(sizeof(S_CombatSettings) == 0x880, "S_CombatSettings size mismatch");

}  // namespace wh::combatmodule
