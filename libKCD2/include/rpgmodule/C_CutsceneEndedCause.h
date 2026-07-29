#pragma once
#include "C_Cause.h"
#include "S_CutsceneEndedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CutsceneEndedCause -- "CutsceneEnded" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CA6BF8 tracked-alloc 24, += 0x18 @0x182CA6C40).
// RTTI TD rva 0x4D4DD78, vtable rva 0x3F1EB80 (4 slots): [0] dtor rva 0x2CA8E30
// (string-releasing, shared with CutsceneStarted), [1] nullsub, [2] Accept rva 0x1855F44
// -> visitor+0x10, [3] GetTypeName rva 0x2C8A988 -> "CutsceneEnded" (verified).
// Pattern-A ctor (raw cause; create-site sub_182C9BC24 wraps).

namespace wh::rpgmodule {

class C_CutsceneEndedCause : public C_Cause<S_CutsceneEndedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CutsceneEndedCause;
};
static_assert(sizeof(C_CutsceneEndedCause) == 0x18, "C_CutsceneEndedCause must be 0x18 (alloc 24 at sub_182CA6BF8)");

}  // namespace wh::rpgmodule
