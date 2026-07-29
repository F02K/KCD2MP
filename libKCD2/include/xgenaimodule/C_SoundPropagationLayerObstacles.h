#pragma once
#include <cstdint>
#include "C_SoundPropagationLayerBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerObstacles : C_SoundPropagationLayerBase
// -- obstacle attenuation layer of the sound grid (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x10 (create-site proven: alloc 16 in the C_HearingSystem ctor
// sub_180A4B310).
// -----------------------------------------------
// RTTI TD rva 0x4F48D50; vtable 0x183FE6900, 3 slots: [0] sub_1832A88EC,
// [1] sub_1819ED384, [2] sub_180801C54 (query/update).

namespace wh::xgenaimodule {

class C_SoundPropagationLayerObstacles : public C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerObstacles;
    void* _vf0() override;   // [0] sub_1832A88EC [U role]
    void _vf1() override;    // [1] sub_1819ED384 [U role]
    void _vf2() override;    // [2] sub_180801C54 -- query/update [U exact signature]

    uint64_t m_unk08;        // +0x08 [U role]
};
static_assert(sizeof(C_SoundPropagationLayerObstacles) == 0x10, "alloc 16");

}  // namespace wh::xgenaimodule
