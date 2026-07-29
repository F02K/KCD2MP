#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ActorLootData -- payload of C_ActorLootCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_181361D8C tracked-alloc 24, one _OWORD copy to
// cause+0x08).  Create-site sub_181FD46A0: lo = *(a2+48) (looter), hi = *(a3->vfunc0())
// (looted actor).  Field names proposed; 8-byte width matches framework::WUID [U].

namespace wh::rpgmodule {

struct S_ActorLootData {
    uint64_t m_looter;   // +0x00  looter WUID [proposed]
    uint64_t m_looted;   // +0x08  looted-actor WUID [proposed]
};
static_assert(sizeof(S_ActorLootData) == 0x10, "S_ActorLootData must be 0x10 (16-byte ctor copy)");

}  // namespace wh::rpgmodule
