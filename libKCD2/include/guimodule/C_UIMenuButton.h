#pragma once
#include <cstdint>
#include <functional>
#include "rttr/rttr_enable.h"
#include "guimodule/E_ButtonId.h"
#include "framework/C_LocalizedString.h"

// -----------------------------------------------
// wh::guimodule::C_UIMenuButton -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (ALLOC-PROVEN).
// -----------------------------------------------
// rttr-reflected data class, element type of C_MenuButtonDatabase ("menu_buttons" tree DB,
// deserialized from XML by class name -- NOT a table row). vtable 0x183AB1A70 = RTTR trio
// ONLY, no virtual dtor. Creator sub_182B851BC (operator new 120); batch ctor sub_180DC1400
// (memset 0x78); default ctor sub_180DC1444, copy ctor sub_180DC1488, get_type sub_181A6CB10.
// Runtime DB = byte-keyed sorted vector @0x1853266E0 (lookup sub_181441328, row getter
// sub_180558E78(pScreen, id)).
//
// Member names VERIFIED against the widget helpers (menu_wrappers.md / menu_page_builders.md):
// AddBasicButton sub_180558CCC sends flash ("AddBasicButton", name=sub_180558EB8(m_buttonId),
// m_containerIndex, text=sub_18039956C(row), tooltip=sub_180558E98(row) -> copy of +0x28,
// !enabled) and move-assigns the click handler into m_onClick (sub_1808E71DC). The flash
// OnButton dispatch sub_180C07A98 maps the name back to the id (sub_180C07AE4) and invokes
// m_onClick -- so the row doubles as the button's click-handler registry.

namespace wh::guimodule {

class C_UIMenuButton {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMenuButton;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x181A6CB10, get_derived 0x182B3E690

    E_ButtonId::Type m_buttonId;                  // +0x08  identity key (DefaultKeyExtractor)
    uint8_t _pad09[7];                            // +0x09
    wh::framework::C_LocalizedString m_text;      // +0x10  button label -- resolved via sub_18039956C, sent as flash "uiText"
    wh::framework::C_LocalizedString m_tooltip;   // +0x20  default tooltip -- sub_180558E98 copies the CryStringT half (+0x28); overridable per Add* call
    int32_t m_containerIndex;                     // +0x30  flash "containerIndex" (column/list the button lands in) -- sent by every Add*/Set* helper
    uint8_t _pad34[4];                            // +0x34
    std::function<void()> m_onClick;              // +0x38  click handler (0x40, impl ptr @+0x70): assigned by AddBasicButton (sub_1808E71DC), built in-place as _Func_impl_no_alloc (e.g. sub_181F809C0), invoked by the OnButton dispatch sub_180C07A98
};
static_assert(sizeof(C_UIMenuButton) == 0x78, "C_UIMenuButton must be 0x78 (creator sub_182B851BC)");
static_assert(offsetof(C_UIMenuButton, m_containerIndex) == 0x30, "containerIndex at 0x30 (read *(row+48))");
static_assert(offsetof(C_UIMenuButton, m_onClick) == 0x38, "click handler at 0x38 (assign sub_1808E71DC, invoke sub_180C07A98)");

}  // namespace wh::guimodule
