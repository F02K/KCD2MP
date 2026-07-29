#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkBuffDatabase -- the perk_buff table (perk -> buff mapping)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D390E8  vtable 0x183C3BCF0.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkBuff, S_PerkBuffDBData>, CryGUID> [key INFERRED from the
// untruncated sibling TDs].  GLOBAL STATIC @0x18532DDA0 (ctor sub_1819376EC: table "perk_buff",
// group "rpg"). No concrete tail (ctor write extent = the template stack).

namespace wh::rpgmodule {

struct S_PerkBuff;         // row type -- not RE'd
struct S_PerkBuffDBData;   // per-row DB payload -- not RE'd

class C_PerkBuffDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkBuff, S_PerkBuffDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkBuffDatabase;
    // Global static instance @RVA 0x532DDA0. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkBuffDatabase* GetInstance();
};
static_assert(sizeof(C_PerkBuffDatabase) == 0xB0, "C_PerkBuffDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
