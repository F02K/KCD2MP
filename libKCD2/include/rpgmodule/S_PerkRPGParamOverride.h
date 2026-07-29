#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkRPGParamOverride -- perk_rpg_param_override table row
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: C_PerkRPGParamOverrideDatabase ParseRows 0x1815168F8, blob stride 32:
// {16B @0, string-ref qword @16, dword @24}; the string ref is materialized to a CryString
// and post-processed by sub_1804F3344 (case/hash normalize) into row +0x10; vector advance
// += 32).

namespace wh::rpgmodule {

struct S_PerkRPGParamOverride {
    CryGUID m_perkId;               // +0x00  [role INFERRED; sort-key candidate]
    CryStringT<char> m_paramName;   // +0x10  rpg_param row name (normalized)
    int32_t m_param18;              // +0x18  blob dword @24 [override value bits UNVERIFIED]
    uint32_t _pad1C;                // +0x1C
};
static_assert(sizeof(S_PerkRPGParamOverride) == 0x20,
              "S_PerkRPGParamOverride must be 0x20 (ParseRows stride 32)");

}  // namespace wh::rpgmodule
