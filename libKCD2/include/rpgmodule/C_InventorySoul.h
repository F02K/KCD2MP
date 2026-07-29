#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include "I_InventorySoul.h"
#include "../entitymodule/I_InventoryListener.h"
#include "../framework/I_ScheduleUpdatable.h"

namespace wh::entitymodule {
class C_EquipmentManager;
class C_Inventory;
class C_Item;
}

namespace wh::playermodule {
class C_OutfitManager;
class C_PlayerInventorySoul;
}

namespace wh::rpgmodule {

class C_Soul;

// wh::rpgmodule::C_InventorySoul -- KCD2 WHGame.dll 1.5.6 (kd7u), sizeof 0x148.
// RTTI CHD 0x1843F4EE0 proves I_InventorySoul @+0x00, I_InventoryListener @+0x08,
// and I_ScheduleUpdatable @+0x10. Ctor sub_1803F22D8 writes the three vtables.
class C_InventorySoul
    : public I_InventorySoul
    , public entitymodule::I_InventoryListener
    , public framework::I_ScheduleUpdatable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InventorySoul;

    entitymodule::C_Inventory* GetInventory() override;                    // [0]  sub_181A73FD0
    entitymodule::C_EquipmentManager* GetEquipmentManager() override;      // [1]  sub_181A77FB0
    void InventorySoulUnk02() override;                                    // [2]  sub_18087C338 [U signature]
    C_Soul* GetSoul() override;                                            // [3]  sub_1809DD2D0
    const C_Soul* GetSoulConst() const override;                            // [4]  sub_1809DD2D0
    void InventorySoulUnk05() override;                                    // [5]  sub_1808F1964 [U signature]
    void InventorySoulUnk06() override;                                    // [6]  sub_181058674 [U signature]
    void InventorySoulUnk07() override;                                    // [7]  sub_181057828 [U signature]
    void InventorySoulUnk08() override;                                    // [8]  sub_181058738 [U signature]
    void InventorySoulUnk09() override;                                    // [9]  sub_1810578C8 [U signature]
    void InventorySoulUnk10() override;                                    // [10] sub_181FE1A00 [U signature]
    void InventorySoulUnk11() override;                                    // [11] sub_181FE1AE0 [U signature]
    void InventorySoulUnk12() override;                                    // [12] sub_181FE05B0 [U signature]
    void InventorySoulUnk13() override;                                    // [13] sub_181FE0E40 [U signature]
    void InventorySoulUnk14() override;                                    // [14] sub_181FE1280 [U signature]
    void InventorySoulUnk15() override;                                    // [15] sub_1806C950C [U signature]
    void InventorySoulUnk16() override;                                    // [16] sub_1808CEDA0 [U signature]
    void InventorySoulUnk17() override;                                    // [17] sub_1804C9B08 [U signature]
    void InventorySoulUnk18() override;                                    // [18] sub_1804CA41C [U signature]
    void InventorySoulUnk19() override;                                    // [19] sub_18087E8A4 [U signature]
    void InventorySoulUnk20() override;                                    // [20] sub_18087E640 [U signature]
    void InventorySoulUnk21() override;                                    // [21] sub_18087E22C [U signature]
    void InventorySoulUnk22() override;                                    // [22] sub_180475DCC [U signature]
    void InventorySoulUnk23() override;                                    // [23] sub_181364B28 [U signature]
    bool InventorySoulUnk24() override;                                    // [24] sub_1806C7BB8
    void InventorySoulUnk25() override;                                    // [25] sub_1808D698C [U signature]
    void InventorySoulUnk26() override;                                    // [26] sub_18185755C [U signature]
    void* InventorySoulUnk27() override;                                   // [27] sub_181A74220 -> +0x128
    void InventorySoulUnk28(void* value) override;                         // [28] sub_181A94860 -> +0x128
    std::uint32_t InventorySoulUnk29() const override;                      // [29] sub_180475E00 -> +0x130
    void SetInventorySoulFlag0(bool value) override;                       // [30] sub_1816B4F30
    bool GetInventorySoulFlag0() const override;                           // [31] sub_181A947D0
    void SetInventorySoulFlag1(bool value) override;                       // [32] sub_1816B4F20
    bool GetInventorySoulFlag1() const override;                           // [33] sub_181A94840
    void SetInventorySoulFlag2(bool value) override;                       // [34] sub_1812D8290
    bool GetInventorySoulFlag2() const override;                           // [35] sub_181A94850
    void InventorySoulUnk36() override;                                    // [36] sub_181FE18C0 [U signature]
    void InventorySoulUnk37() override;                                    // [37] sub_18087AAB8 [U signature]
    void InventorySoulUnk38() override;                                    // [38] sub_18068C0C4 [U signature]
    void InventorySoulUnk39() override;                                    // [39] sub_181FE1810 [U signature]
    void EquipItem(entitymodule::C_Item* item, bool a2) override;          // [40] sub_1808EEE6C
    void UnequipItem(entitymodule::C_Item* item, bool a2) override;        // [41] sub_1808C7430
    void InventorySoulUnk42() override;                                    // [42] sub_1811A2E60 [U signature]
    void* InventorySoulUnk43() override;                                   // [43] sub_18087E308
    void* InventorySoulUnk44() override;                                   // [44] sub_18087E2F4
    void InventorySoulUnk45() override;                                    // [45] sub_18154C09C [U signature]
    void InventorySoulUnk46() override;                                    // [46] sub_18154BFB8 [U signature]
    void InventorySoulUnk47() override;                                    // [47] sub_1803A36AC [U signature]
    void InventorySoulUnk48() override;                                    // [48] sub_181FE0BF0 [U signature]
    playermodule::C_OutfitManager* GetOutfitManager() override;            // [49] sub_1806875A4 -> player soul's embedded manager, else null
    playermodule::C_PlayerInventorySoul* GetPlayerInventorySoul() override; // [50] sub_181A74CD0 -> +0x110
    std::uint32_t InventorySoulUnk51(const entitymodule::C_Item* item) override; // [51] sub_1808F769C

