#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"
#include "../framework/C_ParallelModuleUpdater.h"
#include "../game/S_GameContext.h"

// -----------------------------------------------
// wh::environmentmodule::C_EnvironmentModule -- environment module spine
// (KCD2 1.5.6, kd7u).  sizeof 0xB0 (alloc 176).
// -----------------------------------------------
// Vtables 0x183A840E8 (+0x00, 11 slots: 7 base + RTTR trio) / 0x183A84150 (+0x10,
// C_ParallelModuleUpdater / IGameFrameworkListener). Ctor sub_180BE889C (updater subctor
// sub_180BE9170); registrar sub_180C9DA44 -> S_GameContext +0x138. Module vtable slots:
// [1] OnModuleMessage sub_180E3A9B8, [2] Init sub_180E3C398 (builds the five subsystems
// below + registers C_FunctionIsRaining + sub_180EF3AFC(updater, 11)), [3] Deinit
// sub_182ADA658, [4] Update sub_180533158, [5] GetModuleId = 4 (sub_181A72500),
// [6] GetModuleName = "EnvironmentModule" (sub_181A71AB0). The concrete vtable also carries
// a GetWeatherSystem accessor (sub_180535E94, returns +0x98) -- whether it sits at [7] or
// [8] relative to the RTTR trio is UNRESOLVED, so it is mirrored as the plain inline
// accessor below instead of a virtual.

namespace wh::environmentmodule {

class C_RespawnManager;
class C_ScriptBindEnvironment;
class C_VisualEffectManager;
class C_WeatherSystem;
class C_StreamingHelper;

class C_EnvironmentModule : public framework::C_BaseModule,
                            public framework::C_ParallelModuleUpdater {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EnvironmentModule;
    C_RespawnManager*        m_pRespawnManager;       // +0x80  Init alloc 456
    C_ScriptBindEnvironment* m_pScriptBind;           // +0x88  Init alloc 112 (Lua "EnvironmentModule")
    C_VisualEffectManager*   m_pVisualEffectManager;  // +0x90  Init alloc 24
    C_WeatherSystem*         m_pWeatherSystem;        // +0x98  Init alloc 6448
    C_StreamingHelper*       m_pStreamingHelper;      // +0xA0  Init alloc 40
    uint64_t                 m_unkA8;                 // +0xA8  no writer in ctor 0x180BE889C / Init 0x180E3C398 / Deinit 0x182ADA658 or any module vtable method+callee (scanned); reserved or externally-set

    [[nodiscard]] static C_EnvironmentModule* GetInstance()
    {
        return game::S_GameContext::GetInstance()->m_pEnvironmentModule;
    }

    // Mirrors the concrete-vtable accessor sub_180535E94 (see header note).
    [[nodiscard]] C_WeatherSystem* GetWeatherSystem() const { return m_pWeatherSystem; }
};
static_assert(sizeof(C_EnvironmentModule) == 0xB0, "C_EnvironmentModule must be 0xB0 (alloc 176)");
static_assert(offsetof(C_EnvironmentModule, m_pWeatherSystem) == 0x98, "weather system at +0x98 (sub_180535E94)");

}  // namespace wh::environmentmodule
