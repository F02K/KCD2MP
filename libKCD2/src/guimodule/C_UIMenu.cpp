#include "guimodule/C_UIMenu.h"
#include "Offsets/Offsets.h"

// C_UIMenu page-flow forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// These require the C_UIMenu receiver (they touch m_state/m_currentPageId beyond the
// C_UIMenuBase surface). The root-menu OPENER is not here: it is the I_UIMenu virtual
// 0x180C04B38 (iface +0x58; sets m_state then dispatches a builder below) -- call it
// through the interface. Widget adders live on C_UIMenuBase.

namespace wh::guimodule {

void C_UIMenu::PreparePage(E_MenuPage::Type page)
{
    // sub_180F69448: m_currentPageId = page; menu_pages row sub_180F69558(this, page)
    // (silent no-op when missing); flash "ClearAll" -> "SetMenuColor"(m_state) ->
    // "PreparePage"(ContainerX, ContainerY, MaxButtons, header, ButtonHalfWidth).
    using Fn = void(__fastcall*)(C_UIMenu*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(43) };
    fn(this, page);
}

void C_UIMenu::ShowPage()
{
    // sub_181F8DB80: flash "ShowPage" -- finalize/display the prepared page.
    using Fn = void(__fastcall*)(C_UIMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(75) };
    fn(this);
}

void C_UIMenu::RebuildRootPage(E_ButtonId::Type selectAfter)
{
    // sub_180C0A5AC: re-runs the root builder matching m_state (1/4 -> RootMain,
    // 2 -> RootIngame, 3 -> RootPause) then SelectButton(selectAfter).
    using Fn = void(__fastcall*)(C_UIMenu*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(28) };
    fn(this, selectAfter);
}

void C_UIMenu::BuildRootMainPage()
{
    // sub_180F6820C: PreparePage(RootMain) + "ShowLoadingHint" + "ShowRootMenu" +
    // Continue/TryAgain(death)/NewGame/[debug]/LoadGame/Settings/HelpOverlays/DLC/
    // Credits/Exit + ShowPage. Death variant selected by m_state == 4.
    using Fn = void(__fastcall*)(C_UIMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(40) };
    fn(this);
}

void C_UIMenu::BuildRootIngamePage()
{
    // sub_1805598CC: the ESC pause menu (page 2) -- Resume/[debug]/SaveGame/LoadGame/
    // Settings/HelpOverlays/DLC/Credits/Exit/SaveExit/[SaveReset dbg] + ShowPage.
    // MCM injection point: add an AddBasicButton between this and ShowPage (or hook it).
    using Fn = void(__fastcall*)(C_UIMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(13) };
    fn(this);
}

void C_UIMenu::BuildRootPausePage()
{
    // sub_182BAA95C: restricted pause (page 3) -- same set with SaveGame/Settings/
    // SaveExit disabled ("ui_cant_save_other"/"ui_cant_change_settings").
    using Fn = void(__fastcall*)(C_UIMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(98) };
    fn(this);
}

void C_UIMenu::BuildSettingsHubPage(E_ButtonId::Type selectButton)
{
    // sub_180559C6C: page 10 -- GameSettings/GraphicSettings/AdvancedGraphics/
    // SoundSettings/Controls/[Keybinds kb+m]/[PhotoMode ingame]/Back, then
    // SelectButton(selectButton). The "leave page?" guards re-enter here.
    using Fn = void(__fastcall*)(C_UIMenu*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(14) };
    fn(this, selectButton);
}

void C_UIMenu::BuildHelpOverlaysPage()
{
    // sub_180821504: page 18 -- one "AddHelpOverlayButton" per help-overlay list entry.
    using Fn = void(__fastcall*)(C_UIMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(23) };
    fn(this);
}

void C_UIMenu::BuildDLCListPage()
{
    // sub_182BA9D18: page 19 -- "AddDLCButton" rows per IDLCManager entry + the
    // extras group (PROS/Rewards/Social).
    using Fn = void(__fastcall*)(C_UIMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(93) };
    fn(this);
}

}  // namespace wh::guimodule
