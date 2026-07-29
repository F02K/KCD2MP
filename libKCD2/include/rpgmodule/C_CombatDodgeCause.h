#pragma once
#include "C_Cause.h"
#include "S_CombatDodgeData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CombatDodgeCause -- "CombatDodge" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: ctor sub_1816490FC tracked-alloc 48, += 0x30 @0x18164913B).
// RTTI TD rva 0x4D5DB38, vtable rva 0x3BDF118 (4 slots): [0] dtor rva 0x164FF18,
// [1] nullsub, [2] Accept rva 0x199C128 -> visitor+0x250, [3] GetTypeName rva 0x199C1F4
// -> "CombatDodge" (verified).  Pattern-B ctor (wraps in C_Event inline); create-site
// sub_181648F28.

namespace wh::rpgmodule {

class C_CombatDodgeCause : public C_Cause<S_CombatDodgeData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatDodgeCause;
};
static_assert(sizeof(C_CombatDodgeCause) == 0x30, "C_CombatDodgeCause must be 0x30 (alloc 48 at sub_1816490FC)");

}  // namespace wh::rpgmodule
