#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_IntermissionInvoker -- AI intermission request interface
// (KCD2 WHGame.dll 1.5.6, kd7u).  2 slots, NO virtual dtor.
// -----------------------------------------------
// Base of dialogmodule::C_DialogInstance @+0x20 (vtable 0x183E63C90, exactly 2 slots).
// Both slots on the C_DialogInstance impl are the same body sub_181A80210 -> set
// full+929 = 1 (intermission-pending flag). The binary vtable has NO dtor slot -- do not
// add a virtual dtor here. Slot names UNVERIFIED (coined from behavior).

namespace wh::xgenaimodule {

class I_IntermissionInvoker {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_IntermissionInvoker;
    virtual void _vf0() = 0;   // [0] sets intermission-pending flag (sub_181A80210)
    virtual void _vf1() = 0;   // [1] same body as [0] on C_DialogInstance
};
static_assert(sizeof(I_IntermissionInvoker) == 0x08);

}  // namespace wh::xgenaimodule
