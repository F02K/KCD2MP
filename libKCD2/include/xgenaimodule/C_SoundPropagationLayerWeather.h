#pragma once
#include <cstdint>
#include "C_SoundPropagationLayerBase.h"

namespace wh::rpgmodule { class C_RPGHearing; }

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerWeather : C_SoundPropagationLayerBase
// -- weather attenuation layer of the sound grid (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x10 (create-site proven: alloc 16 in the C_HearingSystem ctor).
// -----------------------------------------------
// RTTI TD rva 0x4F48DF0; vtable 0x183FE6920, 3 slots: [0] sub_1832A88EC,
// [1] sub_181A6DD38, [2] sub_1810494EC (query/update).

namespace wh::xgenaimodule {

class C_SoundPropagationLayerWeather : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerWeather;
    void* _vf0() override;   // [0] sub_1832A88EC [U role]
    void _vf1() override;    // [1] sub_181A6DD38 [U role]
    void _vf2() override;    // [2] sub_1810494EC -- query/update [U exact signature]

    wh::rpgmodule::C_RPGHearing* m_soundCategoryRegistry;  // +0x08  &C_RPGHearing global (0x18492EC80), cached by vf[1] sub_181A6DD38
};
static_assert(sizeof(C_SoundPropagationLayerWeather) == 0x10, "alloc 16");

}  // namespace wh::xgenaimodule
