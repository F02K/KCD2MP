#pragma once
#include "C_PerceptibleRefPoint.h"

// -----------------------------------------------
// wh::xgenaimodule::C_OrientedPerceptibleRefPoint : C_PerceptibleRefPoint --
// reference point with an outward orientation (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x28 (create-site proven: alloc(40) in the cylinder observation-point
// builder sub_180D45FBC @0x180d461aa, which writes pos +0x08..+0x13 and
// orientation +0x18..+0x23).
// -----------------------------------------------
// RTTI TD rva 0x4F4C6B0; vtable 0x183A98948, 3 slots (overrides only).
// m_orientation is a normalized outward-normal Vec3, NOT a quat (GetOrientation
// returns &m_orientation; IsFacingAway is a dot-product half-space test; the
// producer normalizes {cos t * r, sin t * r, 0}).

namespace wh::xgenaimodule {

class C_OrientedPerceptibleRefPoint : public C_PerceptibleRefPoint {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_OrientedPerceptibleRefPoint;
    ~C_OrientedPerceptibleRefPoint() override;            // [0] sub_1815D6144 (reverts to base vftable)
    bool IsFacingAway(const Vec3& dir) override;          // [1] sub_18115556C -- return dot(dir, m_orientation) < 0 (@0x181155578)
    const Vec3* GetOrientation() override;                // [2] sub_181A72EB0 -- return &m_orientation (@0x181a72eb4)

    Vec3 m_orientation;   // +0x18  normalized outward normal (producer @0x180d461e0)
};
static_assert(sizeof(C_OrientedPerceptibleRefPoint) == 0x28, "C_OrientedPerceptibleRefPoint must be 0x28 (alloc 40)");

}  // namespace wh::xgenaimodule
