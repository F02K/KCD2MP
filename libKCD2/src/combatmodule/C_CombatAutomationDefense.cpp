#include "combatmodule/C_CombatAutomationDefense.h"
#include "Offsets/Offsets.h"

// C_CombatAutomationDefense forwarders -- KCD2 WHGame.dll 1.5.6 RVA (verified in kd7u).
// sub_18194C270 is logic-identical to KCD1 sub_1804F83C8 (free old block, alloc 0x14, copy
// OWORD+DWORD = 5 floats, store at m_pWeightOverride) and is reached in-engine via the
// I_CombatAutomation secondary vtable slot [9] thunk 0x182766C04 onto m_defense (+0x118).

namespace wh::combatmodule {

void C_CombatAutomationDefense::SetReactionWeights(const S_ReactionWeights& weights)
{
    using Fn = void(__fastcall*)(C_CombatAutomationDefense*, const S_ReactionWeights*);
    static REL::Relocation<Fn> fn{ REL::ID(61) };
    fn(this, &weights);
}

void C_CombatAutomationDefense::ClearReactionWeights()
{
    using Fn = void(__fastcall*)(C_CombatAutomationDefense*, const S_ReactionWeights*);
    static REL::Relocation<Fn> fn{ REL::ID(61) };  // nullptr clears
    fn(this, nullptr);
}

}  // namespace wh::combatmodule
