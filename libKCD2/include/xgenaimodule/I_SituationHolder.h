#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SituationHolder -- situation-holder interface; secondary
// MI base of C_SituationSubsystem (@+0x8) and C_SituationSubBrain (@+0xD0)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SituationHolder@xgenaimodule@wh@@.  Shape (9 slots) read
// from the subobject vtables (Subsystem rva 0x3A831A8: [0]sub_180638480 (SHARED
// with C_Situation slot [3]) [1]sub_181AA5AB0 [2]sub_1823C9770 [3]sub_180E9AD64
// [4]sub_180BD64A4 [5]sub_180BD5A8C [6]sub_180E9AD90 [7]sub_180760EF4
// [8]sub_180BD7938; SubBrain rva 0x3FD8020).  Slot semantics NOT decompiled [U]
// -- modeled as 9 pure slots (no dtor slot claimed [U]).

namespace wh::xgenaimodule {

class I_SituationHolder {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SituationHolder;
    virtual void ShVf0() = 0;   // [0] [U role]
    virtual void ShVf1() = 0;   // [1] [U role]
    virtual void ShVf2() = 0;   // [2] [U role]
    virtual void ShVf3() = 0;   // [3] [U role]
    virtual void ShVf4() = 0;   // [4] [U role]
    virtual void ShVf5() = 0;   // [5] [U role]
    virtual void ShVf6() = 0;   // [6] [U role]
    virtual void ShVf7() = 0;   // [7] [U role]
    virtual void ShVf8() = 0;   // [8] [U role]
};
static_assert(sizeof(I_SituationHolder) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
