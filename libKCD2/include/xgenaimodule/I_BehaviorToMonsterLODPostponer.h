#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::I_BehaviorToMonsterLODPostponer -- behavior->monster-LOD
// postponer subobject; 6th MI base of C_SchedulerSubbrain (@+0xF0) (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (subobject ctor sub_180413E6C writes
// vptr + two int16 = -1 at +0x08/+0x0A).
// -----------------------------------------------
// RTTI mangled .?AVI_BehaviorToMonsterLODPostponer@xgenaimodule@wh@@.  Its
// vtable is the .data symbol unk_18566BC68 (constructed at runtime; missed by
// the COL scan) -- slot count/bodies NOT enumerated [U]; modeled with a single
// dtor slot.

namespace wh::xgenaimodule {

class I_BehaviorToMonsterLODPostponer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_BehaviorToMonsterLODPostponer;
    virtual ~I_BehaviorToMonsterLODPostponer();   // [0] [U -- vtable unk_18566BC68 not enumerated]

    int16_t m_handleA;   // +0x08  ctor: -1 [U role]
    int16_t m_handleB;   // +0x0A  ctor: -1 [U role]
    uint8_t _pad0C[4];   // +0x0C
};
static_assert(sizeof(I_BehaviorToMonsterLODPostponer) == 0x10, "vptr + two int16 handles");

}  // namespace wh::xgenaimodule
