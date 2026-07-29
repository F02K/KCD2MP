#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_HorseIrritationActionData -- payload of C_HorseIrritationActionCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD5588 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_182CF9AB8.  {qword, dword} blob split per dossier; fields UNVERIFIED.

namespace wh::rpgmodule {

struct S_HorseIrritationActionData {
    uint64_t m_a;         // +0x00  qword [U role]
    int32_t  m_b;         // +0x08  dword [U role]
    uint8_t  _pad0C[4];   // +0x0C
};
static_assert(sizeof(S_HorseIrritationActionData) == 0x10, "S_HorseIrritationActionData must be 0x10");

}  // namespace wh::rpgmodule
