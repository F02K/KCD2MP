#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_ResourceHost -- resource-host interface; secondary MI base
// of C_SchedulerSubbrain (@+0xD0) (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_ResourceHost@xgenaimodule@wh@@.  Shape (4 slots) read from
// the Scheduler subobject vtable rva 0x3FD8550.  Slot semantics NOT decompiled
// [U] -- modeled as [0] dtor + 3 pure slots.

namespace wh::xgenaimodule {

class I_ResourceHost {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ResourceHost;
    virtual ~I_ResourceHost();   // [0] [U placement]
    virtual void RhVf1() = 0;     // [1] [U role]
    virtual void RhVf2() = 0;     // [2] [U role]
    virtual void RhVf3() = 0;     // [3] [U role]
};
static_assert(sizeof(I_ResourceHost) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
