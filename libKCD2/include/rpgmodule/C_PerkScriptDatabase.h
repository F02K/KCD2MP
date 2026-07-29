#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkScriptDatabase -- the perk_script table (perk -> Lua script hook)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D37B60  vtable 0x183C3B3E0.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkScript, S_PerkScriptDBData>, CryGUID> [key INFERRED].
// GLOBAL STATIC @0x18532E390 (ctor sub_181936FB4: table "perk_script", group "rpg").

namespace wh::rpgmodule {

struct S_PerkScript;         // row type -- not RE'd (S_ScriptPerkId RTTR converter is related)
struct S_PerkScriptDBData;   // per-row DB payload -- not RE'd

class C_PerkScriptDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkScript, S_PerkScriptDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkScriptDatabase;
    // Global static instance @RVA 0x532E390. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkScriptDatabase* GetInstance();
};
static_assert(sizeof(C_PerkScriptDatabase) == 0xB0, "C_PerkScriptDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
