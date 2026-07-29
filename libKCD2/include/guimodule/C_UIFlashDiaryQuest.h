#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashDiaryQuest -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_180DC7544
// N=96 (ctor 0x180DC70D4). FillUIArgs override 0x181979294 (serializes the name and
// both objective string lists inline). Names descriptive (roles from the two lists'
// usage; UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashDiaryQuest : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashDiaryQuest;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181979294

    CryStringT<char> m_name;                              // +0x18
    std::vector<CryStringT<char>> m_activeObjectives;     // +0x20  (0x18) stride 8
    std::vector<CryStringT<char>> m_completedObjectives;  // +0x38  (0x18)
};
static_assert(sizeof(C_UIFlashDiaryQuest) == 0x50, "C_UIFlashDiaryQuest must be 0x50 (creator sub_180DC7544)");

}  // namespace wh::guimodule
