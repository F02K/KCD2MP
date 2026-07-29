#pragma once
#include <cstdint>
#include "guimodule/C_UIExtendedInfo.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseCharacterContent -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x200.
// -----------------------------------------------
// RTTI .?AVC_UIApseCharacterContent@guimodule@wh@@ (TD 0x184C91EB8). ctor sub_180BC7EA0.
// Bases: C_UIExtendedInfo @+0x00 (-> C_UIFlashBase -> C_UIBase) + IUIElementEventListener
// @+0x10 + IHardwareMouseEventListener @+0x58 (same shape as C_UIExtendedInfo).
// vtables: primary 0x183A81770 (19 slots = C_UIExtendedInfo shape, NO additions;
// overrides [1] Init 0x180ED227C, [2] Deinit 0x182B12354, [4] Update 0x180567FBC,
// [10] SetVisible 0x180562A2C), hw-mouse 0x183A81058 ([1] 0x181179340).
//
// The character-paperdoll content pane, flash element "ApseCharacter"
// (C_UIExtendedInfo ctor sub_180BC8628(this, "ApseCharacter")). Embedded BY VALUE in
// C_UICharacterBase @+0x1C8 (size pinned by the base's next member block @+0x3C8).
// Own member roles beyond the signal UNVERIFIED.

namespace wh::guimodule {

class C_UIApseCharacterContent : public C_UIExtendedInfo {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseCharacterContent;
    wh::shared::C_Signal<> m_signalD8;    // +0xD8  delegate table &unk_185669278 [signature UNVERIFIED]
    std::function<void(const CryStringT<char>&, const CryStringT<char>&)> m_onStartDrag;      // +0xE8  (0x40) invoked 2-arg (sub_182AFD20C) from OnStartDrag handler 0x182B00678
    std::function<void(const CryStringT<char>&)>                          m_onDoubleClicked;  // +0x128 (0x40) invoked 1-arg (sub_182AFD1DC) from OnDoubleClicked handler 0x182AFFCCC
    std::function<void(const CryStringT<char>&, const CryStringT<char>&)> m_fn168;            // +0x168 (0x40) invoked 2-arg (sub_182AFD20C) from OnDoubleClicked handler 0x182AFFCCC else-branch
    std::function<void(bool)>                                             m_onOutfitClicked;  // +0x1A8 (0x40) invoked 1-arg (_Do_call vf[2]) from OnOutfitClicked handler 0x182B002BC [arg type INFERRED]
    bool     m_flag1E8;                    // +0x1E8  gates m_onOutfitClicked / OnHighlightOutfit (ctor 0; read as !flag)
    uint8_t  _pad1E9[7];                   // +0x1E9
    std::map<uint64_t, void*> m_map1F0;    // +0x1F0  (0x10) MSVC std::_Tree {_Myhead@+0x1F0, _Mysize@+0x1F8}; node 0x30, value_type 0x10 {key8, T*}; Update 0x180567FBC in-order-walks it & animates each T* via sub_1805339D4 [K/V exact types INFERRED]
};
static_assert(sizeof(C_UIApseCharacterContent) == 0x200, "C_UIApseCharacterContent must be 0x200");
static_assert(offsetof(C_UIApseCharacterContent, m_signalD8) == 0xD8, "signal at 0xD8");

}  // namespace wh::guimodule
