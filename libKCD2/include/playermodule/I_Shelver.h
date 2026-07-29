#pragma once

// -----------------------------------------------
// wh::playermodule::I_Shelver -- "shelver" interface (shelve/restore a gameplay
// subsystem's state around player-mode transitions).  KCD2 WHGame.dll 1.5.6, kd7u.
// Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_Shelver@playermodule@wh@@.  No COL of its own (never most-derived).
// Slot count (4) read from the sole enumerated implementor vtable,
// xgenaimodule::C_CrimeShelver (0x184770758): [0] sub_1832A5EA0, [1] sub_1832A6554,
// [2] nullsub_1, [3] sub_181AA5660.  Slot roles UNRESOLVED; whether [0] is a
// virtual-dtor slot is UNVERIFIED (kept a plain slot here -- layout identical).
// Trailing slots beyond [3] cannot be excluded from a single implementor.

namespace wh::playermodule {

class I_Shelver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Shelver;
    virtual void _vf0() = 0;   // [0] [U role; dtor placement unverified]
    virtual void _vf1() = 0;   // [1] [U role]
    virtual void _vf2() = 0;   // [2] [U role]
    virtual void _vf3() = 0;   // [3] [U role]
};
static_assert(sizeof(I_Shelver) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::playermodule
