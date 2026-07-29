#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionGuardVarParams -- KCD2-NEW.  WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// [KCD2-NEW] No KCD1 counterpart -- the C_CombatActorActionGuardVar leaf is new in KCD2.
// POD parameter block embedded by value at +0x50 inside C_CombatActorActionGuardVar (via
// C_ParamAction<...>). Size PROVEN 0x08 by layout (C_CombatActorObject base follows at +0x58) and by
// the base ctor sub_1810F2F9C which writes a single qword 0 to +0x50. No standalone RTTI TD (pure
// POD). Field decomposition UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionGuardVarParams {
    uint64_t m_data;            // +0x00  opaque 8-byte payload (zero-init; semantics UNVERIFIED)
};
static_assert(sizeof(S_CombatActorActionGuardVarParams) == 0x08);

}  // namespace wh::combatmodule
