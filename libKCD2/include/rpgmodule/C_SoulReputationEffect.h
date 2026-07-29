#pragma once
#include <cstdint>
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulReputationEffect -- per-soul reputation change (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_SoulReputationEffect@rpgmodule@wh@@ (TD 0x184D4EB08)  vtable 0x183A523C8.
// sizeof 0x20 (PROVEN: factories sub_180784DC4 / sub_1807859FC operator new(32)); KCD1 parity.
// Two ctors: (owner, targetWuid, changeId) with weight = 1.0f, and (owner, targetWuid,
// changeId, weightBits) with an explicit weight.

namespace wh::rpgmodule {

class C_SoulReputationEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulReputationEffect;
    int32_t m_repChangeId;   // +0x18  reputation_change row id
    float   m_weight;        // +0x1C  scales the applied delta (default 1.0f)
};
static_assert(sizeof(C_SoulReputationEffect) == 0x20, "C_SoulReputationEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
