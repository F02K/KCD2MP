#pragma once
#include "C_Cause.h"
#include "S_BlacksmithingFinishedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BlacksmithingFinishedCause -- "BlacksmithingFinished" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x50 (PROVEN: factory sub_180B68AAC tracked-alloc 80).  RTTI TD rva 0x4D5C2C0,
// vtable rva 0x3A7BB88 (4 slots): [0] dtor 0x18191DAAC (own body -- releases the
// CryStringT member), [1] nullsub, [2] Accept 0x1817C8998 -> visitor+80, [3] GetTypeName
// 0x1817C8C88 -> "BlacksmithingFinished" (verified).  Cause-factory shape (raw cause;
// caller wraps in C_Event).

namespace wh::rpgmodule {

class C_BlacksmithingFinishedCause : public C_Cause<S_BlacksmithingFinishedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BlacksmithingFinishedCause;
};
static_assert(sizeof(C_BlacksmithingFinishedCause) == 0x50,
              "C_BlacksmithingFinishedCause must be 0x50 (alloc 80 at sub_180B68AAC)");

}  // namespace wh::rpgmodule
