#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartAreaManager -- smart-area manager interface;
// PRIMARY base of C_SmartAreaManager (@+0x0) (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartAreaManager@xgenaimodule@wh@@.  Shape (19 slots)
// read from the C_SmartAreaManager primary vtable rva 0x3FE9900:
// [0]0x7FA334 [1]0xB43250 [2]0xB43258 [3]0xB4300C [4]0x32C3CE0 [5]0xB41F14
// [6]0x32C33D4 [7]0x32C3438 [8]0xB41F74 [9]0x1AA5B10 [10]0x19C1668
// [11]0x32C32C0 [12]0x1323B84 [13]0x32C3BFC [14]0x460B10 [15]0x467F88
// [16]0xAF27E0 [17]0x32C42EC [18]0x1AA5B30.  Listener add/remove slots are in
// here (C_SmartEntityResolver subscribes via manager vtbl+128 = slot [16]).
// Slot semantics NOT decompiled [U] -- modeled as 19 pure slots (no dtor claim).

namespace wh::xgenaimodule {

class I_SmartAreaManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartAreaManager;
    virtual void SamVf0()  = 0;   // [0]  [U role]
    virtual void SamVf1()  = 0;   // [1]  [U role]
    virtual void SamVf2()  = 0;   // [2]  [U role]
    virtual void SamVf3()  = 0;   // [3]  [U role]
    virtual void SamVf4()  = 0;   // [4]  [U role]
    virtual void SamVf5()  = 0;   // [5]  [U role]
    virtual void SamVf6()  = 0;   // [6]  [U role]
    virtual void SamVf7()  = 0;   // [7]  [U role]
    virtual void SamVf8()  = 0;   // [8]  [U role]
    virtual void SamVf9()  = 0;   // [9]  [U role]
    virtual void SamVf10() = 0;   // [10] [U role]
    virtual void SamVf11() = 0;   // [11] [U role]
    virtual void SamVf12() = 0;   // [12] [U role]
    virtual void SamVf13() = 0;   // [13] [U role]
    virtual void SamVf14() = 0;   // [14] [U role]
    virtual void SamVf15() = 0;   // [15] [U role]
    virtual void SamVf16() = 0;   // [16] AddListener (resolver subscribes via vtbl+128) [U name]
    virtual void SamVf17() = 0;   // [17] [U role]
    virtual void SamVf18() = 0;   // [18] [U role]
};
static_assert(sizeof(I_SmartAreaManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
