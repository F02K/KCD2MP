#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::I_POIShape -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_POIShape@rpgmodule@wh@@.  Spatial shape owned by C_POI (+0x70). Slots read from the
// C_PointShape implementation (vtable 0x183A64908, 17 slots); most are leaf math on the
// {position, discovery-distance} pair. Vec2/Vec3 from the PCH prelude (Cry_Math.h).
// Slot [0] is the scalar-deleting destructor (resets to I_POIShape::`vftable` before free).

namespace wh::rpgmodule {

class I_POIShape {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_POIShape;
    virtual ~I_POIShape() = 0;                             // [0]  0x00  scalar-deleting dtor (0x18094FF3C)
    virtual int  GetType() = 0;                            // [1]  0x08  C_PointShape returns 1 [enum values not enumerated]
    virtual Vec3* GetPosition(Vec3& out) = 0;              // [2]  0x10  copies +0x08
    virtual void SetPosition(const Vec3& pos) = 0;         // [3]  0x18
    virtual float GetRadius() = 0;                         // [4]  0x20  m_discoveryDistance (+0x14)
    virtual void GetAABB2D(Vec2& outMin, Vec2& outMax) = 0;  // [5] 0x28  pos.xy -/+ radius (0x18094F9AC)
    // [6]  0x30  (mult * radius)^2 >= dist3d(query.pos)^2  (query = {Vec3 pos, float multiplier})
    virtual bool IsWithinScaledRadius(const void* query) = 0;
    virtual bool ContainsPoint(const Vec3& p) = 0;         // [7]  0x38  dist3d^2 <= radius^2 (0x1809507A4)
    virtual bool ContainsPoint2D(const Vec2& p) = 0;       // [8]  0x40  xy-dist^2 <= radius^2
    // [9]  0x48  sphere.radius + radius > dist3d(sphere.center)  (sphere = {Vec3 c, float r})
    virtual bool IntersectsSphere(const void* sphere) = 0;
    virtual void _vf10(void* range) = 0;                   // [10] 0x50  segment/range test vs position (sub_182CC0460) [UNVERIFIED]
    virtual float GetRadius2() = 0;                        // [11] 0x58  duplicate of [4] in C_PointShape
    virtual void SaveTo(void* stream) = 0;                 // [12] 0x60  writes {int type=1, Vec3 pos, float radius}
    virtual void LoadFrom(void* stream) = 0;               // [13] 0x68  reads {Vec3 pos, float radius}
    virtual void VisitProperties(void* visitor) = 0;       // [14] 0x70  "m_Position", "m_DiscoveryDistance" (ENGINE member names)
    virtual void SerializeTree(void* writer, const char* nodeName) = 0;  // [15] 0x78  "type"="point", "position", "radius"
    virtual void _vf16() = 0;                              // [16] 0x80  shared ret stub in C_PointShape
};
static_assert(sizeof(I_POIShape) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::rpgmodule
