#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkBuffOverrideDatabase -- the perk_buff_override table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D38B20  vtable 0x183C3BB70.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkBuffOverride, S_PerkBuffOverrideDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532DE50 (ctor sub_1819375B8: table "perk_buff_override", group "rpg").
// Consumed by C_BuffOverridePerk (RTTI-listed perk type, not yet RE'd).

namespace wh::rpgmodule {

struct S_PerkBuffOverride;         // row type -- not RE'd
struct S_PerkBuffOverrideDBData;   // per-row DB payload -- not RE'd

class C_PerkBuffOverrideDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkBuffOverride, S_PerkBuffOverrideDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkBuffOverrideDatabase;
    // Global static instance @RVA 0x532DE50. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkBuffOverrideDatabase* GetInstance();
};
static_assert(sizeof(C_PerkBuffOverrideDatabase) == 0xB0, "C_PerkBuffOverrideDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
