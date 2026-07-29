#pragma once
#include <cstdint>
#include "guimodule/I_Cutscene.h"

// -----------------------------------------------
// wh::guimodule::C_SkipTimeCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB0648; vtable 0x183B2DF38. Simple I_Cutscene leaf (the wait/skip-time
// transition); creator sub_1811CF5D8 (alloc 0x68 @0x1811cf605). Own slots: [0]
// 0x182B35FCC, [1] 0x182B385D0, [2] 0x182B376A4, [3] 0x182B37E5C, [4] 0x181A8CA70,
// [5] 0x180838AE0, [6] 0x181A78EA0, [7] 0x182B377E4.

namespace wh::guimodule {

class C_SkipTimeCutscene : public I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkipTimeCutscene;
    void Play() override;            // [1] 0x182B385D0
    void Stop() override;            // [2] 0x182B376A4
    void Update() override;          // [3] 0x182B37E5C
    void Reset() override;           // [4] 0x181A8CA70
    void _vf5() override;            // [5] 0x180838AE0
    E_CutsceneType::Type GetType() const override;   // [6] 0x181A78EA0  SkipTime(4)
    bool IsActive() const override;  // [7] 0x182B377E4

    int32_t  m_skipTimeParam60;  // +0x60  Reset 0x181A8CA70 sets -1 (idle sentinel); Play 0x182B385D0 passes as int arg to skip-time query sub_182ECFC6C
    uint32_t m_skipTimeParam64;  // +0x64  Reset sets 0; Play passes as 2nd arg to sub_182ECFC6C
};
static_assert(sizeof(C_SkipTimeCutscene) == 0x68, "C_SkipTimeCutscene must be 0x68 (creator sub_1811CF5D8)");

}  // namespace wh::guimodule
