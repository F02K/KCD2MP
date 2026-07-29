#include "guimodule/C_UIModalDialogQuestion.h"
#include "Offsets/Offsets.h"
#include <utility>   // std::move

// C_UIModalDialogQuestion forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::guimodule {

void C_UIModalDialogQuestion::Open(CryStringT<char> question, void* delegate,
                                   std::function<void()> onConfirm, std::function<void()> onCancel,
                                   CryStringT<char> hintConfirm, CryStringT<char> hintCancel)
{
    // sub_1811A0D58: std::function move-assign onConfirm->m_onConfirm(+0x58) /
    // register the "action"/"close" inbound dispatchers, set m_flagE0, then
    // CallFunction("OpenQuestionDialog", question, "action", "close", hintConfirm, hintCancel).
    using Fn = void(__fastcall*)(C_UIModalDialogQuestion*, CryStringT<char>, void*,
                                 std::function<void()>, std::function<void()>,
                                 CryStringT<char>, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(58) };
    fn(this, std::move(question), delegate, std::move(onConfirm), std::move(onCancel),
       std::move(hintConfirm), std::move(hintCancel));
}

void C_UIModalDialogQuestion::Close()
{
    // sub_1808C1840: clear m_flagE0, CallFunction("CloseQuestionDialog"), set audio state
    // "modal_dialog_question"=0, un-dim (SetFaderState 8) and release the delegate(+0xD8).
    using Fn = void(__fastcall*)(C_UIModalDialogQuestion*);
    static REL::Relocation<Fn> fn{ REL::ID(25) };
    fn(this);
}

}  // namespace wh::guimodule
