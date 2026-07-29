#pragma once
#include <cstdint>

// -----------------------------------------------
// Offsets::IWindowMessageHandler -- CryEngine window-message sink (KCD2 1.5.6, kd7u)
// -----------------------------------------------
// 3-slot interface (size 8). Single canonical definition; used as a secondary base in:
//   CSystem        @ +0x18  (crysystem/CSystem.h, via Offsets/vtables/ISystem.h)
//   C_GameStartup  @ +0x08  (game/C_GameStartup.h, vtable 0x183D18730)
// Slot identities VERIFIED from the C_GameStartup secondary vtable.

struct tagMSG;   // Win32 MSG (hwnd@0, message@8, wParam@0x10, lParam@0x18)

namespace Offsets {

struct IWindowMessageHandler {
    virtual void Dtor(char flags) = 0;                                  // [0]  0x18213E534  adjustor thunk -> owner dtor
    virtual void PreprocessMessage() = 0;                               // [1]  0x1803B6E80  empty (retn)  /* tentative name */
    virtual bool HandleMessage(const tagMSG* pMsg, long* pResult) = 0;  // [2]  0x180A587A4  WM_SETCURSOR/focus, writes *pResult  VERIFIED
};
static_assert(sizeof(IWindowMessageHandler) == 8);

}  // namespace Offsets
