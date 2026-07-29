#pragma once
#include <cstdint>
#include <vector>
#include "I_ItemRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_EquippableItemRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xA8.
// -----------------------------------------------
// RTTI .?AVC_EquippableItemRuntimeData@entitymodule@wh@@ (TD 0x184C38D30, COL 0x184322EA0)
// vtable 0x183EC1870  ctor sub_18046FB14.  sizeof from last member (float @+0xA4 -> 0xA8); both
// Armor and WeaponEquip start their own members at +0xA8.
//
// Holds the item QUALITY (+0xA0, default = owner->vf6()) and CONDITION fraction (+0xA4, clamp
// [0,1], default 1.0 -- the classic KCD durability signature). The initial-health path
// sub_180469348(owner, clamp[0,1]) also writes the authoritative condition to C_Item+0x64.
// RTTR SetItemHealth 0x1808D61B8 writes +0xA4 (equippable branch); the Condition getter
// 0x18096F7D4 reads it via 0x180465C6C. The extended vtable (0x183EC1870) adds a WASH virtual
// @+0xC8 past the 24-slot I_ItemRuntimeData base -- C_Item::WashItem (0x182A6B758) routes there
// for Equippable-type items (per-item dirt state lives in the subclasses; fields unmapped).

namespace wh::entitymodule {

class C_EquippableItemRuntimeData : public I_ItemRuntimeData   // +0x00  (0x58)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EquippableItemRuntimeData;
    void*    m_sentinel58;       // +0x58  single-qword node the second list points at (UNRESOLVED)
    bool     m_flag60;           // +0x60  (=0)
    uint8_t  _pad61[7];          // +0x61
    void*    m_pList2Next;       // +0x68  init = &m_sentinel58 (empty; init sub_18046FDE0)
    void*    m_pList2Prev;       // +0x70  init = &m_sentinel58
    std::vector<void*> m_vec78;  // +0x78  element cleanup sub_181036550 (element type unresolved)
    void*    m_pModifierHolder;  // +0x90  = sub_180452BA4(): heap 0x30-byte node {self@0,self@8,self@0x10,u16 0x0101@0x18},
                                 //        NO vtable (1st qword = self) -- intrusive list/registry head, NOT polymorphic
    void*    m_p98;              // +0x98  (=0; role unresolved)
    int32_t  m_quality;          // +0xA0  default = owner->vf6() (item quality)  VERIFIED source
    float    m_condition;        // +0xA4  clamp [0,1], default 1.0 (durability fraction)  VERIFIED
};
static_assert(sizeof(C_EquippableItemRuntimeData) == 0xA8, "C_EquippableItemRuntimeData must be 0xA8");
static_assert(offsetof(C_EquippableItemRuntimeData, m_quality) == 0xA0, "quality at 0xA0");
static_assert(offsetof(C_EquippableItemRuntimeData, m_condition) == 0xA4, "condition at 0xA4");

}  // namespace wh::entitymodule
