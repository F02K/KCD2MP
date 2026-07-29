#pragma once
#include <cstdint>
#include "C_SUBBBase.h"
#include "I_BehaviorHost.h"
#include "I_ResourceHost.h"
#include "I_SchedulerActivityHost.h"
#include "I_SmartEntitySubbrainListener.h"
#include "I_InjectionExternalDataUpdateListener.h"
#include "I_BehaviorToMonsterLODPostponer.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SchedulerSubbrain : C_SUBBBase + 6 host/listener bases --
// the scheduler-driven subbrain (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x210
// (create-site proven: factory sub_18041A4DC operator new(528) -> ctor
// sub_180413D00).  Base subobjects: I_BehaviorHost @+0xC8, I_ResourceHost
// @+0xD0, scheduler::I_SchedulerActivityHost @+0xD8, I_SmartEntitySubbrainListener
// @+0xE0, I_InjectionExternalDataUpdateListener @+0xE8,
// I_BehaviorToMonsterLODPostponer @+0xF0 (0x10 subobject: vptr + 2 int16 = -1).
// -----------------------------------------------
// RTTI TD rva 0x4EFF530.  Primary vtable rva 0x3FD8390 (37 slots, COL 0x451F230);
// subobject vtables: 0x3FD8500 (9), 0x3FD8550 (4), 0x3FD84C0 (3), 0x3FD84E0 (3),
// 0x3FD8578 (1), and .data unk_18566BC68 (LODPostponer, runtime-built).
// ctor: 6 vptrs; sub_180413E6C(+0xF0); sub_180414100(+0x108) 48-byte zero; -1
// fills @+0x138/+0x148/+0x14C; sub_180414204(+0x160) ~0x90 embedded struct;
// +0x1F8 = 0x4000000; +0x200/+0x208 ptrs; registers a "__resource" variable via
// the base m_varIndex.  Primary overrides: [1]dtor, [17], [18],
// [22]Step=sub_180760D00, [23]=sub_18041A6A0 (return 1), [27]=sub_181A74270,
// [28], [29]Serialize, [30], [32][33][34] (live here; base stubs), [35], [36].

namespace wh::xgenaimodule {

class C_SchedulerSubbrain
    : public C_SUBBBase,
      public I_BehaviorHost,
      public I_ResourceHost,
      public scheduler::I_SchedulerActivityHost,
      public I_SmartEntitySubbrainListener,
      public I_InjectionExternalDataUpdateListener,
      public I_BehaviorToMonsterLODPostponer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SchedulerSubbrain;
    void  Step() override;                  // [22] sub_180760D00
    bool  IsRunning() override;             // [23] sub_18041A6A0 (return 1)
    void* GetSubbrainType() override;       // [27] sub_181A74270
    void  Serialize(void* stream) override; // [29]
    void  _vf32() override;                 // [32] live here
    void  _vf33() override;                 // [33] live here
    void  _vf34() override;                 // [34] live here
    void  RequestOwnerTick() override;      // [35]
    void  CancelOwnerTick() override;       // [36]
    // interface impls [U roles]
    void  BhVf1() override; void BhVf2() override; void BhVf3() override; void BhVf4() override;
    void  BhVf5() override; void BhVf6() override; void BhVf7() override; void BhVf8() override;
    void  RhVf1() override; void RhVf2() override; void RhVf3() override;
    void  SahVf1() override; void SahVf2() override;
    void  SeslVf0() override; void SeslVf1() override; void SeslVf2() override;
    void  OnExternalDataUpdate() override;

    int32_t  m_schedulerPhase;    // +0x100  ctor=2; sub_18205B420 writes 0 then 2 via &field (scheduler step phase)
    uint8_t  _pad104[4];          // +0x104
    uint8_t  m_embedded108[0x30]; // +0x108..+0x138  48-byte zeroed block (sub_180414100) [U interior]
    int32_t  m_138;               // +0x138  ctor: -1 [U role]
    uint8_t  _pad13C[4];          // +0x13C  alignment pad (ctor leaves uninit; 8-byte align for m_140)
    void*    m_140;               // +0x140  ptr -> active scheduled-activity context; (*m_140)[+8] used by scheduler [U pointee]
    int32_t  m_148;               // +0x148  ctor: -1 [U role]
    int32_t  m_14C;               // +0x14C  ctor: -1 [U role]
    void*    m_150;               // +0x150  ptr -> scheduler activity source; null-checked; pointee[+0]=ptr, pointee+0x1D byte read [U pointee]
    uint8_t  m_158;               // +0x158  byte accumulator (_vf33 max-of pointee+0x1D, then clears)
    uint8_t  _pad159[7];          // +0x159
    uint8_t  m_embedded160[0x90]; // +0x160..+0x1F0  embedded struct (sub_180414204) [U interior]
    int32_t  m_1F0;               // +0x1F0  tail dword of the +0x160 embedded struct (sub_180414204 a1+144 = dword_184932288 = 0)
    uint8_t  _pad1F4[4];          // +0x1F4
    uint32_t m_1F8;               // +0x1F8  ctor: 0x4000000 [U meaning]
    uint8_t  _pad1FC[4];          // +0x1FC
    // +0x200  Concurrency::details::TaskStack (16B embedded; dtor ??1TaskStack@details@Concurrency@@QEAA@XZ @0x1823C8A8C)
    void*    m_taskStackHead;     // +0x200  head; deref -> vtable'd task node (flag byte @pointee+0x38)
    void*    m_taskStackRef;      // +0x208  refcounted ptr; ~TaskStack releases via sub_1804F6588 when non-null
};
static_assert(sizeof(C_SchedulerSubbrain) == 0x210, "C_SchedulerSubbrain must be 0x210 (operator new(528))");

}  // namespace wh::xgenaimodule
