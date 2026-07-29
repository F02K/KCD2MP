#pragma once
#include "I_CrimeDescription.h"

// -----------------------------------------------
// wh::xgenaimodule::S_VictimCrimeDescription : I_CrimeDescription -- crime with a
// victim (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F44398 (.?AUS_VictimCrimeDescription@... -- struct in source;
// class here per project convention).  vtable 0x1847656E8, 5 slots:
// [0] sub_180F857DC, [1] sub_18039956C, [2] sub_1808D2480, [3] sub_18066CD10
// (return 0), [4] sub_1823CA690.  ctor sub_18327F9B4; created by the factory
// branch sub_18329FFD4 (kind VictimCrime).  Derived: S_TheftDescription,
// S_AnimalCrimeDescription.

namespace wh::xgenaimodule {

class S_VictimCrimeDescription : public I_CrimeDescription {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_VictimCrimeDescription;
    ~S_VictimCrimeDescription() override;   // [0] deleting dtor sub_180F857DC
    void _vf1() override;                   // [1] sub_18039956C (shared with siblings) [U role]
    void _vf2() override;                   // [2] sub_1808D2480 (shared with siblings) [U role]
    void _vf3() override;                   // [3] sub_18066CD10 -- return 0 [U role]
    void _vf4() override;                   // [4] sub_1823CA690 [U role]
};
// payload members (victim id, item values, ...) unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
