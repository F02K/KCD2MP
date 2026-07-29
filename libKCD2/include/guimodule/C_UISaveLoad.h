#pragma once
#include <cstdint>
#include <memory>
#include "guimodule/C_UIMenuBase.h"
#include "framework/C_ReadinessObserver.h"
#include "Offsets/vtables/ISystem.h"   // Offsets::ISystemEventListener

// -----------------------------------------------
// wh::guimodule::C_UISaveLoad -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x128.
// -----------------------------------------------
// RTTI .?AVC_UISaveLoad@guimodule@wh@@ (TD 0x184CD9A58). ctor sub_1805FF494 (in-place).
// Bases: C_UIMenuBase @+0x00 (binds the shared "Menu" movie), IUIElementEventListener
// @+0x10, ISystemEventListener @+0x58 (registered as "CUISaveLoad" in Init). vtables:
// primary 0x183EF5C60 ([1] Init 0x1807CF3AC, [2] Deinit 0x182BA7110, [3]
// OnModuleMessage 0x18085E15C, [4] Update = inherited C_UIFlashBase drain 0x180533878),
// system listener (OnSystemEvent 0x180FC489C).
// Lives EMBEDDED in C_UIMenu at +0xB0 (ctor call @0x1805ff429; a2 = owner backptr).
//
// The save/load + new-game controller: owns the playline/save-list menu pages
// (E_MenuPage 4..9), the save-request flow (savior schnapps / slot caps / overwrite
// page), the hardcore-mode confirmation, and the save/load CVar block.
// Init 0x1807CF3AC: base Init; caches a shared_ptr obtained from C_GUIModule+0x58
// into m_unkShared110 (sub_1807CFA58/sub_1807CF820); inbound flash dispatchers
// "OnPlaylineButton" -> 0x182BA8A68, "OnLoadButton" -> 0x182BA885C,
// "OnDeleteLoadButton" -> 0x182BA8158, "OnHCModeConfirm" -> 0x182BA867C.
// OnModuleMessage 0x18085E15C: msg 4 -> m_flag72 = 1; msg 5 -> drains pending save
// (m_flag70) or pending load (m_flag71, disables "no_input", force-drains the menu's
// flash queue via 0x18053378C(m_pMenu)), clears m_flag72; msg 0x3E -> if m_flag73 == 5
// sub_1823DD420(250); msg 0x50 -> 0x182BA8574(this, 0).
// OnSystemEvent 0x180FC489C: 10 (gameplay start) -> auto-load-last-save path through
// I_UIMenu when m_flag61 armed; 20/21 -> clear/set m_flag60..61; 28 -> set both +
// refresh; 36/37/68 -> save-list refresh helpers.
//
// [MODERATE vs KCD1] KCD1 C_UISaveLoad was a CUIManager-registered event system; KCD2
// demotes it to a value member of C_UIMenu.

namespace wh::guimodule {

class C_UIMenu;

class C_UISaveLoad
    : public C_UIMenuBase
    , public Offsets::ISystemEventListener
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UISaveLoad;

    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x1807CF3AC
    void Deinit() override;                     // [2] 0x182BA7110
    void OnModuleMessage(void* msg) override;   // [3] 0x18085E15C
    // ISystemEventListener @+0x58.
    void _vf0() override;                                                                    // [S0] dtor-region thunk
    void OnSystemEventAnyThread(uint32_t event, uint64_t wparam, uint64_t lparam) override;  // [S1]
    void OnSystemEvent(uint32_t event, uint64_t wparam, uint64_t lparam) override;           // [S2] 0x180FC489C

    // --- Native methods (RVA forwarders in src/guimodule/C_UISaveLoad.cpp) ---
    // Page builders (widget sets: analysis/ui_survey/menu_page_builders.md §2; each
    // runs m_pMenu->PreparePage(N) .. m_pMenu->ShowPage()).
    // Save-request flow: performs the save when a slot is free (savior-schnapps check
    // wh_sys_NoSavePotion, sub_180B41034), else builds the page-9 overwrite list
    // ("ui_you_have_to_overwrite_save"); failure popups via AddConfirmation 17/18.
    // Param roles beyond saveType/slot UNVERIFIED (a4/a5 gate the potion check and the
    // confirmation path at the observed call sites).
    void RequestSave(uint8_t saveType, uint32_t slot, bool a4, bool a5);   // 0x182BA938C  page 9
    void BuildLoadGamePage(int playline);                                  // 0x182BAA354  page 8
    // a2 role UNVERIFIED (skips straight to LoadGame when only one playline has saves).
    void BuildSelectPlaylineLoadPage(char a2);                             // 0x182BAA7D8  page 7
    void BuildSelectPlaylineNewPage();                                     // 0x182BAA8C0  page 4
    void BuildNewGameModePage();                                           // 0x182BAAC18  page 5 (Normal/Hardcore; skipped when wh_sys_GameModeSelecting == 0)
    void BuildNewGameModeDebugPage(void* pDebugCtx);                       // 0x181846FF0  page 6
    // Emit the five "AddPlaylineButton" flash rows ("ui_Playline %d" + Empty/Hardcore
    // suffixes). forLoad selects the load-page variant; a3 role UNVERIFIED.
    void AddPlaylineButtons(int saveType, char a3, bool forLoad);          // 0x182BAA55C

