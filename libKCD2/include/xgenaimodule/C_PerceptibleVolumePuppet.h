#pragma once
#include "I_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptibleVolumePuppet : I_AIPuppet -- puppet for perceptible
// volumes (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (create-site proven:
// sub_1804F75C0(16) inside sub_180D45C38 = the C_PerceptibleVolume host ctor).
// -----------------------------------------------
// RTTI TD rva 0x4F4C670; vtable 0x183A98868, 25 slots. Real ctor
// sub_180D45D6C(this, volume). The host ctor then loads r8 = this puppet before the
// spine ctor call -- the disasm proof that C_AIObject+0x18 holds the PUPPET
// (G2_supplement §3.1/§7.1).
// Overrides: [0] dtor sub_1810083F4; [1..3] return 0; [4] sub_181AA57C0 --
// returns &m_volume->wuid (+8) [GetIdPtr semantics]; [5] sub_181206350 --
// returns &m_volume->m_pos (+0x68); [6] sub_1832AECFC; [7] sub_180D465B8 --
// derives dir/extent from m_volume->GetBounds() AABB; [8] sub_1832AECC8;
// [9] sub_181AA57A0; [10] sub_181AA5790; [11] sub_181A72B70; [12]/[13] sub_18046B830
// (shared identity stub, return a2); [14] sub_181AA57B0; [16] sub_18066CD10;
// [17..20] nullsub_1.  Interface defaults [15]/[21..24] NOT overridden (same
// EAs as I_AIPuppet defaults: sub_181AA5630 / sub_18066CD10 / sub_18041A6A0 /
// nullsub_1 x2).

namespace wh::xgenaimodule {

class C_PerceptibleVolume;   // host (see C_PerceptibleVolume.h)

class C_PerceptibleVolumePuppet : public I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptibleVolumePuppet;
    ~C_PerceptibleVolumePuppet() override;
    void _vf1() override;
    void _vf2() override;
    void* _vf3() override;
    void _vf4() override;
    void UpdateCachedTransform() override;   // [5] sub_181206350 -- this impl just returns &m_volume->m_pos (volume pos IS the transform)
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
    void _vf19() override;
    void _vf20() override;

    C_PerceptibleVolume* m_volume;   // +0x08  host volume [V]
};
static_assert(sizeof(C_PerceptibleVolumePuppet) == 0x10, "C_PerceptibleVolumePuppet must be 0x10");

}  // namespace wh::xgenaimodule
