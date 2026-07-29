#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashListStatistics.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListStatisticsGroup -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_18061F9AC
// N=80 (ctor 0x18061F858; name = "group"+id, vector filled by an I_Counter lambda).
// FillUIArgs [1] 0x1815AB9A8 (shared base+str+int body); the stats vector is serialized
// by the _vf2 [2] override. Element type inferred from the sibling record.

namespace wh::guimodule {

class C_UIFlashListStatisticsGroup : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListStatisticsGroup;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1815AB9A8 (shared body)
    void _vf2(void* pArgs) const override;         // [2] serializes m_stats

    CryStringT<char> m_groupName;   // +0x18  "group"+id
    int32_t          m_int20;       // +0x20
    uint8_t          _pad24[4];     // +0x24
    std::vector<std::shared_ptr<C_UIFlashListStatistics>> m_stats;   // +0x28  (0x18) [element type INFERRED]
};
static_assert(sizeof(C_UIFlashListStatisticsGroup) == 0x40, "C_UIFlashListStatisticsGroup must be 0x40 (creator sub_18061F9AC)");

}  // namespace wh::guimodule
