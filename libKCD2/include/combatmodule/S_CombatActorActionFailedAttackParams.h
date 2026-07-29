#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionFailedAttackParams -- params embedded by
// C_CombatActorActionFailedAttack (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.  [KCD2-NEW]
// -----------------------------------------------
// KCD2-NEW: no KCD1 reference. POD value struct -- NO vtable, NO RTTI TypeDescriptor (the FailedAttack
// base ctor sub_182753B34 writes obj+0x50 = 0, a plain qword, NOT a params vftable). Sits at
// action+0x50 (C_ParamAction::m_params).
//
// Layout proved by sub_182753B34:
//   +0x50 (obj) = 0 (qword)  -> m_field0 (+0x00)
// C_CombatActorObject base of the action begins at obj+0x58, confirming this struct ends at 0x08.
// Field role UNVERIFIED (POD) -- likely a pointer to the attack that failed.

namespace wh::combatmodule {

struct S_CombatActorActionFailedAttackParams {
    void* m_field0;   // +0x00  writer sub_180D53B74@0x180D53BF9 stores action+0x50 = *v9 (first elem of the failed-attack candidate query sub_180D53C40); a candidate pointer (qword). Exact pointee class UNVERIFIED.
};
static_assert(sizeof(S_CombatActorActionFailedAttackParams) == 0x08);

}  // namespace wh::combatmodule
