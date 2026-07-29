#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include "I_InventoryListener.h"
#include "C_WorldInventory.h"
#include "../framework/I_WUIDMappingProvider.h"
#include "../framework/S_WuidSlot.h"
#include "../framework/C_Signal.h"
#include "../framework/HashPrimitives.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::entitymodule::C_InventoryManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x100280 (1 MB).
// -----------------------------------------------
// RTTI .?AVC_InventoryManager@entitymodule@wh@@  primary vtable 0x183EB6610 (3 slots)  ctor sub_180BEAA38
// Bases (combined single vtable, both mdisp 0): I_WUIDMappingProvider (polymorphic identity here) and
// I_InventoryListener. Singleton: global qword_185487A58 (also C_EntityModule+0xE8). Dominated by the
// 1 MB inline WUID slot table m_slots. Registers by CryGUID -> C_Inventory* and broadcasts
// create/remove via two C_Signals.

namespace wh::entitymodule {

class C_Inventory;

// Object-pool block used by the inventory allocator at +0x28 (bespoke; only 'used' @+12 is proven).
struct S_InventoryPoolBlock {
    void*    storage;       // +0x00
    uint32_t _reserved;     // +0x08
    uint32_t used;          // +0x0C
};

class C_InventoryManager : public wh::framework::I_WUIDMappingProvider {  // +0x00  (combined vtable; also I_InventoryListener)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InventoryManager;
    // Created by the C_EntityModule ctor (sub_180BE8968, slot a1[29], alloc 0x100280 == sizeof);
    // mirrored to global qword_185487A58.
    static C_InventoryManager* GetInstance();
    // Registry lookup sub_181EC6B20(this+0x28, &wuid): validates type byte 3 + generation
    // (WUID bits 16..31) + the inventory's stored WUID.
    C_Inventory* LookupByWUID(const wh::framework::WUID& wuid);

    wh::shared::C_Signal<const CryGUID&, C_Inventory*> m_onInventoryCreated;   // +0x08  emitted on create
    wh::shared::C_Signal<const CryGUID&, C_Inventory*> m_onInventoryRemoved;   // +0x18  emitted on remove
    // --- bespoke C_Inventory object pool (+0x28..+0x50) ---
    S_InventoryPoolBlock* m_poolCurBlock;               // +0x28  current block cursor (alloc sub_181EE0240 reads curBlock->used @+0xC)
    S_InventoryPoolBlock* m_poolFreeHint;               // +0x30  free-slot hint; compared against block pointers and cleared to 0 in sub_181EE0240
    std::vector<S_InventoryPoolBlock> m_poolBlocks;     // +0x38
    // --- inline WUID slot registry (occupied slots hold the C_Inventory*; free slots thread a list
    //     through the slot's [2..6] words). Index 0 and 0xFFFF are reserved control slots. ---
    wh::framework::S_WuidSlot<C_Inventory> m_slots[0x10000];   // +0x50   (0x100000)
    uint64_t m_regStat0;                               // +0x100050  registry statistics (unverified)
    uint64_t m_regStat1;                               // +0x100058
    uint64_t m_regFreePushCount;                       // +0x100060
    uint32_t m_liveCount;                              // +0x100068  ++ on acquire / -- on release
    uint8_t  _pad10006C[4];                            // +0x10006C
    C_WorldInventory m_worldInventory;                 // +0x100070  embedded (0x1A8)
    // Registry: CryGUID(16B) -> owning C_Inventory*. Real hasher is FNV-1a per 8-byte half XOR'd;
    // S_DefaultHash<CryGUID> (FNV-1a over 16B) is the size-equivalent transparent hasher.
    std::unordered_map<CryGUID, C_Inventory*, wh::shared::S_DefaultHash<CryGUID>> m_guidToInventory;  // +0x100218 (0x40)
    // Safe-iteration listener list (vector + reentrancy depth guard + compaction flag).
    std::vector<I_InventoryListener*> m_listeners;     // +0x100258
    uint64_t m_notifyDepth;                            // +0x100270  dispatch reentrancy counter
    uint16_t m_notifyDirtyFlag;                        // +0x100278  pending-compaction flag
    uint8_t  _pad10027A[6];                            // +0x10027A
};
static_assert(sizeof(C_InventoryManager) == 0x100280, "C_InventoryManager must be 0x100280");

}  // namespace wh::entitymodule
