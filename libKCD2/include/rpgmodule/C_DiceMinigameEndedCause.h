#pragma once
#include "C_Cause.h"
#include "S_DiceMinigameEndedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_DiceMinigameEndedCause -- "DiceMinigameEnded" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: ctor sub_182CD5034 tracked-alloc 48, += 0x30 @0x182CD5074).
// RTTI TD rva 0x4D5D638, vtable rva 0x3F273C8 (4 slots): [0] dtor rva 0x164FF18,
// [1] nullsub, [2] Accept rva 0x163D3C0 -> visitor+0xC8, [3] GetTypeName rva 0x2CAC648
// -> "DiceMinigameEnded" (verified).  Pattern-B ctor; create-site sub_182CF9554.

namespace wh::rpgmodule {

class C_DiceMinigameEndedCause : public C_Cause<S_DiceMinigameEndedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DiceMinigameEndedCause;
};
static_assert(sizeof(C_DiceMinigameEndedCause) == 0x30,
              "C_DiceMinigameEndedCause must be 0x30 (alloc 48 at sub_182CD5034)");

}  // namespace wh::rpgmodule
