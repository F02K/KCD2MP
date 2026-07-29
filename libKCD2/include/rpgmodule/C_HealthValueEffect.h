#pragma once
#include <cstdint>
#include <cstddef>
#include "C_SoulStateEffect.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_HealthValueEffect -- change soul health (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4EA98  vtable 0x183A8BD00.  sizeof 0x48 (PROVEN: factory sub_180C66194
// pool-alloc(0x48) -> in-place ctor sub_180C66228).  : C_SoulStateEffect (m_valueId preset to
// E_SoulState::Health = 0).  Unlike its siblings it carries damage-attribution extras
// (source WUID init = the invalid constant qword_185330810). One instance rides each
// S_DamageEventData (+0xD0); the damage pipeline fills the extras before the negative
// delta lands in m_amount (sub_180C627CD).

namespace wh::rpgmodule {

class C_HealthValueEffect : public C_SoulStateEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HealthValueEffect;
    int32_t  m_bodySubpartId;           // +0x30  ctor -1; apply core copies its resolved body_subpart_id intermediate (@0x180C64131), not the final blood_zone_id
    uint32_t _pad34;                   // +0x34
    wh::framework::WUID m_sourceWuid;  // +0x38  ctor = invalid WUID; attacker soul WUID (C_Soul+0x30) written by sub_180726FF0 @0x180727037 -- kill attribution
    uint8_t  m_flag40;                 // +0x40  ctor 0 (role UNVERIFIED)
    uint8_t  _pad41[3];                // +0x41
    int32_t  m_damageSourceCode;       // +0x44  producer code via sub_180726FD0 @0x180726FEB: 1 melee, 1|2 missile (2 when weapon class 13), 4 collision, 10 block-chip; 7 = fall (sub_1807243B0, stamped inline -- bypasses S_DamageEventData/Dispatch() entirely, VERIFIED)
};
static_assert(sizeof(C_HealthValueEffect) == 0x48, "C_HealthValueEffect must be 0x48 (ctor write extent)");
static_assert(offsetof(C_HealthValueEffect, m_bodySubpartId) == 0x30, "body subpart id at 0x30");
static_assert(offsetof(C_HealthValueEffect, m_sourceWuid) == 0x38, "source wuid at 0x38");

}  // namespace wh::rpgmodule
