#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PlayerEndedToLootStashData -- payload of C_PlayerEndedToLootStashCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x50 (PROVEN: ctor sub_182CDE9A8 tracked-alloc 88, += 0x58; dword *a2 @+0,
// qword *(a2+8) @+8, then 64-byte aggregate copy via sub_180B5EED0 @+16).  Caller
// sub_182CFA59C @0x182CFA621.  The 64B aggregate = {dword; intrusive-list head; ptr;
// std::vector(0x18); 2 trailing qwords} -- looted-item element type UNVERIFIED, mirrored
// as a raw blob.  Ctor returns the raw cause (*a1 = v); caller posts separately.

namespace wh::rpgmodule {

struct S_PlayerEndedToLootStashData {
    int32_t  m_id;              // +0x00  id/count (= caller a2) [proposed]
    uint8_t  _pad04[4];         // +0x04
    uint64_t m_stashHandle;     // +0x08  (= caller a3) [proposed]
    uint8_t  m_lootedItems[64]; // +0x10  list+vector container (copied by sub_180B5EED0) [U element types]
};
static_assert(sizeof(S_PlayerEndedToLootStashData) == 0x50, "S_PlayerEndedToLootStashData must be 0x50");

}  // namespace wh::rpgmodule
