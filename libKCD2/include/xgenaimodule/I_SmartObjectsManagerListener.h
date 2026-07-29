#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SmartObjectsManagerListener -- smart-objects manager
// event listener; primary MI base of C_SmartEntityResolver (@+0x0) (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartObjectsManagerListener@xgenaimodule@wh@@.  Shape
// (4 slots) read from the C_SmartEntityResolver primary vtable rva 0x3A78C78:
// [0]0xA2A8D8 [1]0x1491830 [2]nullsub_1 [3]0x32D6B30 (deleting-dtor thunk in
// the resolver -- dtor placement at [3] [U]).  Slot semantics NOT decompiled
// [U].

namespace wh::xgenaimodule {

class I_SmartObjectsManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartObjectsManagerListener;
    virtual void SomlVf0() = 0;              // [0] [U role]
    virtual void SomlVf1() = 0;              // [1] [U role]
    virtual void SomlVf2() = 0;              // [2] nullsub in resolver [U role]
    virtual ~I_SmartObjectsManagerListener(); // [3] [U placement]
};
static_assert(sizeof(I_SmartObjectsManagerListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
