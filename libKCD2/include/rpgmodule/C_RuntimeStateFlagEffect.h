#pragma once
#include <cstdint>
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_RuntimeStateFlagEffect -- set/clear a soul runtime state flag
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D60260  vtable 0x183A8AB28.  sizeof 0x20 (PROVEN: factory sub_180C5C484
// operator new(32)).  Ctor (owner, targetWuid, flagId, bSet).

namespace wh::rpgmodule {

class C_RuntimeStateFlagEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RuntimeStateFlagEffect;
    int32_t m_flagId;   // +0x18  runtime state flag id [flag table not enumerated]
    uint8_t m_bSet;     // +0x1C  set vs clear
    uint8_t _pad1D[3];  // +0x1D
};
static_assert(sizeof(C_RuntimeStateFlagEffect) == 0x20, "C_RuntimeStateFlagEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
