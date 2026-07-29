#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartEnvironemntBehaviorCondition -- smart-environment
// behavior condition interface; ref-counted (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.  NOTE: the in-binary class name carries the typo "Environemnt"
// -- preserved verbatim.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartEnvironemntBehaviorCondition@xgenaimodule@wh@@.
// Vtable rva 0x3BFC8B8 (13 slots): [0] concrete [U role], [1..11] _purecall,
// [12] concrete _Destroy (ref-count release -- NOT a C++ dtor slot claim [U]).
// Concretes are built by factory sub_18077FD14 (enum-keyed).

namespace wh::xgenaimodule {

class I_SmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartEnvironemntBehaviorCondition;
    virtual void _vf0();          // [0]  concrete in iface [U role]
    virtual void _vf1()  = 0;     // [1]  [U role]
    virtual void _vf2()  = 0;     // [2]  [U role]
    virtual void _vf3()  = 0;     // [3]  [U role]
    virtual void _vf4()  = 0;     // [4]  [U role]
    virtual void _vf5()  = 0;     // [5]  [U role]
    virtual void _vf6()  = 0;     // [6]  [U role]
    virtual void _vf7()  = 0;     // [7]  [U role]
    virtual void _vf8()  = 0;     // [8]  [U role]
    virtual void _vf9()  = 0;     // [9]  [U role]
    virtual void _vf10() = 0;     // [10] [U role]
    virtual void _vf11() = 0;     // [11] [U role]
    virtual void _Destroy();      // [12] ref-count release [U exact semantics]
};
static_assert(sizeof(I_SmartEnvironemntBehaviorCondition) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
