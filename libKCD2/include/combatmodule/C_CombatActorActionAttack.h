#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave): C_CombatActorAnimatedAction<TParams,N,TPrivate>
#include "I_CombatActorActionPrivate.h"
#include "I_CombatActionHelperAttackOwner.h"
#include "S_CombatActorActionAttackParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionAttack -- normal melee attack action (KCD2 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionAttack@combatmodule@wh@@  (TD 0x184AFD3A0).
// primary vtable 0x183A61C58 (68 slots)   I_ActionImpl @+0x10   C_CombatActorObject @+0x68   I_CombatActionHelperAttackOwner @+0x88
// object ctor sub_18091571C   factory sub_18091590C (operator new 0xB0 -> proven sizeof)   base-chain ctor sub_18091581C
//
// Full inheritance spine (from RTTI base array, most-derived first):
//   C_CombatActorActionAttack
//    : C_CombatActorAnimatedAction<S_CombatActorActionAttackParams, 1, I_CombatActorActionPrivate>   // primary @+0x00
//    , I_CombatActionHelperAttackOwner                                                                // MI base @+0x88
//   and the AnimatedAction chain resolves to:
//     C_CombatActorAnimatedAction -> C_CombatActorParamAction -> C_CombatActorAction<Private,
//       C_ParamAction<AttackParams, C_Action<Private>>> -> C_ParamAction -> C_Action<Private>.
//   C_CombatActorAction<> introduces the C_CombatActorObject base (@+0x68, 0x10) plus its own 0x10 of
//   data (@+0x78..+0x88). m_params (AttackParams, 0x18) sits at +0x50 (embedded in C_ParamAction).
//
// [vs KCD1] Shrank 0x138 -> 0xB0 (C_Action 0xD0->0x50; one C_Signal instead of three). The KCD1
// intermediate C_CombatActorActionT is replaced by the C_CombatActorAnimatedAction/ParamAction/Action
// template chain above. KCD1 fields map 1:1: unknown_110->+0x90, flag_118->+0x98, pAttackHelper->+0xA0,
// scoreSentinel->+0xA8.
//
// Own primary-vtable overrides (leaf 0x183A61C58 vs base C_Action<Private> 0x183A60988): GetActionTypeId
// [14]=0x181A72CF0, many I_CombatActorAction slots [26..55], _vfOwn0[56]=0x18090C6F8,
// OnStart[57]=0x180B27308, OnStop[58]=0x180D4CAC0, OnTick[60]=0x180D4CFAC, GetNameString[64]=0x18275860C.
// Plus the 8 I_CombatActionHelperAttackOwner impls (vtable 0x183A61440). Vtable not re-enumerated here.

namespace wh::combatmodule {

class C_CombatActionHelperAttack;   // owned attack helper (alloc 0x50; ctor sub_180915088; module-core wave)

class C_CombatActorActionAttack
    : public C_CombatActorAnimatedAction<S_CombatActorActionAttackParams, 1, I_CombatActorActionPrivate>,
      public I_CombatActionHelperAttackOwner
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionAttack;
    // ---- own data (offsets are absolute in the leaf object) ----
    uint32_t                m_attackVariant90; // +0x90  OnStart sub_180B27308 stores 2 (@0x180B274C9, long-reach path) or 3 (@0x180B274D5); enum candidate {2,3}
    uint8_t                 _pad94;            // +0x94
    bool                    m_flag95;          // +0x95  OnStart @0x180B27373 = sub_1809CB40C(&qword_185320478, key, qword_185320578) set-membership
    bool                    m_flag96;          // +0x96  OnStart @0x180B27395 = sub_1809CB40C(&qword_185320478, key, dword_185320494) set-membership
    uint8_t                 _pad97;            // +0x97
    uint16_t                m_flag_98;         // +0x98  (init 0)
    uint8_t                 _pad9A[6];         // +0x9A
    C_CombatActionHelperAttack* m_pAttackHelper;  // +0xA0  (alloc 0x50; owner = this HelperAttackOwner subobject @+0x88)
    int64_t                 m_scoreSentinel;   // +0xA8  (NOT ctor-initialized; KCD1 role = best/min-score sentinel; UNVERIFIED)
};
static_assert(sizeof(C_CombatActorActionAttack) == 0xB0);

}  // namespace wh::combatmodule
