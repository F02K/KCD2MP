#pragma once
#include <cstdint>
#include <cstddef>
#include "C_SoulValueEffect.h"
#include "E_SoulStat.h"

// -----------------------------------------------
// wh::rpgmodule::C_StatXPEffect -- add stat XP / advance stat level (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_StatXPEffect@rpgmodule@wh@@ (TD 0x184D60228)  vtable 0x183A8BD88 (9 slots).
// sizeof 0x40 (PROVEN: factory sub_180C66EF0 operator new(64)).
// C_SoulValueEffect<E_SoulStat, fixed_t<uint32_t,25>> -> C_SoulEffect -> C_Effect.
// Ctor sub_180C67268(this, owner, targetWuid, statId, &amount, bAdvanceToLevel, contextParam).
// Apply sub_180C653E4: resolves the soul (LookupByWUID), reads old XP from soul+0x56C+8*statId
// (+1388 of the per-stat block), rate = DerivedStat(47) (XP multiplier) unless advancing,
// advance target from RPG params 172..176 (statId 0..3, 6), writes back via sub_180C65888 and
// records main-level movement (DerivedStat(28) = main level).
// GetName [8] -> TLS CryString "StatXP" (engine's own name).
// Accept [1]/[2] dispatch to visitor slots [31]/[32].

namespace wh::rpgmodule {

class C_StatXPEffect : public C_SoulValueEffect<E_SoulStat> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StatXPEffect;
    // (m_valueId @+0x18 = stat id, m_amount @+0x1C from the base template)
    float    m_targetXP;        // +0x20  computed target XP (Apply)
    int32_t  m_oldStatXP;       // +0x24  soul's stat XP before Apply
    int32_t  m_statLevelDelta;  // +0x28  level-ups produced by the XP write [role INFERRED]
    int32_t  m_oldMainLevel;    // +0x2C  (int)DerivedStat(28) before
    int32_t  m_mainLevelDelta;  // +0x30  max(0, newMainLevel - old)
    uint8_t  m_bAdvanceToLevel; // +0x34  0 = AddStatXP, 1 = AdvanceToStatLevel (skips the rate mult)
    uint8_t  _pad35[3];         // +0x35
    int32_t  m_contextParam;    // +0x38  packed into the derived-stat query context [role INFERRED]
    uint32_t _pad3C;            // +0x3C
};
static_assert(sizeof(C_StatXPEffect) == 0x40, "C_StatXPEffect must be 0x40 (operator new(64))");
static_assert(offsetof(C_StatXPEffect, m_bAdvanceToLevel) == 0x34, "advance flag at 0x34");

}  // namespace wh::rpgmodule
