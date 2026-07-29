#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SituationGlobalCondition -- global situation-condition
// interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4F4D960.  Pure vtable rva 0x3FE87B0 (11 slots): [0..9]
// _purecall, [10] iface deleting dtor sub_1832B76C4.  Implementor chain:
// C_AbstractSituationGlobalCondition -> C_AbstractPairwiseSituationGlobalCondition
// -> {Dialog, BreakSymmetry, Distance}; direct-from-abstract leaves
// {PlayerDistance, Weather}.  Slot semantic roles NOT decompiled [U].

namespace wh::xgenaimodule {

class I_SituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SituationGlobalCondition;
    virtual void _vf0() = 0;                 // [0] [U role]
    virtual void _vf1() = 0;                 // [1] [U role]
    virtual void _vf2() = 0;                 // [2] [U role]
    virtual void _vf3() = 0;                 // [3] [U role]
    virtual void _vf4() = 0;                 // [4] [U role]
    virtual void _vf5() = 0;                 // [5] [U role]
    virtual void _vf6() = 0;                 // [6] [U role]
    virtual void _vf7() = 0;                 // [7] [U role]
    virtual void _vf8() = 0;                 // [8] [U role]
    virtual void _vf9() = 0;                 // [9] [U role]
    virtual ~I_SituationGlobalCondition();   // [10] iface deleting dtor sub_1832B76C4
};
static_assert(sizeof(I_SituationGlobalCondition) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
