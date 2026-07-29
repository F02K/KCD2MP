#pragma once
#include "C_Cause.h"
#include "S_RandomEventData.h"

// -----------------------------------------------
// wh::rpgmodule::C_RandomEventCause -- "RandomEvent" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x38 (PROVEN: ctor sub_1806216BC tracked-alloc 56, += 0x38).  RTTI TD rva
// 0x4D5C358, vtable rva 0x3A42CD0 (4 slots): [0] dtor 0x180621328 (own body -- releases
// 4 strings), [1] nullsub, [2] Accept -> visitor+0x20, [3] GetName -> "RandomEvent"
// (verified).  make-only shape; caller sub_1806214BC.

namespace wh::rpgmodule {

class C_RandomEventCause : public C_Cause<S_RandomEventData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RandomEventCause;
};
static_assert(sizeof(C_RandomEventCause) == 0x38, "C_RandomEventCause must be 0x38 (alloc 56 at sub_1806216BC)");

}  // namespace wh::rpgmodule
