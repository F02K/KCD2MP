#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::playermodule::S_FastTravelConfig -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58 (SYNTHETIC name).
// -----------------------------------------------
// Fast-travel tuning block embedded in C_FastTravel @+0xA8, copy-constructed (sub_1804C6180)
// from the global default &unk_1855EE200 (Meyers singleton sub_180BE9C38). Field OFFSETS are
// the verify-pass corrected set (the first research pass had the interior shifted +8); field
// MEANINGS are UNRESOLVED -- only the shapes are proven from the copy-ctor:
// the two leading fields are COW CryStringT (copy = ptr + addref sub_181E38F20 via
// sub_1804C4950), +0x18 is a 16B sub-struct copied whole (sub_18039AE88), +0x28 is a 16B
// head-allocated-node container (node alloc sub_1803F7FAC + copy sub_1804C6218 -- matches the
// MSVC std::map/set 0x10 header shape; key/value types UNRESOLVED), +0x40 is one OWORD copy.

namespace wh::playermodule {

struct S_FastTravelConfig {
    CryStringT<char> m_str00;   // +0x00  COW string (meaning unresolved)
    CryStringT<char> m_str08;   // +0x08  COW string (meaning unresolved)
    uint32_t m_u10;             // +0x10
    uint32_t m_u14;             // +0x14
    uint64_t m_sub18[2];        // +0x18  16B sub-struct, copied whole (interior UNRESOLVED)
    uint64_t m_map28[2];        // +0x28  head-allocated-node container (std::map/set-shaped;
                                //        element types UNRESOLVED)
    uint64_t m_qw38;            // +0x38
    float    m_vec40[4];        // +0x40  OWORD-copied 16B (Vec4/Quat -- INFERRED)
    uint32_t m_u50;             // +0x50
    uint32_t _pad54;            // +0x54
};
static_assert(sizeof(S_FastTravelConfig) == 0x58, "S_FastTravelConfig must be 0x58");

}  // namespace wh::playermodule
