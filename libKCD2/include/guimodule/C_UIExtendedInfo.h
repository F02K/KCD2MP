#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/C_UIFlashDynTextureHelper.h"
struct ISystemEventListener;                      // needed by the vendored SDK header below
#include "CryEngine/CryCommon/IHardwareMouse.h"   // IHardwareMouseEventListener

// -----------------------------------------------
// wh::guimodule::C_UIExtendedInfo -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_UIExtendedInfo@guimodule@wh@@ (TD 0x184C92900).
// Bases: C_UIFlashBase @+0x00 (COL 0x413A568), IUIElementEventListener @+0x10
// (COL 0x413A590), IHardwareMouseEventListener @+0x58 (COL 0x413A5B8).
// vtables: primary 0x183A811E8 (19 slots = C_UIFlashBase 14 + own [14..18]),
// listener 0x183A811A0 (8), hw-mouse 0x183A810C8 (2).
// ctor sub_180BC8628(this, const char* szElementName) -- inline C_UIFlashBase init,
// stores the element name at +0x50.
//
// KCD2-NEW intermediate flash base adding: visibility state, tooltip plumbing
// ("OnShowTooltip"/"OnShowTooltipPos"/"OnHideTooltip"/"OnUpdateTooltip" dispatchers
// registered by Init sub_1805638A4, which also adds the hw-mouse listener via
// g_pHardwareMouse qword_18492D918 vf[+0x20]) and a dyn-texture rebind helper.
// Subclass analyzed: C_UIMapContent ("ApseMap").
//
// Own slots:
//   [14] 0x18166C088  create dyn-texture/sub-element (sub_18073D68C on m_dynTexture)
//        and REBIND the flash element (rebind helper sub_180565FD0)  [name INFERRED]
//   [15] 0x180563AB4  teardown: rebind(null) + release m_lastTooltip + remove the
//        hw-mouse listener (sub_18088E91C)  [name INFERRED]
//   [16] 0x1812C1480  ShowTooltip(name) = ShowTooltip(name, -1, -1) via own vf[17]
//   [17] 0x1812C14D4  ShowTooltip(name, x, y): forwards to the module's tooltip element
//        (cached rttr getter sub_180C45150, elem subobj +0x58 vf[2]) and stores the name
//        in m_lastTooltip
//   [18] 0x182B144F0  if m_lastTooltip non-empty, forward (x, y) to the tooltip element
//        (subobj +0x58 vf[3])  [name INFERRED]
// Override: [10] SetVisible 0x180562B08 (base helper + m_visible = a2; on hide also
// dismisses the tooltip sub_180562BA4).  The C_UIFlashBase slot [10] is hereby pinned
// as SetVisible(bool).

namespace wh::guimodule {

class C_UIExtendedInfo : public C_UIFlashBase, public IHardwareMouseEventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIExtendedInfo;
    // NOTE: 3-arg overload DECLARED FIRST on purpose -- MSVC reverses adjacent same-name
    // virtual overloads (2nd-declared lands in the LOWER slot), and the binary has the
    // 1-arg forwarder at [16] below the 3-arg body at [17].
    virtual void _vf14(const CryStringT<char>& name, int a3, const CryStringT<char>& texName);  // [14] 0x18166C088  dyn-texture rebind [args INFERRED]
    virtual void ReleaseExtendedInfo();                                                         // [15] 0x180563AB4  [name INFERRED]
    virtual void ShowTooltip(const CryStringT<char>& name, uint32_t x, uint32_t y);             // [17] 0x1812C14D4
    virtual void ShowTooltip(const CryStringT<char>& name);                                     // [16] 0x1812C1480
    virtual void UpdateTooltipPos(uint32_t x, uint32_t y);                                      // [18] 0x182B144F0  [name INFERRED]

    // IHardwareMouseEventListener occupies +0x58 (vtable only); base handler
    // 0x180572D54: on LBUTTONDOWN over the movie (m_pElement vf[+0x370] hit test),
    // when hidden, queues a deferred action from m_mouseAction  [flow partially traced].
    void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT event, int wheelDelta) override;   // [HM1] 0x180572D54

    bool             m_visible;        // +0x60  set by SetVisible slot [10] (0x180562B08)
    bool             m_flag61;         // +0x61  gates the hw-mouse handler [role UNVERIFIED]
    uint8_t          _pad62[6];        // +0x62
    CryStringT<char> m_lastTooltip;    // +0x68  last ShowTooltip name (cleared by [15])
    std::function<void()> m_mouseAction;  // +0x70  (0x40) invoked from the hw-mouse path [signature UNVERIFIED]
    C_UIFlashDynTextureHelper m_dynTexture;  // +0xB0  (0x28) dyn-texture bind helper
};
static_assert(sizeof(C_UIExtendedInfo) == 0xD8, "C_UIExtendedInfo must be 0xD8");
static_assert(offsetof(C_UIExtendedInfo, m_lastTooltip) == 0x68, "tooltip name at 0x68");
static_assert(offsetof(C_UIExtendedInfo, m_dynTexture) == 0xB0, "dyn-texture helper at 0xB0");

}  // namespace wh::guimodule
