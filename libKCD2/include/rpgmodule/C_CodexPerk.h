#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_CodexPerk -- perk unlocking a codex entry (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D53EE0  vtable 0x183B9E5C8.  sizeof 0x40 (PROVEN: factory sub_1814BFDB0
// operator new(64)). Extras: +0x30 qword NOT initialized by the ctor, +0x38 byte = 0.
// Backed by C_PerkCodexDatabase rows.

namespace wh::rpgmodule {

class C_CodexPerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CodexPerk;
    void*    m_pResolvedEntry;   // +0x30  NOT set by ctor; Resolve[9] override (0x181608C90) caches the sub_18046B3C0(m_id) lookup result here [pointer to the codex definition/entry; pointee not RE'd]. Name inferred.
    uint8_t  m_flag38;   // +0x38  ctor 0 [role UNVERIFIED]
    uint8_t  _pad39[7];  // +0x39
};
static_assert(sizeof(C_CodexPerk) == 0x40, "C_CodexPerk must be 0x40 (operator new(64))");

}  // namespace wh::rpgmodule
