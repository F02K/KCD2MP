#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIExtendedInfo.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIMapContent -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x168.
// -----------------------------------------------
// RTTI .?AVC_UIMapContent@guimodule@wh@@ (TD 0x184C92850).
// Bases: C_UIExtendedInfo @+0x00 (COL 0x413BF08), IUIElementEventListener @+0x10
// (COL 0x413BF30), IHardwareMouseEventListener @+0x58 (COL 0x413BF58).
// vtables: primary 0x183D2A638 (19), listener 0x183D2E438 (8), hw-mouse 0x183D32B60 (2).
// ctor sub_1809EF244 -- C_UIExtendedInfo("ApseMap"); NOT rttr-instantiated: embedded
// BY VALUE in C_UIMap at +0x208.
//
// The flash side of the KCD2 map: binds the "ApseMap" element (the map movie inside the
// Apse hub) and owns the map-cursor flash traffic.  C_UIMap keeps the game-side state
// and calls through this member for every outbound flash call ("AddPoiMarkers"/
// "RemovePoiMarkers"/"SetPlayer"/"UpdateFastTravelProgress"/"SetTime"/"FastTravelStarted"/
// "FastTravelFinished"/"GetCurrentCursorPosX"/"GetCurrentCursorPosY", ...).
//
// [FUNDAMENTAL vs KCD1] KCD2-new split: KCD1's C_UIMap was itself the flash event
// system ("Map" UIElement + hash-dispatch OnUIEvent @+0x338).  KCD2 moves the element
// binding + inbound dispatchers here and demotes C_UIMap to a non-flash C_UIBase.
//
// Inbound dispatchers (Init override [1] 0x180ED17C4, after C_UIExtendedInfo::Init):
//   "OnCursorOnActiveAreaChanged" -> 0x181950D08  (bool)  fire m_sigCursorOnActiveArea
//   "OnHighlightFastTravelPoint"  -> 0x182AFFF94  show tooltip + fire m_sigFTPointHighlighted
//   "OnUnhighlightFastTravelPoint"-> 0x182B006EC  hide tooltip + fire m_sigFTPointUnhighlighted
//   "OnDoubleClicked"             -> 0x182AFFD64  (string) invoke m_onDoubleClicked if set,
//                                    else forward m_doubleClickTarget to the tooltip element
//   "OnUpdatingChanged"           -> 0x181F4A9F0  (bool)  m_updating = a2
// Overrides: [4] Update 0x181F4B890 (base drain + gamepad/mouse device poll via
// qword_18492D908 vf+0x9F0/0x9F8/0xA00 while m_updating); [10] SetVisible 0x180562A2C;
// [14] 0x18166BF48 (base dyn-texture bind + flash "SetUserConfig"(m_savedZoom,
// m_savedCursorPos) when m_flag15C); [15] 0x1819DE5A4; [17] 0x182B005D4 / [18]
// 0x182B007C4 (flash-space -> screen coord transform via m_pElement vf[+0x368] first);
// hw-mouse [1] 0x181349230 (event 3 = left double click -> flash "LeftDoubleClick").
// Save/load: C_UIMap's serializer chunk 0xC1D restores m_savedCursorPos.

namespace wh::guimodule {

class C_UIMapContent : public C_UIExtendedInfo {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMapContent;
    // event 3 (left double click) -> flash "LeftDoubleClick", then the base handler.
    void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT event, int wheelDelta) override;   // [HM1] 0x181349230

    wh::shared::C_Signal<bool> m_sigCursorOnActiveArea;      // +0xD8  table &unk_185665AC0; fired by "OnCursorOnActiveAreaChanged"
    wh::shared::C_Signal<const CryStringT<char>&> m_sigFTPointHighlighted;    // +0xE8  table &unk_185669258; emit sub_1811A09AC(point name)
    wh::shared::C_Signal<const CryStringT<char>&> m_sigFTPointUnhighlighted;  // +0xF8  table &unk_185669258; emit sub_1811A09AC(point name)
    std::function<void(const CryStringT<char>&)> m_onDoubleClicked;  // +0x108  (0x40) "OnDoubleClicked" sink (invoked via sub_182AFD1DC)
    CryStringT<char>           m_doubleClickTarget;          // +0x148  fallback double-click forward target [role INFERRED]
    Vec2                       m_savedCursorPos;             // +0x150  ctor (-1,-1); persisted via save chunk 0xC1D
    float                      m_savedZoom;                  // +0x158  ctor -100.0f [zoom INFERRED from "SetUserConfig" use]
    bool                       m_flag15C;                    // +0x15C  ctor 0; gates the "SetUserConfig" push [role UNVERIFIED]
    uint8_t                    _pad15D[3];                   // +0x15D
    bool                       m_updating;                   // +0x160  "OnUpdatingChanged" state; polled by Update
    uint8_t                    _pad161[7];                   // +0x161
};
static_assert(sizeof(C_UIMapContent) == 0x168, "C_UIMapContent must be 0x168");
static_assert(offsetof(C_UIMapContent, m_onDoubleClicked) == 0x108, "double-click sink at 0x108");
static_assert(offsetof(C_UIMapContent, m_savedCursorPos) == 0x150, "saved cursor at 0x150");

}  // namespace wh::guimodule
