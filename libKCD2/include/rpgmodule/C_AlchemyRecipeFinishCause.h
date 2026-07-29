#pragma once
#include "C_Cause.h"
#include "S_AlchemyRecipeFinishData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AlchemyRecipeFinishCause -- "AlchemyRecipeFinish" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x58 (PROVEN: factory sub_182CDE4E0 tracked-alloc 88).  RTTI TD rva 0x4D5CB38,
// vtable rva 0x3F272D8 (4 slots): [0] dtor 0x182CED5C8 (own body), [1] nullsub,
// [2] Accept 0x182CA92D4 -> visitor+384, [3] GetTypeName 0x182CAC560 ->
// "AlchemyRecipeFinish" (verified).  Cause-factory shape (raw cause; caller wraps in
// C_Event).

namespace wh::rpgmodule {

class C_AlchemyRecipeFinishCause : public C_Cause<S_AlchemyRecipeFinishData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipeFinishCause;
};
static_assert(sizeof(C_AlchemyRecipeFinishCause) == 0x58,
              "C_AlchemyRecipeFinishCause must be 0x58 (alloc 88 at sub_182CDE4E0)");

}  // namespace wh::rpgmodule
