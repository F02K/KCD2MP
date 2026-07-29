#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_RandomEventPerception -- random-event perception hook
// interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (modeled polymorphic).
// -----------------------------------------------
// RTTI TD rva 0x4ECB428.  No COL/vtable of its own (never most-derived).  Slot
// count (2) read from the sole enumerated implementor vtable,
// C_RandomEventPerception (0x18475CC50): [0] sub_1832BC38C, [1] sub_1832BC2AC.
// Interface/implementor slot split UNVERIFIED [U].

namespace wh::xgenaimodule {

class I_RandomEventPerception {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RandomEventPerception;
    virtual void _vf0() = 0;   // [0] [U role; dtor placement unverified]
    virtual void _vf1() = 0;   // [1] [U role]
};
static_assert(sizeof(I_RandomEventPerception) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
