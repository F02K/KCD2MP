#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_LinkableObjectHolder -- holder-of-a-linkable interface
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (modeled polymorphic).
// -----------------------------------------------
// RTTI TD rva 0x4F5C468.  No COL/vtable of its own anywhere in the image (never
// most-derived, no emitted construction).  Slot count (7) read from the sole
// enumerated implementor vtable, C_LinkableObjectHolder (0x183A651A8) -- the
// interface/implementor slot SPLIT IS UNVERIFIED [U]: all 7 slots are modeled on
// the interface; some could be implementor-introduced.

namespace wh::xgenaimodule {

class I_LinkableObjectHolder {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_LinkableObjectHolder;
    virtual void _vf0() = 0;   // [0] [U role; dtor placement unverified]
    virtual void _vf1() = 0;   // [1] [U role]
    virtual void _vf2() = 0;   // [2] [U role]
    virtual void _vf3() = 0;   // [3] [U role]
    virtual void _vf4() = 0;   // [4] [U role]
    virtual void _vf5() = 0;   // [5] [U role]
    virtual void _vf6() = 0;   // [6] [U role]
};
static_assert(sizeof(I_LinkableObjectHolder) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
