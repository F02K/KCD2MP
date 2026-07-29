#pragma once
#include <cstdint>
#include <functional>
#include <vector>
#include "S_LocationId.h"
#include "../framework/WUID.h"

// ===========================================================================
// wh::rpgmodule::I_RPGLocationManager -- C_RPGLocationManager primary interface
// (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVI_RPGLocationManager@rpgmodule@wh@@ (TD 0x184D4E688).  PURE interface (sizeof 0x08);
// primary base of C_RPGLocationManager (@+0x00). Vtable 0x183AB6400, 50 slots (KCD1: 40).
// All 50 slots decompiled; leaf semantics marked [INFERRED] where naming is judgment.
// NOT interfuscated (wh:: type -> canonical order).
//
// [FUNDAMENTAL vs KCD1] Slot 0 is NO LONGER the destructor -- it is Reset() (no virtual dtor in
// this vtable at all). Keys moved from CryGUID/WUID to the 16-byte S_LocationId; POI queries hand
// out I_POI* (KCD1 handed out I_Location*); a global POI-TYPE registry (56-byte rows
// @xmmword_18532DC80, see I_POI.h) is exposed through slots [43]..[46].
// "manager+0x.." member references below are C_RPGLocationManager offsets.
// ===========================================================================

namespace wh::rpgmodule {

class I_Location;
class I_POI;
class C_POI;
class I_LocationListener;

class I_RPGLocationManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RPGLocationManager;
    // [0]  0x00  reset the whole registry: SetStateFlag(2,true), worker sub_18127E730, flag off,
    //      then I_POI::Reset on every POI (map + the +0x140 vector), I_Location::Reset on every
    //      location, clear the +0xB8 per-type map.  (0x1805678CC)
    virtual void Reset() = 0;
    virtual uint8_t SetStateFlag(uint8_t mask, bool set) = 0;  // [1]  0x08  m_stateFlags (manager+0x18)  (0x180D10D90)
    virtual bool IsSuspended() = 0;                            // [2]  0x10  fast-travel active || m_stateFlags != 0  (0x180534DF8)
    // [3]  0x18  invoke the collector for every EXISTING location whose smart-area contains `point`:
    //      smart-area query -> area WUID -> m_areaKeyToLocId (+0x158) -> FindLocationById[10].
    //      Returns the hit count. The collector std::function is DESTROYED by the callee.  (0x18047BE88)
    virtual uint32_t EnumerateLocationsAtPoint(const Vec3& point,
                                               std::function<void(I_Location*)>& fn) = 0;
    // [4]  0x20  query the location spatial index (+0x218) and feed the collector; returns count.
    //      (0x180B57254)  [query arg shape INFERRED: 8-byte position/id + int]
    virtual uint32_t EnumerateLocationsSpatial(const void* query, int arg,
                                               std::function<void(I_Location*)>& fn) = 0;
    // [5]  0x28  walk m_locationsById, keep I_Location::MatchesKind(kind); returns count.  (0x180D10570)
    virtual uint32_t GetLocationsByKind(std::vector<I_Location*>& out, uint32_t kind) = 0;
    virtual uint32_t GetAllLocations(std::vector<I_Location*>& out) = 0;  // [6] 0x30  = [5] with the default kind (sub_180D1083C)  (0x182CAC478)
    // [7]  0x38  invoke the collector for every id in the sorted id array (+0x268), resolved through
    //      FindLocationById. Collector destroyed by callee.  (0x180B57408)  [array role INFERRED]
    virtual uint32_t EnumerateListedLocations(std::function<void(I_Location*)>& fn) = 0;
    virtual bool IsLocationListed(const S_LocationId& id) = 0;  // [8] 0x40  binary search of +0x268  (0x181FC0CF0)
    virtual I_Location* FindLocationByName(const char* name) = 0;  // [9] 0x48  case-insensitive walk of m_locationsById  (0x182CAC39C)
    // [10] 0x50  m_locationsById (+0x1D8) lookup -- find only, does NOT create (creation happens in
    //      the table loader sub_180B604D4).  (0x180468278)
    virtual I_Location* FindLocationById(const S_LocationId& id) = 0;
    // [11] 0x58  POI create variant -> sub_182CAAA68  (0x182CAABA8)  [args INFERRED from KCD1 slot 9]
    virtual I_POI* CreatePOIVariant(const S_LocationId& id, int, int, int, int) = 0;
    // [12] 0x60  get-or-create POI -> sub_18094FBC0  (0x182CAAB84)  [arg roles UNVERIFIED]
    virtual I_POI* GetOrCreatePOI(int64_t a2, int64_t a3, const S_LocationId& id) = 0;
    virtual bool RemovePOIById(const S_LocationId& id) = 0;   // [13] 0x68  erase m_poisById + spatial remove  (0x182CAE7AC)
    virtual I_POI* FindPOIById(const S_LocationId& id) = 0;   // [14] 0x70  m_poisById lookup  (0x182CAD9C4)
    // [15] 0x78  FindPOIType[44], then row id == one of two static special type ids (one TLS-built
    //      from guid "30089221-9268-450e-8aaf-61e4a7b5cfcd").  (0x18094FB0C)  [role INFERRED]
    virtual bool IsSpecialPOIType(const S_LocationId& typeId) = 0;
    virtual void _vf16_poiTypeOp(const S_LocationId& typeId) = 0;  // [16] 0x80  op on the +0xB8 per-type map (sub_180C4CAC8)  [UNVERIFIED]
    virtual float GetDefaultPOIRadius() = 0;                  // [17] 0x88  returns 20.0f  (0x181A93210)
    // [18] 0x90  POI spatial index (+0x60) sphere query, re-filter shape->ContainsPoint(center);
    //      returns count.  (0x182CADA5C)
    virtual uint32_t FindPOIsInSphere(const Vec3& c, float r, std::vector<I_POI*>& out) = 0;
    virtual uint32_t FindPOIsAtPoint(const Vec3& p, std::vector<I_POI*>& out) = 0;  // [19] 0x98  point query + I_POI[27] filter  (0x1809505C0)
    virtual uint32_t GetAllPOIs(std::vector<I_POI*>& out) = 0;  // [20] 0xA0  walk m_poisById  (0x1809503CC)
    virtual uint32_t FindPOIsByType(const S_LocationId& typeId, std::vector<I_POI*>& out) = 0;  // [21] 0xA8  compare C_POI::m_typeId  (0x182CADC7C)
    // [22] 0xB0  callback-based sphere traversal of the POI spatial index; returns hit count.  (0x181FC0C90)
    virtual uint32_t ForEachPOIInSphere(const Vec3& c, float r, void* cb, void* ctx) = 0;
    // [23] 0xB8  walk m_poisById, predicate sub_181FC0D30(this, poi, pred), sink sub_1823C90A0.  (0x181FC0C00)
    virtual uint32_t FindPOIsWhere(void* sink, const void* pred) = 0;
    // [24] 0xC0  spatial visit over the volume of `obj` (obj vfn[3] yields the query), callback cb.
    //      (0x182CADC20)  [INFERRED]
    virtual int QueryPOIsByVolume(void* obj, void* cb) = 0;
    virtual int ForEachPOIInSphere2(const Vec3& c, float r, void* cb) = 0;  // [25] 0xC8  plain spatial visit, returns 0  (0x18094E518)
    virtual uint32_t CountDiscoveredLocations() = 0;          // [26] 0xD0  walk m_locationsById, count IsDiscovered  (0x182CABFE4)
    virtual uint32_t GetFastTravelPOIs(std::vector<I_POI*>& out) = 0;  // [27] 0xD8  flags & 0x02 filter  (0x182CAC120)
    virtual uint32_t CountMapMarkers() = 0;                   // [28] 0xE0  IsFastTravel && IsDiscovered && !IsUnloaded  (0x1819A22F8)
    // [29] 0xE8  invoke pred for every {S_LocationId, _smart_ptr<C_POI>} map entry (value type
    //      PROVEN by the lambda vftable name)  (0x182CA9CE8)
    virtual uint32_t ForEachPOIEntry(void* pred) = 0;
    // [30] 0xF0  spatial point query into a temp vector, memmove into *out.  (0x182CAE610)  [INFERRED]
    virtual uint32_t QueryPOIsRegion(const void* query, void** out) = 0;
    virtual bool GetLocationPosition(const S_LocationId& id, Vec3& out) = 0;  // [31] 0xF8  FindLocationById -> GetPosition  (0x182CAC408)
    // [32] 0x100  location discovery state machine + listener notify (blocked while IsSuspended);
    //      also feeds the +0xB8 per-type map and the +0x288 listeners.  (0x180B5F39C)
    virtual int SetLocationDiscovered(const S_LocationId& id, int64_t a3, bool undiscover) = 0;
    // [33] 0x108  m_poisById lookup -> I_POI::SetDiscoveredGated(a3, a4).  (0x182CAAF90)
    virtual int SetPOIDiscovered(const S_LocationId& poiId, bool discover, bool force) = 0;
    // [34] 0x110  re-home the POI in m_poisByLocation (+0x78) and write C_POI::m_locationId.  (0x18094FF70)
    virtual int SetPOILocation(const S_LocationId& poiId, const S_LocationId& locationId) = 0;
    // [35] 0x118  create (operator new(0xA0), ctor sub_18094FD64) or update a POI: shape position/
    //      radius, markType, typeId, SetPOILocation, uiLabel, fast-travel confirm text.  (0x18094F508)
    virtual I_POI* AddPOI(const S_LocationId& id, const S_LocationId& locationId, int64_t a4,
                          const Vec3& pos, float radius, int markType, const S_LocationId& typeId,
                          const void* uiLabel, const void* ftConfirmText) = 0;
    // [36] 0x120  insert into BOTH id maps (+0x158 key->id, +0x198 id->key); duplicate re-register
    //      emits a "%s (%s)" rename; sets the dirty byte (+0x280). Returns false when the id was
    //      already registered under another key.  (0x181661C64)
    virtual bool RegisterLocationKey(const S_LocationId& id, wh::framework::WUID areaKey) = 0;
    // [37] 0x128  erase from both id maps; dirty byte. Called by the smart-area listener base when
    //      an area goes away.  (0x180D9AFB0)
    virtual void UnregisterLocationKey(wh::framework::WUID areaKey) = 0;
    // [38] 0x130  read a "location" guid attribute from the node, resolve the location and push the
    //      node's name into it.  (0x18188F704)  [INFERRED]
    virtual void LoadLocationFromNode(void* node) = 0;
    virtual void AddListener(I_LocationListener* l) = 0;      // [39] 0x138  push +0x288 (dup-checked) + l->OnRegistered(this)  (0x180C3F738)
    virtual void RemoveListener(I_LocationListener* l) = 0;   // [40] 0x140  l->OnUnregistered(this); deferred-null removal  (0x1806136AC)
    virtual void _vf41_setOp(void* a2) = 0;                   // [41] 0x148  sub_1812C4194(this, a2, true)  [UNVERIFIED]
    virtual void _vf42_clearOp(void* a2) = 0;                 // [42] 0x150  sub_1812C4194(this, a2, false)  [UNVERIFIED]
    // [43] 0x158  POI-type registry row whose markType (+0x18) == arg.  (0x181FC0B70)
    virtual const void* FindPOITypeByMarkType(int markType) = 0;
    // [44] 0x160  POI-type registry row by type id (binary search of xmmword_18532DC80).  (0x18094FB58)
    virtual const void* FindPOIType(const S_LocationId& typeId) = 0;
    // [45] 0x168  [44] with the static type id TLS-built from guid
    //      "7eac0ae7-b90f-45ce-b97c-c176a7ac8271".  (0x182CAE1B0)  [role INFERRED]
    virtual const void* GetDefaultPOIType() = 0;
    // [46] 0x170  registry row by label strcmp -> its type id (invalid id when not found).  (0x182CADA04)
    virtual S_LocationId* GetPOITypeIdByName(S_LocationId& out, const char* name) = 0;
    virtual bool AnyListedLocations() = 0;                    // [47] 0x178  [7] with a flag-setting lambda  (0x180B573CC)  [INFERRED]
    // [48] 0x180  m_locationsByNameHash (+0x228) lookup by sub_18078B5F0(name) -> SetLocationDiscovered.
    //      (0x182CAAEE4)
    virtual int SetLocationDiscoveredByName(const char* name, bool a3, bool a4) = 0;
    // [49] 0x188  POI state-change sink (I_POI::SetState calls this): notifies the +0x288 listeners
    //      through the packed-event walkers; on becoming discovered updates the +0xB8 per-type map
    //      unless the suppress count (+0xF8) is positive.  (0x180B5F550)
    virtual void OnPOIStateChanged(I_POI* poi, int oldState, int newState) = 0;
};
static_assert(sizeof(I_RPGLocationManager) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::rpgmodule
