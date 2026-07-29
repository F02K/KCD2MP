#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsLayoutDBData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Raw "credit_layout" table row (load-time buffer layout). Column set from the
// C_CreditsLayoutDatabase Register descriptor builder sub_180171BD0 (2 cols, registered
// row stride 0x10 via sub_181949B68); strings are load-time char* fed to CryStringT in
// ParseRows (sub_1814F31F0).

namespace wh::guimodule {

struct S_CreditsLayoutDBData {
    int32_t     credit_layout_id;     // +0x00  int32, PK
    uint8_t     _pad04[4];            // +0x04
    const char* credit_layout_name;   // +0x08  string col (type 5)
};
static_assert(sizeof(S_CreditsLayoutDBData) == 0x10, "credit_layout row stride is 0x10 (sub_181949B68)");

}  // namespace wh::guimodule
