#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_CutsceneEndedData -- payload of C_CutsceneEndedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CA6BF8 tracked-alloc 24; CryStringT copy
// sub_1804FD208(obj+8,a2) @0x182CA6C65 + byte *(obj+16)=*(a2+8) @0x182CA6C74).

namespace wh::rpgmodule {

struct S_CutsceneEndedData {
    CryStringT<char> m_name;    // +0x00  cutscene name/id
    bool             m_flag;    // +0x08  [U role]
    uint8_t          _pad09[7]; // +0x09
};
static_assert(sizeof(S_CutsceneEndedData) == 0x10, "S_CutsceneEndedData must be 0x10");

}  // namespace wh::rpgmodule
