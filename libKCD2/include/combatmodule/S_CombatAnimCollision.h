#pragma once
#include <cstddef>
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatAnimCollision -- animation collision signal payload
// (KCD2 WHGame.dll 1.5.6). sizeof 0xA0.
// -----------------------------------------------
// The source name is recovered from the matching KCD1 C_Signal instantiation. KCD2's
// C_CombatActorAnimCollisions::EmitCollisionInfo constructs the 0xA0-byte record and its consumer
// reads +0x90/+0x98/+0x9C. +0x98 is copied from C_CombatActorAnimCollisions::m_hitIndex.

namespace wh::combatmodule {

struct S_CombatAnimCollision {
    Vec3    m_hitPosition;       // +0x00  collision contact point
    Vec3    m_unk0C;             // +0x0C  collision geometry vector; semantic unverified
    Vec3    m_unk18;             // +0x18  collision geometry vector; semantic unverified
    uint8_t m_unk24[0x74];       // +0x24
    int32_t m_hitIndex;          // +0x98  source animation hit index
    uint8_t m_unk9C[0x04];       // +0x9C
};
static_assert(sizeof(S_CombatAnimCollision) == 0xA0, "S_CombatAnimCollision must be 0xA0");
static_assert(offsetof(S_CombatAnimCollision, m_hitIndex) == 0x98);

}  // namespace wh::combatmodule
