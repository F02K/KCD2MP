#pragma once
#include "C_AbstractPairwiseSituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DistanceSituationGlobalCondition :
// C_AbstractPairwiseSituationGlobalCondition -- participant-distance global
// condition (KCD2 WHGame.dll 1.5.6, kd7u).  Size 0x40 MINIMUM (from ctor
// writes; exact operator-new size not chased) -> NO static_assert.  ctor
// sub_1809BFDAC(this, a2, a3, float a4=minDist, float a5=maxDist) delegates to
// the pairwise ctor.
// -----------------------------------------------
// RTTI TD rva 0x4F4D630.  Vtable rva 0x3A67630 (14 slots): overrides
// [4]sub_18066CD10 [5]sub_1832B9A78 [9]sub_1832B7FE0 [10]deleting dtor
// sub_1832B7514 [13]sub_180E9B418; rest inherited.

namespace wh::xgenaimodule {

class C_DistanceSituationGlobalCondition : public C_AbstractPairwiseSituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DistanceSituationGlobalCondition;
    void _vf4() override;    // [4]  sub_18066CD10
    void _vf5() override;    // [5]  sub_1832B9A78
    void _vf9() override;    // [9]  sub_1832B7FE0
    void _vf13() override;   // [13] sub_180E9B418 (pairwise evaluation)

    float m_minDist;     // +0x30  ctor a4
    float m_maxDist;     // +0x34  ctor a5
    float m_minDistSq;   // +0x38  a4*a4
    float m_maxDistSq;   // +0x3C  a5*a5
};
// size 0x40 MINIMUM (ctor writes only) -- no static_assert.

}  // namespace wh::xgenaimodule
