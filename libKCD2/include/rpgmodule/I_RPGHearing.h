#pragma once

// -----------------------------------------------
// wh::rpgmodule::I_RPGHearing -- interface of the RPG hearing/attenuation service
// (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface, 6 slots, merged into the
// C_RPGHearing primary vtable (no standalone COL).
// -----------------------------------------------
// RTTI TD rva 0x4D60410.  Sole impl: C_RPGHearing (sub-object @ 0x18492EC80).

namespace wh::rpgmodule {

class I_RPGHearing {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RPGHearing;
    virtual const void* GetHearingConfigById(int id) = 0;                 // [0] global config-array lookup
    virtual void  DispatchListeners(void* a2) = 0;                        // [1] iterate listener functors
    virtual float GetWeatherAttenuation(float sound, float weather) = 0;  // [2] MaxWeatherSoundAttenuationCoef
    virtual float GetListenerAbilityAttenuation(float sound, void* soul) = 0;  // [3] soul prop 5, MaxHearingSoundAttenuationCoef
    virtual float GetAttenuationCoef4(void* a2) = 0;                      // [4] lerp params 0x904/0x908 by player props 14/73 [U role]
    virtual float GetBushConcealment(void* soul, const void* surface) = 0;// [5] mat_bushes: prop30 * prop94
};
static_assert(sizeof(I_RPGHearing) == 0x08, "I_RPGHearing is vptr-only");

}  // namespace wh::rpgmodule
