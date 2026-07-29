#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::guimodule::I_UIDragAndDropTarget -- KCD2 WHGame.dll 1.5.6 (kd7u).  Interface, 6 slots, sizeof 0x10.
// -----------------------------------------------
// RTTI .?AVI_UIDragAndDropTarget@guimodule@wh@@ (TD 0x184C93080). Drop-target hook of
// the Apse drag-and-drop machinery (C_UIDragAndDropHelper). Standalone (pure) vtable
// 0x183ED92E8: [0] dtor 0x182AFDB20, [1..2] _purecall, [3..5] rttr trio.
// Implementors: C_UICharacterSlots (subobj vtable 0x183B8F2B8: [1] 0x1811A234C,
// [2] 0x1811A2A40) and C_UIApseInventory @+0x5D0 (0x183D2EE78: [1] 0x1811A2850,
// [2] 0x1811A29B8).
// Method roles/signatures UNVERIFIED (coined; void() until traced).

namespace wh::guimodule {

class I_UIDragAndDropTarget {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIDragAndDropTarget;
    virtual ~I_UIDragAndDropTarget();                       // [0] 0x182AFDB20 (standalone)
    virtual void _vf1() = 0;                                // [1] role UNVERIFIED
    virtual void _vf2() = 0;                                // [2] role UNVERIFIED
    RTTR_ENABLE()  // [3..5]: get_type 0x182B03A20; registered BASELESS, sizeof imm 0x10

    void* m_dragCtx08;   // +0x08  drag payload/ctx slot, written by target impls (verify pass:
                         //        creator 0x182AFC048 sizeof imm 0x10 -- implementors' "+0x18"/
                         //        "+0x5D8 own member" was THIS field; C_UICharacterSlots vf[1]
                         //        0x1811A234C sets it from arg, vf[2] 0x1811A2A40 clears) [name coined]
};
static_assert(sizeof(I_UIDragAndDropTarget) == 0x10, "creator 0x182AFC048 sizeof imm 0x10");

}  // namespace wh::guimodule
