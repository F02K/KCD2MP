#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_JailEndedData -- payload of C_JailEndedCause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD5978 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_182CF9D88.  Two-qword blob; fields UNVERIFIED.

namespace wh::rpgmodule {

struct S_JailEndedData {
    uint64_t m_a;   // +0x00  [U role]
    uint64_t m_b;   // +0x08  [U role]
};
static_assert(sizeof(S_JailEndedData) == 0x10, "S_JailEndedData must be 0x10");

}  // namespace wh::rpgmodule
