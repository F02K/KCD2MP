#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_RecipePerk -- perk unlocking a recipe (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D53E20  vtable 0x183C07950.  sizeof 0x38 (PROVEN: factory sub_181737C8C
// operator new(56)). Extra: +0x30 dword = 0. Backed by C_PerkRecipeDatabase rows.

namespace wh::rpgmodule {

class C_RecipePerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RecipePerk;
    int32_t  m_recipeId; // +0x30  recipe id: Resolve[9] caches S_PerkRecipe row +0x10; Activate[1] passes it to C_RPGMinigames to unlock
    uint32_t _pad34;    // +0x34
};
static_assert(sizeof(C_RecipePerk) == 0x38, "C_RecipePerk must be 0x38 (operator new(56))");

}  // namespace wh::rpgmodule
