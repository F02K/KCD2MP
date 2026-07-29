#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyRecipeIngredient -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI .?AVC_AlchemyRecipeIngredient@playermodule@wh@@ (TD 0x184DE1790).  One <AlchemyIngredient>
// row of a recipe (Tables.pak Libs/Tables/minigame/AlchemyRecipe.xml).  RTTR-reflected by
// sub_18021B430; member offsets from the property_wrapper immediates (CryGUID @+0x08, uint @+0x18).
// Stored INLINE in C_AlchemyRecipe::m_ingredients (element stride 0x20) [stride inferred by
// symmetry with the directly-observed 0x38 Steps stride -- see recipe_dsl_layout.md §1.2].

namespace wh::playermodule {

class C_AlchemyRecipeIngredient {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipeIngredient;
    virtual ~C_AlchemyRecipeIngredient() = default;

    CryGUID  m_ingredientItemId;   // +0x08  item GUID (item__alchemy.xml herb/ingredient)
    uint32_t m_quantity;           // +0x18  required count (recipe "exact" step tests ==)
    uint32_t _pad1C;               // +0x1C
};
static_assert(sizeof(C_AlchemyRecipeIngredient) == 0x20, "C_AlchemyRecipeIngredient must be 0x20");
static_assert(offsetof(C_AlchemyRecipeIngredient, m_ingredientItemId) == 0x08, "GUID at 0x08");

}  // namespace wh::playermodule
