#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudAdaptiveGroup -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI .?AVC_UIHudAdaptiveGroup@guimodule@wh@@ (TD 0x184CD0BA0).
// Bases: C_UIFlashBase @+0x00 (COL 0x4376900), IUIElementEventListener @+0x10
// (COL 0x43768B0). vtables: primary 0x183EEB910 (14 slots; base Init/Deinit; own
// [4] Update 0x181F7B530; base RTTR trio -- NOT separately rttr-registered),
// listener 0x183EEB870 (8). ctor sub_181F77680.
//
// One "adaptive HUD" fade group: a set of flash widget handles whose alpha is faded
// in/out together by C_UIHudAdaptive (idle-HUD auto-hide). NOT a module screen --
// exactly two instances live EMBEDDED in C_UIHudAdaptive (+0x30 / +0xE0); a
// _Ref_count<C_UIHudAdaptiveGroup> TD exists but no shared_ptr factory site was found
// (no constructor_wrapper TD either).
// Update (0x181F7B530): flash-queue drain (sub_18053378C), then if m_pendingAlpha
// != -1 and the fade timer (m_elapsed/m_duration) or alpha pair is unsettled, steps
// m_currentAlpha toward the target and pushes it to every widget handle in m_widgets
// (per-handle setter sub_181F7A2B0).
// Field semantics beyond the fade step are partially UNVERIFIED (ctor float args are
// register-lost at both embed sites).

namespace wh::guimodule {

class C_UIHudAdaptiveGroup : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudAdaptiveGroup;
    void Update() override;                   // [4] 0x181F7B530  fade step (doc above)

    float    m_param58;                       // +0x58  ctor helper sub_180A70610 a3 [role UNVERIFIED]
    float    m_param5C;                       // +0x5C  helper a4 [role UNVERIFIED]
    int32_t  m_unk60;                         // +0x60  untouched by ctor(sub_181F77680)/helper(sub_180A70610)/Update(0x181F7B530) [role UNVERIFIED]
    int32_t  m_state64;                       // +0x64  ctor 0 [role UNVERIFIED]
    float    m_delay68;                       // +0x68  helper a2 [role UNVERIFIED]
    float    m_fadeIn6C;                      // +0x6C  helper a5 [role UNVERIFIED]
    float    m_fadeOut70;                     // +0x70  helper a6 [role UNVERIFIED]
    float    m_duration;                      // +0x74  = m_delay68 + m_fadeIn6C + m_fadeOut70 (ctor helper @0x180a7064f)
    float    m_elapsed;                       // +0x78  ctor 0; Update compares vs m_duration
    int32_t  m_unk7C;                         // +0x7C  zeroed as qword pair with m_elapsed (sub_180A70610 @0x180a70648); Update 0x181F7B530 ignores it [role UNVERIFIED]
    int32_t  m_unk80;                         // +0x80  untouched by ctor(sub_181F77680)/helper(sub_180A70610)/Update(0x181F7B530) [role UNVERIFIED]
    int32_t  m_stepAlpha;                     // +0x84  helper 0; Update copies it into m_currentAlpha [role UNVERIFIED]
    std::vector<uint64_t> m_widgets;          // +0x88  flash widget handles faded together (Update walk @0x181f7b5c9)
    int32_t  m_currentAlpha;                  // +0xA0  ctor 100.0f bit-pattern; Update writes ints [dual use, semantics UNVERIFIED]
    int32_t  m_targetAlpha;                   // +0xA4  ctor -1.0f bit-pattern [semantics UNVERIFIED]
    int32_t  m_pendingAlpha;                  // +0xA8  -1 = idle (Update gate @0x181f7b573)
    uint8_t  _padAC[4];                       // +0xAC
};
static_assert(sizeof(C_UIHudAdaptiveGroup) == 0xB0, "C_UIHudAdaptiveGroup must be 0xB0");
static_assert(offsetof(C_UIHudAdaptiveGroup, m_widgets) == 0x88, "widget vector at 0x88");

}  // namespace wh::guimodule
