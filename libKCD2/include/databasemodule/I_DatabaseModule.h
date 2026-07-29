#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::databasemodule::I_DatabaseModule -- database module interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVI_DatabaseModule@databasemodule@wh@@ (TD 0x184AEF4A8)  vtable 0x183E3D240, 31 slots
// ([0] dtor, [1]..[27] pure, [28..30] RTTR_ENABLE() trio: [28]/[30] defaults, [29] returns this).
// Implemented by C_DatabaseModule (I_DatabaseModule subobject @+0x10, impl vtable 0x183C38CC0).
// Only the slots exercised by C_ObjectDatabaseBase::Register/Load are semantically attested:
//   [7]  0x38  RegisterTable(name, a3, a4, int flags): news an 88-byte table descriptor
//        (ctor sub_180D17F20), ORs flags into desc+0x50, pushes into the module's vector @+0x08
//        (impl 0x180D17E54)
//   [9]  0x48  LoadTable(name, bool): returns false-> "already/none"; used as the Load gate
//        (impl 0x180D18444)  [arg shape INFERRED from the Load caller]
//   [12] 0x60  GetTableMetadata(name, int rowStride) -> the 88-byte descriptor (desc+0x38 = type
//        dword, verified against the database's m_type)  (impl 0x18068B5A4)
// The remaining 24 slots are undecompiled -- declared as numbered placeholders.

namespace wh::databasemodule {

class I_DatabaseModule {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DatabaseModule;
    virtual ~I_DatabaseModule() = 0;                       // [0]  0x00
    virtual void _vf1() = 0;                               // [1]  0x08
    virtual void _vf2() = 0;                               // [2]  0x10
    virtual void _vf3() = 0;                               // [3]  0x18
    virtual void _vf4() = 0;                               // [4]  0x20
    virtual void _vf5() = 0;                               // [5]  0x28
    virtual void _vf6() = 0;                               // [6]  0x30
    virtual bool RegisterTable(const void* name, int64_t a3, int64_t a4, int flags) = 0;  // [7]
    virtual void _vf8() = 0;                               // [8]  0x40
    virtual bool LoadTable(const void* name, bool a3) = 0; // [9]  0x48  [args INFERRED]
    virtual void _vf10() = 0;                              // [10] 0x50
    virtual void _vf11() = 0;                              // [11] 0x58
    virtual void* GetTableMetadata(const void* name, int rowStride) = 0;  // [12] 0x60
    virtual void _vf13() = 0;                              // [13] 0x68
    virtual void _vf14() = 0;                              // [14] 0x70
    virtual void _vf15() = 0;                              // [15] 0x78
    virtual void _vf16() = 0;                              // [16] 0x80
    virtual void _vf17() = 0;                              // [17] 0x88
    virtual void _vf18() = 0;                              // [18] 0x90
    virtual void _vf19() = 0;                              // [19] 0x98
    virtual void _vf20() = 0;                              // [20] 0xA0
    virtual void _vf21() = 0;                              // [21] 0xA8
    virtual void _vf22() = 0;                              // [22] 0xB0
    virtual void _vf23() = 0;                              // [23] 0xB8
    virtual void _vf24() = 0;                              // [24] 0xC0
    virtual void _vf25() = 0;                              // [25] 0xC8
    virtual void _vf26() = 0;                              // [26] 0xD0
    virtual void _vf27() = 0;                              // [27] 0xD8
    RTTR_ENABLE()  // [28..30]: get_type 0x182741398, get_derived 0x18274135C
};
static_assert(sizeof(I_DatabaseModule) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::databasemodule
