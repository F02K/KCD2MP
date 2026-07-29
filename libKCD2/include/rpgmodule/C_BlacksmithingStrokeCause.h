#pragma once
#include "C_Cause.h"
#include "S_BlacksmithingStrokeData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BlacksmithingStrokeCause -- "BlacksmithingStroke" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x38 (PROVEN: factory sub_180C65970 tracked-alloc 56).  RTTI TD rva 0x4D5D030,
// vtable rva 0x3A8BBF8 (4 slots): [0] dtor 0x1816465F0, [1] nullsub, [2] Accept
// 0x180C67884 -> visitor+88, [3] GetTypeName 0x180C66E78 -> "BlacksmithingStroke"
// (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_BlacksmithingStrokeCause : public C_Cause<S_BlacksmithingStrokeData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BlacksmithingStrokeCause;
};
static_assert(sizeof(C_BlacksmithingStrokeCause) == 0x38,
              "C_BlacksmithingStrokeCause must be 0x38 (alloc 56 at sub_180C65970)");

}  // namespace wh::rpgmodule
