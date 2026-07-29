#pragma once
#include <cstdint>
#include "C_WeaponRuntimeData.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::entitymodule::C_MissileWeaponRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xF8.
// -----------------------------------------------
// RTTI .?AVC_MissileWeaponRuntimeData@entitymodule@wh@@ (TD 0x184C6F1D0, COL 0x18413EC30)
// vtable 0x183AD5CA8  ctor sub_180F196C8  clone/copy sub_180F18F34 (copies +0xD8 byte, +0xD9 byte,
// +0xE0 qword, +0xE8 xmmword -- pins the tail field boundaries).
// sizeof VERIFIED from the fixed-slot pool sub_180F1957C..sub_181F1D850: 253952/1024 = 248.
//
// m_loadedAmmo: is-chambered predicate sub_1823D459C does `cmp [rcx+0xE0], qword_1853259B8; setnz`
// -- an explicit "loaded_ammo != invalid-sentinel" test. The sentinel global qword_1853259B8
// (sourced from qword_1853259B0) is 0 in the image (invalid WUID = 0), NOT -1.
// +0xE8 is ctor-unwritten (late InitFromRequest path) but a REAL copied 16-byte field.

namespace wh::entitymodule {

class C_MissileWeaponRuntimeData : public C_WeaponRuntimeData   // +0x00  (0xD8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MissileWeaponRuntimeData;
    uint8_t  m_fieldD8;      // +0xD8  (=0; copied individually by clone fn; role unresolved)
    uint8_t  m_fieldD9;      // +0xD9  (=0; copied individually by clone fn; role unresolved)
    uint8_t  _padDA[6];      // +0xDA
    wh::framework::WUID m_loadedAmmo;  // +0xE0  chambered-ammo handle; 0 = not loaded  VERIFIED semantics
    CryGUID  m_pendingAmmoGuid;  // +0xE8  pending-ammo GUID key; sub_180C5E72C passes &field by-addr to registry find
                                 //        (sub_180465F60 -> sub_181EC83C0 two-half FNV-1a, canonical CryGUID hash); resolved
                                 //        object's WUID -> m_loadedAmmo, then field cleared to 0. ctor-unwritten.
};
static_assert(sizeof(C_MissileWeaponRuntimeData) == 0xF8, "C_MissileWeaponRuntimeData must be 0xF8");
static_assert(offsetof(C_MissileWeaponRuntimeData, m_loadedAmmo) == 0xE0, "loaded ammo at 0xE0");

}  // namespace wh::entitymodule