    // ISystemEventListener vptr @+0x58 (base).
    // Two state bytes driven by OnSystemEvent (20 clears both / 21 sets m_flag60 /
    // 28 sets both): m_flag60 gates the msg-68 refresh, m_flag61 arms the
    // auto-load-last-save path on gameplay start. Roles INFERRED from those handlers.
    uint8_t  m_flag60;                    // +0x60  ctor 0
    uint8_t  m_flag61;                    // +0x61  ctor 0
    bool     m_flag62;                    // +0x62  ctor 1 (role UNVERIFIED)
    uint8_t  _pad63[5];                   // +0x63
    void*    m_pCtx50;                    // +0x68  cached *(S_GameContext+0x50) (object identity UNVERIFIED)
    // Async save/load flow flags, consumed by OnModuleMessage 4/5/0x3E: m_flag70 =
    // save pending (writer 0x181F8AC90, posts module msg 69), m_flag71 = load pending
    // (writer 0x181F8AB20, which also sets m_flag73 = 4), m_flag72 = msg-4 latch,
    // m_flag73 = flow mode (== 5 checked on msg 0x3E).
    uint8_t  m_flag70;                    // +0x70
    uint8_t  m_flag71;                    // +0x71
    uint8_t  m_flag72;                    // +0x72
    uint8_t  m_flag73;                    // +0x73
    // CVar-backed ints (ctor registers via sub_1823D8168 with these addresses).
    int32_t  m_noPlaylineDeleting;        // +0x74  "wh_sys_NoPlaylineDeleting" (0)
    int32_t  m_noSavePotion;              // +0x78  "wh_sys_NoSavePotion" (0, VF_CHEAT)
    int32_t  m_showUsedModsInSaveTooltip; // +0x7C  "wh_mod_ShowUsedModsInSaveTooltip" (0)
    int32_t  m_showInfoInSaveTooltip;     // +0x80  "wh_sys_ShowInfoInSaveTooltip" (0)
    int32_t  m_showDlcInSaveTooltip;      // +0x84  "wh_sys_ShowDlcInSaveTooltip" (0)
    int32_t  m_loadGameFilter;            // +0x88  "wh_sys_LoadGameFilter" (-1; save-type bitmask)
    int32_t  m_autoLoadLastSave;          // +0x8C  "wh_sys_AutoLoadLastSave" (0)
    int32_t  m_disableNewGameDebugFader;  // +0x90  "wh_ui_DisableNewGameDebugFader" (0, VF_CHEAT)
    int32_t  m_gameModeSelecting;         // +0x94  "wh_sys_GameModeSelecting" (1)
    // Stored by the flash "ShowHCMode" composite 0x181F8DBC0 (hardcore-mode perk
    // confirmation param; name INFERRED).
    CryStringT<char> m_hcModeParam;       // +0x98  ctor: empty
    wh::C_ReadinessObserver m_beforeNewGameObserver;   // +0xA0  "BeforeNewGame" readiness event
    std::shared_ptr<void> m_unkShared110; // +0x110  Init: derived from C_GUIModule+0x58 (pointee type UNVERIFIED)
    C_UIMenu* m_pMenu;                    // +0x120  owner backptr (ctor a2); receiver of PreparePage/ShowPage
};
static_assert(sizeof(C_UISaveLoad) == 0x128, "C_UISaveLoad must be 0x128 (C_UIMenu +0xB0..+0x1D8)");
static_assert(offsetof(C_UISaveLoad, m_flag60) == 0x60, "state bytes at 0x60 (OnSystemEvent writers)");
static_assert(offsetof(C_UISaveLoad, m_pCtx50) == 0x68, "ctx cache at 0x68 (ctor 0x1805ff4ff)");
static_assert(offsetof(C_UISaveLoad, m_noPlaylineDeleting) == 0x74, "cvar ints from 0x74 (ctor registrations)");
static_assert(offsetof(C_UISaveLoad, m_hcModeParam) == 0x98, "HC-mode string at 0x98 (writer 0x181F8DBC0)");
static_assert(offsetof(C_UISaveLoad, m_beforeNewGameObserver) == 0xA0, "BeforeNewGame observer at 0xA0");
static_assert(offsetof(C_UISaveLoad, m_pMenu) == 0x120, "menu backptr at 0x120 (builders read *(this+288))");

}  // namespace wh::guimodule
