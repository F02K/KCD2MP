#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoPerk -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_182AFB7A0
// N=136 (ctor 0x182B71408). FillUIArgs override 0x182B71A50 (scalars + two string
// lists inline). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoPerk : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoPerk;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B71A50

    CryStringT<char> m_name;                  // +0x18
    int32_t          m_int20;                 // +0x20
    uint8_t          _pad24[4];               // +0x24
    CryStringT<char> m_str28;                 // +0x28
    CryStringT<char> m_str30;                 // +0x30
    int32_t          m_int38;                 // +0x38
    uint8_t          _pad3C[4];               // +0x3C
    CryStringT<char> m_str40;                 // +0x40
    std::vector<CryStringT<char>> m_list0;    // +0x48  (0x18) stride 8
    std::vector<CryStringT<char>> m_list1;    // +0x60  (0x18)
};
static_assert(sizeof(C_UIFlashInfoPerk) == 0x78, "C_UIFlashInfoPerk must be 0x78 (creator sub_182AFB7A0)");

}  // namespace wh::guimodule
