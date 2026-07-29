#pragma once
#include <cstdint>
#include "conceptmodule/C_RuntimeState.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "conceptmodule/S_Trigger.h"
#include "guimodule/I_CutsceneChangeListener.h"

// -----------------------------------------------
// wh::guimodule::C_CutsceneHandler -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x2E0 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184C9E118; COLs +0x00/0x413A928, +0x40/0x413A950; primary vtable 0x183B1C288
// (the 42-slot conceptmodule C_RuntimeState node table -- not modeled as C++ virtuals
// per module convention), I_CutsceneChangeListener @+0x40 vtable 0x183B1C1B0.
// ctor sub_18115229C (chains the C_Node ctor sub_1806B2744); as_wh_smart_ptr creator
// sub_181152200 (alloc 0x2E0 @0x18115222b); as_std_shared_ptr creator sub_182B0BBE4.
//
// Concept-graph node bridging cutscene lifecycle -> trigger outputs: OnCutsceneChanged
// (0x182B12028) fires the S_Trigger output ports per changeType (dispatch at
// handler+392/+520/+584 via port vf[+0x78]). Port roles coined from that dispatch;
// display-names UNVERIFIED (registrar not isolated).

namespace wh::entitymodule { class C_CutsceneHolder; }

namespace wh::guimodule {

class C_CutsceneHandler : public wh::conceptmodule::C_RuntimeState,   // +0x00 (0x40)
                          public I_CutsceneChangeListener {           // +0x40
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CutsceneHandler;
    void OnCutsceneChanged(I_Cutscene* pCutscene, int changeType) override;   // 0x182B12028

    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_port48;        // +0x48  in [role UNVERIFIED]
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_port88;        // +0x88  [role UNVERIFIED]
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_portC8;        // +0xC8  [role UNVERIFIED]
    wh::conceptmodule::C_TypedPortRef<wh::entitymodule::C_CutsceneHolder*> m_cutscenePort;  // +0x108
    wh::conceptmodule::C_TypedPortRef<bool> m_bool148;                                // +0x148 [role UNVERIFIED]
    wh::conceptmodule::C_TypedPortRef<bool> m_bool188;                                // +0x188 [role UNVERIFIED]
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_out1C8;        // +0x1C8 out [role UNVERIFIED]
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_outStarted;    // +0x208 out (Started? coined)
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_outStopped;    // +0x248 out (Stopped? coined)
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger> m_outSkipped;    // +0x288 out (Skipped? coined)
    int32_t  m_int2C8;              // +0x2C8  ctor 0 [role UNVERIFIED]
    uint8_t  m_byte2CC;             // +0x2CC  ctor 0 [role UNVERIFIED]
    uint8_t  _pad2CD[3];            // +0x2CD
    I_Cutscene* m_pTrackedCutscene; // +0x2D0  currently-tracked cutscene; OnCutsceneChanged 0x182B1203F returns early unless (this == changed cutscene); +0x2D8 = its _smart_ptr control
    volatile int32_t* m_pRefCount;  // +0x2D8  smart-ptr control [ctor 0]
};
static_assert(sizeof(C_CutsceneHandler) == 0x2E0, "C_CutsceneHandler must be 0x2E0 (creator sub_181152200)");
static_assert(offsetof(C_CutsceneHandler, m_port48) == 0x48, "first port at 0x48");
static_assert(offsetof(C_CutsceneHandler, m_outSkipped) == 0x288, "last output port at 0x288");

}  // namespace wh::guimodule
