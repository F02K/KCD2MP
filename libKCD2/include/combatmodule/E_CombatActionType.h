#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatActionType -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum.
// -----------------------------------------------
// combat_action_type id. Registered by C_CombatActionTypeDatabase::Init (sub_1812B8D48,
// RVA 0x12B8D48): for every action type it (1) sets an id-holder global to -1 and (2) inserts
// the action NAME into a name-keyed std::map<CryStringT, std::function<void(C_CombatActionType
// Data const&)>> (qword_185320428); the per-name onLoad callback resolves the numeric id FROM
// the loaded data row at combat_action_type.tbl load time. The id-holder globals live at
// dword_185320480..dword_1853205C4 in the DB singleton (declaration order preserved there).
//
// [VALUES VERIFIED against KCD2 game data] Numeric ids below are the row ids of
// Libs/Tables/combat/combat_action_type.xml extracted from the KCD2 Tables.pak
// (analysis/game_data/combat_tables/, extract_kcd2_combat_tables.py). The table is the
// authoritative id domain -- the DLL hard-codes no ids (proven: KCD1's DB registers "attack"
// first yet its id is 3). Names marked [table-only] have a table row but NO binary id-resolver
// registration in sub_1812B8D48 (they are reached via FK columns / group mapping instead).
//
// [DELTAS vs KCD1]
//   ADDED: attackRiposte(32), attackSpecial(59), clenchedHand(71), masterStrike(55)+
//     masterStrikeHit(56) (KCD1's "ripostePerfectBlock" concept, renumbered -- KCD1 id 32 now
//     belongs to attackRiposte), guardFreeBlock(66), guardPose(76), guardVariation(58),
//     guardVariationLoop(57), stealthTransitionMaster/Slave(72/73), dodge(25),
//     riderGuardForcedRear(77), horsePullDownAttackFail/HitFail(67/68), the Death mirror set
//     (62..65, 69/70, 74/75, 78/79), and 10 new group buckets (1010..1019).
//   RENUMBERED vs KCD1: syncRiposte 55->60, syncRiposteHit 56->61.
//   REMOVED (no KCD2 table row): perfectNormalBlock(13), perfectBlockDodge, syncDodge(22?),
//     syncDodgeHit, stealthAttackFail(29), stealthHitFail(31), ids 53/54 retired.

namespace wh::combatmodule {

enum class E_CombatActionType : int32_t {
    Invalid                         = -1,   // "invalid"

