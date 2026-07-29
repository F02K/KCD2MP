#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "E_WeaponEquipSlot.h"
#include "I_ItemCollectionListener.h"
#include "S_EquipmentSlotIdWrapper.h"
#include "../framework/WUID.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::entitymodule::C_EquipmentManager : I_ItemCollectionListener -- KCD2 1.5.6 (kd7u).  sizeof 0x130.
// -----------------------------------------------
// RTTI .?AVC_EquipmentManager@entitymodule@wh@@  primary vtable 0x183EB1708 (9 slots)  ctor sub_180A30D68
// Single inheritance: I_ItemCollectionListener (pure interface base, vtable @+0x00, no data).
// KCD2 stores eight fixed E_WeaponEquipSlot item pointers. Data-driven clothing EquipmentSlotIds are
// a separate system, mapped to item WUIDs at +0x58.

namespace wh::entitymodule {

class C_Entity;
class C_Item;
class I_EquipmentObserver;

class C_EquipmentManager : public I_ItemCollectionListener {   // +0x00  (vtable only)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EquipmentManager;

    // Native C++ RTTI names this C_EquipmentManager::E_QuickSlotSpec::Type. Public RTTR flattens
    // the reflected names to wh::entitymodule::E_QuickSlotSpec and E_QuickSlotSpecWrapper.
    // This is an EquipPlayersItem selection policy, not part of S_EquipmentSlotIdWrapper.
    struct E_QuickSlotSpec {
        enum Type : std::int32_t {
            QuickSlot1                 = 0,
            QuickSlot2                 = 1,
            QuickSlot3                 = 2,
            QuickSlot4                 = 3,
            QuickSlotMatchingWeaponSet = 4,
            Default                    = 5,
        };
    };

    virtual void* EquipmentManagerUnk06(void* value);          // [6] sub_182A05414 [U role]
    virtual C_EquipmentManager* EquipmentManagerUnk07();       // [7] sub_1805F5DA0 returns this
    virtual void* EquipmentManagerUnk08(void* value);          // [8] sub_182A0529C [U role]

    C_Entity* m_owner;                                         // +0x08  owning entity back-pointer
    // Indices into the attachment-state bitset. sub_181ED89A0 clears the old indices and
    // sub_181ED6800 rebuilds them from every equipped item.
    std::vector<std::uint32_t> m_equippedAttachmentSlotIndices; // +0x10
    // Deferred equipment restoration queue consumed by sub_1803A3930. Each 0x10-byte key is
    // resolved through the item-instance GUID registry; original source member spelling unknown.
    std::vector<CryGUID> m_itemGuidsToEquip;                    // +0x28
    std::vector<C_Item*> m_equippedItems;                       // +0x40  equipped item pointers
    // Four-byte clothing-slot key -> item WUID. The key ABI is proven EquipmentSlotId-compatible;
    // metadata does not distinguish the original source key spelling (uint32_t vs wrapper).
    std::unordered_map<std::uint32_t, wh::framework::WUID> m_clothing; // +0x58  sizeof 0x40
    C_Item* m_weaponEquipSlots[8];                              // +0x98  indexed by E_WeaponEquipSlot
    std::vector<bool> m_weaponEquipSlotOccupied;                // +0xD8  eight occupancy bits
    float m_totalWeight;                                        // +0xF8
    float m_wornWeight;                                         // +0xFC
    std::vector<I_EquipmentObserver*> m_changeObservers;         // +0x100
    std::uint64_t m_dispatchDepth;                              // +0x118
    bool m_observersHasRemovals;                                // +0x120
    bool m_observersNeedsClear;                                 // +0x121
    std::uint8_t _pad122[6];                                   // +0x122
    std::uint32_t m_recomputeSuspendCount;                      // +0x128
    std::uint8_t _pad12C[4];                                   // +0x12C
};

static_assert(sizeof(C_EquipmentManager::E_QuickSlotSpec::Type) == 0x04,
    "E_QuickSlotSpec::Type must be 0x04");
static_assert(sizeof(C_EquipmentManager) == 0x130, "C_EquipmentManager must be 0x130");

}  // namespace wh::entitymodule
