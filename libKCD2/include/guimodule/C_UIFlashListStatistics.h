#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListStatistics -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_180EFA404
// N=56 (ctor 0x180EFA4CC). FillUIArgs override 0x180555884 (decompiled with a _DWORD*
// `this`; offsets hand-corrected -- flash_leaves.md §5.8). Element of
// C_UIFlashListStatisticsGroup::m_stats. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListStatistics : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListStatistics;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180555884

    float   m_val0;    // +0x18
    float   m_val1;    // +0x1C
    int32_t m_int20;   // +0x20
    int32_t m_int24;   // +0x24
};
static_assert(sizeof(C_UIFlashListStatistics) == 0x28, "C_UIFlashListStatistics must be 0x28 (creator sub_180EFA404)");

}  // namespace wh::guimodule
