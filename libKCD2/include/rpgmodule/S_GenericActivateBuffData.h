#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_GenericActivateBuffData -- payload of C_GenericActivateBuffCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_181056068 tracked-alloc 32; _OWORD@0 + qword@0x10 copy
// order per ctor -- create-site sub_181055EF4 builds { *(a1[17]+48) @+0, *(_OWORD*)a1[2]
// @+8 }).

namespace wh::rpgmodule {

struct S_GenericActivateBuffData {
    uint64_t m_target;     // +0x00  target-actor handle (= *(a1[17]+48)) [proposed]
    uint64_t m_buff[2];    // +0x08  buff descriptor _OWORD [U split]
};
static_assert(sizeof(S_GenericActivateBuffData) == 0x18, "S_GenericActivateBuffData must be 0x18");

}  // namespace wh::rpgmodule
