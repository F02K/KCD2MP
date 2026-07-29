#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseIdIndexed.h"

// -----------------------------------------------
// wh::rpgmodule::C_ReputationConditionDatabase -- the reputation_condition table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB8.
// -----------------------------------------------
// RTTI TD 0x184D37580  vtable 0x183C3AC58.  Stack: C_ObjectDatabaseIdIndexed<
// C_ObjectTableDatabase<S_ReputationCondition, S_ReputationConditionDBData>> -- IdIndexed but
// WITHOUT the I_DynamicEnum base the archetype/social-class databases add (single vtable).
// GLOBAL STATIC @0x18532F8B0 (ctor sub_1819369B0: table "reputation_condition", group "rpg").

namespace wh::rpgmodule {

struct S_ReputationCondition;         // row type -- not RE'd
struct S_ReputationConditionDBData;   // per-row DB payload -- not RE'd

class C_ReputationConditionDatabase
    : public wh::databasemodule::C_ObjectDatabaseIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_ReputationCondition, S_ReputationConditionDBData>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReputationConditionDatabase;
    // Global static instance @RVA 0x532F8B0. Impl in src/databasemodule/databasemodule.cpp.
    static C_ReputationConditionDatabase* GetInstance();
};
static_assert(sizeof(C_ReputationConditionDatabase) == 0xB8,
              "C_ReputationConditionDatabase must be 0xB8 (IdIndexed extent, no enum base)");

}  // namespace wh::rpgmodule
