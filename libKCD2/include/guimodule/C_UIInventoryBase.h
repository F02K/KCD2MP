#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/I_UIItemContext.h"
#include "guimodule/C_UITreeList.h"
#include "guimodule/C_UIExtendedInfo.h"
#include "guimodule/C_UIModalDialogAmount.h"
#include "guimodule/C_UIModalDialogQuestion.h"
#include "guimodule/C_UIModalDialogInfo.h"
#include "entitymodule/I_ItemCollectionListener.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIInventoryBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x5D0.
// -----------------------------------------------
// RTTI .?AVC_UIInventoryBase@guimodule@wh@@ (TD 0x184C924D8).
// ctor sub_180BC8224(this, listName, infoName, modalName, a5).
// Bases: C_UIBase @+0x00 + I_ItemCollectionListener @+0x10 + I_UIItemContext @+0x18.
// Own vtables 0x183EDE8A8 (primary, 26 slots) / 0x183EDE870 (item-coll) / 0x183EDE980
// (item-ctx: [0] 0x1808F76E8, [1] 0x181A730F0, [2] 0x1808F7568, [3..4] 0x180838AE0).
// Primary: [0] dtor 0x182B114F0, [1] Init 0x180C41534, [2] Deinit 0x182B123B4,
// [4] Update 0x180533724, own [10..25]: 0x1808C0FAC, 0x180C4686C, 0x180C46790,
// 0x180C49154, 0x180C49054, 0x1812B3EF4, 0x18066CD10, 0x1808EC278, 0x181F52700,
// 0x180838AE0, 0x18041A6A0, 0x18041A6A0, 0x182B143D0, 0x180559F98, 0x1812B36B4,
// 0x180565488. Size proven by the subclass C_UIApseInventory's I_UIDragAndDropTarget
// base landing at +0x5D0.
//
// Shared inventory-screen chassis: hosts the list/info widgets and three modal dialogs
// (all bound to "ApseModalDialog"). C_UIApseInventory instantiates it with
// ("ApseInventoryList", "ApseInventoryInfo", "ApseModalDialog", a5).

namespace wh::guimodule {

class C_UIInventoryBase : public C_UIBase,
                          public wh::entitymodule::I_ItemCollectionListener,  // +0x10
                          public I_UIItemContext {                            // +0x18
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIInventoryBase;
    virtual void _vf10();   // [10] 0x1808C0FAC  role UNVERIFIED
    virtual void _vf11();   // [11] 0x180C4686C  role UNVERIFIED
    virtual void _vf12();   // [12] 0x180C46790  role UNVERIFIED
    virtual void _vf13();   // [13] 0x180C49154  role UNVERIFIED
    virtual void _vf14();   // [14] 0x180C49054  role UNVERIFIED
    virtual void _vf15();   // [15] 0x1812B3EF4  role UNVERIFIED
    virtual void _vf16();   // [16] 0x18066CD10  role UNVERIFIED
    virtual void _vf17();   // [17] 0x1808EC278  role UNVERIFIED
    virtual void _vf18();   // [18] 0x181F52700  role UNVERIFIED
    virtual void _vf19();   // [19] 0x180838AE0  (shared getter) role UNVERIFIED
    virtual void _vf20();   // [20] 0x18041A6A0  (shared nullish body) role UNVERIFIED
    virtual void _vf21();   // [21] 0x18041A6A0  (shared nullish body) role UNVERIFIED
    virtual void _vf22();   // [22] 0x182B143D0  role UNVERIFIED
    virtual void _vf23();   // [23] 0x180559F98  role UNVERIFIED
    virtual void _vf24();   // [24] 0x1812B36B4  role UNVERIFIED
    virtual void _vf25();   // [25] 0x180565488  role UNVERIFIED

    // I_UIItemContext (pure) -- implemented via vtable 0x183EDE980.
    void _vf0() override;   // [0] 0x1808F76E8
    void _vf1() override;   // [1] 0x181A730F0
    void _vf2() override;   // [2] 0x1808F7568
    void _vf3() override;   // [3] 0x180838AE0
    void _vf4() override;   // [4] 0x180838AE0

    wh::shared::C_Signal<> m_signal20;     // +0x20  delegate table &unk_185667208 [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal30;     // +0x30  delegate table &unk_185667108 (sub_180C48214) [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal40;     // +0x40  delegate table &unk_185667108 (sub_180C48214) [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal50;     // +0x50  delegate table &unk_185665430 [signature UNVERIFIED]
    C_UITreeList     m_list;               // +0x60   (0x1E0) ctor arg listName ("ApseInventoryList")
    C_UIExtendedInfo m_info;               // +0x240  (0xD8) ctor arg infoName ("ApseInventoryInfo")
    C_UIModalDialogAmount m_amountDialog;  // +0x318  (0xE8) ctor arg modalName ("ApseModalDialog")
    C_UIModalDialogQuestion m_question;    // +0x400  (0xE8) ctor sub_180BC83D0 ("ApseModalDialog")
    C_UIModalDialogInfo m_modalInfo;       // +0x4E8  (0xA8) ctor sub_180BC842C ("ApseModalDialog")
    uint8_t  m_tail590[0x40];              // +0x590..+0x5D0  tail: ptrs 0, dword = ctor a5 @+0x5A0, byte @+0x5B0 [roles UNVERIFIED]
};
static_assert(sizeof(C_UIInventoryBase) == 0x5D0, "C_UIInventoryBase must be 0x5D0");
static_assert(offsetof(C_UIInventoryBase, m_list) == 0x60, "tree list at 0x60");
static_assert(offsetof(C_UIInventoryBase, m_modalInfo) == 0x4E8, "info dialog at 0x4E8");

}  // namespace wh::guimodule
