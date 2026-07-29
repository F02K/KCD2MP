#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseKeyMultiIndexed.h"

// -----------------------------------------------
// wh::rpgmodule::C_DocumentRewardPerkDatabase -- the document_reward_perk table
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D398A0  vtable 0x183BAFFE0.  Stack: C_ObjectDatabaseKeyMultiIndexed<
// C_ObjectTableDatabase<S_DocumentRewardPerk, S_DocumentRewardPerkDBData>> -- the KeyMulti
// decorator's layout is UNRESOLVED (see its header), so NO sizeof assert here; use only via
// pointer/base access.  GLOBAL STATIC @0x18532D240 (ctor sub_18153FBE0: table
// "document_reward_perk", group "rpg").

namespace wh::rpgmodule {

struct S_DocumentRewardPerk;         // row type -- not RE'd
struct S_DocumentRewardPerkDBData;   // per-row DB payload -- not RE'd

class C_DocumentRewardPerkDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyMultiIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_DocumentRewardPerk, S_DocumentRewardPerkDBData>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DocumentRewardPerkDatabase;
    // Global static instance @RVA 0x532D240. Impl in src/databasemodule/databasemodule.cpp.
    static C_DocumentRewardPerkDatabase* GetInstance();
};
// NO sizeof assert: the KeyMultiIndexed decorator layout is unresolved.

}  // namespace wh::rpgmodule
