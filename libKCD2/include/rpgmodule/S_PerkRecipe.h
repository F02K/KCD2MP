#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PerkRecipe -- perk_recipe table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x14, ALIGN 4 (PROVEN: C_PerkRecipeDatabase ParseRows 0x181632670, blob stride 20:
// {16B @0, dword @16}).  Stride 20 rules out an 8-aligned CryGUID -- mirrored as uint32[4].

namespace wh::rpgmodule {

struct S_PerkRecipe {
    uint32_t m_perkId[4];   // +0x00  16-byte GUID value, 4-aligned [role INFERRED]
    int32_t  m_param10;     // +0x10  [role UNVERIFIED -- recipe id/count candidate]
};
static_assert(sizeof(S_PerkRecipe) == 0x14 && alignof(S_PerkRecipe) == 4,
              "S_PerkRecipe must be 0x14/align-4 (ParseRows stride 20)");

}  // namespace wh::rpgmodule
