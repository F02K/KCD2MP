#pragma once
#include <cstdint>
#include <functional>
#include "S_LocationId.h"

// ===========================================================================
// wh::rpgmodule::I_Location -- the RPG "location" interface (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVI_Location@rpgmodule@wh@@ (TD 0x184D5F8C0).  PURE interface (sizeof 0x08); second base
// of C_RPGLocation (@+0x10) -- all data lives there. The "location" pointer handed out by
// C_RPGLocationManager is this subobject (C_RPGLocation + 0x10).
// Vtable (C_RPGLocation subobject): 0x183A7B310, 28 slots. NOT interfuscated (wh:: type).
// No virtual dtor -- destruction goes through the _reference_target<int> primary base.
//
// [FUNDAMENTAL vs KCD1] KCD1's 28 slots carried the crime state (GetWantedLevel / GetOwnWantedLevel
// / SetWantedLevel, E_CrimeIconLevel). KCD2 keeps the count at 28 but the wanted-level trio is GONE
// (crime state moved out of the location object); in their place sit an actor-inside refcount
// ([20]..[22]) and richer static-def getters. Keys are S_LocationId (16B), not WUID.
//
// "def" below = C_RPGLocation::m_pDef, a row in the global location-definition table
// (88-byte rows @xmmword_18532D630): {+0x08 S_LocationId id, +0x18 int32 numId, +0x20 char* name,
// +0x28 char* category, +0x30 q, +0x3C ptr-target, +0x4C int32 kind, +0x50 q} [row typing partial].
// ===========================================================================

namespace wh::rpgmodule {

class I_Location {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Location;
    // [0]  0x00  location id: def ? def->id : the invalid-id constant (xmmword_18409A0B0)
    virtual S_LocationId* GetId(S_LocationId& out) = 0;                 // 0x180467F14
    virtual const char* GetName() = 0;                                  // [1]  0x08  def+0x20 (0x181A93EB0)
    virtual uint32_t GetNumericId() = 0;                                // [2]  0x10  def+0x18 (0x1809AE1A0)
    // [3]  0x18  category row for def+0x18's numeric id via the global registry
    //      (qword_18532D5D8 array indexed by numId - dword_18532D5F0) -> row+8 (its name; feeds the
    //      "category" field of Serialize) [row semantics INFERRED]
    virtual const char* GetCategoryName() = 0;                          // 0x182CF4468
    // [4]  0x20  parse def+0x28: "Common"=0, "Unique"=1, "Hidden"=2 (default 2)
    virtual int GetCategoryEnum() = 0;                                  // 0x1807841B8
    virtual void* _vf5_defField30() = 0;                                // [5]  0x28  returns def+0x30 [UNVERIFIED] (0x180784A20)
    virtual const char* GetName2() = 0;                                 // [6]  0x30  same impl as [1] (0x181A93EB0)
    virtual void* _vf7_defField50() = 0;                                // [7]  0x38  returns def+0x50 [UNVERIFIED] (0x1809AE1B0)
    // [8]  0x40  enumerate the 16-byte ids associated with this id in a global multimap
    //      (sub_180B5F200 range over own GetId) into the collector [map identity UNVERIFIED]
    virtual void EnumerateLinkedIds(std::function<void(const S_LocationId&)>& fn) = 0;  // 0x180B5F0B4
    virtual void CollectLinkedIds(void* outVec) = 0;                    // [9]  0x48  same range -> 16B-elem vector @out+0x90 (0x180B5F160)
    virtual uint32_t GetState() = 0;                                    // [10] 0x50  C_RPGLocation+0x40 (0=none, 2=discovered)
    virtual void SetState(uint32_t state) = 0;                          // [11] 0x58  raw write, no notify (0x181A72950)
    virtual bool IsStateNone() = 0;                                     // [12] 0x60  GetState() == 0
    virtual bool IsDiscovered() = 0;                                    // [13] 0x68  GetState() == 2
    // [14] 0x70  position override (C_RPGLocation+0x20) if non-zero, else derived from the linked
    //      smart-area AABB via the location manager (center xy) (0x180467DEC)
    virtual Vec3* GetPosition(Vec3& out) = 0;
    virtual void SetPosition(const Vec3& pos) = 0;                      // [15] 0x78  writes C_RPGLocation+0x20 (0x181A93EE0)
    // [16] 0x80  average reputation over the member factions (C_FactionManager::ForEachFaction with
    //      an accumulating lambda); cached @C_RPGLocation+0x30 with a QPC timestamp @+0x38
    virtual float GetReputation(bool forceRecompute, bool arg2) = 0;    // 0x180D91BB8
    virtual void _vf17_recomputeReputation() = 0;                       // [17] 0x88  GetReputation(false, true) + global normalizer [UNVERIFIED] (0x180D11DB8)
    virtual bool GetFactionWanted() = 0;                                // [18] 0x90  any member faction flags the location (lambda fold) (0x182CF7E8C)
    // [19] 0x98  average of another per-faction float (same fold pattern as [16], no cache)
    virtual float _vf19_avgFactionValue() = 0;                          //            [semantic UNVERIFIED] (0x182CF7AA8)
    virtual bool IsOccupied() = 0;                                      // [20] 0xA0  inside-counter (C_RPGLocation+0x2C) > 0
    // [21] 0xA8  ++inside-counter; on 0->1 notifies every manager listener (slot [9] OnLocationOccupied)
    virtual void EnterActor() = 0;                                      // 0x180A1FA24  [name INFERRED]
    // [22] 0xB0  --inside-counter; on 1->0 notifies through the deferred walker (sub_181FB7520)
    virtual void LeaveActor() = 0;                                      // 0x180A1F6B0  [name INFERRED]
    virtual bool _vf23_isNumericId1or2() = 0;                           // [23] 0xB8  (u32)(GetNumericId()-1) <= 1 [semantic UNVERIFIED] (0x1809AE174)
    virtual bool MatchesKind(int kind) = 0;                             // [24] 0xC0  def+0x4C == kind || def+0x4C == -1 (0x180D10740)
    virtual void* _vf25_defField3C() = 0;                               // [25] 0xC8  returns &def+0x3C [UNVERIFIED] (0x181A93DF0)
    virtual void Reset() = 0;                                           // [26] 0xD0  state = 0, inside-counter = 0 (manager Reset calls this)
    // [27] 0xD8  tree-serialize: "id"/"name"/"category"/"state"/"position"/"reputation"/"mapCategory"
    //      ("Location/<category>") + "shape" (polygon points from the smart-area arg, else point)
    virtual void Serialize(void* writer, void* smartArea) = 0;          // 0x182CFCCC4
};
static_assert(sizeof(I_Location) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::rpgmodule
