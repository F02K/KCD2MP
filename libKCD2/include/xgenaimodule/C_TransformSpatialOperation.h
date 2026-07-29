#pragma once
#include "I_SpatialOperation.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::xgenaimodule::C_TransformSpatialOperation : I_SpatialOperation --
// combined rotation+position operation (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof
// 0x30 (ctor sub_1807FE104: operator new(48)).
// -----------------------------------------------
// RTTI TD rva 0x4FFB8D0, COL 0x40CD170.  Vtable rva 0x3A55E48 (2 slots):
// [0] Apply = sub_1810D6F54 (pos+quat via target sub-iface vtbl[44]/+352 then
// vtbl[45]/+360, flag 0x200000), [1] deleting dtor sub_1807E3034 (OWN copy --
// distinct 0x30 size).  The DOF-camera string table following the vtable in
// .rdata is unrelated data, not slots.

namespace wh::xgenaimodule {

class C_TransformSpatialOperation : public I_SpatialOperation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TransformSpatialOperation;
    void Apply() override;                     // [0] sub_1810D6F54
    ~C_TransformSpatialOperation() override;   // [1] sub_1807E3034

    Quat    m_rotation;   // +0x10  (16 bytes)
    Vec3    m_position;   // +0x20
    uint8_t _pad2C[4];    // +0x2C
};
static_assert(sizeof(C_TransformSpatialOperation) == 0x30, "must be 0x30 (new 48 at sub_1807FE104)");

}  // namespace wh::xgenaimodule
