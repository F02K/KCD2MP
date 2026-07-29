#pragma once
#include "C_Cause.h"
#include "S_PlayerCrimeLevelChangedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerCrimeLevelChangedCause -- "PlayerCrimeLevelChanged" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_180EC97D4 tracked-alloc 16, += 0x10).  RTTI TD rva
// 0x4D5D8A0, vtable rva 0x3AC6F20 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x268, [3] GetName -> "PlayerCrimeLevelChanged"
// (verified).  make+post shape; caller sub_180EC9780.

namespace wh::rpgmodule {

class C_PlayerCrimeLevelChangedCause : public C_Cause<S_PlayerCrimeLevelChangedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerCrimeLevelChangedCause;
};
static_assert(sizeof(C_PlayerCrimeLevelChangedCause) == 0x10,
              "C_PlayerCrimeLevelChangedCause must be 0x10 (alloc 16 at sub_180EC97D4)");

}  // namespace wh::rpgmodule
