#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/I_UIDragAndDropViewer.h"
#include "playermodule/I_UIHudHints.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudHints -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// RTTI .?AVC_UIHudHints@guimodule@wh@@ (TD 0x184CD1048).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135EC8), IUIElementEventListener @+0x10
// (COL 0x4135EF0), playermodule::I_UIHudHints @+0x58 (COL 0x4135F18),
// I_UIDragAndDropViewer @+0x60 (COL 0x4135F40).
// vtables: primary 0x183C34210 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C34170 (8), hints 0x183C341B8 (10), drag-drop 0x183C34130 (7).
// ctor sub_18188D4EC; rttr as_std_shared_ptr creator sub_18188D464 (alloc 0x70).
//
// Interaction-hint HUD (crosshair hints, hold progress, drag ghost). Init [1]
// sub_180C3D434: base Init, publishes the I_UIHudHints subobject into the playermodule
// object (*(*(ctx+0x128)+0x58)+0x130 = this+0x58), registers the inbound flash
// dispatcher "OnActionButtonActivated" -> {this, sub_182B8E068}.
// OnGameStart [5] sub_180EC848C / OnGameEnd [6] sub_180EC7B94: add/remove the
// {this, sub_1814E65E4} and {this, sub_18131242C} delegates on the game object's
// signal heads @game+0xB08 (+0x70/+0xA0 sub-lists).

namespace wh::guimodule {

class C_UIHudHints : public C_UIFlashBase,
                     public wh::playermodule::I_UIHudHints,
                     public I_UIDragAndDropViewer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudHints;
    // C_UIBase overrides.
    void OnGameStart(void* pGame) override;   // [5] 0x180EC848C
    void OnGameEnd(void* pGame) override;     // [6] 0x180EC7B94
    // I_UIHudHints override (sampled).
    void SetProgress(float progress, int a3, int a4) override;   // [H1] 0x180C4488C flash "SetProgress"
    // I_UIDragAndDropViewer overrides.
    void ShowDraggedAlternate(uint64_t itemId) override;         // [D1] 0x18170BFF0
    void HideDraggedAlternate() override;                        // [D2] 0x1816EA33C
    void SetDraggedAlternateState(uint8_t state) override;       // [D3] 0x181843BA4

    uint64_t m_68;   // +0x68  ctor 0; not written by Init (role UNVERIFIED)
};
static_assert(sizeof(C_UIHudHints) == 0x70, "C_UIHudHints must be 0x70");

}  // namespace wh::guimodule
