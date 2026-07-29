#include "combatmodule/S_CombatSettings.h"
#include "REL/ID.h"

// KCD2 WHGame.dll 1.5.6 (kd7u).
//
// g_combatSettings @ 0x1855D4D70 (RVA 0x55D4D70, symbol unk_1855D4D70) is the embedded
// wh_cs_* CVar-mirror struct itself -- NOT a pointer. Every field is written as an ICVar
// default by the registration function sub_1810A8DA0 (0x6933 bytes), which is called from
// C_CombatModule::Init (sub_180D243C0) with base = sub_1809D5994(), the TLS-guarded
// function-static accessor that returns &g_combatSettings. sub_182778CA0 (PostInit teardown)
// only UnregisterVariable()s the same names -- it never writes storage.
namespace wh::combatmodule {

S_CombatSettings* S_CombatSettings::GetInstance() {
    return reinterpret_cast<S_CombatSettings*>(REL::ID(2358).address());
}

}  // namespace wh::combatmodule
