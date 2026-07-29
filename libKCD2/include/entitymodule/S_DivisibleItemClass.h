#pragma once
#include <cstddef>
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "S_PlayerItemClass.h"

// -----------------------------------------------
// wh::entitymodule::S_DivisibleItemClass -- stackable/divisible item classes (KCD2 WHGame.dll
// 1.5.6).  sizeof 0xC8.  E_ItemType 23 (intermediate).
// -----------------------------------------------
// RTTI .?AUS_DivisibleItemClass@entitymodule@wh@@; vtable 0x183A4E878, ctor sub_180753830.
// Direct base of S_CraftingMaterialItemClass (and the other stack-item branches).  Field TYPES
// + defaults are ctor-verified; PURPOSES not yet pinned (RTTR attr binding untraced) --
// dossier analysis/item_data/s_itemdata_re.md.
namespace wh::entitymodule {

class S_DivisibleItemClass : public S_PlayerItemClass {
public:
    bool     m_unkB0;           // +0xB0  ctor 0 (IsDivisible= candidate; UNVERIFIED)
    uint8_t  _padB1[7];         // +0xB1
    CryStringT<char> m_strB8;   // +0xB8  purpose UNVERIFIED
    CryStringT<char> m_strC0;   // +0xC0  purpose UNVERIFIED
};

static_assert(sizeof(S_DivisibleItemClass) == 0xC8, "S_DivisibleItemClass must be 0xC8");
static_assert(offsetof(S_DivisibleItemClass, m_strB8) == 0xB8, "strings at 0xB8");

}  // namespace wh::entitymodule
