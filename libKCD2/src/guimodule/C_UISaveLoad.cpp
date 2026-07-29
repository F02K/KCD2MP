#include "guimodule/C_UISaveLoad.h"
#include "Offsets/Offsets.h"

// C_UISaveLoad forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// The builders run widget adds against `this` and PreparePage/ShowPage against
// m_pMenu (+0x120); the playline builder reads m_pMenu->m_currentPageId to pick the
// 4-arg vs 5-arg AddPlaylineButton flash shape. Maps:
// analysis/ui_survey/menu_page_builders.md §2.

namespace wh::guimodule {

void C_UISaveLoad::RequestSave(uint8_t saveType, uint32_t slot, bool a4, bool a5)
{
    // sub_182BA938C: performs the save when a slot is free (savior-schnapps check
    // unless wh_sys_NoSavePotion; failure popups via AddConfirmation 17/18); when the
    // type's slot cap (asc_1840ABE98[saveType]) is reached, builds the page-9
    // overwrite list ("AddLoadButton" rows, "ui_you_have_to_overwrite_save").
    // a4/a5 roles UNVERIFIED (potion-check / confirmation gates at observed sites).
    using Fn = void(__fastcall*)(C_UISaveLoad*, uint8_t, uint32_t, char, char);
    static REL::Relocation<Fn> fn{ REL::ID(92) };
    fn(this, saveType, slot, a4, a5);
}

void C_UISaveLoad::BuildLoadGamePage(int playline)
{
    // sub_182BAA354: page 8 -- one "AddLoadButton" per save of the playline
    // (list sub_180B41BDC, label sub_180821600, enable/tooltip sub_180B41500) + Back.
    using Fn = void(__fastcall*)(C_UISaveLoad*, int);
    static REL::Relocation<Fn> fn{ REL::ID(94) };
    fn(this, playline);
}

void C_UISaveLoad::BuildSelectPlaylineLoadPage(char a2)
{
    // sub_182BAA7D8: page 7 -- playline select feeding BuildLoadGamePage; jumps
    // straight to page 8 when exactly one playline has saves, or back to the root
    // (RebuildRootPage(LoadGame)) when none do. a2 role UNVERIFIED.
    using Fn = void(__fastcall*)(C_UISaveLoad*, char);
    static REL::Relocation<Fn> fn{ REL::ID(96) };
    fn(this, a2);
}

void C_UISaveLoad::BuildSelectPlaylineNewPage()
{
    // sub_182BAA8C0: page 4 -- AddPlaylineButtons(saveType, 0) + Back; tail-calls the
    // page-5 builder when mode selection is unavailable.
    using Fn = void(__fastcall*)(C_UISaveLoad*);
    static REL::Relocation<Fn> fn{ REL::ID(97) };
    fn(this);
}

void C_UISaveLoad::BuildNewGameModePage()
{
    // sub_182BAAC18: page 5 -- NormalMode(69)/HardcoreMode(70)/Back; starts mode 1
    // directly (sub_182BAA530) when m_gameModeSelecting == 0.
    using Fn = void(__fastcall*)(C_UISaveLoad*);
    static REL::Relocation<Fn> fn{ REL::ID(99) };
    fn(this);
}

void C_UISaveLoad::BuildNewGameModeDebugPage(void* pDebugCtx)
{
    // sub_181846FF0: page 6 -- same mode pair with the captured debug context
    // (starts via sub_181847120(this, 1, "", ctx) when selection is unavailable).
    using Fn = void(__fastcall*)(C_UISaveLoad*, void*);
    static REL::Relocation<Fn> fn{ REL::ID(60) };
    fn(this, pDebugCtx);
}

void C_UISaveLoad::AddPlaylineButtons(int saveType, char a3, bool forLoad)
{
    // sub_182BAA55C: five "AddPlaylineButton" flash rows ("ui_Playline %d" +
    // "ui_PlaylineEmpty"/"ui_PlaylineHardcore" suffixes); the 4-arg flash shape is
    // used when m_pMenu->m_currentPageId == SelectPlaylineNew. a3 role UNVERIFIED.
    using Fn = void(__fastcall*)(C_UISaveLoad*, int, char, bool);
    static REL::Relocation<Fn> fn{ REL::ID(95) };
    fn(this, saveType, a3, forLoad);
}

}  // namespace wh::guimodule
