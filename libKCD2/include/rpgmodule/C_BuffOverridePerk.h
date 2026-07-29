#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffOverridePerk -- perk overriding a buff (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D53D40  vtable 0x183BD2EE0.  sizeof 0x38 (PROVEN: factory sub_18160460C
// operator new(56)). Extra: +0x30 qword = 0. Backed by C_PerkBuffOverrideDatabase rows.

namespace wh::rpgmodule {

class C_BuffOverridePerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BuffOverridePerk;
    void* m_pResolvedDef;   // +0x30  ctor 0; Resolve[9] override (0x1811D682C) caches the sub_1811D6860(m_id) lookup result here [pointer to the buff-override definition/row; pointee not RE'd]. Name inferred.
};
static_assert(sizeof(C_BuffOverridePerk) == 0x38, "C_BuffOverridePerk must be 0x38 (operator new(56))");

}  // namespace wh::rpgmodule
