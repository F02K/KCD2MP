#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_UknownScriptData -- payload of C_UknownScriptCause (game's own typo,
// preserved) (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: create fn sub_1809DC2EC tracked-alloc 16; v5[1]=*a3 @0x1809DC34C).
// Generic script-driven cause with MANY factory callers (0x1809DC648, 0x180FA3180,
// 0x181055E4C, 0x18168491C, 0x18168530C, 0x181828F44, 0x182CD3828, ... + data-table refs).

namespace wh::rpgmodule {

struct S_UknownScriptData {
    uint64_t m_context;   // +0x00  script/context handle [proposed]
};
static_assert(sizeof(S_UknownScriptData) == 0x08, "S_UknownScriptData must be 0x08");

}  // namespace wh::rpgmodule
