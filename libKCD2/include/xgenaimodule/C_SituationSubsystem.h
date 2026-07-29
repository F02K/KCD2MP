#pragma once
#include <cstdint>
#include "I_SchedulerInterruptListener.h"
#include "I_SituationHolder.h"
#include "I_InjectionExternalDataProvider.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SituationSubsystem : scheduler::I_SchedulerInterruptListener
// (@+0x0) + I_SituationHolder (@+0x8) + I_InjectionExternalDataProvider
// (@+0x10) -- per-NPC situation subsystem, stored as a pointer at C_NPC+0x1080
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x88 (create-site proven: C_NPC ctor
// sub_180BDC5F8 @0x180bdc984: sub_1804F75C0(136) -> ctor sub_180BDBFDC(this,
// a2=NPC), guarded by the NPC "bCanHoldInformation" query).  dtor sub_1813CB4BC.
// -----------------------------------------------
// RTTI TD rva 0x4F4E178; COLs 0x40DFAC8 / 0x40DFAF0 (@+8) / 0x40DFB18 (@+16).
// Subobject vtables: primary rva 0x3A83180 (4 slots: [0][1]sub_1819EC168
// ICF-folded, [2]sub_180BD6B74 [3]sub_180BD6B30); holder rva 0x3A831A8 (9
// slots: [0]sub_180638480 (SHARED with C_Situation slot [3]) [1]sub_181AA5AB0
// [2]sub_1823C9770 [3]sub_180E9AD64 [4]sub_180BD64A4 [5]sub_180BD5A8C
// [6]sub_180E9AD90 [7]sub_180760EF4 [8]sub_180BD7938); provider rva 0x3A83160
// (3 slots: [0]sub_180BD93D8 [1]sub_180838AE0 [2]sub_1813CC900).

namespace wh::xgenaimodule {

class C_SituationSubsystem
    : public scheduler::I_SchedulerInterruptListener,
      public I_SituationHolder,
      public I_InjectionExternalDataProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SituationSubsystem;
    // I_SchedulerInterruptListener impls [U roles]
    void SilVf0() override; void SilVf1() override; void SilVf2() override; void SilVf3() override;
    // I_SituationHolder impls [U roles]
    void ShVf0() override; void ShVf1() override; void ShVf2() override;
    void ShVf3() override; void ShVf4() override; void ShVf5() override;
    void ShVf6() override; void ShVf7() override; void ShVf8() override;
    // I_InjectionExternalDataProvider impls [U roles]
    void IedpVf0() override; void IedpVf1() override; void IedpVf2() override;

    C_NPC*   m_npc;          // +0x18  owning NPC (ctor a2; create-site C_NPC ctor sub_180BDC5F8 @0x180bdc984)
    I_Situation* m_situation;    // +0x20  situation this holder is a member of; set with m_28 by ShVf3 setter sub_180E9AD64(this, situation, memberId)
    int32_t  m_28;           // +0x28  ctor: -1 [U role]
    uint8_t  _pad2C[4];      // +0x2C
    std::weak_ptr<C_Situation> m_weakSituation;  // +0x30  {T*@+0x30, _Ref_count_base*@+0x38}; lock=sub_1806D6B2C, dtor _Decwref=sub_1803FB78C
    boost::container::vector<void*> m_registrations;  // +0x40  {data, count@+0x48, cap@+0x50}; 8-byte ptr elems, each self-unregistered via sub_1809C5194 in ShVf7 sub_180760EF4
    boost::container::vector<std::pair<void*, int64_t>> m_deadlines;  // +0x58  {data, count@+0x60, cap@+0x68}; sorted flat map<request*, deadline> (insert sub_180BD6898, query sub_180BD7680)
    uint64_t m_sentinel70;   // +0x70  0x8000000000000000 ("no value")
    uint64_t m_sentinel78;   // +0x78  0x8000000000000000
    int32_t  m_80;           // +0x80  ctor: 0x1000000 [U meaning]
    uint8_t  _pad84[4];      // +0x84
};
static_assert(sizeof(C_SituationSubsystem) == 0x88, "C_SituationSubsystem must be 0x88 (alloc 136 at C_NPC ctor)");

}  // namespace wh::xgenaimodule
