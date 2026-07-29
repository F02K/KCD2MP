#pragma once
#include <cstdint>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "rttr/rttr_enable.h"
#include "framework/C_BaseModule.h"
#include "Offsets/vtables/ISystem.h"            // Offsets::ISystemEventListener
#include "Offsets/vtables/IServiceListener.h"   // Cry::GamePlatform::IServiceListener replica
#include "guimodule/C_ScriptBindUI.h"

// -----------------------------------------------
// wh::guimodule::C_GUIModule -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x120.
// -----------------------------------------------
// RTTI .?AVC_GUIModule@guimodule@wh@@ (TD 0x184CD1220). ctor sub_181789268 (alloc 0x120
// in module registrar sub_180C9DA44 @0x180c9df82); stored at S_GameContext+0xE8
// (v2[29]). Reachable from Lua/rttr via wh::shared::C_GameInterface::GetGUIModule
// (rttr property, registration sub_180E33674). GetModuleId = 16, GetModuleName =
// "GUIModule". vtables: primary 0x183EEB9C8 (10 slots), ISystemEventListener
// 0x183EEBA20 (3 slots), IServiceListener 0x183EEB798 (9 slots).
//
// ============================================================================
// [FUNDAMENTAL vs KCD1] What replaced CUIManager's element registry
// ============================================================================
// KCD1: wh::guimodule::CUIManager singleton (qword_182F77888) with a
// std::map<StringHash, IUIObject*> at +0x38; elements self-registered through static
// SAutoRegUIEventSystem<T> factories at DLL init.
//
// KCD2: CUIManager is GONE (no TD). The registry role moved onto the module itself:
//   * STORAGE  -- m_uiElements @+0x40: std::vector<std::shared_ptr<C_UIBase>>
//     (rttr property "UIElements", registration sub_18018AF40, member offset 64).
//   * POPULATION -- C_GUIModule::Init (slot [2], sub_180EE52B8) walks the rttr
//     derived-type list of the C_UIBase base type (type_data derived array via
//     type vf[+0xB8], range setup sub_1806A8DB0), invokes each type's registered
//     class_ctor/as_std_shared_ptr constructor_wrapper (creator invoke mode 5,
//     sub_1823C8F84 -> shared_ptr) and push_backs the result (sub_181F61560).
//     UI screens therefore "register" simply by being rttr-reflected subclasses of
//     C_UIBase (e.g. .?AV?$constructor_wrapper@VC_UIMenu...as_std_shared_ptr...)
//     -- no self-registration factories remain.
//   * NAME LOOKUP -- there is NO stored name map. The rttr read-only property
//     "UIElementsByName" (getter sub_182B8D8D8) builds a transient
//     std::map<std::string, std::shared_ptr<C_UIBase>> by iterating m_uiElements and
//     keying each element on elem->get_type() name (type_data+0x18 string_view).
//     Typed access inside the DLL iterates the vector comparing rttr types.
//   * LIFECYCLE -- module message 0 (sub_180C3CA14) broadcasts elem->Init(this)
//     (binds flash elements); OnModuleMessage/Update broadcast vf[3]/vf[4]; module
//     Deinit (slot [3], sub_182B8C864) broadcasts vf[2] then clears the vector.
// ============================================================================
//
// Other verified Init wiring (sub_180EE52B8): creates C_FaderController
// (sub_1818ACD14 -> shared_ptr @+0x58, rttr property "FaderController", offset 88),
// C_UIPlayer (0x98, ctor sub_1819B0BCC @+0x20), C_CutscenePlayer (0xD8, sub_1818AB4B8
// @+0x68), C_InteractiveSceneManager (0x108, sub_181925180(fader) @+0x70),
// C_UIFullUIModeHelper (0xD0, sub_18194AD80 @+0x78); registers the ISystemEventListener
// subobject with pSystem's event dispatcher under "CGUIModule" and the IServiceListener
// subobject with the GamePlatform service. The ctor itself builds C_UICommonEventHandler
// (0x8 @+0x28), C_UIActionHintManager (0x38, sub_181789544 @+0x30), C_UIBuffsManager
// (0x68, sub_1817894B8 @+0x38), C_UIQAMSelectionHandler (0x38, sub_1817895D4 @+0x88),
// a 0x48 zero-init POD (sub_181789674 @+0x80) and the m_viewMap tree (sub_181F76DE0
// @+0x108, filled by sub_181F5E790 from rttr derived types of a runtime type object,
// keyed by instance vf[1]() int id; dispatched by module message 55 via sub_181F784F0).

