#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionSyncHitParams -- params embedded by C_CombatActorActionSyncHit
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// POD value struct -- NO vtable, NO RTTI TypeDescriptor (the SyncHit base ctor sub_180D4A0AC writes
// plain data into obj+0x50..). Sits at action+0x50 (C_ParamAction::m_params).
//
// Layout proved by sub_180D4A0AC + the SyncHit factory sub_180D4A144:
//   +0x50 (obj) = 0 (qword)  -> m_field0 (+0x00)   [factory writes the source ptr here: v11+0x50 = a3]
//   +0x58 (obj) = 0 (byte)   -> m_flag8  (+0x08)   [factory writes v11+0x58 = a4]
// C_CombatActorObject base of the action begins at obj+0x60, confirming this struct ends at 0x10.
// Field roles UNVERIFIED (POD).

namespace wh::combatmodule {

struct S_CombatActorActionSyncHitParams {
    void*   m_field0;    // +0x00  factory sub_180D4A144 stores a3 = *(originating-action+0x60); source/context object ptr (pointee class unrecovered)
    bool    m_flag8;     // +0x08  (init 0; set by factory)
    uint8_t _pad9[7];    // +0x09
};
static_assert(sizeof(S_CombatActorActionSyncHitParams) == 0x10);

}  // namespace wh::combatmodule
