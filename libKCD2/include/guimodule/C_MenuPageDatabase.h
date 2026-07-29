#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTreeDatabase.h"
#include "databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"
#include "guimodule/C_UIMenuPage.h"

// -----------------------------------------------
// wh::guimodule::C_MenuPageDatabase -- the "menu_pages" tree DB (KCD2 1.5.6, kd7u).
// sizeof 0x78 (tree-template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CAEE98, vtable 0x183B50F08. Stack: C_ObjectDatabaseKeyIndexed<
// C_ObjectTreeDatabase<C_UIMenuPage>, C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuPage>>.
// GLOBAL STATIC @0x185326830 (ctor sub_1812BC924, base sub_1819AAB6C, vtable-install
// sub_1812BD118; ns "wh::guimodule::", name "menu_pages", group "ui").
// TREE vertical (no descriptors/rows; shared tree-parse sub_180C57E60). Load [9]
// 0x18181CB10; post-load [16] 0x1819ED444. Keyed on m_pageId (@element+0x08).

namespace wh::guimodule {

class C_MenuPageDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<C_UIMenuPage>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuPage>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MenuPageDatabase;
};
static_assert(sizeof(C_MenuPageDatabase) == 0x78, "C_MenuPageDatabase must be 0x78 (tree extent)");

}  // namespace wh::guimodule
