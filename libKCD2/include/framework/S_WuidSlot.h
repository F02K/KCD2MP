#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::S_WuidSlot<T> -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Generation-counted slot used by the WUID slot-table registries. The slot is live when
// generation == (uint16)(WUID.slot-generation) AND object != null AND the object's own stored
// WUID matches. Used by wh::entitymodule::C_InventoryManager (S_WuidSlot<C_Inventory> m_slots[0x10000]).
// C_ItemManager uses a wider 0x18-byte variant (S_ItemSlot, declared with that class: it adds a
// per-slot key qword so the object pointer lands at +0x10).

namespace wh::framework {

template<class T>
struct S_WuidSlot {
    uint16_t m_generation;      // +0x00  must match the WUID's generation field
    uint8_t  _pad02[6];         // +0x02
    T*       m_pObject;         // +0x08  null = empty slot
};

}  // namespace wh::framework
