#pragma once
#include <cstdint>
#include "../framework/WUID.h"
#include "C_PuppetAdapter.h"

// -----------------------------------------------
// wh::xgenaimodule::C_FormationAnchorPuppet : C_PuppetAdapter : I_AIPuppet --
// formation anchor-point puppet (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x28
// (create-site proven: sub_1804F75C0(40) inside sub_1832A598C).
// -----------------------------------------------
// RTTI TD rva 0x4F47258; vtable 0x184766A68, 25 slots. Real ctor
// sub_1832A0390(this, const WUID*, def): writes the C_PuppetAdapter vtable first
// (base), then own fields; the paired C_AIObject is built right after with this
// puppet as its +0x18 (sub_1832A5A55).
// Overrides: [0] dtor sub_1810083C8 (shared); [1..3] return 0; [4] sub_181A71D30;
// [5] sub_181A72EC0; [6] sub_181A72D20; [7] sub_181AA57B0; [8] sub_180838AE0;
// [9] sub_181AA5670; [10] sub_181AABF10; [11] sub_1804C88CC; [12] sub_183450BC0;
// [13] sub_183450C78; [14] sub_181AA5630; [16] sub_18066CD10; [17] sub_181AA56A0;
// [18] sub_181AA5690; [19] sub_1832A5AEC; [20] sub_1832A5BB4. Defaults [15]/[21..24]
// inherited unchanged.

namespace wh::xgenaimodule {

class C_FormationAnchorPuppet : public C_PuppetAdapter {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FormationAnchorPuppet;
    ~C_FormationAnchorPuppet() override;
    void _vf1() override;
    void _vf2() override;
    void* _vf3() override;
    void _vf4() override;
    void UpdateCachedTransform() override;   // [5] sub_181A72EC0
    void _vf6() override;
    void _vf7() override;
    void _vf8() override;
    void _vf9() override;
    void _vf10() override;
    void _vf11() override;
    void _vf12() override;
    void _vf13() override;
    void _vf14() override;
    void _vf16() override;
    void _vf17() override;
    void _vf18() override;
    void _vf19() override;                   // [19] sub_1832A5AEC
    void _vf20() override;                   // [20] sub_1832A5BB4

    framework::WUID m_wuid;        // +0x08  (possibly a C_PuppetAdapter member -- see adapter header note)
    uint64_t        m_defQword;    // +0x10  = def+0x10 [U role]
    uint32_t        m_defDword;    // +0x18  = def+0x18 [U role]
    float           m_direction[3];// +0x1C  direction vector rotated out of the quat at def+0x00..0x0C [V]
};
static_assert(sizeof(C_FormationAnchorPuppet) == 0x28, "C_FormationAnchorPuppet must be 0x28");

}  // namespace wh::xgenaimodule
