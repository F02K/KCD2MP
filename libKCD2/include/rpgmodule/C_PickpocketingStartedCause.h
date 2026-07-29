#pragma once
#include "C_Cause.h"
#include "S_PickpocketingStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PickpocketingStartedCause -- "PickpocketingStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5AC8 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5DE98, vtable rva 0x3F273F0 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x1B8, [3] GetName -> "PickpocketingStarted"
// (verified).  make+post shape; caller sub_182CFA4E0.

namespace wh::rpgmodule {

class C_PickpocketingStartedCause : public C_Cause<S_PickpocketingStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickpocketingStartedCause;
};
static_assert(sizeof(C_PickpocketingStartedCause) == 0x18,
              "C_PickpocketingStartedCause must be 0x18 (alloc 24 at sub_182CD5AC8)");

}  // namespace wh::rpgmodule
