#pragma once
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_LocationId -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10 (PROVEN).
// -----------------------------------------------
// Location identity value type (location.xml rows). RTTI-proven usages: directly-stored member
// of C_Faction @+0x188 (RTTR property_wrapper member_object_ptr "LocationId"), RTTR
// type_converter to/from std::string (@0x184D529B0 / 0x184D53670), std::pair key
// (pair<S_LocationId, _smart_ptr<C_POI>>), and std::vector<S_LocationId> element.
// SIZE 0x10 PROVEN by the C_RPGLocationManager location cluster: 16-byte stride walks of the
// +0x268 id array (slot [7]) and of the POI-type registry rows, OWORD copies at every id
// handoff, and the unordered_map node layouts (value @node+0x18 after an 8-byte key / key
// @node+0x10 spanning 16 bytes).
// RESOLVED (was "interior tiling UNVERIFIED"): S_LocationId IS a 128-bit GUID -- the two qwords are
// the hi/lo halves of a CryGUID {uint64 hipart, uint64 lopart}, parsed from location.xml's
// `location_id` column, which holds literal GUID strings, e.g.
// "0855c91f-2ce2-413f-b16a-113754d2cebe" (location_cikanskyTabor). That is exactly what the RTTR
// type_converter to/from std::string (@0x184D529B0 / 0x184D53670) does -- the GUID<->string parse --
// and why compares/hash treat it as 16 raw bytes (invalid/none = all-zero xmmword_183A38AF0).
//
// The binary registers a DISTINCT RTTR type named "S_LocationId" (its own string converter), but the
// layout and value semantics are exactly CryGUID, so we keep the binary name as a typedef rather than
// duplicate the type. std::hash is provided the same way the game's GUID maps get it:
// wh::shared::S_DefaultHash<S_LocationId> (FNV-1a over the 16 raw bytes).

namespace wh::rpgmodule {

using S_LocationId = ::CryGUID;   // 128-bit location GUID (location.xml `location_id`)

}  // namespace wh::rpgmodule

static_assert(sizeof(wh::rpgmodule::S_LocationId) == 0x10, "S_LocationId (= CryGUID) must be 0x10");
