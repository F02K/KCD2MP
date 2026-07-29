#pragma once
#include "C_Cause.h"
#include "S_RewardReceivedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_RewardReceivedCause -- "RewardReceived" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: create fn sub_180B46118 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5C8C8, vtable rva 0x3A79DD8 (4 slots): [0] dtor rva 0x1407D98 (own body -- releases
// the string), [1] nullsub, [2] Accept rva 0xC69424 -> visitor+320, [3] GetName rva
// 0xC688C4 -> "RewardReceived" (verified).  Factory caller sub_180B46094.

namespace wh::rpgmodule {

class C_RewardReceivedCause : public C_Cause<S_RewardReceivedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RewardReceivedCause;
};
static_assert(sizeof(C_RewardReceivedCause) == 0x20, "C_RewardReceivedCause must be 0x20 (alloc 32 at sub_180B46118)");

}  // namespace wh::rpgmodule
