#pragma once
#include <cstdint>
#include "C_SoulValueEffect.h"
#include "E_SoulState.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulStateEffect -- change a continuous soul state (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CEA4D8  vtable 0x183C63148.  sizeof 0x30 (PROVEN: factory sub_1819A21A4
// operator new(48)).  C_SoulValueEffect<E_SoulState, float> -> C_SoulEffect -> C_Effect.
// Ctor (owner, targetWuid, stateId, amountBits). The typed subclasses (C_HealthValueEffect etc.)
// preset m_valueId -- see E_SoulState.h.

namespace wh::rpgmodule {

class C_SoulStateEffect : public C_SoulValueEffect<E_SoulState, float> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulStateEffect;
    // (m_valueId @+0x18 = E_SoulState, m_amount @+0x1C = float delta from the base template)
    float    m_capturedValue;    // +0x20  writer 0x1814407F4: snapshot of soul state[valueId] (soul+0x708+4*id); float per C_SoulValueEffect<E_SoulState,float>
    float    m_effectiveAmount;  // +0x24  writer 0x1814407F4: sub_18046EA68 clamp(amount) -> float (fminf/fmaxf) [field roles inferred]
    uint8_t  m_sourceTag;  // +0x28  ctor 0; damage pipeline writes a producer tag on the STAMINA effect
                           //        instance via sub_180726FD0 @0x180726FDC (4 melee/missile, 7 collision,
                           //        10 block-chip; melee core overwrites 5 blocked / 6 flag154
                           //        @0x180C6433A/0x180C6431E); role for other effect kinds UNVERIFIED
    uint8_t  _pad29[7];    // +0x29
};
static_assert(sizeof(C_SoulStateEffect) == 0x30, "C_SoulStateEffect must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
