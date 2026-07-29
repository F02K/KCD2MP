#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::C_RPGBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x02 (non-polymorphic).
// -----------------------------------------------
// RTTI .?AVC_RPGBase@rpgmodule@wh@@ -- appears as a NON-polymorphic base in RTTI hierarchies
// (C_RPGLocationManager base @+0x18: MSVC lays it out after the three vfptr-carrying bases).
// Carries the module-object state byte that I_RPGLocationManager::SetStateFlag / IsSuspended
// operate on (the manager's Reset sets/clears mask 2 around its body). KCD1 had no such base --
// the KCD1 manager modeled the same byte as a folded state word member.
// Second byte: zero-init together with the flags (single WORD write in the owner ctor); no
// reader located -- member vs padding UNVERIFIED.

namespace wh::rpgmodule {

class C_RPGBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGBase;
    uint8_t m_stateFlags;  // +0x00  suspend mask; != 0 => suspended (bit1 held during Reset)
    uint8_t m_unk01;       // +0x01  zero-init (member vs padding UNVERIFIED)
};
static_assert(sizeof(C_RPGBase) == 0x02, "C_RPGBase must be 0x02");

}  // namespace wh::rpgmodule
