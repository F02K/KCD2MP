#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_PlayerIsInCombatDangerData -- payload of C_PlayerIsInCombatDangerCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// 1-byte bool payload (object alloc 16).  PROVEN: ctor sub_1817D5EC0
// *(BYTE*)(v5+8)=*a3; caller sub_1809D12C8 @0x1809D1464 (same poller as
// PlayerIsActiveInCombat) latches byte_18548E91C.

namespace wh::rpgmodule {

struct S_PlayerIsInCombatDangerData {
    bool m_inDanger;   // +0x00  [proposed]
};
static_assert(sizeof(S_PlayerIsInCombatDangerData) == 0x01, "S_PlayerIsInCombatDangerData must be 0x01");

}  // namespace wh::rpgmodule
