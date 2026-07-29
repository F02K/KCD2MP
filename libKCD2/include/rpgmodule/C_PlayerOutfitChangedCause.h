#pragma once
#include "C_Cause.h"
#include "S_PlayerOutfitChangedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerOutfitChangedCause -- "PlayerOutfitChanged" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0xE8 (PROVEN: ctor sub_18172E8F4 tracked-alloc 232, += 0xE8; 14x _OWORD payload
// copy).  RTTI TD rva 0x4D71668, vtable rva 0x3C05598 (4 slots): [0] dtor 0x1817F9FF0
// (own body), [1] nullsub, [2] Accept -> visitor+0x30, [3] GetName ->
// "PlayerOutfitChanged" (verified).  make-only shape; caller sub_18172E898.

namespace wh::rpgmodule {

class C_PlayerOutfitChangedCause : public C_Cause<S_PlayerOutfitChangedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerOutfitChangedCause;
};
static_assert(sizeof(C_PlayerOutfitChangedCause) == 0xE8,
              "C_PlayerOutfitChangedCause must be 0xE8 (alloc 232 at sub_18172E8F4)");

}  // namespace wh::rpgmodule
