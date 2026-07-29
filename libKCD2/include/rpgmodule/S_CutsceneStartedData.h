#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_CutsceneStartedData -- payload of C_CutsceneStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CA6C9C tracked-alloc 24; byte-for-byte identical
// layout to S_CutsceneEndedData: sub_1804FD208(obj+8,a2) @0x182CA6D09 + byte @0x182CA6D18).

namespace wh::rpgmodule {

struct S_CutsceneStartedData {
    CryStringT<char> m_name;    // +0x00  cutscene name/id
    bool             m_flag;    // +0x08  [U role]
    uint8_t          _pad09[7]; // +0x09
};
static_assert(sizeof(S_CutsceneStartedData) == 0x10, "S_CutsceneStartedData must be 0x10");

}  // namespace wh::rpgmodule
