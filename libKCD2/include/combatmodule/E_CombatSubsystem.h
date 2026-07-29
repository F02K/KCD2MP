#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatSubsystem -- id/index of a combat-actor subsystem object
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Each C_CombatActorObject subsystem returns its E_CombatSubsystem constant from GetSubsystemId
// (vtable slot [2]) and its display name from GetName (slot [3]). The enum indexes the subsystem
// array C_CombatActor builds in sub_180916ECC.
//
// [FUNDAMENTAL vs KCD1] KCD1 recovered the full 0x00..0x28 enum from ONE enum-to-string switch
// (sub_180F54EF4). KCD2 has NO such converter -- every subsystem carries its own 8-byte GetName
// and 6-byte GetSubsystemId thunk -- and the values were RENUMBERED and made sparse (max seen 47).
//
// The enumerators below are the subset RECOVERED-AND-VERIFIED by pairing the parallel GetName /
// GetSubsystemId runs at 0x181A7DE20 / 0x181A7DF10 (alignment anchored by HuntAttack: GetName
// 0x181A7DE50 "HUNT_ATTACK" <-> GetSubsystemId 0x181A7DF40 -> 29, cross-checked against
// C_CombatActorObject.h and the consecutive HorsePullDown/HuntAttack/MercyKill = 28/29/30 triple),
// plus ActionManager=3 / Automation=25 already established in C_CombatActorObject.h. The remaining
// enumerators (other compilation-unit groups: Basic/Model/Director-order low values, the Automation*
// action ids, etc.) were NOT exhaustively enumerated. [PARTIAL -- verified values only]

namespace wh::combatmodule {

enum E_CombatSubsystem : int32_t {
    COMBAT_SUB_ACTION_MANAGER       = 3,    // C_CombatActorObject.h
    COMBAT_SUB_WEAPONS              = 17,   // GetName "Weapons"              (sub_181A7DEE0)
    COMBAT_SUB_RESTRICTION          = 19,   // GetName "Restriction"          (sub_181A7DE90)
    COMBAT_SUB_OPPONENT_MANAGER     = 21,   // GetName "OpponentManager"      (sub_181A7DE80)
    COMBAT_SUB_AUTOMATION           = 25,   // C_CombatActorObject.h
    COMBAT_SUB_STEALTH              = 27,   // GetName "STEALTH" (sub_181A7DEA0) -- VTABLE-ANCHORED: C_CombatActorStealth primary vtbl 0x183B0A590 slot2 sub_181A7DFA0 = return 27; completes the member-order run 27/28/29/30 = +0x3F8/+0x400/+0x408/+0x410
    COMBAT_SUB_HORSE_PULLDOWN       = 28,   // GetName "HORSE_PULLDOWN"       (sub_181A7DE40)
    COMBAT_SUB_HUNT_ATTACK          = 29,   // GetName "HUNT_ATTACK"          (sub_181A7DE50)  [anchor]
    COMBAT_SUB_MERCY_KILL           = 30,   // GetName "MERCY_KILL"           (sub_181A7DE60)
    COMBAT_SUB_TIME_WARP            = 31,   // GetName "CombatActorTimewarp"  (sub_181A7DEC0)
    COMBAT_SUB_NEXT_GUARD_SELECTOR  = 34,   // GetName "NEXT_GUARD_SELECTOR"  (sub_181A7DE70)
    COMBAT_SUB_EVENTS               = 41,   // GetName "EVENTS"               (sub_181A7DE30)
    // COMBAT_SUB_TIME_COP: previous revision had TIME_COP=27 / STEALTH=42 from unanchored
    // GetName/GetSubsystemId alignment pairing; STEALTH=27 is now vtable-anchored (above), so
    // TimeCop's id must be re-verified from its own vtable slot 2 before re-adding [U]
    COMBAT_SUB_VIRTUAL_WEAPON       = 44,   // GetName "VIRTUAL_WEAPON"       (sub_181A7DED0)
    COMBAT_SUB_DIRECTOR             = 45,   // GetName "Director"             (sub_181A7DE20)
    COMBAT_SUB_ALIGNMENT            = 47,   // GetName "Alignment"            (sub_181A7DEF0)
    // NOTE: gaps (0-2, 4-16, 18, 20, 22-24, 26, 32-33, 35-40, 43, 46, >47) are further subsystems
    // whose GetSubsystemId thunks were not individually decompiled. [UNVERIFIED]
};

}  // namespace wh::combatmodule
