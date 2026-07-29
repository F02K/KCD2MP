#pragma once
#include <cstdint>
#include <string>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::framework::C_LocalizedString -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Thin polymorphic holder for a localizable UI string: {vfptr @+0x00, CryStringT<char>
// m_text @+0x08}.  Its whole job is to TAG a string field as "this is display text that
// goes through localization", so the editor/rttr treat it differently from a raw string.
//
// RTTI .?AVC_LocalizedString@framework@wh@@ (COL 0x1841C1F00).  Vtable 0x183E1DF40, 3
// slots -- ALL THREE are the RTTR_ENABLE() trio, NO virtual dtor (no localization happens
// in the vtable; resolution is external, via the rttr string converter / the loc system
// when the text is fetched):
//   [0] get_type         0x180D216C0 (supersedes the old "dtor -> destroys m_text" reading)
//   [1] get_ptr          0x1805F5DA0 -> `return this;` (ICF-folded identity; this is the
//                                    "get" used inline in the quest path sub_181F46F40)
//   [2] get_derived_info 0x180F42C14 -> {this, get_type()} (supersedes the old
//                                    "copy-out helper" reading)
//
// Ctors: default sub_1803D28B4 (sets vftable, m_text = empty), copy sub_1803D32D0.
// rttr registration: BIDIRECTIONAL type_converter with CryStringT<char>
//   - C_LocalizedString -> CryStringT (COL 0x18416EC50): produce the plain string
//   - CryStringT -> C_LocalizedString (COL 0x18416EC78): wrap a raw string/key
//   plus a constructor from `const CryStringT&` (0x18416E520) and a property_wrapper
//   exposing m_text.  So the type interconverts freely with a plain string.
//
// EMBEDDED BY VALUE throughout the data classes (it is NOT heap/shared):
//   * wh::questmodule::C_LogBase @+0x18 (m_text @+0x20) -- ctor sub_181F43240; this is the
//     QUEST-LOG / objective display-text field (the objective "name" the map shows and the
//     compass does NOT carry -- see C_CompassMark / the sub_180DC5F24 producer).
//   * guimodule menu data (C_UIMenuButton / C_UIMenuChoice / C_UIMenuPage /
//     C_UIMenuConfirmation) as an rttr property.
//
// m_text SEMANTICS: the authored localizable string -- a localization key / "@"-tag or the
// literal text as tabled.  It is stored verbatim (the vtable never resolves it); the plain
// display string is obtained through the rttr CryStringT converter / the localization
// system at fetch time.  [Key-vs-already-resolved is NOT fully certified -- the converter
// invoke fn was not decompiled; do not assume a runtime loc lookup without checking it.]

namespace wh::framework {

class C_LocalizedString {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LocalizedString;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x180D216C0, get_derived 0x180F42C14

    CryStringT<char> m_text;               // +0x08  authored localizable string (key/text)

    // Impl in src/framework/C_LocalizedString.cpp.
    static CryStringT<char> Resolve(const CryStringT<char>& label);
};
static_assert(sizeof(C_LocalizedString) == 0x10, "C_LocalizedString must be 0x10 (ctor sub_1803D28B4)");

}  // namespace wh::framework
