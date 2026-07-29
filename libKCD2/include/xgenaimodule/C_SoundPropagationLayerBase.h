#pragma once

// -----------------------------------------------
// wh::xgenaimodule::C_SoundPropagationLayerBase -- base of the 8 sound-propagation
// grid layers owned by C_HearingSystem (m_layers vector @+0x58).  KCD2 WHGame.dll
// 1.5.6, kd7u.  ABSTRACT ([2] _purecall); sizeof 0x08 (layers add at most 2
// qwords).
// -----------------------------------------------
// RTTI TD rva 0x4F48F80; vtable 0x183FE6868, 3 slots: [0] sub_1832A88C0 (trivial
// getter), [1] nullsub_1, [2] _purecall (the per-layer grid query/update).  All 7
// concrete layers keep the 3-slot shape.

namespace wh::xgenaimodule {

class C_SoundPropagationLayerBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoundPropagationLayerBase;
    virtual void* _vf0();       // [0] sub_1832A88C0 -- trivial getter [U role; dtor placement unverified]
    virtual void _vf1();        // [1] nullsub_1 [U role]
    virtual void _vf2() = 0;    // [2] _purecall here -- per-layer grid query/update [U exact signature]
};
static_assert(sizeof(C_SoundPropagationLayerBase) == 0x08, "layer base is vtable-only");

}  // namespace wh::xgenaimodule
