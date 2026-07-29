#pragma once
#include <cstdint>
#include "../framework/WUID.h"
#include "I_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_GeneratedSOPuppet : I_AIPuppet -- puppet for runtime-generated
// smart objects (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x28 (create-site proven:
// tracked alloc 40 inside sub_180D9A994).
// -----------------------------------------------
// RTTI TD rva 0x4FF5E60; vtable 0x183AA3D08, 25 slots. Real ctor
// sub_180D9AB40(this, const WUID*, blobA, blobB). Generated WUID =
// (++counter | 0x0800000000000000) -- tag 0x08 marks generated-SO puppets. Registers
// in the puppet registry q_185496338 like C_AIPuppet.
// Overrides: [0] dtor sub_1810083C8; [1..3] sub_18066CD10 (return 0); [4] sub_181A71D30;
// [5] sub_181A72EC0; [6] sub_181A72D20; [7] sub_1809F44F0; [8] sub_180838AE0;
// [9] sub_181A6ED60; [10] sub_18066CD10; [11] sub_181A72B70; [12] sub_18344750C;
// [13] sub_1834476BC; [14] sub_1809F44F0; [16] sub_18066CD10; [17..20] nullsub_1;
// [22] sub_180838AE0 (vs return-1 default). [15]/[21]/[23]/[24] keep the defaults.

namespace wh::xgenaimodule {

class C_GeneratedSOPuppet : public I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GeneratedSOPuppet;
    ~C_GeneratedSOPuppet() override;
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
    void _vf19() override;
    void _vf20() override;
    bool _vf22() override;                   // [22] sub_180838AE0

    framework::WUID m_wuid;        // +0x08  generated: (++counter | 0x0800000000000000)
    uint8_t         m_blobA[0xC];  // +0x10  12-byte blob copied from ctor arg 3 [U type]
    uint8_t         m_blobB[0xC];  // +0x1C  12-byte blob copied from ctor arg 4 (packed/unaligned in binary) [U type]
};
static_assert(sizeof(C_GeneratedSOPuppet) == 0x28, "C_GeneratedSOPuppet must be 0x28");

}  // namespace wh::xgenaimodule
