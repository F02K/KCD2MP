#pragma once
#include "C_AbstractSituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AbstractPairwiseSituationGlobalCondition :
// C_AbstractSituationGlobalCondition -- base of the pairwise (two-participant)
// conditions (KCD2 WHGame.dll 1.5.6, kd7u).  STILL ABSTRACT ([4][5][9] pure
// from the abstract base + new pure [13]).  Adds NO data members -- size ==
// abstract base (0x30 MINIMUM, no static_assert).  ctor sub_1809C08C8(this,
// a2, a3) delegates to the abstract ctor; sub_1809C0158 processes the +0x18
// vector.
// -----------------------------------------------
// RTTI TD rva 0x4F4D680.  Vtable rva 0x3FE8738 (14 slots): overrides
// [2]sub_180E9B33C [7]sub_1832B8730 [8]sub_18041A6A0 (shared "return 1" body)
// [12]sub_1832BC344; [10]dtor sub_1832B73B8 (shared with base); adds
// [13]_purecall (NEW, still pure).

namespace wh::xgenaimodule {

class C_AbstractPairwiseSituationGlobalCondition : public C_AbstractSituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AbstractPairwiseSituationGlobalCondition;
    void _vf2() override;        // [2]  sub_180E9B33C
    void _vf7() override;        // [7]  sub_1832B8730
    void _vf8() override;        // [8]  sub_18041A6A0 (shared body)
    void _vf12() override;       // [12] sub_1832BC344
    virtual void _vf13() = 0;    // [13] NEW, pure (pairwise evaluation) [U role]
};
// no new data; size 0x30 MINIMUM -- no static_assert.

}  // namespace wh::xgenaimodule
