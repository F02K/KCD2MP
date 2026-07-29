#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_FallDamageData -- payload of C_FallDamageCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: C_Cause<S_FallDamageData> factory sub_1807252E8 operator new(32)
// copies 24 payload bytes to cause+0x08: {OWORD from arg, QWORD from arg+16}).
// Field roles UNVERIFIED (16-byte value + 8-byte value; fall height/velocity + soul WUID
// are candidates).

namespace wh::rpgmodule {

struct S_FallDamageData {
    uint64_t m_param00;   // +0x00 } 16-byte payload from the caller [roles UNVERIFIED]
    uint64_t m_param08;   // +0x08 }
    uint64_t m_param10;   // +0x10  8-byte payload [role UNVERIFIED -- WUID width]
};
static_assert(sizeof(S_FallDamageData) == 0x18, "S_FallDamageData must be 0x18 (24-byte ctor copy)");

}  // namespace wh::rpgmodule
