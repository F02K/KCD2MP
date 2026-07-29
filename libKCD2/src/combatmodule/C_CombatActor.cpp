#include "combatmodule/C_CombatActor.h"
#include "Offsets/Offsets.h"

// C_CombatActor forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::combatmodule {

void C_CombatActor::DispatchCounterAction(I_CombatActorActionPtr* pOutAction, E_CounterActionType type,
                                          uint32_t scopeIndex, bool useOpponentDefenseZone, int32_t queryId)
{
    // sub_181483580 -- the counter-dispatch core (KCD1 sub_1805633D0 analogue, same trigger-kind
    // constants 4/6/8/2, factories via m_pActionManager +0x3A8, SetLockAction sub_181483A28 on
    // m_pOpponentManager +0x3B8, SetAction via sub_18090C850). Verified against KCD1 by full
    // structural correspondence; type domain differs -- see E_CounterActionType.h.
    // NOTE [vs KCD1]: KCD1's 3-arg thin wrapper (sub_18069536C, a5=0/a6=-1/caller guard struct)
    // does not exist in KCD2 -- the a7 guard struct was dropped and a5/a6 are caller-supplied,
    // so this forwarder targets the core with KCD1-wrapper-equivalent defaults.
    using Fn = void*(__fastcall*)(C_CombatActor*, I_CombatActorActionPtr*, char, uint32_t, char, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(59) };
    fn(this, pOutAction, static_cast<char>(type), scopeIndex, useOpponentDefenseZone, queryId);
}

void C_CombatActor::SetOpponent(C_CombatActor* target)
{
    // sub_182757B10: extracts target->GetEntity()+0x38 (entity handle, identical to KCD1
    // sub_18063A634), then sub_180B21328 gates on m_isFullyBuilt (+0x27A) and forwards to
    // m_pOpponentManager (+0x3B8) -> UpdateOpponent sub_180B2158C (KCD1 sub_180303720
    // analogue: same dedup checks, exclusive-lock release, target store + LinkTarget chain).
    // Called both ways by the hunt-attack Request sub_18275EB64, the KCD1 usage pattern.
    using Fn = void(__fastcall*)(C_CombatActor*, C_CombatActor*);
    static REL::Relocation<Fn> fn{ REL::ID(80) };
    fn(this, target);
}

}  // namespace wh::combatmodule
