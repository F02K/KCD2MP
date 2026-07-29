#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ItemUseData -- payload of C_ItemUseCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD58C8 tracked-alloc 32; _OWORD @+0x08 + qword
// @+0x18, same shape as S_ItemRepairData).  Callers sub_1823D2C30 / sub_182CFC2AC /
// sub_182CFE590.  Fields UNVERIFIED.

namespace wh::rpgmodule {

struct S_ItemUseData {
    uint64_t m_a;   // +0x00  [U role]
    uint64_t m_b;   // +0x08  [U role]
    uint64_t m_c;   // +0x10  [U role]
};
static_assert(sizeof(S_ItemUseData) == 0x18, "S_ItemUseData must be 0x18");

}  // namespace wh::rpgmodule
