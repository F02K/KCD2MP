#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsRole2LanguageExcl -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// Parsed "credit_role2language_excl" Data element (NON-polymorphic) -- identical to its
// row (ParseRows sub_1814B5BAC is a COMDAT-folded verbatim 2-dword copy; in-place push
// sub_18240E2D0 advances +0x08).

namespace wh::guimodule {

struct S_CreditsRole2LanguageExcl {
    int32_t credit_role_id;   // +0x00
    int32_t language_id;      // +0x04
};
static_assert(sizeof(S_CreditsRole2LanguageExcl) == 0x08, "S_CreditsRole2LanguageExcl must be 0x08");

}  // namespace wh::guimodule
