#pragma once
#include <cstdint>
#include "../framework/WUID.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::rpgmodule::S_MissileHitData -- payload of C_MissileHitCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x88 (PROVEN: factory sub_182D226AC tracked-alloc 144 = 8+136; copies 8x _OWORD
// @+0x08..+0x7F then qword @+0x88). Factory returns the raw cause via *a1; caller sub_182D19080
// wraps it in C_Event. Field breakdown below UNVERIFIED this session (applied from an external
// proposal without independent IDA re-derivation) -- only the size/wrap mechanism above is proven.

namespace wh::rpgmodule {

struct S_MissileHitData {
    wh::framework::WUID m_attackerWuid; // +0x00
    uint64_t m_unk08;                   // +0x08
    wh::framework::WUID m_victimWuid;   // +0x10
    uint32_t m_victimEntityId;          // +0x18
    uint32_t m_unk1C;                   // +0x1C

    Vec3 m_hitPosition;                 // +0x20
    uint8_t m_unk2C[0x0C];              // +0x2C
    Vec3 m_impactVelocity;              // +0x38
    Vec3 m_targetNormal;                // +0x44
    int32_t m_origMaterialId;           // +0x50
    int32_t m_unk54;                    // +0x54

    wh::framework::WUID m_attackerWeaponWuid; // +0x58
    wh::framework::WUID m_attackerAmmoWuid;   // +0x60
    uint64_t m_unk68;                         // +0x68
    wh::framework::WUID m_victimWeaponWuid;   // +0x70
    uint64_t m_unk78;                         // +0x78

    int32_t m_targetSurfaceId;           // +0x80
    float m_damageScale;                 // +0x84
};
static_assert(sizeof(S_MissileHitData) == 0x88, "S_MissileHitData must be 0x88 (136-byte ctor copy)");

}  // namespace wh::rpgmodule
