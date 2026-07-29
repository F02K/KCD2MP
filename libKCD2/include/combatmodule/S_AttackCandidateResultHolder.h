#pragma once
#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// wh::combatmodule::S_AttackCandidateResultHolder -- KCD2 WHGame.dll 1.5.6 (kd7u). sizeof 0x128.
// -----------------------------------------------
// Result container the attack-candidate enumerator fills: an inline arena (15 records) served to
// a short_alloc-style std::vector of 0x10-byte records, plus an "enumerating" guard flag. No
// RTTI / no vtable (a plain stack composite; it never had a TD, in KCD1 or KCD2).
//
// [SURVIVED -- KCD2 shape == KCD1 shape] Proven from the enumerator sub_180687F3C (RVA 0x687F3C):
// it reads the vector's first @holder+0x108 / last @holder+0x110, sets the guard @holder+0x120=1
// during fill, walks the global moveset via a std::function<void(unique_ptr<I_DatabaseSelector<
// S_CombatActionAttackQueryData, S_CombatActionAttackData>> const&, S_QueryData&)>> appending
// candidates, then introsorts with element stride (last-first)>>4 = 0x10 and clears the guard.
// This is byte-for-byte the KCD1 layout: arena @+0x000 (0x100), pool-vector @+0x100 (0x20 =
// {allocptr, first, last, capEnd}), flag @+0x120, total 0x128; records 0x10.
//
// KCD2's RE tree has no C_InlinePoolAllocator.h yet, so the arena+vector are modeled with raw
// fields (exact layout) rather than the std::vector<T, C_InlinePoolAllocator<...>> alias KCD1
// used. The behavior (15-slot inline reserve, grow-through-pool) is identical.

namespace wh::combatmodule {

struct S_CombatActionAttackData;   // candidate payload (moveset selector result)

// One 0x10-byte candidate record the enumerator appends.
struct S_AttackCandidateRecord {
    S_CombatActionAttackData* m_pAttackData;  // +0x00  the candidate
    uint8_t  m_feasibilityFlag;               // +0x08  filled for the selected record (KCD1 parity; tentative)
    bool     m_zoneReachableFlag;             // +0x09  per-candidate flag (enumerator sub_180687F3C; KCD1 parity)
    uint8_t  _pad0A[6];                       // +0x0A  (stride is 0x10)
};
static_assert(sizeof(S_AttackCandidateRecord) == 0x10, "S_AttackCandidateRecord must be 0x10");

struct S_AttackCandidateResultHolder {
    uint8_t  m_arena[0x100];        // +0x000  inline arena (15 records) served to the vector below
    // MSVC std::vector<S_AttackCandidateRecord, pool-allocator>: {alloc-state, first, last, capEnd}.
    void*                    m_allocState; // +0x100  stateful inline-pool allocator member of the 0x20 vector {alloc,first,last,capEnd}@+0x100..+0x11F (self-points to m_arena)
    S_AttackCandidateRecord* m_first;      // +0x108  begin
    S_AttackCandidateRecord* m_last;       // +0x110  end
    S_AttackCandidateRecord* m_capEnd;     // +0x118  capacity end
    uint8_t  m_enumerating;         // +0x120  guard flag set during fill (sub_180687F3C)
    uint8_t  _pad121[7];            // +0x121

    S_AttackCandidateRecord* begin() { return m_first; }
    S_AttackCandidateRecord* end()   { return m_last; }
    bool empty() const { return m_first == m_last; }

    // Fill from a built query (engine enumerator sub_180687F3C(context, this, query)).
    void Enumerate(void* context, struct S_CombatActionAttackQueryData* query);
};
static_assert(sizeof(S_AttackCandidateResultHolder) == 0x128, "S_AttackCandidateResultHolder must be 0x128");
static_assert(offsetof(S_AttackCandidateResultHolder, m_first) == 0x108, "m_first offset");
static_assert(offsetof(S_AttackCandidateResultHolder, m_last) == 0x110, "m_last offset");
static_assert(offsetof(S_AttackCandidateResultHolder, m_enumerating) == 0x120, "m_enumerating offset");

}  // namespace wh::combatmodule
