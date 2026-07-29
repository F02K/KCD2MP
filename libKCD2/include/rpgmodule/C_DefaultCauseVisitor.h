#pragma once
#include "C_CauseVisitor.h"

// -----------------------------------------------
// wh::rpgmodule::C_DefaultCauseVisitor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184D5C0B8.  The all-no-op C_CauseVisitor intermediate (mirrors
// C_DefaultConstEffectVisitor on the effect side): derived listeners override only the causes
// they care about.  No members, no new slots -- the binary's overrides are nullsubs.  Slot
// bodies are not modeled (see C_CauseVisitor.h); layout-only.

namespace wh::rpgmodule {

class C_DefaultCauseVisitor : public C_CauseVisitor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DefaultCauseVisitor;
};
static_assert(sizeof(C_DefaultCauseVisitor) == 0x08, "C_DefaultCauseVisitor adds no state");

}  // namespace wh::rpgmodule
