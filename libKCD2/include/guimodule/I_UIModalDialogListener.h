#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::I_UIModalDialogListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 2 slots.
// -----------------------------------------------
// RTTI .?AVI_UIModalDialogListener@guimodule@wh@@ (TD 0x184C92B00). Modal-dialog
// open/close sink. NO own COL (never instantiated standalone); shape certified from the
// C_UIApse subobject @+0x20 (impl vtable 0x183ED9198: [0] 0x1811A11C0, [1] 0x1808C1954).
// Shape: exactly 2 slots, NO virtual dtor, NO rttr trio.
// Roles UNVERIFIED -- likely OnModalOpened/OnModalClosed (coined; order unproven).

namespace wh::guimodule {

class I_UIModalDialogListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIModalDialogListener;
    virtual void OnModalOpened() = 0;    // [0] name coined, role UNVERIFIED
    virtual void OnModalClosed() = 0;    // [1] name coined, role UNVERIFIED
};
static_assert(sizeof(I_UIModalDialogListener) == 8);

}  // namespace wh::guimodule
