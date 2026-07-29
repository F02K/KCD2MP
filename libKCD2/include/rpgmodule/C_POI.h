#pragma once
#include <cstdint>
#include <cstddef>
#include "I_POI.h"
#include "I_POIShape.h"
#include "S_LocationId.h"
#include "../CryEngine/CryCommon/smartptr.h"
#include "../CryEngine/CryCommon/CryString.h"

// ===========================================================================
// wh::rpgmodule::C_POI -- concrete POI implementation (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVC_POI@rpgmodule@wh@@ (TD 0x184D53FD8).  sizeof 0xA0 (PROVEN: operator new(0xA0) in
// I_RPGLocationManager::AddPOI, ctor sub_18094FD64(this, &id, &pos, radius)).
// Bases (per RTTI base array):
//   [+0x00]  ::_reference_target<int>  refcount for _smart_ptr (primary vtable 0x183A648D8,
//            5 slots: dtor + the 4 own virtuals below; int refcount @+0x08)
//   [+0x10]  I_POI                     interface (vtable 0x183A64998, 42 slots -- see I_POI.h)
// NOT an I_Location in KCD2 (KCD1's C_POI was handed out as I_Location*).
//
// Own virtuals appended to the primary vtable (C_POI-base `this`):
//   [1] 0x181A93980  SetLocationIdRaw(const S_LocationId&)  writes +0x28 (no map re-home --
//                    callers use I_RPGLocationManager::SetPOILocation for that)
//   [2] 0x1823D4A74  SetUILabel(string)              assign +0x60 (AddPOI arg 8)
//   [3] 0x182CC9360  SetFastTravelConfirmText(string) assign +0x68 (AddPOI arg 9)
//   [4] 0x181A939A0  SetMarkTypeOverrideRaw(int)      writes +0x4C
//
// Member offsets relative to the C_POI base (= I_POI* - 0x10). All ctor-verified except the
// +0x78 gap, which sub_18094FD64 leaves UNWRITTEN.
// ===========================================================================

namespace wh::rpgmodule {

class C_POI
    : public ::_reference_target<int>   // +0x00  vfptr + int refcount @+0x08
    , public I_POI                      // +0x10  vfptr only
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_POI;
    S_LocationId m_id;                  // +0x18  POI id (ctor arg; unordered_map key)
    S_LocationId m_locationId;          // +0x28  owning location (ctor: invalid; m_poisByLocation key)
    CryStringT<char> m_layerName;       // +0x38  serializer "layerName"
    uint64_t m_entityId;                // +0x40  I_POI Get/SetEntityId [id kind UNVERIFIED]
    uint32_t m_unk48;                   // +0x48  ctor 0; deserialized u32 from POI msg 0x4464 "dD" (read after the 16B id), writer 0x181FBF856
    int32_t  m_markTypeOverride;        // +0x4C  -1 = use the type-registry row's markType
    S_LocationId m_typeId;              // +0x50  POI-type registry key (serializer "typeId")
    CryStringT<char> m_uiLabel;         // +0x60  serializer "uiLabel" (fallback chain in I_POI [34])
    CryStringT<char> m_ftConfirmText;   // +0x68  default "ui_dlg_fasttravel_confirm"
    I_POIShape* m_pShape;               // +0x70  owned; C_PointShape (0x18) from the ctor args
    uint8_t  _gap78[0x18];              // +0x78  ctor-skipped, dtor-untouched (POD); no writer/reader in 47 vtbl slots, serializer, deser or debug -- reserved [UNRESOLVED]
    uint16_t m_initialFlags;            // +0x90  Reset() template } ctor zeroes the pair, then
    uint16_t _pad92;                    // +0x92                   } copies it over the current pair
    uint32_t m_initialState;            // +0x94  I_POI [6]/[7]
    uint16_t m_flags;                   // +0x98  bit table in I_POI.h (engine bit names)
    uint16_t _pad9A;                    // +0x9A
    uint32_t m_state;                   // +0x9C  0 = none, 2 = discovered
};
static_assert(sizeof(C_POI) == 0xA0, "C_POI must be 0xA0 (operator new(0xA0) in AddPOI)");
static_assert(offsetof(C_POI, m_id) == 0x18, "id at 0x18");
static_assert(offsetof(C_POI, m_typeId) == 0x50, "type id at 0x50");
static_assert(offsetof(C_POI, m_flags) == 0x98, "flags at 0x98");

}  // namespace wh::rpgmodule
