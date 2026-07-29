#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartObjectHelpersManager -- smart-object helpers manager
// interface; base of C_SmartObjectHelpersManager (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartObjectHelpersManager@xgenaimodule@wh@@.  Shape
// (5 slots) read from the sole implementor C_SmartObjectHelpersManager (vtable
// rva 0x3FECED8: [0]0x32EE060 [1]0x1096C08 [2]0x32EF8C0 [3]0x67CAB8
// [4]0x1096CA4).  Slot semantics NOT decompiled [U] -- modeled as 5 pure slots
// (no dtor claim).

namespace wh::xgenaimodule {

class I_SmartObjectHelpersManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartObjectHelpersManager;
    virtual void SohVf0() = 0;   // [0] [U role]
    virtual void SohVf1() = 0;   // [1] [U role]
    virtual void SohVf2() = 0;   // [2] [U role]
    virtual void SohVf3() = 0;   // [3] [U role]
    virtual void SohVf4() = 0;   // [4] [U role]
};
static_assert(sizeof(I_SmartObjectHelpersManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
