#include "rpgmodule/C_RPGModule.h"
#include "rpgmodule/C_BuffManager.h"
#include "rpgmodule/C_RPGLocationManager.h"
#include "Offsets/Offsets.h"

// rpgmodule singleton accessors (KCD2 WHGame.dll 1.5.6 RVAs, verified in kd7u).
// S_RpgParamDefs / S_Constants live in S_RpgParams.cpp.

wh::rpgmodule::C_RPGModule* wh::rpgmodule::C_RPGModule::GetInstance() {
    // qword_1853322A0: written by the ctor sub_180BE8EEC, read by the soul ability
    // worker sub_1809DCC70 (all-abilities cvar) and the script-bind soul resolver.
    static REL::Relocation<C_RPGModule**> p{ REL::ID(2349) };
    return *p;
}

wh::rpgmodule::C_BuffManager* wh::rpgmodule::C_BuffManager::GetInstance() {
    // sub_180E820D4: mutex-guarded lazy singleton (instance qword_18548E360,
    // alloc 0x68 + ctor sub_180E82180). Forward so first-call init runs.
    using Fn = C_BuffManager* (__fastcall*)();
    static REL::Relocation<Fn> fn{ REL::ID(38) };
    return fn();
}

wh::rpgmodule::C_RPGLocationManager* wh::rpgmodule::C_RPGLocationManager::GetInstance() {
    // Owned by C_RPGModule @+0xB8 (the module reads it as qword_1853322A0 + 184 throughout).
    C_RPGModule* rpg = C_RPGModule::GetInstance();
    return rpg ? rpg->m_pLocationManager : nullptr;
}
