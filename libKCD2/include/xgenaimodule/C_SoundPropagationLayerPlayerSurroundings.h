#pragma once
#include "C_SoundPropagationLayerBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerPlayerSurroundings :
// C_SoundPropagationLayerBase -- player-surroundings attenuation layer (cvar
// wh_ai_PlayerSurroundingsAttenuation, HearingSystem+0x18).  KCD2 WHGame.dll
// 1.5.6, kd7u.  sizeof 0x08 (create-site proven: alloc 8 in the C_HearingSystem
// ctor).
// -----------------------------------------------
// RTTI TD rva 0x4F48EE0; vtable 0x183FE68C0, 3 slots: [0]/[1] base values,
// [2] sub_180813264 (query/update).

namespace wh::xgenaimodule {

class C_SoundPropagationLayerPlayerSurroundings : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerPlayerSurroundings;
    void _vf2() override;   // [2] sub_180813264 -- query/update [U exact signature]
};
static_assert(sizeof(C_SoundPropagationLayerPlayerSurroundings) == 0x08, "alloc 8");

}  // namespace wh::xgenaimodule
