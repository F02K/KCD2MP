#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::entitymodule::C_StanceState -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x4C.
// -----------------------------------------------
// Embedded at C_Actor+0x508 (ctor sub_180705668). Size proven by two whole-struct block-copy
// writers (sub_1809D2134 fills it from an anim/movement source then sets the valid flag;
// sub_1809D1B4C constructs the default). Semantic names beyond the flag/quat are unresolved.

namespace wh::entitymodule {

class C_StanceState {
public:
    int32_t m_valid;          // +0x00  set to 1 by both writers after a (re)fill
    float   m_pair04[2];      // +0x04  mirrored to C_Actor+0x4E8 (role unresolved)
    float   m_scalar0C;       // +0x0C  mirrored to C_Actor+0x4F0
    Quat    m_orientation;    // +0x10  identity default {0,0,0,1}; mirrored to C_Actor+0x4F4
    uint8_t m_blockA[16];     // +0x20  copied wholesale from animSrc+60 (Vec3+pad vs Quat unresolved)
    uint8_t m_blockB[16];     // +0x30  copied wholesale from animSrc+76
    uint8_t m_stanceByte;     // +0x40  copied from animSrc+104
    uint8_t _pad41[3];        // +0x41
    int32_t m_tail44;         // +0x44  (ctor 0; role unresolved)
    int32_t m_tail48;         // +0x48
};
static_assert(sizeof(C_StanceState) == 0x4C, "C_StanceState must be 0x4C");

}  // namespace wh::entitymodule
