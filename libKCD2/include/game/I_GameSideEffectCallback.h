#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::game::I_GameSideEffectCallback -- game side-effect callback (distinct from
// wh::game::I_EntitySideEffectCallback).  KCD2 WHGame.dll 1.5.6, kd7u.  sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI .?AVI_GameSideEffectCallback@game@wh@@.  2-slot vtable, NO dtor slot -- the earlier
// "[0] = dtor thunk" reading (from the C_CombatScene subobject) was WRONG: C_CombatScene's
// [0] (sub_1827C7D74) is a real callback body (caches sub_1809D385C() into scene+0x58),
// and C_UIMap implements BOTH slots as side-effect handlers.
//
// Registration (C_UIMap::Init 0x180ED0EBC / Deinit 0x182AFE6D0): the side-effect
// dispatcher (S_GameContext+0x18 -> vf+0x118 -> vf+0x38 object) vf+0x98 AddCallback
// (sideEffectId, listener*) / vf+0xA0 RemoveCallback.  The dispatcher also exposes
// vf+0x78 IsSideEffectActive(id) (used by the C_UIMap filter gate sub_180C4D10C).
//
// Slot roles read from the C_UIMap impls (side effects 0xC4/0xC5/0xC6 = map-feature
// unlock flags): [0] 0x181F4B680 force-hides the affected POI filter groups,
// [1] 0x181F4B710 re-evaluates them.  Names INFERRED; the deactivate/activate
// direction is not independently proven.

namespace wh::game {

class I_GameSideEffectCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_GameSideEffectCallback;
    virtual void OnSideEffectDeactivated(char sideEffectId) = 0;   // [0]  C_UIMap 0x181F4B680, C_CombatScene 0x1827C7D74
    virtual void OnSideEffectActivated(char sideEffectId) = 0;     // [1]  C_UIMap 0x181F4B710, C_CombatScene nullsub
};
static_assert(sizeof(I_GameSideEffectCallback) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::game
