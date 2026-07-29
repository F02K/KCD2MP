#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsLayout -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Parsed "credit_layout" Data element (NON-polymorphic), stored in
// C_CreditsLayoutDatabase's rows vector. Layout from ParseRows sub_1814F31F0
// (in-place push advances +0x10): int @+0x00, CryStringT @+0x08.

namespace wh::guimodule {

struct S_CreditsLayout {
    int32_t          credit_layout_id;     // +0x00
    uint8_t          _pad04[4];            // +0x04
    CryStringT<char> credit_layout_name;   // +0x08
};
static_assert(sizeof(S_CreditsLayout) == 0x10, "S_CreditsLayout must be 0x10 (ParseRows sub_1814F31F0)");

}  // namespace wh::guimodule
