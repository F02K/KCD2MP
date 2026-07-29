#pragma once
#include <cstdint>
#include "../combatmodule/E_CombatAttackType.h"
#include "../combatmodule/E_CombatZoneId.h"
#include "E_DamageTypeMask.h"
#include "../framework/WUID.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::rpgmodule::S_CombatHitData -- payload of C_CombatHitCause (KCD2 WHGame.dll 1.5.6, kd7u).
// Largest cause payload. Most of the field breakdown mirrors S_DamageEventData.h's melee-fill
// region and has since been cross-checked there (m_attackZone/m_weakBlock/m_perfectBlock/
// m_forceBrokenBlock/m_itemBreakRngSeed all independently CONFIRMED via decompile -- see that
// header's evidence comments). m_unk5D/m_unk61 are DELIBERATELY left unnamed: KCD2's serializer
// skips both, the damage pipeline has no reader for either byte, and the KCD1 cross-build path
// copies the same bytes without exposing semantics -- naming them would be fabrication.
// -----------------------------------------------
// sizeof 0x90 (PROVEN: ctor sub_180726F08 tracked-alloc 152 = 8+144; bulk copy of
// 9x _OWORD @0x180726F73..FB7). Serialized field names below per sub_180AD9D30 (unconfirmed).

namespace wh::rpgmodule {

struct S_CombatHitData {
    wh::framework::WUID m_attackerWuid; // +0x00
    uint64_t m_unk08;                   // +0x08
    wh::framework::WUID m_victimWuid;   // +0x10
    uint32_t m_victimEntityId;          // +0x18
    uint32_t m_unk1C;                   // +0x1C

    wh::combatmodule::E_CombatAttackType m_attackType; // +0x20
    E_DamageTypeMask m_damageTypes;      // +0x24
    wh::combatmodule::E_CombatZoneId m_attackZone; // +0x28
    float m_attackStrength;              // +0x2C
    float m_attackEnergy;                // +0x30
    Vec3 m_impactVelocity;               // +0x34
    Vec3 m_hitPosition;                  // +0x40
    int32_t m_origMaterialId;            // +0x4C
    int32_t m_targetSurfaceId;           // +0x50

    bool m_blocking;                     // +0x54
    bool m_weakBlock;                    // +0x55
    bool m_perfectBlock;                 // +0x56
    bool m_forceBrokenBlock;             // +0x57
    bool m_firstHit;                     // +0x58
    bool m_combo;                        // +0x59
    bool m_riposte;                      // +0x5A
    bool m_intentionalHit;               // +0x5B
    bool m_freeAttack;                   // +0x5C
    bool m_unk5D;                        // +0x5D
    bool m_damageVictimWeapon;           // +0x5E
    bool m_scaleByImpactVelocity;        // +0x5F
    bool m_applyAttackerPostHitEffects;  // +0x60
    bool m_unk61;                        // +0x61
    bool m_forceFatal;                   // +0x62  input request copied to S_DamageEventData::m_forceFatal; not a result flag
    bool m_forceNonlethalClamp;          // +0x63
    bool m_fail;                         // +0x64
    bool m_stealthAction;                // +0x65
    uint8_t _pad66[2];                   // +0x66

    int32_t m_forcedBodySubpartId;       // +0x68
    float m_forcedAttackValueCoef;       // +0x6C
    float m_victimRotation;              // +0x70
    uint32_t m_itemBreakRngSeed;         // +0x74

    wh::framework::WUID m_attackerWeaponWuid;          // +0x78
    wh::framework::WUID m_attackerSecondaryWeaponWuid; // +0x80
    wh::framework::WUID m_victimWeaponWuid;            // +0x88
};
static_assert(sizeof(S_CombatHitData) == 0x90, "S_CombatHitData must be 0x90 (144-byte ctor copy)");

}  // namespace wh::rpgmodule
