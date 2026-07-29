#pragma once
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_PerkDefinitionId -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Perk identity value type (perk table rows). RTTR-proven: string type_converters exist
// (.?AU?$type_converter@US_PerkDefinitionId...), and every C_Perk ctor inits the id member
// @+0x10 with the 16-byte invalid constant xmmword_18409A0C0 (the invalid-id constant family
// next to S_LocationId's 0x18409A0B0). C_Perk::Resolve looks the id up in the global perk
// definition registry qword_18532E150; the secondary index qword_18532DD88 (32-byte rows) is
// searched by it too.
// RESOLVED (was "interior tiling UNVERIFIED"): S_PerkDefinitionId IS a 128-bit GUID -- the two qwords
// are the hi/lo halves of a CryGUID {uint64 hipart, uint64 lopart}, parsed from the perk table's
// `perk_id` column, which holds literal GUID strings, e.g.
// "00a12397-0ee3-41ed-ab54-3303b049019a" (codex_soc_cech_zivot). That is exactly what the RTTR
// type_converter to/from std::string does (the GUID<->string parse); compares treat it as 16 raw
// bytes, invalid = xmmword_18409A0C0 (invalid-id constant family next to S_LocationId's 0x18409A0B0).
//
// The binary registers a DISTINCT RTTR type named "S_PerkDefinitionId" (its own string converter), but
// the layout and value semantics are exactly CryGUID, so we keep the binary name as a typedef rather
// than duplicate the type.

namespace wh::rpgmodule {

using S_PerkDefinitionId = ::CryGUID;   // 128-bit perk GUID (perk table `perk_id`)

}  // namespace wh::rpgmodule

static_assert(sizeof(wh::rpgmodule::S_PerkDefinitionId) == 0x10, "S_PerkDefinitionId (= CryGUID) must be 0x10");
