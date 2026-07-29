#include "guimodule/C_UIMenuBase.h"
#include "Offsets/Offsets.h"

// C_UIMenuBase widget-builder forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// Receiver evidence: the binary calls these with C_UIMenu (root builders via
// 0x180C04B38), C_UISettings (pages 11-16/20) and C_UISaveLoad (pages 4-9) receivers --
// all C_UIMenuBase-derived, all bound to the shared "Menu" element. The DB tier
// resolves rows from the byte-keyed sorted-vector tables (buttons 0x1853266E0,
// choice labels 0x185326760) and silently no-ops on a missing row.
//
// ABI note: parameters typed BY VALUE (std::function / CryStringT) are destroyed by
// the native callee (MSVC callee-destroy convention, dtor calls at each wrapper tail:
// e.g. 0x180558CCC ends with sub_1821386D0(onClick) + sub_1804FC624(tooltip)); the
// fn-pointer signatures below reproduce exactly that contract.

namespace wh::guimodule {

// ===== E_ButtonId-keyed (DB) tier =====

void C_UIMenuBase::AddBasicButton(E_ButtonId::Type id, std::function<void()> onClick,
                                  bool enabled, CryStringT<char> tooltipOverride)
{
    // sub_180558CCC: row = sub_180558E78(this, id); text sub_18039956C(row), tooltip =
    // override (if non-empty) else row+0x28; MOVE-ASSIGNS onClick into row+0x38
    // (sub_1808E71DC) -- the flash OnButton dispatch sub_180C07A98 invokes it; flash
    // ("AddBasicButton", name, row->m_containerIndex, text, tooltip, !enabled).
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, std::function<void()>, bool, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(12) };
    fn(this, id, std::move(onClick), enabled, std::move(tooltipOverride));
}

void C_UIMenuBase::AddValueButton(E_ButtonId::Type id, bool enabled, float value,
                                  float min, float max, CryStringT<char> tooltipOverride)
{
    // sub_180F6A3B8: DB row lookup, then the raw-tier wrapper 0x181F8BB00 with the
    // row's name/text/containerIndex. Float order (value, min, max) per that wrapper's
    // boxing into ("AddValueButton", id, container, text, value, min, max, tooltip,
    // !enabled); xmm-lossy decompilation at call sites -- order UNVERIFIED beyond it.
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, bool, float, float, float, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(50) };
    fn(this, id, enabled, value, min, max, std::move(tooltipOverride));
}

void C_UIMenuBase::AddChoicesButton(E_ButtonId::Type id, bool enabled,
                                    CryStringT<char> tooltipOverride)
{
    // sub_180F6C164: DB row lookup -> raw-tier wrapper 0x181F8B810. The choice list
    // starts empty; append entries with AddChoiceOption afterwards.
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, bool, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(54) };
    fn(this, id, enabled, std::move(tooltipOverride));
}

void C_UIMenuBase::AddChoiceOption(E_ButtonId::Type id, int choiceValue,
                                   E_ChoiceName::Type labelId, const CryStringT<char>& tooltip)
{
    // sub_181F8BA20: label row = sub_181569B00(this, labelId) from the menu_choices
    // table (no-op when labelId == 0 or missing), label text = row+0x18; forwards to
    // 0x181F8B8E0 with enabled = true. The a5 slot is dead in the native body.
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, int, uint8_t, int64_t, const CryStringT<char>*);
    static REL::Relocation<Fn> fn{ REL::ID(70) };
    fn(this, id, choiceValue, labelId, 0, &tooltip);
}

void C_UIMenuBase::AddChoiceOption(E_ButtonId::Type id, int choiceValue,
                                   CryStringT<char> label, bool enabled,
                                   const CryStringT<char>& tooltip)
{
    // sub_181F8B990: literal-label variant (builders use it for "SMAA 1X", "%ix%i"
    // resolutions, FOV degrees, "FSR %d.%d"); consumes label, forwards to 0x181F8B8E0.
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, int, CryStringT<char>, bool, const CryStringT<char>*);
    static REL::Relocation<Fn> fn{ REL::ID(69) };
    fn(this, id, choiceValue, std::move(label), enabled, &tooltip);
}

void C_UIMenuBase::SetValue(E_ButtonId::Type id, float value)
{
    // sub_180F69BA0: name + row containerIndex -> typed wrapper 0x181F8D9C0.
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, float);
    static REL::Relocation<Fn> fn{ REL::ID(46) };
    fn(this, id, value);
}

void C_UIMenuBase::SetChoice(E_ButtonId::Type id, int choiceValue)
{
    // sub_180F69AE4: name + row containerIndex -> typed wrapper 0x181F8D8D0.
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t, int);
    static REL::Relocation<Fn> fn{ REL::ID(44) };
    fn(this, id, choiceValue);
}

