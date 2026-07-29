#pragma once
#include "C_InventoryOwnership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_HorseInventoryOwnership : C_InventoryOwnership --
// horse-inventory ownership variant (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18
// [U -- create site NOT located; size inferred from slot [17] using the COMDAT body
// sub_181AA5710 which writes *(this+0x10), implying a member at +0x10].
// -----------------------------------------------
// RTTI TD rva 0x4F492E0; vtable 0x183C14148, 18 slots. Overrides vs the plain
// variant: [0] dtor sub_1809674DC (ICF-folded with the shop dtor); [2]
// GetEffectiveOwner sub_1832A9450; [12] ClearAllOwners sub_181AA56F0; [16]
// sub_18041A6A0 (return 1, vs sub_180838AE0); [17] sub_181AA5710 (the C_Ownership
// SetOwner body -> writes +0x10). All other slots inherited unchanged.

namespace wh::xgenaimodule {

class C_HorseInventoryOwnership : public C_InventoryOwnership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HorseInventoryOwnership;
    ~C_HorseInventoryOwnership() override;                 // [0]  sub_1809674DC
    const framework::WUID* GetEffectiveOwner() override;   // [2]  sub_1832A9450
    void ClearAllOwners() override;                        // [12] sub_181AA56F0
    void _vf16() override;                                 // [16] sub_18041A6A0 (return 1)
    void _vf17() override;                                 // [17] sub_181AA5710 -- writes m_unk10 [U role]

    framework::WUID m_owner;   // +0x10  slot [17] SetOwner body sub_181AA5710 stores the 8-byte owner WUID (*a2 -> [this+0x10])
};
static_assert(sizeof(C_HorseInventoryOwnership) == 0x18, "C_HorseInventoryOwnership modeled 0x18 (size inferred, create site not located)");

}  // namespace wh::xgenaimodule
