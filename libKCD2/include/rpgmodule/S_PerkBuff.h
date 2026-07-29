#pragma once
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkBuff -- perk_buff table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: C_PerkBuffDatabase ParseRows 0x180B470B4 copies blob rows with stride
// 32 -- two 16-byte fields).  The same folded ParseRows serves the Location2Perk /
// POIType2Perk / Perk2PerkExclusivity / PerkCombatTechnique databases (identical row shape).
// Field types CryGUID [INFERRED: 16-byte width + the stack's sorted-index key type CryGUID];
// field roles by table name [INFERRED -- perk_buff maps perk -> buff].

namespace wh::rpgmodule {

struct S_PerkBuff {
    CryGUID m_perkId;   // +0x00  [role INFERRED; sort-key candidate]
    CryGUID m_buffId;   // +0x10  [role INFERRED]
};
static_assert(sizeof(S_PerkBuff) == 0x20, "S_PerkBuff must be 0x20 (ParseRows stride 32)");

}  // namespace wh::rpgmodule
