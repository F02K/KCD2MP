#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/C_UIModalDialogItem.h"
#include "framework/I_UIHudMessages.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudMessages -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x320.
// -----------------------------------------------
// RTTI .?AVC_UIHudMessages@guimodule@wh@@ (TD 0x184CCFB98).
// Bases: C_UIFlashBase @+0x00 (COL 0x4136080), IUIElementEventListener @+0x10
// (COL 0x41360A8), framework::I_UIHudMessages @+0x58 (COL 0x41360D0).
// vtables: primary 0x183D2C340 (14; GetElementName [11] 0x181A8E8F0 = "hud";
// [4] Update 0x180533C20 own), listener 0x183D2E4C8 (8), messages 0x183D2EA98 (7).
// ctor sub_180BC9680; rttr as_std_shared_ptr creator sub_1818AC318 (alloc 0x320).
//
// Info-text / tutorial-message HUD. EMBEDS a C_UIModalDialogItem @+0x60 bound to the
// "ApseModalDialog" flash element (name write @0x180bc96e9); the owner Init [1]
// sub_180EC7678 chains item->vf[1](module) (@0x180ec76a4), registers dispatchers
// (sub_180EC6E98), publishes the I_UIHudMessages subobject into the module object at
// *(sub_1823CE8E4()+0x158)+0x60 = this+0x58, and hooks module listeners.
// OnGameStart [5] sub_180EC8510 / OnGameEnd [6] sub_180EC7D3C: add/remove
// {this, sub_182B8E574} @game+0xA90, {this, sub_182B8E5C0} @game+0xAA0 and a delegate
// on the sub_1808DD160(game) object (vf[+0x70]/vf[+0x78]).
// The CryStringT array block is the per-category cached message/sound text store
// (GetMessageText [M2] indexes m_messageTexts; per-array roles UNVERIFIED).

namespace wh::guimodule {

class C_UIHudMessages : public C_UIFlashBase, public wh::framework::I_UIHudMessages {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudMessages;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x180EC7678
    void Update() override;                     // [4] 0x180533C20
    void OnGameStart(void* pGame) override;     // [5] 0x180EC8510
    void OnGameEnd(void* pGame) override;       // [6] 0x180EC7D3C
    // I_UIHudMessages overrides.
    void ShowInfoText(const CryStringT<char>& text, int a3, int a4) override;   // [M0] 0x1812C15EC
    void HideInfoText() override;                                               // [M1] 0x180C44E70
    CryStringT<char>* GetMessageText(uint8_t messageId) override;               // [M2] 0x181A8E8E0 -> &m_messageTexts[messageId]
    CryStringT<char>* GetMessageSound(uint16_t typeFlags) override;             // [M3] 0x182B8D9AC

    C_UIModalDialogItem m_modalDialog;    // +0x60  EMBEDDED, element "ApseModalDialog" (ctor-inline init)
    CryStringT<char> m_messageTexts[12];  // +0x110 per-message-id text cache ([M2] accessor)
    CryStringT<char> m_strings170[7];     // +0x170 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings1A8[9];     // +0x1A8 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings1F0[6];     // +0x1F0 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings220[7];     // +0x220 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings258[4];     // +0x258 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings278[4];     // +0x278 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings298[4];     // +0x298 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings2B8[9];     // +0x2B8 ctor empty (roles UNVERIFIED)
    CryStringT<char> m_strings300[4];     // +0x300 ctor empty (roles UNVERIFIED)
};
static_assert(sizeof(C_UIHudMessages) == 0x320, "C_UIHudMessages must be 0x320");
static_assert(offsetof(C_UIHudMessages, m_modalDialog) == 0x60, "modal dialog item at 0x60");
static_assert(offsetof(C_UIHudMessages, m_messageTexts) == 0x110, "message text cache at 0x110");

}  // namespace wh::guimodule
