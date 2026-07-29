#pragma once
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_Location2Perk -- location2perk table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: shared folded ParseRows 0x180B470B4, stride 32 -- two 16-byte fields;
// see S_PerkBuff).  Types/roles [INFERRED].

namespace wh::rpgmodule {

struct S_Location2Perk {
    CryGUID m_locationId;   // +0x00  [role INFERRED]
    CryGUID m_perkId;       // +0x10  [role INFERRED]
};
static_assert(sizeof(S_Location2Perk) == 0x20, "S_Location2Perk must be 0x20 (ParseRows stride 32)");

}  // namespace wh::rpgmodule
