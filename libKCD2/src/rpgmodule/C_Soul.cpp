#include "rpgmodule/C_Soul.h"
#include "rpgmodule/C_SoulList.h"
#include "rpgmodule/C_RPGModule.h"
#include "Offsets/Offsets.h"
#include "Offsets/vtables/IEntitySystem.h"
#include "Offsets/vtables/IEntity.h"
#include "crysystem/SSystemGlobalEnvironment.h"

// C_Soul / C_SoulList engine-function forwarders. Thin wrappers around verified
// KCD2 1.5.6 RVAs (mirrors C_FactionManager.cpp).

namespace wh { namespace rpgmodule {

C_SoulList* C_SoulList::GetInstance()
{
    C_RPGModule* rpg = C_RPGModule::GetInstance();
    return rpg ? rpg->m_pSoulList : nullptr;   // ctor-owned, C_RPGModule+0x80
}

C_Soul* C_SoulList::LookupByWUID(const wh::framework::WUID& wuid)
{
    // sub_181F985D0(&m_soulTable, &wuid) -- the exact call the script-bind soul
    // resolver makes (sub_18041ED20: "__ThisWUID" -> RPGModule+0x80, table @+0x38).
    using Fn = C_Soul* (__fastcall*)(void*, const wh::framework::WUID*);
    static REL::Relocation<Fn> fn{ REL::ID(79) };
    return fn(&m_soulTable, &wuid);
}

float C_Soul::GetDerivedStat(E_DerivedStat statId) const
{
    // sub_180648B18(soul, statId, 0): derived-stat evaluator; float in xmm0.
    using Fn = float (__fastcall*)(const C_Soul*, int, int64_t);
    static REL::Relocation<Fn> fn{ REL::ID(17) };
    return fn(this, static_cast<int32_t>(statId), 0);
}

bool C_Soul::HasAbility(uint32_t abilityId) const
{
    // Worker sub_1809DCC70(block, out, id): binary-search of the sorted ability
    // block @soul+0x320 into a {bool present, u64 payload} result; ids 62/18 are
    // computed inside the worker, and the C_RPGModule cvar block's all-abilities
    // flag forces true. Ids 0 / 73 additionally gate on derived stat 186 / 187
    // (mirrors the Lua HasAbility handler 0x182CF7D88).
    struct S_Result { bool present; uint8_t _pad[7]; uint64_t payload; };
    using Fn = S_Result* (__fastcall*)(const void*, S_Result*, uint32_t);
    static REL::Relocation<Fn> fn{ REL::ID(26) };
    S_Result r{};
    fn(&m_soulAbilities, &r, abilityId);
    if (abilityId == 0)
        return r.present && GetDerivedStat(E_DerivedStat::Atd) > 0.0f;
    if (abilityId == 73)
        return r.present && GetDerivedStat(E_DerivedStat::Ams) > 0.0f;
    return r.present;
}

float C_Soul::GetSkillFraction(uint32_t skillId, bool visitorFlag) const
{
    // sub_18046F81C (vtable +0x340): GetSkillLevel_18046F854 / maxLevel; the char flag is
    // forwarded into the modifier-visitor arg pack (uninitialized on the game's own
    // tolerance path -- see the header note).
    using Fn = float (__fastcall*)(const C_Soul*, uint32_t, char);
    static REL::Relocation<Fn> fn{ REL::ID(26997) };
    return fn(this, skillId, visitorFlag);
}

float C_Soul::GetPerkStatModifier(E_PerkStat statId, float seed, void* ctx) const
{
    // sub_180649F1C (vtable +0x310): modifier fold over seed + id-specific clamp.
    using Fn = float (__fastcall*)(const C_Soul*, int32_t, float, void*);
    static REL::Relocation<Fn> fn{ REL::ID(35185) };
    return fn(this, static_cast<int32_t>(statId), seed, ctx);
}

Offsets::IEntity* C_Soul::GetBoundEntity() const
{
    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pEntitySystem)
        return nullptr;

    const uint32_t id = env->pEntitySystem->FindEntityByGuid(m_entityGuid);
    return id ? env->pEntitySystem->GetEntity(id) : nullptr;
}

}}  // namespace wh::rpgmodule
