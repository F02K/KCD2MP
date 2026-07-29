#pragma once
#include <cstdint>
#include "../framework/WUID.h"
#include "C_PuppetAdapter.h"

// -----------------------------------------------
// wh::xgenaimodule::movement::C_FormationSpinePointPuppet : C_PuppetAdapter :
// I_AIPuppet -- pooled formation spine-point anchor (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x48 (create-site proven: tracked alloc 72 inside sub_1832A5628).
// NEW class -- absent from the original G2 scope; the ONLY xgenaimodule class in the
// nested `movement` namespace found so far.
// -----------------------------------------------
// RTTI TD via g2 sweep; vtable 0x183FE5DE8 (rva 0x3FE5DE8). Ctor inlined in
// sub_1832A5628: writes the C_PuppetAdapter vtable then own; generated WUID =
// (gen | 0x1300000000000000) -- tag 0x13 marks spine points. Instances are pooled
// per-float-key (spacing) with a refcount in the owner's vector. The vtable content
// was NOT dumped: the overrides below mirror the I_AIPuppet pure set (the class is
// concrete in the binary so all pures are implemented); per-slot impl addresses [U].

namespace wh::xgenaimodule::movement {

class C_FormationSpine;   // owning spine (pools puppets per spacing key)

class C_FormationSpinePointPuppet : public C_PuppetAdapter {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FormationSpinePointPuppet;
    ~C_FormationSpinePointPuppet() override;
    void _vf1() override;
    void _vf2() override;
    void* _vf3() override;
    void _vf4() override;
    void UpdateCachedTransform() override;
    void _vf6() override;
    void _vf7() override;
    void _vf8() override;
    void _vf9() override;
    void _vf10() override;
    void _vf11() override;
    void _vf12() override;
    void _vf13() override;
    void _vf14() override;
    void _vf16() override;
    void _vf17() override;
    void _vf18() override;
    void _vf19() override;
    void _vf20() override;

    framework::WUID m_wuid;         // +0x08  generated: (gen | 0x1300000000000000)
    Vec3            m_cachedPoint;      // +0x10  cached spine point at m_spacing (sub_1832A524C: from sub_1832A15A4)
    Vec3            m_cachedDir;        // +0x1C  cached normalized tangent/dir (sub_1832A524C: from sub_1832A123C)
    Vec3            m_cachedScaledDir;  // +0x28  cached m_cachedDir * scalar sub_1832A3490 (sub_1832A524C)
    bool            m_transformCached;  // +0x34  cache-valid flag: ctor 0, set 1 by sub_1832A524C after filling m_cached* (gate @0x1832A525B)
    uint8_t         _pad35[3];      // +0x35
    C_FormationSpine* m_owner;      // +0x38  owning spine; pools this puppet per m_spacing (release sub_1832A55DC, cache src sub_1832A524C)
    float           m_spacing;      // +0x40  pool key
    uint8_t         _pad44[4];      // +0x44
};
static_assert(sizeof(C_FormationSpinePointPuppet) == 0x48, "C_FormationSpinePointPuppet must be 0x48");

}  // namespace wh::xgenaimodule::movement
