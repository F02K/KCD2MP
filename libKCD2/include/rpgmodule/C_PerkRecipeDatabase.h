#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkRecipeDatabase -- the perk_recipe table (perk -> unlocked recipe)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D36890  vtable 0x183C3B560.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkRecipe, S_PerkRecipeDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532E2E0 (ctor sub_1819370E8: table "perk_recipe", group "rpg").

namespace wh::rpgmodule {

struct S_PerkRecipe;         // row type -- not RE'd
struct S_PerkRecipeDBData;   // per-row DB payload -- not RE'd

class C_PerkRecipeDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkRecipe, S_PerkRecipeDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkRecipeDatabase;
    // Global static instance @RVA 0x532E2E0. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkRecipeDatabase* GetInstance();
};
static_assert(sizeof(C_PerkRecipeDatabase) == 0xB0, "C_PerkRecipeDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
