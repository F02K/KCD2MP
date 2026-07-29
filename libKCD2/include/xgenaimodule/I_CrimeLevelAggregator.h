#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::I_CrimeLevelAggregator -- crime/wanted-level aggregation
// interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (modeled polymorphic; it
// carries the implementor's primary vptr @+0).
// -----------------------------------------------
// RTTI TD rva 0x4F483E0.  No COL/vtable of its own (never most-derived).  Slot
// count (9) read from the sole implementor's primary vtable,
// C_CrimeLevelAggregator (0x183ABEA48); NO dtor slot ([0] is
// GetEffectiveCrimeLevel).  Unknown slots are prefixed _cla_ to avoid MSVC MI
// name-merging with rpgmodule::I_LocationListener's _vfN slots in the implementor
// (G2 lesson).  Interface/implementor slot split modeled all-on-interface [U].

namespace wh::xgenaimodule {

class I_CrimeLevelAggregator {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CrimeLevelAggregator;
    virtual int32_t GetEffectiveCrimeLevel() = 0;   // [0] impl sub_18082DBFC -- bit-scan over the level-flags word, float threshold gate [U exact signature]
    virtual void _cla_vf1() = 0;                    // [1] impl sub_181AA5650 [U role]
    virtual void _cla_vf2() = 0;                    // [2] impl sub_180EC9720 [U role]
    virtual void _cla_vf3() = 0;                    // [3] impl sub_180EC9430 [U role]
    virtual void RemoveDistrictEntry(uint64_t districtKey) = 0;  // [4] impl sub_180EC7CB8 -- linear search of the stride-16 district table, decrements counts [U exact signature]
    virtual void _cla_vf5() = 0;                    // [5] impl sub_180EC9408 [U role]
    virtual void _cla_vf6() = 0;                    // [6] impl sub_180EC7E3C [U role]
    virtual void _cla_vf7() = 0;                    // [7] impl sub_180EC93E0 [U role]
    virtual void _cla_vf8() = 0;                    // [8] impl sub_180EC7E64 [U role]
};
static_assert(sizeof(I_CrimeLevelAggregator) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
