#pragma once
#include "../framework/WUID.h"
#include "I_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_InventoryPuppet : I_AIPuppet -- puppet for AI inventories
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (create-site proven: tracked alloc 24
// inside sub_180A308D4 = C_AIInventoryManager vtable slot 0, which builds the
// C_InventoryPuppet + C_AIInventory (0xE8) pair).
// -----------------------------------------------
// RTTI TD rva 0x4FF5DB8; vtable 0x1840143B0, 25 slots. Ctor inlined: {vptr,
// WUID = q_185338958 (runtime-init invalid-WUID global), inventory*}.
// Overrides: [0] dtor sub_181008420; [1] sub_183447408; [2] sub_1834473E4;
// [3] sub_181028B00; [4] sub_181A72EC0; [5] sub_183447660; [6] sub_18344736C;
// [7] sub_183447220; [8] sub_180838AE0; [9] sub_1834474D0; [10] sub_183447174;
// [11] sub_1803E3B40; [12] sub_183447540; [13] sub_183447704; [14] sub_183447854;
// [16] sub_183447618; [17..20] nullsub_1; [22] sub_180838AE0 (vs return-1 default).
// Defaults [15]/[21]/[23]/[24] inherited unchanged.

namespace wh::xgenaimodule {

class C_AIInventory;   // 0xE8 host (G6 wave; ctor sub_180A309EC)

class C_InventoryPuppet : public I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InventoryPuppet;
    ~C_InventoryPuppet() override;
    void _vf1() override;
    void _vf2() override;
    void* _vf3() override;
    void _vf4() override;
    void UpdateCachedTransform() override;   // [5] sub_183447660
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
    bool _vf22() override;                   // [22] sub_180838AE0

    framework::WUID m_wuid;        // +0x08  = q_185338958 (runtime-init invalid-WUID sentinel)
    C_AIInventory*  m_inventory;   // +0x10  paired inventory [V]
};
static_assert(sizeof(C_InventoryPuppet) == 0x18, "C_InventoryPuppet must be 0x18");

}  // namespace wh::xgenaimodule
