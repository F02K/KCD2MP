#pragma once
#include "C_Cause.h"
#include "S_CollisionHitData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CollisionHitCause -- "CollisionHit" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x70 (PROVEN: ctor sub_1816C7BB4 tracked-alloc 112, += 0x70 @0x1816C7BEF).
// RTTI TD rva 0x4D719C0, vtable rva 0x3BF2F80 (4 slots): [0] dtor rva 0x17FA01C,
// [1] nullsub, [2] Accept rva 0xAD9480 -> visitor+0x230, [3] GetTypeName rva 0xAD94A4 ->
// "CollisionHit" (verified).  Pattern-A ctor (returns raw cause; create-site
// sub_1816C79F8 raises two events with the A/B participants swapped).

namespace wh::rpgmodule {

class C_CollisionHitCause : public C_Cause<S_CollisionHitData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CollisionHitCause;
};
static_assert(sizeof(C_CollisionHitCause) == 0x70, "C_CollisionHitCause must be 0x70 (alloc 112 at sub_1816C7BB4)");

}  // namespace wh::rpgmodule
