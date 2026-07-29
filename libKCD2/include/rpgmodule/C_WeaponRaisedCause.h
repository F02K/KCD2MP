#pragma once
#include "C_Cause.h"
#include "S_WeaponRaisedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_WeaponRaisedCause -- "WeaponRaised" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: create fn sub_180C5BCE4 tracked-alloc 32).  RTTI TD rva 0x4D5F010,
// vtable rva 0x3A8AA88 (4 slots): [0] dtor rva 0x15A3D58 (ICF trivial), [1] nullsub,
// [2] Accept rva 0xAD9A3C -> visitor+736, [3] GetName rva 0xAD9AF8 -> "WeaponRaised"
// (verified).  Factory builder sub_180C5BB14.

namespace wh::rpgmodule {

class C_WeaponRaisedCause : public C_Cause<S_WeaponRaisedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeaponRaisedCause;
};
static_assert(sizeof(C_WeaponRaisedCause) == 0x20, "C_WeaponRaisedCause must be 0x20 (alloc 32 at sub_180C5BCE4)");

}  // namespace wh::rpgmodule
