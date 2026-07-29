#include "KCSE/KCSEAPI.h"
#include "rpgmodule/S_RpgParams.h"
#include "crysystem/SSystemGlobalEnvironment.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

using namespace wh::rpgmodule;

static std::vector<std::string> g_cvarNames;

// A small int stored in a float slot has a zero exponent field; use that to decide
// whether to expose the RPG param as an int or float CVar.
static bool IsLikelyInt(float val)
{
    if (val == 0.0f)
        return false;
    uint32_t bits;
    std::memcpy(&bits, &val, 4);
    uint32_t exp = (bits >> 23) & 0xFF;
    return exp == 0;
}

KCSE_PLUGIN_INFO("Console RPG Params", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type != KCSE::IMessagingInterface::kMessage_DataLoaded)
            return;

        auto* con  = SSystemGlobalEnvironment::GetInstance()->pConsole;
        auto* log  = SSystemGlobalEnvironment::GetInstance()->pLog;
        auto* defs = S_RpgParamDefs::Get();

        g_cvarNames.reserve(S_RpgParamDefs::kParamCount);

        int nFloat = 0, nInt = 0;
        for (auto& def : *defs) {
            if (!def.name || !def.name[0])
                continue;

            g_cvarNames.emplace_back(std::string("kcse_rpgparam_") + def.name);
            const char* help = def.description ? def.description : "";

            S_RPGValue& slot = def.Value();   // reference into the live value store
            if (IsLikelyInt(slot.asFloat)) {
                con->RegisterCVarInt(g_cvarNames.back().c_str(), &slot.asInt, slot.asInt, VF_NULL, help);
                ++nInt;
            } else {
                con->RegisterCVarFloat(g_cvarNames.back().c_str(), &slot.asFloat, slot.asFloat, VF_NULL, help);
                ++nFloat;
            }
        }

        log->LogAlways("[ConsoleRpgParam] Registered %d float + %d int RPG params as CVars", nFloat, nInt);
    });

    return true;
}
