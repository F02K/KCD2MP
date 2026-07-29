#pragma once
#include <cstdint>
#include "C_VisualEffect.h"

// -----------------------------------------------
// wh::environmentmodule::C_LightEffectVec -- light color/vector visual effect
// (KCD2 1.5.6, kd7u).  sizeof 0x38 (alloc 56).
// -----------------------------------------------
// Own vtable 0x183A70D68; ctor sub_180AB4998. Resolves a light parameter via
// sub_180AB4CCC / sub_181889B7C, then caches the default RGB (globals qword_1849302A0 /
// dword_1849302A8) at +0x28.

namespace wh::environmentmodule {

class C_LightEffectVec : public C_VisualEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LightEffectVec;
    uint64_t m_target10[3];   // +0x10..+0x27  copied _OWORD+qword ctor arg (light-param
                              //               target descriptor / name id) [not walked]
    Vec3     m_colorDefault;  // +0x28  default RGB (ctor copies globals 0x1849302A0/+8)
    uint32_t _pad34;          // +0x34
};
static_assert(sizeof(C_LightEffectVec) == 0x38, "C_LightEffectVec must be 0x38 (alloc 56)");

}  // namespace wh::environmentmodule
