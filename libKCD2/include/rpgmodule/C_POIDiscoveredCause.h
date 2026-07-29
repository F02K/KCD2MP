#pragma once
#include "C_Cause.h"
#include "S_POIDiscoveredData.h"

// -----------------------------------------------
// wh::rpgmodule::C_POIDiscoveredCause -- "POIDiscovered" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_180B5FF8C tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D4DE48, vtable rva 0x3A7B2B0 (4 slots): [0] dtor rva 0x15A3D58 (ICF trivial),
// [1] nullsub, [2] Accept rva 0xE7A8AC -> visitor+0x038, [3] GetName rva 0xE7A9EC ->
// "POIDiscovered" (verified).  Caller sub_180B5FE90.

namespace wh::rpgmodule {

class C_POIDiscoveredCause : public C_Cause<S_POIDiscoveredData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_POIDiscoveredCause;
};
static_assert(sizeof(C_POIDiscoveredCause) == 0x20, "C_POIDiscoveredCause must be 0x20 (alloc 32 at sub_180B5FF8C)");

}  // namespace wh::rpgmodule
