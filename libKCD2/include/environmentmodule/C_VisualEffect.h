#pragma once
#include <cstdint>
#include "I_VisualEffect.h"

// -----------------------------------------------
// wh::environmentmodule::C_VisualEffect -- visual-effect base (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Abstract base of C_LightEffectVec / C_MaterialEffectVec / C_MaterialEffectFloat (no
// standalone COL -- appears only as a base subobject). Derived ctors stamp
// {uint32 0 @+0x08, uint8 valid-flag @+0x0C}.  sizeof 0x10 (PROVEN: all three derived
// classes place their first own member at +0x10).

namespace wh::environmentmodule {

class C_VisualEffect : public I_VisualEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_VisualEffect;
    uint32_t m_unk08;     // +0x08  dword init 0 in all 3 derived ctors (C_MaterialEffectVec 0x180AB5D60 / C_MaterialEffectFloat 0x180AB6040 / C_LightEffectVec 0x180AB4998); paired with m_valid; role UNVERIFIED
    uint8_t  m_valid;     // +0x0C  valid flag (set when the target param resolves)
    uint8_t  _pad0D[3];   // +0x0D
};
static_assert(sizeof(C_VisualEffect) == 0x10, "C_VisualEffect must be 0x10 (derived members at +0x10)");

}  // namespace wh::environmentmodule
