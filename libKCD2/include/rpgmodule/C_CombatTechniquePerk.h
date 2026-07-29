#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_CombatTechniquePerk -- perk unlocking a combat technique
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D53DA8  vtable 0x183B562D0.  sizeof 0x40 (PROVEN: factory sub_1812E9240
// operator new(64)). Factory sub_1812E9240 ZERO-inits +0x30/+0x38 (empty range); the Resolve[9]
// override sub_1804C8DFC FILLS them with the equal_range [lower_bound, upper_bound) of this perk's
// m_id (CryGUID) in C_PerkCombatTechniqueDatabase's sorted S_PerkCombatTechnique* pointer index
// (std::vector {first @db+0x98, last @db+0xA0}) -- one perk -> N combat-technique rows.

namespace wh::rpgmodule {

struct S_PerkCombatTechnique;   // row type (see S_PerkCombatTechnique.h)

class C_CombatTechniquePerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTechniquePerk;
    S_PerkCombatTechnique** m_rowsBegin;   // +0x30  Resolve[9] 0x1804C8DFC: lower_bound (sub_18046AF58) of m_id in C_PerkCombatTechniqueDatabase sorted S_PerkCombatTechnique* index (db+0x98..+0xA0); factory zero-inits (empty)
    S_PerkCombatTechnique** m_rowsEnd;     // +0x38  Resolve[9] 0x1804C8DFC: upper_bound (sub_1804C8E50) of the equal_range; iterate [m_rowsBegin, m_rowsEnd)
};
static_assert(sizeof(C_CombatTechniquePerk) == 0x40, "C_CombatTechniquePerk must be 0x40 (operator new(64))");

}  // namespace wh::rpgmodule
