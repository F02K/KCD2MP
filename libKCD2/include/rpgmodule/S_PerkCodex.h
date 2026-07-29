#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkCodex -- perk_codex table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: C_PerkCodexDatabase ParseRows 0x18130656C + row builder sub_181306660;
// blob stride 48: {16B @0, dword @16, dword @20, strref @24, strref @32, byte @40, dword @44};
// both strings are materialized + normalized (sub_180942380 / sub_1804F3344)).

namespace wh::rpgmodule {

struct S_PerkCodex {
    CryGUID  m_perkId;            // +0x00  [role INFERRED; sort-key candidate]
    int32_t  m_param10;           // +0x10  [role UNVERIFIED]
    int32_t  m_param14;           // +0x14  [role UNVERIFIED]
    CryStringT<char> m_str18;     // +0x18  normalized [codex entry name candidate]
    CryStringT<char> m_str20;     // +0x20  normalized [role UNVERIFIED]
    uint8_t  m_flag28;            // +0x28  [role UNVERIFIED]
    uint8_t  _pad29[3];           // +0x29
    int32_t  m_param2C;           // +0x2C  [role UNVERIFIED]
};
static_assert(sizeof(S_PerkCodex) == 0x30, "S_PerkCodex must be 0x30 (ParseRows stride 48)");

}  // namespace wh::rpgmodule
