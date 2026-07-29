#pragma once
#include "C_Cause.h"
#include "S_QuestStateChangedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_QuestStateChangedCause -- "QuestStateChanged" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_180DC911C tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5CA00, vtable rva 0x3AB1BB8 (4 slots): [0] dtor 0x181407D98 (own body -- releases
// 1 string), [1] nullsub, [2] Accept -> visitor+0x130, [3] GetName -> "QuestStateChanged"
// (verified).  make-only shape; caller sub_180DC9060.

namespace wh::rpgmodule {

class C_QuestStateChangedCause : public C_Cause<S_QuestStateChangedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_QuestStateChangedCause;
};
static_assert(sizeof(C_QuestStateChangedCause) == 0x20,
              "C_QuestStateChangedCause must be 0x20 (alloc 32 at sub_180DC911C)");

}  // namespace wh::rpgmodule
