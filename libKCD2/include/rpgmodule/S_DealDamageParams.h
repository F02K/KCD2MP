#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_DealDamageParams -- scripted-damage request descriptor (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Struct name RE-CHOSEN. Built on the stack by C_CombatSoul::DealDamage sub_180EE031C
// (default-init sub_180EE03D0, effect payload copied in by sub_180EE0098 @+0x18) and consumed
// by C_CombatSoul vtable slot 28 (sub_1807256D4), which reads exactly these offsets.
// sizeof 0x60 = 0x18 head + 0x48 payload (stack extent in sub_180EE031C).

namespace wh::rpgmodule {

struct S_DealDamageParams {
    float    m_stamina;       // +0x00  requested stamina damage
    float    m_health;        // +0x04  requested health damage
    uint64_t m_source;        // +0x08  source handle (slot 28 copies it via sub_180D571E4 when non-null)
    uint8_t  m_flag10;        // +0x10  forwarded verbatim into the slot-28 descriptor (role UNVERIFIED)
    uint8_t  _pad11[7];       // +0x11
    uint8_t  m_effect[0x48];  // +0x18  0x48 payload copied by sub_180EE0098; slot 28 reads dwords
                              //        @+0x18/+0x5C -- NOT vtable-first, so not a C_*ValueEffect
                              //        object image; interior layout UNRESOLVED
};
static_assert(sizeof(S_DealDamageParams) == 0x60, "S_DealDamageParams must be 0x60");

}  // namespace wh::rpgmodule
