#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PickpocketingStartedData -- payload of C_PickpocketingStartedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5AC8 tracked-alloc 24).  Caller sub_182CFA4E0
// @0x182CFA543: thief=*v8 (getter), victim=v7 (getter).

namespace wh::rpgmodule {

struct S_PickpocketingStartedData {
    uint64_t m_thief;    // +0x00  thief handle/WUID [proposed]
    uint64_t m_victim;   // +0x08  victim handle/WUID [proposed]
};
static_assert(sizeof(S_PickpocketingStartedData) == 0x10, "S_PickpocketingStartedData must be 0x10");

}  // namespace wh::rpgmodule
