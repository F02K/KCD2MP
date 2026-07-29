#pragma once
#include <cstdint>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_DmgREffect -- damage report effect (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E478  vtable 0x183F1EBE0.  sizeof 0x28 (PROVEN: factory sub_180C63B44
// operator new(40)).  Derives C_Effect DIRECTLY (inline C_Effect init + owner self-register).
// Ctor (owner, a3, &data12): +0x10 = a3, +0x18 = data.qword0, +0x20 = data.dword8.

namespace wh::rpgmodule {

class C_DmgREffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DmgREffect;
    uint64_t m_param10;   // +0x10  ctor arg 3 [role UNVERIFIED -- target WUID candidate]
    uint64_t m_param18;   // +0x18  12-byte ctor struct qword [role UNVERIFIED]
    int32_t  m_param20;   // +0x20  12-byte ctor struct dword [role UNVERIFIED]
    // +0x24 4-byte tail padding
};
static_assert(sizeof(C_DmgREffect) == 0x28, "C_DmgREffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
