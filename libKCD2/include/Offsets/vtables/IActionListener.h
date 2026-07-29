#pragma once
#include <cstdint>

// -----------------------------------------------
// IActionListener -- KCD2 binary vtable order (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. RTTI: .?AUIActionListener@@ (global namespace; CryAction
// IActionMapManager.h). Action-map event sink (blocking listener registration path).
//
// Shape evidence: C_UIMenu's IActionListener vtable (subobject +0x60, COL 0x4381CE0,
// vtable 0x183EF5938) = 3 slots: [0] adjustor thunk sub_18213A704 into the class dtor,
// [1] sub_180557658 = the real OnAction body (reads *(ActionId*)a2 as const char*,
// forwards "SetInput"(action, activationMode) to flash), [2] nullsub_1 = AfterAction
// default. Matches the stock SDK declaration set {~dtor, OnAction, AfterAction} with
// the dtor emitted first.

namespace Offsets {

struct SActionId;   // CCryName-shaped: single const char* (C_UIMenu::OnAction reads *pActionId directly)

struct IActionListener {
    virtual ~IActionListener() = default;                                            // [0]
    virtual void OnAction(const SActionId& action, int activationMode, float value) = 0;  // [1]
    virtual void AfterAction();                                                      // [2] nullsub default
};
static_assert(sizeof(IActionListener) == 8);

}  // namespace Offsets
