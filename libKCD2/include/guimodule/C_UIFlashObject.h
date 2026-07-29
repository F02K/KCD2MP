#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashObject -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// RTTI .?AVC_UIFlashObject@guimodule@wh@@ (TD 0x184C92AC8, COL 0x413BAD0, no listed bases).
// vtable 0x183A55268 (6 slots).  ctor sub_1807EA8AC(this, const CryStringT& id,
// const CryStringT& a3); dtor sub_1807EA878.
//
// Base for objects that serialize themselves into flash CallFunction argument lists
// (SUIArguments) -- the "row/marker record" the AS side consumes.  Analyzed subobject:
// C_CompassMark @+0x10 (its id string is a monotonically increasing counter
// ++dword_18548B09C rendered to text; C_UICompass::Update collects each visible mark's
// C_UIFlashObject and sends the array to flash as "CompassMarkers").
//
// [vs KCD1] New class -- KCD1 marks pushed loose args; KCD2 wraps the flash-facing
// identity + arg serialization into this small polymorphic record.
//
// Slots:
//   [0] 0x182AFD914  scalar deleting dtor
//   [1] 0x1803C15FC  FillUIArgs(SUIArguments&): appends m_id and m_str10 (string tag 4,
//       writer sub_1803C2034); overridden by C_CompassMark (0x1805551D8: + type int,
//       screen floats, in-area bytes)
//   [2] 0x180556C4C  appends one default int arg and stamps args+0x28 = 255
//       [role UNVERIFIED -- alpha/color tail?]
//   [3..5] RTTR_ENABLE() trio (get_type 0x182B039CC / get_ptr 0x1805F5DA0 /
//       get_derived_info 0x182B038DC)

namespace wh::guimodule {

class C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashObject;
    virtual ~C_UIFlashObject();                              // [0] 0x182AFD914
    // Serialize this record into a flash argument pack (arg type = SUIArguments&).
    virtual void FillUIArgs(void* pArgs) const;              // [1] 0x1803C15FC
    virtual void _vf2(void* pArgs) const;                    // [2] 0x180556C4C  role UNVERIFIED
    RTTR_ENABLE()  // [3..5]: get_type 0x182B039CC, get_derived 0x182B038DC

    CryStringT<char> m_id;      // +0x08  flash-side object id (decimal string). The KEY flash
                                //        uses to create/update/remove the marker clip (fc_add/
                                //        remove/changeCompassMarkerState `id`, g_MarkersO dict).
                                //        C_CompassMark: fresh UNIQUE monotonic counter
                                //        str(++dword_18548B09C) (ctor sub_181F479C0).
                                //        C_UIFlashMapPoiMarker: instead str(S_EntityMapMark.m_id).
    CryStringT<char> m_str10;   // +0x10  second serialized string (empty for marks; role UNVERIFIED)
};
static_assert(sizeof(C_UIFlashObject) == 0x18, "C_UIFlashObject must be 0x18");

}  // namespace wh::guimodule
