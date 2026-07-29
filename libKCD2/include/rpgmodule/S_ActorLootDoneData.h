#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ActorLootDoneData -- payload of C_ActorLootDoneCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x50 (PROVEN: factory sub_181745488 tracked-alloc 88 = 8+80; v5[1]=*a2,
// v5[2]=a2[1], then 64-byte aggregate copy via sub_180B5EED0 @0x1817454FF).
// The 64B aggregate = {dword; intrusive std::list head; ptr; std::vector(0x18);
// 2 trailing qwords} -- element type of the loot-entry list/vector UNVERIFIED, so it is
// mirrored as a raw blob.

namespace wh::rpgmodule {

struct S_ActorLootDoneData {
    uint64_t m_a;              // +0x00  WUID [proposed]
    uint64_t m_b;              // +0x08  WUID [proposed]
    uint8_t  m_lootSet[64];    // +0x10  list+vector container (copied by sub_180B5EED0) [U element types]
};
static_assert(sizeof(S_ActorLootDoneData) == 0x50, "S_ActorLootDoneData must be 0x50 (alloc 88 minus vptr)");

}  // namespace wh::rpgmodule