    void InventoryListenerUnk0(entitymodule::C_Item* item) override;       // I_InventoryListener [0] sub_18068B84C
    void InventoryListenerUnk1(void* value) override;                      // I_InventoryListener [1] sub_1814BC32C
    void InventoryListenerUnk2(void* value) override;                      // I_InventoryListener [2] sub_18068BE38
    ~C_InventorySoul() override;                                           // I_InventoryListener [7] sub_18213AA1C

    void ScheduleUpdate(std::int32_t value) override;                      // I_ScheduleUpdatable [0] sub_1808F2E88

    C_Soul* m_pOwner;                                      // +0x018
    std::uint8_t m_unk20[0xF0];                            // +0x020
    std::unique_ptr<playermodule::C_PlayerInventorySoul> m_pPlayerInventorySoul; // +0x110, owns concrete 0x748 allocation
    entitymodule::C_Inventory* m_pInventory;               // +0x118
    entitymodule::C_EquipmentManager* m_pEquipmentManager; // +0x120
    void* m_unk128;                                        // +0x128
    std::uint32_t m_unk130;                                // +0x130
    std::uint8_t m_unk134[4];                              // +0x134
    std::uint32_t m_unk138;                                // +0x138
    std::int32_t m_unk13C;                                 // +0x13C, ctor -1
    std::uint16_t m_word140;                               // +0x140
    std::uint16_t m_word142;                               // +0x142
    std::uint8_t m_byte144;                                // +0x144
    std::uint8_t m_flags145;                               // +0x145
    std::uint8_t m_flag146;                                // +0x146
    std::uint8_t _pad147;                                  // +0x147
};
static_assert(sizeof(C_InventorySoul) == 0x148, "C_InventorySoul must be 0x148");
static_assert(offsetof(C_InventorySoul, m_pOwner) == 0x18, "owner must be at +0x18");
static_assert(offsetof(C_InventorySoul, m_pPlayerInventorySoul) == 0x110, "player inventory soul must be at +0x110");
static_assert(offsetof(C_InventorySoul, m_pInventory) == 0x118, "inventory must be at +0x118");
static_assert(offsetof(C_InventorySoul, m_pEquipmentManager) == 0x120, "equipment manager must be at +0x120");
static_assert(offsetof(C_InventorySoul, m_flags145) == 0x145, "flags must be at +0x145");

}  // namespace wh::rpgmodule
