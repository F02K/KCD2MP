#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashCreditsPeople.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCreditsRole -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_182BA4320
// N=72 (ctor 0x182B61C08). FillUIArgs [1] 0x1816829DC (shared base+int body, serializes
// m_roleId); the people vector is serialized by the _vf2 [2] override.

namespace wh::guimodule {

class C_UIFlashCreditsRole : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCreditsRole;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1816829DC (shared base+int body)
    void _vf2(void* pArgs) const override;         // [2] serializes m_people

    int32_t m_roleId;    // +0x18
    uint8_t _pad1C[4];   // +0x1C
    std::vector<std::shared_ptr<C_UIFlashCreditsPeople>> m_people;   // +0x20  (0x18)
};
static_assert(sizeof(C_UIFlashCreditsRole) == 0x38, "C_UIFlashCreditsRole must be 0x38 (creator sub_182BA4320)");

}  // namespace wh::guimodule
