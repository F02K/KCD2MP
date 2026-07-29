#include "guimodule/C_UIHudMask.h"
#include "Offsets/Offsets.h"

// C_UIHudMask tool surface -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// KCD1 PORT NOTE: this file replaces KCD1's C_UIEHud.cpp. KCD1's HUD root
// (wh::guimodule::uielement::C_UIEHud, global qword_183785948, REL::ID(879))
// held a per-element hide-REFCOUNT int[24] that cheats could pin (RatioStrips =
// bit 20 for the letterbox). KCD2 has NO refcount: visibility re-derives from
// the framework UI-source monitor through C_UIHudMask's rule table on every
// source change, so one-shot pokes are undone by the next refresh -- persistent
// mods must hook the refresh path (RefreshAll / UpdateRatioPreset below) or
// rewrite m_elementRules. Recipes in the header banner.

namespace wh::guimodule {

C_UIHudMask* C_UIHudMask::GetInstance()
{
    // ctor sub_1817944DC publishes `this` at qword_18548BFA8; the dtor clears it.
    // Rebuilt with the UI (rttr creator sub_1817943BC), NOT process-lifetime.
    static REL::Relocation<C_UIHudMask**> p{ REL::ID(2354) };
    return *p;
}

bool C_UIHudMask::ElementNameToId(const char* name, wh::framework::E_HudElements::Type& outId)
{
    // sub_180556410: strcmp chain over the 28 element names; false on no match.
    using Fn = bool(__fastcall*)(const char*, wh::framework::E_HudElements::Type*);
    static REL::Relocation<Fn> fn{ REL::ID(11) };
    return fn(name, &outId);
}

const char* C_UIHudMask::ElementIdToName(wh::framework::E_HudElements::Type id)
{
    // sub_180555AC0: switch over ids 0..27; "<invalid enum value>" otherwise.
    using Fn = const char*(__fastcall*)(uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(9) };
    return fn(static_cast<uint8_t>(id));
}

void C_UIHudMask::RefreshAll()
{
    // sub_180C3BE68 (module msg 52 / listener [2] thunk 0x180C3BE5C): re-applies
    // wh_ui_ShowHud (m_pShowHudCVar->GetIVal -> whole "hud" IUIElement::SetVisible
    // vf+0xE0), re-evaluates all 28 element rules against the source monitor
    // (iterator sub_180C3BF10, lambda 0x181F7C940) and tail-calls the ratio-preset
    // selector. Undoes any direct m_visibleBits/m_ratioPreset pokes.
    using Fn = void(__fastcall*)(C_UIHudMask*);
    static REL::Relocation<Fn> fn{ REL::ID(29) };
    fn(this);
}

void C_UIHudMask::UpdateRatioPreset()
{
    // sub_1805561F0: active sources -> preset (4/6/20/13/14 -> 4/1/5/2/3, else 0);
    // on m_ratioPreset (+0x318) change flash-calls "SetRatioPreset"(int), then
    // always "SetDialogShiftMovieClips"(bool) for sources 14/8. Force-no-letterbox
    // hooks land here.
    using Fn = void(__fastcall*)(C_UIHudMask*);
    static REL::Relocation<Fn> fn{ REL::ID(10) };
    fn(this);
}

void C_UIHudMask::PushElementVisibility(wh::framework::E_HudElements::Type id)
{
    // sub_180555A10: queries the element's current visibility through the
    // I_UIHudMask subobject (+0x58), then sets the equally-named "hud" movie
    // variable (name via ElementIdToName, IFlashVariableObject path
    // sub_180555E48) and runs the per-element epilogue sub_180555C4C
    // (role UNVERIFIED). No-op when the flash element (+0x48) is unbound.
    using Fn = void(__fastcall*)(C_UIHudMask*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(8) };
    fn(this, static_cast<uint8_t>(id));
}

}  // namespace wh::guimodule
