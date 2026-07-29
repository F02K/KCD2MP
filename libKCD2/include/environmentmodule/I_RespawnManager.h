#pragma once

// -----------------------------------------------
// wh::environmentmodule::I_RespawnManager -- respawn-manager interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183ED0608. Primary base of C_RespawnManager (herb/item respawn grids).
// Slot set not enumerated.

namespace wh::environmentmodule {

class I_RespawnManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RespawnManager;
    virtual ~I_RespawnManager();   // [0]; further slots not enumerated
};
static_assert(sizeof(I_RespawnManager) == 0x08, "I_RespawnManager is a vptr-only interface");

}  // namespace wh::environmentmodule
