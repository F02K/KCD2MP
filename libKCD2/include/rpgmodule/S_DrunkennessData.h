#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_DrunkennessData -- payload of C_DrunkennessCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_182BC3A58 tracked-alloc 16; v5[1]=*a3 @0x182BC3AB9).
// 4 create-sites (sub_182BC85B8 / sub_182BCBA24 / sub_182BCBC4C / sub_182BDB7B0);
// sub_182BC85B8 passes *(*(a1+136)+48) -> actor handle [proposed].

namespace wh::rpgmodule {

struct S_DrunkennessData {
    uint64_t m_actor;   // +0x00  actor WUID [proposed]
};
static_assert(sizeof(S_DrunkennessData) == 0x08, "S_DrunkennessData must be 0x08");

}  // namespace wh::rpgmodule
