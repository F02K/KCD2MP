#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BlacksmithingStrokeData -- payload of C_BlacksmithingStrokeCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: factory sub_180C65970 tracked-alloc 56 = 8+48; 3x _OWORD copies to
// cause+0x08/+0x18/+0x28).  Internal field boundaries beyond the three 16-byte copy units
// UNVERIFIED -- mirrored as a raw blob.

namespace wh::rpgmodule {

struct S_BlacksmithingStrokeData {
    uint8_t _raw[48];   // +0x00  3x _OWORD [U fields]
};
static_assert(sizeof(S_BlacksmithingStrokeData) == 0x30, "S_BlacksmithingStrokeData must be 0x30 (48-byte ctor copy)");

}  // namespace wh::rpgmodule
