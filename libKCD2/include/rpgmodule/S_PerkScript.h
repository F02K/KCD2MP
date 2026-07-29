#pragma once
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkScript -- perk_script table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: C_PerkScriptDatabase ParseRows 0x181418B80, blob stride 24:
// {16B @0, string-ref qword @16}; the string ref is materialized (sub_180942380) and
// stored as a CryString at row +0x10; vector advance += 24).

namespace wh::rpgmodule {

struct S_PerkScript {
    CryGUID m_perkId;             // +0x00  [role INFERRED; sort-key candidate]
    CryStringT<char> m_script;    // +0x10  script name/body ref from the table blob
};
static_assert(sizeof(S_PerkScript) == 0x18, "S_PerkScript must be 0x18 (ParseRows stride 24)");

}  // namespace wh::rpgmodule
