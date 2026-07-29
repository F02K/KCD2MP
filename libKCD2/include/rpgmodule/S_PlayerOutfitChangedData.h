#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PlayerOutfitChangedData -- payload of C_PlayerOutfitChangedCause (KCD2
// WHGame.dll 1.5.6, kd7u).  Second-largest cause payload.
// -----------------------------------------------
// sizeof 0xE0 (PROVEN: ctor sub_18172E8F4 tracked-alloc 232, += 0xE8; 14x _OWORD copied
// verbatim).  Forwarded whole by caller sub_18172E898 @0x18172E8AD; likely a fixed array
// of equipment/appearance slots -- internal structure UNVERIFIED, mirrored as a raw blob.

namespace wh::rpgmodule {

struct S_PlayerOutfitChangedData {
    uint8_t _raw[224];   // +0x00  outfit descriptor (14x _OWORD) [U fields]
};
static_assert(sizeof(S_PlayerOutfitChangedData) == 0xE0, "S_PlayerOutfitChangedData must be 0xE0");

}  // namespace wh::rpgmodule