float C_UIMenuBase::GetValue(E_ButtonId::Type id)
{
    // sub_182BA7590: result-returning flash call ("GetValue", name, container) via
    // helper 0x182BA1E68; -1.0f when the element/call fails.
    using Fn = float(__fastcall*)(C_UIMenuBase*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(91) };
    return fn(this, id);
}

int C_UIMenuBase::GetChoice(E_ButtonId::Type id)
{
    // sub_182BA7518: result-returning flash call ("GetChoice", name, container) via
    // helper 0x182BA1DA4; -1 when the element/call fails.
    using Fn = int(__fastcall*)(C_UIMenuBase*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(90) };
    return fn(this, id);
}

void C_UIMenuBase::SelectButton(E_ButtonId::Type id)
{
    // sub_180C09B40: flash ("SelectButton", name, row containerIndex).
    using Fn = void(__fastcall*)(C_UIMenuBase*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(27) };
    fn(this, id);
}

// ===== string-id (raw) tier =====

void C_UIMenuBase::AddValueButton(const CryStringT<char>& id, const CryStringT<char>& text,
                                  bool enabled, float value, float min, float max,
                                  CryStringT<char> tooltip, int containerIndex)
{
    // sub_181F8BB00: flash ("AddValueButton", id, containerIndex, text, value, min,
    // max, tooltip, !enabled) via the 8-arg helper 0x181F822B0.
    using Fn = void(__fastcall*)(C_UIMenuBase*, const CryStringT<char>*, const CryStringT<char>*,
                                 bool, float, float, float, CryStringT<char>, int);
    static REL::Relocation<Fn> fn{ REL::ID(71) };
    fn(this, &id, &text, enabled, value, min, max, std::move(tooltip), containerIndex);
}

void C_UIMenuBase::AddChoicesButton(const CryStringT<char>& id, const CryStringT<char>& text,
                                    bool enabled, CryStringT<char> tooltip, int containerIndex)
{
    // sub_181F8B810: flash ("AddChoicesButton", id, containerIndex, text, tooltip,
    // !enabled) via the 5-arg helper 0x181F823B0.
    using Fn = void(__fastcall*)(C_UIMenuBase*, const CryStringT<char>*, const CryStringT<char>*,
                                 bool, CryStringT<char>, int);
    static REL::Relocation<Fn> fn{ REL::ID(67) };
    fn(this, &id, &text, enabled, std::move(tooltip), containerIndex);
}

void C_UIMenuBase::AddChoiceOption(const CryStringT<char>& id, int choiceValue,
                                   const CryStringT<char>& label, bool enabled,
                                   const CryStringT<char>& tooltip, int containerIndex)
{
    // sub_181F8B8E0: flash "AddChoiceOption" (contract alias "AddChoice") via the
    // 6-arg helper 0x181F81FD0 (choiceValue, id, !enabled, label, tooltip, container).
    using Fn = void(__fastcall*)(C_UIMenuBase*, const CryStringT<char>*, int, const CryStringT<char>*,
                                 bool, const CryStringT<char>*, int);
    static REL::Relocation<Fn> fn{ REL::ID(68) };
    fn(this, &id, choiceValue, &label, enabled, &tooltip, containerIndex);
}

void C_UIMenuBase::SetValue(const CryStringT<char>& id, float value, int containerIndex)
{
    // sub_181F8D9C0: flash ("SetValue", id, containerIndex, value).
    using Fn = void(__fastcall*)(C_UIMenuBase*, const CryStringT<char>*, float, int);
    static REL::Relocation<Fn> fn{ REL::ID(74) };
    fn(this, &id, value, containerIndex);
}

void C_UIMenuBase::SetChoice(const CryStringT<char>& id, int choiceValue, int containerIndex)
{
    // sub_181F8D8D0: flash ("SetChoice", id, containerIndex, choiceValue).
    using Fn = void(__fastcall*)(C_UIMenuBase*, const CryStringT<char>*, int, int);
    static REL::Relocation<Fn> fn{ REL::ID(72) };
    fn(this, &id, choiceValue, containerIndex);
}

void C_UIMenuBase::SetDisable(const CryStringT<char>& id, bool enabled,
                              const CryStringT<char>& tooltip, int containerIndex)
{
    // sub_181F8D930: flash ("SetDisable", id, containerIndex, tooltip, !enabled) --
    // used by the refresh fns for "ui_button_one_choice" lockouts.
    using Fn = void(__fastcall*)(C_UIMenuBase*, const CryStringT<char>*, bool, const CryStringT<char>*, int);
    static REL::Relocation<Fn> fn{ REL::ID(73) };
    fn(this, &id, enabled, &tooltip, containerIndex);
}

}  // namespace wh::guimodule
