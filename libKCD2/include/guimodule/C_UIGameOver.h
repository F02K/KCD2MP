#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "playermodule/I_UIGameOver.h"

// -----------------------------------------------
// wh::guimodule::C_UIGameOver -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xA8.
// -----------------------------------------------
// RTTI .?AVC_UIGameOver@guimodule@wh@@ (TD 0x184CE1C38).
// Bases: C_UIFlashBase @+0x00 (COL 0x4134618), IUIElementEventListener @+0x10
// (COL 0x4134640), playermodule::I_UIGameOver @+0x58 (COL 0x4134668).
// vtables: primary 0x183C33AC0 (14; GetElementName [11] 0x181A8F4D0 = "GameOver" --
// own flash movie), listener 0x183C33A78 (8), game-over 0x183C33A48 (5). ctor
// sub_18188A614; rttr as_std_shared_ptr creator sub_18188A588 (alloc 0xA8).
//
// Death/game-over screen. Init [1] sub_180C3B9E4: base Init, registers the inbound
// dispatchers "OnPictureShown" -> {this, sub_182BBA1F4} and "OnPictureHided" ->
// {this, sub_182BBA1A0}, publishes the I_UIGameOver subobject into the playermodule
// object (*(*(ctx+0x128)+0x48)+0x08 = this+0x58) and binds the "game_over" action-map
// action "accept_game_over" to a dismiss lambda.
// Show [G0]: copies the payload (sub_1808E71DC -> +0x60), IUIElement::SetVisible(true)
// (vf+0xE0), flash "Show"(text, mode), starts a module fader op (module+0x58
// FaderController vf[+0x58], preset unk_18557F528) and keeps the returned handle at
// +0xA0. Hide [G1]: cancels the handle (vf[+0x10](0)) + sub_18088E91C teardown.

namespace wh::guimodule {

class C_FaderSuspend;   // 0x60 fade-op guard (see C_FaderSuspend.h)

class C_UIGameOver : public C_UIFlashBase, public wh::playermodule::I_UIGameOver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIGameOver;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;     // [1] 0x180C3B9E4
    const char* GetElementName() const override;  // [11] 0x181A8F4D0 = "GameOver"
    // I_UIGameOver overrides.
    void Show(const CryStringT<char>& text, int mode, void* pPayload) override;   // [G0] 0x182BBB120
    void Hide() override;                                                         // [G1] 0x182BB9A78

    uint8_t _payload60[0x38];   // +0x60  Show payload copy (sub_1808E71DC); layout UNVERIFIED, ctor-untouched
    void*   m_98;               // +0x98  active-payload ptr of the +0x60 small-buffer holder (sub_1808E71DC @0x1808e72d3; == &_payload60 when inline, else heap clone); read as guard in OnPictureShown 0x182BBA1F4; pointee type UNVERIFIED
    C_FaderSuspend* m_pFaderHandle;   // +0xA0  owned fade-op = C_FaderSuspend (verifier REFUTED the earlier C_BasicFader<C_FaderController> claim: Show factory C_FaderController vt[0] sub_180C09080 -> sub_180C09154 allocs 0x60 + installs C_FaderSuspend vftable 0x183A85A28); Show moves in @0x182bbb1e0, Hide cancels vf[+0x10] @0x182bb9a82, release vf[+8](1)
};
static_assert(sizeof(C_UIGameOver) == 0xA8, "C_UIGameOver must be 0xA8");
static_assert(offsetof(C_UIGameOver, m_pFaderHandle) == 0xA0, "fader handle at 0xA0");

}  // namespace wh::guimodule
