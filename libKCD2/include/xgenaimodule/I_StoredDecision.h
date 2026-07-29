#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_StoredDecision -- stored-decision interface (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x08 (vptr only).
// -----------------------------------------------
// RTTI mangled .?AVI_StoredDecision@xgenaimodule@wh@@.  No standalone vtable
// observed; shape (8 slots) read from the sole audited implementor
// C_VariableDecision (vtable 0x183A2D640).  Slot roles UNRESOLVED beyond the
// dtor convention -- modeled as [0] dtor + 7 pure slots [U].

namespace wh::xgenaimodule {

class I_StoredDecision {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_StoredDecision;
    virtual ~I_StoredDecision();   // [0] [U placement]
    virtual void _vf1() = 0;       // [1] [U role]
    virtual void _vf2() = 0;       // [2] [U role]
    virtual void _vf3() = 0;       // [3] [U role]
    virtual void _vf4() = 0;       // [4] [U role]
    virtual void _vf5() = 0;       // [5] [U role]
    virtual void _vf6() = 0;       // [6] [U role]
    virtual void _vf7() = 0;       // [7] [U role]
};
static_assert(sizeof(I_StoredDecision) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
