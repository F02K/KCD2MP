#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SoulRegistry -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x158 (non-polymorphic).
// -----------------------------------------------
// SYNTHETIC NAME. C_Soul's hash/registry aggregate @C_Soul+0x340. ctor sub_1803F244C; dtor
// sub_180966E14. The only resolved interior anchor is a 3-bucket inline hash initialized by
// sub_1811D0528(this+0xB0, 3) (uses Str/TLS globals). Candidate identity: a relationship/
// reputation map (UNCONFIRMED -- no faction or reputation scalar exists inside C_Soul itself;
// as in KCD1, faction data is external, keyed by the soul's WUID). Everything else UNRESOLVED.

namespace wh::rpgmodule {

struct S_SoulRegistry {
    uint64_t m_unk00[22];    // +0x00..+0xB0   unresolved
    uint64_t m_hashB0[21];   // +0xB0..+0x158  3-bucket inline hash (sub_1811D0528(+0xB0, 3)); interior unresolved
};
static_assert(sizeof(S_SoulRegistry) == 0x158, "S_SoulRegistry must be 0x158");

}  // namespace wh::rpgmodule
