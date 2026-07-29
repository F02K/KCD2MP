#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"   // Vec3 / Ang3 / Quat

// -----------------------------------------------
// wh::entitymodule::S_MountAnimState -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70 (POD).
// -----------------------------------------------
// The per-frame movement/animation REQUEST produced by wh::entitymodule::C_ActorMovementController
// (vtable 0x183E74AC0, getter slot +0x68 = sub_1804B8E88(this, float dt, OUT*)). C_Horse caches a
// verbatim copy of it as a member at +0x9F0 (writer C_Horse vtable slot 2152 = sub_1806CF62C copies
// 7 OWORDs). Flow: C_Actor::sub_181E7D6F0 zero-inits (sub_180A709E8), fills via [[this+0x180]+0x68],
// dispatches to CryMannequin as CryFixedArray<SStateEventData,5>, then hands to the C_Horse cache.
//
// The field semantics were resolved in the backlog deep map (verify-confirmed by re-deriving each
// scalar from an independent consumer: getter sub_1804B8E88, reader sub_1804415E0, consumer
// sub_180640950). The name "S_MountAnimState" is a KCD1 carry-over (the type has no RTTI); a more
// accurate name would be S_ActorMoveAnimRequest -- kept as-is per the port's naming continuity.

namespace wh::entitymodule {

struct S_MountAnimState {
    Vec3 m_extraMove;        // +0x00  extra movement contribution (accumulated from ctrl+376)
    Vec3 m_desiredVelocity;  // +0x0C  linear velocity (built + normalized, += ctrl+140)
    Ang3 m_aimAngles;        // +0x18  {pitch = asinf @+0x18, roll @+0x1C ~0, yaw = atan2f @+0x20}
    Vec3 m_moveDirection;    // +0x24  normalized; valid iff m_hasMoveDirection
    Vec3 m_aimDirection;     // +0x30  normalized (logged "aimDirection"); valid iff m_hasAimDirection
    // --- 32B "additional movement" sub-block, raw-copied from C_ActorMovementController+284 ---
    Quat m_rootRotation;     // +0x3C  root-motion rotation (raw copy of ctrl+284; MED)
    Vec3 m_rootTranslation;  // +0x4C  root-motion translation (x@+0x4C, y@+0x50, z@+0x54)
    int16_t m_rootMeta;      // +0x58  root-motion block metadata (id/type; LOW)
    uint8_t m_rootCount;     // +0x5A  additional-movement entry count / valid flag (validator sub_180E97DC4)
    uint8_t _pad5B;          // +0x5B
    // --- flags ---
    bool m_hasMoveDirection; // +0x5C  (getter sets 1; consumer gates on it)
    bool m_hasAimDirection;  // +0x5D  (getter sets 1)
    uint8_t m_flag5E;        // +0x5E  from movement-controller predicate sub_1823CE124 (MED)
    uint8_t _pad5F;          // +0x5F
    uint64_t m_unk60;        // +0x60  zero-init only (sub_180A709E8 @a1+96=0); producer getter sub_1804B8E88 never writes a3+96; type UNRESOLVED (WUID hypothesis unproven)
    uint8_t m_flag68;        // +0x68  allow/animForced from predicate sub_1823CE130 (MED)
    uint8_t _pad69[7];       // +0x69  tail padding to 0x70
};
static_assert(sizeof(S_MountAnimState) == 0x70, "S_MountAnimState must be 0x70");
static_assert(offsetof(S_MountAnimState, m_rootRotation) == 0x3C, "root rotation at 0x3C");
static_assert(offsetof(S_MountAnimState, m_hasMoveDirection) == 0x5C, "move-dir flag at 0x5C");
static_assert(offsetof(S_MountAnimState, m_unk60) == 0x60, "unk60 at 0x60");

}  // namespace wh::entitymodule
