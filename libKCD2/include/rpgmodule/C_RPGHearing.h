#pragma once
#include "I_RPGHearing.h"

// -----------------------------------------------
// wh::rpgmodule::C_RPGHearing : I_RPGHearing -- stateless RPG hearing/attenuation
// service (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable-only: the global
// sub-object @ 0x18492EC80 has the C_RPGHearing vtable at +0x00 and the
// C_RPGInventory vtable at +0x08 -- a packed composite of the RPG services, so
// this class carries no own data) [size UNPROVEN by alloc -- inferred from the
// packed layout].
// -----------------------------------------------
// RTTI TD rva 0x4D5F818; COL 0x1843EA2B8; vtable 0x40AED70 (6 slots 0..5 --
// ends at the C_RPGInventory COL).  Slot impls:
//  [0] sub_180A4BF54 GetHearingConfigById -- array {base 0x18532F638, end
//      0x18532F640, idBase 0x18532F650}; "<invalid>" default
//  [1] sub_1816EE7E8 DispatchListeners
//  [2] sub_18104951C weather atten: (sqrt(clamp(1-w)))*(1-Cw)+Cw, Cw=0x8F8
//  [3] sub_180EB7CF8 ability atten: (sqrt(prop5))*(1-Ch)+Ch, Ch=0x8FC
//  [4] sub_18146904C coef provider (params 0x900/0x904/0x908) [U role]
//  [5] sub_180B705E8 bush concealment (mat_bushes; prop30 base x prop94 mod)
// Consumers hold C_RPGHearing* pointing at 0x18492EC80: C_HearingSystem+0x50,
// C_SoundPropagationLayerWeather/TargetRPG +0x08.

namespace wh::rpgmodule {

class C_RPGHearing : public I_RPGHearing {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGHearing;
    const void* GetHearingConfigById(int id) override;                 // [0] sub_180A4BF54
    void  DispatchListeners(void* a2) override;                        // [1] sub_1816EE7E8
    float GetWeatherAttenuation(float sound, float weather) override;  // [2] sub_18104951C
    float GetListenerAbilityAttenuation(float sound, void* soul) override;  // [3] sub_180EB7CF8
    float GetAttenuationCoef4(void* a2) override;                      // [4] sub_18146904C [U role]
    float GetBushConcealment(void* soul, const void* surface) override;// [5] sub_180B705E8
};
static_assert(sizeof(C_RPGHearing) == 0x08, "C_RPGHearing is stateless (vptr only)");

}  // namespace wh::rpgmodule
