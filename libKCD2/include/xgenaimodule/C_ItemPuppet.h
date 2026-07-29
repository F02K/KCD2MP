#pragma once
#include <cstdint>
#include "I_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ItemPuppet : I_AIPuppet -- puppet for AI items (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x20 (create-site proven: tracked alloc 32 inside
// sub_18047BB68, reached via callback forwarder sub_18047BAE0).
// -----------------------------------------------
// RTTI TD rva 0x4FF5DF0; vtable 0x183A39730, 25 slots. Ctor inlined: {vptr, item*,
// 12 zeroed bytes @+0x10}. Created only if Find(q_185481FC8, item+0x30) misses or the
// ownable checked-cast sub_1805D2624 fails; the same site then builds the C_AIItem
// (0x78, ctor sub_18047BAEC).
// Overrides: [0] dtor sub_18100844C; [1] sub_18082D008; [2]/[3] sub_18082B7B8 (shared
// body); [4] sub_181AABEE0; [5] sub_18082B74C; [6] sub_183447398; [7] sub_183447274;
// [8] sub_183447118; [9] sub_1834474FC; [10] sub_1834471A0; [11] sub_1834471E8;
// [12] sub_18344758C; [13] sub_1816BCBE8; [14] sub_1834478A8; [16] sub_183447638;
// [17..20] nullsub_1. Defaults [15]/[21..24] inherited unchanged.

namespace wh::xgenaimodule {

class C_AIItem;   // 0x78 host (G6 wave; ctor sub_18047BAEC)

class C_ItemPuppet : public I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemPuppet;
    ~C_ItemPuppet() override;
    void _vf1() override;
    void _vf2() override;
    void* _vf3() override;
    void _vf4() override;
    void UpdateCachedTransform() override;   // [5] sub_18082B74C
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

    C_AIItem* m_item;         // +0x08  host item [V]
    Vec3      m_cachedPos;    // +0x10  UpdateCachedTransform [5] caches host translation (host vf[46])
    uint8_t   _pad1C[4];      // +0x1C
};
static_assert(sizeof(C_ItemPuppet) == 0x20, "C_ItemPuppet must be 0x20");

}  // namespace wh::xgenaimodule
