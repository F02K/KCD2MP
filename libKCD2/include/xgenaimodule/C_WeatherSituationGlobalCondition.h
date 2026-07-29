#pragma once
#include "C_AbstractSituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_WeatherSituationGlobalCondition :
// C_AbstractSituationGlobalCondition -- weather global condition
// (direct-from-abstract, NOT pairwise) (KCD2 WHGame.dll 1.5.6, kd7u).  No own
// data members observed; size >= 0x30 MINIMUM -> NO static_assert.
// -----------------------------------------------
// RTTI TD rva 0x4F52110.  Vtable rva 0x3A67728 (13 slots): overrides
// [2]sub_180E9B544 [4]sub_181A72500 [5]sub_1832C3924 [7]sub_1832C316C
// [8]sub_18041A6A0 (shared "return 1" body) [9]sub_1832C2E48 [10]deleting dtor
// sub_1832C283C; [12]sub_1832BC380 inherited from the abstract base.

namespace wh::xgenaimodule {

class C_WeatherSituationGlobalCondition : public C_AbstractSituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeatherSituationGlobalCondition;
    void _vf2() override;    // [2] sub_180E9B544
    void _vf4() override;    // [4] sub_181A72500
    void _vf5() override;    // [5] sub_1832C3924
    void _vf7() override;    // [7] sub_1832C316C
    void _vf8() override;    // [8] sub_18041A6A0 (shared body)
    void _vf9() override;    // [9] sub_1832C2E48
};

}  // namespace wh::xgenaimodule
