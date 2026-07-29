#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartObjectsManager -- smart-objects manager interface;
// MI base of C_SmartObjectsManager (@+0x8) (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartObjectsManager@xgenaimodule@wh@@.  Shape (14 slots)
// read from the C_SmartObjectsManager subobject vtable rva 0x3FECDE0:
// [0]0x7E3D68 [1]0xAF2A6C [2]0x32F0624 [3]0x7FA3B0 [4]0x1A9D440(ICF)
// [5]0x7E3D60(ICF) [6]0x5CA8D8 [7]0xA25BA4 [8]0xB43E84 [9]0x7F9AE4
// [10]0x32EFB7C [11]0x1096C94 [12]0x10457DC [13]0x129F208.  Slots [4]/[5]
// carry ICF labels (folded stubs, not real Concurrency calls).  Slot semantics
// NOT decompiled [U] -- modeled as 14 pure slots (no dtor claim).

namespace wh::xgenaimodule {

class I_SmartObjectsManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartObjectsManager;
    virtual void SomVf0()  = 0;   // [0]  [U role]
    virtual void SomVf1()  = 0;   // [1]  [U role]
    virtual void SomVf2()  = 0;   // [2]  [U role]
    virtual void SomVf3()  = 0;   // [3]  [U role]
    virtual void SomVf4()  = 0;   // [4]  ICF-folded stub [U role]
    virtual void SomVf5()  = 0;   // [5]  ICF-folded stub [U role]
    virtual void SomVf6()  = 0;   // [6]  [U role]
    virtual void SomVf7()  = 0;   // [7]  [U role]
    virtual void SomVf8()  = 0;   // [8]  [U role]
    virtual void SomVf9()  = 0;   // [9]  [U role]
    virtual void SomVf10() = 0;   // [10] [U role]
    virtual void SomVf11() = 0;   // [11] [U role]
    virtual void SomVf12() = 0;   // [12] [U role]
    virtual void SomVf13() = 0;   // [13] [U role]
};
static_assert(sizeof(I_SmartObjectsManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
