#pragma once
#include <cstdint>
#include <vector>
#include "C_SoundPropagationLayerBase.h"
#include "../game/I_GameSideEffectCallback.h"

// -----------------------------------------------
// wh::xgenaimodule::C_HearingSystem : game::I_GameSideEffectCallback -- THE
// hearing hub: cvars + sound-category ids + the 8-layer sound-propagation grid
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x168 (create-site proven: alloc 360 in
// the lazy-init getter sub_180B37DE0 -> qword_185493920).
// -----------------------------------------------
// RTTI TD rva 0x4F49D90; vtable 0x183FE6888, 2 slots ([0] sub_181AA5720,
// [1] sub_181AA5730 -- the two I_GameSideEffectCallback slots; NO dtor slot, the
// base has none).  ctor sub_180A4B310 heap-allocates and pushes the layers into
// m_layers: Noisiness(8) / Weather(16) / CircularQuery(24, +0x10 = this) /
// Obstacles(16) / TargetRPG(16) / PlayerSurroundings(8) / AddIntensities(8) /
// base.  Cvar registrar qword_18492D8A8.
// CHEAT LEVERS (mute): m_hearingEnabled (+0x08) = 0 disables the entire hearing
// ingestion/sound grid; huge m_distanceAttenuationPower (+0x0C) or zero event
// intensity collapses the propagation radius to ~0.

namespace wh::rpgmodule { class C_RPGHearing; }

namespace wh::xgenaimodule {

class C_HearingSystem : public game::I_GameSideEffectCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HearingSystem;
    void OnSideEffectDeactivated(char sideEffectId) override;  // [0] sub_181AA5720 [U slot direction, per interface header]
    void OnSideEffectActivated(char sideEffectId) override;    // [1] sub_181AA5730

    int32_t  m_hearingEnabled;             // +0x08  cvar wh_ai_hearing_enabled (1) -- MASTER hearing toggle [V]
    float    m_distanceAttenuationPower;   // +0x0C  cvar wh_ai_hearing_distance_attenuation_power [V]
    int32_t  m_parallelUpdate;             // +0x10  cvar wh_ai_hearing_parallelUpdate (1)
    int32_t  m_parallelUpdateSize;         // +0x14  cvar wh_ai_hearing_parallelUpdateSize (150)
    float    m_playerSurroundingsAttenuation;  // +0x18  cvar wh_ai_PlayerSurroundingsAttenuation
    uint8_t  _pad1C[4];                    // +0x1C
    std::vector<int32_t> m_soundCategoryIds;   // +0x20  28 sound-category ids [V]
    uint8_t  _unk38[0x18];                 // +0x38..+0x4F  UNRESOLVED: 3 qwords, ctor-zeroed like the 4 sibling std::vectors; likely std::vector<T> but no writer found in reachable hearing members
    wh::rpgmodule::C_RPGHearing* m_soundCategoryRegistry;  // +0x50  &C_RPGHearing global (0x18492EC80); vslot0 GetSoundCategory(idx)
    std::vector<C_SoundPropagationLayerBase*> m_layers;  // +0x58  the 8-layer propagation grid [V]
    uint8_t  _unk70[0x38];                 // +0x70..+0xA7 [U roles]
    std::mutex m_eventQueueLock;           // +0xA8  (0x50; _Mtx_try@+0xA8=2, cs-storage +0xB0..+0xEF, owner-thread@+0xF0=-1, count@+0xF4=0) guards m_soundEventQueue
    uint8_t  m_blockF8[0x68];              // +0xF8..+0x15F  member block (init sub_18197ACF0) [U interior]
    uint8_t  m_tail160[0x08];              // +0x160  member block (init sub_18078BDEC) [U interior]
};
static_assert(sizeof(C_HearingSystem) == 0x168, "C_HearingSystem must be 0x168 (360)");

}  // namespace wh::xgenaimodule
