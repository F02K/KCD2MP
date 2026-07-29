#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_POIType2PerkDatabase -- the poi_type2perk table (POI type -> granted perk)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D39428  vtable 0x183C3BE70.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_POIType2Perk, S_POIType2PerkDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532DBA0 (ctor sub_181937820: table "poi_type2perk", group "rpg").
// (POI types = the 56-byte registry rows exposed by I_RPGLocationManager slots [43]..[46].)

namespace wh::rpgmodule {

struct S_POIType2Perk;         // row type -- not RE'd
struct S_POIType2PerkDBData;   // per-row DB payload -- not RE'd

class C_POIType2PerkDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_POIType2Perk, S_POIType2PerkDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_POIType2PerkDatabase;
    // Global static instance @RVA 0x532DBA0. Impl in src/databasemodule/databasemodule.cpp.
    static C_POIType2PerkDatabase* GetInstance();
};
static_assert(sizeof(C_POIType2PerkDatabase) == 0xB0, "C_POIType2PerkDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
