#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_ReputationNotification -- reputation_notification table row
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN fields: row builder sub_1816393C4 -- {dword @0, three normalized
// CryStrings @+0x08/+0x10/+0x18}; size = field extent, same caveat as S_ReputationCondition).

namespace wh::rpgmodule {

struct S_ReputationNotification {
    int32_t  m_param00;         // +0x00  [role UNVERIFIED -- notification id candidate]
    uint32_t _pad04;            // +0x04
    CryStringT<char> m_str08;   // +0x08  normalized [role UNVERIFIED]
    CryStringT<char> m_str10;   // +0x10  normalized [role UNVERIFIED]
    CryStringT<char> m_str18;   // +0x18  normalized [role UNVERIFIED]
};
static_assert(sizeof(S_ReputationNotification) == 0x20,
              "S_ReputationNotification must be 0x20 (row-builder field extent)");

}  // namespace wh::rpgmodule
