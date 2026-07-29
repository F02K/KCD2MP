#pragma once
#include <cstdint>
#include "guimodule/C_TrackViewBase.h"

// -----------------------------------------------
// wh::guimodule::C_IngameCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x140 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB0610; primary vtable 0x183B2E040. Base: C_TrackViewBase (sibling of
// C_TrackViewCutscene). The in-game (non-menu) trackview cutscene runner.
// ctor sub_1811CFA84 (chains the base ctor, then sub_1811CFF34 tail-init); creator
// sub_1811CF964 (alloc 0x140 @0x1811cf98a). Own slots: [0] 0x182B36188,
// [1] 0x182B383D8, [2] 0x181F58B90, [3] 0x182B37E44, [4] 0x182B380AC, [5] 0x182B38EDC,
// [6] 0x181A74A40, [7] 0x181A834D0.

namespace wh::guimodule {

class C_IngameCutscene : public C_TrackViewBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_IngameCutscene;
    void Play() override;            // [1] 0x182B383D8
    void Stop() override;            // [2] 0x181F58B90
    void Update() override;          // [3] 0x182B37E44
    void Reset() override;           // [4] 0x182B380AC
    void _vf5() override;            // [5] 0x182B38EDC
    E_CutsceneType::Type GetType() const override;   // [6] 0x181A74A40  Ingame(2)
    bool IsActive() const override;  // [7] 0x181A834D0

    // Sparse ctor-anchored state (bytes @+0xC0/+0xD0/+0xE0/+0xF0/+0x100, flags 0x0100
    // @+0x110, qwords @+0x118/+0x120/+0x130, bytes @+0x12C/+0x138 + sub_1811CFF34
    // tail-init) -- member semantics UNVERIFIED, kept opaque.
    uint8_t m_state[0x80];   // +0xC0..+0x140  [roles UNVERIFIED]
};
static_assert(sizeof(C_IngameCutscene) == 0x140, "C_IngameCutscene must be 0x140 (creator sub_1811CF964)");

}  // namespace wh::guimodule
