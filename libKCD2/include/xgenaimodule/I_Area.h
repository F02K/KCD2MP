#pragma once
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::xgenaimodule::I_Area -- area interface; MI base of C_TriggerArea /
// C_AreaUnion (@+0x60) and C_SmartArea (@+0x188) (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_Area@xgenaimodule@wh@@.  Shape (17 slots) read from the
// subobject vtables: C_TriggerArea rva 0x3A6C0E0, C_AreaUnion rva 0x3B27A40,
// C_SmartArea rva 0x3AA3E78.  [0] = Overlaps: C_TriggerArea impl sub_1805CD0E4
// reads the +0x84 min/max Vec3 pair as an AABB and tests the argument point [V]
// (exact signature [U]).  [1] shared adjustor body sub_181AA49C0 (this-0x58);
// [10] sub_1805CB3E8 and [15] sub_18213A608 shared between TriggerArea/AreaUnion.
// Remaining slot roles NOT decompiled [U].

namespace wh::xgenaimodule {

class I_Area {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Area;
    virtual bool Overlaps(const Vec3& pos) = 0;  // [0]  AABB/shape point test [V role, U signature]
    virtual void ArVf1()  = 0;   // [1]  shared adjustor body [U role]
    virtual void ArVf2()  = 0;   // [2]  [U role]
    virtual void ArVf3()  = 0;   // [3]  [U role]
    virtual void ArVf4()  = 0;   // [4]  [U role]
    virtual void ArVf5()  = 0;   // [5]  [U role]
    virtual void ArVf6()  = 0;   // [6]  nullsub in both areas [U role]
    virtual void ArVf7()  = 0;   // [7]  [U role]
    virtual void ArVf8()  = 0;   // [8]  [U role]
    virtual void ArVf9()  = 0;   // [9]  [U role]
    virtual void ArVf10() = 0;   // [10] shared body sub_1805CB3E8 [U role]
    virtual void ArVf11() = 0;   // [11] [U role]
    virtual void ArVf12() = 0;   // [12] [U role]
    virtual void ArVf13() = 0;   // [13] [U role]
    virtual void ArVf14() = 0;   // [14] [U role]
    virtual void ArVf15() = 0;   // [15] shared body sub_18213A608 [U role]
    virtual void ArVf16() = 0;   // [16] [U role]
};
static_assert(sizeof(I_Area) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
