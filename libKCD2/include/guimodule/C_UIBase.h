#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::guimodule::C_UIBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI .?AVC_UIBase@guimodule@wh@@ (TD 0x184C92A68, single COL @+0x00, no bases).
// vtable 0x183ED93C0 (10 slots). shared_ptr-managed (std::_Ref_count<C_UIBase> TD exists).
//
// Root of the KCD2 UI-screen hierarchy. Every UI screen/manager the module owns by
// value in its screen list derives from this (C_UIFlashBase -> C_UIMenuBase -> ... and
// the non-flash C_UIBuffsManager). Concrete subclasses are registered with rttr
// (constructor_wrapper<T, class_ctor, as_std_shared_ptr> TDs) and are INSTANTIATED BY
// ENUMERATION in C_GUIModule::Init -- see C_GUIModule.h for the registry story.
//
// [FUNDAMENTAL vs KCD1] KCD1 had no common UI-object root with lifecycle virtuals; the
// element registry held IUIObject-ish pointers keyed by a StringHash map inside the
// CUIManager singleton, populated by SAutoRegUIEventSystem factories. KCD2 replaces all
// of that with this rttr-reflected shared_ptr class tree.
//
// Slot roles (all proven from C_GUIModule broadcast sites):
//   [0] scalar deleting dtor (sub_182AFD808)
//   [1] Init(C_GUIModule*)   -- base impl sub_1816DBC70 stores arg at +0x08; broadcast by
//                               module-message-0 handler sub_180C3CA14 (elem->vf[1](module))
//   [2] Deinit               -- nullsub in base; called per element by module Deinit
//                               sub_182B8C864 before the screen list is cleared
//   [3] OnModuleMessage(msg) -- nullsub in base; broadcast by C_GUIModule::OnModuleMessage
//                               sub_18085DAF4 (elem->vf[3](msg))
//   [4] Update               -- nullsub in base; broadcast by C_GUIModule::Update
//                               sub_180533814 (elem->vf[4]())
//   [5] OnGameStart(game)    -- nullsub in base; broadcast by the module-message-40
//                               handler sub_180EC8400 (elem->vf[5](pGame); pGame =
//                               *(ctx+8)->vf[+0x200](), gated on game->vf[+0x2A8](17),
//                               also cached at C_GUIModule+0x90). HUD overriders
//                               (C_UIHudHints/Messages/States/Adaptive) subscribe to
//                               game-object signals here. [names coined -- G3b]
//   [6] OnGameEnd(game)      -- nullsub in base; broadcast by the module-message-41
//                               handler sub_180EC7B34 (elem->vf[6](module+0x90), then
//                               clears the cached game ptr); overriders unsubscribe
//   [7..9] RTTR_ENABLE() trio: get_type (sub_182B039B0, magic-static rttr::type),
//          get_ptr (sub_1805F5DA0, `return this`), get_derived_info (sub_182B038BC,
//          returns {this, get_type()}). get_type is what C_GUIModule keys its
//          name->element map on (UIElementsByName getter sub_182B8D8D8 reads
//          type_data+0x18 std::string name).

namespace wh::guimodule {

class C_GUIModule;

class C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIBase;
    virtual ~C_UIBase();                                        // [0] 0x182AFD808
    virtual void Init(C_GUIModule* pModule);                    // [1] 0x1816DBC70  stores pModule at +0x08
    virtual void Deinit();                                      // [2] nullsub_1
    virtual void OnModuleMessage(void* msg);                    // [3] nullsub_1  (wh::framework::C_ModuleMessage&)
    virtual void Update();                                      // [4] nullsub_1
    virtual void OnGameStart(void* pGame);                      // [5] nullsub_1  broadcast on module msg 40 (sub_180EC8400); name coined
    virtual void OnGameEnd(void* pGame);                        // [6] nullsub_1  broadcast on module msg 41 (sub_180EC7B34); name coined
    RTTR_ENABLE()  // [7..9]: get_type 0x182B039B0, get_derived 0x182B038BC; the root base GUI screens list in their macros

    C_GUIModule* m_pModule;   // +0x08  owning module, set by Init (null until module message 0)
};
static_assert(sizeof(C_UIBase) == 0x10, "C_UIBase must be 0x10");

}  // namespace wh::guimodule
