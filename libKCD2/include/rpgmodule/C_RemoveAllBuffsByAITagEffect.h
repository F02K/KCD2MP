#pragma once
#include <cstdint>
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_RemoveAllBuffsByAITagEffect -- strip buffs by AI tag (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5FE70  vtable 0x183F27898.  sizeof 0x20 (PROVEN: factory sub_182CD43DC
// operator new(32)).  Ctor (owner, targetWuid, aiTag).

namespace wh::rpgmodule {

class C_RemoveAllBuffsByAITagEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RemoveAllBuffsByAITagEffect;
    int32_t  m_aiTag;   // +0x18  buff AI tag filter
    uint32_t _pad1C;    // +0x1C
};
static_assert(sizeof(C_RemoveAllBuffsByAITagEffect) == 0x20,
              "C_RemoveAllBuffsByAITagEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
