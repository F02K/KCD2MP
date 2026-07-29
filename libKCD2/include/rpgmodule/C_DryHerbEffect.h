#pragma once
#include <cstdint>
#include "C_PrepareItemEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_DryHerbEffect -- dry herbs preparation (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E948  vtable 0x18472F2C8.  sizeof 0x50 (PROVEN: factory sub_182CD3FE0
// operator new(80)).  Ctor (owner, a3, &itemVec, a5): base C_PrepareItemEffect(owner, itemVec),
// then +0x40 = a3, +0x48 = a5.

namespace wh::rpgmodule {

class C_DryHerbEffect : public C_PrepareItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DryHerbEffect;
    uint64_t m_param40;   // +0x40  ctor arg 3 [role UNVERIFIED -- WUID/pointer width]
    int32_t  m_param48;   // +0x48  ctor arg 5 [role UNVERIFIED]
    // +0x4C 4-byte tail padding
};
static_assert(sizeof(C_DryHerbEffect) == 0x50, "C_DryHerbEffect must be 0x50 (operator new(80))");

}  // namespace wh::rpgmodule
