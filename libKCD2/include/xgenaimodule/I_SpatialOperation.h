#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SpatialOperation -- deferred spatial-transform operation
// applied to a target object (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10:
// vptr + m_pTarget.
// -----------------------------------------------
// Own vftable referenced by the deleting-dtor bodies (sub_1807E3060 resets
// *this = &I_SpatialOperation::vftable @0x1807E3070).  2 slots:
// [0] Apply -- per-class; reads m_pTarget (+0x8), fetches the movement
//     sub-interface via target->vtbl[3] and invokes a transform setter on it.
// [1] virtual deleting dtor (Position/Rotation share ICF-folded body
//     sub_1807E3060; Transform has its own sub_1807E3034).
// m_pTarget ownership by the BASE is high-confidence (all three Apply bodies
// read this+8 identically) but no standalone base ctor found [U].

namespace wh::xgenaimodule {

class I_SpatialOperation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SpatialOperation;
    virtual void Apply() = 0;        // [0] applies the transform to m_pTarget [U exact signature]
    virtual ~I_SpatialOperation();   // [1] deleting dtor

    void* m_pTarget;    // +0x08  target: vtbl[3](target) yields a movement iface, then iface->vtbl[39](transform@subclass+0x10,0,0,flag) applies it (Apply sub_1803CEEB8) [U pointee class]
};
static_assert(sizeof(I_SpatialOperation) == 0x10, "I_SpatialOperation: vptr + target ptr");

}  // namespace wh::xgenaimodule
