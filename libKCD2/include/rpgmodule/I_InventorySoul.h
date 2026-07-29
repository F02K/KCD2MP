#pragma once
#include <cstdint>

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

// wh::rpgmodule::I_InventorySoul -- KCD2 WHGame.dll 1.5.6 (kd7u).
// Pure interface. C_InventorySoul is the enumerated implementation; its primary
// vtable 0x183A33798 contains 52 individually validated function entries.
class I_InventorySoul {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InventorySoul;

    virtual entitymodule::C_Inventory* GetInventory() = 0;                    // [0]  sub_181A73FD0
    virtual entitymodule::C_EquipmentManager* GetEquipmentManager() = 0;      // [1]  sub_181A77FB0
    virtual void InventorySoulUnk02() = 0;                                    // [2]  sub_18087C338 [U signature]
    virtual C_Soul* GetSoul() = 0;                                            // [3]  sub_1809DD2D0
    virtual const C_Soul* GetSoulConst() const = 0;                            // [4]  sub_1809DD2D0
    virtual void InventorySoulUnk05() = 0;                                    // [5]  sub_1808F1964 [U signature]
    virtual void InventorySoulUnk06() = 0;                                    // [6]  sub_181058674 [U signature]
    virtual void InventorySoulUnk07() = 0;                                    // [7]  sub_181057828 [U signature]
    virtual void InventorySoulUnk08() = 0;                                    // [8]  sub_181058738 [U signature]
    virtual void InventorySoulUnk09() = 0;                                    // [9]  sub_1810578C8 [U signature]
    virtual void InventorySoulUnk10() = 0;                                    // [10] sub_181FE1A00 [U signature]
    virtual void InventorySoulUnk11() = 0;                                    // [11] sub_181FE1AE0 [U signature]
    virtual void InventorySoulUnk12() = 0;                                    // [12] sub_181FE05B0 [U signature]
    virtual void InventorySoulUnk13() = 0;                                    // [13] sub_181FE0E40 [U signature]
    virtual void InventorySoulUnk14() = 0;                                    // [14] sub_181FE1280 [U signature]
    virtual void InventorySoulUnk15() = 0;                                    // [15] sub_1806C950C [U signature]
    virtual void InventorySoulUnk16() = 0;                                    // [16] sub_1808CEDA0 [U signature]
    virtual void InventorySoulUnk17() = 0;                                    // [17] sub_1804C9B08 [U signature]
    virtual void InventorySoulUnk18() = 0;                                    // [18] sub_1804CA41C [U signature]
    virtual void InventorySoulUnk19() = 0;                                    // [19] sub_18087E8A4 [U signature]
    virtual void InventorySoulUnk20() = 0;                                    // [20] sub_18087E640 [U signature]
    virtual void InventorySoulUnk21() = 0;                                    // [21] sub_18087E22C [U signature]
    virtual void InventorySoulUnk22() = 0;                                    // [22] sub_180475DCC [U signature]
    virtual void InventorySoulUnk23() = 0;                                    // [23] sub_181364B28 [U signature]
    virtual bool InventorySoulUnk24() = 0;                                    // [24] sub_1806C7BB8
    virtual void InventorySoulUnk25() = 0;                                    // [25] sub_1808D698C [U signature]
    virtual void InventorySoulUnk26() = 0;                                    // [26] sub_18185755C [U signature]
    virtual void* InventorySoulUnk27() = 0;                                   // [27] sub_181A74220 -> +0x128
    virtual void InventorySoulUnk28(void* value) = 0;                         // [28] sub_181A94860 -> +0x128
    virtual std::uint32_t InventorySoulUnk29() const = 0;                      // [29] sub_180475E00 -> +0x130
    virtual void SetInventorySoulFlag0(bool value) = 0;                       // [30] sub_1816B4F30
    virtual bool GetInventorySoulFlag0() const = 0;                           // [31] sub_181A947D0
    virtual void SetInventorySoulFlag1(bool value) = 0;                       // [32] sub_1816B4F20
    virtual bool GetInventorySoulFlag1() const = 0;                           // [33] sub_181A94840
    virtual void SetInventorySoulFlag2(bool value) = 0;                       // [34] sub_1812D8290
    virtual bool GetInventorySoulFlag2() const = 0;                           // [35] sub_181A94850
    virtual void InventorySoulUnk36() = 0;                                    // [36] sub_181FE18C0 [U signature]
    virtual void InventorySoulUnk37() = 0;                                    // [37] sub_18087AAB8 [U signature]
    virtual void InventorySoulUnk38() = 0;                                    // [38] sub_18068C0C4 [U signature]
    virtual void InventorySoulUnk39() = 0;                                    // [39] sub_181FE1810 [U signature]
    // [40] sub_1808EEE6C  EQUIP -- the Lua EquipInventoryItem route (0x1808ED9E4): actor+0x668
    // soul -> soul vf+0x1E0 (returns &m_inventorySoul) -> this slot (item, 1).
    // a2 role [U] (all traced callers pass 1); return [U] (callers propagate rax, Lua discards).
    virtual void EquipItem(entitymodule::C_Item* item, bool a2) = 0;
    // [41] sub_1808C7430  UNEQUIP -- same chain from Lua UnequipInventoryItem (0x1808C73D0).
    virtual void UnequipItem(entitymodule::C_Item* item, bool a2) = 0;
    virtual void InventorySoulUnk42() = 0;                                    // [42] sub_1811A2E60 [U signature]
    virtual void* InventorySoulUnk43() = 0;                                   // [43] sub_18087E308
    virtual void* InventorySoulUnk44() = 0;                                   // [44] sub_18087E2F4
    virtual void InventorySoulUnk45() = 0;                                    // [45] sub_18154C09C [U signature]
    virtual void InventorySoulUnk46() = 0;                                    // [46] sub_18154BFB8 [U signature]
    virtual void InventorySoulUnk47() = 0;                                    // [47] sub_1803A36AC [U signature]
    virtual void InventorySoulUnk48() = 0;                                    // [48] sub_181FE0BF0 [U signature]
    virtual playermodule::C_OutfitManager* GetOutfitManager() = 0;             // [49] sub_1806875A4 -> player soul's embedded manager, else null
    virtual playermodule::C_PlayerInventorySoul* GetPlayerInventorySoul() = 0; // [50] sub_181A74CD0 -> +0x110
    virtual std::uint32_t InventorySoulUnk51(const entitymodule::C_Item* item) = 0; // [51] sub_1808F769C
};
static_assert(sizeof(I_InventorySoul) == 0x08, "I_InventorySoul must be vptr-only");

}  // namespace wh::rpgmodule
