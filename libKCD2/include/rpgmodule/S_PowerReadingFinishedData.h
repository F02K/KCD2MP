#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PowerReadingFinishedData -- payload of C_PowerReadingFinishedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5C10 tracked-alloc 32; _OWORD @+0x08 + tail).
// Callers sub_182CFAD84 @0x182CFAE93 and sub_182CFD46C @0x182CFD60B: reader =
// *(_QWORD*)(v10+48); item = 16-byte block *a4 (WUID pair or item ref + subid) [U split].

namespace wh::rpgmodule {

struct S_PowerReadingFinishedData {
    uint64_t m_reader;    // +0x00  reader handle/WUID [proposed]
    uint64_t m_item[2];   // +0x08  16-byte item/book block [U split]
};
static_assert(sizeof(S_PowerReadingFinishedData) == 0x18, "S_PowerReadingFinishedData must be 0x18");

}  // namespace wh::rpgmodule
