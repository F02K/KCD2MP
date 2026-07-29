#pragma once
#include <cstdint>
#include "../framework/WUID.h"
#include "C_CachedPuppet.h"

struct IGameObject;

// -----------------------------------------------
// wh::xgenaimodule::C_AIPuppet : C_CachedPuppet : I_AIPuppet -- the full puppet
// implementation held by NPCs (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x48
// (create-site proven: alloc 72 in both NPC factories).
// -----------------------------------------------
// RTTI TD rva 0x4FF5D80; vtable 0x183A399A8, 25 slots (full impl). Two real ctors:
//   sub_18047E4C8(this, IGameObject* host, const WUID*)              -- main (NPC factories)
//   sub_180A2A458(this, IGameObject* host, const WUID*, bool static) -- C_SOPuppet path;
//     staticFlag = !entity("bMovingSmartObject")
// Both inline the C_CachedPuppet base ctor (write vtable 0x183A398D8 then own).
// Registers itself in the puppet registry q_185496338 via sub_18047E598, keyed by
// _vf3()->vf[2]() (host-record id). Slot [5] (sub_180647EA4) reads the host entity
// (*+0x38) via GetPos(vf+0x170)/GetRot(vf+0x188) and refreshes the cached transform,
// clearing a 0x400000 dirty flag (G2_dossier §6).

namespace wh::xgenaimodule {

class C_AIPuppet : public C_CachedPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIPuppet;
    ~C_AIPuppet() override;                 // [0]  deleting dtor sub_18047B120 -> dtor sub_18047B094
    void _vf1() override;                   // [1]  sub_180479B40
    void _vf2() override;                   // [2]  sub_180602360
    void* _vf3() override;                  // [3]  sub_180602360 (COMDAT-folded with [2]) -> host record
    void _vf4() override;                   // [4]  sub_181A749E0
    void UpdateCachedTransform() override;  // [5]  sub_180647EA4 [V]
    void _vf6() override;                   // [6]  sub_180761604
    void _vf7() override;                   // [7]  sub_180D44DC8
    void _vf8() override;                   // [8]  sub_183450898
    void _vf9() override;                   // [9]  sub_18041B838
    void _vf10() override;                  // [10] sub_1803E5410
    void _vf11() override;                  // [11] sub_1803E3B40
    void _vf12() override;                  // [12] sub_180813048
    void _vf13() override;                  // [13] sub_18081351C
    void _vf14() override;                  // [14] sub_18087780C
    void _vf15() override;                  // [15] sub_180732A14 (vs default sub_181AA5630)
    void _vf16() override;                  // [16] sub_1807E7A94
    void _vf17() override;                  // [17] sub_18081362C
    void _vf18() override;                  // [18] sub_180953E58
    void _vf19() override;                  // [19] sub_183451C70
    void _vf20() override;                  // [20] sub_1807FE0FC
    void* _vf21() override;                 // [21] sub_181A8B5A0 (vs return-0 default)
    // [22] return-1 / [23] nullsub / [24] sub_180F8CC10 defaults inherited unchanged.

    uint8_t         m_cachedTransform[0x18];  // +0x08..+0x1F  cached pos @+0x08 (Vec3), rot region follows -- exact split UNVERIFIED (not touched by ctor)
    CryStringT<char> m_name;                  // +0x20  CryString name (ctor = shared empty atom sub_1804FD80C()+0xC); base dtor decrefs via sub_1804FD898(ptr-12) [U exact role; empty at ctor]
    uint32_t        m_dirtyFlags;             // +0x28  transform dirty flags (0x400000 = cached transform stale)
    bool            m_staticTransform;        // +0x2C  true = non-moving smart object (4-arg ctor only)
    uint8_t         _pad2D[3];                // +0x2D
    framework::WUID m_wuid;                   // +0x30
    IGameObject*    m_hostGameObject;         // +0x38  host game object [V]
    entitymodule::C_Actor* m_hostActor;       // +0x40  = m_pActorSystem->GetActor(m_hostGameObject->GetId()) (S_GameContext+0x180, slot 3); the host's IActor
};
static_assert(sizeof(C_AIPuppet) == 0x48, "C_AIPuppet must be 0x48");

}  // namespace wh::xgenaimodule
