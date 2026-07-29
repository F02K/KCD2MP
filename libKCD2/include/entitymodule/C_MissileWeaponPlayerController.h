#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"    // platform.h chain (int64/ILINE) for TimeValue.h
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/TimeValue.h"

// -----------------------------------------------
// wh::entitymodule::C_MissileWeaponPlayerController -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40.
// -----------------------------------------------
// Bow/crossbow/gun player controller (trajectory preview + anim-event handling). Embedded in
// C_Player at +0xD70 (ctor sub_180BC6D5C; registers anim-event handlers "bow_primary",
// "crossbow_prepare", "gun_execute", ...). MI (RTTI): IActionMapEventListener merged into the
// primary vtable (0x183A80E30: [0] dtor, [1] event callback sub_182AA91C4) +
// wh::framework::I_SourceMonitorListener @+0x08 (0x183A80E20, 1 slot).

#include "../framework/I_SourceMonitorListener.h"   // canonical 1-slot listener (callback here: sub_1814DBC2C)
#include "../CryEngine/CryAction/IActionMapEventListener.h"   // [1] here = sub_182AA91C4 (matches m_attachName on event code 3)

namespace wh::entitymodule {

class C_Player;

class C_MissileWeaponPlayerController
    : public IActionMapEventListener              // +0x00
    , public wh::framework::I_SourceMonitorListener   // +0x08
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MissileWeaponPlayerController;
    void OnActionMapEvent(void* event) override {}
    void OnSourceEvent(void* a2, bool bActive) override {}

    CTimeValue m_field10;        // +0x10  init -100000 = CTimeValue(-1.0s) "never" sentinel (ctor 0x180BC6D8E)
    C_Player* m_pOwner;          // +0x18
    uint32_t m_flags;            // +0x20  low byte read as active bool
    int16_t  m_field24;          // +0x24  (ctor 0)
    uint8_t  m_field26;          // +0x26  (ctor 0)
    uint8_t  _pad27;             // +0x27
    CryStringT<char> m_attachName;   // +0x28  matched in the action-map event handler
    int32_t  m_segments;         // +0x30  init 20 (trajectory sample count; inferred)
    uint32_t m_color;            // +0x34  init 0xFF000000 (ARGB; inferred)
    bool     m_bool38;           // +0x38  init 1
    bool     m_bool39;           // +0x39  init 1
    uint8_t  _pad3A[6];          // +0x3A
};
static_assert(sizeof(C_MissileWeaponPlayerController) == 0x40, "C_MissileWeaponPlayerController must be 0x40");

}  // namespace wh::entitymodule
