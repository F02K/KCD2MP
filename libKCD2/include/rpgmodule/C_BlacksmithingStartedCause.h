#pragma once
#include "C_Cause.h"
#include "S_BlacksmithingStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BlacksmithingStartedCause -- "BlacksmithingStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_180B68CCC tracked-alloc 16).  RTTI TD rva 0x4D5CF38,
// vtable rva 0x3A7BD18 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x18158647C -> visitor+104, [3] GetTypeName 0x18194D954 ->
// "BlacksmithingStarted" (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_BlacksmithingStartedCause : public C_Cause<S_BlacksmithingStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BlacksmithingStartedCause;
};
static_assert(sizeof(C_BlacksmithingStartedCause) == 0x10,
              "C_BlacksmithingStartedCause must be 0x10 (alloc 16 at sub_180B68CCC)");

}  // namespace wh::rpgmodule
