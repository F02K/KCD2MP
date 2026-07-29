#include "playermodule/C_AlchemyActionDirector.h"
#include "Offsets/Offsets.h"

// C_AlchemyActionDirector engine-function forwarders (KCD2 WHGame.dll 1.5.6, kd7u;
// ids = kcd2 address library).

namespace wh::playermodule {

bool C_AlchemyActionDirector::IsSlotOccupied(E_AlchemyDirectorSlot::Type slot) const
{
    // 0x1808D2450: bounds-checked probe of m_actions[slot] (see E_AlchemyDirectorSlot).
    using Fn = bool (__fastcall*)(const C_AlchemyActionDirector*, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(48168) };  // 0x1808D2450
    return fn(this, slot);
}

void C_AlchemyActionDirector::PushAction(_smart_ptr<I_AlchemyAction>& spAction)
{
    // 0x1809CC3D4: insert/replace into the action's GetSlotIndex() slot (priority resolve
    // sub_1809CC5F0 + listener notify).  GOG maps to an ICF-folded copy (addresslib score 0.85).
    using Fn = void (__fastcall*)(C_AlchemyActionDirector*, _smart_ptr<I_AlchemyAction>*);
    static REL::Relocation<Fn> fn{ REL::ID(53632) };  // 0x1809CC3D4
    fn(this, &spAction);
}

void C_AlchemyActionDirector::Tick()
{
    // 0x180737A7C: step every occupied slot via its I_ActionImpl; clear finished slots.  The
    // session input-drain (sub_180737920) already calls this each frame while the minigame runs.
    using Fn = void (__fastcall*)(C_AlchemyActionDirector*);
    static REL::Relocation<Fn> fn{ REL::ID(39960) };  // 0x180737A7C
    fn(this);
}

void C_AlchemyActionDirector::AdvanceSlot(E_AlchemyDirectorSlot::Type slot, int32_t resultCode)
{
    // 0x1809CD780: complete the slot's action with resultCode (2 = success-advance; the code the
    // Mannequin completion callbacks use, e.g. autocook's montage cb via sub_1806C3A90).
    using Fn = void (__fastcall*)(C_AlchemyActionDirector*, int32_t, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(53664) };  // 0x1809CD780
    fn(this, slot, resultCode);
}

}  // namespace wh::playermodule
