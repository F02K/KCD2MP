#pragma once
#include <cstdint>
#include <cstddef>
#include "C_FactionEffect.h"
#include "S_ReputationChangeRequest.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionReputationEffect -- faction-level reputation change
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_FactionReputationEffect@rpgmodule@wh@@ (TD 0x184D4E888)  vtable 0x183F1EE60.
// sizeof 0x48.  Ctor sub_180784F44(this, faction, causeName, changeId): C_Effect::m_pOwner = the
// target faction (self-registers into faction+0x10), m_causeName = cause, then default-constructs
// the embedded request (filled during Apply from the reputation_change row).

namespace wh::rpgmodule {

class C_FactionReputationEffect : public C_FactionEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionReputationEffect;
    int32_t  m_repChangeId;               // +0x18  reputation_change row id
    uint32_t _pad1C;                      // +0x1C
    S_ReputationChangeRequest m_request;  // +0x20  built from the row at Apply time
};
static_assert(sizeof(C_FactionReputationEffect) == 0x48, "C_FactionReputationEffect must be 0x48");
static_assert(offsetof(C_FactionReputationEffect, m_request) == 0x20, "request at 0x20");

}  // namespace wh::rpgmodule
