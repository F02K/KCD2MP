#pragma once
#include <cstdint>
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulBleedingHealEffect -- heal bleeding wounds (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D60078  vtable 0x183BE9638.  sizeof 0x20 (PROVEN: factory sub_1816853FC
// operator new(32)).  Ctor (owner, targetWuid, a4, &a5) -- both int payloads.
// (Bleeding lives in C_BleedingBuffInstance; this effect drives its heal path.)

namespace wh::rpgmodule {

class C_SoulBleedingHealEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulBleedingHealEffect;
    int32_t m_param18;   // +0x18  ctor arg 4 [role UNVERIFIED]
    int32_t m_param1C;   // +0x1C  ctor arg 5 deref [role UNVERIFIED]
};
static_assert(sizeof(C_SoulBleedingHealEffect) == 0x20,
              "C_SoulBleedingHealEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
