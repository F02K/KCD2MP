#pragma once
#include "C_ConstEffectVisitor.h"

// -----------------------------------------------
// wh::rpgmodule::C_DefaultConstEffectVisitor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184CE8F50.  The all-no-op C_ConstEffectVisitor intermediate (already anticipated
// by the C_ConstEffectVisitor.h banner): derived listeners override only the effects they
// care about.  No members, no new slots -- the binary's overrides are nullsubs, so the
// per-slot overrides are not redeclared here; layout-only.

namespace wh::rpgmodule {

class C_DefaultConstEffectVisitor : public C_ConstEffectVisitor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DefaultConstEffectVisitor;
};
static_assert(sizeof(C_DefaultConstEffectVisitor) == 0x08, "C_DefaultConstEffectVisitor adds no state");

}  // namespace wh::rpgmodule
