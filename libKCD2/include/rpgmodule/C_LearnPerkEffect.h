#pragma once
#include <cstdint>
#include "C_SoulEffect.h"
#include "S_PerkDefinitionId.h"

// -----------------------------------------------
// wh::rpgmodule::C_LearnPerkEffect -- learn a perk, consuming a perk point
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_LearnPerkEffect@rpgmodule@wh@@ (TD 0x184D5FDF0)  vtable 0x1847306C8.
// sizeof 0x30 (PROVEN: factory sub_182CD41D0 operator new(48)); KCD1 was also 0x30 (CryGUID key).
// Distinct from C_AddPerkEffect (which grants without the point bookkeeping/result flag).

namespace wh::rpgmodule {

class C_LearnPerkEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LearnPerkEffect;
    S_PerkDefinitionId m_perkId;   // +0x18  perk to learn
    uint8_t  m_bResult;            // +0x28  ctor 0; set on successful learn [role carried from KCD1]
    uint8_t  _pad29[7];            // +0x29
};
static_assert(sizeof(C_LearnPerkEffect) == 0x30, "C_LearnPerkEffect must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
