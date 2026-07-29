#pragma once
#include "C_AbstractPairwiseSituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_BreakSymmetrySituationGlobalCondition :
// C_AbstractPairwiseSituationGlobalCondition -- symmetry-breaking global
// condition (prevents mirrored duplicate assignments) (KCD2 WHGame.dll 1.5.6,
// kd7u).  No own data members observed; size >= 0x30 MINIMUM -> NO
// static_assert.
// -----------------------------------------------
// RTTI TD rva 0x4F4D5E0.  Vtable rva 0x47672D8 (14 slots): overrides
// [4]sub_181A72470 (shared body with C_SituationSubBrain GetSubbrainType)
// [5]sub_1832B99C8 [9]sub_1832B7ED0 [13]sub_1832BCF24; [10]dtor sub_1832B73B8
// shared with the abstract base (ICF-folded); rest inherited.

namespace wh::xgenaimodule {

class C_BreakSymmetrySituationGlobalCondition : public C_AbstractPairwiseSituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BreakSymmetrySituationGlobalCondition;
    void _vf4() override;    // [4]  sub_181A72470 (shared body)
    void _vf5() override;    // [5]  sub_1832B99C8
    void _vf9() override;    // [9]  sub_1832B7ED0
    void _vf13() override;   // [13] sub_1832BCF24 (pairwise evaluation)
};

}  // namespace wh::xgenaimodule
