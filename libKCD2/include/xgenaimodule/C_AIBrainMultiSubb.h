#pragma once
#include <vector>
#include "C_AIBrain.h"
#include "C_SubbSwitchingLogic.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIBrainMultiSubb : C_AIBrain -- concrete brain driving a
// VECTOR of subbrains with embedded switching logic (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x198 (create-site proven: sub_1804F75C0(408) @0x180419CE2 /
// 0x1821E6ECD in factory sub_180419AA8).
// -----------------------------------------------
// RTTI TD rva 0x4EFD700; COL rva 0x451DB68; vtable 0x183FD7A48, 35 slots (adds
// NONE; overrides 24: 0,5,6,7,9,10..14,17,21,23,24,26..32,34 -- fills all 15
// pure -> CONCRETE).  ctor sub_180418854(this, owner) (base ctor sub_180418CF4
// then vptr + members); dtor sub_180D4712C (deletes each subbrain, resets the
// switching-logic vptr to I_DebugDraw); deleting dtor = slot [0] sub_180D47898.
// Shared stub sub_18075EDD0 serves slots [31][32][34].  Override bodies:
// [5]sub_181157C38 [6]sub_18041A6A4 [7]sub_1807600B4 [9]sub_181A7FB10
// [10]sub_183209E78 [11]sub_1832097E0 [12]sub_183209C7C [13]sub_1813A704C
// [14]sub_1823DA474 [17]sub_1832096CC [21]sub_181A9D720 [23]sub_1805B0154
// [24]sub_18041878C [26]sub_180419264 [27]sub_183209D1C [28]sub_1808AE084
// [29]sub_1832095C4 [30]sub_183209524.

namespace wh::xgenaimodule {

class C_SUBBBase;

class C_AIBrainMultiSubb : public C_AIBrain {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIBrainMultiSubb;
    void _vf7() override;    // [7]  sub_1807600B4 [U role]
    void _vf9() override;    // [9]  sub_181A7FB10 [U role]
    void _vf12() override;   // [12] sub_183209C7C [U role]
    void _vf13() override;   // [13] sub_1813A704C [U role]
    void _vf14() override;   // [14] sub_1823DA474 [U role]
    void _vf17() override;   // [17] sub_1832096CC [U role]
    void _vf21() override;   // [21] sub_181A9D720 [U role]
    void _vf23() override;   // [23] sub_1805B0154 [U role]
    void _vf24() override;   // [24] sub_18041878C [U role]
    void _vf26() override;   // [26] sub_180419264 [U role]
    void _vf27() override;   // [27] sub_183209D1C [U role]
    void _vf28() override;   // [28] sub_1808AE084 [U role]
    void _vf29() override;   // [29] sub_1832095C4 [U role]
    void _vf30() override;   // [30] sub_183209524 [U role]
    void _vf31() override;   // [31] sub_18075EDD0 (shared stub)
    void _vf32() override;   // [32] sub_18075EDD0 (shared stub)
    void _vf34() override;   // [34] sub_18075EDD0 (overrides the base no-op)

    std::vector<C_SUBBBase*> m_subbrains;      // +0xF8  dtor deletes each; pointee = SUBB family root
                                               //        (G4A "C_AISubbrain" corrected via G5E) [U original name]
    C_SubbSwitchingLogic     m_switchingLogic; // +0x110  embedded (0x88), back-ptr @+0x130 = this
};
static_assert(sizeof(C_AIBrainMultiSubb) == 0x198, "C_AIBrainMultiSubb must be 0x198 (operator new(408))");

}  // namespace wh::xgenaimodule
