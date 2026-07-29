#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_PlayerIsWantedData -- payload of C_PlayerIsWantedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// 1-byte bool payload (object alloc 16).  PROVEN: ctor sub_18189AB40 *(BYTE*)(v5+8)=*a3;
// caller sub_18189AAF0 @0x18189AB1B passes a2.

namespace wh::rpgmodule {

struct S_PlayerIsWantedData {
    bool m_isWanted;   // +0x00  [proposed]
};
static_assert(sizeof(S_PlayerIsWantedData) == 0x01, "S_PlayerIsWantedData must be 0x01");

}  // namespace wh::rpgmodule
