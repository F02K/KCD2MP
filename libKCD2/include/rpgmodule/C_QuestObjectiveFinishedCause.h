#pragma once
#include "C_Cause.h"
#include "S_QuestObjectiveFinishedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_QuestObjectiveFinishedCause -- "QuestObjectiveFinished" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_182CDEA48 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5C820, vtable rva 0x3F270A8 (4 slots): [0] dtor 0x182CED6D0 (own body -- releases
// 2 strings), [1] nullsub, [2] Accept -> visitor+0x138, [3] GetName ->
// "QuestObjectiveFinished" (verified).  make-only shape; caller sub_182CFA994.

namespace wh::rpgmodule {

class C_QuestObjectiveFinishedCause : public C_Cause<S_QuestObjectiveFinishedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_QuestObjectiveFinishedCause;
};
static_assert(sizeof(C_QuestObjectiveFinishedCause) == 0x20,
              "C_QuestObjectiveFinishedCause must be 0x20 (alloc 32 at sub_182CDEA48)");

}  // namespace wh::rpgmodule
