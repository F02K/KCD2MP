#pragma once
#include "S_ConstVariableReference.h"

// -----------------------------------------------
// wh::xgenaimodule::S_VariableReference : S_ConstVariableReference -- mutable
// variable reference (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x60 (same footprint
// as the const base; derived adds NO data [U -- assumed from identical dtor
// behavior; own 10-slot vtable not individually decompiled]).
// -----------------------------------------------
// RTTI TD rva 0x4FFD288; COL rva 0x40CCF90; vtable 0x183A43A48, 10 slots.
// Embedded pairs of these live in C_SmartObjectSubBrain (+0x1B0/+0x210) and
// C_SmartAreaSubBrain (+0x1C8/+0x228).

namespace wh::xgenaimodule {

class S_VariableReference : public S_ConstVariableReference {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_VariableReference;
};
static_assert(sizeof(S_VariableReference) == 0x60, "S_VariableReference must be 0x60 (no new data)");

}  // namespace wh::xgenaimodule
