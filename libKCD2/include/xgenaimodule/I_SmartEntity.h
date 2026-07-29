#pragma once

// -----------------------------------------------
// wh::xgenaimodule::XGenPublic::I_SmartEntity -- public smart-entity interface;
// MI base of C_SmartEntity (@+0xB0), inherited unchanged by the whole entity
// family (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartEntity@XGenPublic@xgenaimodule@wh@@.  Shape (6 slots)
// read from the C_SmartEntity subobject vtable rva 0x4013FD8:
// [0]0x75E404 [1]0x87A540 [2]0xA25AC0 [3]0x3442D60 [4]0x3442D3C [5]0x1A9CEC0 --
// byte-identical across C_SmartEntity/C_InteractiveSmartEntity/C_SmartObject/
// C_NavigationSmartObject/C_SmartArea (implemented once at C_SmartEntity).
// Slot semantics NOT decompiled [U] -- modeled as 6 pure slots (no dtor claim).

namespace wh::xgenaimodule::XGenPublic {

class I_SmartEntity {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartEntity;
    virtual void XseVf0() = 0;   // [0] [U role]
    virtual void XseVf1() = 0;   // [1] [U role]
    virtual void XseVf2() = 0;   // [2] [U role]
    virtual void XseVf3() = 0;   // [3] [U role]
    virtual void XseVf4() = 0;   // [4] [U role]
    virtual void XseVf5() = 0;   // [5] [U role]
};
static_assert(sizeof(I_SmartEntity) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule::XGenPublic
