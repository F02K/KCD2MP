#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AimBeginData -- payload of C_AimBeginCause, and REUSED as the template
// argument of C_WeaponDestroyCause (C_Cause<S_AimBeginData>; no S_WeaponDestroyData exists)
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD4C74 tracked-alloc 32; _OWORD copy to cause+0x08 +
// qword *(a3+16) to cause+0x18).  Field names proposed (WUID-width handles) [U].

namespace wh::rpgmodule {

struct S_AimBeginData {
    uint64_t m_a;   // +0x00  WUID [proposed]
    uint64_t m_b;   // +0x08  WUID/handle [proposed]
    uint64_t m_c;   // +0x10  qword [U role]
};
static_assert(sizeof(S_AimBeginData) == 0x18, "S_AimBeginData must be 0x18 (24-byte ctor copy)");

}  // namespace wh::rpgmodule
