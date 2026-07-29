#pragma once
#include "C_AbstractSituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PlayerDistanceSituationGlobalCondition :
// C_AbstractSituationGlobalCondition -- player-distance global condition
// (direct-from-abstract, NOT pairwise) (KCD2 WHGame.dll 1.5.6, kd7u).  Size
// 0x34 MINIMUM (from ctor writes) -> NO static_assert.  ctor
// sub_1809C0914(this, a2, a3, float a4=distance) delegates DIRECTLY to the
// abstract ctor.
// -----------------------------------------------
// RTTI TD rva 0x4F4D780.  Vtable rva 0x3A676B8 (13 slots): overrides
// [2]sub_180E9AA8C [4]sub_181A72CF0 [5]sub_1832B9B50 [7]sub_1832B876C
// [8]sub_18041A6A0 (shared "return 1" body) [9]sub_1832B80C4 [10]deleting dtor
// sub_1832B758C; [12]sub_1832BC380 inherited from the abstract base.

namespace wh::xgenaimodule {

class C_PlayerDistanceSituationGlobalCondition : public C_AbstractSituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerDistanceSituationGlobalCondition;
    void _vf2() override;    // [2] sub_180E9AA8C
    void _vf4() override;    // [4] sub_181A72CF0
    void _vf5() override;    // [5] sub_1832B9B50
    void _vf7() override;    // [7] sub_1832B876C
    void _vf8() override;    // [8] sub_18041A6A0 (shared body)
    void _vf9() override;    // [9] sub_1832B80C4

    float m_distance;    // +0x30  ctor a4
};
// size 0x34 MINIMUM (ctor writes only) -- no static_assert.

}  // namespace wh::xgenaimodule
