#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AlchemyRecipeFinishData -- payload of C_AlchemyRecipeFinishCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x50 (PROVEN: factory sub_182CDE4E0 tracked-alloc 88 = 8+80; field-by-field copy
// from a2: qword@0, 7 dwords @+8..+0x20, 2x _OWORD (4 dwords each) @+0x24/+0x34, byte
// @+0x44, dword @+0x48; ends +0x4C, tail pad to 0x50).  All field roles [U] (brewing
// stats/quality counters candidates).

namespace wh::rpgmodule {

struct S_AlchemyRecipeFinishData {
    uint64_t m_q0;         // +0x00  qword [U role]
    int32_t  m_d[7];       // +0x08..+0x24  7 dwords [U roles]
    int32_t  m_blkA[4];    // +0x24  _OWORD copied as 4 dwords [U]
    int32_t  m_blkB[4];    // +0x34  _OWORD copied as 4 dwords [U]
    uint8_t  m_b44;        // +0x44  byte [U role]
    uint8_t  _pad45[3];    // +0x45
    int32_t  m_d48;        // +0x48  dword [U role]
    uint8_t  _pad4C[4];    // +0x4C  tail pad
};
static_assert(sizeof(S_AlchemyRecipeFinishData) == 0x50, "S_AlchemyRecipeFinishData must be 0x50 (alloc 88 minus vptr)");

}  // namespace wh::rpgmodule
