#pragma once

// -----------------------------------------------
// wh::rpgmodule::I_Soul -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vtable only).
// -----------------------------------------------
// Primary base of C_Soul (RTTI CHD 0x1843F5060, mdisp 0; shares the ~60-slot primary vtable
// 0x183F44F28). The slot table was NOT enumerated (honest gap in the C_Soul deep map) -- only a
// dtor is declared here to give the base its vptr; do NOT rely on any slot index from this header.

namespace wh::rpgmodule {

class I_Soul {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Soul;
    virtual ~I_Soul() = default;   // slot position UNVERIFIED
};

}  // namespace wh::rpgmodule
