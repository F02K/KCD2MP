#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UITreeList.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseCodex -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x228.
// -----------------------------------------------
// RTTI .?AVC_UIApseCodex@guimodule@wh@@ (TD 0x184C91E10). ctor sub_18146CAC4(this,
// owner, &owner.m_crafting). Base C_UIBase only; vtable 0x183B8F078 (10 slots):
// [1] Init 0x180ED28E8, [2] Deinit 0x182AFE418, [4] Update 0x180567B54.
// Embedded BY VALUE in C_UIApse @+0x1C58 (size pinned by next embed C_UIApseCrafting
// @+0x1E80).
//
// Codex tab. Shares the crafting panel's content pane: m_pCraftingContent aliases
// &C_UIApseCrafting::m_content (passed via the ctor's a3).

namespace wh::guimodule {

class C_UIApse;
class C_UICodexCraftingContent;

class C_UIApseCodex : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseCodex;
    C_UITreeList m_list;                          // +0x10  (0x1E0) flash element "ApseCodexList" (ctor sub_180BC8484, a3=0)
    C_UICodexCraftingContent* m_pCraftingContent; // +0x1F0  -> C_UIApseCrafting::m_content ("ApseCraftingContent", shared pane)
    C_UIApse*    m_pApse;                         // +0x1F8  owner hub (ctor a2)
    float        m_vec200[4];                     // +0x200  ctor = xmmword_183B8F060 (16-byte constant; NOT modelled as __m128 -- the Apse embed offset 0x1C58 is not 16-aligned) [role UNVERIFIED]
    float        m_float210;                      // +0x210  ctor -1.0f [role UNVERIFIED]
    uint8_t      _pad214[4];                      // +0x214
    CryStringT<char> m_str218;                    // +0x218  ctor empty magic-static [role UNVERIFIED]
    uint16_t     m_word220;                       // +0x220  ctor 0
    uint8_t      _pad222[6];                      // +0x222
};
static_assert(sizeof(C_UIApseCodex) == 0x228, "C_UIApseCodex must be 0x228");
static_assert(offsetof(C_UIApseCodex, m_pCraftingContent) == 0x1F0, "crafting-content alias at 0x1F0");

}  // namespace wh::guimodule
