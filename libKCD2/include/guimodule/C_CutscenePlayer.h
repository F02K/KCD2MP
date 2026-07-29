#pragma once
#include <cstdint>
#include "guimodule/I_InteractiveScenePlayer.h"
#include "guimodule/I_CutsceneChangeListener.h"
#include "framework/C_Signal.h"
#include "framework/C_Listeners.h"

// -----------------------------------------------
// wh::guimodule::C_CutscenePlayer -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB0AF8, COL 0x435FC10; vtable 0x183EE1F90 (9 slots -- NOT rttr-
// registered). Base: I_InteractiveScenePlayer@+0x00 (single). ctor sub_1818AB4B8;
// creator sub_1818AB444 (alloc 0xD8 @0x1818ab46f). Module-owned at C_GUIModule+0x68.
//
// The db-driven cutscene player: ctor registers console commands wh_ui_PlayCutscene
// (sub_182B366D4; autocomplete = C_CutsceneDatabase's IConsoleArgumentAutoComplete
// subobject, registration @0x1818ab5b5), wh_ui_StopCutscene (sub_181F58FE0) and
// wh_ui_ListCutscenes (sub_182B3659C). Notifies I_CutsceneChangeListener sinks on
// lifecycle changes. Impl slot addrs: [1] 0x182B388C0, [2] 0x182B37608, [3] 0x182B36DD4,
// [4] 0x182B38AD8, [5] 0x182B36B58, [6] 0x181A73320, [7] 0x182B38AA4, [8] 0x182B39094
// (roles UNVERIFIED).

namespace wh::guimodule {

class C_CutscenePlayer : public I_InteractiveScenePlayer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CutscenePlayer;
    void _vf1() override;   // [1] 0x182B388C0
    void _vf2() override;   // [2] 0x182B37608
    void _vf3() override;   // [3] 0x182B36DD4
    void _vf4() override;   // [4] 0x182B38AD8
    void _vf5() override;   // [5] 0x182B36B58
    void _vf6() override;   // [6] 0x181A73320
    void _vf7() override;   // [7] 0x182B38AA4
    void _vf8() override;   // [8] 0x182B39094

    wh::shared::C_Signal<> m_signal08;   // +0x08  delegate table unk_185669370 [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal18;   // +0x18  delegate table unk_185665430 [signature UNVERIFIED]
    uint8_t m_unk28[0x50];               // +0x28..+0x78  ctor-zeroed (qwords @+0x28..+0x40; 5-qword block @+0x48 -- container?; int @+0x70, byte @+0x74) [roles UNVERIFIED]
    wh::shared::C_Listeners<I_CutsceneChangeListener, 16> m_listeners;   // +0x78  (0x60; init sub_180A03994(+0x98,16,...))
};
static_assert(sizeof(C_CutscenePlayer) == 0xD8, "C_CutscenePlayer must be 0xD8 (creator sub_1818AB444)");
static_assert(offsetof(C_CutscenePlayer, m_listeners) == 0x78, "change-listener registry at 0x78");

}  // namespace wh::guimodule
