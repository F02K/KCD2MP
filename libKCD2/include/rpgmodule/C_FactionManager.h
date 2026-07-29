#pragma once
#include <cstdint>
#include <vector>
#include "I_FactionManager.h"
#include "../databasemodule/I_DatabaseListener.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// RTTI TD 0x184D4E730  primary vtable 0x183B62098 (I_FactionManager, 16 slots)  secondary vtable
// 0x183B62088 (I_DatabaseListener @+0x08).  Global singleton @0x1854B88C0 (ctor sub_181330B8C,
// dtor sub_1821817E0; a heap dtor variant sub_182CA85D0 also exists).  SIZEOF derived from the
// ctor write extent (last member @+0x68) -- no new(sizeof) immediate; matches KCD1's 0x70.
// The global ctor tail (sub_181330C24) registers a std::function<void(C_Faction&)> callback into
// a C_Faction signal (sub_180D924E0) -- the database-listener wiring.
//
// [FUNDAMENTAL vs KCD1] The KCD1 "3 boost flat_maps in the manager" premise is FALSIFIED:
// factions live in an external wh::databasemodule::C_ObjectDatabase (see C_Faction.h) and the
// manager keeps only the two bespoke compact indices below.

namespace wh::rpgmodule {

// 16-byte entry of the compact index; the key8/value8 split is inferred ONLY from the RTTR
// method signatures (GetFaction / GetFactionRelation) -- LOW confidence, flagged.
struct S_FactionMap16Entry {
    uint64_t m_key;     // +0x00
    uint64_t m_value;   // +0x08
};
static_assert(sizeof(S_FactionMap16Entry) == 0x10, "S_FactionMap16Entry must be 0x10");

// Bespoke wh compact 16B-entry array container (0x10): a single pointer to a size-prefixed heap
// block (int32 size @ptr-8; empty state = the shared ZERO-filled sentinel unk_185669670) plus
// two int16 init -1. Storage family is byte-identical to C_Signal's delegate array and
// S_WuidKeyHolder (same grow helper sub_180396724, elem 16; resize sub_1805FF138 reads *(ptr-8)).
// Whether the int16 pair is a cached binary-search HINT (sorted-array-map reading) or 2 inline
// bucket heads (2-bucket-hash reading) is UNRESOLVED -- definitively NOT a boost flat_map (0x18)
// and NOT a std::map (0x10 tree with 0x0101-nil head).
struct S_FactionCompactMap {
    S_FactionMap16Entry* m_entries;   // +0x00  size @ptr-8; shared zero sentinel while empty
    int16_t m_hint0;                  // +0x08  init -1 (search hint OR bucket head -- unresolved)
    int16_t m_hint1;                  // +0x0A  init -1
    uint8_t _pad0C[4];                // +0x0C
};
static_assert(sizeof(S_FactionCompactMap) == 0x10, "S_FactionCompactMap must be 0x10");

class C_FactionManager
    : public I_FactionManager                        // +0x00  (16-slot primary vtable)
    , public wh::databasemodule::I_DatabaseListener  // +0x08  (secondary vtable)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionManager;

    // Forwards to the engine accessor sub_180471F74 (TLS-guarded lazy init of the
    // 0x1854B88C0 static, atexit dtor). Impl in src/rpgmodule/C_FactionManager.cpp.
    static C_FactionManager* GetInstance();

    S_FactionCompactMap m_factionIndex;   // +0x10  role inferred: faction index (key8 -> value8; LOW)
    S_FactionCompactMap m_relationIndex;  // +0x20  role inferred: cross-faction relations (RTTR
                                          //        GetFactionRelation(string,string) -> C_FactionRelation; LOW)
    std::vector<void*> m_vec30;           // +0x30  owned + memory-accounted buffer (element unresolved;
                                          //        teardown sub_1813311A8 + conditional free sub_181AB5160)
    void*    m_pBuffer48;                 // +0x48  owned buffer, freed with accounting (sub_18082D290)
                                          //        when m_ownsBuffer58 is set
    uint64_t m_bufferAux50;               // +0x50  aux/size for +0x48 (LOW)
    bool     m_ownsBuffer58;              // +0x58  ownership / free-gate for +0x48
    uint8_t  _pad59[7];                   // +0x59
    int32_t  m_unk60;                     // +0x60  (ctor 0)
    uint32_t _pad64;                      // +0x64
    int64_t  m_unk68;                     // +0x68  free-gate / counter (LOW; last member -> 0x70)
};
static_assert(sizeof(C_FactionManager) == 0x70, "C_FactionManager must be 0x70");

}  // namespace wh::rpgmodule
