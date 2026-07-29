#pragma once
#include "I_SpatialOperation.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::xgenaimodule::C_RotationSpatialOperation : I_SpatialOperation --
// rotation-set operation (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x20 (ctor
// sub_180953EBC: operator new(32)).
// -----------------------------------------------
// RTTI TD rva 0x4FFB880, COL 0x40CD148.  Vtable rva 0x3A64C40 (2 slots):
// [0] Apply = sub_181609348 (applies m_rotation via target sub-iface
// vtbl[41]/+328), [1] deleting dtor sub_1807E3060 (ICF-SHARED with
// C_PositionSpatialOperation).

namespace wh::xgenaimodule {

class C_RotationSpatialOperation : public I_SpatialOperation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RotationSpatialOperation;
    void Apply() override;                    // [0] sub_181609348
    ~C_RotationSpatialOperation() override;   // [1] sub_1807E3060 (shared)

    Quat m_rotation;   // +0x10  (16 bytes)
};
static_assert(sizeof(C_RotationSpatialOperation) == 0x20, "must be 0x20 (new 32 at sub_180953EBC)");

}  // namespace wh::xgenaimodule
