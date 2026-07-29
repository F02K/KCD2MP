#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SituationSearch -- constraint-search strategy interface
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4F4D850.  Pure vtable rva 0x3A6B810 (7 slots): [0..4]
// _purecall, [5] CONCRETE sub_1824301CC (defined on the interface itself),
// [6] iface deleting dtor sub_1832B76F0.  Implementors: C_AbstractSituationSearch
// -> {C_LazyNCSituationSearch, C_ForwardCheckingSituationSearch}.  Slot
// semantic roles NOT decompiled [U].

namespace wh::xgenaimodule {

class I_SituationSearch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SituationSearch;
    virtual void _vf0() = 0;      // [0] pure (algorithm-specific) [U role]
    virtual void _vf1() = 0;      // [1] pure [U role]
    virtual void _vf2() = 0;      // [2] pure [U role]
    virtual void _vf3() = 0;      // [3] pure (algorithm-specific) [U role]
    virtual void _vf4() = 0;      // [4] pure [U role]
    virtual void _vf5();          // [5] CONCRETE sub_1824301CC [U role]
    virtual ~I_SituationSearch(); // [6] iface deleting dtor sub_1832B76F0
};
static_assert(sizeof(I_SituationSearch) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
