#pragma once
#include <cstdint>
#include "C_CombatActorParamAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionGuardParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionGuard -- guard action leaf (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x98.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionGuard@combatmodule@wh@@ (TD 0x184AFDD30).
// ctor sub_180916CF4 (alloc 0x98 == PROVEN sizeof); base subobject ctor sub_180916DD8; dtor sub_181710FD4.
// Three vtables in the complete object (all resolve to this class' RTTI):
//   +0x00 primary 0x183A62B80  (C_Action / combat-action chain, 65 slots)
//   +0x10 secondary 0x183A623C0 (framework::I_ActionImpl, from C_Action's MI)
//   +0x58 secondary 0x183A62388 (combatmodule::C_CombatActorObject, MI base -- the "+0x58 mystery")
//
// TRUE Warhorse hierarchy (decoded from the RTTI BaseClassArray of TD 0x184AFDD30):
//   C_CombatActorActionGuard
//     : C_CombatActorParamAction<S_CombatActorActionGuardParams, I_CombatActorActionPrivate>
//         : C_CombatActorAction<I_CombatActorActionPrivate,
//                               C_ParamAction<S_CombatActorActionGuardParams, C_Action<I_CombatActorActionPrivate>>>
//             : C_ParamAction<S_CombatActorActionGuardParams, C_Action<I_CombatActorActionPrivate>>   (primary, mdisp 0x00)
//             : C_CombatActorObject                                                                   (MI base,  mdisp 0x58)
// The wrapper templates live in C_CombatActorParamAction.h / C_CombatActorAction.h; the
// +0x68/+0x6C pair is C_CombatActorAction<> data (set 0 by base ctor sub_180916DD8).
//
// [vs KCD1] KCD1 C_CombatActorActionGuard was 0x118 (C_Action was 0xD0 there and had NO Animated layer).
// KCD2 shrank to 0x98 (C_Action 0xD0->0x50). Guard is the ONLY guard leaf WITHOUT the
// C_CombatActorAnimatedAction interposition (GuardMovement/GuardSyncMovement/PoseModifier/GuardVar have it).

namespace wh::combatmodule {

class C_CombatActorActionGuard
    : public C_CombatActorParamAction<S_CombatActorActionGuardParams, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionGuard;
    // ---- C_CombatActorActionGuard own data (+0x70..+0x98; init 0 unless noted) ----
    _smart_ptr<I_CombatActorActionPrivate> m_action0;  // +0x70  owning ref to a delegate sub-action; ctor 0, dtor 0x18171101E Releases via vtbl[2]
    _smart_ptr<I_CombatActorActionPrivate> m_action1;  // +0x78  owning ref to a delegate sub-action; ctor 0, dtor 0x18171100F Releases via vtbl[2]
    _smart_ptr<I_CombatActorActionPrivate> m_action2;  // +0x80  owning ref to a delegate sub-action; ctor 0, dtor 0x181710FFD Releases via vtbl[2]
    bool     m_field88;            // +0x88  bool; dtor-path slot[58] skips a reset (context+336) when set
    bool     m_field89;            // +0x89  bool; dtor-path slot[58] skips a reset (context+1488=-1) when set; setters 0x181A71900/0x181A71910 write 0/1
    uint16_t _pad8A;               // +0x8A
    uint32_t m_field8C;            // +0x8C  (init 0)  dword copied to action-info out[0] by slot[35] 0x1809E37FF; role UNVERIFIED
    uint32_t m_field90;            // +0x90  (init 0)  dword copied to action-info out[4] and tested (if(!field90)) by slot[35] 0x1809E380A/0x1809E381E; role UNVERIFIED
    bool     m_field94;            // +0x94  bool init 1; virtual getter slot[30] 0x181A7D990 returns this byte
    uint8_t  _pad95;               // +0x95
    uint16_t _pad96;               // +0x96
};
static_assert(sizeof(C_CombatActorActionGuard) == 0x98, "ctor sub_180916CF4 allocates 0x98");

}  // namespace wh::combatmodule
