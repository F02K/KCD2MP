#pragma once
#include <cstdint>
#include "guimodule/C_UICharacterSlots.h"
#include "entitymodule/I_EquipmentManagerListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseCharacterSlots -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_UIApseCharacterSlots@guimodule@wh@@ (TD 0x184C91FC0).
// Bases: C_UICharacterSlots @+0x00 (= C_UIBase + I_UIDragAndDropTarget@+0x10) +
// I_EquipmentManagerListener @+0x48. vtables: primary 0x183B8F3A0 (14) /
// IDnDTarget 0x183B8F2B8 (6: [1] 0x1811A234C, [2] 0x1811A2A40) / IEquip 0x183B8F290
// (4: dtor-thunk 0x18213A1E0, [1]=[2] 0x181889DA0, [3] nullsub).
// Primary overrides: [0] dtor 0x182AFD660, [4] Update 0x180567EBC ([1] Init = base
// default); own C_UICharacterSlots slots [10..13] = 0x180C41F40 / 0x180C4A540 /
// 0x181F4B660 / 0x1805682AC.
//
// HEAP-ALLOCATED (alloc-proven 0x78: sub_18146D050 `qword_18549D378(120)`), held as a
// shared_ptr-managed object from C_UICharacterBase+0xF0 -- NOT embedded by value
// (unique among the Apse panels). Constructed with (owner = C_UIApseCharacter,
// content = owner+0x1C8).

namespace wh::guimodule {

class C_UIApseCharacterSlots : public C_UICharacterSlots,
                               public wh::entitymodule::I_EquipmentManagerListener {  // +0x48
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseCharacterSlots;
    // C_UICharacterSlots pure slots (primary vtable 0x183B8F3A0 [10..13]).
    void _vf10() override;   // [10] 0x180C41F40  role UNVERIFIED
    void _vf11() override;   // [11] 0x180C4A540  role UNVERIFIED
    void _vf12() override;   // [12] 0x181F4B660  role UNVERIFIED
    void _vf13() override;   // [13] 0x1805682AC  role UNVERIFIED

    // I_EquipmentManagerListener overrides (subobject vtable 0x183B8F290; [1] and [2]
    // share one body 0x181889DA0, [3] stays the nullsub default).
    void OnItemEquipped(void* a2, uint64_t itemWuid) override;    // [1] 0x181889DA0
    void OnItemUnequipped(void* a2, uint64_t itemWuid) override;  // [2] 0x181889DA0

    void*   m_unk50;     // +0x50  non-owning context ptr (mirror of C_UICharacterSlots::m_unk30); slot[13] 0x1805682AC: if (m_unk50 && m_flag58) commit via sub_1808F51C8(this,0) then clear m_flag58
    bool    m_flag58;    // +0x58  ctor 0 [role UNVERIFIED]
    uint8_t _pad59[7];   // +0x59
    std::vector<void*> m_subscribed60;  // +0x60  (0x18 = begin/end/cap) objects this panel subscribed to; slot[10] 0x180C41F40 unsubscribes this+0x48 from each via sub_1823CF140 then clears (last=first); element type UNVERIFIED
};
static_assert(sizeof(C_UIApseCharacterSlots) == 0x78, "C_UIApseCharacterSlots must be 0x78 (alloc sub_18146D050)");

}  // namespace wh::guimodule
