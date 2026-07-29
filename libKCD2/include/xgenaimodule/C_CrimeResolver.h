#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "I_CrimeResolver.h"
#include "I_CrimeDescription.h"
#include "C_CrimeLevelAggregator.h"

// -----------------------------------------------
// wh::xgenaimodule::C_CrimeResolver : I_CrimeResolver -- THE crime-resolution
// singleton: collects crime descriptions, computes the crime level, opens the
// confrontation dialog; embeds the wanted-level aggregator (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x100 (create-site proven: alloc 256 in the lazy-init
// getter sub_180E5CDF0 -> qword_185496110; post-ctor registration
// sub_18341F538()+96 -> sub_180A46280).
// -----------------------------------------------
// RTTI TD rva 0x4F48420; vtable 0x183ABE9B8, 11 slots.  ctor sub_180E5CEB0, tail
// sub_180E5D03C.
// THE single funnel for ALL crime resolution is the NON-virtual
//   Resolve = sub_1832A0B08(resolver, descriptions):
//     if (m_resolving) return 0;  m_resolving = 1;
//     copy descriptions -> m_descriptions (sub_18329EC18);
//     m_crimeLevel = CalculateCrimeLevel sub_1832A1790 (sums item values, theft
//       kind == 28 special-cased; writes m_netCrimeLevel/m_flag58);
//     sub_1832A5C8C(this, 1) -- opens the confrontation dialog_crime_list via the
//       dialog mgr (*(TLS+296)+96), binding sub_1832A62AC / sub_1832A3BF8;
//     return 1;
// SUPPRESSION PATCH (cheat): patch sub_1832A0B08 entry -> xor eax,eax; ret.  The
// BT C_ResolveCrimeDecorator (handler sub_18312BD6C, collector sub_1832A46C0,
// factory sub_1832A4400) then sees "nothing resolved" (node state 6) -- no
// dialog, no apply.  Alt levers: empty the collector sub_1832A46C0, or zero
// CalculateCrimeLevel sub_1832A1790.  The parallel wanted-level accrual runs
// through the embedded aggregator's mailboxes.

namespace wh::xgenaimodule {

class C_CrimeResolver : public I_CrimeResolver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CrimeResolver;
    ~C_CrimeResolver() override;   // [0]  deleting dtor sub_1832A0A48
    void _vf1() override;          // [1]  sub_180416300 [U role]
    void _vf2() override;          // [2]  sub_18093A6D8 [U role]
    bool _vf3() override;          // [3]  sub_18082DBF0 -- IsResolving/IsEmpty-style check [U role]
    void* _vf4() override;         // [4]  sub_181A7FAD0 [U role]
    void _vf5() override;          // [5]  sub_18194D94C [U role]
    void _vf6() override;          // [6]  sub_182081B00 [U role]
    void _vf7() override;          // [7]  sub_1805BA114 [U role]
    void _vf8() override;          // [8]  sub_1832A2360 -- subscribe [U role]
    void _vf9() override;          // [9]  sub_1832A24BC [U role]
    void _vf10() override;         // [10] sub_1832A26B8 [U role]

    void*    m_inboxDescr;         // +0x08  DynArray<T> head (WHDynStorage) of subscription entries; empty -> &unk_18566BEE0 sentinel; elem 0x10 = {u64,u64}. T [U]
    int16_t  m_inboxIdx10;         // +0x10  ctor: -1 (inbox handle index pair)
    int16_t  m_inboxIdx12;         // +0x12  ctor: -1
    uint8_t  _pad14[4];            // +0x14
    uint8_t  m_resolving;          // +0x18  Resolve() guard (sub_1832A0B08 early-outs if set) [V; width U]
    uint8_t  _pad19[7];            // +0x19
    uint64_t m_unk20;              // +0x20  ctor: 0 [U role]
    std::vector<std::shared_ptr<I_CrimeDescription>> m_descriptions;  // +0x28  active crime descriptions (copied in by Resolve via sub_18329EC18) [V]
    uint64_t m_unk40;              // +0x40  ctor tail: 0 [U role]
    uint64_t m_unk48;              // +0x48  ctor tail: 0 [U role]
    int32_t  m_crimeLevel;         // +0x50  = CalculateCrimeLevel result (written by Resolve) [V; width U]
    int32_t  m_netCrimeLevel;      // +0x54  net level (written by CalculateCrimeLevel) [V role]
    uint8_t  m_flag58;             // +0x58  byte flag (written by CalculateCrimeLevel) [U role]
    uint8_t  _pad59[3];            // +0x59
    int32_t  m_dword5C;            // +0x5C  ctor tail: 0 [U role]
    C_CrimeLevelAggregator m_levelAggregator;  // +0x60 (0xA0) embedded -- mailbox-driven wanted-level accrual [V]
};
static_assert(sizeof(C_CrimeResolver) == 0x100, "C_CrimeResolver must be 0x100 (256)");

}  // namespace wh::xgenaimodule
