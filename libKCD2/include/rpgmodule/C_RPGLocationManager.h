#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <vector>
#include "I_RPGLocationManager.h"
#include "C_RPGBase.h"
#include "I_LocationListener.h"
#include "C_POI.h"
#include "C_RPGLocation.h"
#include "S_LocationId.h"
#include "../framework/WUID.h"
#include "../framework/HashPrimitives.h"
#include "../Offsets/vtables/ISystem.h"   // Offsets::ISystemEventListener replica
#include "../xgenaimodule/I_SmartAreasManagerListener.h"
#include "../CryEngine/CryCommon/smartptr.h"

// ===========================================================================
// wh::rpgmodule::C_RPGLocationManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x2B0 (PROVEN:
// ctor sub_180E0F960 write extent; C_RPGModule.h documents the operator new size).
// ===========================================================================
// RTTI .?AVC_RPGLocationManager@rpgmodule@wh@@ (TD 0x184D4E6C0).  Vtables: primary 0x183AB6400
// (50 slots, see I_RPGLocationManager.h), +0x08 0x183AB63C0 (ISystemEventListener),
// +0x10 0x183AB63E0 (I_SmartAreasManagerListener: forwards area removal to UnregisterLocationKey).
// Owned by C_RPGModule @+0xB8 (qword_1853322A0 + 184 throughout the module).
// Base declaration order per the RTTI base array: [I_RPGLocationManager, ISystemEventListener,
// C_RPGBase, I_SmartAreasManagerListener] -- MSVC lays the non-polymorphic C_RPGBase out AFTER the
// three vfptr bases, at +0x18 (RTTI mdisp 24).
//
// Hash members: the ctor shows the stock MSVC std::_Hash fingerprint for the maps at +0x20/+0x78
// and family-B inits (sub_1806E4638) for +0x158/+0x198/+0x1D8 (bucket-mask ops at map+0x30 confirm
// std::_Hash) -- those five are typed std::unordered_map (game hashers modeled with S_DefaultHash;
// the 0x40 size is functor-independent). The family-A blocks (sub_1806030E4: +0xB8/+0x100/+0x228)
// were NOT layout-fingerprinted and stay untyped 0x40 blobs.
//
// [FUNDAMENTAL vs KCD1] 0x298 -> 0x2B0; keys CryGUID/WUID -> S_LocationId; the by-name POI map and
// the two special-location guids are gone (POI types moved to a global registry, the "special"
// location ids are TLS statics); new: smart-area listener base, name-hash location map, sorted
// listed-id array.
// ===========================================================================

namespace wh::rpgmodule {

// POI spatial acceleration tree (+0x60; insert sub_18094F350 / sub_18094F894, queries
// sub_182C9CCB0 / sub_18094F0F0 / sub_18094F230 / sub_182C9CB94).
struct S_LocationSpatialIndex {
    void*    m_root;             // +0x00  spatial-tree root node* (ctor 0, lazy-alloc sub_18094F894 = 0x48-byte node {AABB bounds, vector<_smart_ptr<C_POI>>, vector<node*> children}; recursive insert sub_1805D47B8)
    uint32_t m_count;            // +0x08  inserted count (ctor 0)
    uint32_t m_maxItemsPerNode;  // +0x0C  ctor 16 [INFERRED config]
    uint32_t m_splitParam;       // +0x10  ctor 4  [INFERRED config]
    uint32_t _pad14;             // +0x14
};
static_assert(sizeof(S_LocationSpatialIndex) == 0x18, "spatial index descriptor (ctor sub_180E0F960)");

// Location spatial index (+0x218; query sub_180B58C48) -- same family, no count field.
struct S_LocationSpatialIndexLite {
    void*    m_root;             // +0x00  spatial-tree root node* (ctor 0, lazy; query sub_180B58C48 walks node = {bounds @+0, vector<_smart_ptr<C_RPGLocation>> items @+0x10, vector<node*> children @+0x28})
    uint32_t m_maxItemsPerNode;  // +0x08  ctor 16 [INFERRED config]
    uint32_t m_splitParam;       // +0x0C  ctor 8  [INFERRED config]
};
static_assert(sizeof(S_LocationSpatialIndexLite) == 0x10, "location spatial index (ctor sub_180E0F960)");

// The +0x268 sorted id array ({data, count, capacity}; 16-byte stride walk in slot [7],
// lower-bound membership in slot [8]).
struct S_LocationIdArray {
    S_LocationId* m_data;    // +0x00
    uint64_t      m_count;   // +0x08
    uint64_t      m_capacity;// +0x10
};
static_assert(sizeof(S_LocationIdArray) == 0x18, "listed-id array (ctor zeroes all three)");

class C_RPGLocationManager
    : public I_RPGLocationManager                        // +0x00
    , public Offsets::ISystemEventListener               // +0x08
    , public C_RPGBase                                   // +0x18  (laid out after the vfptr bases)
    , public wh::xgenaimodule::I_SmartAreasManagerListener  // +0x10
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGLocationManager;
    // = C_RPGModule::GetInstance()->m_pLocationManager. Impl in src/rpgmodule/rpgmodule.cpp.
    static C_RPGLocationManager* GetInstance();

