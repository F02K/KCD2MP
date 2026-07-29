#pragma once
#include "C_InventoryOwnership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ShopInventoryOwnership : C_InventoryOwnership -- shop-inventory
// ownership variant (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (create-site proven:
// tracked alloc 24, factory sub_18135044C).
// -----------------------------------------------
// RTTI TD rva 0x4F48FC8; vtable 0x183B65370, 18 slots. Chosen over the plain variant
// when the inventory's shop-data lookup (sub_180A30B08) hits. Overrides only
// [0] (dtor sub_1809674DC) and [2] (GetEffectiveOwner sub_181601E10); all other
// slots inherited unchanged. Inline ctor: {vptr, inventory* @+0x08, shopData* @+0x10}.

namespace wh::xgenaimodule {

class C_ShopInventoryOwnership : public C_InventoryOwnership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ShopInventoryOwnership;
    ~C_ShopInventoryOwnership() override;                  // [0] sub_1809674DC
    const framework::WUID* GetEffectiveOwner() override;   // [2] sub_181601E10

    void* m_shopData;   // +0x10  ptr to shop-data record from sub_180A30B08 (intrusive-list lookup by 16B WUID key @rec+0xC0/+0xC8); GetEffectiveOwner fwds to (rec+8)->vf0(); class identity [U]
};
static_assert(sizeof(C_ShopInventoryOwnership) == 0x18, "C_ShopInventoryOwnership must be 0x18");

}  // namespace wh::xgenaimodule
