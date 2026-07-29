#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_BehaviorHost -- behavior-host interface; secondary MI base
// of C_SituationSubBrain (@+0xC8) and C_SchedulerSubbrain (@+0xC8) (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_BehaviorHost@xgenaimodule@wh@@.  Shape (9 slots) read from
// the two subobject vtables (Situation rva 0x3FD81A0, Scheduler rva 0x3FD8500;
// slots [0][4][5] share bodies across the two).  Individual slot semantics NOT
// decompiled [U] -- modeled as [0] dtor + 8 pure slots.

namespace wh::xgenaimodule {

class I_BehaviorHost {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_BehaviorHost;
    virtual ~I_BehaviorHost();   // [0] [U placement]
    virtual void BhVf1() = 0;     // [1] [U role]
    virtual void BhVf2() = 0;     // [2] [U role]
    virtual void BhVf3() = 0;     // [3] [U role]
    virtual void BhVf4() = 0;     // [4] [U role]
    virtual void BhVf5() = 0;     // [5] [U role]
    virtual void BhVf6() = 0;     // [6] [U role]
    virtual void BhVf7() = 0;     // [7] [U role]
    virtual void BhVf8() = 0;     // [8] [U role]
};
static_assert(sizeof(I_BehaviorHost) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
