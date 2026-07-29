#pragma once
#include <cstddef>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "../CryEngine/CryCommon/CryString.h"
#include "S_CraftingMaterialItemClass.h"

// -----------------------------------------------
// wh::entitymodule::S_HerbItemClass -- herb item classes, fresh AND dried rows (KCD2
// WHGame.dll 1.5.6).  sizeof 0x110.  E_ItemType 10.
// -----------------------------------------------
// RTTI .?AUS_HerbItemClass@entitymodule@wh@@; primary vtable 0x183A4D630 (mixin thunk vtable
// 0x183A4D840); wrapper RTTI S_ItemClassWrapper<S_HerbItemClass, S_CraftingMaterialItemClass,
// 10>.  Loaded from Libs/Tables/item/item.xml <Herb> rows; the member names below ARE the
// game's own RTTR property strings, offsets baked into the property wrappers by the registrar
// sub_180CDA174 ("wh::entitymodule::Herb"; DriedItemId wrapper 0x180CDAE58 offset 0x100,
// DecayTime wrapper 0x180CDAEE8 offset 0xFC).
namespace wh::entitymodule {

class S_HerbItemClass : public S_CraftingMaterialItemClass {
public:
    CryStringT<char> m_latinName;    // +0xD8  "LatinName"
    CryStringT<char> m_desc;         // +0xE0  "Desc" loc key
    CryStringT<char> m_effect;       // +0xE8  "Effect" loc key
    CryStringT<char> m_occur;        // +0xF0  "Occur" loc key
    bool             m_poisonous;    // +0xF8  "Poisonous"
    bool             m_inHerbarium;  // +0xF9  "InHerbarium" (false on dried rows)
    uint8_t          _padFA[2];      // +0xFA
    float            m_decayTime;    // +0xFC  "DecayTime" hours; 0 = never decays
    CryGUID          m_driedItemId;  // +0x100 "DriedItemId" -- dried-variant class guid or null
};

static_assert(sizeof(S_HerbItemClass) == 0x110, "S_HerbItemClass must be 0x110");
static_assert(offsetof(S_HerbItemClass, m_latinName) == 0xD8, "herb fields start at 0xD8");
static_assert(offsetof(S_HerbItemClass, m_driedItemId) == 0x100, "DriedItemId at 0x100");

}  // namespace wh::entitymodule
