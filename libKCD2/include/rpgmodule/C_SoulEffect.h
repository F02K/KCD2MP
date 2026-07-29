#pragma once
#include "C_Effect.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulEffect -- soul-targeted effect branch (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_SoulEffect@rpgmodule@wh@@ (TD 0x184CE4BF0)  vtable 0x183F1ED20 (9 slots; [4]
// GetTargetId returns +0x10, [5] inherits the base soul-flag gate, rest pure).
// Shared ctor sub_1809DC974(this, owner, targetWuid): C_Effect init + self-register into the
// owner's pending vector, then +0x10 = target soul WUID.

namespace wh::rpgmodule {

class C_SoulEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_rpgmodule_C_SoulEffect;
    wh::framework::WUID m_targetWuid;   // +0x10  target soul (C_SoulList::LookupByWUID key)
};
static_assert(sizeof(C_SoulEffect) == 0x18, "C_SoulEffect must be 0x18");

}  // namespace wh::rpgmodule
