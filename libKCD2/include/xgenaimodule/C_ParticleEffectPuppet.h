#pragma once
#include "C_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ParticleEffectPuppet : C_AIPuppet -- puppet for particle-effect
// hosts (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x48 (create-site proven: tracked
// alloc 72 inside sub_181366760 = the C_ParticleEffect host ctor).
// -----------------------------------------------
// RTTI TD rva 0x4FF5D40; vtable 0x183B67F28, 25 slots -- byte-identical to the
// C_AIPuppet vtable except [0] (own deleting dtor sub_18136668C). NO extra members.
// Built via base ctor sub_18047E4C8 + leaf vtable overwrite, then hashmap-registered
// in q_1854961A8 (node+0x18 = puppet).

namespace wh::xgenaimodule {

class C_ParticleEffectPuppet : public C_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ParticleEffectPuppet;
    ~C_ParticleEffectPuppet() override;   // [0] sub_18136668C; slots [1..24] inherited unchanged
};
static_assert(sizeof(C_ParticleEffectPuppet) == 0x48, "C_ParticleEffectPuppet adds no members to C_AIPuppet");

}  // namespace wh::xgenaimodule
