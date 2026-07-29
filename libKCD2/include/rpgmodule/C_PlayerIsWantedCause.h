#pragma once
#include "C_Cause.h"
#include "S_PlayerIsWantedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerIsWantedCause -- "PlayerIsWanted" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_18189AB40 tracked-alloc 16, += 0x10; 1-byte bool payload
// + 7B tail pad).  RTTI TD rva 0x4D5D940, vtable rva 0x3C34F60 (4 slots): [0] dtor
// 0x1819A99AC (ICF trivial), [1] nullsub, [2] Accept -> visitor+0x270, [3] GetName ->
// "PlayerIsWanted" (verified).  make+post shape; caller sub_18189AAF0.

namespace wh::rpgmodule {

class C_PlayerIsWantedCause : public C_Cause<S_PlayerIsWantedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerIsWantedCause;
};
static_assert(sizeof(C_PlayerIsWantedCause) == 0x10,
              "C_PlayerIsWantedCause must be 0x10 (alloc 16 at sub_18189AB40)");

}  // namespace wh::rpgmodule
