#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "../CryEngine/CryCommon/CryString.h"
#include "E_AlchemyPotionBaseType.h"
#include "C_AlchemyRecipeIngredient.h"
#include "C_AlchemyRecipeProduct.h"
#include "C_AlchemyRecipeStep.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyRecipe -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_AlchemyRecipe@playermodule@wh@@ (TD 0x184DE1A80).  One row of
// Libs/Tables/minigame/AlchemyRecipe.xml (+ __autotests / DLC variants).  RTTR-reflected by
// sub_18077C204; member offsets from the property_wrapper immediates.  Size stride-VERIFIED:
// the DSL eval driver sub_182E2BD10 iterates the DB row array with `v7 += 0x80`.
// Sub-vectors hold elements INLINE (Steps stride 0x38 observed directly; Ingredients/Products
// 0x20 inferred by symmetry).
//
// Runtime lookup: C_AlchemyRecipeDatabase static singleton -- object base qword_185332AF0,
// id-indexed MI sub-object qword_185332B68 (= base+0x78); GetById = sub_1808C3BF0(&qword_185332B68,
// id) -> row*.  Rows vector at base+0x30 (xmmword_185332B20).
// NOTE: the solver's `*(a2+0x100)` "target score" belongs to the SOLVER STATE object, NOT this row
// (row is only 0x80) -- corrected vs brew_state_grading.md §5.2.

namespace wh::entitymodule { class C_Inventory; }

namespace wh::playermodule {

class C_AlchemyRecipe {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipe;
    virtual ~C_AlchemyRecipe() = default;                                     // [0]
    // Slots [1..7]/[9..10] are uncertified fillers -- DO NOT call through them.  [8]/[11]/[12]
    // verified from the autocook gate sub_1808C3AF4 + the DSL PotionBase functor 0x182E2B480.
    virtual void _vf1() = 0;                                                  // [1]
    virtual void _vf2() = 0;                                                  // [2]
    virtual void _vf3() = 0;                                                  // [3]
    virtual void _vf4() = 0;                                                  // [4]
    virtual void _vf5() = 0;                                                  // [5]
    virtual void _vf6() = 0;                                                  // [6]
    virtual void _vf7() = 0;                                                  // [7]
    // Availability gate: true iff the inventory holds every ingredient (autocook gate calls it
    // before gathering; alias/substitute-aware).
    virtual bool CanBrewFrom(entitymodule::C_Inventory* inventory) = 0;       // [8]  +0x40
    virtual void _vf9() = 0;                                                  // [9]
    virtual void _vf10() = 0;                                                 // [10]
    // Base-material getter (impl 0x181A8BBC0 = *(u8*)(this+12); the PotionBase() functor
    // compares its result against the poured base row's byte).
    virtual E_AlchemyPotionBaseType::Type GetBaseMaterial() const = 0;        // [11] +0x58
    // Walks the ingredient rows invoking cb(row&) per row (param = MSVC std::function<void(
    // C_AlchemyRecipeIngredient&)>& in the binary; modeled opaque -- iterate m_ingredients
    // directly instead of calling this).
    virtual void ForEachIngredient(void* cb) = 0;                             // [12] +0x60

    uint32_t                            m_recipeId;             // +0x08  <AlchemyRecipe RecipeId>
    E_AlchemyPotionBaseType::Type       m_baseMaterial;         // +0x0C  required base liquid -- ONE BYTE
                                                                //        (getter vtbl+0x58 0x181A8BBC0 reads u8)
    uint8_t                             _pad0D[3];              // +0x0D  uninitialized heap noise
    CryStringT<char>                    m_uiName;               // +0x10  loc key
    CryStringT<char>                    m_uiUsage;              // +0x18  loc key
    CryStringT<char>                    m_uiIcon;               // +0x20  icon name
    int32_t                             m_baseQualityRewardXP;  // +0x28  base XP grant (18-20 in data)
    uint32_t                            _pad2C;                 // +0x2C
    std::vector<C_AlchemyRecipeIngredient> m_ingredients;       // +0x30  inline elements (0x20)
    std::vector<C_AlchemyRecipeProduct>    m_products;          // +0x48  inline elements (0x20), quality 1..4
    std::vector<C_AlchemyRecipeStep>       m_steps;             // +0x60  inline elements (0x38)
    int32_t                             m_dlcId;                // +0x78  optional <DlcId>
    uint32_t                            _pad7C;                 // +0x7C
};
static_assert(sizeof(C_AlchemyRecipe) == 0x80, "C_AlchemyRecipe must be 0x80");
static_assert(offsetof(C_AlchemyRecipe, m_ingredients) == 0x30, "ingredients at 0x30");
static_assert(offsetof(C_AlchemyRecipe, m_steps) == 0x60, "steps at 0x60");

}  // namespace wh::playermodule
