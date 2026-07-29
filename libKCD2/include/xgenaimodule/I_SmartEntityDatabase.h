#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartEntityDatabase -- smart-entity template DB
// interface; primary MI base of C_SmartEntityDatabase (@+0x0) (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartEntityDatabase@xgenaimodule@wh@@.  Shape (6 slots)
// read from the C_SmartEntityDatabase primary vtable rva 0x3BB0710:
// [0]sub_1819C1814 [1]sub_181097764 [2]sub_180D9B720 [3]sub_1832C3194
// [4]sub_1832C3B54 [5]sub_1832C2998.  Method roles NOT reversed [U] -- modeled
// as 6 pure slots (no dtor claim).

namespace wh::xgenaimodule {

class I_SmartEntityDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartEntityDatabase;
    virtual void SedVf0() = 0;   // [0] [U role]
    virtual void SedVf1() = 0;   // [1] [U role]
    virtual void SedVf2() = 0;   // [2] [U role]
    virtual void SedVf3() = 0;   // [3] [U role]
    virtual void SedVf4() = 0;   // [4] [U role]
    virtual void SedVf5() = 0;   // [5] [U role]
};
static_assert(sizeof(I_SmartEntityDatabase) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
