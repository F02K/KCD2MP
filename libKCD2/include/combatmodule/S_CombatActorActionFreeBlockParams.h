#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionFreeBlockParams -- params of C_CombatActorActionFreeBlock
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.  PLAIN POD (no RTTI TypeDescriptor / no vtable).
// -----------------------------------------------
// Not polymorphic.  Embedded by value at +0x50 of the action; the C_CombatActorObject base begins at
// +0x58, proving the params block is exactly 8 bytes.  The action base ctor sub_182753C68 does not write
// any params field (left zero by the allocator), so the payload is a single zero-initialised qword.
// Field semantics UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionFreeBlockParams {
    uint64_t m_data;         // +0x00  init 0  (UNVERIFIED)
};
static_assert(sizeof(S_CombatActorActionFreeBlockParams) == 0x08,
              "S_CombatActorActionFreeBlockParams POD, 8 bytes");

}  // namespace wh::combatmodule
