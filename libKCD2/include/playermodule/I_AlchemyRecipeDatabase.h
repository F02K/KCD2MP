#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::playermodule::I_AlchemyRecipeDatabase -- recipe-query interface of the recipe database
// (KCD2 WHGame.dll 1.5.6, kd7u).  MI base of C_AlchemyRecipeDatabase at +0x78.
// -----------------------------------------------
// RTTI .?AVI_AlchemyRecipeDatabase@playermodule@wh@@; live vtable 0x183D193E0 -- exactly 3 slots,
// each verified as a function start (wh:: type, NOT interfuscator-shuffled):
//   [0] 0x1808C3948  collect every row passing row-vtbl[15] (availability probe) into the result
//   [1] 0x1808C39E4  same, additionally requiring row-vtbl[10](row, filterArg)
//   [2] 0x1808C3BF0  by-id lookup: std::lower_bound over the key-sorted inline rows, verify
//                    row-vtbl[1] (GetId) == id, else null
// The vtable has NO dtor slot -- do not delete through this interface (dtor kept protected).
// [0]/[1] construct the result vector through the hidden return pointer: the GAME's CRT
// allocates the buffer -- destroy it only if the plugin shares that CRT.
// Method names [INFERRED] from the filter semantics; GetById VERIFIED (canonical caller
// recipe-book UI sub_18119F030, autocook gate sub_1808C3AF4).

namespace wh::playermodule {

class C_AlchemyRecipe;

class I_AlchemyRecipeDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AlchemyRecipeDatabase;

    // [0] rows passing the availability probe (row vtbl slot [15])
    virtual std::vector<C_AlchemyRecipe*> CollectAvailableRecipes() = 0;
    // [1] as [0], additionally matching row vtbl slot [10] against filterArg [arg role UNRESOLVED]
    virtual std::vector<C_AlchemyRecipe*> CollectAvailableRecipesMatching(uint64_t filterArg) = 0;
    // [2] id -> row (null when unknown)
    virtual C_AlchemyRecipe* GetById(uint32_t recipeId) = 0;

protected:
    ~I_AlchemyRecipeDatabase() = default;   // no virtual dtor in the binary's vtable
};
static_assert(sizeof(I_AlchemyRecipeDatabase) == 8, "vptr-only interface subobject");

}  // namespace wh::playermodule
