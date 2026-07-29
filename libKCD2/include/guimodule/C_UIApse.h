#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/I_UIModalDialogListener.h"
#include "guimodule/C_UIApseInventory.h"
#include "guimodule/C_UIApsePlayer.h"
#include "guimodule/C_UIQuestLog.h"
#include "guimodule/C_UIMap.h"
#include "guimodule/C_UIApseCodex.h"
#include "guimodule/C_UIApseCrafting.h"
#include "guimodule/C_UIApseCharacter.h"
#include "guimodule/C_UIApseCamera.h"
#include "guimodule/C_UIModalDialogInfo.h"
#include "framework/I_UIApse.h"
#include "framework/I_SourceMonitorListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIApse -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x2868 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI .?AVC_UIApse@guimodule@wh@@ (TD 0x184C92E30). ctor sub_18146C89C; creator
// sub_18146C7B8: sub_1804F75C0(0x2868) -> ctor -> rttr as_std_shared_ptr adopt -- the
// ONLY rttr-registered module screen of the Apse wave (lands in C_GUIModule::
// m_uiElements; every other Apse class is an embedded sub-panel).
// Cached singleton C_UIApse* qword_18548B050 (filled by the GetInstance forwarder
// sub_1805677CC -- the same getter C_UIMap.cpp's GetInstance uses, then +0x1508).
// Bases: C_UIBase @+0x00 + wh::framework::I_UIApse @+0x10 +
// wh::framework::I_SourceMonitorListener @+0x18 + I_UIModalDialogListener @+0x20.
// vtables: primary 0x183ED9140 (10, C_UIBase shape: [0] dtor 0x182AFD558, [1] Init
// 0x180BC51BC, [2] Deinit 0x182AFE174, [3] OnModuleMessage 0x18085E010, [4] Update
// 0x1805672D8) / I_UIApse 0x183ED91B0 / source-monitor 0x183ED9250 (OnSourceEvent
// 0x181828224) / modal-listener 0x183ED9198 ([0] 0x1811A11C0, [1] 0x1808C1954).
//
// The Apse: KCD2's map/menu/character hub screen. Its tabs are value-embedded
// sub-panels (table below); the flash traffic goes through their embedded widgets.
// [G4a] The old map_vertical.md claim "apse cam tick @+0x10C8" was WRONG -- +0x10C8 is
// the C_UIQuestLog embed (ctor-proven); the camera lives at +0x25C0 (apse.md §1).
//
// The four C_ModelProperty<Enum> members are kept as sized opaques: their 0x28 stride
// (vfptr + value @+0x08 + C_Signal @+0x10 + 8-byte tail) matches NEITHER existing
// C_ModelProperty specialization (0x30 owner-full / 0x20 owner-elided) -- the tail's
// identity (owner ptr vs default-value trait) is UNVERIFIED, as are the enumerator
// sets of E_UIApseView/E_UIApseState/E_UICharacterState.

struct ICVar;

