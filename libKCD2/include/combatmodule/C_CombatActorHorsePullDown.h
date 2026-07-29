#pragma once
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"
#include "I_CombatActorHorsePullDown.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorHorsePullDown -- per-actor horse pull-down (dehorse) subsystem (KCD2 1.5.6, kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorHorsePullDown@combatmodule@wh@@ (TD 0x184B005F0). ctor sub_18091765C; dtor
// sub_181897AAC. Stored at C_CombatActor+0x400. Subsystem id 28 (GetSubsystemId sub_181A7DF30),
// name "HORSE_PULLDOWN" (GetName sub_181A7DE40). Reset (sub_181A7E050) zeroes +0x20 and +0x24.
//
// MI (decoded from RTTI BaseClassArray of TD 0x184B005F0):
//   +0x00 C_CombatActorUpdatedObject  (primary, mdisp 0x00; vtable 0x183A62F70)  ->  C_CombatActorObject
//   +0x18 I_CombatActorHorsePullDown  (secondary, mdisp 0x18; vtable 0x183A62DB0)
// The ctor calls the UpdatedObject ctor sub_180914754 with updateFlags=0 (registered as an actor object but
// not on any per-frame update list). Own data at +0x20/+0x24.
//
// [FUNDAMENTAL vs KCD1] KCD1 derived from C_CombatActorObject (0x10) so the interface sat at +0x10 and
// sizeof was 0x20. KCD2 inserts the C_CombatActorUpdatedObject base (0x18), pushing the I_CombatActorHorse-
// PullDown vtable to +0x18 and the state fields to +0x20/+0x24, and growing the object to 0x28. Subsystem id
// changed 0x1B -> 28.

namespace wh::combatmodule {

class C_CombatActorHorsePullDown : public C_CombatActorUpdatedObject,
                                   public I_CombatActorHorsePullDown {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorHorsePullDown;
    // GetSubsystemId() -> 28 ; GetName() -> "HORSE_PULLDOWN" ; Can/Request implemented in binary
    // (sub_18072FA9C / sub_18275E848) -- overrides omitted (layout-only header).
    EntityId m_victimEntityId;  // +0x20  latched pulldown target (Request sub_18275E848 stores it @0x18275EA49; 0 = idle)
    bool     m_flag24;     // +0x24  (init 0; zeroed by Reset)
    uint8_t  _pad25[3];    // +0x25
};
static_assert(sizeof(C_CombatActorHorsePullDown) == 0x28, "C_CombatActor+0x400 subsystem is 0x28");

}  // namespace wh::combatmodule
