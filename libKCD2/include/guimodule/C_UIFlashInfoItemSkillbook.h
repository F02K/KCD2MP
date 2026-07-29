#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemDocument.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemSkillbook -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70, 8 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C8D0 N=128 (ctor
// 0x182B71224). FillUIArgs override 0x182B719EC (re-serializes the Document layout +
// the skill block). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemSkillbook : public C_UIFlashInfoItemDocument {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemSkillbook;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B719EC

    float            m_f58;     // +0x58
    float            m_f5C;     // +0x5C
    CryStringT<char> m_skill;   // +0x60
    int32_t          m_int68;   // +0x68
    int32_t          m_int6C;   // +0x6C
};
static_assert(sizeof(C_UIFlashInfoItemSkillbook) == 0x70, "C_UIFlashInfoItemSkillbook must be 0x70 (creator sub_182B0C8D0)");

}  // namespace wh::guimodule
