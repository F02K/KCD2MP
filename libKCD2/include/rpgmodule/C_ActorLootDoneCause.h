#pragma once
#include "C_Cause.h"
#include "S_ActorLootDoneData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ActorLootDoneCause -- "ActorLootDone" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x58 (PROVEN: factory sub_181745488 tracked-alloc 88).  RTTI TD rva 0x4D5F0A0,
// vtable rva 0x3C09718 (4 slots): [0] dtor 0x1819A3DC0 (own body -- releases the 64B
// container), [1] nullsub, [2] Accept 0x180E7B494 -> visitor+776, [3] GetTypeName
// 0x180E7B548 -> "ActorLootDone" (verified).  Cause-factory shape: returns the raw cause;
// a thin caller (sub_180B689C0) wraps it in C_Event.

namespace wh::rpgmodule {

class C_ActorLootDoneCause : public C_Cause<S_ActorLootDoneData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActorLootDoneCause;
};
static_assert(sizeof(C_ActorLootDoneCause) == 0x58, "C_ActorLootDoneCause must be 0x58 (alloc 88 at sub_181745488)");

}  // namespace wh::rpgmodule
