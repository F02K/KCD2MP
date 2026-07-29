#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::E_ItemType -- the item-class type-id namespace (KCD2 WHGame.dll 1.5.6).
// -----------------------------------------------
// [SYNTHETIC NAME] -- the integer S_ItemClass::GetTypeId [1] returns and IsType [3] matches
// (IsKindOf: self + ancestors).  Values proven by the CRTP wrapper RTTI
// (S_ItemClassWrapper<Derived, Parent, TypeId> non-type params) + per-leaf GetTypeId bodies;
// tree table: analysis/item_data/s_itemdata_re.md.  Known binary consumers of the raw ints:
// the item loader (vf3(10)/vf3(6)), the alchemy ingredient counter 0x18175A4BC
// (IsType(11) base / IsType(10) herb).

namespace wh::entitymodule {

struct E_ItemType {
    enum Type : int32_t {
        Misc               = 0,
        Melee              = 1,
        Missile            = 2,
        Ammo               = 3,
        Armor              = 4,
        Food               = 5,
        Money              = 6,
        DiceBadge          = 7,
        Document           = 8,
        CraftingMaterial   = 9,
        Herb               = 10,
        AlchemyBase        = 11,   // water/wine/spirit -- ingredient counter returns 1, never counted
        NPCTool            = 12,
        Ointment           = 13,
        Poison             = 14,
        Die                = 15,
        Helmet             = 16,
        Key                = 17,
        KeyRing            = 18,
        QuickSlotContainer = 19,
        Root               = 21,   // S_ItemClass itself
        Pickable           = 22,   // intermediates
        Divisible          = 23,
        WeaponEquip        = 24,
        Player             = 25,
        Equippable         = 26,
        Weapon             = 27,
        Consumable         = 28,
        Hood               = 29,
    };
};

}  // namespace wh::entitymodule
