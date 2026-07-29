#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionPoseModifierParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// POD parameter block embedded by value at +0x50 inside C_CombatActorActionPoseModifier (via
// C_ParamAction<...>). Size PROVEN 0x08 by layout (C_CombatActorObject base follows at +0x58) and by
// the base ctor sub_18090D1F4 which writes a single qword 0 to +0x50. No standalone RTTI TD (pure
// POD). Field decomposition UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionPoseModifierParams {
    uint64_t m_data;            // +0x00  opaque 8-byte payload (zero-init; semantics UNVERIFIED)
};
static_assert(sizeof(S_CombatActorActionPoseModifierParams) == 0x08);

}  // namespace wh::combatmodule
