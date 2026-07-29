#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::entitymodule::C_ActorBounds -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x3C.
// -----------------------------------------------
// Embedded at C_Actor+0x600 (ctor sub_181237C88). The +/-1e6 / -1000 sentinels do NOT form a clean
// reset AABB {Vec3 min, Vec3 max} (the Y extents invert), so the floats are presented raw --
// interpretation unresolved. dtor helper sub_1823CEFFC.

namespace wh::entitymodule {

class C_ActorBounds {
public:
    int32_t m_field00;    // +0x00  (ctor 0)
    int32_t m_field04;    // +0x04
    Vec3    m_vec08;      // +0x08  init (0,0,0) (center/offset candidate)
    int32_t m_field14;    // +0x14
    float   m_ext18;      // +0x18  init -1000000.0
    float   m_ext1C;      // +0x1C  init 0.0
    float   m_ext20;      // +0x20  init -1000000.0
    float   m_ext24;      // +0x24  init +1000000.0
    float   m_ext28;      // +0x28  init -1000.0
    float   m_ext2C;      // +0x2C  init 0.0
    float   m_ext30;      // +0x30  init -1000000.0
    uint16_t m_flags34;   // +0x34  (ctor 0)
    uint8_t  m_flag36;    // +0x36
    uint8_t  _pad37;      // +0x37
    uint32_t m_field38;   // +0x38  registration handle (u16 slot index + u16 generation); dtor sub_1815BB2CC de-registers via sub_1803ECF9C when != 0
};
static_assert(sizeof(C_ActorBounds) == 0x3C, "C_ActorBounds must be 0x3C");

}  // namespace wh::entitymodule
