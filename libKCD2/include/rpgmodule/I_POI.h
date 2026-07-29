#pragma once
#include <cstdint>
#include <functional>
#include "S_LocationId.h"
#include "../CryEngine/CryCommon/CryString.h"

// ===========================================================================
// wh::rpgmodule::I_POI -- POI (map point-of-interest) interface (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVI_POI@rpgmodule@wh@@ (TD 0x184D54008).  PURE interface (sizeof 0x08); second base of
// C_POI (@+0x10) -- all data lives there. NOT an I_Location in KCD2 (C_POI's RTTI hierarchy is
// [_reference_target<int>, I_POI] only; KCD1 handed POIs out as I_Location*).
// Vtable (C_POI subobject): 0x183A64998, 42 slots (KCD1: 35). No virtual dtor (slot 0 is GetId);
// destruction goes through the _reference_target<int> primary base.
//
// Flag bits (m_flags @C_POI+0x98; bit names are the ENGINE'S OWN, from the serializer slot [41]):
//   0x01 IsDiscoverable   0x02 IsFastTravel   0x04 IsProcedural   0x08 IsBed
//   0x10 IsFollowsEntity  0x20 IsUnloaded     0x40 IsNotDiscoverableByLocation
// State (m_state @C_POI+0x9C): 0 = none, 2 = discovered (1 = revealed-but-undiscovered [INFERRED
// from SetDiscoveredGated's transitions]).
//
// "def" below = the POI-TYPE registry row for m_typeId (global sorted array xmmword_18532DC80,
// 56-byte rows): {+0x00 S_LocationId typeId, +0x18 int32 markType, +0x20 char* label,
// +0x2D uint8 availabilityMode, +0x30 int32} [row typing partial]. Resolved via sub_18094EFB4.
// ===========================================================================

