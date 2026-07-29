#pragma once
#include <cstdint>
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulPropertyNotifier -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50.
// -----------------------------------------------
// RTTI-named (vtable written by ctor sub_1803F1EFC); dtor sub_1809665BC. Embedded in C_Soul
// @+0x58. THE soul-property-changed broadcaster: this is the signal C_Actor+0xF0 connects to.
// Interior beyond {vptr, signal head} is ctor-zeroed and UNRESOLVED (flagged).

namespace wh::rpgmodule {

class C_SoulPropertyNotifier {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulPropertyNotifier;
    virtual ~C_SoulPropertyNotifier() = default;   // slot order UNVERIFIED
    wh::shared::C_Signal<> m_onPropertyChanged;    // +0x08  (empty sentinel &unk_1856671F8; arg signature unresolved)
    uint64_t m_unk18[7];                           // +0x18..+0x50  ctor-zeroed, unresolved
};
static_assert(sizeof(C_SoulPropertyNotifier) == 0x50, "C_SoulPropertyNotifier must be 0x50");

}  // namespace wh::rpgmodule
