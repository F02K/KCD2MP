#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionGuardParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// POD parameter block embedded by value inside C_CombatActorActionGuard via
// C_ParamAction<S_CombatActorActionGuardParams, C_Action<I_CombatActorActionPrivate>> at object
// offset +0x50. Size PROVEN 0x08 by layout: the next base subobject (C_CombatActorObject) sits at
// +0x58, so the embedded params occupy exactly +0x50..+0x58. No RTTI type descriptor exists for this
// struct (pure POD; not polymorphic), matching the KCD1 shape. The single 8-byte payload is
// zero-initialised at construction; its exact field decomposition is UNVERIFIED (opaque blob).

namespace wh::combatmodule {

struct S_CombatActorActionGuardParams {
    uint64_t m_data;            // +0x00  opaque 8-byte payload (zero-init; semantics UNVERIFIED)
};
static_assert(sizeof(S_CombatActorActionGuardParams) == 0x08, "params embed at +0x50, C_CombatActorObject at +0x58");

}  // namespace wh::combatmodule
