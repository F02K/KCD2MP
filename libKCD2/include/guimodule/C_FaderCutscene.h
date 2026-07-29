#pragma once
#include <cstdint>
#include "guimodule/I_Cutscene.h"

// -----------------------------------------------
// wh::guimodule::C_FaderCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB0530; vtable 0x183B2E170. Simple I_Cutscene leaf (a pure screen-fade
// "cutscene" -- no members beyond the base); creator sub_1811CFC58 (alloc 0x60
// @0x1811cfc85). Own slots: [1]=[2] 0x182B37670, [3] 0x182B37C9C, [4] nullsub,
// [5]=[6] 0x180838AE0, [7] 0x18041A6A0 ([0] shares the base dtor).

namespace wh::guimodule {

class C_FaderCutscene : public I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FaderCutscene;
    void Play() override;            // [1] 0x182B37670 (shared with Stop)
    void Stop() override;            // [2] 0x182B37670
    void Update() override;          // [3] 0x182B37C9C
    void Reset() override;           // [4] nullsub
    void _vf5() override;            // [5] 0x180838AE0
    E_CutsceneType::Type GetType() const override;   // [6] 0x180838AE0  Fader(0)
    bool IsActive() const override;  // [7] 0x18041A6A0
};
static_assert(sizeof(C_FaderCutscene) == 0x60, "C_FaderCutscene must be 0x60 (creator sub_1811CFC58; == I_Cutscene)");

}  // namespace wh::guimodule
