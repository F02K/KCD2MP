#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionGuardMovementParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// POD parameter block embedded by value at +0x50 inside C_CombatActorActionGuardMovement (via
// C_ParamAction<...>). Size PROVEN 0x08 by layout (C_CombatActorObject base follows at +0x58). No
// standalone RTTI TD (pure POD). Zero-initialised; field decomposition UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionGuardMovementParams {
    uint64_t m_data;            // +0x00  opaque 8-byte payload (zero-init; semantics UNVERIFIED)
};
static_assert(sizeof(S_CombatActorActionGuardMovementParams) == 0x08);

}  // namespace wh::combatmodule
