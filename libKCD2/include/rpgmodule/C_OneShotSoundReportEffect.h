#pragma once
#include <cstdint>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_OneShotSoundReportEffect -- report a one-shot sound to the RPG layer
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E848  vtable 0x183F1ECD0.  sizeof 0x28 (PROVEN: factory sub_1816411F8
// operator new(40)).  Derives C_Effect DIRECTLY.  Ctor (owner, a3:int, &data12, a5:int):
// +0x10 = a3, +0x14 = a5, +0x18 = data.qword0, +0x20 = data.dword8.

namespace wh::rpgmodule {

class C_OneShotSoundReportEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_OneShotSoundReportEffect;
    int32_t  m_param10;   // +0x10  ctor arg 3 [role UNVERIFIED -- sound id candidate]
    int32_t  m_param14;   // +0x14  ctor arg 5 [role UNVERIFIED]
    uint64_t m_param18;   // +0x18  12-byte ctor struct qword [role UNVERIFIED -- source WUID candidate]
    int32_t  m_param20;   // +0x20  12-byte ctor struct dword [role UNVERIFIED]
    // +0x24 4-byte tail padding
};
static_assert(sizeof(C_OneShotSoundReportEffect) == 0x28,
              "C_OneShotSoundReportEffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
