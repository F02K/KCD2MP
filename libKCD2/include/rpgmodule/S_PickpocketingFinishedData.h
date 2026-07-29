#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PickpocketingFinishedData -- payload of C_PickpocketingFinishedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_182CD5A1C tracked-alloc 40, dword_1851DFB18 += 0x28
// @0x182CD5A5C).  Caller sub_182CFA134 @0x182CFA1FC: handleA=*v17 (getter), handleB=v16
// (functor), flag=a4, itemCount=(a5[1]-*a5)>>3 (vector count), value=sub_181FD0500(...)
// (computed price/value).

namespace wh::rpgmodule {

struct S_PickpocketingFinishedData {
    uint64_t m_handleA;     // +0x00  handle/WUID [proposed]
    uint64_t m_handleB;     // +0x08  handle/WUID [proposed]
    bool     m_flag;        // +0x10  [proposed]
    uint8_t  _pad11[3];     // +0x11
    int32_t  m_itemCount;   // +0x14  vector element count [proposed]
    float    m_value;       // +0x18  computed price/value [proposed]
    uint8_t  _pad1C[4];     // +0x1C
};
static_assert(sizeof(S_PickpocketingFinishedData) == 0x20, "S_PickpocketingFinishedData must be 0x20");

}  // namespace wh::rpgmodule
