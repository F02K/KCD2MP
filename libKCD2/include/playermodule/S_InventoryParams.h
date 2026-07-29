#pragma once

// -----------------------------------------------
// wh::playermodule::S_InventoryParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// Inventory-picker action-param block (RTTI template arg US_InventoryParams@playermodule; POD).
// Lives at action+0x50: a single item-FILTER pointer (default = the global filter
// qword_185332870).  The picker RESULT fields (+0x80/+0x88 UI result, +0x90 indexer, +0x9A
// category byte) belong to the C_OpenInventoryAction LEAF, not this param block (m_pAlchemy sits
// between them at +0x58) -- corrected vs the first-wave grouping.

namespace wh::playermodule {

struct S_InventoryParams {
    void* m_pItemFilter;   // +0x00  item-filter object (default qword_185332870)
};
static_assert(sizeof(S_InventoryParams) == 8, "S_InventoryParams is one filter ptr");

}  // namespace wh::playermodule
