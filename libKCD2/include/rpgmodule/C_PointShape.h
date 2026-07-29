#pragma once
#include <cstdint>
#include "I_POIShape.h"

// -----------------------------------------------
// wh::rpgmodule::C_PointShape -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18 (PROVEN: operator new(24)).
// -----------------------------------------------
// RTTI .?AVC_PointShape@rpgmodule@wh@@ (COL 0x184129BA0)  vtable 0x183A64908 (17 slots, all
// I_POIShape overrides -- slot semantics documented in I_POIShape.h).
// Point-with-radius POI shape. Constructed inline by the C_POI ctor (sub_18094FD64: memory-
// accounted alloc 24, position/radius from the AddPOI args). Member NAMES are the engine's own,
// from the property visitor (slot [14]): "m_Position" / "m_DiscoveryDistance".

namespace wh::rpgmodule {

class C_PointShape : public I_POIShape {   // +0x00  vfptr
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PointShape;
    Vec3  m_Position;           // +0x08
    float m_DiscoveryDistance;  // +0x14  the POI radius (GetRadius / all containment tests)
};
static_assert(sizeof(C_PointShape) == 0x18, "C_PointShape must be 0x18 (ctor operator new(24))");

}  // namespace wh::rpgmodule
