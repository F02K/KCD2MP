#pragma once
#include <cstdint>
#include <vector>
#include "C_ItemHolder.h"
#include "I_InventoryListener.h"
#include "../framework/WUID.h"
#include "../framework/C_Listeners.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::entitymodule::C_InventoryBase : C_ItemHolder -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xE8.
// -----------------------------------------------
// Primary base of C_Inventory (shares the vtable @+0x00 via C_ItemHolder). Holds the item lists and the
// inventory listener registry. base-ctor sub_1803A4BF0.
// m_items element type CORRECTED C_ItemHolder* -> C_Item*: the class-guid finder 0x1808D315C reads
// each element's class record via sub_1804695B4 = *(elem+0x48) -- C_Item::m_pClassData (VERIFIED
// member; C_ItemHolder is vptr-only) -- and the alchemy gather consumes elem->m_wuid (+0x30).

namespace wh::entitymodule {

class C_Item;

class C_InventoryBase : public C_ItemHolder {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InventoryBase;

    // First item whose CLASS guid (S_ItemClass +0x08) equals classId; null if none (0x1808D315C,
    // an alchemy-TU helper walking m_items).  EXACT match only -- the autocook gather callback
    // (0x181FFE040) adds a one-hop substitute retry via the item-class registry
    // (sub_180468340(qword_185325820) -> class vf+0x138 -> +0x100 guid) that is NOT wired yet.
    C_Item* FindItemByClass(const CryGUID& classId);
    // The sanctioned in-inventory amount change (Lua DeleteItem's partial-removal path):
    // pre-check 0x18179D064, pre-notify 0x18179CFBC, C_Item::SetAmount(amount+delta), post-notify
    // 0x18179D124, then item broadcast 0x1804664E0. Returns false if the pre-check vetoes.
    // 0x18179CF50
    bool ChangeItemAmount(C_Item* item, int32_t delta);
    // Full-removal core (Lua DeleteItem with count >= amount): fires the listener trio on this
    // inventory AND the item's +0x98 holder (reason codes 1/4 holder-held vs 5 plain), delists,
    // and reports class+wuid to the post-remove listeners.  reason: 2 = script delete (observed);
    // count values below the stack size reroute to ChangeItemAmount.  0x180479758
    void RemoveItem(C_Item* item, uint32_t reason, uint32_t count);
    // The MoveItem core, called on the DESTINATION inventory (Lua MoveItemOfClass's per-item op):
    // count 0 = whole stack; splits the stack (0x1808F0DA4) on partial moves, merges into an
    // existing same-class stack when found (finder 0x1804CCD4C, merge 0x1808F3300), assigns a
    // fresh instance guid on un-merged full moves (SetInstanceGuid 0x180467A6C), then re-parents
    // (item+0x90 = this, +0x98 = 0) and runs both inventories' listener trios. Returns the
    // surviving item (moved or merge target), null when the acceptance check (this vf+0x68)
    // vetoes.  unkOwnerFlag [U role]: gates the merge search (0x1808D56D0); the keep-owner move
    // path passes 1, the change-owner path 0.  0x1808D534C
    C_Item* MoveItemIn(C_Item* item, uint32_t count, bool unkOwnerFlag);

    std::vector<C_Item*>       m_items;        // +0x08  held items  VERIFIED layout
    std::vector<C_ItemHolder*> m_items2;       // +0x20  secondary list (role/elem type UNVERIFIED)
    // Keyed listener registry (0xB0): delegate vector @+0x40, primary/secondary WUID guard stacks.
    wh::shared::C_DependentListeners<I_InventoryListener, wh::framework::WUID, 4> m_listeners;  // +0x38
};
static_assert(sizeof(C_InventoryBase) == 0xE8, "C_InventoryBase must be 0xE8");

}  // namespace wh::entitymodule
