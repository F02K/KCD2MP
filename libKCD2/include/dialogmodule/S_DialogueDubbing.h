#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::dialogmodule::S_DialogueDubbing -- "response_depot_wave_file" tree-database row
// (KCD2 1.5.6, kd7u).  sizeof 0x08, POD.
// -----------------------------------------------
// Row/element of C_DialogueDubbingDatabase (by value, +0x30 vector). Size proven by the
// sort helper sub_1819ED9A4: count = (end - begin) >> 3 => stride 8; the base dtor frees
// the buffer as POD (no per-element dtor, 8-aligned) => trivially destructible, no owned
// CryStringT. Most likely {uint32 keyCrc; uint32 valueCrc} (dialog-line -> wave-file id
// pair); the exact 8-byte field split is UNVERIFIED (no consumer reads found).

namespace wh::dialogmodule {

struct S_DialogueDubbing {
    uint32_t m_keyCrc;     // +0x00  [split UNVERIFIED]
    uint32_t m_valueCrc;   // +0x04  [split UNVERIFIED]
};
static_assert(sizeof(S_DialogueDubbing) == 0x08, "S_DialogueDubbing must be 0x08 (sort stride 8)");

}  // namespace wh::dialogmodule
