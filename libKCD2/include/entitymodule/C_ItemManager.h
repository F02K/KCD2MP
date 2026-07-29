#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <exception>
#include "../framework/I_WUIDMappingProvider.h"
#include "../framework/CryDeferrable.h"
#include "../framework/WUID.h"
#include "../framework/HashPrimitives.h"

// -----------------------------------------------
// wh::entitymodule::C_ItemManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x5B9070 (~6 MB).
// -----------------------------------------------
// RTTI .?AVC_ItemManager@entitymodule@wh@@  primary vtable 0x183EC1850 (3 slots)  ctor sub_180BEAB68
// Bases: I_WUIDMappingProvider (polymorphic, vtable @+0x00) + UnsafeOp::CryDeferrable<0> (empty).
// Singleton: C_EntityModule a1[25]. Dominated by the inline WUID->C_Item* registry (m_itemTable).
// The WUID key holders / wuid hashes are a bespoke Warhorse inline-bucket open hash (S_WuidHash) --
// NOT std::unordered_* (no load-factor/list/bucket-vector/mask); flagged accordingly. The three
// +0x5B8F68 maps ARE stock std::unordered_map; the two +0x5B9030 trees ARE stock std::map.

namespace wh::entitymodule {

class C_Item;
class C_ItemReservationManager;   // RTTI .?AVC_ItemReservationManager@entitymodule@wh@@

// One entry of the bespoke inline-bucket WUID hash (16B; internal field split tentative).
struct S_WuidHashEntry {
    wh::framework::WUID key;    // +0x00
    uint16_t            value;  // +0x08  slot/index payload
    uint16_t            next;   // +0x0A  chain index (0xFFFF = end)
    uint32_t            _pad0C; // +0x0C
};

// Bespoke inline-bucket open hash: a pointer to a heap entry buffer (with a {u32 cap@-8,u32 size@-4}
// header; == a shared all-zero static sentinel while empty) plus inline u16 bucket-head indices.
// Two forms exist: the 2-bucket "key holder" (0x10) and the 40-bucket "wuid hash" (0x60, trailing count).
struct S_WuidKeyHolder {                 // 0x10
    S_WuidHashEntry* m_entries;          // +0x00
    uint16_t         m_buckets[2];       // +0x08  (init 0xFFFF)
    uint8_t          _pad0C[4];          // +0x0C
};
struct S_WuidHash40 {                    // 0x60
    S_WuidHashEntry* m_entries;          // +0x00
    uint16_t         m_buckets[40];      // +0x08  (init 0xFFFF)
    uint64_t         m_count;            // +0x58
};
struct S_WuidKeyedHash {                 // 0x70  {2-bucket key part, 40-bucket hash part}
    S_WuidKeyHolder m_keyPart;           // +0x00
    S_WuidHash40    m_hashPart;          // +0x10
};

// One slot of the 6 MB inline item registry (0x18). Occupied: generation + pItem. Free: intrusive
// free-list links in [+0x04]/[+0x08].
struct S_ItemSlot {
    uint16_t generation;    // +0x00  must match (WUID >> 18)
    uint16_t _pad02;        // +0x02
    uint32_t freeListPrev;  // +0x04  (free chain)
    uint32_t freeListNext;  // +0x08  (free chain)
    uint32_t _pad0C;        // +0x0C
    C_Item*  pItem;         // +0x10  null = empty
};
static_assert(sizeof(S_ItemSlot) == 0x18, "S_ItemSlot must be 0x18");

// The embedded item WUID registry (spans C_ItemManager +0x168..+0x5B8F50). ctor sub_181F06920,
// free-list init sub_181F0FA80, lookup sub_181E3E090 (idx = WUID & 0x3FFFF, gen == WUID>>18).
struct S_ItemWuidTable {
    std::exception_ptr             m_ctorException;   // +0x000  (dtor calls _DeleteExceptionPtr on this)
    uint64_t                       m_header[3];       // +0x010  bookkeeping (runtime)
    std::array<S_ItemSlot, 250000> m_slots;           // +0x028  (0x5B8D80)
    uint64_t                       m_trailer[8];      // +0x5B8DA8  free-head/counter/recycled-index bookkeeping (fields unverified)
};
static_assert(sizeof(S_ItemWuidTable) == 0x5B8DE8, "S_ItemWuidTable size");

// 16-byte composite key of the three item maps (two WUID halves; two-WUID split inferred).
struct S_ItemMapKey {
    wh::framework::WUID a;   // +0x00
    wh::framework::WUID b;   // +0x08
    bool operator==(const S_ItemMapKey& o) const { return a == o.a && b == o.b; }
};

// 40-byte mapped value of m_treeB (opaque POD; contents unresolved).
struct S_TreeBValue { uint8_t _raw[0x28]; };

class C_ItemManager
    : public wh::framework::I_WUIDMappingProvider   // +0x00  (3-slot vtable; combined identity)
    , public UnsafeOp::CryDeferrable<0>             // empty mixin (-> ICryDeferrable)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemManager;
    // Created by the C_EntityModule ctor (sub_180BE8968, slot a1[25], alloc 0x5B9070 == sizeof);
    // mirrored to global qword_185487A78.
    static C_ItemManager* GetInstance();
    // Registry lookup sub_181E3E090(&m_itemTable, &wuid): validates type byte 2 + generation
    // (WUID >> 18) + the item's stored WUID.
    C_Item* LookupByWUID(const wh::framework::WUID& wuid);

