#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::storm::C_StormProgram -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI-CONFIRMED name (vtable set in sub_181023BEC; dtor sub_18181C604). Embedded in C_SoulList
// @+0x100178. "storm" appears to be Warhorse's soul-simulation/scheduling VM ("storm program").
// Interior NOT tiled (honest gap) -- flagged qwords only.

namespace wh::rpgmodule::storm {

class C_StormProgram {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StormProgram;
    virtual ~C_StormProgram() = default;   // slot order UNVERIFIED
    uint64_t m_unk08[15];                  // +0x08..+0x80  unresolved interior
};
static_assert(sizeof(C_StormProgram) == 0x80, "C_StormProgram must be 0x80");

}  // namespace wh::rpgmodule::storm
