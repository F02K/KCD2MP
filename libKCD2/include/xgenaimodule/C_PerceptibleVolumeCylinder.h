#pragma once
#include <cstdint>
#include "C_PerceptibleVolume.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptibleVolumeCylinder : C_PerceptibleVolume -- the
// cylinder-shaped concrete perceptible volume (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x118 (create-site proven: qword_18549D378(280) in all spawners
// sub_180D45244 / sub_180D4579C / sub_1832AA8A0).
// -----------------------------------------------
// RTTI TD rva 0x4F4C5E0; primary vtable 0x183A98730, 25 slots; I_RWLocked
// subobject vtable @+0x20 = 0x183A98710; IEntityEventListener subobject @+0x60,
// vtable 0x183A986F8 ([0] dtor thunk sub_18213D52C, [1] OnEntityEvent
// sub_1832AF6BC -- same body as the base).  Implements all four base purecalls
// and overrides Save/Load; slots [2..18] inherited spine values.
// ctor sub_180D45BCC(this, id, &pos, radius, height, a4, a5).
// Slot bodies: [20] GetBounds = center(pos) +/- radius (x,y) and height*0.5 (z)
// -> AABB @0x180d46655; [24] ContainsPoint2D = 2D dist(center,pt) - radius -
// margin <= 0 @0x181007ca1; [23] IntersectSegment2D = circle-arc half-plane
// test via point-on-circle helper sub_1832AED0C @0x1832aeda7; [19]
// CollectObservationPoints (sub_180D45EAC -> body sub_180D45FBC) samples
// 2*pi*r*h surface points, pushes 40-byte C_OrientedPerceptibleRefPoint
// {surface pos, outward normal} filtered by physics-raycast reachability
// sub_180D463FC (auto-pass when m_losCheckEnabled == 0).

namespace wh::xgenaimodule {

class C_PerceptibleVolumeCylinder : public C_PerceptibleVolume {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptibleVolumeCylinder;
    ~C_PerceptibleVolumeCylinder() override;   // [0]  deleting dtor sub_1811FBF0C (body sub_1811FBF50)
    uint32_t GetTypeMask() override;           // [1]  sub_1810DCB00 (returns dword_18533AEC0)
    void CollectObservationPoints(std::vector<C_PerceptibleRefPoint*>& out, void* ctx) override;  // [19] sub_180D45EAC -> sub_180D45FBC
    void GetBounds(float* outAABB6) override;                                                     // [20] sub_180D4664C
    void Save(void* ar) override;                                                                 // [21] sub_1832AFA84 -- radius+height, then base Save
    void Load(void* ar) override;                                                                 // [22] sub_1832AF168 -- radius+height, then base Load
    bool IntersectSegment2D(const Vec3* a, const Vec3* b, const Vec3* c, float margin) override;  // [23] sub_1832AED84
    bool ContainsPoint2D(const Vec3& pt, float margin) override;                                  // [24] sub_181007C90

    float   m_radius;   // +0x110  cylinder radius (ctor @0x180d45c1b)
    int32_t m_height;   // +0x114  cylinder height, stored as int (debug schema "height"; ctor @0x180d45c28)
};
static_assert(sizeof(C_PerceptibleVolumeCylinder) == 0x118, "C_PerceptibleVolumeCylinder must be 0x118 (alloc 280)");

}  // namespace wh::xgenaimodule
