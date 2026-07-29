#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::E_UIApseView -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper. The Apse view selector: C_UIApse holds two
// C_ModelProperty<E_UIApseView> members (@+0x28/+0x50, value byte @prop+0x08) and
// C_ApseViewTrigger reads it via a C_TypedPortRef<E_UIApseView::Type> (cached as a byte
// @trigger+0x170).  Enumerators certified from C_UIApse's E_UIApseView ToString
// (sub_180C43980), verified kd7u.

namespace wh::framework {

struct E_UIApseView {
    enum Type : uint8_t {
        None = 0, Crafting, Inventory, InventoryInfo, Player, PlayerInfo,
        Map , Codex, MapLegend, QuestLog, QuestDiary,
    };
};

}  // namespace wh::framework
