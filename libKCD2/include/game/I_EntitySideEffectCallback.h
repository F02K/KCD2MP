#pragma once

// -----------------------------------------------
// wh::game::I_EntitySideEffectCallback -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Callback interface for per-entity side-effect add/remove notifications.
// SHAPE CORRECTED (was "[0] dtor / [1] OnEntitySideEffect"): the
// C_RecognizingThresholdManager impl proves BOTH slots are real handlers and
// there is NO virtual dtor -- [0] sub_1808ACF2C push_backs a per-entity
// threshold float (side-effect ADDED, @0x1808acf35), [1] sub_1808AC6F4
// erase-removes matching floats (side-effect REMOVED, @0x1808ac737).  Mirrors
// the same correction already made on I_GameSideEffectCallback.
// Other impls: C_CombatAutomationWeapons (secondary base, mdisp 0x38; both
// slots ICF-folded onto sub_182767740), C_LightSourceManager, C_NPCManager
// (subobject vt 0x183FF1930), C_SituationManager, C_CrimeLevelAggregator.

namespace wh::game {

class I_EntitySideEffectCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_EntitySideEffectCallback;
    virtual void OnEntitySideEffectAdded(void* sideEffect) = 0;    // [0] fires when a side-effect is added to an entity
    virtual void OnEntitySideEffectRemoved(void* sideEffect) = 0;  // [1] fires when a side-effect is removed
};

}  // namespace wh::game
