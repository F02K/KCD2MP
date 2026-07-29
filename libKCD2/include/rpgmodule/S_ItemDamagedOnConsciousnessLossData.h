#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ItemDamagedOnConsciousnessLossData -- payload of
// C_ItemDamagedOnConsciousnessLossCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_181FCE6D0 PLAIN-alloc sub_1804F75C0(24) -- no
// tracked-allocator counters; _OWORD @+0x08).  Caller sub_181FD36A0 (via sub_181FCE7D0).
// {item, owner} handle split UNVERIFIED.

namespace wh::rpgmodule {

struct S_ItemDamagedOnConsciousnessLossData {
    uint64_t m_a;   // +0x00  item handle [U]
    uint64_t m_b;   // +0x08  owner handle [U]
};
static_assert(sizeof(S_ItemDamagedOnConsciousnessLossData) == 0x10,
              "S_ItemDamagedOnConsciousnessLossData must be 0x10");

}  // namespace wh::rpgmodule
