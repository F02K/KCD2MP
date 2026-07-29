#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListCombosGroup -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_18132C6E0
// N=80 (ctor 0x18132C7BC). FillUIArgs override 0x18178A01C. Names descriptive
// (UNVERIFIED; m_combos element type unresolved -- likely shared_ptr<C_UIFlashListCombo>).

namespace wh::guimodule {

class C_UIFlashListCombosGroup : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListCombosGroup;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x18178A01C

    CryStringT<char> m_name;       // +0x18
    int32_t          m_int20;      // +0x20
    int32_t          m_int24;      // +0x24
    std::vector<uint64_t> m_combos;  // +0x28  (0x18) element type UNVERIFIED (flash_leaves.md §5.3)
};
static_assert(sizeof(C_UIFlashListCombosGroup) == 0x40, "C_UIFlashListCombosGroup must be 0x40 (creator sub_18132C6E0)");

}  // namespace wh::guimodule
