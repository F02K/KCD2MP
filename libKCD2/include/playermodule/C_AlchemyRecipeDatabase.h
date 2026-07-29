#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "../databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"
#include "C_AlchemyRecipe.h"
#include "I_AlchemyRecipeDatabase.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyRecipeDatabase -- the recipe table database (KCD2 1.5.6, kd7u).
// sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_AlchemyRecipeDatabase@playermodule@wh@@ (TD name-region 0x184DE10B0).  Bases from
// the ??_R2 walk (COL of the +0x78 vtable 0x183D193E0):
//   C_ObjectDatabaseKeyIndexed<C_ObjectTreeDatabase<C_AlchemyRecipe, std::vector>,
//                              C_ObjectDatabaseDefaultKeyExtractor<C_AlchemyRecipe>>  @+0x00
//   I_AlchemyRecipeDatabase                                                          @+0x78
// STATIC-INIT SINGLETON @0x185332AF0 (ctor sub_1819DE484 -> registration ctor sub_1812BB09C:
// tree identity ns "wh::playermodule::", name "AlchemyRecipe", group "minigame" via
// sub_1819AAF18; then the two ??_7 writes at +0x00/+0x78).  Rows: INLINE 0x80-stride
// C_AlchemyRecipe elements in the +0x30 vector (xmmword_185332B20), key-sorted by RecipeId.
// Canonical GetById callers: recipe-book UI sub_18119F030, autocook gate sub_1808C3AF4.  A
// name-keyed sibling global qword_185332D08 serves the book UI's non-integer branch [identity
// UNVERIFIED].  Sibling alchemy DBs (StepType/PotionBase/Feedback/Crushable) are plain
// C_ObjectDatabase instantiations (feedback rows global xmmword_185332A20, 0x28 stride).

namespace wh::playermodule {

class C_AlchemyRecipeDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<C_AlchemyRecipe>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_AlchemyRecipe>>
    , public I_AlchemyRecipeDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipeDatabase;

    // The static singleton @0x185332AF0 (src/playermodule/C_AlchemyRecipeDatabase.cpp).
    static C_AlchemyRecipeDatabase* GetInstance();

    // Convenience: GetInstance()->GetById(recipeId) through the live I_AlchemyRecipeDatabase vtable.
    static C_AlchemyRecipe* GetRecipeById(uint32_t recipeId);
};
static_assert(sizeof(wh::databasemodule::C_ObjectDatabaseKeyIndexed<
                  wh::databasemodule::C_ObjectTreeDatabase<C_AlchemyRecipe>,
                  wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_AlchemyRecipe>>) == 0x78,
              "key-indexed tree stack must span 0x78 so the interface lands at +0x78");
static_assert(sizeof(C_AlchemyRecipeDatabase) == 0x80, "C_AlchemyRecipeDatabase must be 0x80");

}  // namespace wh::playermodule
