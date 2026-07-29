#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <map>
#include <set>
#include "C_InventoryBase.h"
#include "../framework/C_Listeners.h"
#include "../framework/HashPrimitives.h"

// -----------------------------------------------
// wh::entitymodule::C_WorldInventory : C_InventoryBase -- KCD2 1.5.6 (kd7u).  sizeof 0x1A8.
// -----------------------------------------------
// The single global "world" inventory (embedded by value in C_InventoryManager at +0x100070).
// ctor sub_180BE8594, dtor sub_182A1CD08. RTTI bases: C_InventoryBase (-> C_ItemHolder). Adds, past
// the 0xE8 base, a set of tracked items plus a world-inventory-listener registry.

namespace wh::entitymodule {

class C_Item;
class I_WorldInventoryListener;

class C_WorldInventory : public C_InventoryBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WorldInventory;
    // 4-byte-element vector; element type only size-proven (EntityId / enum / uint32) -- tentative.
    std::vector<uint32_t>                                                  m_vecE8;           // +0x0E8
    std::unordered_set<C_Item*, wh::shared::S_DefaultHash<C_Item*>>        m_registeredItems; // +0x100  (FNV-1a of 8B ptr)
    std::vector<C_Item*>                                                   m_vec140;          // +0x140  (8-byte elem; tentative)
    std::map<C_Item*, uint64_t>                                            m_itemMeta;        // +0x158  (mapped value tentative)
    std::set<C_Item*>                                                      m_itemSet;         // +0x168
    wh::shared::C_Listeners<I_WorldInventoryListener, 4>                   m_listeners;       // +0x178  (0x30)
};
static_assert(sizeof(C_WorldInventory) == 0x1A8, "C_WorldInventory must be 0x1A8");

}  // namespace wh::entitymodule
