#pragma once
#include <cstdint>
#include <cstddef>

struct ICVar;

// -----------------------------------------------
// wh::dialogmodule::C_PlayerDialogController -- the player's side of a conversation
// (KCD2 1.5.6, kd7u).  sizeof 0x178 (operator new(376) in C_DialogInstance::Init).
// -----------------------------------------------
// RTTI TD 0x184B53508; vtable 0x183E696B0 (1 slot: deleting dtor); ctor
// sub_181E6E870(this, scene, handle); dtor sub_180C06F28 -> sub_180C07688. Created by
// C_DialogInstance::Init when the player participates (stored at instance +0x2E8; freed
// FIRST in the instance dtor). Subscribes two delegates to scene+200/+216. Saves and
// restores two render CVars across the conversation (r_Sharpening @+0xF0,
// e_svoTI_RsmConeMaxLengthFOVMulEnabled @+0x100).
// CHEAT: +0x140..+0x168 hold the player choice/decision vectors (16-byte-stride
// records) -- the place to force/unlock available responses. WHICH vector is the
// visible list is UNVERIFIED, as is the exact container split (modeled opaque).

namespace wh::dialogmodule {

class C_PlayerDialogController {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerDialogController;
    virtual ~C_PlayerDialogController();   // [0] (only vtable slot)

    void*    m_pScene;                // +0x08  ctor a2; 232B non-poly dialog scene runtime (== C_DialogInstance::m_pScene @+0x158); flags dword @+188, C_Signal delegate lists @+200/+216
    uint64_t _q10;                    // +0x10  [not walked]
    uint8_t  m_inputMap[0x40];        // +0x18..+0x57  input/decision map obj A [~64B, layout UNVERIFIED]
    uint8_t  m_decisionMap[0x40];     // +0x58..+0x97  input/decision map obj B [~64B, layout UNVERIFIED]
    float    m_phaseTimerA;           // +0x98  dt accumulator, 20.0s timeout (sub_180C0752C)
    float    m_phaseTimerB;           // +0x9C  dt accumulator, 20.0s timeout (sub_180C074F0)
    float    m_phaseTimerC;           // +0xA0  dt accumulator, 20.0s timeout (sub_180C072C0 case5)
    float    m_phaseTimerD;           // +0xA4  dt accumulator, 20.0s timeout (sub_180C072C0 case8)
    float    m_phaseTimerE;           // +0xA8  dt accumulator, 20.0s timeout (sub_180C072C0 case5)
    bool     m_flagAC;                // +0xAC  ctor 0; set 1 on timeout/abort (sub_180C072C0)
    bool     m_needsCleanup;          // +0xAD  dtor gate -> sub_180C88FE4; set 1 by sub_180C06238
    bool     m_flagAE;                // +0xAE  sub_180C074F0 result; checked sub_180C072C0 case6
    uint8_t  _padAF;                  // +0xAF  alignment
    void*    m_pRegistrar;            // +0xB0  &unk_185579380 (static registration record)
    uint8_t  _unkB8[0x38];            // +0xB8..+0xEF  [not walked]
    ICVar*   m_pCVarSharpening;       // +0xF0  "r_Sharpening" (saved/restored)
    uint64_t _qF8;                    // +0xF8  [not walked]
    ICVar*   m_pCVarSvoTI;            // +0x100 "e_svoTI_RsmConeMaxLengthFOVMulEnabled"
    int32_t  m_obstacleHandle;        // +0x108 nav-obstacle registration id, -1=none (set sub_180C87D44; release sub_180C88020->sub_180D326A4)
    uint32_t m_obstacleActor;         // +0x10C actor EntityId (sub_181977E58 record base)
    Vec3     m_obstacleDir;           // +0x110 obstacle direction/forward (sub_181977E58)
    Vec3     m_obstaclePos;           // +0x11C obstacle world position (sub_181977E58)
    float    m_obstacleRadius;        // +0x128 obstacle radius (max 0.01); also sub_180C072C0 case4 <=0 gate / case6 reset
    float    m_obstacleIgnoreRadius;  // +0x12C ignore-radius (sub_181977E58 record+0x20)
    void*    m_pScene130;             // +0x130 scene ptr; head of embedded decision/response mgr (owns vecs @+0x140/+0x158)
    C_PlayerDialogController* m_pSelf; // +0x138 back-ref to this; second word of decision-mgr head
    uint8_t  m_decisionVecs[0x28];    // +0x140..+0x167  player choice/decision vectors
                                      //   (16B-stride records; container split UNVERIFIED)  <-- CHEAT
    uint64_t _q168;                   // +0x168  [not walked]
    uint64_t _q170;                   // +0x170  [not walked]
};
static_assert(sizeof(C_PlayerDialogController) == 0x178, "C_PlayerDialogController must be 0x178 (alloc 376)");
static_assert(offsetof(C_PlayerDialogController, m_pCVarSharpening) == 0xF0);
static_assert(offsetof(C_PlayerDialogController, m_decisionVecs) == 0x140);

}  // namespace wh::dialogmodule
