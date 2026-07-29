#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_PositionAndOrientation -- position/orientation provider
// interface; base of C_SmartHelper (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_PositionAndOrientation@xgenaimodule@wh@@.  No standalone
// vtable; shape (3 slots) read from the sole enumerated implementor
// C_SmartHelper (vtable rva 0x4014090: [0]0x1A72EB0 [1]0x1A7FB20 [2]0x3442294).
// Slot split iface-vs-own and semantics NOT decompiled [U] -- modeled as 3
// pure slots (no dtor claim).

namespace wh::xgenaimodule {

class I_PositionAndOrientation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_PositionAndOrientation;
    virtual void PaoVf0() = 0;   // [0] [U role]
    virtual void PaoVf1() = 0;   // [1] [U role]
    virtual void PaoVf2() = 0;   // [2] [U role]
};
static_assert(sizeof(I_PositionAndOrientation) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
