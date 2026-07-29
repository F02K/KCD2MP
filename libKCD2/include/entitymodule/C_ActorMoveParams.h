#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::entitymodule::C_ActorMoveParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x84.
// -----------------------------------------------
// Embedded at C_Actor+0x57C (ctor sub_1810F44DC; ends exactly at +0x600). Byte-exact from the
// ctor; semantic names inferred (m_maxSpeed = 80.0 matches the CMovementRequest sibling; no
// genuine C_Actor-context reader could be isolated -- the absolute offsets collide heavily).

namespace wh::entitymodule {

class C_ActorMoveParams {
public:
    Vec3    m_vecA;         // +0x00  (dir/vel; names unresolved)
    Vec3    m_vecB;         // +0x0C
    Vec3    m_vecC;         // +0x18
    Vec3    m_vecD;         // +0x24
    Vec3    m_vecE;         // +0x30
    Vec3    m_vecF;         // +0x3C
    int32_t m_field48;      // +0x48  (ctor 0)
    int32_t m_field4C;      // +0x4C
    float   m_ratio50;      // +0x50  init 1.0
    float   m_field54;      // +0x54  init 0
    int32_t m_field58;      // +0x58
    float   m_maxSpeed;     // +0x5C  init 80.0 (abs C_Actor+0x5D8)
    float   m_ratio60;      // +0x60  init -1.0 (sentinel)
    int32_t m_field64;      // +0x64
    int32_t m_field68;      // +0x68
    int32_t m_field6C;      // +0x6C
    int32_t m_id70;         // +0x70  init -1 (invalid-id sentinel)
    int32_t m_id74;         // +0x74  init -1
    int32_t m_id78;         // +0x78  init -1
    int32_t m_field7C;      // +0x7C
    int32_t m_field80;      // +0x80
};
static_assert(sizeof(C_ActorMoveParams) == 0x84, "C_ActorMoveParams must be 0x84");

}  // namespace wh::entitymodule
