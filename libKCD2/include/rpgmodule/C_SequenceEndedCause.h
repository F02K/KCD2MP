#pragma once
#include "C_Cause.h"
#include "S_SequenceEndedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SequenceEndedCause -- "SequenceEnded" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: create fn sub_1806147C8 tracked-alloc 32).  RTTI TD rva 0x4D5DBC8,
// vtable rva 0x3A424C0 (4 slots): [0] dtor rva 0x1407D98 (own body -- releases string),
// [1] nullsub, [2] Accept rva 0xC69438 -> visitor+632, [3] GetName rva 0xADA1FC ->
// "SequenceEnded" (verified).  Factory caller sub_180614690.

namespace wh::rpgmodule {

class C_SequenceEndedCause : public C_Cause<S_SequenceEndedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SequenceEndedCause;
};
static_assert(sizeof(C_SequenceEndedCause) == 0x20, "C_SequenceEndedCause must be 0x20 (alloc 32 at sub_1806147C8)");

}  // namespace wh::rpgmodule
