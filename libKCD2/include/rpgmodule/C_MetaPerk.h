#pragma once
#include <cstdint>
#include <vector>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_MetaPerk -- perk grouping child perks (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_MetaPerk@rpgmodule@wh@@ (TD 0x184D53E50)  vtable 0x183B68B78.
// sizeof 0x60 (PROVEN: factory sub_18136F388 operator new(96)).
// Ctor zeroes +0x30..+0x5F: modeled as two std::vectors (child perk collections -- element
// types UNRESOLVED, container reading INFERRED from the six zeroed qwords).

namespace wh::rpgmodule {

class C_MetaPerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MetaPerk;
    std::vector<void*> m_children30;   // +0x30  [element type UNRESOLVED]
    std::vector<void*> m_children48;   // +0x48  [element type UNRESOLVED]
};
static_assert(sizeof(C_MetaPerk) == 0x60, "C_MetaPerk must be 0x60 (operator new(96))");

}  // namespace wh::rpgmodule
