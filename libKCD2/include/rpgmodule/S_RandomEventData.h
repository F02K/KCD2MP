#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_RandomEventData -- payload of C_RandomEventCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: ctor sub_1806216BC tracked-alloc 56, += 0x38; 4x CryStringT copies
// sub_1804FD208 @+8/+16/+24/+32, qword @+40, dword @+48, then bit0+bit1 copied from
// *(a2+44) into a byte bitfield @+44).  Caller sub_1806214BC @0x1806214D1.  Ctor returns
// the raw cause; caller posts separately.  Own dtor 0x180621328 releases the 4 strings.

namespace wh::rpgmodule {

struct S_RandomEventData {
    CryStringT<char> m_str0;      // +0x00  [U role]
    CryStringT<char> m_str1;      // +0x08  [U role]
    CryStringT<char> m_str2;      // +0x10  [U role]
    CryStringT<char> m_str3;      // +0x18  [U role]
    uint64_t         m_handle;    // +0x20  handle/WUID [proposed]
    int32_t          m_id;        // +0x28  [proposed]
    uint8_t          m_flags;     // +0x2C  bit0 + bit1 bools [proposed]
    uint8_t          _pad2D[3];   // +0x2D
};
static_assert(sizeof(S_RandomEventData) == 0x30, "S_RandomEventData must be 0x30");

}  // namespace wh::rpgmodule
