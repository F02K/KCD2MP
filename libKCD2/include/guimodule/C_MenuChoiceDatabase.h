#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTreeDatabase.h"
#include "databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"
#include "guimodule/C_UIMenuChoice.h"

// -----------------------------------------------
// wh::guimodule::C_MenuChoiceDatabase -- the "menu_choices" tree DB (KCD2 1.5.6, kd7u).
// sizeof 0x78 (tree-template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CB03E8, vtable 0x183B51078. Stack: C_ObjectDatabaseKeyIndexed<
// C_ObjectTreeDatabase<C_UIMenuChoice>, C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuChoice>>.
// GLOBAL STATIC @0x185326730 (ctor sub_1812BC7F0, base sub_1819AAC28, vtable-install
// sub_1812BD160; ns "wh::guimodule::", name "menu_choices", group "ui").
// TREE vertical (no descriptors/rows; shared tree-parse sub_180C57E60). Load [9]
// 0x18181CE60; post-load [16] 0x1819ED4B4. Keyed on m_choiceName (@element+0x08).

namespace wh::guimodule {

class C_MenuChoiceDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<C_UIMenuChoice>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuChoice>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MenuChoiceDatabase;
};
static_assert(sizeof(C_MenuChoiceDatabase) == 0x78, "C_MenuChoiceDatabase must be 0x78 (tree extent)");

}  // namespace wh::guimodule
