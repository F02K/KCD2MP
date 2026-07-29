#pragma once
#include <cstdint>
#include <cstddef>
#include "C_SoulValueEffect.h"
#include "E_SoulSkill.h"

// -----------------------------------------------
// wh::rpgmodule::C_SkillXPEffect -- add skill XP / advance skill level (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_SkillXPEffect@rpgmodule@wh@@ (TD 0x184D4EAD0)  vtable 0x183A8BC48 (9 slots).
// sizeof 0x40 (ctor sub_180C65AD0 writes through +0x3C; same factory family as C_StatXPEffect).
// C_SoulValueEffect<E_SoulSkill, fixed_t<uint32_t,25>> -> C_SoulEffect -> C_Effect.
// Ctor sub_180C65AD0(this, owner, targetWuid, skillId, &amount, bAdvanceToLevel, contextParam).
// Mid-field roles NOT traced through Apply (only the ctor init values are attested) -- the
// +0x2C init of 35 is a sentinel (E_SoulSkill count = "no linked stat" candidate, UNVERIFIED).

namespace wh::rpgmodule {

class C_SkillXPEffect : public C_SoulValueEffect<E_SoulSkill> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkillXPEffect;
    // (m_valueId @+0x18 = skill id, m_amount @+0x1C from the base template)
    float    m_targetXP;        // +0x20  ctor 0 [role INFERRED from the C_StatXPEffect twin]
    int32_t  m_oldSkillXP;      // +0x24  ctor 0 [role INFERRED]
    int32_t  m_skillLevelDelta; // +0x28  ctor 0 [role INFERRED]
    int32_t  m_linkedId2C;      // +0x2C  ctor 35 (sentinel; role UNVERIFIED)
    int32_t  m_oldLinkedStatXP;  // +0x30  linked stat's XP before apply (soul XP block, only if m_linkedId2C != 35)
    int32_t  m_linkedStatXPDelta;// +0x34  linked stat's XP gained = newXP - m_oldLinkedStatXP
    uint8_t  m_bAdvanceToLevel; // +0x38  0 = AddSkillXP, 1 = AdvanceToSkillLevel
    uint8_t  _pad39[3];         // +0x39
    int32_t  m_contextParam;    // +0x3C  [role INFERRED]
};
static_assert(sizeof(C_SkillXPEffect) == 0x40, "C_SkillXPEffect must be 0x40");
static_assert(offsetof(C_SkillXPEffect, m_bAdvanceToLevel) == 0x38, "advance flag at 0x38");

}  // namespace wh::rpgmodule
