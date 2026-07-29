#pragma once
#include <cstdint>
#include <cstddef>
#include <map>
#include "C_CombatActorObject.h"
#include "../framework/C_ActionDirector.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorDirector -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xB8.
// -----------------------------------------------
// ctor sub_180917E80. Stored at C_CombatActor+0x200. MI (two vtables emitted by the ctor):
// wh::framework::C_ActionDirector primary @+0x00 (ctor'd with mode 0; vtable 0x183A63360 overrides
// dtor + IsActive -- IsActive checks the owning combat actor's state via owner->m_pState) and
// C_CombatActorObject secondary @+0x90 (vtable 0x183A63378; owner stored at +0x98). Own members
// from +0xA0. The ctor also connects a listener onto the inherited m_onActionNotification
// (delegate sub_18142D724: event 0 arms/caches a signal payload id, event 1 consumes it).
//
// [MODERATE vs KCD1] All own members shifted -0x20 (the C_ActionDirector base shrank 0xB0 -> 0x90);
// sizeof 0xD8 -> 0xB8.

namespace wh::combatmodule {

class C_CombatActorDirector
    : public wh::framework::C_ActionDirector   // +0x00  (0x90; mode 0)
    , public C_CombatActorObject               // +0x90  (vtable @+0x90, C_CombatActor* owner @+0x98)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorDirector;
    // channel id -> connection handle (node 0x30 => 16-byte pair payload; value = signal/connection
    // handle per KCD1 correspondence, insert site not decompiled).
    std::map<int32_t, void*> m_channelSubscriptions;   // +0xA0
    int32_t  m_pendingActionId;                        // +0xB0  armed by notification event 0, consumed by event 1 (name tentative)
    uint32_t _padB4;                                   // +0xB4
};
static_assert(sizeof(C_CombatActorDirector) == 0xB8, "C_CombatActorDirector must be 0xB8");
static_assert(offsetof(C_CombatActorDirector, m_channelSubscriptions) == 0xA0, "m_channelSubscriptions offset");

}  // namespace wh::combatmodule
