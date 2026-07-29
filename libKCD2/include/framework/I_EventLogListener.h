#pragma once

// -----------------------------------------------
// wh::framework::I_EventLogListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 1 slot.
// -----------------------------------------------
// RTTI .?AVI_EventLogListener@framework@wh@@ (TD 0x184CCFCC0). Game-log event sink.
// C_UIHudEvents implements it @+0x68 (vtable 0x183C305E8, single slot):
//   [0] 0x182B8E254  OnGameLogEvent: gates on the wh_ui_GameEventLogLevel CVar
//       (owner+0x78, GetIVal >= event->vf[+0x18]() level), then flash-calls
//       "ShowGameLog"(type vf[+0x10], level, text vf[+0x28]).
// The event parameter is a polymorphic game-log record (vtable: [+0x10] type,
// [+0x18] level, [+0x28] text buffer) -- concrete type UNVERIFIED, name coined.

namespace wh::framework {

class I_EventLogListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_EventLogListener;
    virtual void OnGameLogEvent(void* pEvent) = 0;   // [0] 0x182B8E254 (impl in C_UIHudEvents)
};
static_assert(sizeof(I_EventLogListener) == 8);

}  // namespace wh::framework
