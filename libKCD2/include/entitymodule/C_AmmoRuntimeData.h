#pragma once
#include <cstdint>
#include "C_WeaponEquipRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_AmmoRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC8.
// -----------------------------------------------
// RTTI .?AVC_AmmoRuntimeData@entitymodule@wh@@ (TD 0x184C37FC0, COL 0x1843064D0)
// vtable 0x183EB5E68  ctor sub_180F19534 (delegates to C_WeaponEquipRuntimeData).
// sizeof VERIFIED from the fixed-slot pool sub_180F1957C: 204800/1024 = 200 (stride 50 dwords).

namespace wh::entitymodule {

class C_AmmoRuntimeData : public C_WeaponEquipRuntimeData   // +0x00  (0xC0)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AmmoRuntimeData;
    int32_t  m_ammoCount;   // +0xC0  (=0; INFERRED stack/charge/ammo count -- no reader decompiled)
    uint32_t _padC4;        // +0xC4
};
static_assert(sizeof(C_AmmoRuntimeData) == 0xC8, "C_AmmoRuntimeData must be 0xC8");

}  // namespace wh::entitymodule
