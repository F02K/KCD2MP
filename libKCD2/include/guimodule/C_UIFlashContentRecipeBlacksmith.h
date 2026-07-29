#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashContentRecipeBlacksmith -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_18119F710
// N=112 (ctor 0x18119F7E8). FillUIArgs override 0x18172E580. Both vector element
// types UNVERIFIED (flash_leaves.md §5.3). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashContentRecipeBlacksmith : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashContentRecipeBlacksmith;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x18172E580

    CryStringT<char> m_name;         // +0x18
    CryStringT<char> m_str20;        // +0x20
    std::vector<uint64_t> m_vec28;   // +0x28  (0x18) element type UNVERIFIED
    float            m_weight;       // +0x40
    int32_t          m_int44;        // +0x44
    std::vector<uint64_t> m_vec48;   // +0x48  (0x18) element type UNVERIFIED
};
static_assert(sizeof(C_UIFlashContentRecipeBlacksmith) == 0x60, "C_UIFlashContentRecipeBlacksmith must be 0x60 (creator sub_18119F710)");

}  // namespace wh::guimodule
