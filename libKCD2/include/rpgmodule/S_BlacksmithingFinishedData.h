#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_BlacksmithingFinishedData -- payload of C_BlacksmithingFinishedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x48 (PROVEN: factory sub_180B68AAC tracked-alloc 80 = 8+72; qword @+0x08,
// CryStringT copy sub_1804FD208 @0x180B68B1C to +0x10, _OWORD @+0x18, byte @+0x28,
// 4 dwords @+0x2C..+0x38, _OWORD (4 dwords) @+0x3C, byte @+0x4C; ends +0x4D, 3B tail
// pad to 0x50).  Field roles [U] (forging-result stats candidates).

namespace wh::rpgmodule {

struct S_BlacksmithingFinishedData {
    uint64_t          m_q0;         // +0x00  qword WUID [proposed]
    CryStringT<char>  m_str;        // +0x08  ref-counted string (item/recipe id?) [U]
    uint64_t          m_blk10[2];   // +0x10  _OWORD [U]
    uint8_t           m_b20;        // +0x20  byte [U role]
    uint8_t           _pad21[3];    // +0x21
    int32_t           m_d24;        // +0x24  dword [U role]
    int32_t           m_d28;        // +0x28  dword [U role]
    int32_t           m_d2C;        // +0x2C  dword [U role]
    int32_t           m_d30;        // +0x30  dword [U role]
    int32_t           m_blk34[4];   // +0x34  _OWORD copied as 4 dwords [U]
    uint8_t           m_b44;        // +0x44  byte [U role]
    uint8_t           _pad45[3];    // +0x45  tail pad to 0x48
};
static_assert(sizeof(S_BlacksmithingFinishedData) == 0x48, "S_BlacksmithingFinishedData must be 0x48 (alloc 80 minus vptr)");

}  // namespace wh::rpgmodule
