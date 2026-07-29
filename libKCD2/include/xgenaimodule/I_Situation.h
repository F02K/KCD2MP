#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_Situation -- situation interface (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08.
// -----------------------------------------------
// Pure vtable rva 0x3AA4068 (18 slots): [0..16] _purecall, [17] iface deleting
// dtor sub_1832B766C -- dtor is the LAST slot (declared after the 17 pure
// methods).  Sole implementor: C_Situation (overrides all 18).  Slot semantic
// roles NOT decompiled [U].

namespace wh::xgenaimodule {

class I_Situation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Situation;
    virtual void _vf0()  = 0;   // [0]  [U role]
    virtual void _vf1()  = 0;   // [1]  [U role]
    virtual void _vf2()  = 0;   // [2]  [U role]
    virtual void _vf3()  = 0;   // [3]  [U role]
    virtual void _vf4()  = 0;   // [4]  [U role]
    virtual void _vf5()  = 0;   // [5]  [U role]
    virtual void _vf6()  = 0;   // [6]  [U role]
    virtual void _vf7()  = 0;   // [7]  [U role]
    virtual void _vf8()  = 0;   // [8]  [U role]
    virtual void _vf9()  = 0;   // [9]  [U role]
    virtual void _vf10() = 0;   // [10] [U role]
    virtual void _vf11() = 0;   // [11] [U role]
    virtual void _vf12() = 0;   // [12] [U role]
    virtual void _vf13() = 0;   // [13] [U role]
    virtual void _vf14() = 0;   // [14] [U role]
    virtual void _vf15() = 0;   // [15] [U role]
    virtual void _vf16() = 0;   // [16] [U role]
    virtual ~I_Situation();     // [17] iface deleting dtor sub_1832B766C
};
static_assert(sizeof(I_Situation) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
