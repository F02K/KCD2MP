#pragma once
#include <cstdint>
#include "guimodule/C_UIExtendedInfo.h"

// -----------------------------------------------
// wh::guimodule::C_UICodexCraftingContent -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_UICodexCraftingContent@guimodule@wh@@ (TD 0x184C91DD0).
// Bases: C_UIExtendedInfo @+0x00 (-> C_UIFlashBase -> C_UIBase) + IUIElementEventListener
// @+0x10 + IHardwareMouseEventListener @+0x58 -- identical shape to C_UIExtendedInfo.
// vtables 0x183B8F1B8 / 0x183B8F118 / 0x183B8F100.
//
// The shared codex/crafting content pane, flash element "ApseCraftingContent".
// Embedded BY VALUE in C_UIApseCrafting @+0x1F0 (ctor: C_UIExtendedInfo ctor
// sub_180BC8628(this, "ApseCraftingContent") then vtable swap @0x18146cbb3); size =
// C_UIExtendedInfo's 0xD8 (next member C_UIApseCrafting::m_pApse @+0x2C8 pins it).
// C_UIApseCodex::m_pCraftingContent points at the crafting panel's instance.
// NO additional members; own override bodies NOT traced this wave (UNVERIFIED roles).

namespace wh::guimodule {

class C_UICodexCraftingContent : public C_UIExtendedInfo {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UICodexCraftingContent;
};
static_assert(sizeof(C_UICodexCraftingContent) == 0xD8, "C_UICodexCraftingContent must be 0xD8");

}  // namespace wh::guimodule
