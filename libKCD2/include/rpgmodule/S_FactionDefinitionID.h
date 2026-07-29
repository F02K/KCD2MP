#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_FactionDefinitionID -- KCD2 WHGame.dll 1.5.6 (kd7u).  LAYOUT UNRESOLVED.
// -----------------------------------------------
// Faction definition-ID handle. RTTI-proven to EXIST (rttr type name @0x183CE3430; pointer form
// @0x183F075B0; wrapped in C_TypedPortRef @0x183CD4CC0) and appears in the manager surface
// (RTTR GetFaction(const S_FactionDefinitionID&)). Sibling definition-ID handles in the same
// family: S_AngrinessFlagDefinitionID (@0x183CE52A0), S_AngrinessChangeDefinitionID
// (@0x183CE5310; also as std::shared_ptr<...>). These are lightweight database-layer id
// structs, distinct from the runtime C_Faction / C_FactionAngriness objects.
//
// CORRECTION (supersedes this header's earlier revision): the previously-claimed 0x18 layout
// {bool m_hasUid, uint64 m_uid, int m_index} embedded at C_Faction+0xD0 was a MISATTRIBUTION.
// The RTTR registrar pins that region as boost::optional<uint64_t> "OverrideMemberNumber"
// (+0xD0), float "InitialReputation" (+0xE0), int "NumChildsCache" (+0xE4) -- the old "id
// getter" sub_1803F30C4 is actually the member-number reader (override-or-child-count). That
// also resolves the overlap caveat the old revision recorded. No independent evidence for this
// struct's interior remains, so it is declared without a body until its own ctor/uses are RE'd.

namespace wh::rpgmodule {

struct S_FactionDefinitionID;   // interior layout pending (see correction note above)

}  // namespace wh::rpgmodule
