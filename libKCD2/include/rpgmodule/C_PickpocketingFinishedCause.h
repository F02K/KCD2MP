#pragma once
#include "C_Cause.h"
#include "S_PickpocketingFinishedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PickpocketingFinishedCause -- "PickpocketingFinished" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x28 (PROVEN: ctor sub_182CD5A1C tracked-alloc 40, += 0x28 @0x182CD5A5C).
// RTTI TD rva 0x4D5E080, vtable rva 0x3F27418 (4 slots): [0] dtor 0x182CED5F4 (own
// symbol, POD body), [1] nullsub, [2] Accept -> visitor+0x1B0, [3] GetName ->
// "PickpocketingFinished" (verified).  make+post shape; caller sub_182CFA134.

namespace wh::rpgmodule {

class C_PickpocketingFinishedCause : public C_Cause<S_PickpocketingFinishedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickpocketingFinishedCause;
};
static_assert(sizeof(C_PickpocketingFinishedCause) == 0x28,
              "C_PickpocketingFinishedCause must be 0x28 (alloc 40 at sub_182CD5A1C)");

}  // namespace wh::rpgmodule
