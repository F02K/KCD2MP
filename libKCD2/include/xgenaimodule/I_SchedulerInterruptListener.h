#pragma once

// -----------------------------------------------
// wh::xgenaimodule::scheduler::I_SchedulerInterruptListener -- scheduler
// interrupt listener interface; primary MI base of C_SituationSubsystem (@+0x0)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SchedulerInterruptListener@scheduler@xgenaimodule@wh@@.
// No standalone vtable emitted (embedded-only); shape (4 slots) read from the
// C_SituationSubsystem primary vtable rva 0x3A83180: [0]sub_1819EC168
// [1]sub_1819EC168 (same body, ICF-folded) [2]sub_180BD6B74 [3]sub_180BD6B30.
// Slot semantics NOT decompiled [U] -- modeled as 4 pure slots (no dtor slot
// claimed [U]).

namespace wh::xgenaimodule::scheduler {

class I_SchedulerInterruptListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SchedulerInterruptListener;
    virtual void SilVf0() = 0;   // [0] [U role]
    virtual void SilVf1() = 0;   // [1] [U role]
    virtual void SilVf2() = 0;   // [2] [U role]
    virtual void SilVf3() = 0;   // [3] [U role]
};
static_assert(sizeof(I_SchedulerInterruptListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule::scheduler
