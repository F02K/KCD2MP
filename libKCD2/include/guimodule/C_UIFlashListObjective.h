#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListObjective -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_18182DDEC
// N=64 (ctor 0x18182DEC4). FillUIArgs override 0x1819B66DC. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListObjective : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListObjective;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819B66DC

    int32_t          m_int18;    // +0x18
    uint8_t          _pad1C[4];  // +0x1C
    CryStringT<char> m_text;     // +0x20
    bool             m_bool28;   // +0x28
    bool             m_bool29;   // +0x29
    uint8_t          _pad2A[6];  // +0x2A
};
static_assert(sizeof(C_UIFlashListObjective) == 0x30, "C_UIFlashListObjective must be 0x30 (creator sub_18182DDEC)");

}  // namespace wh::guimodule
