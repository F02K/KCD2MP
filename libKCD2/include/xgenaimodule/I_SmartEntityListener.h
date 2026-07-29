#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartEntityListener -- smart-entity event listener;
// secondary MI base of C_SmartObjectSubBrain (@+0xD0) and (with
// I_SmartAreaListener) of C_SmartAreaSubBrain (@+0xD0) (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartEntityListener@xgenaimodule@wh@@.  Shape (10 slots)
// read from the C_SmartObjectSubBrain subobject vtable rva 0x3FD8208.  Slot
// semantics NOT decompiled [U] -- modeled as [0] dtor + 9 pure slots.

namespace wh::xgenaimodule {

class I_SmartEntityListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartEntityListener;
    virtual ~I_SmartEntityListener();   // [0] [U placement]
    virtual void SelVf1() = 0;            // [1] [U role]
    virtual void SelVf2() = 0;            // [2] [U role]
    virtual void SelVf3() = 0;            // [3] [U role]
    virtual void SelVf4() = 0;            // [4] [U role]
    virtual void SelVf5() = 0;            // [5] [U role]
    virtual void SelVf6() = 0;            // [6] [U role]
    virtual void SelVf7() = 0;            // [7] [U role]
    virtual void SelVf8() = 0;            // [8] [U role]
    virtual void SelVf9() = 0;            // [9] [U role]
};
static_assert(sizeof(I_SmartEntityListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
