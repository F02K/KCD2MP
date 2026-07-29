#pragma once

// -----------------------------------------------
// wh::xgenaimodule::scheduler::I_SchedulerActivityHost -- scheduler activity-host
// interface; secondary MI base of C_SchedulerSubbrain (@+0xD8) (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SchedulerActivityHost@scheduler@xgenaimodule@wh@@.  Shape
// (3 slots) read from the Scheduler subobject vtable rva 0x3FD84C0.  Slot
// semantics NOT decompiled [U] -- modeled as [0] dtor + 2 pure slots.

namespace wh::xgenaimodule::scheduler {

class I_SchedulerActivityHost {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SchedulerActivityHost;
    virtual ~I_SchedulerActivityHost();   // [0] [U placement]
    virtual void SahVf1() = 0;              // [1] [U role]
    virtual void SahVf2() = 0;              // [2] [U role]
};
static_assert(sizeof(I_SchedulerActivityHost) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule::scheduler
