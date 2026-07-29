#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_WeaponRaisedData -- payload of C_WeaponRaisedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_180C5BCE4 tracked-alloc 32; _OWORD @+0x08 + qword
// *(v5+24)=*(a3+16)).  Builder sub_180C5BB14 (site 0x180C5BB7E): handle = *(a2->vf0()),
// BYTE8 = a3 (raised/lowered flag, VERIFIED bool), qword @+16 = a4.

namespace wh::rpgmodule {

struct S_WeaponRaisedData {
    uint64_t m_actor;     // +0x00  actor/weapon handle (= a2->vf0()) [proposed]
    bool     m_raised;    // +0x08  raised/lowered flag (verified bool store)
    uint8_t  _pad09[7];   // +0x09
    uint64_t m_value;     // +0x10  builder a4 [U role]
};
static_assert(sizeof(S_WeaponRaisedData) == 0x18, "S_WeaponRaisedData must be 0x18");

}  // namespace wh::rpgmodule
