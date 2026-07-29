#pragma once
#include "C_Cause.h"
#include "S_UknownScriptData.h"

// -----------------------------------------------
// wh::rpgmodule::C_UknownScriptCause -- "UknownScript" event cause (game's own typo,
// preserved) (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_1809DC2EC tracked-alloc 16).  RTTI TD rva 0x4D5F400,
// vtable rva 0x3A68C28 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x9DC160 -> visitor+288, [3] GetName rva 0x16F9A14 -> "UknownScript"
// (verified).  Generic script-driven cause with many factory callers (0x9DC648,
// 0xFA3180, 0x1055E4C, ... + data-table refs).

namespace wh::rpgmodule {

class C_UknownScriptCause : public C_Cause<S_UknownScriptData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UknownScriptCause;
};
static_assert(sizeof(C_UknownScriptCause) == 0x10, "C_UknownScriptCause must be 0x10 (alloc 16 at sub_1809DC2EC)");

}  // namespace wh::rpgmodule
