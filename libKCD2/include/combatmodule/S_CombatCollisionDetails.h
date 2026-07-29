#pragma once
#include <cstddef>
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatCollisionDetails -- resolved collision geometry passed through the
// combat collision pipeline (KCD2 WHGame.dll 1.5.6). sizeof 0xD8.
// -----------------------------------------------
// C_CombatActorCollisions embeds two consecutive instances at +0x98/+0x170. The first Vec3 is
// copied verbatim by C_CombatRPG::BuildAndSubmitMeleeHit (0x180D568B0) into
// S_CombatHitData::m_hitPosition. InitCollisionInfoTailDefaults_LeavesGeometryUntouched
// (0x180D508C0) starts initializing this record at +0x24, leaving all three leading Vec3 values
// to the collision-query producer. The meanings of the second and third vectors remain unverified.

namespace wh::combatmodule {

struct S_CombatCollisionDetails {
    Vec3    m_hitPosition;       // +0x00  world-space contact point consumed by the melee-hit path
    Vec3    m_unk0C;             // +0x0C  collision geometry vector; semantic unverified
    Vec3    m_unk18;             // +0x18  collision geometry vector; semantic unverified
    uint8_t m_unk24[0xB4];       // +0x24  collision participants/material/filter/result data
};
static_assert(sizeof(S_CombatCollisionDetails) == 0xD8, "S_CombatCollisionDetails must be 0xD8");
static_assert(offsetof(S_CombatCollisionDetails, m_hitPosition) == 0x00);
static_assert(offsetof(S_CombatCollisionDetails, m_unk24) == 0x24);

}  // namespace wh::combatmodule
