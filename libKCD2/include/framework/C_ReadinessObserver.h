#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"    // platform.h chain (int64/ILINE) for TimeValue.h
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/TimeValue.h"

// -----------------------------------------------
// wh::C_ReadinessObserver -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// Named-event observer on the readiness system (vtable "wh::C_ReadinessObserver"; namespace wh::
// per the RTTI, not wh::framework). ctor sub_180B3FF14(this, pReadinessSystem, eventName).
// Embedded in C_SaveGameManager @+0x198 watching "AfterGameLoad". The ctor writes vptr@+0x00,
// system ptr@+0x08, event name@+0x10, int@+0x18, CTimeValue(-100000 = "never")@+0x20,
// qword@+0x60, byte@+0x68; the +0x28..+0x5F region is ctor-UNWRITTEN (late-init observer state;
// untiled -- flagged, not fabricated).

namespace wh {

class I_ReadinessService;   // RTTI .?AVI_ReadinessService@wh@@

class C_ReadinessObserver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReadinessObserver;
    virtual ~C_ReadinessObserver() = default;   // +0x00  vptr (slot order UNVERIFIED)
    I_ReadinessService* m_pReadinessSystem;  // +0x08  readiness service (ctor stores env->framework->GetReadinessService()); dtor unregisters via vslot3
    CryStringT<char> m_eventName;  // +0x10  observed event (e.g. "AfterGameLoad")
    int32_t  m_int18;              // +0x18  (ctor 0; role unresolved)
    uint32_t _pad1C;               // +0x1C
    CTimeValue m_time;             // +0x20  init -100000 ("never"; last-signal time -- INFERRED)
    std::function<void()> m_callback;  // +0x28  (0x40) deferred nullary callback; MSVC impl ptr @+0x60 (0 = empty), storage +0x28..+0x5F
    uint8_t  m_flag68;             // +0x68  (ctor 0; role unresolved)
    uint8_t  _pad69[7];            // +0x69
};
static_assert(sizeof(C_ReadinessObserver) == 0x70, "C_ReadinessObserver must be 0x70");
static_assert(offsetof(C_ReadinessObserver, m_time) == 0x20, "time at 0x20");

}  // namespace wh
