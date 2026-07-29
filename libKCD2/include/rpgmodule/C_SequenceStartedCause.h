#pragma once
#include "C_Cause.h"
#include "S_SequenceStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SequenceStartedCause -- "SequenceStarted" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: create fn sub_180C680A0 tracked-alloc 32).  RTTI TD rva 0x4D5DA18,
// vtable rva 0x3A8BE58 (4 slots): [0] dtor rva 0x1407D48 (own body -- releases string),
// [1] nullsub, [2] Accept rva 0xC6944C -> visitor+640, [3] GetName rva 0xADA274 ->
// "SequenceStarted" (verified).  Factory caller sub_180C67FE8.

namespace wh::rpgmodule {

class C_SequenceStartedCause : public C_Cause<S_SequenceStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SequenceStartedCause;
};
static_assert(sizeof(C_SequenceStartedCause) == 0x20,
              "C_SequenceStartedCause must be 0x20 (alloc 32 at sub_180C680A0)");

}  // namespace wh::rpgmodule
