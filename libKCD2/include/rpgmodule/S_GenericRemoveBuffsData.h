#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_GenericRemoveBuffsData -- payload of C_GenericRemoveBuffsCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_181361ED8 tracked-alloc 16; v5[1]=*a3).  Caller
// sub_182CFC978 passes its a2 handle/WUID.

namespace wh::rpgmodule {

struct S_GenericRemoveBuffsData {
    uint64_t m_buffSource;   // +0x00  handle/WUID [proposed]
};
static_assert(sizeof(S_GenericRemoveBuffsData) == 0x08, "S_GenericRemoveBuffsData must be 0x08");

}  // namespace wh::rpgmodule