    uint8_t _pad1A[6];                                   // +0x1A  (C_RPGBase ends at 0x1A)

    // ---- POI registry ----
    // Value type PROVEN: std::pair<S_LocationId, _smart_ptr<C_POI>> (lambda vftable name in [29]).
    std::unordered_map<S_LocationId, _smart_ptr<C_POI>,
                       wh::shared::S_DefaultHash<S_LocationId>> m_poisById;        // +0x20
    S_LocationSpatialIndex m_poiSpatialIndex;                                      // +0x60
    // POIs grouped by owning location (SetPOILocation re-homes; value walked as raw C_POI*).
    std::unordered_map<S_LocationId, std::vector<C_POI*>,
                       wh::shared::S_DefaultHash<S_LocationId>> m_poisByLocation;  // +0x78
    // Per-POI-TYPE tracking: keyed by typeId (slot [16], updated on discovery in [49], cleared by
    // Reset via sub_180A592E8). Family-A 0x40 block -- internals NOT fingerprinted, left untyped.
    uint64_t m_poiTypeTracking[8];                                                 // +0xB8
    int32_t  m_mapNotifySuppress;    // +0xF8  > 0 suppresses [49]'s type update and I_POI [35]
    uint32_t _padFC;                 // +0xFC
    // std::_Hash (unordered_set/map), 0x40: ctor init sub_1806030E4 writes the stock fingerprint
    // (load-factor 1.0f @+0, self-ref list head @+8 [sub_1804F75C0(32) sentinel -> 16-byte value_type],
    // size @+0x10, bucket vec @+0x18, mask 7 @+0x30, maxidx 8 @+0x38) -- same shape as the family-B
    // std::unordered_map helper sub_1806E4638. Family-A sibling +0xB8 is a PROVEN
    // std::unordered_set<S_LocationId> (S_DefaultHash FNV-1a over 16B key, membership op sub_180C4CAC8).
    // m_unkMap100 value_type identity UNPROVEN (no reader/writer among the 50 slots); candidate
    // std::unordered_set<S_LocationId>. role UNKNOWN.
    uint64_t m_unkMap100[8];                                                       // +0x100
    // Second POI list, also reset by Reset(); population site not located [role UNRESOLVED].
    std::vector<C_POI*> m_poisAux140;                                              // +0x140

    // ---- smart-area key <-> location id translation ----
    std::unordered_map<wh::framework::WUID, S_LocationId,
                       wh::shared::S_DefaultHash<wh::framework::WUID>> m_areaKeyToLocId;  // +0x158
    std::unordered_map<S_LocationId, wh::framework::WUID,
                       wh::shared::S_DefaultHash<S_LocationId>> m_locIdToAreaKey;         // +0x198

    // ---- location registry (one C_RPGLocation per definition-table row, loader sub_180B604D4) ----
    std::unordered_map<S_LocationId, _smart_ptr<C_RPGLocation>,
                       wh::shared::S_DefaultHash<S_LocationId>> m_locationsById;   // +0x1D8
    S_LocationSpatialIndexLite m_locationSpatialIndex;                             // +0x218
    // Locations keyed by sub_18078B5F0(name) (loader fills; slot [48] looks up). Family-A 0x40
    // block -- untyped.
    uint64_t m_locationsByNameHash[8];                                             // +0x228
    S_LocationIdArray m_listedIds;   // +0x268  sorted; slots [7]/[8]/[47] [role INFERRED]
    uint8_t  m_idMapsDirty;          // +0x280  set by Register/UnregisterLocationKey
    uint8_t  _pad281[7];             // +0x281

    // ---- change-notify listeners (deferred-removal notify stack) ----
    std::vector<I_LocationListener*> m_listeners;  // +0x288  init sub_180E0FB0C; add [39] / remove [40]
    uint64_t m_notifyDepth;          // +0x2A0  > 0 defers removals (RemoveListener nulls in place)
    uint8_t  m_hasNullEntries;       // +0x2A8  listener nulled mid-notify -> compaction pending
    uint8_t  m_shrinkOnCompact;      // +0x2A9  zeroed together with the flag (WORD write) [role carried from KCD1]
    uint8_t  _pad2AA[6];             // +0x2AA
};
static_assert(sizeof(C_RPGLocationManager) == 0x2B0, "ctor operator new(0x2B0) via C_RPGModule");
static_assert(offsetof(C_RPGLocationManager, m_poisById) == 0x20, "POI map at 0x20");
static_assert(offsetof(C_RPGLocationManager, m_poiSpatialIndex) == 0x60, "POI spatial index at 0x60");
static_assert(offsetof(C_RPGLocationManager, m_areaKeyToLocId) == 0x158, "key->id map at 0x158");
static_assert(offsetof(C_RPGLocationManager, m_locationsById) == 0x1D8, "location map at 0x1D8");
static_assert(offsetof(C_RPGLocationManager, m_listedIds) == 0x268, "listed-id array at 0x268");
static_assert(offsetof(C_RPGLocationManager, m_listeners) == 0x288, "listener vector at 0x288");

}  // namespace wh::rpgmodule
