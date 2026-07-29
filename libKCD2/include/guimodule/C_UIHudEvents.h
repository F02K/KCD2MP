#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashBase.h"
#include "questmodule/I_UIHudEventsQuest.h"
#include "framework/I_UIHudEvents.h"
#include "framework/I_EventLogListener.h"
#include "rpgmodule/I_LocationListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudEvents -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x98.
// -----------------------------------------------
// RTTI .?AVC_UIHudEvents@guimodule@wh@@ (TD 0x184CCFC88).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135DD8), IUIElementEventListener @+0x10
// (COL 0x4135E00), questmodule::I_UIHudEventsQuest @+0x58 (COL 0x4135E28),
// framework::I_UIHudEvents @+0x60 (COL 0x4135E50), framework::I_EventLogListener
// @+0x68 (COL 0x4135E78), rpgmodule::I_LocationListener @+0x70 (COL 0x4135EA0).
// vtables: primary 0x183C306C8 (14; GetElementName [11] 0x181A8E8F0 = "hud";
// [4] Update 0x180533BD8 = base drain + queued-event pump sub_180DC69B4 -- NOTE the
// IDB mislabels this slot body as "_DeleteExceptionPtr"), listener 0x183C30628 (8),
// quest 0x183C305F8 (5), events 0x183C30768 (26), log 0x183C305E8 (1),
// location 0x183C30670 (10: overrides [1] 0x180C4CB40 + [3] 0x180783AB0 = location
// discovered/updated banners). ctor sub_181877728; rttr as_std_shared_ptr creator
// sub_18187769C (alloc 0x98).
//
// The centre-screen game-event feed (quest banners, item transfer, skill checks,
// game log, location discovery). Init [1] sub_180ED19D8: base Init + caches the
// wh_ui_GameEventLogLevel ICVar (+0x78) + listener registrations. OnModuleMessage
// [3] sub_18085DEB8: msg 52 -> flash "ResetAfterGameplayStart", msg 2 -> clears
// *(ctx+0x160)+0x110, msg 77 -> flash "ResetAll" + clears the queued-event vector.
// It also registers many of its methods with rttr (method_wrapper TD family:
// CryStringT/CryGUID/E_QuestProgress/E_ItemTransferDirection signatures).

struct ICVar;

namespace wh::guimodule {

class C_UIHudEvents : public C_UIFlashBase,
                      public wh::questmodule::I_UIHudEventsQuest,
                      public wh::framework::I_UIHudEvents,
                      public wh::framework::I_EventLogListener,
                      public wh::rpgmodule::I_LocationListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudEvents;
    // Queued quest-event record (layout UNVERIFIED beyond the 32-byte stride; holds
    // {a2, int progress, CryStringT text, int -1} per the [Q1] queue writer).
    struct S_QueuedQuestEvent { uint8_t _raw[32]; };

    // I_EventLogListener override.
    void OnGameLogEvent(void* pEvent) override;   // [L0] 0x182B8E254  flash "ShowGameLog" gated on m_pGameEventLogLevelCVar

    ICVar* m_pGameEventLogLevelCVar;                    // +0x78  wh_ui_GameEventLogLevel (Init @0x180ed1a1b)
    std::vector<S_QueuedQuestEvent> m_queuedQuestEvents;  // +0x80  (push @0x181746515, cleared by msg 77 sub_180DC6A5C)
};
static_assert(sizeof(C_UIHudEvents) == 0x98, "C_UIHudEvents must be 0x98");
static_assert(offsetof(C_UIHudEvents, m_pGameEventLogLevelCVar) == 0x78, "CVar at 0x78");

}  // namespace wh::guimodule
