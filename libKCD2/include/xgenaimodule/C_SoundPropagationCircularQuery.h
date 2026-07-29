#pragma once
#include <cstdint>
#include "C_SoundPropagationLayerBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationCircularQuery : C_SoundPropagationLayerBase
// -- the radial sound-event propagation writer (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x18 (create-site proven: alloc 24 in the C_HearingSystem ctor; +0x10 =
// back-ptr to the owning system).
// -----------------------------------------------
// RTTI TD rva 0x4F48E40; vtable 0x183FE66F8, 3 slots: [0] sub_1832A8894,
// [1] sub_1819EDDE8, [2] sub_181036584.
// [2] query sub_181036584(this, evt, phase):
//   power  = m_hearingSystem->m_distanceAttenuationPower (HS+0x0C);
//   base   = (evt+0x18 flag) ? 0.1 : 0.01;
//   radius = powf(*(float*)(evt+0x14) / base, 1.0f / power);   // evt+0x14 = intensity
//   writes propagated intensity into grid cells around evt (+0x08/+0x0C = cell
//   coords) via sub_1805CF744 / sub_182082D00.
// => huge attenuation power or zero event intensity collapses radius to ~0 (mute
//    lever; see C_HearingSystem banner).

namespace wh::xgenaimodule {

class C_HearingSystem;

class C_SoundPropagationCircularQuery : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationCircularQuery;
    void* _vf0() override;   // [0] sub_1832A8894 [U role]
    void _vf1() override;    // [1] sub_1819EDDE8 [U role]
    void _vf2() override;    // [2] sub_181036584 -- radial propagation (see banner) [U exact signature]

    void*           m_unk08;          // +0x08  sound-grid target the query writes cells into (read at 0x181036639, passed as the base handle to sub_1805CF744); set post-construction, not by the alloc site [U grid class]
    C_HearingSystem* m_hearingSystem; // +0x10  back-ptr to the owning system (ctor arg) [V]
};
static_assert(sizeof(C_SoundPropagationCircularQuery) == 0x18, "alloc 24");

}  // namespace wh::xgenaimodule
