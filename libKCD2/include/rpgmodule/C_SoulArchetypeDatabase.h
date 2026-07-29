#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include "S_SoulArchetype.h"
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseIdIndexed.h"
#include "../databasemodule/I_DynamicEnum.h"
#include "../framework/HashPrimitives.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulArchetypeDatabase -- the soul_archetype table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x100.
// -----------------------------------------------
// RTTI .?AVC_SoulArchetypeDatabase@rpgmodule@wh@@ (TD 0x184D42EA8)  vtables: primary 0x183F1A730,
// I_DynamicEnum @+0xB8 0x183F1A7E8 (the enum view over the 0x58 S_SoulArchetype rows).
// Stack: C_ObjectDatabaseIdIndexed<C_ObjectTableDatabase<S_SoulArchetype, S_SoulArchetypeDBData>>
// + I_DynamicEnum declared HERE (the concrete ctor sub_1815A1290 writes the +0xB8 vfptr).
// GLOBAL STATIC instance @0x1853302B0 (table "soul_archetype", group "rpg").

namespace wh::rpgmodule {

// Per-row DB payload type of the table template (second arg). Opaque -- not RE'd.
struct S_SoulArchetypeDBData;

class C_SoulArchetypeDatabase
    : public wh::databasemodule::C_ObjectDatabaseIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_SoulArchetype, S_SoulArchetypeDBData>>
    , public wh::databasemodule::I_DynamicEnum   // +0xB8
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulArchetypeDatabase;
    // Returns the global static instance @RVA 0x53302B0. Impl in src/databasemodule/databasemodule.cpp.
    static C_SoulArchetypeDatabase* GetInstance();

    // +0xC0  ctor-inited stock MSVC unordered_map (56-byte nodes; key/value typing UNRESOLVED)
    std::unordered_map<uint64_t, uint64_t, wh::shared::S_DefaultHash<uint64_t>> m_mapC0;
};
static_assert(sizeof(C_SoulArchetypeDatabase) == 0x100,
              "C_SoulArchetypeDatabase must be 0x100 (ctor sub_1815A1290 write extent)");
static_assert(offsetof(C_SoulArchetypeDatabase, m_mapC0) == 0xC0, "map at 0xC0");

}  // namespace wh::rpgmodule
