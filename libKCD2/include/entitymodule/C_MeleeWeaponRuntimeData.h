#pragma once
#include <cstdint>
#include "C_WeaponRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_MeleeWeaponRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_MeleeWeaponRuntimeData@entitymodule@wh@@ (TD 0x184C6F0B8, COL 0x18413EC08)
// vtable 0x183A393B8  ctor sub_18047442C -- sets ONLY the vtable; adds NO own members (CONFIRMED).
// Shares C_WeaponRuntimeData's pool (sub_181F1D700, stride 216).

namespace wh::entitymodule {

class C_MeleeWeaponRuntimeData : public C_WeaponRuntimeData   // +0x00  (0xD8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MeleeWeaponRuntimeData;
};
static_assert(sizeof(C_MeleeWeaponRuntimeData) == 0xD8, "C_MeleeWeaponRuntimeData must be 0xD8");

}  // namespace wh::entitymodule
