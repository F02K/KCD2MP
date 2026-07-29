#pragma once
#include "S_VictimCrimeDescription.h"

// -----------------------------------------------
// wh::xgenaimodule::S_AnimalCrimeDescription : S_VictimCrimeDescription -- crime
// against an animal (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT proven -> no size
// assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F44418; vtable 0x184765748, 5 slots (overrides only):
// [0] sub_183280674, [2] sub_182FCFF40, [3] sub_180836120; [1]/[4] inherited
// values.  ctor sub_18329FD80 (doubles as the factory branch for kind Animal).

namespace wh::xgenaimodule {

class S_AnimalCrimeDescription : public S_VictimCrimeDescription {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_AnimalCrimeDescription;
    ~S_AnimalCrimeDescription() override;   // [0] deleting dtor sub_183280674
    void _vf2() override;                   // [2] sub_182FCFF40 [U role]
    void _vf3() override;                   // [3] sub_180836120 (base returns 0) [U role]
};
// payload members unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
