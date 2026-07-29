#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::framework::S_SaveGameTypeSlot -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x48 (SYNTHETIC name).
// -----------------------------------------------
// Per-save-type record of C_SaveGameManager::m_slotsByType[5] (array-constructed by
// sub_18050B734(base, stride 72, count 5, ctor sub_1819BB49C)). The histogram was VERIFIED
// (correcting the first-pass "40 reserved bytes" reading): rebuild helper sub_1806E4F94 --
// called from the ctor and on every insert (sub_1806E4F0C) -- zeroes +0x18..+0x40 then, for each
// held description, increments the dword at +0x18 + 4*desc.m_saveType (desc+0x08, range 0..9).
// m_nextSlotIndex is bumped to desc.m_saveIndex+1 on insert.

namespace wh::framework {

class C_SaveGameDescription;

struct S_SaveGameTypeSlot {
    std::vector<C_SaveGameDescription*> m_saves;  // +0x00  (0x18) descriptions of this type
                                                  //        (8B ptr elements; insert sub_1806E4F0C)
    int32_t m_countsBySaveType[10];               // +0x18  per-E_SaveGameType count histogram
                                                  //        (rebuilt each insert by sub_1806E4F94)
    int32_t m_nextSlotIndex;                      // +0x40  next %03d index (init 1)
    uint32_t _pad44;                              // +0x44
};
static_assert(sizeof(S_SaveGameTypeSlot) == 0x48, "S_SaveGameTypeSlot must be 0x48");
static_assert(offsetof(S_SaveGameTypeSlot, m_countsBySaveType) == 0x18, "histogram at 0x18");
static_assert(offsetof(S_SaveGameTypeSlot, m_nextSlotIndex) == 0x40, "next index at 0x40");

}  // namespace wh::framework
