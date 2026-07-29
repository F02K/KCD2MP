#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"
#include "guimodule/E_ChoiceName.h"
#include "framework/C_LocalizedString.h"

// -----------------------------------------------
// wh::guimodule::C_UIMenuChoice -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20 (ALLOC-PROVEN).
// -----------------------------------------------
// rttr-reflected data class, element type of C_MenuChoiceDatabase ("menu_choices" tree DB).
// vtable 0x183BF13C8 = RTTR trio ONLY, no virtual dtor. Creator sub_182B8525C (operator
// new 32); default ctor sub_1816B6FF4, copy ctor sub_1816B6F84.
// rttr props: C_LocalizedString, E_ChoiceName::Type. Member names inferred from types.

namespace wh::guimodule {

class C_UIMenuChoice {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMenuChoice;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x181A6CAF4, get_derived 0x182B3E6B0

    E_ChoiceName::Type m_choiceName;           // +0x08  identity key (DefaultKeyExtractor)
    uint8_t _pad09[7];                         // +0x09
    wh::framework::C_LocalizedString m_text;   // +0x10  localized text [name INFERRED]
};
static_assert(sizeof(C_UIMenuChoice) == 0x20, "C_UIMenuChoice must be 0x20 (creator sub_182B8525C)");

}  // namespace wh::guimodule
