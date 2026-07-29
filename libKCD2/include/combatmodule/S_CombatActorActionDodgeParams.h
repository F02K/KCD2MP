#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionDodgeParams -- params of C_CombatActorActionDodge
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x30.  PLAIN POD (no RTTI TypeDescriptor / no vtable).
// -----------------------------------------------
// KCD2-NEW (no KCD1 counterpart).  Embedded by value at +0x50 of the Dodge action; the
// C_CombatActorObject base begins at +0x80, proving the 0x30 span.
// Layout from the params ctor sub_18090A8E4:  qword@+0x00 = 0, word@+0x08 = 0, dword@+0x0C = 0, then a
// 0x20-byte sub-struct at +0x10 initialised by sub_18090B2A4 (the SAME 0x20 motion/state sub-struct is
// re-used inside the Dodge action leaf at +0xA0).  The whole 0x30 block is bulk-copied from a caller
// argument by the factory sub_181482FAC (three OWORD stores).  Field semantics UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionDodgeParams {
    uint64_t m_field00;      // +0x00
    uint16_t m_field08;      // +0x08
    uint8_t  _pad0A[2];      // +0x0A
    uint32_t m_field0C;      // +0x0C

    // ---- embedded 0x20 motion/state sub-struct (ctor sub_18090B2A4) ----
    uint64_t m_sub10;        // +0x10
    uint32_t m_sub18[3];     // +0x18  (3 dwords; UNVERIFIED -- possibly Vec3)
    uint32_t m_sub24[2];     // +0x24  (2 dwords; UNVERIFIED -- possibly Vec2)
    uint8_t  m_sub2C;        // +0x2C
    uint8_t  _pad2D[3];      // +0x2D
};
static_assert(sizeof(S_CombatActorActionDodgeParams) == 0x30,
              "S_CombatActorActionDodgeParams POD, 0x30");

}  // namespace wh::combatmodule
