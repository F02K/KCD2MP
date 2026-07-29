#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_DebugDraw -- debug-draw hook interface (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_DebugDraw@xgenaimodule@wh@@. No standalone vtable observed
// (never most-derived); known only as the +0x10 MI base of C_NPCManager, whose
// subobject vtable 0x183FF1AE8 has 2 slots: [0] dtor thunk sub_18213D6A0,
// [1] nullsub_1. Slot [1] presumed the draw hook -- name inferred from the class
// name, UNVERIFIED.

namespace wh::xgenaimodule {

class I_DebugDraw {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DebugDraw;
    virtual ~I_DebugDraw();          // [0]
    virtual void DebugDraw() = 0;    // [1] [U name/signature; C_NPCManager impl = nullsub_1]
};
static_assert(sizeof(I_DebugDraw) == 0x08, "I_DebugDraw is vptr-only");

}  // namespace wh::xgenaimodule
