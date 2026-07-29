#pragma once
#include <cstdint>
#include <cstddef>
#include "I_Location.h"
#include "S_LocationId.h"
#include "../CryEngine/CryCommon/smartptr.h"

// ===========================================================================
// wh::rpgmodule::C_RPGLocation -- runtime RPG location object (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVC_RPGLocation@rpgmodule@wh@@ (TD 0x184D5C190).  sizeof 0x48 (PROVEN: operator new(72)
// in the table loader sub_180B604D4, which also inline-constructs every member).
// Bases (per RTTI base array):
//   [+0x00]  ::_reference_target<int>  CryEngine refcount base for _smart_ptr (vtable 0x183A7B300,
//            1 slot = scalar-deleting dtor sub_180B601A8; int refcount @+0x08)
//   [+0x10]  I_Location                interface (vtable 0x183A7B310, 28 slots -- see I_Location.h)
//
// One instance per row of the global location-definition table (88-byte rows @xmmword_18532D630);
// the loader (sub_180B604D4) news one per row, stores the row pointer at +0x18, and indexes the
// object in C_RPGLocationManager::m_locationsById (+0x1D8, by S_LocationId) and in the
// name-hash map (+0x228, by sub_18078B5F0(name)).
// Pointers handed to callers are the I_Location subobject (= this + 0x10).
// Abstract in this mirror (I_Location pure virtuals not overridden) -> never instantiated here.
// ===========================================================================

namespace wh::rpgmodule {

// Row of the location-definition table (0x58 stride). Only the fields the I_Location getters read
// are typed -- see I_Location.h's banner. Opaque here.
struct S_LocationDef;

class C_RPGLocation
    : public ::_reference_target<int>   // +0x00  vfptr + int refcount @+0x08
    , public I_Location                 // +0x10  vfptr only
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGLocation;
    const S_LocationDef* m_pDef;        // +0x18  = the table row (loader writes the iteration cursor)
    Vec3     m_positionOverride;        // +0x20  (0,0,0) => GetPosition derives from the smart-area AABB
    int32_t  m_insideCounter;           // +0x2C  actor-inside refcount (I_Location [21]/[22]); 0->1 / 1->0 notify listeners
    float    m_cachedReputation;        // +0x30  GetReputation cache (faction average)
    uint32_t _pad34;                    // +0x34
    int64_t  m_reputationTimestamp;     // +0x38  QueryPerformanceCounter stamp of the cache
    uint32_t m_state;                   // +0x40  0 = none, 2 = discovered
    uint32_t _pad44;                    // +0x44
};
static_assert(sizeof(C_RPGLocation) == 0x48, "C_RPGLocation must be 0x48 (operator new(72), sub_180B604D4)");
static_assert(offsetof(C_RPGLocation, m_pDef) == 0x18, "def pointer at 0x18");
static_assert(offsetof(C_RPGLocation, m_state) == 0x40, "state at 0x40");

}  // namespace wh::rpgmodule
