#include "playermodule/C_Alchemy.h"
#include "playermodule/C_PlayerModule.h"
#include "playermodule/C_MinigameManager.h"
#include "game/S_GameContext.h"
#include "Offsets/Offsets.h"

// C_Alchemy engine-function forwarders (KCD2 WHGame.dll 1.5.6, kd7u; ids = kcd2 address library).

namespace wh::playermodule {

C_Alchemy* C_Alchemy::GetSession(uint32_t userId, bool create)
{
    // Typed walk of the verified owner chain (Alchemy.OnUse sub_180D5AE04):
    // S_GameContext -> m_pPlayerModule -> m_pMinigameManager -> FindOrCreateSession(type 3).
    auto* ctx = wh::game::S_GameContext::GetInstance();
    auto* pm = ctx ? ctx->m_pPlayerModule : nullptr;
    auto* mgr = pm ? pm->m_pMinigameManager : nullptr;
    if (!mgr)
        return nullptr;
    return static_cast<C_Alchemy*>(
        mgr->FindOrCreateSession(userId, E_MinigameType::Alchemy, create, false));
}

float C_Alchemy::GetBrewTolerance()
{
    // 0x182E2C594: takes no real input (fetches S_GameContext itself); returns the boil/heat
    // tolerance band = lerp(AlchemyToleranceBase, AlchemyToleranceMaxLevel, alchemy level) via
    // the player skill object (*(playerCtx+0x668)); 0.0f while no player skill object exists.
    using Fn = float (__fastcall*)();
    static REL::Relocation<Fn> fn{ REL::ID(376223) };  // 0x182E2C594
    return fn();
}

bool C_Alchemy::Start(uint32_t tableEntityId)
{
    // 0x180897E1C: gate vtable[18], bind the table entity, reset the workspace, push the
    // "alchemy" action-map context; false if the session refuses (already running/invalid table).
    using Fn = char (__fastcall*)(C_Alchemy*, uint32_t);
    static REL::Relocation<Fn> fn{ REL::ID(47131) };  // 0x180897E1C
    return fn(this, tableEntityId) != 0;
}

void C_Alchemy::PerformVerb(E_AlchemyVerb::Type verb)
{
    // 0x1815FC494: NOP unless m_brewState.m_mode == 2; create (sub_1808D1FF8) + validate
    // (sub_1808D23E0) + enqueue (sub_1815FC55C) the verb's action, append to m_performedVerbs.
    using Fn = void (__fastcall*)(C_Alchemy*, E_AlchemyVerb::Type);
    static REL::Relocation<Fn> fn{ REL::ID(125936) };  // 0x1815FC494
    fn(this, verb);
}

bool C_Alchemy::CanPerformVerb(E_AlchemyVerb::Type verb)
{
    // 0x1808D1F90: per-verb permission byte (settings[181..197]) + create/validate dry-run.
    using Fn = bool (__fastcall*)(C_Alchemy*, E_AlchemyVerb::Type);
    static REL::Relocation<Fn> fn{ REL::ID(48159) };  // 0x1808D1F90
    return fn(this, verb);
}

E_AlchemyState::Type C_Alchemy::GetEffectiveState() const
{
    // 0x1808D237C: m_brewState.m_primaryState, or m_subState while primary == Idle (21).
    using Fn = E_AlchemyState::Type (__fastcall*)(const C_Alchemy*);
    static REL::Relocation<Fn> fn{ REL::ID(48162) };  // 0x1808D237C
    return fn(this);
}

void C_Alchemy::GetVerbActionName(CryStringT<char>& out, E_AlchemyVerb::Type verb)
{
    // 0x1808D1ECC: create the verb's action, ask it GetActionName (vtbl+0xD8); empty if none.
    using Fn = void (__fastcall*)(C_Alchemy*, CryStringT<char>*, E_AlchemyVerb::Type);
    static REL::Relocation<Fn> fn{ REL::ID(48156) };  // 0x1808D1ECC
    fn(this, &out, verb);
}

void C_Alchemy::ApplyIngredient(wh::framework::WUID itemWuid)
{
    // 0x180A955DC (AutocookApplyIngredient): classifies herb vs special, then assigns the item
    // to the first free station slot (0x180A95668 / 0x182E145B4): validates the wuid via
    // C_ItemManager::LookupByWUID, un-hides the station prop, writes m_herbSlot[i] = wuid and
    // m_herbSlotKey[i] = the item's class guid.
    using Fn = void (__fastcall*)(C_Alchemy*, wh::framework::WUID);
    static REL::Relocation<Fn> fn{ REL::ID(57382) };  // 0x180A955DC
    fn(this, itemWuid);
}

void C_Alchemy::ClearIngredientSlots()
{
    // 0x180A95760: for each herb (7-9) and special (10-12) station: hide the prop, slot value
    // <- sentinel, slot key <- null guid.  Autocook runs it before restocking; the in-session
    // Reset() calls it too.
    using Fn = void (__fastcall*)(C_Alchemy*);
    static REL::Relocation<Fn> fn{ REL::ID(57385) };  // 0x180A95760
    fn(this);
}

}  // namespace wh::playermodule
