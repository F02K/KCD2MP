#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTreeDatabase.h"
#include "databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"
#include "guimodule/C_UIMenuButton.h"

// -----------------------------------------------
// wh::guimodule::C_MenuButtonDatabase -- the "menu_buttons" tree DB (KCD2 1.5.6, kd7u).
// sizeof 0x78 (tree-template extent -- global static; globals laid out on 0x80 stride).
// -----------------------------------------------
// RTTI TD RVA 0x4CB01A8, vtable 0x183B51130. Stack: C_ObjectDatabaseKeyIndexed<
// C_ObjectTreeDatabase<C_UIMenuButton>, C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuButton>>.
// GLOBAL STATIC @0x1853266B0 (ctor sub_1812BC750, base sub_1812BD324, vtable-install
// sub_1812BD184; ns "wh::guimodule::", name "menu_buttons", group "ui").
// TREE vertical: NO column descriptors (Register [7] = ret-stub 0x1803B6E80), NO row
// struct; elements are rttr-deserialized C_UIMenuButton objects (shared tree-parse
// ParseRows [10] sub_180C57E60). Load [9] 0x18186602C; post-load [16] 0x1819ED4DC.
// KeyIndexed keys on C_UIMenuButton::m_buttonId (E_ButtonId::Type @element+0x08).

namespace wh::guimodule {

class C_MenuButtonDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<C_UIMenuButton>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuButton>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MenuButtonDatabase;
};
static_assert(sizeof(C_MenuButtonDatabase) == 0x78, "C_MenuButtonDatabase must be 0x78 (tree extent)");

}  // namespace wh::guimodule
