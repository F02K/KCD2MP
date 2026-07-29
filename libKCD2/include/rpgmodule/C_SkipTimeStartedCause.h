#pragma once
#include "C_Cause.h"
#include "S_SkipTimeStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SkipTimeStartedCause -- "SkipTimeStarted" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// alloc 16 (PROVEN: create fn sub_181950EB4 tracked-alloc 16) but writes ONLY the vtable
// -- payload region uninitialized (S_SkipTimeStartedData modeled empty; sizeof mirrors
// the alloc via vptr+padding).  RTTI TD rva 0x4D5D770, vtable rva 0x3C42570 (4 slots):
// [0] dtor rva 0x9DC2C0 (ICF trivial), [1] nullsub, [2] Accept rva 0x19B7D5C ->
// visitor+192, [3] GetName rva 0x19B7D70 -> "SkipTimeStarted" (verified).  Factory caller
// sub_181950D2C.

namespace wh::rpgmodule {

class C_SkipTimeStartedCause : public C_Cause<S_SkipTimeStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkipTimeStartedCause;
};
static_assert(sizeof(C_SkipTimeStartedCause) == 0x10,
              "C_SkipTimeStartedCause must be 0x10 (alloc 16 at sub_181950EB4)");

}  // namespace wh::rpgmodule