namespace wh::guimodule {

class C_UIBase;
class C_UICompass;
class C_FaderController;
class C_UIPlayer;                 // 0x98, ctor sub_1819B0BCC
class C_CutscenePlayer;           // 0xD8, ctor sub_1818AB4B8
class C_InteractiveSceneManager;  // 0x108, ctor sub_181925180
class C_UICommonEventHandler;
class C_UIActionHintManager;
class C_UIBuffsManager;
class C_UIFullUIModeHelper;
class C_UIQAMSelectionHandler;

class C_GUIModule
    : public wh::framework::C_BaseModule
    , public Offsets::ISystemEventListener
    , public Offsets::IServiceListener
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GUIModule;
    void OnModuleMessage(void* msg) override;      // [1] 0x18085DAF4  broadcasts to elements/buffs/UIPlayer, then id-switch (0=InitElements, 36=cutscene, 55=view map, ...)
    bool Init(void* ctx) override;                 // [2] 0x180EE52B8  builds everything (banner)
    void Deinit() override;                        // [3] 0x182B8C864  full teardown
    void Update() override;                        // [4] 0x180533814  elem->vf[4], UIPlayer tick sub_1805A0ABC, buffs->vf[4]
    int GetModuleId() const override;              // [5] 0x181A72890  = 16
    const char* GetModuleName() const override;    // [6] 0x181A71AA0  = "GUIModule"
    RTTR_ENABLE()  // [7..9]: get_type 0x182B92720, get_derived 0x182B92398; baseless (C_BaseModule chain not rttr-visible)

    // rttr-visible computed property "UIElementsByName" (getter sub_182B8D8D8) --
    // builds the name map on demand; see banner. Impl in src/guimodule/C_GUIModule.cpp.
    std::map<std::string, std::shared_ptr<C_UIBase>> GetUIElementsByName() const;

    // The live module instance, held by S_GameContext @+0xE8 (KCD1's CUIManager
    // singleton is gone -- see banner). Impl in src/guimodule/C_GUIModule.cpp.
    static C_GUIModule* GetInstance();

    // The compass screen out of m_uiElements (rttr type match, cached process-wide
    // in qword_18548B068; null until the UI is built). Impl in src/guimodule/C_GUIModule.cpp.
    C_UICompass* GetUICompass();

    // ISystemEventListener base occupies +0x10, IServiceListener base +0x18.
    C_UIPlayer*               m_pUIPlayer;             // +0x20  fade/letterbox + E_UITimerHit ticker (Init)
    C_UICommonEventHandler*   m_pCommonEventHandler;   // +0x28  per-element "CommonEventHandler" listener (ctor)
    C_UIActionHintManager*    m_pActionHintManager;    // +0x30  input-hint manager (ctor)
    C_UIBuffsManager*         m_pBuffsManager;         // +0x38  buff HUD feed (ctor)
    std::vector<std::shared_ptr<C_UIBase>> m_uiElements;         // +0x40  THE screen registry (rttr "UIElements")
    std::shared_ptr<C_FaderController>     m_pFaderController;   // +0x58  rttr "FaderController"
    C_CutscenePlayer*         m_pCutscenePlayer;       // +0x68  (Init)
    C_InteractiveSceneManager* m_pInteractiveSceneMgr; // +0x70  (Init, takes the fader)
    C_UIFullUIModeHelper*     m_pFullUIModeHelper;     // +0x78  (Init)
    void*                     m_pScratch80;            // +0x80  -> 0x48 heap map {byte UI-action-slot 0..18 -> int resolved input-action id}; msg-0 helper sub_181352700 fills it from action names (ui_modal_open/confirm/cancel, ui_apse_*, ui_inv_*, ui_menu_open) via operator[] sub_181352AA8 + id-resolver sub_181352C48; exact container class UNVERIFIED
    C_UIQAMSelectionHandler*  m_pQAMSelectionHandler;  // +0x88  quick-action-menu selection (ctor)
    void*                     m_pGame;                 // +0x90  live game object between module msgs 40/41: msg-40 handler sub_180EC8400 stores *(ctx+8)->vf[+0x200]() here + broadcasts elem->OnGameStart (vf[5]); msg-41 sub_180EC7B34 broadcasts OnGameEnd (vf[6]) then clears [resolved G3b]
    C_ScriptBindUI            m_scriptBindUI;          // +0x98  EMBEDDED Lua "UI" bind (ctor-inline init)
    std::map<int, void*>      m_viewMap;               // +0x108 int-id -> rttr-created handler object (sub_181F5E790; element type UNVERIFIED); msg-55 dispatch sub_181F784F0 calls value->vf[2]()
    bool                      m_flag118;               // +0x118 ctor 0; gates msg-78 branch in OnModuleMessage (role UNVERIFIED)
    uint8_t                   _pad119[7];              // +0x119
};
static_assert(sizeof(C_GUIModule) == 0x120, "C_GUIModule must be 0x120");
static_assert(offsetof(C_GUIModule, m_uiElements) == 0x40, "UIElements vector at 0x40 (rttr member offset 64)");
static_assert(offsetof(C_GUIModule, m_pFaderController) == 0x58, "FaderController at 0x58 (rttr member offset 88)");
static_assert(offsetof(C_GUIModule, m_scriptBindUI) == 0x98, "embedded C_ScriptBindUI at 0x98");
static_assert(offsetof(C_GUIModule, m_viewMap) == 0x108, "view map at 0x108");

}  // namespace wh::guimodule
