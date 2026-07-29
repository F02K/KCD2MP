#pragma once
#include "C_Cause.h"
#include "S_GenericMinigameData.h"

// -----------------------------------------------
// wh::rpgmodule::C_GenericMinigameCause -- "GenericMinigame" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5440 tracked-alloc 16, += 0x10 @0x182CD5480).
// RTTI TD rva 0x4D5C390, vtable rva 0x3F27210 (4 slots): [0] dtor rva 0x9DC2C0 (ICF
// trivial), [1] nullsub, [2] Accept rva 0x2CA9310 -> visitor+0x40, [3] GetTypeName rva
// 0x2CAC9E8 -> "GenericMinigame" (verified).  Pattern-B ctor; create-site sub_182CF1DD8
// (RNG-gated).

namespace wh::rpgmodule {

class C_GenericMinigameCause : public C_Cause<S_GenericMinigameData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GenericMinigameCause;
};
static_assert(sizeof(C_GenericMinigameCause) == 0x10,
              "C_GenericMinigameCause must be 0x10 (alloc 16 at sub_182CD5440)");

}  // namespace wh::rpgmodule
