#include "combatmodule/C_CombatComboManager.h"
#include "Offsets/Offsets.h"

// C_CombatComboManager forwarder -- KCD2 WHGame.dll 1.5.6 RVA (verified in kd7u).

namespace wh::combatmodule {

// Direct call into the engine's combo-advance handler sub_180C5C52C (KCD1 sub_180602C34
// rebuild; invoked in-engine from the state-event dispatcher sub_180911250 case 32). Unlike
// KCD1 it takes only `this` (the unused emitter arg + char gate were dropped) and adds the
// m_stepAppliedFlag dedup gate. See the header for the full semantics / preconditions.
void C_CombatComboManager::ForceAdvanceStep()
{
    using Fn = void(__fastcall*)(C_CombatComboManager*);
    static REL::Relocation<Fn> fn{ REL::ID(35) };
    fn(this);
}

}  // namespace wh::combatmodule
