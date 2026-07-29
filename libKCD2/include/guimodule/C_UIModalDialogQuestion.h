#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogQuestion -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xE8.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogQuestion@guimodule@wh@@ (TD 0x184C9EA60).
// Bases: C_UIFlashBase @+0x00 (COL 0x413A798), IUIElementEventListener @+0x10
// (COL 0x413A7C0) -- plain flash screen, no extra bases.
// vtables: primary 0x183A81468 (14 slots; overrides [1] Init 0x180C41600),
// listener 0x183A81420 (8).
// ctor sub_180BC83D0(this, const char* szElementName).
//
// KCD2-NEW modal yes/no question dialog.  Embedded BY VALUE in C_UIMap at +0x378 bound to
// the "ApseModalDialog" flash element.  Init (slot [1], sub_180C41600) registers the flash
// events "OnQuestionDialogConfirmClicked"/"OnQuestionDialogCancelClicked" to fixed forwarders
// (sub_1819DC4B4/sub_1819DD750) that Close() the dialog then invoke m_onConfirm / m_onCancel;
// each Open() swaps in fresh callbacks.  The confirm/cancel role split (+0x58/+0x98), the
// open flag (+0xE0) and the delegate slot (+0xD8) are traced (kd7u).

namespace wh::guimodule {

class C_UIModalDialogQuestion : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogQuestion;

    // Show the dialog: moves onConfirm->m_onConfirm(+0x58) / onCancel->m_onCancel(+0x98),
    // stores delegate(+0xD8, nullable -- guarded by `if(delegate)`), sets m_isOpen, then
    // CallFunction("OpenQuestionDialog", question, "action", "close", hintConfirm, hintCancel).
    // All class-type args are by value (MSVC callee-destroys ABI).
    void Open(CryStringT<char> question, void* delegate,
              std::function<void()> onConfirm, std::function<void()> onCancel,
              CryStringT<char> hintConfirm, CryStringT<char> hintCancel);   // 0x1811A0D58
    // Clear m_isOpen, CallFunction("CloseQuestionDialog"), un-dim + release the delegate.
    void Close();                                                          // 0x1808C1840

    std::function<void()> m_onConfirm;   // +0x58  invoked on confirm (after Close) by sub_1819DC4B4
    std::function<void()> m_onCancel;    // +0x98  invoked on cancel  (after Close) by sub_1819DD750
    void*                 m_pDelegate;   // +0xD8  dim-delegate = an interface subobject of the owner screen (C_UIMap passes m_pApse+0x20, sub_182B00C15): Open sub_1811A0D58 stores it + vf[0](0) (dim), Close sub_1808C1840 calls vf[1](0) (undim) then zeroes; iface class UNRESOLVED
    bool                  m_isOpen;      // +0xE0  dialog-open flag: set by Open, cleared by Close; gates both forwarders
    uint8_t               _padE1[7];     // +0xE1
};
static_assert(sizeof(C_UIModalDialogQuestion) == 0xE8, "C_UIModalDialogQuestion must be 0xE8");
static_assert(offsetof(C_UIModalDialogQuestion, m_onCancel) == 0x98, "second callback at 0x98");

}  // namespace wh::guimodule
