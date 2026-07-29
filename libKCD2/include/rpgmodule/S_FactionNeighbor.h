#pragma once
#include <cstdint>
#include <memory>

// -----------------------------------------------
// wh::rpgmodule::S_FactionNeighbor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18 (SYNTHETIC name).
// -----------------------------------------------
// Element of C_Faction's RUNTIME spatial neighbor cache (m_neighborCache @C_Faction+0x1D0;
// absent from the RTTR registrar, so never persisted). Rebuilt by sub_180467ADC: iterates the
// global faction registry (qword_185330898), resolves each faction's entity position via the
// manager (sub_180468230 -> WUID -> entity vtable+0x70 Vec3), and for every pair within the
// config distance threshold (tuning table sub_180649D30()[737]) pushes {weak_ptr(other), dist}
// into BOTH factions' caches (symmetric). weak_ptr proven by element release sub_1803FB78C
// (_Decwref on _Weaks @ctrl+0xC); emplace sub_1803F3180 (+ shared->weak convert sub_1803F3320)
// then float @elem+0x10. Pointee static type C_Faction vs C_FactionBase not 100% pinned
// (registry elements own a +0x1D0 vector => C_Faction layout; flagged).

namespace wh::rpgmodule {

class C_Faction;

struct S_FactionNeighbor {
    std::weak_ptr<C_Faction> m_node;   // +0x00  (0x10) neighbor faction (weak; _Decwref-released)
    float    m_distance;               // +0x10  pairwise entity distance (sqrtf) at rebuild time
    uint32_t _pad14;                   // +0x14
};
static_assert(sizeof(S_FactionNeighbor) == 0x18, "S_FactionNeighbor must be 0x18");
static_assert(offsetof(S_FactionNeighbor, m_distance) == 0x10, "distance at 0x10");

}  // namespace wh::rpgmodule
