#pragma once
#include <cstdint>
#include <array>
#include <exception>
#include <set>
#include <unordered_map>
#include <vector>
#include "I_SoulResolver.h"
#include "C_StormProgram.h"
#include "../framework/I_WUIDMappingProvider.h"
#include "../framework/S_WuidSlot.h"
#include "../framework/WUID.h"
#include "../framework/C_Signal.h"
#include "../framework/HashPrimitives.h"
#include "../databasemodule/I_DatabaseListener.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulList -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x100220 (~1 MB).
// -----------------------------------------------
// RTTI .?AVC_SoulList@rpgmodule@wh@@ (TD 0x184D72170)  ctor sub_180BEAE80  dtor sub_182D25680.
// Owned by C_RPGModule @+0x80 (operator new(0x100220) -- sizeof PROVEN by the alloc immediate).
// MI: I_SoulResolver @+0x00 (combined 39-slot primary vtable 0x183F44DD8),
//     I_WUIDMappingProvider @+0x08 (vtable 0x183F44F10: [0] MapGuid 0x180BEBEA8, [1] MapWuid 0x180BEBD90),
//     I_DatabaseListener @+0x10 (vtable 0x183F44DC8, single virtual 0x18213AA34).
// Dominated by the inline WUID->C_Soul* slot registry: SAME generic S_WuidSlot family as
// C_InventoryManager (lookup sub_181F985D0: idx = low u16 (nonzero), generation = bits[16:31],
// tag = HIBYTE == 5; slot 0 reserved with generation 0xFFFF; free-list init sub_181EDF250).

namespace wh::rpgmodule {

class C_Soul;

// The embedded soul WUID registry (C_SoulList +0x38..+0x100080). Same header/slots/trailer shape
// as C_ItemManager's S_ItemWuidTable; header free-list field NAMES + the trailer/count semantics
// are inferred from zero-init only (flagged), the slot array itself is verify-confirmed.
struct S_SoulWuidTable {
    std::exception_ptr m_ctorException;   // +0x000  (dtor calls _DeleteExceptionPtr here -- verify-confirmed)
    uint64_t m_freeList[3];               // +0x010  free-list bookkeeping (freeHead/freeTail/count -- names unverified)
    std::array<wh::framework::S_WuidSlot<C_Soul>, 0x10000> m_slots;  // +0x028  (0x100000; idx 0 reserved, gen 0xFFFF)
    uint64_t m_trailer[3];                // +0x100028  bookkeeping (unverified)
    uint32_t m_count;                     // +0x100040  soul/active count (LOW confidence)
    uint32_t _pad100044;                  // +0x100044
};
static_assert(sizeof(S_SoulWuidTable) == 0x100048, "S_SoulWuidTable size");

class C_SoulList
    : public I_SoulResolver                          // +0x00
    , public wh::framework::I_WUIDMappingProvider    // +0x08
    , public wh::databasemodule::I_DatabaseListener  // +0x10
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulList;

    // ---- engine-function forwarders (src/rpgmodule/C_Soul.cpp) ----
    static C_SoulList* GetInstance();                          // C_RPGModule::GetInstance()->m_pSoulList
    C_Soul* LookupByWUID(const wh::framework::WUID& wuid);     // sub_181F985D0(&m_soulTable, &wuid)

    wh::shared::C_Signal<> m_onSoulAdded;    // +0x18  (purpose inferred from context; dtor sub_182D2532C)
    wh::shared::C_Signal<> m_onSoulRemoved;  // +0x28  (purpose inferred; dtor sub_182D25384)
    S_SoulWuidTable m_soulTable;             // +0x38  slots @abs +0x60 (lookup base = this+0x38)
    // Two CryGUID-keyed hash maps (FNV-1a over the 16-byte key, sub_18047AAC4; node 0x28).
    // WHICH guid each keys on (instance vs template) is unresolved.
    std::unordered_map<CryGUID, C_Soul*, wh::shared::S_DefaultHash<CryGUID>> m_soulsByGuid;   // +0x100080
    std::unordered_map<CryGUID, C_Soul*, wh::shared::S_DefaultHash<CryGUID>> m_soulsByGuid2;  // +0x1000C0
    uint32_t m_unk100100;                    // +0x100100  flag/count (unresolved)
    uint32_t _pad100104;                     // +0x100104
    C_Soul*  m_pNullSoul;                    // +0x100108  ctor-time cached copy of the global fallback
                                             //            qword_185332358 (MapGuid returns the GLOBAL on miss)
    std::set<uint64_t> m_tree100110;         // +0x100110  element type unresolved (8B payload; C_Soul*/WUID/u32-pair candidates)
    std::set<uint64_t> m_tree100120;         // +0x100120  (both torn down by sub_182D255AC)
    std::unordered_map<uint64_t, void*, wh::shared::S_DefaultHash<uint64_t>> m_ownedMap;  // +0x100130
                                             //            values HEAP-OWNED (node dtor sub_1829D5C8C frees value@+0x20);
                                             //            key/value types unresolved (EntityId->C_Soul* hint NOT confirmed)
    bool     m_flag100170;                   // +0x100170
    uint8_t  _pad100171[7];                  // +0x100171
    storm::C_StormProgram m_stormProgram;    // +0x100178  (0x80)
    std::vector<uint32_t> m_ids1001F8;       // +0x1001F8  4-byte elems PROVEN; std::vector vs boost
                                             //            flat_set<u32> unresolved (likely EntityId list)
    std::set<uint64_t> m_tree100210;         // +0x100210  element type unresolved
};
static_assert(sizeof(C_SoulList) == 0x100220, "C_SoulList must be 0x100220");
static_assert(offsetof(C_SoulList, m_soulTable) + offsetof(S_SoulWuidTable, m_slots) == 0x60, "slots at 0x60");
static_assert(offsetof(C_SoulList, m_soulsByGuid) == 0x100080, "guid map at 0x100080");

}  // namespace wh::rpgmodule
