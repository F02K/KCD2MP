#pragma once
#include <cstdint>
#include "C_CompanionManager.h"
#include "S_SoulStatBlock.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulRPGStats -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x660 (SYNTHETIC name).
// -----------------------------------------------
// C_Soul's RPG stats/skills aggregate @C_Soul+0x498 (non-polymorphic). ctor sub_18070F31C(this,
// ownerSoul); dtor sub_180966B10. Fully tiled by the backlog deep map (verify-confirmed):
//   companions @+0x08, LIVE stat block @+0xC8 (= C_Soul+0x560), a 0x78 accumulator @+0x330, the
//   BASE/snapshot stat block @+0x3A8 (= C_Soul+0x840), a 0x48 sub-object @+0x610, tail qword.
// This is where health/stamina/level are DERIVED from (base stats live in m_liveBlock; see
// S_SoulStatBlock.h -- derived stats are computed, not stored).

namespace wh::rpgmodule {

class C_Soul;

// 0x78 stat accumulator between the two blocks (ctor sub_18072A3C8; rcx = aggregate+0x330).
struct S_SoulStatAccumulator {
    double   m_slots[13];   // +0x00  (sub_18072A400 writes a double per slot, stride 8)
    C_Soul*  m_pOwner;      // +0x68  owner backref
    float    m_scale;       // +0x70  init 1.0
    float    m_cap;         // +0x74  init FLT_MAX
};
static_assert(sizeof(S_SoulStatAccumulator) == 0x78, "S_SoulStatAccumulator must be 0x78");

struct C_SoulRPGStats {
    int32_t  m_unk00;                     // +0x000
    uint8_t  m_flag04;                    // +0x004
    uint8_t  _pad05[3];                   // +0x005
    C_CompanionManager m_companions;      // +0x008  (0xC0, ctor sub_181049CAC)
    S_SoulStatBlock m_liveBlock;          // +0x0C8  = C_Soul+0x560  (AUTHORITATIVE stat/skill sheet)
    S_SoulStatAccumulator m_accumulator;  // +0x330  (0x78)
    S_SoulStatBlock m_baseBlock;          // +0x3A8  = C_Soul+0x840  (base/snapshot sheet; role UNRESOLVED)
    uint64_t m_subobj610[9];              // +0x610  (0x48, ctor sub_181190860; interior UNRESOLVED)
    C_Soul*  m_pOwnerSoul;                // +0x658  owner-soul backref (ctor 0x18070F376 stores a2; non-owning -- not freed in dtor sub_180966B10)
};
static_assert(sizeof(C_SoulRPGStats) == 0x660, "C_SoulRPGStats must be 0x660");
static_assert(offsetof(C_SoulRPGStats, m_liveBlock) == 0xC8, "live block at +0xC8 (C_Soul+0x560)");
static_assert(offsetof(C_SoulRPGStats, m_baseBlock) == 0x3A8, "base block at +0x3A8 (C_Soul+0x840)");

}  // namespace wh::rpgmodule
