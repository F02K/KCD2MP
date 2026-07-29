#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::I_UIItemContext -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 5 slots.
// -----------------------------------------------
// RTTI .?AVI_UIItemContext@guimodule@wh@@ (TD 0x184C92510). Item-context provider for
// the Apse inventory/combine UI. NO own COL (never instantiated standalone); shape
// certified from the C_UIInventoryBase subobject @+0x18 (impl vtable 0x183EDE980:
// [0] 0x1808F76E8, [1] 0x181A730F0, [2] 0x1808F7568, [3..4] 0x180838AE0 shared getter).
// C_UIModalDialogCombine implements it too (@+0x60, vtable 0x183EDE7E8).
// Shape: exactly 5 slots, NO virtual dtor, NO rttr trio.
// Method roles/signatures UNVERIFIED (coined _vfNN, void() until traced).

namespace wh::guimodule {

class I_UIItemContext {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIItemContext;
    virtual void _vf0() = 0;    // [0] role UNVERIFIED
    virtual void _vf1() = 0;    // [1] role UNVERIFIED
    virtual void _vf2() = 0;    // [2] role UNVERIFIED
    virtual void _vf3() = 0;    // [3] role UNVERIFIED (impl: shared getter 0x180838AE0)
    virtual void _vf4() = 0;    // [4] role UNVERIFIED (impl: shared getter 0x180838AE0)
};
static_assert(sizeof(I_UIItemContext) == 8);

}  // namespace wh::guimodule
