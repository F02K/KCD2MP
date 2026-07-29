#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkSoulAbilityDatabase -- the perk_soul_ability table (perk -> soul ability)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D36710  vtable 0x183C3B260.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkSoulAbility, S_PerkSoulAbilityDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532E440 (ctor sub_181936E80: table "perk_soul_ability", group "rpg").
// Relevant to C_Soul::HasAbility -- the ability ids granted by perks come from these rows.

namespace wh::rpgmodule {

struct S_PerkSoulAbility;         // row type -- not RE'd
struct S_PerkSoulAbilityDBData;   // per-row DB payload -- not RE'd

class C_PerkSoulAbilityDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkSoulAbility, S_PerkSoulAbilityDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkSoulAbilityDatabase;
    // Global static instance @RVA 0x532E440. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkSoulAbilityDatabase* GetInstance();
};
static_assert(sizeof(C_PerkSoulAbilityDatabase) == 0xB0, "C_PerkSoulAbilityDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
