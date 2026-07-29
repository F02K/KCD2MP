#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UITreeList.h"
#include "guimodule/C_UICodexCraftingContent.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseCrafting -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x2D8.
// -----------------------------------------------
// RTTI .?AVC_UIApseCrafting@guimodule@wh@@ (TD 0x184C91D68). ctor sub_18146CB5C.
// Base C_UIBase only; vtable 0x183B8F160 (10 slots): [1] Init 0x180ED2884,
// [2] Deinit 0x182AFE45C, [4] Update 0x180567A34.
// Embedded BY VALUE in C_UIApse @+0x1E80 (size pinned by next embed C_UIApseCharacter
// @+0x2158).
//
// Crafting tab. Owns the codex/crafting shared content pane: the ctor runs the
// C_UIExtendedInfo ctor sub_180BC8628(+0x1F0, "ApseCraftingContent") then swaps the
// vtable to C_UICodexCraftingContent (0x18146cbb3); C_UIApseCodex::m_pCraftingContent
// points here.

namespace wh::guimodule {

class C_UIApse;

class C_UIApseCrafting : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseCrafting;
    C_UITreeList m_list;                    // +0x10   (0x1E0) flash element "ApseCraftingList" (ctor sub_180BC8484, a3=0)
    C_UICodexCraftingContent m_content;     // +0x1F0  (0xD8) flash element "ApseCraftingContent"
    C_UIApse*    m_pApse;                   // +0x2C8  owner hub (ctor a2)
    uint16_t     m_word2D0;                 // +0x2D0  ctor 0
    uint8_t      _pad2D2[6];                // +0x2D2
};
static_assert(sizeof(C_UIApseCrafting) == 0x2D8, "C_UIApseCrafting must be 0x2D8");
static_assert(offsetof(C_UIApseCrafting, m_content) == 0x1F0, "content pane at 0x1F0");

}  // namespace wh::guimodule
