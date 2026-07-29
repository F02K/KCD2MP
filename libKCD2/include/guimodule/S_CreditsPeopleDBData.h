#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsPeopleDBData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// Raw "credit_people" table row. Columns from the C_CreditsPeopleDatabase Register
// descriptor builder sub_180171CB0 (4 cols, row stride 0x18 via sub_181949A74).

namespace wh::guimodule {

struct S_CreditsPeopleDBData {
    int32_t     credit_people_id;     // +0x00  int32, PK
    uint8_t     _pad04[4];            // +0x04
    const char* credit_people_name;   // +0x08  string col (type 5)
    int32_t     credit_role_id;       // +0x10  int32
    int32_t     order_by;             // +0x14  int32
};
static_assert(sizeof(S_CreditsPeopleDBData) == 0x18, "credit_people row stride is 0x18 (sub_181949A74)");

}  // namespace wh::guimodule
