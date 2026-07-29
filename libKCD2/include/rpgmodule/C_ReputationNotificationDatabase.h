#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseIdIndexed.h"

// -----------------------------------------------
// wh::rpgmodule::C_ReputationNotificationDatabase -- the reputation_notification table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB8.
// -----------------------------------------------
// RTTI TD 0x184D35D60  vtable 0x183C3AAD0.  Stack: C_ObjectDatabaseIdIndexed<
// C_ObjectTableDatabase<S_ReputationNotification, S_ReputationNotificationDBData>> (no enum base).
// GLOBAL STATIC @0x18532F7F0 (ctor sub_18193687C: table "reputation_notification", group "rpg").

namespace wh::rpgmodule {

struct S_ReputationNotification;         // row type -- not RE'd
struct S_ReputationNotificationDBData;   // per-row DB payload -- not RE'd

class C_ReputationNotificationDatabase
    : public wh::databasemodule::C_ObjectDatabaseIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_ReputationNotification, S_ReputationNotificationDBData>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReputationNotificationDatabase;
    // Global static instance @RVA 0x532F7F0. Impl in src/databasemodule/databasemodule.cpp.
    static C_ReputationNotificationDatabase* GetInstance();
};
static_assert(sizeof(C_ReputationNotificationDatabase) == 0xB8,
              "C_ReputationNotificationDatabase must be 0xB8 (IdIndexed extent, no enum base)");

}  // namespace wh::rpgmodule
