#pragma once
#include <cstdint>
#include <set>
#include <utility>
#include "S_KeybindsSuperactionSlots.h"

// -----------------------------------------------
// wh::playermodule::S_KeybindsSuperaction -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x128.
// -----------------------------------------------
// STRUCT NAME EVIDENCED: std::function signatures over the superaction map mangle the value
// type as US_KeybindsSuperaction@playermodule@wh@@ (U = struct tag).  One record per
// <superaction> of Libs/Config/keybindSuperactions.xml; value of C_Keybinds::m_superactions
// (map node 0x140: next/prev/key CryStringT/value -- record = node+0x18).
// Size proven three ways: operator new(0x140) - 0x18 (sub_181244660); memset(rec,0,0x128) in
// the node ctor sub_181FFCDF0 (@0x181FFCE11); 0x38 header + 3*0x50 blocks.  Default ctor
// sub_1815F076C; filled by the parser sub_1808FB4CC (@0x1808FB68C..0x1808FBBA8).
// A superaction bundles the engine actions it drives (m_actions) with per-controller
// binding slots (m_controllers) and the UI presentation row (group/name/tooltip).

namespace wh::playermodule {

struct S_KeybindsSuperaction {
    CryStringT<char> m_name;       // +0x00  "name" attr; duplicates the map key (loader 0x1808FB6DC)
    CryStringT<char> m_uiGroup;    // +0x08  "ui_group" attr -> S_KeybindsGroup key (0x1808FB701)
    int32_t          m_index;      // +0x10  load-order ordinal (row order inside the page; 0x1808FB755)
    uint32_t         _pad14;       // +0x14  never written (memset 0) -- purpose UNVERIFIED, modeled as padding
    CryStringT<char> m_uiName;     // +0x18  "ui_name" attr (row caption localization key; 0x1808FB725)
    CryStringT<char> m_uiTooltip;  // +0x20  "ui_tooltip" attr (0x1808FB746)
    // (actionMap, actionName) pairs from <action map= name=>, plus the same action for every
    // ancestor action map (loop 0x1808FB953).  std::_Tree is certain ("map/set too long"
    // _Xlength_error in the inserter sub_1808FBEC8); the set-of-pair model and the comparator
    // (sub_1808FC33C) are UNVERIFIED in detail.
    std::set<std::pair<CryStringT<char>, CryStringT<char>>> m_actions;  // +0x28
    S_KeybindsSuperactionSlots m_controllers[3];  // +0x38  indexed by E_KeybindController::Type (keyboard/xboxpad/ps4pad)
};
static_assert(sizeof(S_KeybindsSuperaction) == 0x128, "S_KeybindsSuperaction must be 0x128");
static_assert(offsetof(S_KeybindsSuperaction, m_actions) == 0x28, "action set at 0x28");
static_assert(offsetof(S_KeybindsSuperaction, m_controllers) == 0x38, "controller blocks at 0x38");

}  // namespace wh::playermodule
