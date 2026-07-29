#pragma once
#include <cstdint>
#include "C_SoundPropagationLayerBase.h"

namespace wh::rpgmodule { class C_RPGHearing; }

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerTargetRPG : C_SoundPropagationLayerBase
// -- target RPG-stat modifier layer of the sound grid (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x10 (create-site proven: alloc 16 in the C_HearingSystem ctor).
// -----------------------------------------------
// RTTI TD rva 0x4F48F30; vtable 0x183FE68E0, 3 slots: [0] sub_1832A88EC,
// [1] sub_181A6DD38, [2] sub_180EB7C94 (query/update).

namespace wh::xgenaimodule {

class C_SoundPropagationLayerTargetRPG : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerTargetRPG;
    void* _vf0() override;   // [0] sub_1832A88EC [U role]
    void _vf1() override;    // [1] sub_181A6DD38 [U role]
    void _vf2() override;    // [2] sub_180EB7C94 -- query/update [U exact signature]

    wh::rpgmodule::C_RPGHearing* m_soundCategoryRegistry;  // +0x08  &C_RPGHearing global (0x18492EC80), cached by vf[1] sub_181A6DD38
};
static_assert(sizeof(C_SoundPropagationLayerTargetRPG) == 0x10, "alloc 16");

}  // namespace wh::xgenaimodule
