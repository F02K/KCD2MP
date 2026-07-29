#pragma once
#include <cstdint>
#include <cstddef>
#include "S_PerkDefinitionId.h"

// ===========================================================================
// wh::rpgmodule::C_Perk -- runtime perk instance base (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVC_Perk@rpgmodule@wh@@ (TD 0x184D53EB0)  vtable 0x183A39248, 13 slots.
// sizeof 0x30 (C_ScriptPerk allocs 0x30 with no extra members).
// Typed subclasses: C_BuffPerk / C_MetaPerk / C_ScriptPerk / C_ApplyOnLoadPerk (abstract,
// introduces slot [13] ApplyEffects) / C_SoulAbilityPerk (headers here) and C_CodexPerk /
// C_CompanionPerk / C_CombatTechniquePerk / C_RecipePerk / C_RPGParamOverridePerk /
// C_BuffOverridePerk (RTTI-listed, NOT yet RE'd).
// Type identity runs on 16-byte type-id constants, not dynamic_cast: [10] returns the class's
// static type id and [11]/[12] compare-and-downcast against it (base id unk_1853309C0).
//
// Definition registries (globals): qword_18532E150 = the C_PerkDatabase global instance
// (Resolve [9] looks m_id up in it -- see C_PerkDatabase.h); qword_18532DD88 / count
// @xmmword_18532DD90+8 = sorted 32-byte-row index searched by id ([4]).

namespace wh::rpgmodule {

// Row type of the perk definition table (C_ObjectTableDatabase<S_Perk, S_PerkDBData> per the
// C_PerkDatabase RTTI stack). Opaque -- not RE'd yet.
struct S_Perk;

class C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Perk;
    virtual ~C_Perk();                          // [0]  0x00  scalar-deleting (0x18142DE38)
    // [1]  0x08  m_active = 1; run the definition worker sub_180470930(m_pDef, a2, this); log the
    //      result via the off_18409C520 string-pair table  (0x18047077C)
    virtual int64_t Activate(void* ctx);
    virtual int64_t Deactivate(void* ctx);      // [2]  0x10  same with m_active = 0  (0x180470698)
    virtual bool IsResolved();                  // [3]  0x18  m_pDefinition != nullptr  (0x180470950)
    // [4]  0x20  binary-search the 32-byte-row index (qword_18532DD88) by m_id; copies a
    //      {qword,qword} result  (0x180B46738)  [row/result typing UNVERIFIED]
    virtual void* LookupIndexEntry(void* out16);
    virtual void* _vf5_tlsStatic();             // [5]  0x28  returns a TLS-init static (&qword_1855E5FE8) [role UNVERIFIED] (0x180468784)
    virtual int64_t SaveTo(void* node);         // [6]  0x30  writes save node tag 4990  (0x18151CA70)
    virtual bool LoadFrom(void* node);          // [7]  0x38  expects node tag 4990  (0x1804C89B0)
    virtual int64_t ResolveAlias();             // [8]  0x40  forwards to [9]  (0x1804C88CC)
    virtual void* Resolve();                    // [9]  0x48  m_pDefinition = registry(qword_18532E150)[m_id]  (0x18046B058)
    virtual void* GetTypeId(void* out16);       // [10] 0x50  base returns &unk_1853309C0  (0x181A93960)
    // [11]/[12] 0x58/0x60  type-id downcast: `this` when a2's id matches this class's type id,
    //      else nullptr (both base slots share impl 0x1804715D4; subclasses override with their id)
    virtual C_Perk* AsType(const void* typeId);
    virtual C_Perk* AsType2(const void* typeId);

    const S_Perk* m_pDefinition;   // +0x08  ctor 0; filled by Resolve [9]
    S_PerkDefinitionId m_id;       // +0x10  ctor = invalid (xmmword_18409A0C0)
    void*    m_pOwner;             // +0x20  non-owning owner back-ptr = ctor arg (C_CompanionPerk ctor 0x18096497E: mov [this+20h], a2; creator sub_1809648D0 passes *a1); subclasses subscribe a listener at owner+0x4A0; dtor does not free -- owner class not certified
    uint8_t  m_active;             // +0x28  set/cleared by Activate/Deactivate
    uint8_t  _pad29[7];            // +0x29
};
static_assert(sizeof(C_Perk) == 0x30, "C_Perk must be 0x30 (C_ScriptPerk operator new(48))");
static_assert(offsetof(C_Perk, m_id) == 0x10, "id at 0x10");

}  // namespace wh::rpgmodule