namespace wh::rpgmodule {

class I_Location;

class I_POI {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_POI;
    virtual const S_LocationId* GetId() = 0;                 // [0]  0x00  &m_id (C_POI+0x18) (0x181A71D30)
    virtual S_LocationId* GetTypeId(S_LocationId& out) = 0;  // [1]  0x08  m_typeId (C_POI+0x50) by value (0x180B5F740)
    virtual const S_LocationId* GetLocationId() = 0;         // [2]  0x10  &m_locationId (C_POI+0x28) (0x181A72EB0)
    // [3]  0x18  owning location: m_locationId -> C_RPGLocationManager::FindLocationById
    virtual I_Location* GetLocation() = 0;                   // 0x1819C2BB8
    virtual CryStringT<char>* GetLayerName() = 0;            // [4]  0x20  &m_layerName (C_POI+0x38) (0x181A73320)
    virtual void SetLayerName(const char* s, size_t len) = 0;  // [5] 0x28  string assign (0x1807EA6A4)
    virtual uint32_t GetInitialState() = 0;                  // [6]  0x30  C_POI+0x94 (0x181A7D6A0)
    virtual void SetInitialState(uint32_t s) = 0;            // [7]  0x38  (0x181A7F830)
    virtual uint32_t GetState() = 0;                         // [8]  0x40  C_POI+0x9C (0x181A733C0)
    // [9]  0x48  state write; on change and notify==true calls
    //      I_RPGLocationManager::OnPOIStateChanged(this, old, new) (manager slot [49])
    virtual bool SetState(uint32_t s, bool notify) = 0;      // 0x181FCCFA0
    virtual bool IsDiscovered() = 0;                         // [10] 0x50  GetState() == 2 (0x18094E9C4)
    virtual bool IsStateNone() = 0;                          // [11] 0x58  GetState() == 0 (0x182CC7ECC)
    virtual uint16_t GetFlags() = 0;                         // [12] 0x60  m_flags (C_POI+0x98) (0x181A93920)
    virtual uint16_t SetFlags(uint16_t mask, bool set) = 0;  // [13] 0x68  |= mask / &= ~mask (0x1805679BC)
    virtual bool HasFlags(uint16_t mask) = 0;                // [14] 0x70  (flags & mask) == mask (0x18191DDE4)
    virtual uint64_t GetEntityId() = 0;                      // [15] 0x78  C_POI+0x40 [id kind UNVERIFIED] (0x18072DD20)
    virtual void SetEntityId(uint64_t id) = 0;               // [16] 0x80  (0x181A7E070)
    virtual bool IsDiscoverable() = 0;                       // [17] 0x88  flags & 0x01 (0x18139730C)
    virtual bool IsFastTravel() = 0;                         // [18] 0x90  flags & 0x02 (0x1805679F0)
    virtual bool IsFastTravelMark7() = 0;                    // [19] 0x98  IsFastTravel && GetMarkType()==7 [name INFERRED] (0x181FCCE60)
    virtual bool IsProcedural() = 0;                         // [20] 0xA0  flags & 0x04 (0x182CAE354)
    virtual bool IsBed() = 0;                                // [21] 0xA8  flags & 0x08 (0x181385EC8)
    virtual bool IsUnloaded() = 0;                           // [22] 0xB0  flags & 0x20 (0x1805679DC)
    virtual bool HasMappableMarkType() = 0;                  // [23] 0xB8  !def || def->markType >= 0 [name INFERRED] (0x18094ECCC)
    // [24] 0xC0  shown-on-map gate: !unloaded && discoverable && !notDiscoverableByLocation, then
    //      fast-travel => true, else the type-registry row's availability byte (+0x2D)
    virtual bool IsDiscoverableOnMap() = 0;                  // 0x182CC7E50  [name INFERRED]
    virtual void* _vf25_typeTrackingOp() = 0;                // [25] 0xC8  manager slot [16] on the +0xB8 per-type map with m_typeId [UNVERIFIED] (0x180C4CDE4)
    virtual Vec3* GetPosition(Vec3& out) = 0;                // [26] 0xD0  shape->GetPosition; (0,0,0) if no shape (0x1815B9D30)
    virtual bool ContainsPoint(const Vec3& p) = 0;           // [27] 0xD8  shape->ContainsPoint; false if no shape (0x180950770)
    virtual int32_t GetMarkType() = 0;                       // [28] 0xE0  m_markTypeOverride (C_POI+0x4C) if >= 0 else def->markType (0x18094EF7C)
    virtual int32_t GetDefMarkType() = 0;                    // [29] 0xE8  def ? def->markType : -1 (0x18094EF94)
    virtual void SetMarkTypeOverride(int32_t t) = 0;         // [30] 0xF0  C_POI+0x4C (0x181A72920)
    // [31] 0xF8  enumerate the def row's linked 16-byte ids (same global-multimap range pattern as
    //      I_Location [8]) into the collector [map identity UNVERIFIED]
    virtual void EnumerateDefLinkedIds(std::function<void(const S_LocationId&)>& fn) = 0;  // 0x180B5FBA0
    virtual void CollectDefLinkedIds(void* outVec) = 0;      // [32] 0x100  same range -> 16B-elem vector @out+0x90 (0x180B5FC54)
    virtual const char* GetUILabelRaw() = 0;                 // [33] 0x108  m_uiLabel (C_POI+0x60) or fallback [34] (0x180C4CD54)
    // [34] 0x110  fallback label: fast-travel => owning location's name, else def->label,
    //      else "<Missing Label>"
    virtual const char* GetUILabel() = 0;                    // 0x180C4CD6C
    // [35] 0x118  visibility recheck (manager suppress count, def availability, global exclusion
    //      multimap unk_18532D6A0 keyed by typeId) [semantics partially traced]
    virtual bool _vf35_shouldShowOnMap() = 0;                // 0x180C4C9C4
    virtual const char* GetFastTravelConfirmText() = 0;      // [36] 0x120  m_ftConfirmText (C_POI+0x68) or "ui_dlg_fasttravel_confirm" (0x182CC7D3C)
    virtual int32_t GetDefPriority() = 0;                    // [37] 0x128  def ? def+0x30 : INT_MAX [field name INFERRED] (0x1815B9D4C)
    // [38] 0x130  gated discover/undiscover: blocked while manager IsSuspended or (unless force)
    //      when !discoverable or unloaded; discover: state 0 -> 1, undiscover: state < 2 -> 2 [sic --
    //      transition semantics kept verbatim from the binary; naming NOT settled]
    virtual bool SetDiscoveredGated(bool discover, bool force) = 0;  // 0x18095080C
    virtual float GetRadius() = 0;                           // [39] 0x138  shape->GetRadius2; 0 if no shape (0x1814F5050)
    // [40] 0x140  restore initial state: current {flags,state} (C_POI+0x98) = initial {flags,state}
    //      (C_POI+0x90), preserving the IsUnloaded bit (manager Reset calls this on every POI)
    virtual uint16_t Reset() = 0;                            // 0x180567984
    // [41] 0x148  tree-serialize: "id"/"typeId"/flag booleans/"shape"/"mapCategory"("POI/<label>")/
    //      "locationId"/"location"/"initialState"/"state"/"layerName"/"markType"/"position"/"uiLabel"
    virtual void Serialize(void* writer) = 0;                // 0x182CC8E28
};
static_assert(sizeof(I_POI) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::rpgmodule
