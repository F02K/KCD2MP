#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "guimodule/S_CreditsPeople.h"
#include "guimodule/S_CreditsPeopleDBData.h"

// -----------------------------------------------
// wh::guimodule::C_CreditsPeopleDatabase -- the "credit_people" table (KCD2 1.5.6, kd7u).
// sizeof 0x98 (template extent -- global static, no operator-new proof).
// -----------------------------------------------
// RTTI TD RVA 0x4CAFD78, COL RVA 0x4138F38, vtable 0x183C40CD0. Stack: plain
// C_ObjectTableDatabase<S_CreditsPeople, S_CreditsPeopleDBData>.
// GLOBAL STATIC @0x1853264D0 (thunk sub_18194637C; table "credit_people", group "ui").
// Register [7] sub_181949A74 (descs qword_1854B1E28, builder sub_180171CB0, 4 cols,
// stride 0x18); ParseRows [10] sub_18112EC34 (accessor sub_18112ECE0, converter
// sub_18112ECFC, push sub_18112ED6C); post-load [16] stub sub_18041A6A0.

namespace wh::guimodule {

class C_CreditsPeopleDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_CreditsPeople, S_CreditsPeopleDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CreditsPeopleDatabase;
};
static_assert(sizeof(C_CreditsPeopleDatabase) == 0x98, "C_CreditsPeopleDatabase must be 0x98 (OTD extent)");

}  // namespace wh::guimodule
