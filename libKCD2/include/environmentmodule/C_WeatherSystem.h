#pragma once
#include <cstdint>
#include "IWeatherSystem.h"
#include "../framework/CryDeferrable.h"

// -----------------------------------------------
// wh::environmentmodule::C_WeatherSystem -- weather / time-of-day engine
// (KCD2 1.5.6, kd7u).  sizeof 0x1930 (alloc 6448).
// -----------------------------------------------
// Vtables 0x183AB8D20 (+0x00, IWeatherSystem impl, 20 slots) / second vtable @+0x08 (RTTI:
// CryDeferrable<0>+ICryDeferrable base, mdisp 0x08 -- the stateful polymorphic deferrable,
// modeled as UnsafeOp::CryDeferrableSlot per the C_Soul precedent). Ctor sub_180E3BBC0;
// owned by C_EnvironmentModule @+0x98 (vtable accessor sub_180535E94). Registers cvar
// wh_env_moon; data cvars wh_env_debugToTOD / wh_env_RandomPresetDelay.
// The interior is a ~0x1900 weather/TOD state machine and was only anchor-walked -- the
// blocks below mark the ctor-visible subobject starts, not field-accurate layouts.
// Resolved levers (via the IWeatherSystem base): GetRainIntensity [9] 0x180535EC0,
// ForceImmediateWeatherUpdate [11] 0x180552724, AddSpatialWeatherModifier [18] 0x180A4A028
// (inserts into the +0x18F0 container).

namespace wh::environmentmodule {

class C_WeatherSystem : public IWeatherSystem, public UnsafeOp::CryDeferrableSlot {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeatherSystem;
    uint8_t m_state18[0x13C0];      // +0x18..+0x13D7   weather-state block (sub_180E3BCB4; not walked)
    float   m_param13D8;            // +0x13D8  ctor -1.0f [role UNVERIFIED]
    uint8_t _pad13DC[4];            // +0x13DC
    uint8_t m_sub13E0[0x1F0];       // +0x13E0  sub_1810A702C subobject (not walked)
    uint8_t m_sub15D0[0x28];        // +0x15D0  sub_180E3BD64 subobject (not walked)
    uint8_t m_sub15F8[0x2A8];       // +0x15F8  sub_180E3BA40 subobject (not walked)
    uint8_t m_handle18A0[0x28];     // +0x18A0  handle + zeroed tail (not walked)
    float   m_target18C8;           // +0x18C8  ctor -1.0f  [wind/time target candidates,
    uint8_t _pad18CC[4];            // +0x18CC                roles UNVERIFIED]
    float   m_target18D0;           // +0x18D0  ctor -1.0f
    float   m_target18D4;           // +0x18D4  ctor -1.0f
    float   m_target18D8;           // +0x18D8  ctor -1.0f
    uint8_t _pad18DC[4];            // +0x18DC
    float   m_target18E0;           // +0x18E0  ctor -1.0f
    float   m_smoothValue18E4;      // +0x18E4  SmoothDamp current value (writer 0x180552705 movss; ctor 0)
    float   m_smoothVelocity18E8;   // +0x18E8  SmoothDamp velocity state (writer 0x1805526FD movss; ctor 0)
    bool    m_flag18EC;             // +0x18EC  bool: set 1 @0x180E3AA26, cleared 0 @0x180E3B90D, tested @0x180552C66
    uint8_t _pad18ED[3];            // +0x18ED  align to +0x18F0
    uint8_t m_spatialModifiers[0x40];   // +0x18F0  spatial weather-modifier container
                                        //          (sub_180BBB344; slot [18] insert target)
};
static_assert(sizeof(C_WeatherSystem) == 0x1930, "C_WeatherSystem must be 0x1930 (alloc 6448)");
static_assert(offsetof(C_WeatherSystem, m_spatialModifiers) == 0x18F0, "spatial-modifier container at +0x18F0");

}  // namespace wh::environmentmodule
