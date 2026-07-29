#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsPeople -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// Parsed "credit_people" Data element (NON-polymorphic), stored in
// C_CreditsPeopleDatabase's rows vector. Layout from the ParseRows converter
// sub_18112ECFC (fills to +0x14; size = registered stride 0x18 -- push-helper
// sub_18112ED6C end-advance not separately dumped [G4d §UNVERIFIED 7]).

namespace wh::guimodule {

struct S_CreditsPeople {
    int32_t          credit_people_id;     // +0x00
    uint8_t          _pad04[4];            // +0x04
    CryStringT<char> credit_people_name;   // +0x08
    int32_t          credit_role_id;       // +0x10
    int32_t          order_by;             // +0x14
};
static_assert(sizeof(S_CreditsPeople) == 0x18, "S_CreditsPeople must be 0x18 (converter sub_18112ECFC)");

}  // namespace wh::guimodule
