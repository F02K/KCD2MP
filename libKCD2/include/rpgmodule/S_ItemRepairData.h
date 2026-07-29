#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ItemRepairData -- payload of C_ItemRepairCause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD5818 tracked-alloc 32; _OWORD @+0x08 + qword
// @+0x18).  Callers sub_182CFE4C4 / sub_182CFE688 build { *(getter) @+0, arg1sub @+8,
// *(a4+48) @+16 }.

namespace wh::rpgmodule {

struct S_ItemRepairData {
    uint64_t m_actor;   // +0x00  actor handle [proposed]
    uint64_t m_b;       // +0x08  [U role]
    uint64_t m_item;    // +0x10  item handle [proposed]
};
static_assert(sizeof(S_ItemRepairData) == 0x18, "S_ItemRepairData must be 0x18");

}  // namespace wh::rpgmodule
