#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::dialogmodule::S_DialogueAttachmentProceduralParams -- Mannequin proc-clip params
// (KCD2 1.5.6, kd7u).  sizeof 0x10 (alloc 16; also embedded in the clip @+0x28).
// -----------------------------------------------
// RTTI TD 0x184B4EEF0; vtable 0x183BB6AC8 (7 slots); created standalone as a shared_ptr
// via sub_181560D58; dtor [0] sub_182763234. Bases per RTTI: IProceduralParams +
// IProceduralParamsEditor (all mdisp 0, single collapsed vtable) -- those SDK interfaces
// are NOT declared in this env's ICryMannequin.h, so the collapsed 7-slot vtable is
// mirrored directly here (do not also derive the SDK types). Slots: [1] sub_18275E524
// writes a 0x38 runtime-params default (1.0f @+16, empty string @+48 -- the
// Serialize/GetRuntimeDefaults); [3] sub_181A7E000; [4] sub_1815ECACC; others unwalked.

namespace wh::dialogmodule {

struct S_DialogueAttachmentProceduralParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_DialogueAttachmentProceduralParams;
    virtual ~S_DialogueAttachmentProceduralParams();   // [0] sub_182763234
    virtual void _vf1();    // [1] sub_18275E524 Serialize/GetRuntimeDefaults [name UNVERIFIED]
    virtual void _vf2();    // [2] [role UNVERIFIED]
    virtual void _vf3();    // [3] sub_181A7E000 [role UNVERIFIED]
    virtual void _vf4();    // [4] sub_1815ECACC [role UNVERIFIED]
    virtual void _vf5();    // [5] [role UNVERIFIED]
    virtual void _vf6();    // [6] [role UNVERIFIED]

    uint8_t  m_b8;          // +0x08  copied by clip OnEnter sub_1828079BC (src+8)
    uint8_t  m_b9;          // +0x09  (src+9)
    uint16_t m_padFlagA;    // +0x0A  zeroed [pad/flag UNVERIFIED]
    uint32_t m_d12;         // +0x0C  (src+12)
};
static_assert(sizeof(S_DialogueAttachmentProceduralParams) == 0x10,
              "S_DialogueAttachmentProceduralParams must be 0x10 (alloc 16)");

}  // namespace wh::dialogmodule
