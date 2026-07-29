#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_ReputationCondition -- reputation_condition table row
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Field extent 0x18 (PROVEN fields: row builder sub_181635280 -- {dword @0, CryString @+0x08
// (normalized from a blob char*), dword @+0x10}; SIZE INFERRED from the field extent -- the
// push path goes through opaque helpers, so trailing padding-only bytes cannot be fully
// excluded).

namespace wh::rpgmodule {

struct S_ReputationCondition {
    int32_t  m_param00;         // +0x00  [role UNVERIFIED -- condition id candidate]
    uint32_t _pad04;            // +0x04
    CryStringT<char> m_name;    // +0x08  normalized [condition name candidate]
    int32_t  m_param10;         // +0x10  [role UNVERIFIED]
    uint32_t _pad14;            // +0x14
};
static_assert(sizeof(S_ReputationCondition) == 0x18,
              "S_ReputationCondition must be 0x18 (row-builder field extent)");

}  // namespace wh::rpgmodule
