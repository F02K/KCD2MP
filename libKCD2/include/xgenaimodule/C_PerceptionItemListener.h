#pragma once
#include "../entitymodule/I_WorldInventoryListener.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptionItemListener : entitymodule::
// I_WorldInventoryListener -- feeds world-inventory item changes into the
// perception system (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (create-site
// proven: qword_18549D378(16) in C_PerceptionManager's secondary init
// sub_180F661D0 @0x180f66368; stored at PM+0x118).
// -----------------------------------------------
// RTTI TD rva 0x4F4CD00; COL 0x18454F968; vtable 0x183FE6C40, 3 slots:
// [0] sub_1819DBEF4 -- OnItemAdded: registry lookup sub_18047C1F8(
//     qword_185481FC8, event+48) then sub_1803EB888(m_manager, rec, 0, 0)
//     creates a C_PerceptibleObject for the item (@0x1819dbf0c);
// [1] sub_181850510 -- OnItemChanged/Removed: same lookup, then rec vslot6 +
//     sub_180D479BC(m_manager, rec, 1) (@0x181850528);
// [2] sub_1832AE9A4 -- scalar-deleting dtor (NOTE: dtor is slot [2] here, not
//     [0] -- interface slot order).
// Registered in the world-inventory C_Listeners<I_WorldInventoryListener> at
// *(gameCtx+224)+232 (@0x180f663b7).

namespace wh::xgenaimodule {

class C_PerceptionManager;

class C_PerceptionItemListener : public entitymodule::I_WorldInventoryListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptionItemListener;
    void _vf0() override;   // [0] sub_1819DBEF4 -- OnItemAdded (creates perceptible record)
    void _vf1() override;   // [1] sub_181850510 -- OnItemChanged/Removed
    void _vf2() override;   // [2] sub_1832AE9A4 -- scalar-deleting dtor slot

    C_PerceptionManager* m_manager;   // +0x08  back-ref to the owning perception manager (@0x180f6638d)
};
static_assert(sizeof(C_PerceptionItemListener) == 0x10, "C_PerceptionItemListener must be 0x10 (alloc 16)");

}  // namespace wh::xgenaimodule
