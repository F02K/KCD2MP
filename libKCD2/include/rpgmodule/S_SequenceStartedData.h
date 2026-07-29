#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_SequenceStartedData -- payload of C_SequenceStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_180C680A0 tracked-alloc 32; qword @+0, CryStringT
// copy sub_1804FD208 @+8, dword *(v5+24)=*(a2+16), bytes *(v5+28)/*(v5+29)).  Factory
// caller sub_180C67FE8 (site 0x180C67FFD).

namespace wh::rpgmodule {

struct S_SequenceStartedData {
    uint64_t         m_actor;     // +0x00  handle/WUID [proposed]
    CryStringT<char> m_name;      // +0x08  sequence name [proposed]
    int32_t          m_id;        // +0x10  id/enum [proposed]
    bool             m_b0;        // +0x14  [U role]
    bool             m_b1;        // +0x15  [U role]
    uint8_t          _pad16[2];   // +0x16
};
static_assert(sizeof(S_SequenceStartedData) == 0x18, "S_SequenceStartedData must be 0x18");

}  // namespace wh::rpgmodule
