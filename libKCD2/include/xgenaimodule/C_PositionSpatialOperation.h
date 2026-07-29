#pragma once
#include "I_SpatialOperation.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PositionSpatialOperation : I_SpatialOperation --
// position-set operation (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x20
// (ctor/factory sub_1808136C8: operator new(32)).
// -----------------------------------------------
// RTTI TD rva 0x4FFB960, COL 0x40CD120.  Vtable rva 0x3A56E08 (2 slots):
// [0] Apply = sub_1803CEEB8 (applies m_position via target sub-iface
// vtbl[39]/+312), [1] deleting dtor sub_1807E3060 (ICF-SHARED with
// C_RotationSpatialOperation -- both 0x20).

namespace wh::xgenaimodule {

class C_PositionSpatialOperation : public I_SpatialOperation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PositionSpatialOperation;
    void Apply() override;                    // [0] sub_1803CEEB8
    ~C_PositionSpatialOperation() override;   // [1] sub_1807E3060 (shared)

    Vec3    m_position;   // +0x10
    uint8_t _pad1C[4];    // +0x1C
};
static_assert(sizeof(C_PositionSpatialOperation) == 0x20, "must be 0x20 (new 32 at sub_1808136C8)");

}  // namespace wh::xgenaimodule
