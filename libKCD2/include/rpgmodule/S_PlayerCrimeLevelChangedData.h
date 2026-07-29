#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PlayerCrimeLevelChangedData -- payload of C_PlayerCrimeLevelChangedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_180EC97D4 tracked-alloc 16).  Caller sub_180EC9780
// @0x180EC97B0 packs __PAIR64__(a3,a2): low dword = a2 (previous), high dword = a3 (new).

namespace wh::rpgmodule {

struct S_PlayerCrimeLevelChangedData {
    uint32_t m_previousLevel;   // +0x00  [proposed]
    uint32_t m_newLevel;        // +0x04  [proposed]
};
static_assert(sizeof(S_PlayerCrimeLevelChangedData) == 0x08, "S_PlayerCrimeLevelChangedData must be 0x08");

}  // namespace wh::rpgmodule
