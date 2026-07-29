#pragma once
#include <cstdint>
#include "../entitymodule/I_InventoryManagerListener.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIInventoryManager : entitymodule::I_InventoryManagerListener
// -- AI-side inventory event tracker SINGLETON at qword_185494428 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0xD0 (getter sub_1807D21F0 @0x1807d2240:
// sub_18549D378(208,...), += 0xD0, ctor sub_180BEA770).  dtor sub_1832EDAE4.
// -----------------------------------------------
// RTTI TD rva 0x4F5C1C8.  Vtable rva 0x3FECD38 (7 slots: [0] dtor
// sub_180A308D4, [1]sub_180A2F418 [2]sub_180966824 [3]sub_180965C4C
// [4]sub_1832F00E4 [5]sub_1812EBD54 [6]sub_1832EDFF8).  ctor stores the
// inventory subsystem (*(sub_1809155C8()+224)+232) at +0x08 and self-appends
// into that subsystem's listener vector (+1049176/+1049184).  Container
// regions typed as blobs [U interior].

namespace wh::xgenaimodule {

class C_AIInventoryManager : public entitymodule::I_InventoryManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIInventoryManager;
    ~C_AIInventoryManager() override;   // [0] sub_180A308D4
    void ImlVf1() override; void ImlVf2() override; void ImlVf3() override;
    void ImlVf4() override; void ImlVf5() override; void ImlVf6() override;   // [1..6] impls [U roles]

    entitymodule::C_InventoryManager* m_subsystem;  // +0x08  = *(C_EntityModule+0xE8) singleton; self-appends into its m_listeners (+0x100258/+0x100260)
    uint8_t m_byte10;          // +0x10  ctor: 0 [U role]
    uint8_t _pad11[3];         // +0x11
    float   m_scale14;         // +0x14  ctor: 1.0f [U role]
    uint8_t m_hashSet18[0x38]; // +0x18..+0x50  WH hash-set (node 32B; mask 7 @+0x40, count 8 @+0x48) [U interior]
    uint8_t m_sub50[0x40];     // +0x50..+0x90  container (sub_1806030E4) [U interior]
    uint8_t m_sub90[0x40];     // +0x90..+0xD0  container (sub_180BEA890) [U interior]
};
static_assert(sizeof(C_AIInventoryManager) == 0xD0, "C_AIInventoryManager must be 0xD0 (alloc 208 at sub_1807D21F0)");

}  // namespace wh::xgenaimodule
