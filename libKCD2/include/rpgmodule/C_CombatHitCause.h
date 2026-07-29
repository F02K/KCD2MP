#pragma once
#include "C_Cause.h"
#include "S_CombatHitData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CombatHitCause -- "CombatHit" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).  Largest cause object (0x98).
// -----------------------------------------------
// sizeof 0x98 (PROVEN: ctor sub_180726F08 tracked-alloc 152, += 0x98 @0x180726F43).
// RTTI TD rva 0x4D71C40, vtable rva 0x3A4B740 (4 slots): [0] dtor rva 0x167E658,
// [1] nullsub, [2] Accept rva 0xAD9908 -> visitor+0x248, [3] GetTypeName rva 0xADA2EC ->
// "CombatHit" (verified).  Pattern-A ctor (raw cause; create-site sub_180726DF4 wraps).

namespace wh::rpgmodule {

class C_CombatHitCause : public C_Cause<S_CombatHitData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatHitCause;
};
static_assert(sizeof(C_CombatHitCause) == 0x98, "C_CombatHitCause must be 0x98 (alloc 152 at sub_180726F08)");

}  // namespace wh::rpgmodule
