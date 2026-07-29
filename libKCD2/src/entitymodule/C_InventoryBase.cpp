#include "entitymodule/C_InventoryBase.h"
#include "Offsets/Offsets.h"

// C_InventoryBase engine-function forwarders (KCD2 WHGame.dll 1.5.6 RVAs, utem;
// ids = kcd2 address library).

namespace wh::entitymodule {

C_Item* C_InventoryBase::FindItemByClass(const CryGUID& classId)
{
    // 0x1808D315C: linear walk of m_items comparing each item's class guid
    // (*(item+0x48) = S_ItemClass, guid at +0x08).
    using Fn = C_Item* (__fastcall*)(C_InventoryBase*, const CryGUID*);
    static REL::Relocation<Fn> fn{ REL::ID(48187) };  // 0x1808D315C
    return fn(this, &classId);
}

bool C_InventoryBase::ChangeItemAmount(C_Item* item, int32_t delta)
{
    using Fn = bool (__fastcall*)(C_InventoryBase*, C_Item*, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(136741) };  // 0x18179CF50
    return fn(this, item, delta);
}

void C_InventoryBase::RemoveItem(C_Item* item, uint32_t reason, uint32_t count)
{
    using Fn = void (__fastcall*)(C_InventoryBase*, C_Item*, uint32_t, uint32_t);
    static REL::Relocation<Fn> fn{ REL::ID(27292) };  // 0x180479758
    fn(this, item, reason, count);
}

C_Item* C_InventoryBase::MoveItemIn(C_Item* item, uint32_t count, bool unkOwnerFlag)
{
    using Fn = C_Item* (__fastcall*)(C_InventoryBase*, C_Item*, uint32_t, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(48244) };  // 0x1808D534C
    return fn(this, item, count, unkOwnerFlag);
}

}  // namespace wh::entitymodule
