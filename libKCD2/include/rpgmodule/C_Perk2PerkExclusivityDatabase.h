#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "../databasemodule/C_ObjectTableDatabase.h"

// -----------------------------------------------
// wh::rpgmodule::C_Perk2PerkExclusivityDatabase -- the perk2perk_exclusivity table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI TD 0x184D38E80  vtable 0x183C38858.  Stack: plain C_ObjectTableDatabase<
// S_Perk2PerkExclusivity, S_Perk2PerkExclusivityDBData> (NO SortedIdIndexed layer -- the +0x98
// index vector below is this class's OWN member).
// GLOBAL STATIC @0x18532DCF0 (ctor sub_18192A52C: table "perk2perk_exclusivity", group "rpg").
// Cross-links: C_Perk::LookupIndexEntry (slot [4]) binary-searches THIS object's +0x98 index
// (qword_18532DD88, 32-byte entries), and C_PerkDatabase slot [12] registers this database as a
// dependency (&qword_18532DCF0).

namespace wh::rpgmodule {

struct S_Perk2PerkExclusivity;         // row type -- not RE'd (32-byte index entries: {16B perk
                                       // id, 16B partner/group} candidate [UNVERIFIED])
struct S_Perk2PerkExclusivityDBData;   // per-row DB payload -- not RE'd

class C_Perk2PerkExclusivityDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_Perk2PerkExclusivity,
                                                       S_Perk2PerkExclusivityDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Perk2PerkExclusivityDatabase;
    // Global static instance @RVA 0x532DCF0. Impl in src/databasemodule/databasemodule.cpp.
    static C_Perk2PerkExclusivityDatabase* GetInstance();

    std::vector<void*> m_index98;   // +0x98  sorted 32-byte-entry index (searched by C_Perk [4])
};
static_assert(sizeof(C_Perk2PerkExclusivityDatabase) == 0xB0,
              "C_Perk2PerkExclusivityDatabase must be 0xB0 (object ends where C_PerkBuffDatabase begins)");
static_assert(offsetof(C_Perk2PerkExclusivityDatabase, m_index98) == 0x98, "index at 0x98");

}  // namespace wh::rpgmodule
