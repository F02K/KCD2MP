#pragma once

// -----------------------------------------------
// wh::playermodule::I_MinigameCamera -- minigame camera-control interface (KCD2 1.5.6, kd7u).
// sizeof 0x08 (vptr only).
// -----------------------------------------------
// Second MI base of the minigame sessions (C_Alchemy subobject at +0x08, vtable 0x183F60AD0).
// Owns the minigame camera takeover (the AlchemyTable camera-limit-angle Lua properties feed this
// layer).  Slot map UNVERIFIED -- only the deleting-dtor position is certain; do not call through
// this base.

namespace wh::playermodule {

class I_MinigameCamera {
public:
    virtual ~I_MinigameCamera() = default;   // [0]; further slots UNVERIFIED
};
static_assert(sizeof(I_MinigameCamera) == 8, "I_MinigameCamera is a vptr-only interface");

}  // namespace wh::playermodule
