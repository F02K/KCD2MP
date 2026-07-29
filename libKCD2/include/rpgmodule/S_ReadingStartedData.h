#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ReadingStartedData -- payload of C_ReadingStartedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5D70 tracked-alloc 32; same {reader, 16B item}
// layout as the PowerReading payloads).  Caller sub_182CFB2AC @0x182CFB32D: reader = *v8
// (getter); item = *(_OWORD*)(sub_1804695B4(a3)+1).

namespace wh::rpgmodule {

struct S_ReadingStartedData {
    uint64_t m_reader;    // +0x00  reader handle/WUID [proposed]
    uint64_t m_item[2];   // +0x08  16-byte book/item block [U split]
};
static_assert(sizeof(S_ReadingStartedData) == 0x18, "S_ReadingStartedData must be 0x18");

}  // namespace wh::rpgmodule
