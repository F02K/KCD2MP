#pragma once
#include "C_Cause.h"
#include "S_SkipTimeEndedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SkipTimeEndedCause -- "SkipTimeEnded" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_18191BC28 tracked-alloc 16; 4-byte float payload +
// 4B tail pad).  RTTI TD rva 0x4D5D468, vtable rva 0x3C378B0 (4 slots): [0] dtor rva
// 0x9DC2C0 (ICF trivial), [1] nullsub, [2] Accept rva 0xE7B150 -> visitor+184, [3] GetName
// rva 0xE7B1E4 -> "SkipTimeEnded" (verified).  Factory builder sub_18191B9CC.

namespace wh::rpgmodule {

class C_SkipTimeEndedCause : public C_Cause<S_SkipTimeEndedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkipTimeEndedCause;
};
static_assert(sizeof(C_SkipTimeEndedCause) == 0x10, "C_SkipTimeEndedCause must be 0x10 (alloc 16 at sub_18191BC28)");

}  // namespace wh::rpgmodule
