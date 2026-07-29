#pragma once
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_Perk2PerkExclusivity -- perk2perk_exclusivity table row
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: shared folded ParseRows 0x180B470B4, stride 32 -- two 16-byte fields;
// see S_PerkBuff).  C_Perk::LookupIndexEntry searches this database's +0x98 sorted index by
// perk GUID.  Types/roles [INFERRED].

namespace wh::rpgmodule {

struct S_Perk2PerkExclusivity {
    CryGUID m_perkId;            // +0x00  [role INFERRED; sort-key candidate]
    CryGUID m_exclusivePerkId;   // +0x10  [role INFERRED]
};
static_assert(sizeof(S_Perk2PerkExclusivity) == 0x20,
              "S_Perk2PerkExclusivity must be 0x20 (ParseRows stride 32)");

}  // namespace wh::rpgmodule
