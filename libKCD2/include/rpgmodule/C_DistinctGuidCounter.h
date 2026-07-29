#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_set>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_DistinctGuidCounter -- KCD2 WHGame.dll 1.5.6 (kd7u).  POINTER-ONLY MODEL.
// -----------------------------------------------
// The DistinctGuid statistic counter (statistic.xml type="DistinctGuid": UniquePotionsBrewed,
// UniqueDocsStudied, UniqueRecipesPrepared, ...).  NO MSVC TypeDescriptor exists for it
// (RTTI-stripped TU; searched) -- the name is its RTTR registration
// "wh::rpgmodule::C_DistinctGuidCounter" (registrar 0x1801DAF20: "Value" -> vf+0x38 wrapper
// 0x182138C98, "Reset" -> vf+0x98 wrapper 0x18213A0B4).  No allocation/ctor site located
// either, so the OBJECT SIZE IS UNPROVEN: the head below (through m_guids, +0x60) is
// certified from AddGuid 0x182CF8054 (set insert @+0x20, change notify @+0x08), the set
// find/insert pair 0x180B5F9A0/0x180B5F8A8 (textbook MSVC std::_Hash field walk: head @set+8,
// buckets @set+0x18, mask @set+0x30; 0x20-byte nodes, guid @node+0x10) and the per-counter
// save-chunk dispatch (vf+0x88, sub_181FE5230).  NEVER instantiate, embed or extend this
// class -- obtain instances only via C_Statistics::GetCounterByName + FromCounter.
// NO BASE DECLARED on purpose: in retail it is surely a C_Counter/I_Counter subclass like its
// RTTI-bearing siblings (registry stores shared_ptr<C_Counter>), but rttr_cast's pointer
// adjustment is data-driven (type-graph walk sub_18069A498) so the facet offset is not
// statically provable -- all offsets here are relative to the POST-CAST pointer FromCounter
// returns, with its own vptr at +0x00.  Declaring a guessed base chain would shift them.
// C_EventCounter caches resolved instances in its per-statistic slot bank and feeds them from
// game events (e.g. guid-adding handler 0x182D4FC70).

namespace wh::rpgmodule {

class I_Counter;

class C_DistinctGuidCounter {
public:
    // The engine's guid hash, read off 0x1808B7C70/0x1808B7C50: FNV1a-64 over each 8-byte
    // guid half, halves XORed (basis 0xCBF29CE484222325, prime 0x100000001B3).  Defined
    // inline because Contains() executes it against the game-built buckets.
    struct S_GuidHashFnv1a {
        uint64_t operator()(const CryGUID& guid) const {
            const auto* b = reinterpret_cast<const uint8_t*>(&guid);
            uint64_t lo = 0xCBF29CE484222325ull;
            uint64_t hi = 0xCBF29CE484222325ull;
            for (int i = 0; i < 8; ++i)
                lo = 0x100000001B3ull * (lo ^ b[i]);
            for (int i = 8; i < 16; ++i)
                hi = 0x100000001B3ull * (hi ^ b[i]);
            return lo ^ hi;
        }
    };

    // The value-changed notify stack AddGuid fires on first-time inserts (reverse {listener,
    // callback}-pair walk sub_1803A524C; observers register through vf+0xB0).
    struct S_NotifyStack {
        void*    m_pairsTable;   // +0x00  {listener, fn} pairs, count word at table-4
        uint16_t m_cursors[2];   // +0x08  live-iteration cursors (0xFFFF = idle)
        uint8_t  _pad0C[4];      // +0x0C
    };
    static_assert(sizeof(S_NotifyStack) == 0x10, "notify stack head is 0x10");

    // Checked rttr downcast (0x182CE973C) -- null when the counter is not a distinct-guid
    // statistic; the exact cast the engine's own guid-adder chain 0x181FCE870 makes.
    static C_DistinctGuidCounter* FromCounter(I_Counter* counter);

    bool Contains(const CryGUID& guid) const { return m_guids.count(guid) != 0; }

    // Certified slots: [7] +0x38 Value (= set size, RTTR "Value"), [17] +0x88 LoadChunk
    // (save-chunk reader), [19] +0x98 Reset (RTTR "Reset"), [22] +0xB0 Subscribe (observer
    // registration).  Everything else is an uncertified filler -- DO NOT call through it;
    // the vtable continues past [22] (bound unknown).
    virtual void    _vf0() = 0;                     // [0]
    virtual void    _vf1() = 0;                     // [1]
    virtual void    _vf2() = 0;                     // [2]
    virtual void    _vf3() = 0;                     // [3]
    virtual void    _vf4() = 0;                     // [4]
    virtual void    _vf5() = 0;                     // [5]
    virtual void    _vf6() = 0;                     // [6]
    virtual int64_t GetValue() = 0;                 // [7]  +0x38  distinct-guid count
    virtual void    _vf8() = 0;                     // [8]
    virtual void    _vf9() = 0;                     // [9]
    virtual void    _vf10() = 0;                    // [10]
    virtual void    _vf11() = 0;                    // [11]
    virtual void    _vf12() = 0;                    // [12]
    virtual void    _vf13() = 0;                    // [13]
    virtual void    _vf14() = 0;                    // [14]
    virtual void    _vf15() = 0;                    // [15]
    virtual void    _vf16() = 0;                    // [16]
    virtual void    LoadChunk(void* chunk) = 0;     // [17] +0x88  save-chunk reader
    virtual void    _vf18() = 0;                    // [18]
    virtual void    Reset() = 0;                    // [19] +0x98  clear the set
    virtual void    _vf20() = 0;                    // [20]
    virtual void    _vf21() = 0;                    // [21]
    virtual void    Subscribe(void* record) = 0;    // [22] +0xB0  observer registration

    S_NotifyStack m_changedNotify;   // +0x08  fired by AddGuid when a NEW guid lands
    uint8_t       _unk18[8];         // +0x18  unmapped
    std::unordered_set<CryGUID, S_GuidHashFnv1a>
                  m_guids;           // +0x20  the distinct-guid set (save-persisted)
    // Object continues past +0x60 -- size unproven, tail unmodeled (see banner).
};
static_assert(offsetof(C_DistinctGuidCounter, m_guids) == 0x20, "guid set at 0x20");
static_assert(sizeof(std::unordered_set<CryGUID, C_DistinctGuidCounter::S_GuidHashFnv1a>) == 0x40,
              "MSVC unordered_set header must be 0x40 to match the engine walk");

}  // namespace wh::rpgmodule
