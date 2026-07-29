#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_POIDiscoveredData -- payload of C_POIDiscoveredCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_180B5FF8C tracked-alloc 32; _OWORD @+0x08 + qword
// @+0x18).  Caller sub_180B5FE90 builds { *(v3+48) @+0, *v6 (16B) @+8 }.

namespace wh::rpgmodule {

struct S_POIDiscoveredData {
    uint64_t m_a;        // +0x00  handle (= *(v3+48)) [U role]
    uint64_t m_poi[2];   // +0x08  16-byte POI block (POI id / WUID pair candidate) [proposed]
};
static_assert(sizeof(S_POIDiscoveredData) == 0x18, "S_POIDiscoveredData must be 0x18");

}  // namespace wh::rpgmodule
