#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include "S_ReputationChange.h"
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../framework/HashPrimitives.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_ReputationChangeDatabase -- the reputation_change table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xF0.
// -----------------------------------------------
// RTTI .?AVC_ReputationChangeDatabase@rpgmodule@wh@@ (TD 0x184D35B60)  vtable 0x183F18B10 (22 slots).
// Stack: C_ObjectDatabaseSortedIdIndexed<C_ObjectTableDatabase<S_ReputationChange,
// S_ReputationChangeDBData>, CryGUID> -> ... -> I_ObjectDatabase (id key = CryGUID, from the
// untruncated C_PerkDatabase sibling TD).
// GLOBAL STATIC instance @0x18532F700 (ctor sub_1811CEEB8: table "reputation_change", group
// "rpg"; registers itself with C_ObjectDatabaseManager). Rows are 0x28 S_ReputationChange.
// The unordered_map @+0xB0 is this CONCRETE class's own member (the ctor inits it with the
// stock MSVC fingerprint; sibling databases on the same template lack it).

namespace wh::rpgmodule {

// Per-row DB payload type of the table template (second arg). Opaque -- not RE'd.
struct S_ReputationChangeDBData;

class C_ReputationChangeDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_ReputationChange, S_ReputationChangeDBData>,
          CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReputationChangeDatabase;
    // Returns the global static instance @RVA 0x532F700 (already constructed at static-init).
    // Impl in src/databasemodule/databasemodule.cpp.
    static C_ReputationChangeDatabase* GetInstance();

    // +0xB0  id -> row lookup (32-byte nodes) [key/value typing INFERRED]
    std::unordered_map<CryGUID, uint64_t, wh::shared::S_DefaultHash<CryGUID>> m_idIndex;
};
static_assert(sizeof(C_ReputationChangeDatabase) == 0xF0,
              "C_ReputationChangeDatabase must be 0xF0 (ctor sub_1811CEEB8 write extent)");
static_assert(offsetof(C_ReputationChangeDatabase, m_tableName) == 0x50, "table name at 0x50");
static_assert(offsetof(C_ReputationChangeDatabase, m_idIndex) == 0xB0, "id index at 0xB0");

}  // namespace wh::rpgmodule
