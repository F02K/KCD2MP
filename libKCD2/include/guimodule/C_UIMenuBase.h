#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/E_ButtonId.h"
#include "guimodule/E_ChoiceName.h"

// -----------------------------------------------
// wh::guimodule::C_UIMenuBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58.
// -----------------------------------------------
// RTTI .?AVC_UIMenuBase@guimodule@wh@@ (TD 0x184CD9A88).
// Bases: C_UIFlashBase @+0x00 (COL 0x41353B0), IUIElementEventListener @+0x10
// (COL 0x41353D8). vtables: primary 0x183A815A0 (14 slots, no additions), listener
// 0x183A81070 (8 slots). ctor sub_180BC7F58 = C_UIFlashBase ctor + own vtables;
// adds NO members and NO virtuals.
//
// Menu-family flash base: pins the flash element to the shared "Menu" movie --
// GetElementName override [11] sub_181A8F230 returns the literal "Menu", so every
// C_UIMenuBase-derived screen (C_UIMenu and the menu leaves) binds to the same
// IUIElement. Init [1] sub_180EC781C is a pure thunk to C_UIFlashBase::Init
// (sub_180565D38); Deinit [2] sub_182BA6F40 extends the base unbind.
//
// The Menu.gfx widget-builder call surface lives here as non-virtual native methods
// (forwarders in src/guimodule/C_UIMenuBase.cpp): the binary calls them with C_UIMenu,
// C_UISettings AND C_UISaveLoad receivers alike -- they only use the C_UIFlashBase
// element binding plus the three byte-keyed menu DB tables (buttons 0x1853266E0,
// choice labels 0x185326760, pages 0x185326860). Two tiers:
//   - E_ButtonId-keyed ("DB") tier: resolves text/tooltip/containerIndex from the
//     menu_buttons row and SILENTLY NO-OPS when the id has no row (sub_180558E78
//     returns null). AddBasicButton also REGISTERS the click handler -- the flash
//     OnButton round-trip (fscommand -> sub_180C07A98 -> name->id sub_180C07AE4 ->
//     row -> invoke) fires the row's std::function.
//   - string-id ("raw") tier: sends arbitrary id strings straight to flash (no DB row
//     needed) -- the MCM-friendly surface; clicks on unknown ids still reach
//     C_UIFlashBase listeners but sub_180C07AE4 maps them to 0/no-op, so raw-tier
//     buttons need their own OnUIEvent handling.
// Page order: C_UIMenu::PreparePage -> Add*/Set* -> C_UIMenu::ShowPage.
//
// [FUNDAMENTAL vs KCD1] KCD1's menu spine was C_UIHUDElements/C_UIMenuEvents event
// systems on CUIManager; KCD2 folds "a screen living inside the Menu movie" into this
// flash-base specialization.

namespace wh::guimodule {

class C_UIMenuBase : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMenuBase;
    const char* GetElementName() const override;   // [11] 0x181A8F230  = "Menu"

    // --- Native menu-widget builders (RVA forwarders in src/guimodule/C_UIMenuBase.cpp) ---
    // NOTE on ownership: parameters taken BY VALUE below are destroyed by the native
    // callee (MSVC callee-destroy byval ABI, dtor calls visible at each wrapper tail);
    // const& parameters are only copied by the callee.

    // == E_ButtonId-keyed tier (menu_buttons row required; no-op without one) ==
    // Add a basic button AND register its click handler into the button row (+0x38).
    // Empty tooltipOverride -> row default. enabled is sent to flash as !disable.
    void AddBasicButton(E_ButtonId::Type id, std::function<void()> onClick,
                        bool enabled, CryStringT<char> tooltipOverride);                    // 0x180558CCC
    // Add a float slider. Float order (value, min, max) recovered from the raw-tier
    // wrapper's flash boxing (xmm args are decompiler-lossy at DB-tier call sites --
    // order UNVERIFIED beyond that mapping).
    void AddValueButton(E_ButtonId::Type id, bool enabled, float value, float min,
                        float max, CryStringT<char> tooltipOverride);                       // 0x180F6A3B8
    // Add an (initially empty) multi-choice button; append entries with AddChoiceOption.
    void AddChoicesButton(E_ButtonId::Type id, bool enabled,
                          CryStringT<char> tooltipOverride);                                // 0x180F6C164
    // Append a choice with a menu_choices (E_ChoiceName) label row; no-op when labelId
    // is 0 or has no row. Flash name is "AddChoiceOption" (contract alias "AddChoice").
    void AddChoiceOption(E_ButtonId::Type id, int choiceValue,
                         E_ChoiceName::Type labelId, const CryStringT<char>& tooltip);      // 0x181F8BA20
    // Append a choice with a literal label ("SMAA 1X", "%ix%i" resolutions, FOV numbers).
    void AddChoiceOption(E_ButtonId::Type id, int choiceValue, CryStringT<char> label,
                         bool enabled, const CryStringT<char>& tooltip);                    // 0x181F8B990
    void  SetValue(E_ButtonId::Type id, float value);                                      // 0x180F69BA0
    void  SetChoice(E_ButtonId::Type id, int choiceValue);                                  // 0x180F69AE4
    float GetValue(E_ButtonId::Type id);      // flash readback; -1.0f on failure          // 0x182BA7590
    int   GetChoice(E_ButtonId::Type id);     // flash readback; -1 on failure             // 0x182BA7518
    void  SelectButton(E_ButtonId::Type id);  // move flash focus                          // 0x180C09B40

    // == string-id ("raw") tier (no DB row; arbitrary ids) ==
    void AddValueButton(const CryStringT<char>& id, const CryStringT<char>& text,
                        bool enabled, float value, float min, float max,
                        CryStringT<char> tooltip, int containerIndex);                      // 0x181F8BB00
    void AddChoicesButton(const CryStringT<char>& id, const CryStringT<char>& text,
                          bool enabled, CryStringT<char> tooltip, int containerIndex);      // 0x181F8B810
    void AddChoiceOption(const CryStringT<char>& id, int choiceValue,
                         const CryStringT<char>& label, bool enabled,
                         const CryStringT<char>& tooltip, int containerIndex);              // 0x181F8B8E0
    void SetValue(const CryStringT<char>& id, float value, int containerIndex);            // 0x181F8D9C0
    void SetChoice(const CryStringT<char>& id, int choiceValue, int containerIndex);       // 0x181F8D8D0
    void SetDisable(const CryStringT<char>& id, bool enabled,
                    const CryStringT<char>& tooltip, int containerIndex);                   // 0x181F8D930
    // (Group headers: no dedicated wrapper -- the builders send "SetGroup"(groupId,
    // localizedLabel, -1) through the generic 3-arg helper sub_180F6A2C0; use
    // m_pElement->CallFunction for custom groups.)
};
static_assert(sizeof(C_UIMenuBase) == 0x58, "C_UIMenuBase must be 0x58");

}  // namespace wh::guimodule
