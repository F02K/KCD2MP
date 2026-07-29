#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::environmentmodule::C_ScriptBindEnvironment -- Lua "EnvironmentModule" bind
// (KCD2 1.5.6, kd7u).  sizeof 0x70 (alloc 112).
// -----------------------------------------------
// Own vtable 0x183AB8EB8; ctor sub_180E3C27C; owned by C_EnvironmentModule @+0x88. Registers
// global table "EnvironmentModule" (SetGlobalName sub_180B85140) and its methods via
// sub_18181EACC. TOP env cheat entry point -- Lua handlers:
//   MakeHole(make, entityId)          sub_182ADE480   terrain hole punch
//   CoverHole(cover, entityId)        sub_182ADD3C8
//   (third method, name via sub_18181EC6C)  sub_182ADD018
//   RebuildClouds()                   sub_182ADEA74
//   ForceImmediateWeatherUpdate()     binder sub_180C820A0 -> IWeatherSystem slot [11]
//   GetRainIntensity() -> float       binder sub_18197C070 -> IWeatherSystem slot [9]

namespace wh::environmentmodule {

class C_ScriptBindEnvironment : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindEnvironment;
    Offsets::IScriptSystem* m_pScriptSystem;   // +0x60  GameContext+8 -> vfunc+152 result
                                               //        [duplicate of base m_pSS role, UNVERIFIED]
    Offsets::IScriptTable*  m_pTable68;        // +0x68  table from the shared Init sub_18144CE0C
};
static_assert(sizeof(C_ScriptBindEnvironment) == 0x70, "C_ScriptBindEnvironment must be 0x70 (alloc 112)");

}  // namespace wh::environmentmodule
