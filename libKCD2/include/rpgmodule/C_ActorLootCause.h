#pragma once
#include "C_Cause.h"
#include "S_ActorLootData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ActorLootCause -- "ActorLoot" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_181361D8C tracked-alloc 24).  RTTI TD rva 0x4D5F138,
// vtable rva 0x3B67670 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial), [1] nullsub,
// [2] Accept 0x180E7B178 -> visitor vtbl byte-offset 784, [3] GetTypeName 0x180E7B2FC ->
// "ActorLoot" (verified).  Event-factory shape (wraps in C_Event via sub_1809DB5A4 inline).

namespace wh::rpgmodule {

class C_ActorLootCause : public C_Cause<S_ActorLootData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActorLootCause;
};
static_assert(sizeof(C_ActorLootCause) == 0x18, "C_ActorLootCause must be 0x18 (alloc 24 at sub_181361D8C)");

}  // namespace wh::rpgmodule
