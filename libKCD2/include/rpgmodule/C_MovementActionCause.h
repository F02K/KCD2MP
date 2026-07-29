#pragma once
#include "C_Cause.h"
#include "S_MovementActionData.h"

// -----------------------------------------------
// wh::rpgmodule::C_MovementActionCause -- "MovementAction" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_180CF8CF0 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5C628, vtable rva 0x3A93968 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0xE7AB40 -> visitor+0x110, [3] GetName rva 0xE7ABF4 ->
// "MovementAction" (verified).  Caller sub_180CF8B6C (switches on the payload enum).

namespace wh::rpgmodule {

class C_MovementActionCause : public C_Cause<S_MovementActionData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MovementActionCause;
};
static_assert(sizeof(C_MovementActionCause) == 0x18, "C_MovementActionCause must be 0x18 (alloc 24 at sub_180CF8CF0)");

}  // namespace wh::rpgmodule
