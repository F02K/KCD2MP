#pragma once
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::questmodule::I_UIHudEventsQuest -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 5 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudEventsQuest@questmodule@wh@@ (TD 0x184CD0BD8). Quest-module face of
// the HUD event feed (quest started/updated banners). C_UIHudEvents implements it
// @+0x58 (vtable 0x183C305F8): 2 methods + RTTR trio [2..4] (0x18213A638/0x18213A5FC/
// 0x18213A584). No virtual dtor slot.
//   [0] 0x180DC855C  immediate quest-banner flash call (int progress-1, quest text,
//       bool localize, varargs) -- matches the rttr method_wrapper family
//       void(CryStringT const&, E_QuestProgress, CryStringT const&, bool, int)
//   [1] 0x1817464B4  queues a 32-byte quest-event record into owner+0x80 (deferred
//       display) -- (a2, int, CryStringT)
// Names coined; exact parameter lists UNVERIFIED.

namespace wh::questmodule {

class I_UIHudEventsQuest {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudEventsQuest;
    virtual void ShowQuestEvent(void* a2, int progress, const CryStringT<char>& text, bool bLocalize) = 0;  // [0] 0x180DC855C (coined)
    virtual void QueueQuestEvent(void* a2, int progress, const CryStringT<char>& text) = 0;                 // [1] 0x1817464B4 (coined)
    // RTTR trio [2..4] (impl-vtable thunks 0x18213A638/0x18213A5FC/0x18213A584, -0x58).
    // I_UIHudEventsQuest itself is NOT rttr-registered and appears in no base list.
    // Slots binary-proven; macro spelling is the inferred idiom.
    RTTR_ENABLE()
};
static_assert(sizeof(I_UIHudEventsQuest) == 8);

}  // namespace wh::questmodule
