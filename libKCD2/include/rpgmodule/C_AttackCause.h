#pragma once
#include "C_Cause.h"
#include "S_AttackData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AttackCause -- "Attack" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: factory sub_180C64E80 tracked-alloc 48).  RTTI TD rva 0x4D5EEF0,
// vtable rva 0x3A8BB28 (4 slots): [0] dtor 0x18164FF18, [1] nullsub, [2] Accept
// 0x181348688 -> visitor+752, [3] GetTypeName 0x181348760 -> "Attack" (verified).
// Event-factory shape.

namespace wh::rpgmodule {

class C_AttackCause : public C_Cause<S_AttackData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AttackCause;
};
static_assert(sizeof(C_AttackCause) == 0x30, "C_AttackCause must be 0x30 (alloc 48 at sub_180C64E80)");

}  // namespace wh::rpgmodule
