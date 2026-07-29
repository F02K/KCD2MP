#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/I_UIItemContext.h"
#include "guimodule/C_UISelectedItemsReporter.h"
#include "entitymodule/I_ItemCollectionListener.h"
struct ISystemEventListener;                      // needed by the vendored SDK header below
#include "CryEngine/CryCommon/IHardwareMouse.h"   // IHardwareMouseEventListener

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogCombine -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x2C8.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogCombine@guimodule@wh@@ (TD 0x184C9EB58). ctor sub_181951CE4.
// Bases: C_UIFlashBase @+0x00 + I_ItemCollectionListener @+0x58 + I_UIItemContext @+0x60
// + IHardwareMouseEventListener @+0x68 -- the MI outlier of the 7 modal dialogs.
// vtables: primary 0x183EDE768 (15 slots -- C_UIFlashBase 14 + ONE own [14] 0x182B12D40),
// IItemColl 0x183EDE818 (6) / IUIItemCtx 0x183EDE7E8 (5) / hw-mouse 0x183EDE850 (2).
// Overrides: [0] dtor 0x182B1158C, [1] Init 0x180ED200C; [2]/[4]/[10]/[11] base impls.
//
// Item-combine modal (alchemy/stack combining), flash element "ApseModalDialog".
// Init registers 5 dispatchers: "OnCombineDialogSelectClicked" -> sub_182B161F0,
// "OnCombineDialogConfirmClicked" -> sub_182B13A08, "OnCombineDialogCancelClicked" ->
// sub_182B13944, "OnCombineFocusChanged" -> sub_182B13D14 (registered via sub_180564FA8
// with get_ptr(this) -- member-fn dispatcher with args, signature UNVERIFIED),
// "OnDoubleClicked" -> sub_181F52790.
// Embedded BY VALUE in C_UIApseInventory @+0x6E8 (size pinned by next embed
// C_UIDragAndDropHelper @+0x9B0). Member roles largely UNVERIFIED (ctor-anchor map only).

namespace wh::guimodule {

class C_UIModalDialogCombine : public C_UIFlashBase,
                               public wh::entitymodule::I_ItemCollectionListener,  // +0x58
                               public I_UIItemContext,                             // +0x60
                               public IHardwareMouseEventListener {                // +0x68
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogCombine;
    virtual void _vf14();   // [14] 0x182B12D40  own slot; role UNVERIFIED

    // I_UIItemContext (pure) -- implemented via vtable 0x183EDE7E8.
    void _vf0() override;   // 0x183EDE7E8[0]
    void _vf1() override;   // [1]
    void _vf2() override;   // [2]
    void _vf3() override;   // [3]
    void _vf4() override;   // [4]

    void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT event, int wheelDelta) override;   // hw-mouse 0x183EDE850[1]

    uint8_t  m_fn70[0x40];               // +0x70   std::function<Sig?> (MSVC 0x40; _Impl@+0xA8 nulled in ctor; dtor _Tidy 0x1803E23C0 via thunk 0x180629280) [signature UNVERIFIED]
    uint8_t  m_fnB0[0x40];               // +0xB0   std::function<Sig?> (MSVC 0x40; _Impl@+0xE8 nulled in ctor; dtor _Tidy 0x1803E23C0 via thunk 0x1821386D0) [signature UNVERIFIED]
    uint8_t  m_fnF0[0x40];               // +0xF0   std::function<Sig?> (ctor sub_1805618EC nulls _Impl @this+0x128; dtor _Tidy 0x1803E23C0 via thunk 0x1821386D0) [signature UNVERIFIED]
    uint8_t  m_itemIndexer130[0x90];     // +0x130  wh::entitymodule::C_ItemIndexer (ctor 0x180C48410 stores C_ItemIndexer vtable; no RE header yet -> keep bytes)
    uint64_t m_unk1C0[6];                // +0x1C0..+0x1F0  ctor 0
    uint8_t  m_itemIndexer1F0[0x90];     // +0x1F0  wh::entitymodule::C_ItemIndexer #2 (ctor 0x180C48410 stores C_ItemIndexer vtable; no RE header yet -> keep bytes)
    C_UISelectedItemsReporter m_reporter; // +0x280 (0x18; boundary with the next two qwords UNVERIFIED)
    bool     m_flag298;                  // +0x298  ctor 0 [role UNVERIFIED]
    uint8_t  _pad299[7];                 // +0x299
    CryStringT<char> m_str2A0;           // +0x2A0  ctor empty [role UNVERIFIED]
    CryStringT<char> m_str2A8;           // +0x2A8  ctor empty [role UNVERIFIED]
    CryStringT<char> m_str2B0;           // +0x2B0  ctor empty [role UNVERIFIED]
    CryStringT<char> m_str2B8;           // +0x2B8  ctor empty [role UNVERIFIED]
    uint64_t m_unk2C0;                   // +0x2C0  ctor 0 [tail]
};
static_assert(sizeof(C_UIModalDialogCombine) == 0x2C8, "C_UIModalDialogCombine must be 0x2C8");
static_assert(offsetof(C_UIModalDialogCombine, m_reporter) == 0x280, "reporter at 0x280");
static_assert(offsetof(C_UIModalDialogCombine, m_str2A0) == 0x2A0, "string block at 0x2A0");

}  // namespace wh::guimodule
