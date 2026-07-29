#pragma once
#include <cstdint>
#include <vector>
#include "I_SituationGlobalCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AbstractSituationGlobalCondition : I_SituationGlobalCondition
// -- shared base of all global situation conditions (KCD2 WHGame.dll 1.5.6,
// kd7u).  ABSTRACT ([2][4][5][7][8][9] stay pure).  Base size 0x30 MINIMUM
// (from ctor writes; exact operator-new size not chased -- factory/registration
// path) -> NO static_assert.  ctor sub_1809C1B50(this, __int128* a2, a3).
// -----------------------------------------------
// RTTI TD rva 0x4F4D890.  Vtable rva 0x3A67798 (13 slots): [0]sub_181A731A0
// [1]sub_181AA36C0 [2]_purecall [3]sub_181192104 [4]_purecall [5]_purecall
// [6]sub_1832BB308 [7]_purecall [8]_purecall [9]_purecall [10]dtor
// sub_1832B73B8 [11]sub_1832B9850 (NEW) [12]sub_1832BC380 (NEW).

namespace wh::xgenaimodule {

class C_AbstractSituationGlobalCondition : public I_SituationGlobalCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AbstractSituationGlobalCondition;
    void _vf0() override;                          // [0] sub_181A731A0
    void _vf1() override;                          // [1] sub_181AA36C0
    void _vf3() override;                          // [3] sub_181192104
    void _vf6() override;                          // [6] sub_1832BB308
    ~C_AbstractSituationGlobalCondition() override; // [10] dtor sub_1832B73B8
    virtual void _vf11();                          // [11] NEW sub_1832B9850
    virtual void _vf12();                          // [12] NEW sub_1832BC380

    uint8_t               m_paramBlob[16]; // +0x08  __int128 copied from ctor a2; likely an id/param
                                           //   pair (possibly 2x framework::WUID) [U exact type]
    std::vector<uint32_t> m_indices;       // +0x18  built by sub_18059D164 (elem 4)
};
// size 0x30 MINIMUM (ctor writes only) -- no static_assert.

}  // namespace wh::xgenaimodule
