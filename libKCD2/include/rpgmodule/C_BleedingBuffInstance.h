#pragma once
#include <cstdint>
#include "C_ConstantSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_BleedingBuffInstance : C_ConstantSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xE0.
// -----------------------------------------------
// RTTI .?AVC_BleedingBuffInstance@rpgmodule@wh@@  make sub_18182A010 (alloc 224 = 0xE0,
// C_ConstantSoulBuffInstance ctor sub_182BC515C, then dword@0xD8 = 0x3F800000 = 1.0f).
// The bleeding damage-over-time buff; representative of the member-adding 0xE0 leaf bucket.
// +0xD8 semantics: only the 1.0f ctor default is proven; "intensity/rate" name is INFERRED.

namespace wh::rpgmodule {

class C_BleedingBuffInstance : public C_ConstantSoulBuffInstance   // +0x00  (0xD8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BleedingBuffInstance;
    float   m_intensity;   // +0xD8  ctor default 1.0f (bleed intensity/rate -- name INFERRED)
    uint8_t _padDC[4];     // +0xDC
};
static_assert(sizeof(C_BleedingBuffInstance) == 0xE0, "C_BleedingBuffInstance must be 0xE0");
static_assert(offsetof(C_BleedingBuffInstance, m_intensity) == 0xD8, "intensity at 0xD8");

}  // namespace wh::rpgmodule
