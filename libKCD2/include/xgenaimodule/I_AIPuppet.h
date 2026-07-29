#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_AIPuppet -- the 25-slot puppet interface, root of the puppet
// hierarchy (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (abstract).
// -----------------------------------------------
// RTTI TD rva 0x4F48350; standalone vtable 0x183AF07F0. Puppets are a SEPARATE
// hierarchy from the C_AIObject spine (puppets do NOT derive C_AIObject); every
// C_AIObject instead HOLDS an I_AIPuppet* at +0x18 and forwards its slots [2..5] into
// puppet slots [5]/[14]/[17]/[19] (G2_supplement §7.1). Tree:
//   I_AIPuppet
//   +- C_CachedPuppet -> C_AIPuppet -> {C_SOPuppet, C_ParticleEffectPuppet}
//   +- C_GeneratedSOPuppet
//   +- C_PuppetAdapter -> {C_FormationAnchorPuppet, movement::C_FormationSpinePointPuppet}
//   +- C_PerceptibleVolumePuppet / C_InventoryPuppet / C_ItemPuppet
// Standalone vtable: [0] deleting dtor sub_1832A0ADC; [1..14],[16..20] _purecall;
// defaults [15] sub_181AA5630, [21] sub_18066CD10 (return 0), [22] sub_18041A6A0
// (return 1), [23]/[24] nullsub_1. Slot roles beyond [3]/[5] UNRESOLVED.

namespace wh::xgenaimodule {

class I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AIPuppet;
    virtual ~I_AIPuppet();                     // [0]  deleting dtor sub_1832A0ADC
    virtual void _vf1() = 0;                   // [1]  [U role]
    virtual void _vf2() = 0;                   // [2]  [U role]
    virtual void* _vf3() = 0;                  // [3]  returns per-entity host record (record->vf[2]() = host id, keys the puppet registry q_185496338) [U name]
    virtual void _vf4() = 0;                   // [4]  [U role]
    virtual void UpdateCachedTransform() = 0;  // [5]  C_AIPuppet impl caches host pos/rot; C_AIObject slot [2] forwards here [V]
    virtual void _vf6() = 0;                   // [6]  [U role]
    virtual void _vf7() = 0;                   // [7]  [U role]
    virtual void _vf8() = 0;                   // [8]  [U role]
    virtual void _vf9() = 0;                   // [9]  [U role]
    virtual void _vf10() = 0;                  // [10] [U role]
    virtual void _vf11() = 0;                  // [11] [U role]
    virtual void _vf12() = 0;                  // [12] [U role]
    virtual void _vf13() = 0;                  // [13] [U role]
    virtual void _vf14() = 0;                  // [14] C_AIObject slot [3] forwards here [U role]
    virtual void _vf15();                      // [15] default impl sub_181AA5630 [U role]
    virtual void _vf16() = 0;                  // [16] [U role]
    virtual void _vf17() = 0;                  // [17] C_AIObject slot [4] forwards here [U role]
    virtual void _vf18() = 0;                  // [18] [U role]
    virtual void _vf19() = 0;                  // [19] C_AIObject slot [5] forwards here [U role]
    virtual void _vf20() = 0;                  // [20] [U role]
    virtual void* _vf21();                     // [21] default sub_18066CD10 -- return 0
    virtual bool _vf22();                      // [22] default sub_18041A6A0 -- return 1
    virtual void _vf23();                      // [23] default nullsub_1
    virtual void _vf24();                      // [24] default nullsub_1
};
static_assert(sizeof(I_AIPuppet) == 0x08, "I_AIPuppet is vptr-only");

}  // namespace wh::xgenaimodule
