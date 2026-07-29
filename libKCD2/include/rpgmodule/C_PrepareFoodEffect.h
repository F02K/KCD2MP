#pragma once
#include <cstdint>
#include "C_PrepareItemEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_PrepareFoodEffect -- cook/prepare food (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E980  vtable 0x18472F258.  sizeof 0x50 (PROVEN: factory sub_182CD4280
// operator new(80)).  Ctor (owner, a3, &itemVec, a5, a6): base C_PrepareItemEffect(owner,
// itemVec), then +0x40 = a3, +0x48 = (byte)a5, +0x4C = a6.

namespace wh::rpgmodule {

class C_PrepareFoodEffect : public C_PrepareItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PrepareFoodEffect;
    uint64_t m_param40;   // +0x40  ctor arg 3 [role UNVERIFIED -- WUID/pointer width]
    uint8_t  m_flag48;    // +0x48  ctor arg 5 (byte) [role UNVERIFIED]
    uint8_t  _pad49[3];   // +0x49
    int32_t  m_param4C;   // +0x4C  ctor arg 6 [role UNVERIFIED]
};
static_assert(sizeof(C_PrepareFoodEffect) == 0x50, "C_PrepareFoodEffect must be 0x50 (operator new(80))");

}  // namespace wh::rpgmodule
