#pragma once
#include <cstdint>
#include "C_SoulEffect.h"
#include "S_PerkDefinitionId.h"

// -----------------------------------------------
// wh::rpgmodule::C_AddPerkEffect -- grant a perk directly (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_AddPerkEffect@rpgmodule@wh@@ (TD 0x184D5FDB8)  vtable 0x183B27078.
// sizeof 0x28 (PROVEN: factory sub_1811A0B24 operator new(40)).
// Same shape as C_LearnPerkEffect minus the result byte -- the direct-add path (the KCD1
// "AddPerk bypasses the perk point check" equivalent).

namespace wh::rpgmodule {

class C_AddPerkEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AddPerkEffect;
    S_PerkDefinitionId m_perkId;   // +0x18  perk to add
};
static_assert(sizeof(C_AddPerkEffect) == 0x28, "C_AddPerkEffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
