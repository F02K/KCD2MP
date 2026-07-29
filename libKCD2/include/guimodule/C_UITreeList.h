#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <vector>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/C_UIFlashDynTextureHelper.h"
#include "framework/C_Signal.h"
struct ISystemEventListener;                      // needed by the vendored SDK header below
#include "CryEngine/CryCommon/IHardwareMouse.h"   // IHardwareMouseEventListener

// -----------------------------------------------
// wh::guimodule::C_UITreeList -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1E0.
// -----------------------------------------------
// RTTI .?AVC_UITreeList@guimodule@wh@@ (TD 0x184C9E9C0).
// Bases: C_UIFlashBase @+0x00 (COL 0x413A888), IUIElementEventListener @+0x10
// (COL 0x413A8B0), IHardwareMouseEventListener @+0x58 (COL 0x413A8D8).
// vtables: primary 0x183A812E8 (14 slots -- no additions; overrides [1] Init
// 0x1805646F8, [2] 0x182B12354, [4] 0x180533900, [10] SetVisible 0x180562A98),
// hw-mouse 0x183A81288 (2, [1] 0x180572CB4).
// ctor sub_180BC8484(this, const char* szElementName, int a3).
//
// KCD2-NEW generic flash tree/list screen.  Instances found embedded BY VALUE:
// C_UIMap+0x28 ("ApseMapLegendList", a3=0) and C_UIApseCodex+0x10 ("ApseCodexList", a3=0).
// OPAQUE SHELL beyond the ctor-proven layout -- member roles marked UNVERIFIED were not
// traced this wave (G3 scope was the map vertical).

namespace wh::guimodule {

class C_UITreeList : public C_UIFlashBase, public IHardwareMouseEventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UITreeList;
    void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT event, int wheelDelta) override;   // [HM1] 0x180572CB4

    wh::shared::C_Signal<> m_signal60;       // +0x60  delegate table &unk_185669268 [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal70;       // +0x70  delegate table &unk_1856682E0 [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal80;       // +0x80  delegate table &unk_185669278 [signature UNVERIFIED]
    CryStringT<char>       m_str90;          // +0x90  ctor empty [role UNVERIFIED]
    std::vector<uint64_t>  m_vec98;          // +0x98  (0x18) element type UNVERIFIED
    int32_t                m_paramB0;        // +0xB0  ctor arg a3 (0 at both embed sites)
    uint8_t                _padB4[4];        // +0xB4
    C_UIFlashDynTextureHelper m_dynTexture;  // +0xB8  (0x28)
    std::map<int32_t, void*> m_mapE0;        // +0xE0  (0x10 head/size) K/V UNVERIFIED
    std::function<void()>  m_callbackF0;     // +0xF0  (0x40) [signature UNVERIFIED]
    std::function<void()>  m_callback130;    // +0x130 (0x40) [signature UNVERIFIED]
    std::function<void()>  m_callback170;    // +0x170 (0x40) [signature UNVERIFIED]
    CryStringT<char>       m_str1B0;         // +0x1B0 ctor empty [role UNVERIFIED]
    uint32_t               m_color1B8;       // +0x1B8 ctor 0x01000000 [ARGB? UNVERIFIED]
    int32_t                m_int1BC;         // +0x1BC ctor -1
    int32_t                m_int1C0;         // +0x1C0 ctor -1
    uint8_t                _pad1C4[4];       // +0x1C4 ctor-untouched
    float                  m_float1C8;       // +0x1C8 ctor -1.0f
    int32_t                m_int1CC;         // +0x1CC ctor 0 (unaligned qword zero-fill)
    int32_t                m_int1D0;         // +0x1D0 ctor 0
    bool                   m_flag1D4;        // +0x1D4 ctor 1
    uint8_t                _pad1D5[3];       // +0x1D5
    CryStringT<char>       m_str1D8;         // +0x1D8 ctor empty [role UNVERIFIED]
};
static_assert(sizeof(C_UITreeList) == 0x1E0, "C_UITreeList must be 0x1E0");
static_assert(offsetof(C_UITreeList, m_dynTexture) == 0xB8, "dyn-texture helper at 0xB8");
static_assert(offsetof(C_UITreeList, m_callbackF0) == 0xF0, "first callback at 0xF0");

}  // namespace wh::guimodule
