#pragma once
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkCombatTechnique -- perk_combat_technique table row
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: shared folded ParseRows 0x180B470B4, stride 32 -- two 16-byte fields;
// see S_PerkBuff).  Types/roles [INFERRED].

namespace wh::rpgmodule {

struct S_PerkCombatTechnique {
    CryGUID m_perkId;              // +0x00  [role INFERRED; sort-key candidate]
    CryGUID m_combatTechniqueId;   // +0x10  [role INFERRED]
};
static_assert(sizeof(S_PerkCombatTechnique) == 0x20,
              "S_PerkCombatTechnique must be 0x20 (ParseRows stride 32)");

}  // namespace wh::rpgmodule
