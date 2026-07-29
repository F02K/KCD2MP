#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "databasemodule/C_ObjectDatabaseIdIndexed.h"
#include "guimodule/S_CreditsLayout.h"
#include "guimodule/S_CreditsLayoutDBData.h"

// -----------------------------------------------
// wh::guimodule::C_CreditsLayoutDatabase -- the "credit_layout" table (KCD2 1.5.6, kd7u).
// sizeof 0xB8 (template extent -- global static, no operator-new proof).
// -----------------------------------------------
// RTTI TD RVA 0x4CAFF60, COL RVA 0x4138F60, vtable 0x183C3CC10. Stack:
// C_ObjectDatabaseIdIndexed<C_ObjectTableDatabase<S_CreditsLayout, S_CreditsLayoutDBData>, int>.
// GLOBAL STATIC @0x185326570 (vtable-install thunk sub_1819382F4 -> base ctor
// sub_1819383A4; table "credit_layout", group "ui"; IdIndexed extras zeroed
// @0x185326608/0x185326610). Register [7] sub_181949B68 (descs qword_1854B1E48, builder
// sub_180171BD0, 2 cols, stride 0x10); ParseRows [10] sub_1814F31F0; post-load [16]
// sub_1814F3300. [A possible concrete-declared I_DynamicEnum base @+0xB8 was NOT
// re-verified this wave -- G4d §UNVERIFIED 6; extend if certified.]

namespace wh::guimodule {

class C_CreditsLayoutDatabase
    : public wh::databasemodule::C_ObjectDatabaseIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_CreditsLayout, S_CreditsLayoutDBData>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CreditsLayoutDatabase;
};
static_assert(sizeof(C_CreditsLayoutDatabase) == 0xB8, "C_CreditsLayoutDatabase must be 0xB8 (IdIndexed extent)");

}  // namespace wh::guimodule
