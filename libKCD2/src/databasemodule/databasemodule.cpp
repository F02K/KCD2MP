#include "databasemodule/C_ObjectDatabaseManager.h"
#include "databasemodule/C_DatabaseModule.h"
#include "rpgmodule/C_ReputationChangeDatabase.h"
#include "rpgmodule/C_PerkDatabase.h"
#include "rpgmodule/C_SoulArchetypeDatabase.h"
#include "rpgmodule/C_SocialClassDatabase.h"
#include "rpgmodule/C_PerkBuffDatabase.h"
#include "rpgmodule/C_PerkScriptDatabase.h"
#include "rpgmodule/C_PerkSoulAbilityDatabase.h"
#include "rpgmodule/C_PerkRecipeDatabase.h"
#include "rpgmodule/C_PerkCombatTechniqueDatabase.h"
#include "rpgmodule/C_PerkCompanionDatabase.h"
#include "rpgmodule/C_PerkCodexDatabase.h"
#include "rpgmodule/C_Perk2PerkExclusivityDatabase.h"
#include "rpgmodule/C_ReputationConditionDatabase.h"
#include "rpgmodule/C_ReputationNotificationDatabase.h"
#include "rpgmodule/C_Location2PerkDatabase.h"
#include "rpgmodule/C_POIType2PerkDatabase.h"
#include "rpgmodule/C_PerkBuffOverrideDatabase.h"
#include "rpgmodule/C_PerkRPGParamOverrideDatabase.h"
#include "rpgmodule/C_DocumentRewardPerkDatabase.h"
#include "rpgmodule/C_SoulStateEffectContextDatabase.h"
#include "Offsets/Offsets.h"

// databasemodule singleton accessors (KCD2 WHGame.dll 1.5.6 RVAs, verified in kd7u).

namespace wh::databasemodule {

C_ObjectDatabaseManager* C_ObjectDatabaseManager::GetInstance()
{
    // sub_180EF4F8C: TLS-guarded lazy init of the 0x1854A49A0 function-local static
    // (ctor sub_1814BCFF8, atexit dtor). Forward so first-call init runs.
    using Fn = C_ObjectDatabaseManager* (__fastcall*)();
    static REL::Relocation<Fn> fn{ REL::ID(39) };
    return fn();
}

C_DatabaseModule* C_DatabaseModule::GetInstance()
{
    // qword_185168B48: the ctor sub_18192BBB4 publishes the undisplaced `this` here.
    // (module-registry+0x148 holds this+0x10 -- the I_DatabaseModule subobject -- so reading
    // that slot as C_DatabaseModule* would be off by 0x10.)
    static REL::Relocation<C_DatabaseModule**> p{ REL::ID(2325) };
    return *p;
}

}  // namespace wh::databasemodule

