#pragma once
#include <cstdint>
#include "guimodule/C_UIMenuBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIPhotomodeMenu -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xF8.
// -----------------------------------------------
// RTTI .?AVC_UIPhotomodeMenu@guimodule@wh@@ (TD 0x184CD9B98).
// Bases (CHD): C_UIMenuBase @+0x00 (binds the shared "Menu" movie), IUIElementEventListener
// @+0x10. vtables: primary 0x183EF5640 (14 slots: [0] dtor 0x181F8B290, [1] Init
// 0x181F8CAD0, [2] Deinit 0x181F8C2F0, [4] Update = inherited C_UIFlashBase drain
// 0x180533878, [11] GetElementName 0x181A8F230 "Menu"), listener 0x183EF55F8.
// ctor sub_181F89F00 (C_UIFlashBase ctor sub_180BC86C4 + own vtables + members).
//
// NOT a module screen: lives EMBEDDED in C_UISettings at +0x398 (C_UISettings ctor
// @0x180600658; tail of the 0x490 object), i.e. at C_UIMenu +0x570. Drives the
// RootPhotomode menu page (E_MenuPage 20): the C_UISettings composite sub_181F8DE50
// does PreparePage(20) -> flash "ShowRootMenu" -> BuildCvarWidgets (sub_181F8DDA0 on
// this subobject) -> ShowPage.
//
// The page is CVAR-DRIVEN, not E_ButtonId-driven: the ctor's five registrar calls
// (sub_181F829F0/82C00/82940/82B50/82AA0) populate m_widgetHandlers keyed off the
// global photomode cvar list @0x185593560 (wh_ui_photomode*); BuildCvarWidgets walks
// that list and adds one widget per cvar via sub_181F8B780, with special handling for
// wh_ui_photomodeVignetteIntensity (sub_181F8E0F0) and wh_ui_photomodeLogoType
// (sub_181F8DEE0).
//
// [FUNDAMENTAL vs KCD1] KCD1 has no photomode; no counterpart.

namespace wh::guimodule {

class C_UIPhotomodeMenu : public C_UIMenuBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIPhotomodeMenu;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x181F8CAD0  (called from C_UISettings::Init via vf[1])
    void Deinit() override;                     // [2] 0x181F8C2F0

    // --- Native methods (RVA forwarders in src/guimodule/C_UIPhotomodeMenu.cpp) ---
    // Emit one widget per photomode cvar (global list @0x185593560) onto the prepared
    // RootPhotomode page. Caller must have run PreparePage(RootPhotomode) first --
    // the C_UISettings wrapper sub_181F8DE50 is the canonical sequence.
    void BuildCvarWidgets();                    // 0x181F8DDA0

    // MSVC std::unordered_map shape (max_load 1.0f @+0x58, list {sentinel, size}
    // @+0x60, bucket vector @+0x70, _Mask 7 @+0x88, _Maxidx 8 @+0x90); 0x58-byte list
    // nodes. Populated by the ctor's five registrar calls; key/value types UNVERIFIED
    // (cvar-name -> widget/apply handler by usage).
    uint8_t  m_widgetHandlers[0x40];   // +0x58
    uint8_t  m_vec98[0x18];            // +0x98  std::vector shape, ctor {0,0,0}; element type UNVERIFIED
    uint8_t  m_unkB0[0x40];            // +0xB0  sub-struct built by sub_181F89B40(this+0xB0, &int0); internals UNVERIFIED
    int32_t  m_unkF0;                  // +0xF0  ctor 0; role UNVERIFIED
    uint8_t  _padF4[4];                // +0xF4
};
static_assert(sizeof(C_UIPhotomodeMenu) == 0xF8, "C_UIPhotomodeMenu must be 0xF8 (C_UISettings +0x398..+0x490)");
static_assert(offsetof(C_UIPhotomodeMenu, m_widgetHandlers) == 0x58, "widget map at 0x58 (ctor 0x181F89F0D..)");
static_assert(offsetof(C_UIPhotomodeMenu, m_unkF0) == 0xF0, "tail int at 0xF0");

}  // namespace wh::guimodule
