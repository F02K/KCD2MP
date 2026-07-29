#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkRPGParamOverrideDatabase -- the perk_rpg_param_override table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D362A0  vtable 0x183C3B6E8.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkRPGParamOverride, S_PerkRPGParamOverrideDBData>, CryGUID>
// [key INFERRED].  GLOBAL STATIC @0x18532E230 (ctor sub_18193721C: table
// "perk_rpg_param_override", group "rpg").  Consumed by C_RPGParamOverridePerk (perks that
// override S_RpgParams values -- cheat-relevant: this is how perks patch RPG tuning).

namespace wh::rpgmodule {

struct S_PerkRPGParamOverride;         // row type -- not RE'd
struct S_PerkRPGParamOverrideDBData;   // per-row DB payload -- not RE'd

class C_PerkRPGParamOverrideDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkRPGParamOverride, S_PerkRPGParamOverrideDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkRPGParamOverrideDatabase;
    // Global static instance @RVA 0x532E230. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkRPGParamOverrideDatabase* GetInstance();
};
static_assert(sizeof(C_PerkRPGParamOverrideDatabase) == 0xB0,
              "C_PerkRPGParamOverrideDatabase must be 0xB0 (ctor write extent)");

}  // namespace wh::rpgmodule
