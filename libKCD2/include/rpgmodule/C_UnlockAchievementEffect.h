#pragma once
#include <cstdint>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_UnlockAchievementEffect -- unlock a platform achievement
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5FF20  vtable 0x184730670.  sizeof 0x18 (PROVEN: factory sub_182CD4B0C
// operator new(24)).  Derives C_Effect DIRECTLY.  Ctor (owner, achievementId).

namespace wh::rpgmodule {

class C_UnlockAchievementEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UnlockAchievementEffect;
    int32_t m_achievementId;   // +0x10
    // +0x14 4-byte tail padding
};
static_assert(sizeof(C_UnlockAchievementEffect) == 0x18,
              "C_UnlockAchievementEffect must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
