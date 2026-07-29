#include "combatmodule/S_CombatActionAttackQueryData.h"
#include "Offsets/Offsets.h"

// S_CombatActionAttackQueryData forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::combatmodule {

// engine ctor sub_1806881C8: installs the vtable, allocates the +0x28 intrusive-list sentinel
// (sub_180452BA4, 0x30 node), and defaults the id fields to -1.
S_CombatActionAttackQueryData::S_CombatActionAttackQueryData()
{
    using Fn = void(__fastcall*)(S_CombatActionAttackQueryData*);
    static REL::Relocation<Fn> fn{ REL::ID(19) };
    fn(this);
}

void* S_CombatActionAttackQueryData::GetTypeName(void* out)   // vtable[0] sub_18275AACC
{
    using Fn = void*(__fastcall*)(S_CombatActionAttackQueryData*, void*);
    static REL::Relocation<Fn> fn{ REL::ID(82) };
    return fn(this, out);
}

S_CombatActionAttackQueryData* S_CombatActionAttackQueryData::GetSelf()  // vtable[1] sub_1805F5DA0
{
    using Fn = S_CombatActionAttackQueryData*(__fastcall*)(S_CombatActionAttackQueryData*);
    static REL::Relocation<Fn> fn{ REL::ID(16) };
    return fn(this);
}

void* S_CombatActionAttackQueryData::GetCacheKeyData(void* out)  // vtable[2] sub_18275A87C
{
    using Fn = void*(__fastcall*)(S_CombatActionAttackQueryData*, void*);
    static REL::Relocation<Fn> fn{ REL::ID(81) };
    return fn(this, out);
}

uint64_t S_CombatActionAttackQueryData::GetTypeNameHash()   // vtable[3] sub_18277CB58
{
    using Fn = uint64_t(__fastcall*)(S_CombatActionAttackQueryData*);
    static REL::Relocation<Fn> fn{ REL::ID(83) };
    return fn(this);
}

}  // namespace wh::combatmodule