namespace wh::rpgmodule {

C_ReputationChangeDatabase* C_ReputationChangeDatabase::GetInstance()
{
    // Global static instance (ctor sub_1811CEEB8 runs at static-init).
    static REL::Relocation<C_ReputationChangeDatabase*> inst{ REL::ID(2343) };
    return inst.get();
}

C_PerkDatabase* C_PerkDatabase::GetInstance()
{
    // Global static instance (ctor sub_18188ED28) -- the registry C_Perk::Resolve reads.
    static REL::Relocation<C_PerkDatabase*> inst{ REL::ID(2338) };
    return inst.get();
}

C_SoulArchetypeDatabase* C_SoulArchetypeDatabase::GetInstance()
{
    // Global static instance (ctor sub_1815A1290).
    static REL::Relocation<C_SoulArchetypeDatabase*> inst{ REL::ID(2347) };
    return inst.get();
}

C_SocialClassDatabase* C_SocialClassDatabase::GetInstance()
{
    // Global static instance (ctor sub_18192F920).
    static REL::Relocation<C_SocialClassDatabase*> inst{ REL::ID(2346) };
    return inst.get();
}

// Global static table instances (each ctor runs at static-init; RVAs verified in kd7u).
C_PerkBuffDatabase* C_PerkBuffDatabase::GetInstance()
{
    static REL::Relocation<C_PerkBuffDatabase*> inst{ REL::ID(2333) };
    return inst.get();
}

C_PerkScriptDatabase* C_PerkScriptDatabase::GetInstance()
{
    static REL::Relocation<C_PerkScriptDatabase*> inst{ REL::ID(2341) };
    return inst.get();
}

C_PerkSoulAbilityDatabase* C_PerkSoulAbilityDatabase::GetInstance()
{
    static REL::Relocation<C_PerkSoulAbilityDatabase*> inst{ REL::ID(2342) };
    return inst.get();
}

C_PerkRecipeDatabase* C_PerkRecipeDatabase::GetInstance()
{
    static REL::Relocation<C_PerkRecipeDatabase*> inst{ REL::ID(2340) };
    return inst.get();
}

C_PerkCombatTechniqueDatabase* C_PerkCombatTechniqueDatabase::GetInstance()
{
    static REL::Relocation<C_PerkCombatTechniqueDatabase*> inst{ REL::ID(2336) };
    return inst.get();
}

C_PerkCompanionDatabase* C_PerkCompanionDatabase::GetInstance()
{
    static REL::Relocation<C_PerkCompanionDatabase*> inst{ REL::ID(2337) };
    return inst.get();
}

C_PerkCodexDatabase* C_PerkCodexDatabase::GetInstance()
{
    static REL::Relocation<C_PerkCodexDatabase*> inst{ REL::ID(2335) };
    return inst.get();
}

C_Perk2PerkExclusivityDatabase* C_Perk2PerkExclusivityDatabase::GetInstance()
{
    static REL::Relocation<C_Perk2PerkExclusivityDatabase*> inst{ REL::ID(2332) };
    return inst.get();
}

C_ReputationConditionDatabase* C_ReputationConditionDatabase::GetInstance()
{
    static REL::Relocation<C_ReputationConditionDatabase*> inst{ REL::ID(2345) };
    return inst.get();
}

C_ReputationNotificationDatabase* C_ReputationNotificationDatabase::GetInstance()
{
    static REL::Relocation<C_ReputationNotificationDatabase*> inst{ REL::ID(2344) };
    return inst.get();
}

C_Location2PerkDatabase* C_Location2PerkDatabase::GetInstance()
{
    static REL::Relocation<C_Location2PerkDatabase*> inst{ REL::ID(2330) };
    return inst.get();
}

C_POIType2PerkDatabase* C_POIType2PerkDatabase::GetInstance()
{
    static REL::Relocation<C_POIType2PerkDatabase*> inst{ REL::ID(2331) };
    return inst.get();
}

C_PerkBuffOverrideDatabase* C_PerkBuffOverrideDatabase::GetInstance()
{
    static REL::Relocation<C_PerkBuffOverrideDatabase*> inst{ REL::ID(2334) };
    return inst.get();
}

C_PerkRPGParamOverrideDatabase* C_PerkRPGParamOverrideDatabase::GetInstance()
{
    static REL::Relocation<C_PerkRPGParamOverrideDatabase*> inst{ REL::ID(2339) };
    return inst.get();
}

C_DocumentRewardPerkDatabase* C_DocumentRewardPerkDatabase::GetInstance()
{
    static REL::Relocation<C_DocumentRewardPerkDatabase*> inst{ REL::ID(2329) };
    return inst.get();
}

C_SoulStateEffectContextDatabase* C_SoulStateEffectContextDatabase::GetInstance()
{
    // Global static (tree-database stack, registered by class name; ctor sub_180F9F344).
    static REL::Relocation<C_SoulStateEffectContextDatabase*> inst{ REL::ID(2348) };
    return inst.get();
}

}  // namespace wh::rpgmodule
