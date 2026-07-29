#pragma once
#include <cstdint>
#include "guimodule/E_MarkType.h"

// -----------------------------------------------
// wh::guimodule::S_EntityMapMark -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI: only via std::_Ref_count_obj2<S_EntityMapMark> (TD 0x184C91500) -- the struct is
// non-polymorphic and always make_shared'd (alloc 0x30 = 0x10 control block + 0x20).
// Creator sub_180C4E2E8(shared_ptr* out, const uint32_t* type, C_LinkableObject* obj,
// const uint32_t* param): id from the global generator sub_1803A480C(&unk_185326320).
//
// A persistent "mark this entity on the map/compass" REQUEST record owned by C_UIMap
// (m_entityMarks vector @+0x5A0).  When its POI-filter group is visible, C_UIMap
// materializes a live C_CompassMark from it (sub_181F47470(&sp, &m_type, m_pObject,
// &m_param)), registers the mark with C_UICompass (cached-getter sub_180C4E098 +
// C_UICompass add sub_180C4D1E8) and keys it into m_activeMarks by m_id; hiding the
// filter tears the C_CompassMark down again (SetPoiMarkersVisible sub_182B02B34).
// C_ShowMapMarker (concept-graph trigger) holds a shared_ptr to the record it created.
//
// Field evidence: creator (id@+0, obj@+0x10) + dedup find sub_181F46B30 (type@+0x04,
// param@+0x08, obj@+0x10 compared) + re-register refcount bump sub_180C4DD5C
// (++m_useCount@+0x18).

namespace wh::xgenaimodule { class C_LinkableObject; }

namespace wh::guimodule {

struct S_EntityMapMark {
    int32_t  m_id;         // +0x00  unique mark id (global counter sub_1803A480C); flash key
    E_MarkType::Type m_type;  // +0x04  marker-category (C_ShowMapMarker: E_ConceptMarkerType remapped in via sub_180C4DC78)
    uint32_t m_param;      // +0x08  creation param (2 from C_ShowMapMarker + checkpoint path) [role UNVERIFIED]
    uint32_t _pad0C;       // +0x0C  creator-untouched
    wh::xgenaimodule::C_LinkableObject* m_pObject;  // +0x10  tracked entity/object
    int32_t  m_useCount;   // +0x18  duplicate-registration refcount (sub_180C4DD5C)
    uint32_t _pad1C;       // +0x1C
};
static_assert(sizeof(S_EntityMapMark) == 0x20, "S_EntityMapMark must be 0x20");

}  // namespace wh::guimodule
