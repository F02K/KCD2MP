#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_RewardReceivedData -- payload of C_RewardReceivedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_180B46118 tracked-alloc 32, += 0x20
// @0x180B46150..5A; qword *(v5+8)=*a2 @0x180B4617F, CryStringT copy
// sub_1804FD208(v5+16,a2+8) @0x180B46187, dword *(v5+24)=*(a2+16) @0x180B4618F).
// Factory caller sub_180B46094 (site 0x180B460C0).

namespace wh::rpgmodule {

struct S_RewardReceivedData {
    uint64_t         m_actor;     // +0x00  handle/WUID [proposed]
    CryStringT<char> m_reward;    // +0x08  reward name/id [proposed]
    int32_t          m_amount;    // +0x10  amount/count [proposed]
    uint8_t          _pad14[4];   // +0x14
};
static_assert(sizeof(S_RewardReceivedData) == 0x18, "S_RewardReceivedData must be 0x18");

}  // namespace wh::rpgmodule
