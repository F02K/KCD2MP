#pragma once
#include <cstdint>
#include "guimodule/C_UIInventoryBase.h"
#include "guimodule/I_UIDragAndDropTarget.h"
#include "guimodule/C_UIModalDialogHealing.h"
#include "guimodule/C_UIModalDialogCombine.h"
#include "guimodule/C_UIDragAndDropHelper.h"
#include "guimodule/C_UIApseQAMInteraction.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseInventory -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xAC8.
// -----------------------------------------------
// RTTI .?AVC_UIApseInventory@guimodule@wh@@ (TD 0x184C924A0). ctor sub_180BC8ABC.
// Bases: C_UIInventoryBase @+0x00 (= C_UIBase + I_ItemCollectionListener@+0x10 +
// I_UIItemContext@+0x18) + I_UIDragAndDropTarget @+0x5D0.
// vtables: primary 0x183D28030 (26 slots = C_UIInventoryBase shape, overrides ~10 of
// the base's own [10..25]: 0x1808C0FAC, 0x180C4686C, 0x180C4527C, 0x1808EE0C4,
// 0x1817EF6D4, 0x1808F6610, 0x182AFEFA8, 0x1808EC088, 0x181F52700, 0x181F4A320,
// 0x182AFF190, 0x1819CB430, 0x182B002EC, 0x180559F98, 0x1812B36B4, 0x180565488;
// [0] dtor 0x182AFD70C, [1] Init 0x180ED3EBC, [2] Deinit 0x182AFE4AC, [4] Update
// 0x180567AB8) / item-coll 0x183D2EE40 (6) / item-ctx 0x183D2FA20 (5) /
// dnd-target 0x183D2EE78 (6: [1] 0x1811A2850, [2] 0x1811A29B8).
// Embedded BY VALUE in C_UIApse @+0xD8 (size pinned by next embed C_UIApsePlayer
// @+0xBA0).
//
// The Apse inventory tab: base chassis ("ApseInventoryList"/"ApseInventoryInfo"/
// "ApseModalDialog") + healing/combine dialogs, drag-and-drop, and QAM interaction.
// The QAM embed spans to the object end (its own tail covers +0xAB0..+0xAC8).

namespace wh::guimodule {

class C_UIApse;

class C_UIApseInventory : public C_UIInventoryBase,
                          public I_UIDragAndDropTarget {   // +0x5D0
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseInventory;
    // I_UIDragAndDropTarget (pure) -- implemented via vtable 0x183D2EE78.
    void _vf1() override;   // [1] 0x1811A2850  role UNVERIFIED
    void _vf2() override;   // [2] 0x1811A29B8  role UNVERIFIED

    // (+0x5D8 = inherited I_UIDragAndDropTarget::m_dragCtx08, ctor 0)
    C_UIApse* m_pApse;                    // +0x5E0  owner hub (ctor a2)
    C_UIModalDialogHealing m_healingDialog; // +0x5E8  (0x100) "ApseModalDialog"
    C_UIModalDialogCombine m_combineDialog; // +0x6E8  (0x2C8) "ApseModalDialog" (ctor sub_181951CE4)
    C_UIDragAndDropHelper m_dndHelper;    // +0x9B0  (0x80)
    C_UIApseQAMInteraction m_qamInteraction; // +0xA30  (0x98) spans to the object end
};
static_assert(sizeof(C_UIApseInventory) == 0xAC8, "C_UIApseInventory must be 0xAC8");
static_assert(offsetof(C_UIApseInventory, m_healingDialog) == 0x5E8, "healing dialog at 0x5E8");
static_assert(offsetof(C_UIApseInventory, m_qamInteraction) == 0xA30, "QAM interaction at 0xA30");

}  // namespace wh::guimodule
