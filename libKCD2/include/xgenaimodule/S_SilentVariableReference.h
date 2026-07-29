#pragma once
#include "S_ConstVariableReference.h"

// -----------------------------------------------
// wh::xgenaimodule::S_SilentVariableReference : S_ConstVariableReference --
// variable reference whose mutations do NOT fire callbacks (semantic difference
// only; KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x60 (same footprint as the const
// base; no new data [U -- assumed; own 10-slot vtable not individually
// decompiled]).
// -----------------------------------------------
// RTTI TD rva 0x4E8E6A0; COL rva 0x4102640; vtable 0x183A33228, 10 slots.

namespace wh::xgenaimodule {

class S_SilentVariableReference : public S_ConstVariableReference {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_SilentVariableReference;
};
static_assert(sizeof(S_SilentVariableReference) == 0x60, "S_SilentVariableReference must be 0x60 (no new data)");

}  // namespace wh::xgenaimodule
