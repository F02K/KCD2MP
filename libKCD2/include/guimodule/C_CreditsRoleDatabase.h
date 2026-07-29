#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "databasemodule/C_ObjectDatabaseIdIndexed.h"
#include "guimodule/S_CreditsRole.h"
#include "guimodule/S_CreditsRoleDBData.h"

// -----------------------------------------------
// wh::guimodule::C_CreditsRoleDatabase -- the "credit_role" table (KCD2 1.5.6, kd7u).
// sizeof 0xB8 (template extent -- global static, no operator-new proof).
// -----------------------------------------------
// RTTI TD RVA 0x4CAFC48, COL RVA 0x4138F10, vtable 0x183C3CA90. Stack:
// C_ObjectDatabaseIdIndexed<C_ObjectTableDatabase<S_CreditsRole, S_CreditsRoleDBData>, int>.
// GLOBAL STATIC @0x185326410 (thunk sub_1819381C0; table "credit_role", group "ui";
// IdIndexed extras zeroed @0x1853264A8/0x1853264B0). Register [7] sub_181949980
// (descs qword_1854B1E08, builder sub_180171F10, 4 cols, stride 0x18); ParseRows [10]
// sub_1811D89E0 (constructs the POLYMORPHIC S_CreditsRole in place, ctor sub_1811D8B4C);
// post-load [16] sub_1811D8C08. [Possible concrete I_DynamicEnum base @+0xB8 NOT
// re-verified -- G4d §UNVERIFIED 6.]

namespace wh::guimodule {

class C_CreditsRoleDatabase
    : public wh::databasemodule::C_ObjectDatabaseIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_CreditsRole, S_CreditsRoleDBData>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CreditsRoleDatabase;
};
static_assert(sizeof(C_CreditsRoleDatabase) == 0xB8, "C_CreditsRoleDatabase must be 0xB8 (IdIndexed extent)");

}  // namespace wh::guimodule
