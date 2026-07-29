#pragma once

// -----------------------------------------------
// wh::playermodule::I_AlchemyMannequinListener -- anim-event listener base of the alchemy leaf
// actions (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vptr only).
// -----------------------------------------------
// [SYNTHETIC NAME] -- the listener subobject the anim-listening leaves embed as their THIRD base
// at +0x68 (C_UseBellowsAction listener vtable 0x183F60200; mortar/retort have their own).  It is
// registered with the Mannequin anim-event dispatcher by sub_1808C0194 (into GameContext[48]+968/
// +984) with a per-leaf "handled" callback (bellows sub_1806C64F0, mortar sub_1806C38A8, retort
// sub_182E17FD4), which routes fired fragment events to the action's OnAnimEvent / crc dispatch.
// Slot map UNVERIFIED (only the deleting-dtor adjustor is certain) -- do not call through this
// base; it exists to give the +0x68 vptr its real place in the leaf layouts.

namespace wh::playermodule {

class I_AlchemyMannequinListener {
public:
    virtual ~I_AlchemyMannequinListener() = default;   // [0] adjustor deleting dtor; further slots UNVERIFIED
};
static_assert(sizeof(I_AlchemyMannequinListener) == 8, "vptr-only listener base");

}  // namespace wh::playermodule
