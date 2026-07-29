#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_NervousnessFlagDefinitionID -- KCD2 WHGame.dll 1.5.6 (kd7u).
// LAYOUT UNRESOLVED.
// -----------------------------------------------
// Nervousness-flag definition-ID handle (mirrors S_FactionDefinitionID).  No
// standalone class RTTI/vtable/COL exists; existence proven via rttr surfaces:
// type funcs f<S_NervousnessFlagDefinitionID> @0x183CE1D90 / pointer form
// @0x183F077E0; parameter_info_wrapper<const S_NervousnessFlagDefinitionID&> TD
// @0x184D00E70; dynamic-enum "wh::rpgmodule::NervousnessFlagID" @0x183F06320;
// rttr method I_Soul*(*)(const S_NervousnessFlagDefinitionID&) TD @0x184CFAF90.
// Indexes the nervousness_flag named-interval table (C_NamedIntervalDatabase,
// global 0x18532DA40).  Consumed by the concept node HasNervousnessFlag
// (ports Faction + NervousnessFlag -> Result, doc @0x183D2E160).  Sibling
// declare-only handles: S_FactionDefinitionID, S_AngrinessFlagDefinitionID,
// S_AngrinessChangeDefinitionID.

namespace wh::rpgmodule {

struct S_NervousnessFlagDefinitionID;   // interior layout pending (declare-only, see banner)

}  // namespace wh::rpgmodule
