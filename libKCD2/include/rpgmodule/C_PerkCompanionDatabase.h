#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkCompanionDatabase -- the perk_companion table (perk -> companion effect)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D38908  vtable 0x183C3B868.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkCompanion, S_PerkCompanionDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532E0A0 (ctor sub_181937350: table "perk_companion", group "rpg").

namespace wh::rpgmodule {

struct S_PerkCompanion;         // row type -- not RE'd
struct S_PerkCompanionDBData;   // per-row DB payload -- not RE'd

class C_PerkCompanionDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkCompanion, S_PerkCompanionDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkCompanionDatabase;
    // Global static instance @RVA 0x532E0A0. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkCompanionDatabase* GetInstance();
};
static_assert(sizeof(C_PerkCompanionDatabase) == 0xB0, "C_PerkCompanionDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
