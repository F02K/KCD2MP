#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_InjectionExternalDataUpdateListener -- injection
// external-data update listener; secondary MI base of C_SchedulerSubbrain
// (@+0xE8) (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_InjectionExternalDataUpdateListener@xgenaimodule@wh@@.
// Shape (1 slot) read from the Scheduler subobject vtable rva 0x3FD8578 (trailed
// by an assert string in .rdata).  Slot semantics NOT decompiled [U].

namespace wh::xgenaimodule {

class I_InjectionExternalDataUpdateListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InjectionExternalDataUpdateListener;
    virtual void OnExternalDataUpdate() = 0;   // [0] [U name/signature]
};
static_assert(sizeof(I_InjectionExternalDataUpdateListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
