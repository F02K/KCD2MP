#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_RPGPerceptionHelper -- RPG-side perception hook interface
// (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface, 2 slots, NO virtual dtor.
// -----------------------------------------------
// RTTI TD rva 0x4FF3730 (never most-derived, no own COL). Implemented by
// S_RPGPerceptionHelper (embedded in C_XGenAIModule @+0xA0, shared vtable
// 0x184772508, exactly 2 slots -- do NOT add a virtual dtor).

namespace wh::xgenaimodule {

class I_RPGPerceptionHelper {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RPGPerceptionHelper;
    virtual void* GetPerceptibleVolumeData(const void* wuid) = 0;  // [0] volume lookup bridge (rpg -> xgenai)
    virtual void* GetStealthWatcher() = 0;                         // [1] returns the lazy C_StealthWatcher singleton
};
static_assert(sizeof(I_RPGPerceptionHelper) == 0x08, "I_RPGPerceptionHelper is vptr-only");

}  // namespace wh::xgenaimodule
