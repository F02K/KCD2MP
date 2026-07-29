#include "combatmodule/C_CombatScene.h"
#include "combatmodule/C_CombatModule.h"

// No engine call needed: the module owns the scene at +0x20 (ctor-verified, C_CombatModule.h).

wh::combatmodule::C_CombatScene* wh::combatmodule::C_CombatScene::GetInstance() {
    auto* mod = C_CombatModule::GetInstance();
    return mod ? mod->m_pCombatScene : nullptr;
}
