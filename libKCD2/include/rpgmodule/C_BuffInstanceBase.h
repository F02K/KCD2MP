#pragma once
#include <cstdint>
#include <vector>
#include "../framework/WUID.h"
#include "../framework/InplaceVector.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffInstanceBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88.
// -----------------------------------------------
// RTTI .?AVC_BuffInstanceBase@rpgmodule@wh@@ (TD 0x184CE4E98)  vtable 0x183A38BA0 (16 slots)
// ctor sub_18046C548  dtor sub_18063A690.  Root of the buff-instance hierarchy:
//   C_BuffInstanceBase <- C_BuffInstance<C_Soul> <- C_SoulBuffInstance <- C_TimedSoulBuffInstance
//   (all mdisp 0, pure single inheritance, one vtable -- RTTI CHDs verified).
//
// [FUNDAMENTAL vs KCD1] There is NO C_BuffFactory in KCD2 (zero RTTI/string hits). Instantiation
// goes through a GLOBAL RTTR reflection factory: sub_18063AE40 builds "wh.rpgmodule.buff." +
// <typename from the buff definition>, sub_18063B004 FNV-hashes it and resolves a registered
// creator. Virtual slot names below are KCD1 PRIORS (KCD2 slot order verified only for
// Init [0] / Update [1] via the C_TimedSoulBuffInstance vtable; the rest carried over, flagged).
//
// The KCD1 header read +0x50..+0x88 as an intrusive list node; the KCD2 deep map (verify-
// confirmed) resolves it as an SSO small-vector of uint32 with inline capacity 2 -- byte-exact
// match for wh::shared::InplaceVector<uint32_t, 2> (arena @+0x50, inUse @+0x58, self @+0x60,
// allocator @+0x68, {first,last,end} @+0x70..+0x88; init helper sub_18046C5CC(this+0x68, 2)).

namespace wh::rpgmodule {

class I_BuffInitParams;   // KCD1 carry-over declaration (default impl C_BuffInitParamsDefault)

// 16-byte element of m_records; elem[0] is an OWNED, memory-accounted heap ptr (per-element
// teardown sub_18063A740). Element identity UNVERIFIED -- vector<16B> vs flat_map pair is
// genuinely undetermined (no lookup site reachable from ctor/dtor). SYNTHETIC name.
struct S_BuffRecord16 {
    void*    m_pOwned;   // +0x00  owned heap ptr (freed with memory accounting)
    uint64_t m_unk08;    // +0x08  (unresolved)
};
static_assert(sizeof(S_BuffRecord16) == 0x10, "S_BuffRecord16 must be 0x10");

class C_BuffInstanceBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BuffInstanceBase;
    // 16 virtual slots (vtable 0x183A38BA0). Slot NAMES/semantics are KCD1 priors except where noted.
    virtual bool Init(void* a_pBuffData, I_BuffInitParams* a_pInitParams) = 0;  // [0]  KCD2 leaf impl 0x18063A8F0
    virtual bool Update() = 0;                                                  // [1]  KCD2 leaf impl 0x1811908DC
    virtual void Terminate() = 0;                                               // [2]  KCD2 soul impl 0x18063AA3C
    virtual bool CommitModifiers() = 0;                                         // [3]  KCD2 soul impl 0x18063B7C8
    virtual void CopyFrom(C_BuffInstanceBase* a_pSrc, void* a_pArchive) {}      // [4]  [KCD1 prior]
    virtual void unk_05() {}                                                    // [5]  [KCD1 prior]
    virtual float GetStatus(void* a_pOutResult, int a_a3);                      // [6]  [KCD1 prior]
    virtual void Save(void* a_pArchive);                                        // [7]  [KCD1 prior]
    virtual void Load(void* a_pArchive);                                        // [8]  [KCD1 prior]
    virtual void AddNewModifier(void* a_pModifierDef) = 0;                      // [9]  [KCD1 prior]
    virtual ~C_BuffInstanceBase();                                              // [10] KCD2 dtor sub_18063A690
    virtual void NotifyTagChanged(uint32_t a_nTagId, bool a_bAdded) {}          // [11] [KCD1 prior]
    virtual bool IsActive() { return true; }                                    // [12] [KCD1 prior]
    virtual void* GetTypeName(void* a_pOutStr);                                 // [13] [KCD1 prior] "wh.rpgmodule.buff.<Type>" string typing
    virtual C_BuffInstanceBase* CastTo(const void* a_pTypeName);                // [14] [KCD1 prior]
    virtual C_BuffInstanceBase* CastToConst(const void* a_pTypeName);           // [15] [KCD1 prior]

    wh::framework::WUID m_wuid;        // +0x08  instance key; dtor FNV-1a-hashes it (sub_18046C8B4) to
                                       //        erase from the global registry; ctor inits from the
                                       //        global default/sentinel qword_185326B58
    void*    m_pBuffData;              // +0x10  buff definition (C_BuffData* by KCD1 analogy -- concrete
                                       //        class not RE'd; leaf Init sub_18063A8F0 stores it, reads float
                                       //        duration @def+0x30; ctor default = &qword_18532CCA0. RTTR
                                       //        factory sub_18063AE40 builds "wh.rpgmodule.buff.<Type>" from it
    void*    m_unk18;                  // +0x18  alternate/override buff definition (same type as m_pBuffData;
                                       //        == m_pBuffData semantics when null). Clone slot4 sub_1805181C8
                                       //        uses (this+0x18 ? this+0x18 : this+0x10) as factory type source;
                                       //        factory sub_18063ACE4 copies src+0x18 into new+0x18 when distinct
    void*    m_unk20;                  // +0x20  owner/source object* (vtable-bearing) resolved by WUID lookup;
                                       //        factory sub_18063ACE4 sets it = sub_1804686E8(ctx+0x758, initWUID)
                                       //        when the init WUID != default (xmmword_183A43B48)
    uint64_t m_unk28;                  // +0x28  head of the sub-struct passed to teardown sub_18063A740
    uint64_t m_unk30;                  // +0x30  (ctor 0; unresolved)
    std::vector<S_BuffRecord16> m_records;             // +0x38  16B elems; dtor frees each elem[0] then the buffer
    wh::shared::InplaceVector<uint32_t, 2> m_inlineIds; // +0x50  SSO u32 small-vector, inline cap 2
                                                        //        (element semantics UNVERIFIED -- tag ids?)
};
static_assert(sizeof(C_BuffInstanceBase) == 0x88, "C_BuffInstanceBase must be 0x88");

}  // namespace wh::rpgmodule