    S_WuidKeyHolder m_keyHolderA;                      // +0x08   {sentinel, WUID key} holder
    S_WuidKeyHolder m_keyHolderB;                      // +0x18   owner index: C_Item::SetOwner (0x181F0F730)
                                                       //         upserts via 0x1804CA7EC(mgr+0x18, item, flag)
    S_WuidHash40    m_wuidHashA;                        // +0x28   WUID->u16 inline-bucket hash (40 buckets)
    S_WuidHash40    m_wuidHashB;                        // +0x88
    S_WuidKeyedHash m_wuidHashC;                        // +0xE8   keyed variant (0x70)
    uint64_t        m_cachedCtxHandles158[2];          // +0x158  cached in deferred init sub_18106498C: [0]=*(entityCtx+0x190), [1]=*(entityCtx+0x188) (entityCtx = sub_1809155C8; pointee types unverified)
    S_ItemWuidTable m_itemTable;                       // +0x168  the giant WUID->C_Item* registry
    // owning polymorphic handlers (released via virtual deleting dtor in ~C_ItemManager); iface unresolved
    std::vector<void*> m_ownedHandlers;                // +0x5B8F50
    // Three stock std::unordered_map keyed by the 16-byte composite key (FNV-1a S_DefaultHash).
    std::unordered_map<S_ItemMapKey, void*,                           wh::shared::S_DefaultHash<S_ItemMapKey>> m_mapA;  // +0x5B8F68  value = owning intrusive-refcount ptr (class unresolved)
    std::unordered_map<S_ItemMapKey, C_Item*,                         wh::shared::S_DefaultHash<S_ItemMapKey>> m_mapB;  // +0x5B8FA8  value = non-owning C_Item*
    std::unordered_map<S_ItemMapKey, std::vector<wh::framework::WUID>, wh::shared::S_DefaultHash<S_ItemMapKey>> m_mapC; // +0x5B8FE8
    C_ItemReservationManager* m_pReservationManager;   // +0x5B9028  owning; alloc'd 0x68B in sub_18106498C, released via vtbl[0](obj,1) deleting-dtor in sub_182A68FC4
    std::map<wh::framework::WUID, uint64_t> m_treeA;   // +0x5B9030
    uint64_t m_treeAKeyCounter;                        // +0x5B9040  pre-incremented in sub_1808D08C8; the ++value is the key inserted into m_treeA
    std::map<wh::framework::WUID, S_TreeBValue> m_treeB;// +0x5B9048
    int32_t  m_mapLock;                                // +0x5B9058  RW spinlock (writer bit 0x10000)
    uint32_t _pad5B905C;                               // +0x5B905C
    int32_t  m_itemTableLock;                          // +0x5B9060  spinlock (sub_18083EB2C = _InterlockedExchange(&,1) spin); guards m_itemTable
    uint32_t _pad5B9064;                               // +0x5B9064
    int32_t  m_wuidHashLock;                           // +0x5B9068  spinlock (sub_18083EB2C); guards the WUID key holders / m_keyHolderA (base+8)
    bool     m_treeBRecording;                         // +0x5B906C  gates m_treeB writes: set 1 @0x1809B4930, cleared @sub_180D1E180/0x1829E1BA6, tested @sub_180468578
    uint8_t  _pad5B906D[3];                            // +0x5B906D  align to 0x5B9070
};
static_assert(sizeof(C_ItemManager) == 0x5B9070, "C_ItemManager must be 0x5B9070");

}  // namespace wh::entitymodule
