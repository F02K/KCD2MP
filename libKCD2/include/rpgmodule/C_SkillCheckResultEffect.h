#pragma once
#include <cstdint>
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_SkillCheckResultEffect -- report a skill-check outcome
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D40EC0  vtable 0x183F1A6E0.  sizeof 0x20 (PROVEN: factory sub_182CD4648
// operator new(32)).  Ctor (owner, targetWuid, a4:float, a5:int).

namespace wh::rpgmodule {

class C_SkillCheckResultEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkillCheckResultEffect;
    float   m_param18;   // +0x18  ctor arg 4 [role UNVERIFIED -- check magnitude candidate]
    int32_t m_param1C;   // +0x1C  ctor arg 5 [role UNVERIFIED -- skill id candidate]
};
static_assert(sizeof(C_SkillCheckResultEffect) == 0x20,
              "C_SkillCheckResultEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
