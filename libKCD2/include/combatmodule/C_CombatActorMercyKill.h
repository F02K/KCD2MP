#pragma once
#include <cstdint>
#include "C_CombatActorObject.h"
#include "I_CombatActorMercyKill.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorMercyKill -- per-actor mercy-kill (finisher) subsystem (KCD2 1.5.6, kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorMercyKill@combatmodule@wh@@ (TD 0x184B00768). ctor sub_180917910; dtor sub_18182133C.
// Stored at C_CombatActor+0x408. Subsystem id 30 (GetSubsystemId sub_181A7DF50), name "MERCY_KILL"
// (GetName sub_181A7DE60). Reset (sub_181A7E060) zeroes +0x18. The ctor also connects a (nop) callback to
// the actor's combat-state signal (state object at actor+0x210, +0x18).
//
// MI (decoded from RTTI BaseClassArray of TD 0x184B00768):
//   +0x00 C_CombatActorObject     (primary, mdisp 0x00; vtable 0x183A63148)  -- note: NO UpdatedObject layer
//   +0x10 I_CombatActorMercyKill  (secondary, mdisp 0x10; vtable 0x183A63128)
// Own data at +0x18.
//
// [vs KCD1] Same 0x20 shape (C_CombatActorObject base, interface at +0x10, u32 state at +0x18). Subsystem id
// changed 0x1D -> 30.

namespace wh::combatmodule {

class C_CombatActorMercyKill : public C_CombatActorObject,
                               public I_CombatActorMercyKill {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorMercyKill;
    // GetSubsystemId() -> 30 ; GetName() -> "MERCY_KILL" ; Can/Request implemented in binary
    // (sub_1816043EC / sub_18275ED74) -- overrides omitted (layout-only header).
    EntityId m_victimEntityId;  // +0x18  latched mercy-kill target (Request sub_18275ED74 stores it @0x18275EE9E; 0 = idle)
    uint32_t _pad1C;       // +0x1C
};
static_assert(sizeof(C_CombatActorMercyKill) == 0x20, "C_CombatActor+0x408 subsystem is 0x20");

}  // namespace wh::combatmodule
