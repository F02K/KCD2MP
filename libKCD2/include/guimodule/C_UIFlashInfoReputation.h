#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoReputation -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_180783564
// N=72 (ctor 0x180783748). FillUIArgs override 0x1819ACBE0. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoReputation : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoReputation;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819ACBE0

    CryStringT<char> m_name;     // +0x18
    float            m_value;    // +0x20
    int32_t          m_level;    // +0x24
    bool             m_flag;     // +0x28
    uint8_t          _pad29[7];  // +0x29
    CryStringT<char> m_icon;     // +0x30
};
static_assert(sizeof(C_UIFlashInfoReputation) == 0x38, "C_UIFlashInfoReputation must be 0x38 (creator sub_180783564)");

}  // namespace wh::guimodule