namespace wh::guimodule {

class C_UIApse : public C_UIBase,
                 public wh::framework::I_UIApse,                 // +0x10
                 public wh::framework::I_SourceMonitorListener,  // +0x18
                 public I_UIModalDialogListener {                // +0x20
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApse;
    // I_UIApse impls (subobject vtable 0x183ED91B0; roles UNVERIFIED).
    void _vf1() override;    // [1]  0x181F4AA00
    void _vf2() override;    // [2]  0x180C44F48
    void _vf3() override;    // [3]  0x182AFEEB8
    void _vf4() override;    // [4]  0x181A73520
    void _vf5() override;    // [5]  0x180C42848
    void _vf6() override;    // [6]  0x180C42D28
    void SetApseView(wh::framework::E_UIApseView::Type view, char force, char flag) override;  // [7] 0x1819A6CF0
    void _vf8() override;    // [8]  0x182AFF600
    void _vf9() override;    // [9]  0x182AFF550
    void _vf10() override;   // [10] 0x182AFF6B0
    void _vf11() override;   // [11] 0x180B68E78
    void _vf12() override;   // [12] 0x1811A07D0
    void _vf13() override;   // [13] 0x182AFEA04
    void _vf14() override;   // [14] 0x1809E5B50
    void _vf15() override;   // [15] 0x182B009D8

    // I_SourceMonitorListener (subobject vtable 0x183ED9250).
    void OnSourceEvent(void* a2, bool bActive) override;   // 0x181828224

    // I_UIModalDialogListener (subobject vtable 0x183ED9198; roles UNVERIFIED).
    void OnModalOpened() override;    // [0] 0x1811A11C0
    void OnModalClosed() override;    // [1] 0x1808C1954

    uint8_t  m_viewProp[0x28];       // +0x28  C_ModelProperty<E_UIApseView> (vfptr 0x18146c8f9; value byte @+0x30; C_Signal {&unk_185669208} @+0x38) [see header note]
    uint8_t  m_viewProp2[0x28];      // +0x50  C_ModelProperty<E_UIApseView> (vfptr 0x18146c91f; signal @+0x60)
    uint8_t  m_stateProp[0x28];      // +0x78  C_ModelProperty<E_UIApseState::Type> (see E_UIApseState.h; vfptr 0x18146c936; value byte @+0x80; signal {&unk_1856691F8} @+0x88) -- the "apse view signal @+0x78" anchor
    uint8_t  m_charStateProp[0x28];  // +0xA0  C_ModelProperty<E_UICharacterState::Type> (see E_UICharacterState.h; vfptr 0x18146c95c; signal {&unk_1855A77F0} @+0xB0)
    ICVar*   m_pCVarDebugApse;       // +0xC8  "wh_ui_DebugApse" (registered sub_180B9512C @0x18146ca58)
    int32_t  m_unkD0;                // +0xD0  ctor 0 [role UNVERIFIED]
    uint8_t  _padD4[4];              // +0xD4
    C_UIApseInventory m_inventory;   // +0xD8    (0xAC8) ctor sub_180BC8ABC
    C_UIApsePlayer    m_player;      // +0xBA0   (0x528) ctor sub_180BC8C58
    C_UIQuestLog      m_questLog;    // +0x10C8  (0x440) ctor sub_18146D5B4 -- NOT a camera (apse.md §1)
    C_UIMap           m_map;         // +0x1508  (0x750) ctor sub_18146C3C4
    C_UIApseCodex     m_codex;       // +0x1C58  (0x228) ctor sub_18146CAC4(this, owner, &m_crafting)
    C_UIApseCrafting  m_crafting;    // +0x1E80  (0x2D8) ctor sub_18146CB5C
    C_UIApseCharacter m_character;   // +0x2158  (0x468) ctor sub_18146CDF8
    C_UIApseCamera    m_camera;      // +0x25C0  (0x1A8) ctor sub_18146D1E0
    int32_t  m_unk2768;              // +0x2768  ctor 0; read as boolean gate by sub_181E26910 (return *this && sub_181E26A40()!=0), called from OnModuleMessage sub_18085E010 on msg id 52 -> m_flag2818 [enable flag, role inferred; type unchanged, no 0/1 writer proven]
    uint8_t  _pad276C[4];            // +0x276C
    C_UIModalDialogInfo m_modalInfo; // +0x2770  (0xA8) ctor sub_180BC842C ("ApseModalDialog")
    bool     m_flag2818;             // +0x2818  ctor 0 [role UNVERIFIED]
    uint8_t  _pad2819[7];            // +0x2819
    uint64_t m_unk2820;              // +0x2820  pending focus-item position token; ctor = shared invalid sentinel qword_185326308 (=qword_185326300, sub_18016EF50); sub_1808EE0C4 (0x1808ee197) tests it !=sentinel via sub_1803A45B0 -> dispatches "FocusItem" flash call -> resets to sentinel (0x1808ee1f4)
    std::function<void()> m_callback;  // +0x2828  (0x40) MSVC std::function; _Ptr @+0x2860 (ctor 0 = empty, +0x2828..+0x2860 = SSO buffer); move-ctor sub_18042DC90 (SSO _Move vslot1 / steal), dtor sub_1803E23C0 (_Delete_this(_Ptr!=this) vslot4) [signature void() inferred]
};
static_assert(sizeof(C_UIApse) == 0x2868, "C_UIApse must be 0x2868 (alloc sub_18146C7B8)");
static_assert(offsetof(C_UIApse, m_inventory) == 0xD8, "inventory tab at 0xD8");
static_assert(offsetof(C_UIApse, m_questLog) == 0x10C8, "quest log at 0x10C8");
static_assert(offsetof(C_UIApse, m_map) == 0x1508, "map at 0x1508");
static_assert(offsetof(C_UIApse, m_camera) == 0x25C0, "camera at 0x25C0");
static_assert(offsetof(C_UIApse, m_modalInfo) == 0x2770, "modal info at 0x2770");

}  // namespace wh::guimodule
