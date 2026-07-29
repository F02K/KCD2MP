#pragma once

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptibleRefPoint -- perceivable reference point (a bare
// position a perceptor can notice / test LOS against).  KCD2 WHGame.dll 1.5.6,
// kd7u.  sizeof 0x18 (create-site proven twice: sub_1803EB888 alloc(24)
// @0x1803eba81 and the cylinder observation-point builder sub_180D45FBC).
// -----------------------------------------------
// RTTI TD rva 0x4F4C838; vtable 0x183A33368, 3 slots.  ROOT class (not part of
// the C_PerceptionObject hierarchy).  Derived: C_OrientedPerceptibleRefPoint
// (whose overrides prove the base slot roles: [1] half-space facing test with
// return-1 "visible from all sides" default; [2] orientation getter returning
// the static default &unk_18506DC48).

namespace wh::xgenaimodule {

class C_PerceptibleRefPoint {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptibleRefPoint;
    virtual ~C_PerceptibleRefPoint();            // [0] sub_1803ECBE0 [U -- dtor placement unverified]
    virtual bool IsFacingAway(const Vec3& dir);  // [1] sub_18041A6A0 -- base: return 1 (unoriented point is visible from all sides)
    virtual const Vec3* GetOrientation();        // [2] sub_1832AED04 -- base: returns static default Vec3 &unk_18506DC48

    Vec3 m_pos;   // +0x08  world position (builders @0x1803eba2a, @0x180d461aa)
};
static_assert(sizeof(C_PerceptibleRefPoint) == 0x18, "C_PerceptibleRefPoint must be 0x18 (alloc 24)");

}  // namespace wh::xgenaimodule
