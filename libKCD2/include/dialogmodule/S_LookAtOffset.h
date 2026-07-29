#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::S_LookAtOffset -- "look_at_offsets" tree-database row
// (KCD2 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// Row/element of C_LookAtOffsetDatabase (by value, +0x30 vector, clear helper
// sub_181862BFC iterates with stride 0x18; per-element dtor sub_1827F2E7C frees exactly
// one CryStringT at +0x00). The 0x10 tail is trivially-copyable POD -- a positional
// look-at offset (likely Vec3+float / quat; exact interpretation UNVERIFIED).

namespace wh::dialogmodule {

struct S_LookAtOffset {
    CryStringT<char> m_name;   // +0x00  offset name (key; only field with a dtor)
    uint8_t m_payload[0x10];   // +0x08  16-byte POD offset payload [interpretation UNVERIFIED]
};
static_assert(sizeof(S_LookAtOffset) == 0x18, "S_LookAtOffset must be 0x18 (clear stride 24)");

}  // namespace wh::dialogmodule
