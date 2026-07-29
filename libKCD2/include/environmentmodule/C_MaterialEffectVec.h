#pragma once
#include <cstdint>
#include "C_VisualEffect.h"

// -----------------------------------------------
// wh::environmentmodule::C_MaterialEffectVec -- material vector-param visual effect
// (KCD2 1.5.6, kd7u).  modeled sizeof 0x40 [alloc UNPROVEN -- caller-allocated].
// -----------------------------------------------
// Own vtable 0x183A70DD8; ctor sub_180AB5D60. Binds a named material vector parameter and
// caches the default RGB (same globals as C_LightEffectVec) at +0x30. The ctor writes at
// +0x24 (qword) / +0x2C (dword) are misaligned relative to each other -- that span is kept
// as raw bytes until the field split is walked.

namespace wh::environmentmodule {

class C_MaterialEffectVec : public C_VisualEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MaterialEffectVec;
    CryStringT<char> m_materialParam;   // +0x10  material/param name (sub_18097D174 init)
    int32_t  m_param18;                 // +0x18  [role UNVERIFIED]
    uint32_t _pad1C;                    // +0x1C
    int32_t  m_param20;                 // +0x20  [role UNVERIFIED]
    uint8_t  m_raw24[0x0C];             // +0x24..+0x2F  ctor qword@+0x24 + dword@+0x2C (split not walked)
    Vec3     m_colorDefault;            // +0x30  default RGB (globals 0x1849302A0/+8)
    uint32_t _pad3C;                    // +0x3C
};
static_assert(sizeof(C_MaterialEffectVec) == 0x40, "modeled extent (ctor field extent; alloc UNPROVEN)");

}  // namespace wh::environmentmodule
