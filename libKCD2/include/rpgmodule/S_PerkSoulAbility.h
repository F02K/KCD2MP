#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PerkSoulAbility -- perk_soul_ability table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x14, ALIGN 4 (PROVEN: C_PerkSoulAbilityDatabase ParseRows 0x18155DF24, blob stride
// 20: {16B @0, dword @16 clamped to <=100}).  Stride 20 rules out an 8-aligned CryGUID --
// mirrored as uint32[4].

namespace wh::rpgmodule {

struct S_PerkSoulAbility {
    uint32_t m_perkId[4];   // +0x00  16-byte GUID value, 4-aligned [role INFERRED]
    uint32_t m_value;       // +0x10  clamped to [0..100] by ParseRows [role UNVERIFIED]
};
static_assert(sizeof(S_PerkSoulAbility) == 0x14 && alignof(S_PerkSoulAbility) == 4,
              "S_PerkSoulAbility must be 0x14/align-4 (ParseRows stride 20)");

}  // namespace wh::rpgmodule
