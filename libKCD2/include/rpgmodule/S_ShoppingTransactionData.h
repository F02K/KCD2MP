#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::rpgmodule::S_ShoppingTransactionData -- payload of C_ShoppingTransactionCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x78 (PROVEN: create fn sub_180785CA0 tracked-alloc 128, += 0x80; qword @+0,
// qword @+8, 8 dwords @+0x10..+0x30, then 3x std::vector copy-ctor sub_1804187BC
// (8-byte element stride) @+0x30/+0x48/+0x60).  Factory caller sub_180785B8C (site
// 0x180785BA1).  Vector element type mirrored as uint64_t (8-byte stride) [U].

namespace wh::rpgmodule {

struct S_ShoppingTransactionData {
    uint64_t              m_vendor;     // +0x00  vendor/actor handle [proposed]
    uint64_t              m_player;     // +0x08  player/actor handle [proposed]
    int32_t               m_ints[8];    // +0x10  8 dwords [U roles]
    std::vector<uint64_t> m_itemsA;     // +0x30  (0x18) item list A [U element type]
    std::vector<uint64_t> m_itemsB;     // +0x48  (0x18) item list B [U element type]
    std::vector<uint64_t> m_itemsC;     // +0x60  (0x18) item list C [U element type]
};
static_assert(sizeof(S_ShoppingTransactionData) == 0x78, "S_ShoppingTransactionData must be 0x78");

}  // namespace wh::rpgmodule
