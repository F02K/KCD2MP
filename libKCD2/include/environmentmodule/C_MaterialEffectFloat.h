#pragma once
#include <cstdint>
#include "C_VisualEffect.h"

// -----------------------------------------------
// wh::environmentmodule::C_MaterialEffectFloat -- material float-param visual effect
// (KCD2 1.5.6, kd7u).  sizeof 0x30 (alloc 48).
// -----------------------------------------------
// Own vtable 0x183A70E10; ctor sub_180AB6040. Binds a shader/material float parameter
// (material vfuncs +368/+544) and stores the default value at +0x28.

namespace wh::environmentmodule {

class C_MaterialEffectFloat : public C_VisualEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MaterialEffectFloat;
    CryStringT<char> m_materialParam;   // +0x10  material/param name (sub_18097D174 init)
    int32_t  m_param18;                 // +0x18  [role UNVERIFIED]
    uint32_t _pad1C;                    // +0x1C
    int32_t  m_param20;                 // +0x20  [role UNVERIFIED]
    int32_t  m_param24;                 // +0x24  [role UNVERIFIED]
    float    m_valueDefault;            // +0x28  default float value
    uint32_t _pad2C;                    // +0x2C
};
static_assert(sizeof(C_MaterialEffectFloat) == 0x30, "C_MaterialEffectFloat must be 0x30 (alloc 48)");

}  // namespace wh::environmentmodule
