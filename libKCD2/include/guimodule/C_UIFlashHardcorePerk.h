#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashHardcorePerk -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). make_shared 64 in builder
// sub_181F8DBC0 (`sub_1804F75C0(64)`; ctor 0x181F5B8C0). FillUIArgs override
// 0x181F5BFB0 (3 strings). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashHardcorePerk : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashHardcorePerk;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181F5BFB0

    CryStringT<char> m_name;          // +0x18
    CryStringT<char> m_description;   // +0x20
    CryStringT<char> m_icon;          // +0x28
};
static_assert(sizeof(C_UIFlashHardcorePerk) == 0x30, "C_UIFlashHardcorePerk must be 0x30 (make_shared 64)");

}  // namespace wh::guimodule
