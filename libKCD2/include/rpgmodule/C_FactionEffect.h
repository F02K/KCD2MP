#pragma once
#include "C_Effect.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionEffect -- faction-targeted effect branch (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_FactionEffect@rpgmodule@wh@@ (TD 0x184D4E810)  vtable 0x183F1EF50 (9 slots; own
// [4] 0x181A931E0 / [5] 0x18041A6A0 overrides -- the target here is a faction, not a soul).
// Ctor evidence (inlined in C_FactionReputationEffect::ctor sub_180784F44): C_Effect::m_pOwner =
// the target C_Faction (self-registers into faction+0x10 vector), then +0x10 = cause name string.

namespace wh::rpgmodule {

class C_FactionEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionEffect;
    CryStringT<char> m_causeName;   // +0x10  cause label (copied into S_ReputationChangeRequest)
};
static_assert(sizeof(C_FactionEffect) == 0x18, "C_FactionEffect must be 0x18");

}  // namespace wh::rpgmodule
