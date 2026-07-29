#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseSortedIdIndexed.h"
#include "../framework/HashPrimitives.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkCombatTechniqueDatabase -- the perk_combat_technique table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xF0.
// -----------------------------------------------
// RTTI TD 0x184D38710  vtable 0x183AE4F10.  Stack: C_ObjectDatabaseSortedIdIndexed<
// C_ObjectTableDatabase<S_PerkCombatTechnique, S_PerkCombatTechniqueDBData>, CryGUID>
// [key INFERRED].  GLOBAL STATIC @0x18532DFB0 (ctor sub_180F821D0: table "perk_combat_technique",
// group "rpg"). Like C_ReputationChangeDatabase it carries its OWN unordered_map @+0xB0
// (stock MSVC fingerprint, mask/maxidx pair at +0xE0).

namespace wh::rpgmodule {

struct S_PerkCombatTechnique;         // row type -- not RE'd
struct S_PerkCombatTechniqueDBData;   // per-row DB payload -- not RE'd

class C_PerkCombatTechniqueDatabase
    : public wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_PerkCombatTechnique, S_PerkCombatTechniqueDBData>, CryGUID>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkCombatTechniqueDatabase;
    // Global static instance @RVA 0x532DFB0. Impl in src/databasemodule/databasemodule.cpp.
    static C_PerkCombatTechniqueDatabase* GetInstance();

    // +0xB0  id -> row lookup [key/value typing INFERRED]
    std::unordered_map<CryGUID, uint64_t, wh::shared::S_DefaultHash<CryGUID>> m_idIndex;
};
static_assert(sizeof(C_PerkCombatTechniqueDatabase) == 0xF0,
              "C_PerkCombatTechniqueDatabase must be 0xF0 (ctor sub_180F821D0 write extent)");
static_assert(offsetof(C_PerkCombatTechniqueDatabase, m_idIndex) == 0xB0, "id index at 0xB0");

}  // namespace wh::rpgmodule
