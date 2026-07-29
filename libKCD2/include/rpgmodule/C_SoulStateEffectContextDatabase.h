#pragma once
#include "../databasemodule/C_ObjectTreeDatabase.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "../databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulStateEffectContextDatabase -- soul-state effect context registry
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D423F0  vtable 0x183AE72C0.  GLOBAL STATIC @0x185330430 (ctor sub_180F9F344 ->
// sub_180F9F018 -> tree ctor sub_180F9FCB4).  Registered by CLASS NAME (ns "wh::rpgmodule::",
// name "SoulStateEffectContext", group "rpg") -- the tree-database registration style, unlike
// the table databases.  Stack: C_ObjectDatabaseKeyIndexed<C_ObjectTreeDatabase<
// C_SoulStateEffectContextData, std::vector>, C_ObjectDatabaseDefaultKeyExtractor<...>>.
// Adds no members of its own (ctor writes only the vftable).  TOTAL SIZE UNVERIFIED (global
// static, no allocation site; the KeyIndexed layer's index storage is zero-init BSS) --
// do not sizeof this.

namespace wh::rpgmodule {

class C_SoulStateEffectContextData;   // row type -- not RE'd

class C_SoulStateEffectContextDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<C_SoulStateEffectContextData>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_SoulStateEffectContextData>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulStateEffectContextDatabase;
    // Global static instance @RVA 0x5330430. Impl in src/databasemodule/databasemodule.cpp.
    static C_SoulStateEffectContextDatabase* GetInstance();
};

}  // namespace wh::rpgmodule
