#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::entitymodule::I_ItemRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58, ABSTRACT.
// -----------------------------------------------
// RTTI .?AVI_ItemRuntimeData@entitymodule@wh@@ (TD 0x184C38C80, COL 0x184141138)
// vtable 0x183A38F18 (24 slots; slot0/20/23 = _purecall => abstract; slot22 @vtable+0xB0 =
// sub_182A43FC0 = ref-counted scalar-deleting release, called vf(obj,1) to delete throughout)
// ctor sub_18046FBF0  dtor sub_180F19238.  Base sizeof from derived deltas: Consumable(0x68) -
// own 0x10 = 0x58; Key(0x60) - own 8 = 0x58.
//
// Per-item runtime state, lazily created and stored at C_Item+0x58 (m_pRuntimeData); back-ptr to
// the owning item @+0x48 (ctor a2; resolver sub_1804695B4 = "owner or global default"). Ctors read
// the item's static class definition via owner virtuals (idx6/idx24/idx26) to compute defaults.
//
// [FUNDAMENTAL vs KCD1] KCD1's concrete middle class C_ItemRuntimeData is GONE in KCD2 (bin_search
// for its RTTI string is empty) -- subclasses derive straight from this abstract base.
//
// Hierarchy: I_ItemRuntimeData <- {C_ConsumableRuntimeData, C_KeyRuntimeData,
// C_EquippableItemRuntimeData <- {C_ArmorRuntimeData, C_WeaponEquipRuntimeData <-
// {C_AmmoRuntimeData, C_WeaponRuntimeData <- {C_MeleeWeaponRuntimeData, C_MissileWeaponRuntimeData}}}}
// (all single-inheritance; every CHD BCD mdisp=0 -- no virtual bases, no this-adjust).

namespace wh::entitymodule {

class C_Item;

class I_ItemRuntimeData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ItemRuntimeData;
    virtual ~I_ItemRuntimeData() = 0;   // vtable slot0 = _purecall in the binary (abstract);
                                        // real deletion goes through slot22 (ref-release, vf(obj,1))

    uint8_t m_sboInline[0x10];  // +0x08  SBO inline storage: 1 element (stride 0x10) of the small-vector @+0x28.
                                //        NOT a list sentinel. In-use flag is m_flag18 @+0x18 (=inline_ptr+0x10).
                                //        Element type UNRESOLVED (no external insert site found).
    bool    m_flag18;           // +0x18  (=0)
    uint8_t _pad19[7];          // +0x19
    void*   m_pListNext;        // +0x20  init = &m_sentinel08 (empty intrusive list)
    void*   m_pSboInlinePtr;    // +0x28  = &m_sboInline; base of the SBO small-vector control block
                                //        {inline_ptr@+0x28, begin@+0x30, end@+0x38, cap_end@+0x40}, element stride 0x10
    std::vector<void*> m_vec30; // +0x30  REAL element stride 0x10 (element type unresolved; freed
                                //        via sub_180FF9998 in dtor)
    C_Item* m_pOwner;           // +0x48  owning item back-ptr (ctor a2)  VERIFIED
    void*   m_pProvider50;      // +0x50  settable provider ptr: virtual setter slot8=sub_180EEF62C (stores arg or
                                //        default &unk_1855A5060 when null), clearer slot10=sub_181A88CB0 (=0). Pointee type UNRESOLVED
};
static_assert(sizeof(I_ItemRuntimeData) == 0x58, "I_ItemRuntimeData must be 0x58");
static_assert(offsetof(I_ItemRuntimeData, m_pOwner) == 0x48, "owner back-ptr at 0x48");

}  // namespace wh::entitymodule
