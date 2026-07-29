#pragma once
#include "C_Cause.h"
#include "S_AimBeginData.h"

// -----------------------------------------------
// wh::rpgmodule::C_WeaponDestroyCause -- "WeaponDestroy" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).  SPECIAL: no dedicated S_WeaponDestroyData -- the cause REUSES the
// S_AimBeginData payload, so the base is C_Cause<S_AimBeginData> (confirmed via CHD
// base[1] TD `.?AV?$C_Cause@US_AimBeginData@...` and create-fn vtable literal at
// 0x2CD63BB).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: create fn sub_182CD6360 tracked-alloc 32).  RTTI TD rva 0x4D5D9E0,
// vtable rva 0x3F276A0 (4 slots): [0] dtor rva 0x15A3D58 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x2C8A4B0 -> visitor+680, [3] GetName rva 0x2C8AE84 -> "WeaponDestroy"
// (verified).  Only the concrete vtable/name and dispatch offset differ from AimBegin.
// Factory caller sub_182CFD9F8.

namespace wh::rpgmodule {

class C_WeaponDestroyCause : public C_Cause<S_AimBeginData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeaponDestroyCause;
};
static_assert(sizeof(C_WeaponDestroyCause) == 0x20, "C_WeaponDestroyCause must be 0x20 (alloc 32 at sub_182CD6360)");

}  // namespace wh::rpgmodule
