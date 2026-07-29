#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_CompanionPerk -- perk affecting companions (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D53DE8  vtable 0x183A64F90.  sizeof 0x38 (ctor sub_180964944 write extent;
// in-place ctor -- allocation not observed, size INFERRED from the +0x30 tail member).
// Dtor chain sub_18096485C. Backed by C_PerkCompanionDatabase rows.

namespace wh::rpgmodule {

struct S_PerkCompanion;   // row type (see S_PerkCompanion.h)

class C_CompanionPerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CompanionPerk;
    S_PerkCompanion* m_pCompanionRow;   // +0x30  Resolve[9] 0x18116DA84: resolved perk_companion row (sub_18116DAB0 lookup of m_id, null if absent); ctor 0. Activate 0x180A20180 reads +0x10 m_companionType
};
static_assert(sizeof(C_CompanionPerk) == 0x38, "C_CompanionPerk must be 0x38 (ctor write extent)");

}  // namespace wh::rpgmodule
