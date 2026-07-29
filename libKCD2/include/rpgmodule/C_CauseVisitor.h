#pragma once

// -----------------------------------------------
// wh::rpgmodule::C_CauseVisitor -- cause visitor interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5FF60, no own vtable (abstract).  The cause-side sibling of
// C_ConstEffectVisitor: one Visit slot per C_*Cause type.  The per-cause slot list is NOT
// enumerated yet -- modeled layout-only (vfptr, 8 bytes); do NOT call or override through
// this model.  Base of C_DefaultCauseVisitor (the all-no-op intermediate) which
// C_EventCounter embeds @+0x18.

namespace wh::rpgmodule {

class C_CauseVisitor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CauseVisitor;
    virtual void _vf0() = 0;   // [0]  slot list not enumerated -- layout-only placeholder
};
static_assert(sizeof(C_CauseVisitor) == 0x08, "C_CauseVisitor is a vfptr-only interface");

}  // namespace wh::rpgmodule
