#pragma once
#include "C_SoundPropagationLayerBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerNoisiness : C_SoundPropagationLayerBase
// -- ambient-noisiness layer of the sound grid (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08 (create-site proven: alloc 8 in the C_HearingSystem ctor).
// -----------------------------------------------
// RTTI TD rva 0x4F48DA0; vtable 0x183FE6940, 3 slots: [0]/[1] base values,
// [2] sub_1808AEB94 (query/update).

namespace wh::xgenaimodule {

class C_SoundPropagationLayerNoisiness : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerNoisiness;
    void _vf2() override;   // [2] sub_1808AEB94 -- query/update [U exact signature]
};
static_assert(sizeof(C_SoundPropagationLayerNoisiness) == 0x08, "alloc 8");

}  // namespace wh::xgenaimodule
