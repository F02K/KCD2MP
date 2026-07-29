#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PlayerStartedToLootStashData -- payload of
// C_PlayerStartedToLootStashCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5B6C tracked-alloc 24).  Caller sub_182CFA714
// @0x182CFA76F: LODWORD=a2; high qword source untracked by the decompiler (leftover
// register) -- present but provenance UNVERIFIED.

namespace wh::rpgmodule {

struct S_PlayerStartedToLootStashData {
    int32_t  m_stashType;   // +0x00  id/stash type (= caller a2) [proposed]
    uint8_t  _pad04[4];     // +0x04
    uint64_t m_handle;      // +0x08  [U provenance]
};
static_assert(sizeof(S_PlayerStartedToLootStashData) == 0x10, "S_PlayerStartedToLootStashData must be 0x10");

}  // namespace wh::rpgmodule
