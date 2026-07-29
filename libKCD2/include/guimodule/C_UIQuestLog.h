#pragma once
#include <cstdint>
#include <map>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UITreeList.h"
#include "guimodule/C_UIExtendedInfo.h"
#include "guimodule/C_UIModalDialogInfo.h"
#include "entitymodule/I_ItemCollectionListener.h"
#include "framework/C_Signal.h"
#include "Offsets/vtables/IActionListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIQuestLog -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x440.
// -----------------------------------------------
// RTTI .?AVC_UIQuestLog@guimodule@wh@@ (TD 0x184C91DA0).
// Bases: C_UIBase @+0x00 (COL 0x4349A78), IActionListener @+0x10 (COL 0x4349AA0),
// entitymodule::I_ItemCollectionListener @+0x18 (COL 0x4349AC8).
// vtables: primary 0x183ED8DC8 (10, C_UIBase shape -- base RTTR trio, so NOT a
// separately rttr-registered screen), action 0x183ED8E20 (3), item-collection
// 0x183ED8E40 (6: overrides [0] 0x180DC51B4 + [2] 0x180DC5758). ctor sub_18146D5B4
// (this, C_UIApse* owner).
//
// NOT flash-bound itself and NOT in C_GUIModule::m_uiElements: lives EMBEDDED in the
// Apse (character/menu framework) screen C_UIApse at +0x10C8 (C_UIApse ctor
// sub_18146C89C @0x18146c9af; next embed at +0x1508 pins sizeof = 0x440); the owner
// pointer is the ctor arg. Presentation is delegated to three embedded flash widgets:
// the quest list (C_UITreeList "ApseQuestLogList"), the diary panel (C_UIExtendedInfo
// "ApseQuestLogDiary") and a modal info box.
// Init [1] sub_18174E130(module): chains m_questList.vf[1]() / m_diary.vf[1](module) /
// m_modalInfo.vf[1](module), registers {this, sub_180DC4C3C}/{this, sub_180DC5088}
// with the questmodule listener lists (*(ctx+0x118)+0x10 -> +8/+24) and connects
// {this, sub_1805620F0} to the owner C_UIApse's view signal (owner+0x88).
// Deinit [2] sub_182AFE83C mirrors. OnModuleMessage [3] sub_18085E104: msg 10 ->
// sub_180DC8784, 60 -> sub_180DC8830, 52/65 -> reset(true), 63/77 -> reset(false)
// (sub_1819AFFCC). Update [4] sub_180567A6C pumps the three embeds' vf[4].
// OnAction (action vtable [1] 0x18194D3BC) dispatches through the action-name ->
// member-handler map @+0x428. Item-collection callbacks mark quest entries dirty in
// the map @+0xA0 (sub_18061C9C4).
//
// [MODERATE vs KCD1] KCD1 C_UIQuestLog was a CUIManager event system; KCD2 demotes it
// to a value member of C_UIApse composed of generic flash widgets.

namespace wh::guimodule {

class C_UIApse;   // Apse menu hub (G4 scope; embeds this class at +0x10C8)

class C_UIQuestLog : public C_UIBase,
                     public Offsets::IActionListener,
                     public wh::entitymodule::I_ItemCollectionListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIQuestLog;
    // Per-tab list state written by the ctor helper sub_181F455D0(addr, count, "")
    // with counts {3,3,6,1}; internal layout UNVERIFIED (G4 scope).
    struct S_ListState { uint8_t _raw[0x18]; };

    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;      // [1] 0x18174E130
    void Deinit() override;                        // [2] 0x182AFE83C
    void OnModuleMessage(void* msg) override;      // [3] 0x18085E104
    void Update() override;                        // [4] 0x180567A6C
    // IActionListener override.
    void OnAction(const Offsets::SActionId& action, int activationMode, float value) override;   // [A1] 0x18194D3BC

    wh::shared::C_Signal<> m_signal20;        // +0x20  (sentinel unk_185665AC0) [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal30;        // +0x30  (sentinel unk_185665430) [signature UNVERIFIED]
    S_ListState m_listStates[4];              // +0x40  ctor counts {3,3,6,1}
    std::map<uint64_t, void*> m_dirtyQuests;  // +0xA0  item-collection dirty marks (sub_18061C9C4; K/V UNVERIFIED)
    std::map<uint64_t, void*> m_mapB0;        // +0xB0  K/V + role UNVERIFIED
    C_UITreeList     m_questList;             // +0xC0  EMBEDDED, element "ApseQuestLogList" (ctor @0x18146d673)
    C_UIExtendedInfo m_diary;                 // +0x2A0 EMBEDDED, element "ApseQuestLogDiary" (ctor @0x18146d686)
    C_UIModalDialogInfo m_modalInfo;          // +0x378 EMBEDDED (ctor @0x18146d699)
    C_UIApse* m_pApse;                        // +0x420 owning Apse screen (ctor arg)
    std::map<uint64_t, void*> m_actionHandlers;  // +0x428 action name -> member handler (OnAction lookup sub_1817111E8)
    bool m_flag438;                           // +0x438 ctor 0 [role UNVERIFIED]
    bool m_flag439;                           // +0x439 ctor 0 [role UNVERIFIED]
    uint8_t _pad43A[6];                       // +0x43A
};
static_assert(sizeof(C_UIQuestLog) == 0x440, "C_UIQuestLog must be 0x440");
static_assert(offsetof(C_UIQuestLog, m_questList) == 0xC0, "quest list at 0xC0");
static_assert(offsetof(C_UIQuestLog, m_diary) == 0x2A0, "diary at 0x2A0");
static_assert(offsetof(C_UIQuestLog, m_modalInfo) == 0x378, "modal info at 0x378");
static_assert(offsetof(C_UIQuestLog, m_pApse) == 0x420, "owner at 0x420");

}  // namespace wh::guimodule
