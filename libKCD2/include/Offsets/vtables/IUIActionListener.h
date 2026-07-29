#pragma once

// -----------------------------------------------
// IUIActionListener -- KCD2 binary vtable order (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. SDK: CryEngine CryCommon/IFlashUI.h (struct IUIActionListener:
// OnStart(IUIAction*, SUIArguments&), OnEnd(IUIAction*, SUIArguments&), protected virtual
// ~dtor -- declared in that order, so MSVC emits {OnStart, OnEnd, dtor}). Mirrored here
// because the stock IFlashUI.h cannot be included in this build env.
//
// Shape evidence: wh::dialogmodule::C_BarberExtensionController's vtable 0x183E63460 has
// exactly 3 slots -- [0]/[1] UI-action callbacks, [2] nullsub (the dtor position; a nullsub
// there means the concrete dtor was folded/omitted -- do NOT delete through this interface).
// Mirrored for layout/slot count; args typed loosely (SUIArguments not ported).

struct IUIAction;

namespace Offsets {

struct IUIActionListener {
    virtual void OnStart(::IUIAction* pAction, const void* args) = 0;   // [0]
    virtual void OnEnd(::IUIAction* pAction, const void* args) = 0;     // [1]
    virtual ~IUIActionListener() = default;                             // [2] nullsub on C_BarberExtensionController
};
static_assert(sizeof(IUIActionListener) == 8);

}  // namespace Offsets
