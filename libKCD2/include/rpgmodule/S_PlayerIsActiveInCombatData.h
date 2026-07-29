#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_PlayerIsActiveInCombatData -- payload of C_PlayerIsActiveInCombatCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// 1-byte bool payload in an 8-byte-aligned region (object alloc 16 = 8 vptr + 1 rounded
// up).  PROVEN: ctor sub_1817D5F60 *(BYTE*)(v5+8)=*a3; caller sub_1809D12C8 @0x1809D14B0
// latches byte_18548E91D.

namespace wh::rpgmodule {

struct S_PlayerIsActiveInCombatData {
    bool m_isActive;   // +0x00  [proposed]
};
static_assert(sizeof(S_PlayerIsActiveInCombatData) == 0x01, "S_PlayerIsActiveInCombatData must be 0x01");

}  // namespace wh::rpgmodule
