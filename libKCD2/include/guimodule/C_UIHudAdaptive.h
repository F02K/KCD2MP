#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/I_UIBuffsListener.h"
#include "guimodule/C_UIHudAdaptiveGroup.h"
#include "framework/I_SourceMonitorListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudAdaptive -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1A8.
// -----------------------------------------------
// RTTI .?AVC_UIHudAdaptive@guimodule@wh@@ (TD 0x184CD0B68).
// Bases: C_UIBase @+0x00 (COL 0x43768D8), I_UIBuffsListener @+0x10 (COL 0x4376888),
// wh::framework::I_SourceMonitorListener @+0x18 (COL 0x4376860).
// vtables: primary 0x183EEB8B8 (10, C_UIBase shape), buffs 0x183EEB838 (4),
// source-monitor 0x183EEB860 (1). ctor sub_181F775F0; rttr as_std_shared_ptr creator
// sub_181F75E00 (alloc 0x1A8) -> lives in C_GUIModule::m_uiElements.
// SINGLETON-STYLE GLOBAL: ctor publishes `this` at qword_18548BFB0 (RVA 0x548BFB0).
//
// The "adaptive HUD" auto-hide controller (wh_ui_EnableAdaptiveHud): NOT flash-bound
// itself -- it owns two embedded C_UIHudAdaptiveGroup fade groups and fades them based
// on activity flags fed by buff events, named UI sources and module messages.
// Slot overrides: [1] Init 0x181F78E90 (caches wh_ui_EnableAdaptiveHud GetIVal==1 at
// +0x190, change-callback sub_181F79460); [3] OnModuleMessage 0x181F79B40 (msg 52 ->
// m_bResetPending); [4] Update 0x181F7B390 (fade logic); [5] OnGameStart 0x181F79AA0
// (stores the game ptr at +0x28, samples the ctx+0x18 mode ==2 into +0x197, kicks
// sub_181F7A270); [6] OnGameEnd 0x181F79A80 (sub_181F79DC0 + clears +0x28).
// I_UIBuffsListener[0] 0x181F782F0: buff UI-type 1/3 -> m_bBuffsDirty.
// I_SourceMonitorListener[0] 0x181F79200: named sources -> flags ("uiApseClose" ->
// !active at +0x198, "uiCrime" +0x199, "uiMinigameBlacksmithing" +0x19A, "uiBuff" +0x1A2).

namespace wh::guimodule {

class C_UIHudAdaptive : public C_UIBase,
                        public I_UIBuffsListener,
                        public wh::framework::I_SourceMonitorListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudAdaptive;
    // C_UIBase overrides (primary vtable 0x183EEB8B8).
    void Init(C_GUIModule* pModule) override;      // [1] 0x181F78E90
    void OnModuleMessage(void* msg) override;      // [3] 0x181F79B40  msg 52 -> m_bResetPending
    void Update() override;                        // [4] 0x181F7B390
    void OnGameStart(void* pGame) override;        // [5] 0x181F79AA0
    void OnGameEnd(void* pGame) override;          // [6] 0x181F79A80
    // I_UIBuffsListener override.
    void OnBuffAdded(void* a2, void* pBuff) override;   // [B0] 0x181F782F0 -> m_bBuffsDirty
    // I_SourceMonitorListener override.
    void OnSourceEvent(void* a2, bool bActive) override;  // [S0] 0x181F79200  name->flag switch

    int32_t  m_20;                        // +0x20  ctor 0 [role UNVERIFIED]
    uint8_t  _pad24[4];                   // +0x24
    void*    m_pGame;                     // +0x28  OnGameStart writes a2 @0x181f79aa9, OnGameEnd clears; C_UIBase::OnGameStart param is void* [concrete type UNVERIFIED]
    C_UIHudAdaptiveGroup m_groupA;        // +0x30  fade group (role split vs m_groupB UNVERIFIED)
    C_UIHudAdaptiveGroup m_groupB;        // +0xE0
    bool     m_bAdaptiveHudEnabled;       // +0x190 wh_ui_EnableAdaptiveHud GetIVal()==1 (Init @0x181f78ee9)
    bool     m_bBuffsDirty;               // +0x191 buff type 1/3 seen (@0x181f7830a)
    uint8_t  _pad192[5];                  // +0x192 ctor zero-fill; roles unknown
    bool     m_bMode2;                    // +0x197 ctx+0x18 mode ==2 at game start (@0x181f79ad1) [name coined]
    bool     m_bApseClosed;               // +0x198 "uiApseClose" -> !active (@0x181f79293)
    bool     m_bCrimeUIActive;            // +0x199 "uiCrime" active (@0x181f79245)
    bool     m_bBlacksmithingActive;      // +0x19A "uiMinigameBlacksmithing" active (@0x181f792af)
    uint8_t  _pad19B[7];                  // +0x19B
    bool     m_bBuffUIActive;             // +0x1A2 "uiBuff" active (@0x181f79273)
    uint8_t  _pad1A3;                     // +0x1A3
    bool     m_bResetPending;             // +0x1A4 module message 52 (@0x181f79b46)
    uint8_t  _pad1A5[3];                  // +0x1A5
};
static_assert(sizeof(C_UIHudAdaptive) == 0x1A8, "C_UIHudAdaptive must be 0x1A8");
static_assert(offsetof(C_UIHudAdaptive, m_groupA) == 0x30, "group A at 0x30 (ctor @0x181f77629)");
static_assert(offsetof(C_UIHudAdaptive, m_groupB) == 0xE0, "group B at 0xE0 (ctor @0x181f77635)");
static_assert(offsetof(C_UIHudAdaptive, m_bAdaptiveHudEnabled) == 0x190, "flag block at 0x190");

}  // namespace wh::guimodule
