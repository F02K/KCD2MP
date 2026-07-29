#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsRole2LanguageExclDBData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// Raw "credit_role2language_excl" table row (role-to-language exclusion pairs).
// Columns from the C_CreditsRole2LanguageExclDatabase Register descriptor builder
// sub_180171E40 (2 cols, both PK, row stride 0x08 via sub_181944420). The Data type
// (S_CreditsRole2LanguageExcl) is a verbatim copy of this row.

namespace wh::guimodule {

struct S_CreditsRole2LanguageExclDBData {
    int32_t credit_role_id;   // +0x00  int32, PK
    int32_t language_id;      // +0x04  int32, PK
};
static_assert(sizeof(S_CreditsRole2LanguageExclDBData) == 0x08, "credit_role2language_excl row stride is 0x08");

}  // namespace wh::guimodule
