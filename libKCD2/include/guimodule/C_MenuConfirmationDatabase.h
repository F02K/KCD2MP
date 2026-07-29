#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTreeDatabase.h"
#include "databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"
#include "guimodule/C_UIMenuConfirmation.h"

// -----------------------------------------------
// wh::guimodule::C_MenuConfirmationDatabase -- the "menu_confirmations" tree DB
// (KCD2 1.5.6, kd7u).  sizeof 0x78 (tree-template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CAEC58, vtable 0x183B50FC0. Stack: C_ObjectDatabaseKeyIndexed<
// C_ObjectTreeDatabase<C_UIMenuConfirmation>,
// C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuConfirmation>>.
// GLOBAL STATIC @0x1853267B0 (ctor sub_1812BC884, base sub_1812BD280, vtable-install
// sub_1812BD13C; ns "wh::guimodule::", name "menu_confirmations", group "ui").
// TREE vertical (no descriptors/rows; shared tree-parse sub_180C57E60). Load [9]
// 0x180EF1484; post-load [16] 0x1819ED47C. Keyed on m_confirmationId (@element+0x08).

namespace wh::guimodule {

class C_MenuConfirmationDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<C_UIMenuConfirmation>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<C_UIMenuConfirmation>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MenuConfirmationDatabase;
};
static_assert(sizeof(C_MenuConfirmationDatabase) == 0x78, "C_MenuConfirmationDatabase must be 0x78 (tree extent)");

}  // namespace wh::guimodule
