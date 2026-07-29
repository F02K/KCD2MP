#include "entitymodule/C_Actor.h"
#include "Offsets/Offsets.h"
#include "rpgmodule/C_Soul.h"

// C_Actor engine-function forwarders (KCD2 WHGame.dll 1.5.6 RVAs, verified in kd7u).

namespace wh::entitymodule {

wh::combatmodule::C_CombatActor* C_Actor::GetOrCreateCombatActor()
{
    // sub_18072DC90: returns m_pCombatActor (+0x278), allocating the 0x448 C_CombatActor
    // on first use.
    using Fn = wh::combatmodule::C_CombatActor* (__fastcall*)(C_Actor*);
    static REL::Relocation<Fn> fn{ REL::ID(21) };
    return fn(this);
}

C_Inventory* C_Actor::GetInventory()
{
    // Typed reimplementation of sub_1808D285C (every hop is an RE'd member/virtual):
    // m_pSoul (+0x668) -> C_Soul::m_inventorySoul (+0x198) -> I_InventorySoul::GetInventory [0].
    return m_pSoul ? m_pSoul->m_inventorySoul.GetInventory() : nullptr;
}

}  // namespace wh::entitymodule
