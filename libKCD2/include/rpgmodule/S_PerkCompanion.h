#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PerkCompanion -- perk_companion table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x24, ALIGN 4 (PROVEN: C_PerkCompanionDatabase ParseRows 0x180B20D74, blob stride 36:
// {16B @0, dword @16 clamped to <=2 and stored as a BYTE @16, 16B @20}).  The 16-byte value at
// offset 20 rules out an 8-aligned CryGUID type -- mirrored as uint32[4]/uint8[16].
// Roles [INFERRED].

namespace wh::rpgmodule {

struct S_PerkCompanion {
    uint32_t m_perkId[4];         // +0x00  16-byte GUID value, 4-aligned [role INFERRED]
    uint8_t  m_companionType;     // +0x10  blob dword clamped: >1 -> 2 [enum values unknown]
    uint8_t  _pad11[3];           // +0x11
    uint8_t  m_companionId[16];   // +0x14  16-byte GUID value, byte-copied [role INFERRED]
};
static_assert(sizeof(S_PerkCompanion) == 0x24 && alignof(S_PerkCompanion) == 4,
              "S_PerkCompanion must be 0x24/align-4 (ParseRows stride 36, field @+0x14)");

}  // namespace wh::rpgmodule
