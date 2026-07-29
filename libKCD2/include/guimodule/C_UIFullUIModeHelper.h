#pragma once
#include <cstdint>
#include "framework/I_SourceMonitorListener.h"
#include "framework/CryDeferrable.h"

// -----------------------------------------------
// wh::guimodule::C_UIFullUIModeHelper -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD0.
// -----------------------------------------------
// RTTI .?AVC_UIFullUIModeHelper@guimodule@wh@@ (TD 0x184C9ED30, COL 0x43530E8).
// vtable 0x183EDE9B0 (2 slots: [0] listener callback, [1] the class's OWN virtual
// deleting dtor sub_182B114BC -- I_SourceMonitorListener itself has no dtor slot).
// ctor sub_18194AD80; factory sub_18194AD0C (alloc 0xD0) called from C_GUIModule::Init,
// owned at C_GUIModule+0x78, destroyed by module Deinit via vf[+0x08](1).
// Bases: I_SourceMonitorListener @+0x00 (primary), UnsafeOp::CryDeferrable<0> @+0x08
// (EMPTY base sharing its address with m_pCVarFog -- see CryDeferrable.h).
//
// "Full UI mode" = a fullscreen-menu graphics downgrade/override helper. The ctor
// caches 12 ICVar* (pConsole global qword_18492D8A8, GetCVar vf[+0xB8]) and preloads
// the "audio_setup_inventory" audio setup (qword_18492D8B8 vf[+0x38] -> +0xB8); it
// registers itself with the framework source monitor (name global qword_18557E988,
// type 6). The OnSourceEvent override (sub_181F52710) snapshots two values from
// gamectx+0x1A8 (+0x48/+0x50 -> m_savedA0/m_saved98) on enter and resets both to the
// 0x8000000000000000 sentinel on leave. NEW vs KCD1 (no TD there).

namespace wh::guimodule {

class C_UIFullUIModeHelper
    : public wh::framework::I_SourceMonitorListener
    , public UnsafeOp::CryDeferrable<0>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFullUIModeHelper;
    void OnSourceEvent(void* a2, bool bActive) override;   // [0] 0x181F52710 (banner)
    virtual ~C_UIFullUIModeHelper();                        // [1] deleting dtor 0x182B114BC (class-introduced)

    // Cached console variables (ctor, in store order):
    ICVar* m_pCVarFog;                   // +0x08  "e_Fog"
    ICVar* m_pCVarEyeAdaptationSpeed;    // +0x10  "r_HDREyeAdaptationSpeed"
    ICVar* m_pCVarEyeAdaptationEVMin;    // +0x18  "wh_e_HDREyeAdaptationEVMin"
    ICVar* m_pCVarAntialiasingMode;      // +0x20  "r_AntialiasingMode"
    ICVar* m_pCVarSvoTIApply;            // +0x28  "e_svoTI_Apply"
    ICVar* m_pCVarShadowsUpdateViewDistRatio; // +0x30  "e_ShadowsUpdateViewDistRatio"
    ICVar* m_pCVarColorGrading;          // +0x38  "r_ColorGrading"
    ICVar* m_pCVarSRReactiveMaskCutoff;  // +0x40  "r_SuperResolution_ReactiveMask_CutoffThreshold"
    ICVar* m_pCVarSRReactiveMaskBinary;  // +0x48  "r_SuperResolution_ReactiveMask_BinaryValue"
    ICVar* m_pCVarSRFSRTransparencyMask; // +0x50  "r_SuperResolution_AMD_FSR_TransparencyAndCompositionMask"
    ICVar* m_pCVarRainIntensityOverride; // +0x58  "wh_env_RainIntensityOverride"
    ICVar* m_pCVarViewDistRatio;         // +0x60  "e_ViewDistRatio"
    uint64_t m_saved68[6];               // +0x68  ctor-untouched; saved CVar values while active (roles UNVERIFIED)
    uint64_t m_saved98;                  // +0x98  ctor 0x8000000000000000 sentinel; gamectx+0x1A8 +0x50 snapshot
    uint64_t m_savedA0;                  // +0xA0  ctor 0x8000000000000000 sentinel; gamectx+0x1A8 +0x48 snapshot
    uint64_t m_A8;                       // +0xA8  ctor 0 (UNVERIFIED)
    bool     m_activeB0;                 // +0xB0  ctor 0 (UNVERIFIED)
    uint8_t  _padB1[7];                  // +0xB1
    uint64_t m_audioSetupInventory;      // +0xB8  "audio_setup_inventory" handle (audio system vf[+0x38] out-param)
    uint64_t m_C0;                       // +0xC0  ctor 0 (UNVERIFIED)
    float    m_C8;                       // +0xC8  ctor 1.0f (UNVERIFIED)
    uint32_t m_CC;                       // +0xCC  ctor 0 (UNVERIFIED)
};
static_assert(sizeof(C_UIFullUIModeHelper) == 0xD0, "C_UIFullUIModeHelper must be 0xD0");

}  // namespace wh::guimodule
