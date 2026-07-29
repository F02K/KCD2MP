#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"
#include "guimodule/E_MenuPage.h"
#include "framework/C_LocalizedString.h"

// -----------------------------------------------
// wh::guimodule::C_UIMenuPage -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// rttr-reflected data class, element type of C_MenuPageDatabase ("menu_pages" tree DB).
// vtable 0x183BF57D0 = RTTR trio ONLY, no virtual dtor. Creator sub_182BA428C (operator
// new 48); default ctor sub_1816D9514, copy ctor sub_1816D947C.
// Runtime DB = byte-keyed sorted vector @0x185326860 (lookup sub_180F69570, row getter
// sub_180F69558(pMenu, id)).
//
// Member names VERIFIED against C_UIMenu::PreparePage (sub_180F69448): it sends flash
// ("PreparePage", m_containerX, m_containerY, m_maxButtons, header=sub_18039956C(row),
// m_buttonHalfWidth) -- the menu_pages.xml columns ContainerX/ContainerY/MaxButtons/
// [Button]HalfWidth (ctor defaults 1500/370/8/196).

namespace wh::guimodule {

class C_UIMenuPage {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMenuPage;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x181A6CABC, get_derived 0x182B3E6F0

    E_MenuPage::Type m_pageId;                 // +0x08  identity key (DefaultKeyExtractor), ctor 0
    uint8_t _pad09[7];                         // +0x09
    wh::framework::C_LocalizedString m_header; // +0x10  page header text (flash "PreparePage" string arg via sub_18039956C)
    int32_t m_containerX;                      // +0x20  layout X, default 1500 (menu_pages.xml ContainerX)
    int32_t m_containerY;                      // +0x24  layout Y, default 370  (ContainerY)
    int32_t m_maxButtons;                      // +0x28  visible rows before scroll, default 8 (MaxButtons)
    int32_t m_buttonHalfWidth;                 // +0x2C  button half-width px, default 196 (HalfWidth)
};
static_assert(sizeof(C_UIMenuPage) == 0x30, "C_UIMenuPage must be 0x30 (creator sub_182BA428C)");
static_assert(offsetof(C_UIMenuPage, m_containerX) == 0x20, "layout ints at 0x20 (PreparePage reads row+32..44)");

}  // namespace wh::guimodule
