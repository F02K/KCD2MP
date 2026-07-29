#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryAction/IActionMapEventListener.h"
#include "I_ActionSets.h"

// -----------------------------------------------
// wh::playermodule::C_ActionSets -- the player-module contextual-action registry
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x90.
// -----------------------------------------------
// RTTI .?AVC_ActionSets@playermodule@wh@@.  MI: IActionMapEventListener merged into the primary
// vtable @+0x00 (0x183F60D18, 2 slots: [0] dtor 0x182E14244, [1] OnActionMapEvent 0x18085B2B4)
// + I_ActionSets @+0x08 (0x183F60C88, 17 slots).  ctor sub_180BE9E68; owned by C_PlayerModule,
// which stores THIS+8 (the I_ActionSets subobject) at +0x60.
// Full RE: analysis/alchemy_re/action_registration_re.md.
//
// Two containers only:
//   m_registry (+0x10) -- DURABLE: context name -> context node (enabled byte node+24, list of
//     action-map entries; per-action callback std::function<void()> at entry+0x70, secondary
//     channel at +0xB0).  Real node size 104B; survives context enable/disable.
//   m_active   (+0x50) -- the ACTIVE dispatch filter: action name -> live entry (functor read at
//     node+0xA8 by TriggerAction).  Real node size 248B; rebuilt from m_registry on every
//     context enable/disable (OnActionMapEvent type 3 -> sub_18085B3D8 -> sub_1808E65A0).
// Node/value types are NOT modeled (map OBJECT layout only) -- never call methods on these
// members; go through the I_ActionSets interface.
// Context activation itself lives in CryEngine's IActionMapManager (EnableActionMap, vtbl+232);
// minigames keep one current-context string (C_Alchemy+0x30) -- push = disable old + enable new.

namespace wh::playermodule {

class C_ActionSets
    : public IActionMapEventListener   // +0x00 (primary vtable)
    , public I_ActionSets {            // +0x08 (the pointer C_PlayerModule stores)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActionSets;

    // Fire a registered action by name through the game's only invoke path: looks the name up in
    // m_active (so it fires ONLY while its context is enabled) and runs the nullary callback.
    // BY VALUE like the original (the impl tail-releases the param @0x18085A3CD; game callers
    // copy-construct and never release, e.g. the hint wrap 0x181FFEF20).
    // (src/playermodule/C_ActionSets.cpp)
    void TriggerAction(CryStringT<char> action);   // 0x18085A360

    std::unordered_map<CryStringT<char>, void*> m_registry;  // +0x10  durable context registry
    std::unordered_map<CryStringT<char>, void*> m_active;    // +0x50  active dispatch filter
};
static_assert(sizeof(std::unordered_map<CryStringT<char>, void*>) == 0x40,
              "MSVC unordered_map object must be 0x40 for this layout");
static_assert(sizeof(C_ActionSets) == 0x90, "C_ActionSets must be 0x90");
static_assert(offsetof(C_ActionSets, m_registry) == 0x10, "registry at 0x10");
static_assert(offsetof(C_ActionSets, m_active) == 0x50, "active map at 0x50");

}  // namespace wh::playermodule
