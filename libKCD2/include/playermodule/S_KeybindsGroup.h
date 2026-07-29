#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::S_KeybindsGroup -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// STRUCT NAME EVIDENCED: std::function signatures over the m_uiGroups map mangle the value
// type as US_KeybindsGroup@playermodule@wh@@ (U = struct tag).  One row per <ui_group> of
// Libs/Config/keybindSuperactions.xml; value of C_Keybinds::m_uiGroups (node 0x30).
// Layout pinned by the parser sub_1808FB4CC ui_group branch (@0x1808FB5AC..): "name" attr
// -> +0x00, "ui_label" attr -> +0x08, running ordinal -> +0x10; find-or-insert
// sub_182E14088(mgr+0x50, name), fill sub_181378D0C.  Groups the superaction rows into
// captioned sections of the settings keybinds page.

namespace wh::playermodule {

struct S_KeybindsGroup {
    CryStringT<char> m_name;     // +0x00  "name" attr; duplicates the map key
    CryStringT<char> m_uiLabel;  // +0x08  "ui_label" attr (localization key for the section caption)
    int32_t          m_index;    // +0x10  load-order ordinal (page section order)
    uint32_t         _pad14;     // +0x14
};
static_assert(sizeof(S_KeybindsGroup) == 0x18, "S_KeybindsGroup must be 0x18");

}  // namespace wh::playermodule
