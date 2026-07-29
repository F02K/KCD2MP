#pragma once
#include "C_SoundPropagationLayerBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerAddIntensities :
// C_SoundPropagationLayerBase -- intensity-summing layer of the sound grid (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (create-site proven: alloc 8 in the
// C_HearingSystem ctor).
// -----------------------------------------------
// RTTI TD rva 0x4F48E90; vtable 0x183FE68A0, 3 slots: [0]/[1] base values,
// [2] sub_1808B0418 (query/update).

namespace wh::xgenaimodule {

class C_SoundPropagationLayerAddIntensities : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerAddIntensities;
    void _vf2() override;   // [2] sub_1808B0418 -- query/update [U exact signature]
};
static_assert(sizeof(C_SoundPropagationLayerAddIntensities) == 0x08, "alloc 8");

}  // namespace wh::xgenaimodule
