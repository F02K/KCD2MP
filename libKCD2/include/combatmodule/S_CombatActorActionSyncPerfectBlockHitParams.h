#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionSyncPerfectBlockHitParams -- params of
// C_CombatActorActionSyncPerfectBlockHit (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.  PLAIN POD.
// -----------------------------------------------
// Not polymorphic (no RTTI TypeDescriptor).  Embedded by value at +0x50 of the action; the
// C_CombatActorObject base begins at +0x60, proving the params block is 0x10.
// Layout from the factory sub_1810F3148, which after constructing the action writes:
//   *(action+0x50) = a3  (params+0x00, a pointer argument)
//   *(action+0x58) = a4  (params+0x08, a byte/flag argument)
// Pointee/flag semantics UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionSyncPerfectBlockHitParams {
    void*   m_pField00;      // +0x00  factory sub_1810F3148 stores a3 = *(originating-action+0x78); source/context object ptr (pointee class unrecovered)
    uint8_t m_field08;       // +0x08  factory arg a4 = sub_180688B44(actor state+0x210, 4) bit-test; flag
                             //        (bool candidate -- helper's return range unproven, kept uint8_t)
    uint8_t _pad09[7];       // +0x09
};
static_assert(sizeof(S_CombatActorActionSyncPerfectBlockHitParams) == 0x10,
              "S_CombatActorActionSyncPerfectBlockHitParams POD, 0x10");

}  // namespace wh::combatmodule
