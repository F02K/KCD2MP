#pragma once
#include "C_AbstractPairwiseSituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DialogSituationGlobalCondition :
// C_AbstractPairwiseSituationGlobalCondition -- dialog-compatibility global
// condition (KCD2 WHGame.dll 1.5.6, kd7u).  No own data members observed;
// size >= 0x30 MINIMUM (exact operator-new size not chased) -> NO static_assert.
// -----------------------------------------------
// RTTI TD rva 0x4F4D590.  Vtable rva 0x3A675B0 (14 slots): overrides
// [4]sub_181A72480 [5]sub_1832B99D0 [9]sub_1832B7F88 [10]deleting dtor
// sub_1832B74C4 [13]sub_180E9A974; rest inherited.

namespace wh::xgenaimodule {

class C_DialogSituationGlobalCondition : public C_AbstractPairwiseSituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogSituationGlobalCondition;
    void _vf4() override;    // [4]  sub_181A72480
    void _vf5() override;    // [5]  sub_1832B99D0
    void _vf9() override;    // [9]  sub_1832B7F88
    void _vf13() override;   // [13] sub_180E9A974 (pairwise evaluation)
};

}  // namespace wh::xgenaimodule
