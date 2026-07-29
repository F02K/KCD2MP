#pragma once
#include "C_Effect.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemEffect -- item-targeted effect branch (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ItemEffect@rpgmodule@wh@@ (TD 0x184D4E918)  vtable 0x183F1EEB0 (9 slots; [4]
// GetTargetId returns +0x10 -- same accessor 0x181A74170 as C_SoulEffect, so the member is the
// target id; typed WUID by analogy [id kind INFERRED]).

namespace wh::rpgmodule {

class C_ItemEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemEffect;
    wh::framework::WUID m_targetWuid;   // +0x10  target item id [kind INFERRED from shared accessor]
};
static_assert(sizeof(C_ItemEffect) == 0x18, "C_ItemEffect must be 0x18");

}  // namespace wh::rpgmodule
