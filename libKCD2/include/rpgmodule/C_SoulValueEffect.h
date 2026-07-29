#pragma once
#include <cstdint>
#include "C_SoulEffect.h"
#include "../framework/fixed_t.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulValueEffect<TId, TAmount> -- typed value-change soul effect
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven intermediate of the XP effects:
//   .?AV?$C_SoulValueEffect@W4E_SoulStat@rpgmodule@wh@@U?$fixed_t@I$0BJ@@@@rpgmodule@wh@@
//   .?AV?$C_SoulValueEffect@W4E_SoulSkill@rpgmodule@wh@@U?$fixed_t@I$0BJ@@@@rpgmodule@wh@@
// i.e. C_SoulValueEffect<E_SoulStat, fixed_t<uint32_t, 25>> etc. -- KCD2 XP amounts are
// FIXED-POINT (25 fractional bits). Members from the derived ctors (id @+0x18, amount @+0x1C).

namespace wh::rpgmodule {

template <typename TId, typename TAmount = fixed_t<uint32_t, 25>>
class C_SoulValueEffect : public C_SoulEffect {
public:
    TId     m_valueId;   // +0x18  E_SoulStat / E_SoulSkill
    TAmount m_amount;    // +0x1C  fixed-point XP amount (ctor copies the raw uint32)
};

}  // namespace wh::rpgmodule
