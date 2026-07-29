#pragma once
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_ApplyOnLoadPerk -- abstract C_Perk stage: effects re-applied on save-load
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ApplyOnLoadPerk@rpgmodule@wh@@ (TD 0x184D53978).  No vtable emitted (never
// instantiated) and no data members -- derived members start at +0x30 = sizeof(C_Perk).
// Introduces vtable slot [13] past C_Perk's 13: the effect applier that Activate [1] runs
// before the base activate and ResolveAlias [8] re-runs after Resolve when m_active -- the
// save-load re-apply this class is named for (concrete impl: C_SoulAbilityPerk 0x1804706D4).

namespace wh::rpgmodule {

class C_ApplyOnLoadPerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ApplyOnLoadPerk;
    virtual void ApplyEffects() = 0;   // [13] 0x68
};
static_assert(sizeof(C_ApplyOnLoadPerk) == 0x30, "no members beyond C_Perk");

}  // namespace wh::rpgmodule
