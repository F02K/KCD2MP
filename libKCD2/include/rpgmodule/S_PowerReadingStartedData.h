#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PowerReadingStartedData -- payload of C_PowerReadingStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5CC0 tracked-alloc 32; same layout as
// S_PowerReadingFinishedData).  Caller sub_182CFA7B4 @0x182CFA832: v24[0..8]=*v10
// (getter), v24[8..24]=*a3.

namespace wh::rpgmodule {

struct S_PowerReadingStartedData {
    uint64_t m_reader;    // +0x00  reader handle/WUID [proposed]
    uint64_t m_item[2];   // +0x08  16-byte item/book block [U split]
};
static_assert(sizeof(S_PowerReadingStartedData) == 0x18, "S_PowerReadingStartedData must be 0x18");

}  // namespace wh::rpgmodule
