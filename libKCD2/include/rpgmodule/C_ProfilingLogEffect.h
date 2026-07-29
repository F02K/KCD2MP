#pragma once
#include <cstdint>
#include "C_Effect.h"
#include "S_ProfilingLogPayload.h"

// -----------------------------------------------
// wh::rpgmodule::C_ProfilingLogEffect -- emit a profiling log entry (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E440  vtable 0x183B52748.  sizeof 0x50 (PROVEN: factory sub_1812C2F40
// allocates via the RAW allocator sub_1804F75C0(80) -- unlike the other effects' tracked
// allocator).  Derives C_Effect DIRECTLY (outlined base ctor sub_1823D9CE8 = C_Effect init +
// owner self-register).  Ctor (owner, &data12, &payload): +0x10 = data.qword0,
// +0x18 = data.dword8, +0x20 = payload copy.

namespace wh::rpgmodule {

class C_ProfilingLogEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ProfilingLogEffect;
    uint64_t m_param10;   // +0x10  12-byte ctor struct qword [role UNVERIFIED]
    int32_t  m_param18;   // +0x18  12-byte ctor struct dword [role UNVERIFIED]
    // +0x1C 4-byte padding
    S_ProfilingLogPayload m_payload;   // +0x20
};
static_assert(sizeof(C_ProfilingLogEffect) == 0x50,
              "C_ProfilingLogEffect must be 0x50 (raw allocator alloc(80))");

}  // namespace wh::rpgmodule
