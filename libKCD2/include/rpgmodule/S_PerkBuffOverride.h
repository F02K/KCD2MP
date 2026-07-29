#pragma once
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkBuffOverride -- perk_buff_override table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: C_PerkBuffOverrideDatabase ParseRows 0x1816D4328 copies blob rows with
// stride 48 -- three 16-byte fields).  Types/roles [INFERRED -- a perk overriding one buff
// with another].

namespace wh::rpgmodule {

struct S_PerkBuffOverride {
    CryGUID m_perkId;           // +0x00  [role INFERRED; sort-key candidate]
    CryGUID m_buffId;           // +0x10  [role INFERRED]
    CryGUID m_overrideBuffId;   // +0x20  [role INFERRED]
};
static_assert(sizeof(S_PerkBuffOverride) == 0x30,
              "S_PerkBuffOverride must be 0x30 (ParseRows stride 48)");

}  // namespace wh::rpgmodule
