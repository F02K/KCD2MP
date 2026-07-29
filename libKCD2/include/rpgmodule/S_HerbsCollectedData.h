#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_HerbsCollectedData -- payload of C_HerbsCollectedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD54E4 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_181FD48A0 builds { *v11 (getter, likely player WUID), a4 }; downstream checks perk
// "inventory_perkLuckyFind" (herb-gathering reward path).

namespace wh::rpgmodule {

struct S_HerbsCollectedData {
    uint64_t m_actor;     // +0x00  actor WUID [proposed]
    int32_t  m_count;     // +0x08  caller arg a4 [U role]
    uint8_t  _pad0C[4];   // +0x0C
};
static_assert(sizeof(S_HerbsCollectedData) == 0x10, "S_HerbsCollectedData must be 0x10");

}  // namespace wh::rpgmodule
