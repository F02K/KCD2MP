#include "combatmodule/S_AttackCandidateResultHolder.h"
#include "combatmodule/S_CombatActionAttackQueryData.h"
#include "Offsets/Offsets.h"

// S_AttackCandidateResultHolder forwarder -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::combatmodule {

void S_AttackCandidateResultHolder::Enumerate(void* context, S_CombatActionAttackQueryData* query)
{
    // sub_180687F3C(context, this, query): sets the +0x120 guard, walks the global attack moveset
    // via a std::function<void(unique_ptr<I_DatabaseSelector<S_CombatActionAttackQueryData,
    // S_CombatActionAttackData>> const&, S_QueryData&)>> appending 0x10-byte candidate records
    // (growing through the inline pool), introsorts them (stride 0x10), then clears the guard.
    // NOTE [vs KCD1]: KCD2 Enumerate takes an extra leading `context` arg (captured into the walk
    // lambda); KCD1's equivalent ignored its first arg.
    using Fn = void(__fastcall*)(void*, S_AttackCandidateResultHolder*, S_CombatActionAttackQueryData*);
    static REL::Relocation<Fn> fn{ REL::ID(18) };
    fn(context, this, query);
}

}  // namespace wh::combatmodule
