#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PlayerOutfitSwitchedData -- payload of C_PlayerOutfitSwitchedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182D190E8 tracked-alloc 24).  Caller sub_180D8A154
// @0x180D8A1AE: actor=*v6 (getter), BYTE8=a3 (new outfit), BYTE9=a2 (old outfit); fires
// only if a2 != a3.

namespace wh::rpgmodule {

struct S_PlayerOutfitSwitchedData {
    uint64_t m_actor;       // +0x00  actor handle/WUID [proposed]
    uint8_t  m_newOutfit;   // +0x08  playermodule::E_OutfitId::Type truncated to a byte (RTTR reg sub_18022F870: A/B/C)
    uint8_t  m_oldOutfit;   // +0x09  same domain (sender sub_180D8A154 fires on change)
    uint8_t  _pad0A[6];     // +0x0A
};
static_assert(sizeof(S_PlayerOutfitSwitchedData) == 0x10, "S_PlayerOutfitSwitchedData must be 0x10");

}  // namespace wh::rpgmodule
