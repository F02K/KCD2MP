#pragma once
#include "S_VictimCrimeDescription.h"

// -----------------------------------------------
// wh::xgenaimodule::S_TheftDescription : S_VictimCrimeDescription -- theft crime
// record (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F443D8; vtable 0x184765718, 5 slots (overrides only):
// [0] sub_1832806C0, [4] sub_181AA4A00; [1..3] inherited values.  ctor
// sub_18329FEF0; created by the factory branch sub_1832A4968 (theft kind == 28 --
// the kind CalculateCrimeLevel sub_1832A1790 special-cases when summing item
// values).

namespace wh::xgenaimodule {

class S_TheftDescription : public S_VictimCrimeDescription {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_TheftDescription;
    ~S_TheftDescription() override;   // [0] deleting dtor sub_1832806C0
    void _vf4() override;             // [4] sub_181AA4A00 [U role]
};
// payload members unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
