#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartEntitySubbrainListener -- smart-entity subbrain event
// listener; secondary MI base of C_SmartEntity (@+0xA8) and C_SchedulerSubbrain
// (@+0xE0) (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartEntitySubbrainListener@xgenaimodule@wh@@.  Shape
// (3 slots) read from the C_SmartEntity subobject vtable rva 0x4013F98
// ([0]0xCD4CD0 [1]0x12D1778 [2]0x12D174C) and Scheduler's rva 0x3FD84E0.
// Slot semantics NOT decompiled [U] -- modeled as 3 pure slots.

namespace wh::xgenaimodule {

class I_SmartEntitySubbrainListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartEntitySubbrainListener;
    virtual void SeslVf0() = 0;   // [0] [U role]
    virtual void SeslVf1() = 0;   // [1] [U role]
    virtual void SeslVf2() = 0;   // [2] [U role]
};
static_assert(sizeof(I_SmartEntitySubbrainListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
