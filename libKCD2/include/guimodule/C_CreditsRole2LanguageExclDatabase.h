#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "guimodule/S_CreditsRole2LanguageExcl.h"
#include "guimodule/S_CreditsRole2LanguageExclDBData.h"

// -----------------------------------------------
// wh::guimodule::C_CreditsRole2LanguageExclDatabase -- the "credit_role2language_excl"
// table (KCD2 1.5.6, kd7u).  sizeof 0x98 (template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CAFA60, COL RVA 0x4138EE8, vtable 0x183C40B50. Stack: plain
// C_ObjectTableDatabase<S_CreditsRole2LanguageExcl, S_CreditsRole2LanguageExclDBData>.
// GLOBAL STATIC @0x185326370 (thunk sub_181946260; table "credit_role2language_excl",
// group "ui"). Register [7] sub_181944420 (descs qword_1854B1DF8, builder sub_180171E40,
// 2 cols, stride 0x08); ParseRows [10] sub_1814B5BAC (COMDAT verbatim copy, push
// sub_18240E2D0, grow sub_1803EBAFC); post-load [16] stub sub_18041A6A0.

namespace wh::guimodule {

class C_CreditsRole2LanguageExclDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_CreditsRole2LanguageExcl,
                                                       S_CreditsRole2LanguageExclDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CreditsRole2LanguageExclDatabase;
};
static_assert(sizeof(C_CreditsRole2LanguageExclDatabase) == 0x98,
              "C_CreditsRole2LanguageExclDatabase must be 0x98 (OTD extent)");

}  // namespace wh::guimodule
