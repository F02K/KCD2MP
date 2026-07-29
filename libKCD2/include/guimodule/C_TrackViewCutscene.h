#pragma once
#include <cstdint>
#include "guimodule/C_TrackViewBase.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_TrackViewCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD0 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CAF8D8; primary vtable 0x183B2E100. Base: C_TrackViewBase. The generic
// trackview-sequence cutscene (also what C_PlayTrackView instantiates and runs).
// ctor sub_1811CFBCC; creator sub_1811CFB40 (alloc 0xD0 @0x1811cfb66).
// Own slots: [0] 0x182B36258, [1] Play 0x182B3878C (AddMovieListener + play sequence +
// compute duration + fire signal), [2] 0x182B3772C, [3] Update 0x182B37F18 (tick +
// camera precache), [4] Reset 0x182B380B4, [5] 0x180838AE0, [6] GetType 0x181A78ED0
// (`return 5`), [7] IsActive 0x182B377FC -- these bodies are the reference I_Cutscene
// contract evidence.

namespace wh::guimodule {

class C_TrackViewCutscene : public C_TrackViewBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TrackViewCutscene;
    void Play() override;            // [1] 0x182B3878C
    void Update() override;          // [3] 0x182B37F18
    void Reset() override;           // [4] 0x182B380B4
    void _vf5() override;            // [5] 0x180838AE0
    E_CutsceneType::Type GetType() const override;   // [6] 0x181A78ED0  TrackView(5)
    bool IsActive() const override;  // [7] 0x182B377FC

    wh::shared::C_Signal<> m_signalC0;   // +0xC0  delegate table unk_185665430 [signature UNVERIFIED]
};
static_assert(sizeof(C_TrackViewCutscene) == 0xD0, "C_TrackViewCutscene must be 0xD0 (creator sub_1811CFB40)");

}  // namespace wh::guimodule
