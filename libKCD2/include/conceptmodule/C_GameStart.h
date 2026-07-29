#pragma once
#include "C_EventNode.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_GameStart -- game-start event source
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xE8, vtable 0x183B570E0
// (42 slots).  36 corpus uses; pure source (no In pins authored).
// -----------------------------------------------
// Chain: C_Node -> C_EventNode<C_Node> -> here (NOT a TemplatedNode -- 0/36 corpus
// instances carry TypeT).  Ctor 0x1812EE768, creator 0x1812EE6B8; rttr
// "wh::conceptmodule::GameStart" (reg sub_18165D624; the registration binds the
// event name CryStringT("OnStart")).  OnLifecycleEvent [34] 0x1819EBF58 fires
// OnStart with IsDebug in the event map [mechanism LIKELY].  Member offsets forced
// by sizeof: base 0x68 + 2 x 0x40 = 0xE8.

namespace wh::conceptmodule {

class C_GameStart : public C_EventNode<C_Node> {
public:
    RTTR_ENABLE(C_EventNode<C_Node>)   // [5..7] trio overrides
    void EnumerateNodeVariants() override;      // [27] 0x18268E56C
    void OnLifecycleEvent(int32_t event) override;  // [34] 0x1819EBF58

    C_TypedPortRef<S_Trigger> m_onStart;  // +0x68  "OnStart" Out trigger
    C_TypedPortRef<bool>      m_isDebug;  // +0xA8  "IsDebug" Out data (event-scoped)
};
static_assert(sizeof(C_GameStart) == 0xE8, "C_GameStart must be 0xE8");

}  // namespace wh::conceptmodule
