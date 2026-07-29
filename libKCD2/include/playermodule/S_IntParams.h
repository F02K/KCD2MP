#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::S_IntParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x04.
// -----------------------------------------------
// Single-int action-param block (RTTI template arg US_IntParams@playermodule; POD).  Lives at
// action+0x50.  C_TakeObjectAction: factory sub_1808D3EF0 writes 0 (grab object A) or 1 (grab
// object B / pot-slot item); base ctor sub_1808D4074.

namespace wh::playermodule {

struct S_IntParams {
    int32_t m_value;   // +0x00  which grabbable object (0/1 for TakeObject)
};
static_assert(sizeof(S_IntParams) == 4, "S_IntParams is one int");

}  // namespace wh::playermodule