    // ---- individual action types (ids = combat_action_type.xml rows) ----
    HitMovement                     = 0,    // "hitMovement"
    BlockMoveBack                   = 1,    // "blockMoveBack"
    BlockStepBack                   = 2,    // "blockStepBack"
    Attack                          = 3,    // "attack"
    Leap                            = 4,    // "leap"
    Movement                        = 5,    // "movement"
    Block                           = 6,    // "block"
    HitTorso                        = 7,    // "hitTorso"
    HitBlockBroken                  = 8,    // "hitBlockBroken"
    HitBlockWeapon                  = 9,    // "hitBlockWeapon"
    GuardIdleUpperBody              = 10,   // "guardIdleUpperBody"
    GuardPreBlock                   = 11,   // "guardPreBlock"
    GuardAttackStart                = 12,   // "guardAttackStart"
    PerfectBlock                    = 14,   // "perfectBlock"           (13 retired: KCD1 perfectNormalBlock)
    FailedAttackOnPB                = 15,   // "failedAttackOnPB"
    SyncAttack                      = 16,   // "syncAttack"
    SyncHit                         = 17,   // "syncHit"
    ComboAttack                     = 18,   // "comboAttack"
    ComboHit                        = 19,   // "comboHit"
    GuardSyncMovementMaster         = 20,   // "guardSyncMovementMaster"
    GuardSyncMovementSlave          = 21,   // "guardSyncMovementSlave"
    SyncPerfectBlock                = 23,   // "syncPerfectBlock"       (22 retired)
    SyncPerfectBlockHit             = 24,   // "syncPerfectBlockHit"
    Dodge                           = 25,   // "dodge"                  NEW in KCD2
    FreeAttack                      = 26,   // "freeAttack"
    FailedAttackOnBlock             = 27,   // "failedAttackOnBlock"
    StealthAttackSuccess            = 28,   // "stealthAttackSuccess"   (29 retired: KCD1 stealthAttackFail)
    StealthHitSuccess               = 30,   // "stealthHitSuccess"      (31 retired: KCD1 stealthHitFail)
    AttackRiposte                   = 32,   // "attackRiposte"          NEW (KCD1 id 32 was ripostePerfectBlock)
    SyncTransitionMaster            = 33,   // "syncTransitionMaster"
    SyncTransitionSlave             = 34,   // "syncTransitionSlave"
    GuardStartSyncMovementMaster    = 35,   // "guardStartSyncMovementMaster"
    GuardStartSyncMovementSlave     = 36,   // "guardStartSyncMovementSlave"
    FreeRiderAttackStatic           = 37,   // "freeRiderAttackStatic"
    FreeRiderAttack                 = 38,   // "freeRiderAttack"
    RiderGuardIdle                  = 39,   // "riderGuardIdle"
    RiderGuardMovement              = 40,   // "riderGuardMovement"
    RiderGuardJumpStart             = 41,   // "riderGuardJumpStart"
    RiderGuardJump                  = 42,   // "riderGuardJump"
    RiderGuardJumpEnd               = 43,   // "riderGuardJumpEnd"
    RiderGuardMove2Idle             = 44,   // "riderGuardMove2Idle"
    RiderGuardIdle2Move             = 45,   // "riderGuardIdle2Move"
    RiderGuardRear                  = 46,   // "riderGuardRear"
    RiderGuardFastStop              = 47,   // "riderGuardFastStop"
    HorsePullDownAttackSuccess      = 48,   // "horsePullDownAttackSuccess"
    HorsePullDownHitSuccess         = 49,   // "horsePullDownHitSuccess"
    MercyAttack                     = 50,   // "mercyAttack"
    HuntAttackMaster                = 51,   // "huntAttackMaster"
    HuntAttackSlave                 = 52,   // "huntAttackSlave"        [table-only; no binary resolver]
    MasterStrike                    = 55,   // "masterStrike"           NEW name (renumbered from KCD1 ripostePerfectBlock 32)
    MasterStrikeHit                 = 56,   // "masterStrikeHit"        [table-only] NEW
    GuardVariationLoop              = 57,   // "guardVariationLoop"     NEW
    GuardVariation                  = 58,   // "guardVariation"         NEW
    AttackSpecial                   = 59,   // "attackSpecial"          NEW
    SyncRiposte                     = 60,   // "syncRiposte"            RENUMBERED (KCD1 id 55)
    SyncRiposteHit                  = 61,   // "syncRiposteHit"         RENUMBERED (KCD1 id 56)
    MasterStrikeDeath               = 62,   // "masterStrikeDeath"      [table-only] NEW
    MasterStrikeHitDeath            = 63,   // "masterStrikeHitDeath"   [table-only] NEW
    ComboAttackDeath                = 64,   // "comboAttackDeath"       [table-only] NEW
    ComboHitDeath                   = 65,   // "comboHitDeath"          [table-only] NEW
    GuardFreeBlock                  = 66,   // "guardFreeBlock"         NEW
    HorsePullDownAttackFail         = 67,   // "horsePullDownAttackFail" NEW
    HorsePullDownHitFail            = 68,   // "horsePullDownHitFail"   NEW
    SyncAttackDeath                 = 69,   // "syncAttackDeath"        [table-only] NEW
    SyncHitDeath                    = 70,   // "syncHitDeath"           [table-only] NEW
    ClenchedHand                    = 71,   // "clenchedHand"           NEW
    StealthTransitionMaster         = 72,   // "stealthTransitionMaster" NEW
    StealthTransitionSlave          = 73,   // "stealthTransitionSlave" NEW
    SyncRiposteDeath                = 74,   // "syncRiposteDeath"       [table-only] NEW
    SyncRiposteHitDeath             = 75,   // "syncRiposteHitDeath"    [table-only] NEW
    GuardPose                       = 76,   // "guardPose"              NEW
    RiderGuardForcedRear            = 77,   // "riderGuardForcedRear"   NEW
    HuntAttackDeathMaster           = 78,   // "huntAttackDeathMaster"  [table-only] NEW
    HuntAttackDeathSlave            = 79,   // "huntAttackDeathSlave"   [table-only] NEW

    // ---- group action types (broad-phase buckets, combat_action_type_group.xml) ----
    GroupFreeAttack                 = 1000, // "groupFreeAttack"
    GroupRiderAttack                = 1001, // "groupRiderAttack"
    GroupPerfectBlock               = 1002, // "groupPerfectBlock"
    GroupStaticAttack               = 1003, // "groupStaticAttack"
    GroupMeleeAttack                = 1004, // "groupMeleeAttack"
    GroupRiderMovement              = 1005, // "groupRiderMovement"
    GroupAttack                     = 1006, // "groupAttack"
    GroupBlock                      = 1007, // "groupBlock"
    GroupHit                        = 1008, // "groupHit"
    GroupSyncPerfectBlock           = 1009, // "groupSyncPerfectBlock"
    GroupGuardVar                   = 1010, // "groupGuardVar"          NEW
    GroupMasterStrike               = 1011, // "groupMasterStrike"      NEW
    GroupMasterStrikeHit            = 1012, // "groupMasterStrikeHit"   NEW
    GroupComboAttack                = 1013, // "groupComboAttack"       NEW
    GroupComboHit                   = 1014, // "groupComboHit"          NEW
    GroupSyncAttack                 = 1015, // "groupSyncAttack"        NEW
    GroupSyncHit                    = 1016, // "groupSyncHit"           NEW
    GroupMeleeAttackDuringComboSlot = 1017, // "groupMeleeAttackDuringComboSlot" NEW
    GroupHuntAttackMaster           = 1018, // "groupHuntAttackMaster"  NEW
    GroupHuntAttackSlave            = 1019, // "groupHuntAttackSlave"   NEW
};

}  // namespace wh::combatmodule
