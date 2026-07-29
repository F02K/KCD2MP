#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_SequenceEndedData -- payload of C_SequenceEndedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_1806147C8 tracked-alloc 32; qword *(v5+8)=*a2,
// CryStringT copy sub_1804FD208(v5+16,a2+8), bytes *(v5+24)=*(a2+16) and
// *(v5+25)=*(a2+17)).  Factory caller sub_180614690 (site 0x180614707).

namespace wh::rpgmodule {

struct S_SequenceEndedData {
    uint64_t         m_actor;     // +0x00  handle/WUID [proposed]
    CryStringT<char> m_name;      // +0x08  sequence name [proposed]
    bool             m_b0;        // +0x10  [U role]
    bool             m_b1;        // +0x11  [U role]
    uint8_t          _pad12[6];   // +0x12
};
static_assert(sizeof(S_SequenceEndedData) == 0x18, "S_SequenceEndedData must be 0x18");

}  // namespace wh::rpgmodule
