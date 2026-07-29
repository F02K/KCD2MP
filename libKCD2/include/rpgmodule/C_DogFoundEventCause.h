#pragma once
#include "C_Cause.h"
#include "S_DogFoundEventData.h"

// -----------------------------------------------
// wh::rpgmodule::C_DogFoundEventCause -- "DogFoundEvent" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5190 tracked-alloc 16, += 0x10 @0x182CD51D0).
// RTTI TD rva 0x4D5F720, vtable rva 0x3F27BF8 (4 slots): [0] dtor rva 0x19A99AC (ICF
// trivial), [1] nullsub, [2] Accept rva 0x9DC2A0 -> visitor+0x100, [3] GetTypeName rva
// 0x2CF5DC8 -> "DogFoundEvent" (verified).  Pattern-B ctor; create-site sub_182CEEED8
// (RNG-gated).

namespace wh::rpgmodule {

class C_DogFoundEventCause : public C_Cause<S_DogFoundEventData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DogFoundEventCause;
};
static_assert(sizeof(C_DogFoundEventCause) == 0x10, "C_DogFoundEventCause must be 0x10 (alloc 16 at sub_182CD5190)");

}  // namespace wh::rpgmodule
