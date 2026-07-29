#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::S_HandItemParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08 (0x06 + pad).
// -----------------------------------------------
// Hand-item action-param block (RTTI template arg US_HandItemParams@playermodule; POD).  Lives at
// action+0x50.  VERIFIED via the primary-use factory sub_1806C6A00, which builds it by brew state
// for the C_ReturnObjectAction ctor sub_1817F7AF4:
//   state 0 -> { m_value=0, m_flags=0x0100 }
//   state 2 -> { m_value=0, m_flags=0x0101 }
//   state 5 -> { m_value=1, m_flags=0x0001 }
// The int16 hi/lo bytes encode which grabbable object + which hand [exact bit meaning UNVERIFIED].

namespace wh::playermodule {

struct S_HandItemParams {
    int32_t m_value;   // +0x00  object selector (0/1)
    int16_t m_flags;   // +0x04  hand/object encoding (0x0100 / 0x0101 / 0x0001 observed)
};
static_assert(sizeof(S_HandItemParams) == 8, "S_HandItemParams is int + int16 (+pad)");

}  // namespace wh::playermodule
