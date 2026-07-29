#pragma once
#include <cstdint>
#include "C_EquippableItemRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_WeaponEquipRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC0 (DERIVED).
// -----------------------------------------------
// RTTI .?AVC_WeaponEquipRuntimeData@entitymodule@wh@@ (TD 0x184C37EB8, COL 0x18413ED20)
// vtable 0x183A392B8  ctor sub_1804744C0.  sizeof 0xC0 is DERIVED, not pool-measured (no dedicated
// pool for this intermediate): bounded by C_AmmoRuntimeData(0xC8) which extends it with exactly one
// int @+0xC0, and both Weapon/Ammo own-member ranges start at +0xC0.

namespace wh::entitymodule {

class C_WeaponEquipRuntimeData : public C_EquippableItemRuntimeData   // +0x00  (0xA8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeaponEquipRuntimeData;
    float    m_equipState[4];   // +0xA8  OWORD zero-init (INFERRED 4 floats, equip/aim state --
                                //        no reader decompiled)
    int32_t  m_fieldB8;         // +0xB8  int32 scalar (ctor 0; clone dword-copy 0x180F1974D); role unresolved
    uint32_t _padBC;            // +0xBC
};
static_assert(sizeof(C_WeaponEquipRuntimeData) == 0xC0, "C_WeaponEquipRuntimeData must be 0xC0");

}  // namespace wh::entitymodule
