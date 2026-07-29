#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_Location2PerkDatabase -- the location2perk table (location -> granted perk)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D399C8  vtable 0x183C3C2F8.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_Location2Perk, S_Location2PerkDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532D490 (ctor sub_181937BBC: table "location2perk", group "rpg").

namespace wh::rpgmodule {

struct S_Location2Perk;         // row type -- not RE'd
struct S_Location2PerkDBData;   // per-row DB payload -- not RE'd

class C_Location2PerkDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_Location2Perk, S_Location2PerkDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Location2PerkDatabase;
    // Global static instance @RVA 0x532D490. Impl in src/databasemodule/databasemodule.cpp.
    static C_Location2PerkDatabase* GetInstance();
};
static_assert(sizeof(C_Location2PerkDatabase) == 0xB0, "C_Location2PerkDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
