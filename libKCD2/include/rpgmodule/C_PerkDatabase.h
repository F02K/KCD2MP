#pragma once
#include <cstdint>
#include <cstddef>
#include "S_Perk.h"
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkDatabase -- the perk table (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xE0.
// -----------------------------------------------
// RTTI .?AVC_PerkDatabase@rpgmodule@wh@@ (TD 0x184D37868)  vtable 0x183C34AA0.
// Stack (untruncated TD): C_ObjectDatabaseSortedIdIndexed<C_ObjectTableDatabase<S_Perk,
// S_PerkDBData>, CryGUID> -> ... -> I_ObjectDatabase.
// GLOBAL STATIC instance @0x18532E150 (ctor sub_18188ED28: table "perk", group "rpg") -- this IS
// the "perk definition registry" that C_Perk::Resolve reads (qword_18532E150).
// Rows: parsed S_Perk (0x78) built from the raw 120-byte table rows by ParseRows [10]
// (sub_18115C994, converter sub_1809409B8); Register [7] passes rowStride 120; GetRowCount [4]
// divides the +0x30 vector extent by 120.
// Concrete tail: +0xB0..+0xDF ctor-zeroed (three 0x10 blocks / two vectors -- shape UNRESOLVED;
// no unordered_map fingerprint, unlike C_ReputationChangeDatabase).

namespace wh::rpgmodule {

// Per-row DB payload type of the table template (second arg). Opaque -- not RE'd.
struct S_PerkDBData;

class C_PerkDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_Perk, S_PerkDBData>,
          CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkDatabase;
    // Returns the global static instance @RVA 0x532E150. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkDatabase* GetInstance();

    uint64_t m_unkB0[6];   // +0xB0..+0xDF  ctor-zeroed index storage [shape UNRESOLVED]
};
static_assert(sizeof(C_PerkDatabase) == 0xE0, "C_PerkDatabase must be 0xE0 (ctor sub_18188ED28 write extent)");
static_assert(offsetof(C_PerkDatabase, m_tableName) == 0x50, "table name at 0x50");

}  // namespace wh::rpgmodule
