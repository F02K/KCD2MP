#pragma once
#include <cstdint>
#include "conceptmodule/C_Effect.h"
#include "conceptmodule/C_EventNode.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "conceptmodule/S_Trigger.h"
#include "framework/E_UIApseView.h"

// -----------------------------------------------
// wh::guimodule::C_ApseViewTrigger -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x178 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184C9DFD0; COL 0x413AC48; primary vtable 0x183C04AA8 (47-slot
// C_EventNode<C_Effect> node table -- not modeled as C++ virtuals per module
// convention). Base chain: C_EventNode<C_Effect> -> C_Effect -> ... . ctor sub_18171FBCC;
// as_wh creator sub_182B0BA9C (alloc 0x178, memset @0x182b0bafd).
// Own slots vs C_Effect (0x183A45F30): [0] dtor 0x1817695C4, [5]/[7] trio, [12]
// event-port reader 0x18127FEA0, [27] box-self 0x182B12DD8, [43] TRIGGER 0x182B134D8,
// [44] UNTRIGGER 0x182B14030.
// Behavior: TRIGGER subscribes to the apse-view-change signal (S_GameContext obj+432
// vf[+0x20]) and fires m_onEnter/m_onExit when the live view matches m_viewPort;
// UNTRIGGER unsubscribes. Port display-names coined (registrar not isolated).

namespace wh::guimodule {

class C_ApseViewTrigger : public wh::conceptmodule::C_EventNode<wh::conceptmodule::C_Effect> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ApseViewTrigger;
    wh::conceptmodule::C_TypedPortRef<wh::framework::E_UIApseView::Type> m_viewPort;     // +0xB0  (0x40)
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger>      m_onEnterPort;  // +0xF0  out [name coined]
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger>      m_onExitPort;   // +0x130 out [name coined]
    wh::framework::E_UIApseView::Type m_lastView;  // +0x170  cached view (trigger @0x182b1352c)
    uint8_t m_flag171;     // +0x171  ctor 0 [role UNVERIFIED]
    uint8_t _pad172[6];    // +0x172
};
static_assert(sizeof(C_ApseViewTrigger) == 0x178, "C_ApseViewTrigger must be 0x178 (creator sub_182B0BA9C)");
static_assert(offsetof(C_ApseViewTrigger, m_viewPort) == 0xB0, "view port at 0xB0");

}  // namespace wh::guimodule
