#pragma once
#include <cstdint>
#include "C_SUBBBase.h"
#include "I_BehaviorHost.h"
#include "I_SituationHolder.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SituationSubBrain : C_SUBBBase + I_BehaviorHost (@+0xC8) +
// I_SituationHolder (@+0xD0) -- situation-driven subbrain (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x1E8 (create-site proven: factory sub_18041A4DC
// operator new(488) -> ctor sub_18320B1B0).
// -----------------------------------------------
// RTTI TD rva 0x4EFF4F0.  Primary vtable rva 0x3FD8070 (37 slots, COL 0x451F1B8);
// I_BehaviorHost subobject vtable rva 0x3FD81A0 (9 slots, COL 0x451F168);
// I_SituationHolder subobject vtable rva 0x3FD8020 (9 slots, COL 0x451F190).
// ctor: 3 vptrs; +0x120 = -1; zero to +0x150; sub_18320B05C(+0x158); sentinels
// 0x8000000000000000 @+0x1C8/+0x1D0; +0x1E0 = 2; SetState(8).  Primary
// overrides: [1]dtor [5]Init [17] [18] [22]Step=sub_18320D56C [23]sub_18041A688
// [24]Configure [27]type=sub_181A72470 [28] [35] [36].  Interior field typing
// past the ctor writes UNVERIFIED [U] -- modeled as sized blobs.

namespace wh::xgenaimodule {

class C_SituationSubBrain
    : public C_SUBBBase,
      public I_BehaviorHost,
      public I_SituationHolder {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SituationSubBrain;
    void  Step() override;                  // [22] sub_18320D56C
    bool  IsRunning() override;             // [23] sub_18041A688
    void* GetSubbrainType() override;       // [27] sub_181A72470
    void  RequestOwnerTick() override;      // [35]
    void  CancelOwnerTick() override;       // [36]
    // I_BehaviorHost impls ([0][4][5] share bodies with Scheduler's) [U roles]
    void  BhVf1() override; void BhVf2() override; void BhVf3() override; void BhVf4() override;
    void  BhVf5() override; void BhVf6() override; void BhVf7() override; void BhVf8() override;
    // I_SituationHolder impls [U roles]
    void  ShVf0() override; void ShVf1() override; void ShVf2() override; void ShVf3() override;
    void  ShVf4() override; void ShVf5() override; void ShVf6() override; void ShVf7() override;
    void  ShVf8() override;

    void*                m_D8;            // +0xD8   polymorphic obj ptr (null-init); Init->sub_1805B6348(p,this); Step: sub_182051080(p), p->vtbl[1], p+0x38/0x56 [inferred]
    void*                m_controllerE0;  // +0xE0   polymorphic controller ptr (null-init); sub_18320C6E8 calls p->vtbl[3/12/13], stores/frees handle m_handle100 [inferred]
    std::vector<void*>   m_vecE8;         // +0xE8   std::vector (ctor zeroes 3 ptrs; dtor sub_1804134B4 storage-free, trivial elems) [elem U]
    int32_t              m_handle100;     // +0x100  registration id into m_controllerE0 (passed to its vtbl[12]/[13]; reset to -1 in sub_18320C6E8) [inferred]
    uint8_t              _pad104[4];      // +0x104  align
    std::vector<void*>   m_vec108;        // +0x108  std::vector (ctor zeroes 3 ptrs; dtor sub_1804134B4, elems destructed via sub_1832BB3A0; Step reverse-iterates) [elem U]
    int32_t  m_120;               // +0x120  ctor: -1 [U role]
    uint8_t              _pad124[4];      // +0x124  align (after int32 m_120; ctor does not write this)
    std::vector<void*>   m_vec128;        // +0x128  std::vector (dtor sub_180BDA9DC releases each non-null elem via sub_18041AC7C, then frees storage) [elem U]
    std::vector<void*>   m_vec140;        // +0x140  std::vector (dtor sub_180BDA9DC frees storage, trivial elems) [elem U]
    uint8_t  m_embedded158[0x70]; // +0x158..+0x1C8  embedded struct (sub_18320B05C) [U interior]
    uint64_t m_sentinel1C8;       // +0x1C8  0x8000000000000000 ("no value")
    uint64_t m_sentinel1D0;       // +0x1D0  0x8000000000000000
    uint8_t  _unk1D8[8];          // +0x1D8  [U]
    uint32_t m_1E0;               // +0x1E0  ctor: 2 [U role]
    uint8_t  _pad1E4[4];          // +0x1E4
};
static_assert(sizeof(C_SituationSubBrain) == 0x1E8, "C_SituationSubBrain must be 0x1E8 (operator new(488))");

}  // namespace wh::xgenaimodule
