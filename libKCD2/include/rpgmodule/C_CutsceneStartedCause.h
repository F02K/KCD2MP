#pragma once
#include "C_Cause.h"
#include "S_CutsceneStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CutsceneStartedCause -- "CutsceneStarted" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CA6C9C tracked-alloc 24, += 0x18 @0x182CA6CE4).
// RTTI TD rva 0x4D4DDB0, vtable rva 0x3F1EBB8 (4 slots): [0] dtor rva 0x2CA8E30
// (string-releasing, shared with CutsceneEnded), [1] nullsub, [2] Accept rva 0x2C8A428
// -> visitor+0x18, [3] GetTypeName rva 0x2C8A9FC -> "CutsceneStarted" (verified).
// Pattern-A ctor; create-site sub_182C9BC8C.

namespace wh::rpgmodule {

class C_CutsceneStartedCause : public C_Cause<S_CutsceneStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CutsceneStartedCause;
};
static_assert(sizeof(C_CutsceneStartedCause) == 0x18, "C_CutsceneStartedCause must be 0x18 (alloc 24 at sub_182CA6C9C)");

}  // namespace wh::rpgmodule
