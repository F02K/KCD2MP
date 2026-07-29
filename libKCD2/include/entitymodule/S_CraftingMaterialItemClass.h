#pragma once
#include <cstddef>
#include "S_DivisibleItemClass.h"
#include "S_TypedItemClass.h"

// -----------------------------------------------
// wh::entitymodule::S_CraftingMaterialItemClass -- crafting-material item classes (KCD2
// WHGame.dll 1.5.6).  sizeof 0xD8.  E_ItemType 9 (concrete; also the base of S_HerbItemClass).
// -----------------------------------------------
// RTTI .?AUS_CraftingMaterialItemClass@entitymodule@wh@@; primary vtable 0x183A4DB10, wrapper
// RTTI S_ItemClassWrapper<S_CraftingMaterialItemClass, S_DivisibleItemClass, 9>.  The wrapper
// adds the S_TypedItemClass mixin at +0xC8 (secondary vtable); no own fields beyond it --
// derived S_HerbItemClass fields start directly at +0xD8.
namespace wh::entitymodule {

class S_CraftingMaterialItemClass
    : public S_DivisibleItemClass   // +0x00 .. 0xC8
    , public S_TypedItemClass {     // +0xC8 .. 0xD8 (per-leaf thunk vtable)
};

static_assert(sizeof(S_CraftingMaterialItemClass) == 0xD8,
              "S_CraftingMaterialItemClass must be 0xD8");

}  // namespace wh::entitymodule
