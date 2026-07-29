#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionHitParams -- params embedded by C_CombatActorActionHit
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// POD value struct -- NO vtable, NO RTTI TypeDescriptor (the Hit base ctor sub_180D49CF4 writes plain
// data into obj+0x50.., never a params vtable). Sits at action+0x50 (C_ParamAction::m_params).
//
// Layout proved by sub_180D49CF4:
//   +0x50 (obj) = 0 (qword)                 -> m_field0 (+0x00)
//   +0x58 (obj) = 0 (qword)                 -> m_field8 (+0x08)
//   +0x60 (obj) : *(u32)(obj+0x60) &= 0xFC  -> m_flags (+0x10; low 2 bits cleared)
// C_CombatActorObject base of the action begins at obj+0x68, confirming this struct ends at 0x18.
// Field roles UNVERIFIED (POD; no getters to attribute names).

namespace wh::combatmodule {

struct S_CombatActorActionHitParams {
    uint64_t m_field0;   // +0x00  (init 0)
    uint64_t m_field8;   // +0x08  (init 0)
    uint32_t m_flags;    // +0x10  (init: low 2 bits cleared)
    uint32_t _pad14;     // +0x14
};
static_assert(sizeof(S_CombatActorActionHitParams) == 0x18);

}  // namespace wh::combatmodule
