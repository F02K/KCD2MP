#pragma once
#include <cstdint>
#include "conceptmodule/C_Effect.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "conceptmodule/S_Trigger.h"
#include "guimodule/C_TrackViewCutscene.h"
#include "CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::guimodule::C_PlayTrackView -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x128 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184C9E310; COL 0x413A9C8; primary vtable 0x183B1C3E0 (46-slot C_Effect node
// table -- not modeled). Direct C_Effect child. ctor sub_181151FDC; dtor sub_181151EBC;
// as_wh creator sub_182B0BD98 (alloc 0x128).
// Own slots vs C_Effect: [0] dtor 0x181151E78, [5]/[7] trio, [27] 0x182B13398,
// [43] TRIGGER 0x182B135FC, [44] UNTRIGGER 0x182B14120.
// Behavior: TRIGGER resolves m_cutscenePort, looks the trackview cutscene config up by
// name (sub_1809AE704), instantiates a C_TrackViewCutscene and plays it
// (sub_182B0F5D4 / sub_182B36DEC / sub_182B38358; logs "failed to initialize cutscene
// '%s'" / "cutscene '%s' is not a valid trackview cutscene"); UNTRIGGER stops it and
// releases m_pRunning/m_pConfig. Port display-names coined.

namespace wh::entitymodule { class C_CutsceneHolder; }

namespace wh::guimodule {

class C_PlayTrackView : public wh::conceptmodule::C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayTrackView;
    wh::conceptmodule::C_TypedPortRef<wh::entitymodule::C_CutsceneHolder*> m_cutscenePort;  // +0x88  (0x40)
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger>        m_triggerPort;   // +0xC8
    uint8_t m_byte108;    // +0x108  ctor 0 [role UNVERIFIED]
    uint8_t _pad109[7];   // +0x109
    _smart_ptr<C_TrackViewCutscene> m_pRunning;   // +0x110  running instance (built on trigger, released by dtor sub_18052C7A8)
    void* m_pConfig;                              // +0x118  _smart_ptr (with m_pRefCount@+0x120) to a refcounted POLYMORPHIC cutscene-DB config entry (has vtable; name CryString @+0x8); resolved by name via sub_1809AE704 from the CutscenePlayer global registry xmmword_185326660 on TRIGGER; config class UNVERIFIED
    volatile int32_t* m_pRefCount;                // +0x120  config smart-ptr control [role UNVERIFIED]
};
static_assert(sizeof(C_PlayTrackView) == 0x128, "C_PlayTrackView must be 0x128 (creator sub_182B0BD98)");
static_assert(offsetof(C_PlayTrackView, m_pRunning) == 0x110, "running instance at 0x110");

}  // namespace wh::guimodule
