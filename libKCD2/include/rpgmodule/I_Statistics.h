#pragma once

// -----------------------------------------------
// wh::rpgmodule::I_Statistics -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vfptr only).
// -----------------------------------------------
// RTTI TD 0x184D8F1A0.  Primary base of C_Statistics (@+0x00; its vtable 0x183F4D250 holds
// 8 slots).  Slot semantics are NOT certified -- modeled as fillers; use the non-virtual
// C_Statistics API (GetCounterByName etc.) instead of calling through these.

namespace wh::rpgmodule {

class I_Statistics {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Statistics;
    // Uncertified fillers -- DO NOT call through them (8 slots on the concrete vtable).
    virtual void _vf0() = 0;   // [0]
    virtual void _vf1() = 0;   // [1]
    virtual void _vf2() = 0;   // [2]
    virtual void _vf3() = 0;   // [3]
    virtual void _vf4() = 0;   // [4]
    virtual void _vf5() = 0;   // [5]
    virtual void _vf6() = 0;   // [6]
    virtual void _vf7() = 0;   // [7]
};
static_assert(sizeof(I_Statistics) == 0x08, "I_Statistics is a vfptr-only interface");

}  // namespace wh::rpgmodule
