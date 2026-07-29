#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkCodexDatabase -- the perk_codex table (perk -> codex entry)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D38CF8  vtable 0x183C3B9E8.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkCodex, S_PerkCodexDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532DF00 (ctor sub_181937484: table "perk_codex", group "rpg").

namespace wh::rpgmodule {

struct S_PerkCodex;         // row type -- not RE'd
struct S_PerkCodexDBData;   // per-row DB payload -- not RE'd

class C_PerkCodexDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkCodex, S_PerkCodexDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkCodexDatabase;
    // Global static instance @RVA 0x532DF00. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkCodexDatabase* GetInstance();
};
static_assert(sizeof(C_PerkCodexDatabase) == 0xB0, "C_PerkCodexDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
